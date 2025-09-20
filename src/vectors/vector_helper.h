#pragma once

#include "../types.h"

namespace MathFunctions {
static f32 lerp(
	f32 a,
	f32 b,
	f32 t
) {
	return (1.0f - t) * a + t * b;
}
} // namespace MathFunctions
