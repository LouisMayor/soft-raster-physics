#ifndef TIMER_H
#define TIMER_H

long long f2ll(float x);
void Timer_Init(void);
void Timer_Reset(void);
long long Timer_Elapsed(void);

void Timer_WaitFor(float x);

float bpm2ms(float bpm);
float SawtoothHz(float x, float a, float b);
float TriangleHz(float x, float a, float b, float c, float d);
float SinHz(float x, float b, float c, float d);
float CosHz(float x, float b, float c, float d);

#endif