#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>

#ifdef _DEBUG
#include <stdio.h>
#endif

#include "rasteriser.h"

#include "timer.h"
#include "particle/Particle.h"
#include "vectors/Vector2.h"
#include "vectors/Vector4.h"
#include "particle/Generator.h"
#include "physics/solver.h"
#include "physics/physics_helpers.h"

extern const int bufferWidth;
extern long long bufferSize;
extern f32 currentTime;

#define DIMS (bufferWidth * bufferWidth)
#define SIZE (sizeof(FVector) * DIMS)

static f32 b = 0.0f;
static FVector *buffer;

void Ras_Init(f32 BPM) {
	buffer = (FVector *)calloc(DIMS, sizeof(FVector));
	b = bpm2ms(BPM);
}

void Ras_Close(void) {
	if (buffer) {
		free(buffer);
	}
}

// generates a fake low frame rate animation
float LowframeRateEffect(f32 n, f32 min) {
	const f32 sawT = SawtoothHz(currentTime, 1.f, min);
	const i32 rounded = (i32)(sawT * n);
	const f32 sawN = (f32)(rounded) / n;
	return sawN;
}

// a.k.a shadertoy "Hello World"
void cycleColour() {
	for (i32 x = 0; x < bufferWidth; ++x) {
		for (i32 y = 0; y < bufferWidth; ++y) {
			const FVector2 uv = FVector2{(f32)x / (f32)bufferWidth, (f32)y / (f32)bufferWidth};
			const f32 tX = currentTime + uv.XY[0];
			const f32 tY = currentTime + uv.XY[1] + 2.0f;
			const FVector col = FVector{0.5f + 0.5f * cosf(tX), 0.5f + 0.5f * cosf(tY), 4.0f, 1.0f};

			buffer[y * bufferWidth + x] = col;
		}
	}
}

void setPixel(i32 x, i32 y, const FVector &col) {
	i32 index = y * bufferWidth + x;
	if (index < 0 || index > DIMS) {
		return;
	}
	buffer[index] = col;
}

// https://en.wikipedia.org/wiki/Bresenham's_line_algorithm#Algorithm_for_integer_arithmetic
void bresenham(const IVector2 &start, const IVector2 &end, const FVector &col) {
	i32 x0 = start.XY[0];
	i32 y0 = start.XY[1];
	i32 x1 = end.XY[0];
	i32 y1 = end.XY[1];

	i32 dx = abs(x1 - x0);
	i32 sx = x0 < x1 ? 1 : -1;

	i32 dy = -abs(y1 - y0);
	i32 sy = y0 < y1 ? 1 : -1;

	i32 error = dx + dy;

	while (1) {
		setPixel(x0, y0, col);
		if (x0 == x1 && y0 == y1) {
			break;
		}

		i32 e2 = 2 * error;
		if (e2 >= dy) {
			if (x0 == x1) {
				break;
			}
			error = error + dy;
			x0 = x0 + sx;
		}

		if (e2 <= dx) {
			if (y0 == y1) {
				break;
			}
			error = error + dx;
			y0 = y0 + sy;
		}
	}
}

// https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void midpoint_circle_outline(const IVector2 &center, f32 radius, const FVector &col) {
	const i32 r = (i32)radius;
	i32 x = r - 1, y = 0;
	i32 e = 3 - 2 * r;

	while (x >= y) {
		setPixel(center.XY[0] + x, center.XY[1] + y, col);
		setPixel(center.XY[0] - x, center.XY[1] - y, col);
		setPixel(center.XY[0] + y, center.XY[1] + x, col);
		setPixel(center.XY[0] - y, center.XY[1] - x, col);
		setPixel(center.XY[0] + x, center.XY[1] - y, col);
		setPixel(center.XY[0] - x, center.XY[1] + y, col);
		setPixel(center.XY[0] + y, center.XY[1] - x, col);
		setPixel(center.XY[0] - y, center.XY[1] + x, col);

		if (e > 0) {
			e = e - (2 * x + 1);
			x--;
		} else {
			e = e + (2 * y + 1);
			y++;
		}
	}
}

