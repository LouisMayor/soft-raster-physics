#ifndef TYPE_H
#define TYPE_H

#include <cstdint>

#define PRAGMA_DISABLE_OPTIMIZATION __pragma(optimize("",off))
#define PRAGMA_ENABLE_OPTIMIZATION  __pragma(optimize("",on))

#define OUT
#define IN

#define EPSILON 0.00001
#define PI 3.1415926536
#define T_PI 6.2831853072
#define COT(x) cos(x) / sin(x)

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;

using f32 = float;
using f64 = double;

#define stringify(name) #name + '\0'

enum class ERasterDemo : u8 {
	RotatingCalipers,
	Physics,
	Length
};

static const char *DemoStrings[static_cast<int>(ERasterDemo::Length)] = {
	stringify(RotatingCalipers), stringify(Physics)
};

#endif