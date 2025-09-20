#ifndef RASTERISER_H
#define RASTERISER_H

#include "../types.h"
#include "vectors/Vector4.h"

void Ras_Init(f32);
void Ras_Close(void);
void Ras_Draw(
	ERasterDemo,
	f64
);
void Ras_Clear(void); // assumed black

void Ras_OnSequenceUpdate(
	i32,
	f64
);

i32 Ras_Copy(OUT FVector *);

#endif
