#pragma once

#include "types.h"
#include "vectors/Vector2.h"

#include <vector>

class proxy_properties_soa {
  public:
	[[nodiscard]] f64 &get_mass(u32);
	[[nodiscard]] f64 &get_drag_coefficient(u32);
	[[nodiscard]] f64 &get_surface_area(u32);
	[[nodiscard]] f64 &get_medium_density(u32);

	[[nodiscard]] FVector2 &get_position(u32);
	[[nodiscard]] FVector2 &get_velocity(u32);

	[[nodiscard]] f64 get_mass(u32) const;
	[[nodiscard]] f64 get_drag_coefficient(u32) const;
	[[nodiscard]] f64 get_surface_area(u32) const;
	[[nodiscard]] f64 get_medium_density(u32) const;

	[[nodiscard]] FVector2 get_position(u32) const;
	[[nodiscard]] FVector2 get_velocity(u32) const;

	void set_mass(
		u32,
		f64
	);
	void set_drag_coefficient(
		u32,
		f64
	);
	void set_surface_area(
		u32,
		f64
	);
	void set_medium_density(
		u32,
		f64
	);

	void set_position(
		u32,
		const FVector2 &
	);
	void set_velocity(
		u32,
		const FVector2 &
	);

	// debug
	[[nodiscard]] u32 debug_get_generator_id(u32) const;
	void debug_set_generator_id(
		u32,
		u32
	);

	// Unreal Engine style damping
	// 1.0 - LinearDamping * dt;

  private:
	template <typename ValueType>
	bool exists(
		const std::vector<ValueType> &,
		u32
	) const;

	template <typename ValueType>
	void add_or_insert(
		std::vector<ValueType> &,
		u32,
		const ValueType &
	);

	std::vector<f64> mass_;
	std::vector<f64> drag_coefficient_;
	std::vector<f64> surface_area_;
	std::vector<f64> medium_density_; // Hack: simulates the particle moving through some medium

	std::vector<FVector2> position_;
	std::vector<FVector2> velocity_;

	// debug
	std::vector<u32> generator_ids_;
};

template <typename ValueType>
bool proxy_properties_soa::exists(
	const std::vector<ValueType> &vec,
	u32 index
) const {
	return !vec.empty() && index <= vec.size();
}

template <typename ValueType>
void proxy_properties_soa::add_or_insert(
	std::vector<ValueType> &vec,
	u32 index,
	const ValueType &value
) {
	if (vec.empty() || index >= vec.size()) {
		vec.emplace_back(value);
	} else {
		vec[index] = value;
	}
}
