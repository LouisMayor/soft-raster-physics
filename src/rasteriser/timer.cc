#include "timer.h"
#include "../types.h"
#include <math.h>

#if _WIN32
#include <windows.h>
#endif

#if _WIN32
LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds, Frequency;
#endif

long long f2ll(float x) {
	float a;
	float b = modff(x, &a);
	float a_p = a * 1000000.0f;
	float b_p = b * 1000000.0f;
	return (long long)a_p + (long long)b_p;
}

void Timer_Init(void) {
#if _WIN32
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
#endif
}

void Timer_Reset(void) {
#if _WIN32
	QueryPerformanceCounter(&StartingTime);
#endif
}

long long Timer_Elapsed(void) {
#if _WIN32
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	return ElapsedMicroseconds.QuadPart;
#endif
	return -1;
}

void Timer_WaitFor(float x) {
	Timer_Init();
	long long Time = f2ll(x);
	long long Elapsed = 0ll;
	do {
		Elapsed = Timer_Elapsed();
	} while (Elapsed <= Time);
}

float bpm2ms(float bpm) {
	float min_in_ms = 60000.0;
	float beat_ms = (min_in_ms / bpm) / 1000.0;
	return beat_ms;
}

// https://en.wikipedia.org/wiki/Triangle_wave
float TriangleHz(float x, float a, float b, float c, float d) {
	float i = (4. * (a - 0.5)) / b;
	float phase = (c - 0.5) - (b / 4.);
	float j = fmod((x - phase), b);
	float k = b / 2.;
	return i * fabs(j - k) - d;
}

// https://en.wikipedia.org/wiki/Sawtooth_wave
// although, this implementation doesn't match. I can't recall where I got it from.
float SawtoothHz(float x, float a, float b) {
	float i = 2.f * a / PI;
	float j = atan(COT((PI * x) / b));
	return (a * .5f) + ((i * j) / 2.f);
}

// https://en.wikipedia.org/wiki/Sine_wave
float SinHz(float x, float b, float c, float d) {
	float p = T_PI / b;
	return sin(p * (x + c)) + d;
}

// same as above, example more Cos-y
float CosHz(float x, float b, float c, float d) {
	float p = T_PI / b;
	return cos(p * (x + c)) + d;
}