void midpoint_circle_fill(const IVector2 &center, f32 radius, const FVector &col) {
	const i32 r = (i32)radius;
	i32 x = r - 1, y = 0;
	i32 e = 3 - 2 * r;

	while (x >= y) {
		bresenham(IVector2{center.XY[0] - x, center.XY[1] - y}, IVector2{center.XY[0] + x, center.XY[1] + y}, col);
		bresenham(IVector2{center.XY[0] - y, center.XY[1] - x}, IVector2{center.XY[0] + y, center.XY[1] + x}, col);
		bresenham(IVector2{center.XY[0] - y, center.XY[1] + x}, IVector2{center.XY[0] + y, center.XY[1] - x}, col);
		bresenham(IVector2{center.XY[0] + x, center.XY[1] - y}, IVector2{center.XY[0] - x, center.XY[1] + y}, col);

		if (e > 0) {
			e = e - (2 * x + 1);
			x--;
		} else {
			e = e + (2 * y + 1);
			y++;
		}
	}
}

f32 getRandomFloat(i32 max) {
	return (f32)(rand() % max) / (f32)max;
}

void Ras_OnSequenceUpdate(i32 param, f32 dt) {
	Ras_Draw(static_cast<ERasterDemo>(param), dt);
}

// https://www.geometrictools.com/Documentation/MinimumAreaRectangle.pdf
FVector2 SmallestRectangle(i32 j0, i32 j1, IVector2 *points, i32 count) {
	IVector2 point_diff = points[j1] - points[j0];
	FVector2 U[2] = {FVector2{(f32)point_diff.XY[0], (f32)point_diff.XY[1]}, FVector2{}};
	U[0] = U[0].Normalise();
	U[1] = FVector2{(f32)U[0].XY[0], (f32)U[0].XY[1]};

	i32 indices[4] = {j1, j1, j1, j1};
	FVector2 origin = FVector2{(f32)points[j1].XY[0], (f32)points[j1].XY[1]};
	FVector2 support[4] = {FVector2{0.0f, 0.0f}, FVector2{0.0f, 0.0f}, FVector2{0.0f, 0.0f}, FVector2{0.0f, 0.0f}};

	for (i32 i = 0; i < count; ++i) {
		FVector2 diff = origin - FVector2(points[i].XY[0], points[i].XY[1]);
		FVector2 v = FVector2{FVector2::Dot(U[0], diff), FVector2::Dot(U[1], diff)};

		if (v.XY[0] > support[1].XY[0] || (v.XY[0] == support[1].XY[0] && v.XY[1] > support[1].XY[1])) {
			indices[1] = i;
			support[1] = v;
		}

		if (v.XY[1] > support[2].XY[1] || (v.XY[1] == support[2].XY[1] && v.XY[0] < support[2].XY[0])) {
			indices[2] = i;
			support[2] = v;
		}

		if (v.XY[0] < support[3].XY[0] || (v.XY[0] == support[3].XY[0] && v.XY[1] < support[3].XY[1])) {
			indices[3] = i;
			support[3] = v;
		}
	}

	return FVector2{support[1].XY[0] - support[3].XY[0], support[2].XY[1]};
}

