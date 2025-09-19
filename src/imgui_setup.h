#pragma once

#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_sdlrenderer.h"
#include "rasteriser/timer.h"
#include "types.h"
#include <functional>
#include <sstream>

extern const long long bufferSize;
extern float currentTime;
extern ERasterDemo selectedDemo;

struct ImguiData {
	std::function<void(void)> OnTakeScreenshot;
};

void ImguiPopulateDemos(ImguiData *data) {
	int selectedDemoIndex = -1;
	int index = static_cast<int>(selectedDemo);
	int size = static_cast<int>(ERasterDemo::Length);
	const char *currentDemoName = DemoStrings[index];

	if (ImGui::BeginCombo("Scene Selector", currentDemoName, 0)) {
		for (size_t i = 0; i < size; i++) {
			std::stringstream stringStreamName;
			const char *name = DemoStrings[i];
			stringStreamName << name;
			if (ImGui::Selectable(stringStreamName.str().c_str(), index == i)) {
				selectedDemoIndex = i;
			}
		}
		ImGui::EndCombo();
	}

	if (selectedDemoIndex != -1) {
		if (selectedDemoIndex != index) {
			selectedDemo = static_cast<ERasterDemo>(selectedDemoIndex);
			currentTime = 0.0f;
			Timer_Reset();
		}
	}
}

void ImguiFrame(ImguiData *data) {
	ImGui::Begin("Controls");
	ImGui::Text("Time = %f", currentTime);

	// if (selectedDemo == ERasterDemo::BpmSequenceDemo) {
	// 	if (ImGui::Button("Play Audio", {0, 0})) {
	// 		playAudio = !playAudio;
	// 	}
	// 	ImGui::SameLine();
	// 	if (ImGui::Button("Mute audio", {0, 0})) {
	// 		muteAudio = !muteAudio;
	// 	}
	// 	ImGui::SliderInt("Volume", &volumeLevel, 0, 100);
	// 	ImGui::Text("Audio File: %s", audioTrack.c_str());
	// 	ImGui::Text("Sequence File: %s", sequencePath.c_str());
	// 	ImGui::Text("Sequence Type: %s", sequenceType.c_str());
	// 	ImGui::Text("Sequence Beat Count: %d", beatCount);
	// 	ImGui::Text("Sequence BPM: %d", bpm);
	// 	ImGui::Text("Current Beat: %d", beatID + 1);
	// }

	ImguiPopulateDemos(data);

	ImGui::BeginGroup();
	if (ImGui::Button("Screenshot", {0, 0})) {
		if (data->OnTakeScreenshot) {
			data->OnTakeScreenshot();
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset Time", {0, 0})) {
		currentTime = 0.0f;
		Timer_Reset();
	}

	ImGui::EndGroup();
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void InitDearImgui(SDL_Window *window, SDL_Renderer *renderer) {
#ifdef DEBUG
	std::printf("DearImgui: starting init\n");
#endif

	ImGui::CreateContext();

	bool success = true;
	success &= ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	success &= ImGui_ImplSDLRenderer_Init(renderer);

#ifdef DEBUG
	if (!success) {
		std::printf("DearImgui: init failed\n");
	} else {
		std::printf("DearImgui: init complete\n");
	}
#endif
}

void ShutdownDearImgui() {
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}