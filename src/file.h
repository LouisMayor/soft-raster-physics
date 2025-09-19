#pragma once
#include <fstream>
#include <iostream>

#include "types.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include "vectors/Vector4.h"

#include <filesystem>
namespace fs = std::filesystem;

extern const int bufferWidth;

namespace File {
inline constexpr int RGB = 3;
inline constexpr int RGBA = 4;
inline constexpr int number_of_channels = RGB;

std::string Folder = "./";

void clear() {
	std::string build_folder = Folder;
	if (!fs::exists(fs::path(build_folder))) {
		fs::create_directory(build_folder);
	} else {
		for (auto &item : fs::directory_iterator(build_folder)) {
			fs::remove_all(item.path());
		}
	}
}

void write_to_file_ppm(std::string name, FVector *buffer) {
	name.append(".ppm");
	std::ofstream ofs(name.data(), std::ios::out | std::ios::binary);
	if (ofs.fail()) {
		std::cout << "failed to open file\n";
	}

	int width = bufferWidth;
	int height = bufferWidth;

	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; ++x) {
			int pos = y * width + x;
			FVector col = buffer[pos];
			unsigned char norm_r = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[0]) * 255);
			unsigned char norm_g = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[1]) * 255);
			unsigned char norm_b = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[2]) * 255);

			ofs << norm_r << norm_g << norm_b;
		}
	}
	ofs.close();
}

void write_to_file_jpg(std::string name, FVector *buffer) {
	name.append(".jpg");

	int width = bufferWidth;
	int height = bufferWidth;

	int total_pixel_count = height * width;
	unsigned char *pixels = new unsigned char[total_pixel_count * number_of_channels];
	int index = 0;

	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; ++x) {
			int pos = y * width + x;
			FVector col = buffer[pos];
			unsigned char norm_r = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[0]) * 255);
			unsigned char norm_g = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[1]) * 255);
			unsigned char norm_b = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[2]) * 255);

			pixels[index++] = norm_r;
			pixels[index++] = norm_g;
			pixels[index++] = norm_b;
		}
	}

	stbi_write_jpg(name.data(), width, height, number_of_channels, pixels, 100);
	delete[] pixels;
}

void write_to_file_png(std::string name, FVector *buffer) {
	name.append(".png");

	int width = bufferWidth;
	int height = bufferWidth;

	int total_pixel_count = height * width;
	unsigned char *pixels = new unsigned char[total_pixel_count * number_of_channels];
	int index = 0;

	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; ++x) {
			int pos = y * width + x;
			FVector col = buffer[pos];
			unsigned char norm_r = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[0]) * 255);
			unsigned char norm_g = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[1]) * 255);
			unsigned char norm_b = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[2]) * 255);

			pixels[index++] = norm_r;
			pixels[index++] = norm_g;
			pixels[index++] = norm_b;
		}
	}

	stbi_write_png(name.data(), width, height, number_of_channels, pixels, width * number_of_channels);
	delete[] pixels;
}

void write_to_file_bmp(std::string name, FVector *buffer) {
	name.append(".bmp");

	int width = bufferWidth;
	int height = bufferWidth;

	int total_pixel_count = height * width;
	unsigned char *pixels = new unsigned char[total_pixel_count * number_of_channels];
	int index = 0;

	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; ++x) {
			int pos = y * width + x;
			FVector col = buffer[pos];
			unsigned char norm_r = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[0]) * 255);
			unsigned char norm_g = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[1]) * 255);
			unsigned char norm_b = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[2]) * 255);
			pixels[index++] = norm_r;
			pixels[index++] = norm_g;
			pixels[index++] = norm_b;
		}
	}

	stbi_write_bmp(name.data(), width, height, number_of_channels, pixels);
	delete[] pixels;
}

void write_to_file_all_formats(std::string name, FVector *buffer) {
	clear();
	write_to_file_ppm(name, buffer);
	write_to_file_jpg(name, buffer);
	write_to_file_png(name, buffer);
	write_to_file_bmp(name, buffer);
}

// allocates, remember to delete
void to_byte_array(FVector *buffer, unsigned char *preallocated_buffer) {
	int width = bufferWidth;
	int height = bufferWidth;

	int total_pixel_count = height * width;
	int index = 0;

	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; ++x) {
			int pos = y * width + x;
			FVector col = buffer[pos];
			unsigned char norm_r = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[0]) * 255);
			unsigned char norm_g = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[1]) * 255);
			unsigned char norm_b = static_cast<unsigned char>(std::fmin(1.f, col.XYZW[2]) * 255);

			preallocated_buffer[index++] = norm_r;
			preallocated_buffer[index++] = norm_g;
			preallocated_buffer[index++] = norm_b;
		}
	}
}
} // namespace File