void RotatingCalipers() {
	const FVector originCol = FVector{1.0, 1, 0, 1};
	const FVector pointCol = FVector{1, 0, 0, 1};
	const FVector boundCol = FVector{0, 1, 1, 1};
	const FVector caliperBoundCol = FVector{0, 1, 0, 1};

	const i32 centre = bufferWidth / 2;
	const i32 count = 4;
	IVector2 points[count] = {IVector2{centre + 20, centre + 5}, IVector2{centre - 8, centre + 3}, IVector2{centre + 2, centre - 9}, IVector2{centre - 6, centre - 2}};

	// dynamic resizing
	{
		const f32 beatTimeRedLine = b * (f32)4.0f;
		const f32 cx = CosHz(currentTime, -beatTimeRedLine, 1.0f, 0.0f);
		const f32 sx = SinHz(currentTime, -beatTimeRedLine, 1.0f, 0.0f);

		const f32 r = 10.0f;
		points[0] = IVector2{(centre + 20) + (i32)(sx * r), (centre + 5) + (i32)(cx * r)};
		points[1] = IVector2{(centre - 8) - (i32)(sx * r), (centre + 3) - (i32)(cx * r)};
	}

	FVector2 origin = FVector2{0, 0};
	// approximate version
	{
		// approximate centre
		for (i32 i = 0; i < count; ++i) {
			origin = origin + points[i];
		}
		origin = origin / (f32)count;

		// approximate area
		FVector2 sumExtents = FVector2{0.f, 0.f};
		for (i32 i = 0; i < count; ++i) {
			const FVector2 distance = origin - FVector2(points[i].XY[0], points[i].XY[1]);
			sumExtents.XY[0] += fabsf(distance.XY[0]);
			sumExtents.XY[1] += fabsf(distance.XY[1]);
		}
		sumExtents = sumExtents * 0.5f;

		const IVector2 pa = IVector2{(i32)(origin.XY[0] - sumExtents.XY[0]), (i32)(origin.XY[1] + sumExtents.XY[0])};
		const IVector2 pb = IVector2{(i32)(origin.XY[0] + sumExtents.XY[0]), (i32)(origin.XY[1] + sumExtents.XY[0])};
		const IVector2 pc = IVector2{(i32)(origin.XY[0] - sumExtents.XY[0]), (i32)(origin.XY[1] - sumExtents.XY[0])};
		const IVector2 pd = IVector2{(i32)(origin.XY[0] + sumExtents.XY[0]), (i32)(origin.XY[1] - sumExtents.XY[0])};

		bresenham(pa, pb, boundCol);
		bresenham(pb, pd, boundCol);
		bresenham(pd, pc, boundCol);
		bresenham(pc, pa, boundCol);
	}

	// minimum area
	{
		const FVector2 extents = SmallestRectangle(1, 0, points, count);
		const IVector2 pa = IVector2{(i32)(origin.XY[0] - extents.XY[0]), (i32)(origin.XY[1] + extents.XY[0])};
		const IVector2 pb = IVector2{(i32)(origin.XY[0] + extents.XY[0]), (i32)(origin.XY[1] + extents.XY[0])};
		const IVector2 pc = IVector2{(i32)(origin.XY[0] - extents.XY[0]), (i32)(origin.XY[1] - extents.XY[0])};
		const IVector2 pd = IVector2{(i32)(origin.XY[0] + extents.XY[0]), (i32)(origin.XY[1] - extents.XY[0])};

		bresenham(pa, pb, caliperBoundCol);
		bresenham(pb, pd, caliperBoundCol);
		bresenham(pd, pc, caliperBoundCol);
		bresenham(pc, pa, caliperBoundCol);
	}

	// draw points
	for (i32 i = 0; i < count; ++i) {
		setPixel(points[i].XY[0], points[i].XY[1], pointCol);
	}
	setPixel(origin.XY[0], origin.XY[1], originCol);
}

const i32 centre = bufferWidth / 2;
const FVector2 bounds = FVector2{static_cast<f64>(bufferWidth - 1), static_cast<f64>(bufferWidth - 1)};

GeneratorDesc air_particle_generator_desc = GeneratorDesc{
	FVector{1, 1, 1, 1},
	FVector2{static_cast<f64>(centre) - 5.0, static_cast<f64>(centre)},
	1.0, 1.05, Physics::air_density, 1.0,
	false,  // TODO: is always true, until number of particles is reached
	1.0,	// TODO: lifetime ignored
	16,
	0.75
};

GeneratorDesc water_particle_generator_desc = GeneratorDesc{
	FVector{0, 0, 1, 1},
	FVector2{static_cast<f64>(centre), static_cast<f64>(centre)},
	1000.0, 1.05, Physics::water_density, 1.0,
	false,  // TODO: is always true, until number of particles is reached
	1.0,	// TODO: lifetime ignored
	5,
	0.5
};

Generator air_particle_generator = Generator(air_particle_generator_desc);
Generator water_particle_generator = Generator(water_particle_generator_desc);

extern solver sys_solver;
static constexpr f64 Timescale = 1.0;

void Forces(f64 dt) {
	const f64 time = dt * Timescale;
	sys_solver.advance(bounds, time);
	air_particle_generator.tick(time);
	water_particle_generator.tick(time);

	for (const auto& particle : air_particle_generator.Particles) {
		setPixel(static_cast<i32>(particle->VisualPosition.XY[0]), static_cast<i32>(particle->VisualPosition.XY[1]), air_particle_generator_desc.ParticleColour);
	}
	
	for (const auto& particle : water_particle_generator.Particles) {
		setPixel(static_cast<i32>(particle->VisualPosition.XY[0]), static_cast<i32>(particle->VisualPosition.XY[1]), water_particle_generator_desc.ParticleColour);
	}
}

void Ras_Draw(ERasterDemo Demo, f64 dt) {
	switch (Demo) {
	case ERasterDemo::RotatingCalipers:
		RotatingCalipers();
		break;
	case ERasterDemo::Physics:
		Forces(dt);
		break;
	}
}

void Ras_Clear(void) {
	std::memset(buffer, 0, SIZE);
}

// assumed input is the same as internal buffer
i32 Ras_Copy(OUT FVector *externalBuffer) {
	if (externalBuffer) {
		std::memcpy(externalBuffer, buffer, SIZE);
		return 0;
	}
	return -1;
}
