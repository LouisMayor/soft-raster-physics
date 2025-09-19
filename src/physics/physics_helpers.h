#pragma once

#include "types.h"
#include "vectors/Vector2.h"

namespace Physics {
	static constexpr f64 gravity = 9.81;		// ms/2
	static constexpr f64 air_density = 1.1644;	// g/m3 @ 30c
	static constexpr f64 water_density = 997.0;	// g/m3

	/* https://www.grc.nasa.gov/www/k-12/rocket/drageq.html
	 * cd - drag coefficient
	 * a  - surface area of object
	 * p  - density of medium
	 * v  - velocity of object
	 */
	inline FVector2 drag(const f64 cd, const f64 a, const f64 p, const FVector2& v) {
	 	return cd * p * (v / 2.) * a;
	}

	/* turbine - https://www.youtube.com/watch?v=o-Y3N7sNL4k
	 * p - density of medium
	 * a - area of turbine
	 * w - power in watts
	 * d - direction
	 */
	inline FVector2 wind(const f64 p, const f64 a, const f64 w, const FVector2& d) {
		return d * (w / a / 0.5 * p);
	}

	/* terminal velocity - https://en.wikipedia.org/wiki/Free_fall
	 * cd - drag coefficient
	 * a  - surface area of object
	 * p  - density of medium
	 * m  - mass of object
	 *
	 * ignoring the sqrt
	 */
	inline FVector2 acceleration(const f64 m, const f64 cd, const f64 a, const f64 p) {
		return FVector2{0.0, (2. * m * -gravity) / (p * cd * a)};
	}

	/* https://en.wikipedia.org/wiki/Elastic_collision
	 *  u1 - velocity of particle 'a' BEFORE collision
	 *  u2 - velocity of particle 'b' BEFORE collision
	 *  m1 - mass of particle 'a'
	 *  m2 - mass of particle 'b'
	 *  v1 - velocity of particle 'a' AFTER collision
	 *  v2 - velocity of particle 'b' AFTER collision
	 */
	inline void elastic_collision(const FVector2& u1, const FVector2& u2, const f64 m1, const f64 m2, OUT FVector2& v1, OUT FVector2& v2) {
		// check masses aren't the same, otherwise solution becomes v1 = u2, v2 = u1
		if (m1 == m2) {
			v1 = u2;
			v2 = u1;
		}
		else {
			v1 = ((m1 - m2)  / (m1 + m2) * u1) + ((2.0 * m2) / (m1 + m2) * u2);
			v2 = ((2.0 * m1) / (m1 + m2) * u1) + ((m1 - m2)  / (m1 + m2) * u2);
		}
	}
}