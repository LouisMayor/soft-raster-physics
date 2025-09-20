#pragma once
#include "vectors/Vector2.h"

// https://itscai.us/blog/post/ue-physics-framework/#the-grand-stage

class proxy {
  public:
	proxy() = default;
	proxy(
		u32,
		const FVector2 &,
		const FVector2 &,
		f64,
		f64,
		f64,
		f64,
		u32
	);

	[[nodiscard]] f64 &get_mass();
	[[nodiscard]] f64 &get_drag_coefficient();
	[[nodiscard]] f64 &get_surface_area();
	[[nodiscard]] f64 &get_medium_density();

	[[nodiscard]] FVector2 &get_position();
	[[nodiscard]] FVector2 &get_velocity();

	[[nodiscard]] f64 get_mass() const;
	[[nodiscard]] f64 get_drag_coefficient() const;
	[[nodiscard]] f64 get_surface_area() const;
	[[nodiscard]] f64 get_medium_density() const;

	[[nodiscard]] FVector2 get_position() const;
	[[nodiscard]] FVector2 get_velocity() const;

	void set_mass(f64) const;
	void set_drag_coefficient(f64) const;
	void set_surface_area(f64) const;
	void set_medium_density(f64) const;

	void set_position(const FVector2 &) const;
	void set_velocity(const FVector2 &) const;

	// debug
	void debug_set_generator_id(u32) const;
	[[nodiscard]] u32 debug_get_generator_id() const;

	u32 ParticleID; // references the 'Particle' object, which can later draw itself with the Position
};
