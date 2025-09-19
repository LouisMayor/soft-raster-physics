#include <memory>
#include <SDL.h>
#include "imgui/imgui.h"

#include "file.h"
#include "imgui_setup.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_sdlrenderer.h"
#include "physics/marshal.h"
#include "physics/solver.h"
#include "rasteriser/rasteriser.h"
#include "rasteriser/timer.h"

struct SDLWindowDestroyer
{
	void operator()(SDL_Window* w) const
	{
		SDL_DestroyWindow(w);
	}
};

struct SDLRendererDestroyer
{
	void operator()(SDL_Renderer* r) const
	{
		SDL_DestroyRenderer(r);
	}
};

void InitSDL();
void ShutdownSDL();
void LoadImage(std::string path); // load from image
void LoadArray(); // load from memory

std::unique_ptr<SDL_Window, SDLWindowDestroyer> window, uiWindow;
std::unique_ptr<SDL_Renderer, SDLRendererDestroyer> renderer, uiRenderer;
std::unique_ptr<FVector[]> bufferCopy = std::make_unique<FVector[]>(bufferSize);
std::unique_ptr<unsigned char[]> imageArray = std::make_unique<unsigned char[]>(bufferSize * File::number_of_channels);

// PHYSICS SYSTEMS
//
marshal sys_marshal;
solver sys_solver;
//
// PHYSICS SYSTEMS

const int bufferWidth = 200;
const long long bufferSize = bufferWidth * bufferWidth;
float currentTime;
float previousTime;
float deltaTime;
ERasterDemo selectedDemo = ERasterDemo::Physics;

ImguiData imguiData;
const int windowWidth = 400;
const int windowHeight = 400;

int SDL_main(int argc, char* args[])
{
	InitSDL();
	InitDearImgui(uiWindow.get(), uiRenderer.get());
	Timer_Init();

	SDL_Event sdlEvent;
	bool quit = false;

	Ras_Init(128);

	while (!quit)
	{
		SDL_PollEvent(&sdlEvent);
		// special multi-window quit
		// https://stackoverflow.com/questions/17541127/sdl-2-0-quit-event-with-multiple-windows
		if (sdlEvent.type == SDL_WINDOWEVENT && sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			quit = true;
		}

		// retrieves mouse position
		i32 X,Y;
		SDL_GetMouseState(&X, &Y);
		
		previousTime = currentTime;
		currentTime = Timer_Elapsed() / 1000000.0f; // returns microseconds in integer form
		deltaTime = currentTime - previousTime;
		SDL_RenderClear(renderer.get());
		SDL_RenderClear(uiRenderer.get());

		Ras_Clear();
		Ras_Draw(selectedDemo, ImGui::GetIO().DeltaTime);
		Ras_Copy(bufferCopy.get());

		LoadArray();

		// make sure the UI window is the only one we respond to input from
		if (SDL_GetWindowID(uiWindow.get()) == sdlEvent.window.windowID)
		{
			ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		}
		else
		{
			// process input on render window
		}

		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImguiFrame(&imguiData);

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

		SDL_RenderPresent(renderer.get());
		SDL_RenderPresent(uiRenderer.get());
	}

	Ras_Close();
	ShutdownDearImgui();
	ShutdownSDL();
	return 0;
}

void LoadArray()
{
	File::to_byte_array(bufferCopy.get(), imageArray.get());
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(imageArray.get(), bufferWidth, bufferWidth,
	                                                File::number_of_channels * 8,
	                                                File::number_of_channels * bufferWidth, 0x000000ff,
	                                                0x0000ff00, 0x00ff0000, 0);
	SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
	SDL_RenderCopy(renderer.get(), image_texture, nullptr, nullptr);
	SDL_DestroyTexture(image_texture);
	SDL_FreeSurface(surface);
}

void LoadImage(std::string path)
{
	SDL_Surface* image_surface = SDL_LoadBMP(("./" + path + ".bmp").c_str());
	SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer.get(), image_surface);

	SDL_RenderCopy(renderer.get(), image_texture, nullptr, nullptr);
	SDL_DestroyTexture(image_texture);
	SDL_FreeSurface(image_surface);
}

std::unique_ptr<SDL_Window, SDLWindowDestroyer> CreateWindow(std::string Title, int x, int y, int wX, int wH)
{
	return std::unique_ptr<SDL_Window, SDLWindowDestroyer>(
		SDL_CreateWindow(Title.c_str(), x, y, wX, wH, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP));
}

void InitSDL()
{
#ifdef _DEBUG
	std::printf("SDL: starting init\n");
#endif

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
#ifdef _DEBUG
		std::printf("SDL: SDL_Init() failed. Reason - %s\n", SDL_GetError());
#endif
		return;
	}

	window = CreateWindow("Render", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight);

	if (window == nullptr)
	{
#ifdef _DEBUG
		std::printf("SDL: SDL_CreateWindow() failed. Reason - %s\n", SDL_GetError());
#endif
		return;
	}

	int x, y;
	SDL_GetWindowPosition(window.get(), &x, &y);
	uiWindow = CreateWindow("Controls", x + 410, y, windowWidth + (windowWidth * 0.25f), windowHeight);

	if (uiWindow == nullptr)
	{
#ifdef _DEBUG
		std::printf("SDL: SDL_CreateWindow() failed. Reason - %s\n", SDL_GetError());
#endif
		return;
	}

#ifdef _DEBUG
	std::printf("SDL: init complete\n");
#endif

	renderer = std::unique_ptr<SDL_Renderer, SDLRendererDestroyer>(SDL_CreateRenderer(window.get(), -1, 0));
	uiRenderer = std::unique_ptr<SDL_Renderer, SDLRendererDestroyer>(SDL_CreateRenderer(uiWindow.get(), -1, 0));
}

void ShutdownSDL()
{
#ifdef _DEBUG
	std::printf("SDL: shutting down\n");
#endif
	SDL_Quit();
}