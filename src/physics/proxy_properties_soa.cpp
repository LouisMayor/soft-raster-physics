#include "proxy_properties_soa.h"

#include <cassert>

f64 &proxy_properties_soa::get_mass(
	u32 InProxyIndex
) {
	assert(true == exists(mass_, InProxyIndex));
	return mass_[InProxyIndex];
}

f64 &proxy_properties_soa::get_drag_coefficient(
	u32 InProxyIndex
) {
	assert(true == exists(drag_coefficient_, InProxyIndex));
	return drag_coefficient_[InProxyIndex];
}

f64 &proxy_properties_soa::get_surface_area(
	u32 InProxyIndex
) {
	assert(true == exists(surface_area_, InProxyIndex));
	return surface_area_[InProxyIndex];
}

f64 &proxy_properties_soa::get_medium_density(
	u32 InProxyIndex
) {
	assert(true == exists(medium_density_, InProxyIndex));
	return medium_density_[InProxyIndex];
}

f64 proxy_properties_soa::get_drag_coefficient(
	u32 InProxyIndex
) const {
	assert(true == exists(drag_coefficient_, InProxyIndex));
	return drag_coefficient_[InProxyIndex];
}

f64 proxy_properties_soa::get_surface_area(
	u32 InProxyIndex
) const {
	assert(true == exists(surface_area_, InProxyIndex));
	return surface_area_[InProxyIndex];
}

f64 proxy_properties_soa::get_medium_density(
	u32 InProxyIndex
) const {
	assert(true == exists(medium_density_, InProxyIndex));
	return medium_density_[InProxyIndex];
}

FVector2 &proxy_properties_soa::get_position(
	u32 InProxyIndex
) {
	assert(true == exists(position_, InProxyIndex));
	return position_[InProxyIndex];
}

FVector2 &proxy_properties_soa::get_velocity(
	u32 InProxyIndex
) {
	assert(true == exists(velocity_, InProxyIndex));
	return velocity_[InProxyIndex];
}

f64 proxy_properties_soa::get_mass(
	u32 InProxyIndex
) const {
	assert(true == exists(mass_, InProxyIndex));
	return mass_[InProxyIndex];
}

FVector2 proxy_properties_soa::get_position(
	u32 InProxyIndex
) const {
	assert(true == exists(position_, InProxyIndex));
	return position_[InProxyIndex];
}

FVector2 proxy_properties_soa::get_velocity(
	u32 InProxyIndex
) const {
	assert(true == exists(velocity_, InProxyIndex));
	return velocity_[InProxyIndex];
}

void proxy_properties_soa::set_mass(
	u32 InProxyIndex,
	f64 m
) {
	add_or_insert(mass_, InProxyIndex, m);
}

void proxy_properties_soa::set_drag_coefficient(
	u32 InProxyIndex,
	f64 cd
) {
	add_or_insert(drag_coefficient_, InProxyIndex, cd);
}

void proxy_properties_soa::set_surface_area(
	u32 InProxyIndex,
	f64 a
) {
	add_or_insert(surface_area_, InProxyIndex, a);
}

void proxy_properties_soa::set_medium_density(
	u32 InProxyIndex,
	f64 d
) {
	add_or_insert(medium_density_, InProxyIndex, d);
}

void proxy_properties_soa::set_position(
	u32 InProxyIndex,
	const FVector2 &p
) {
	add_or_insert(position_, InProxyIndex, p);
}

void proxy_properties_soa::set_velocity(
	u32 InProxyIndex,
	const FVector2 &v
) {
	add_or_insert(velocity_, InProxyIndex, v);
}

u32 proxy_properties_soa::debug_get_generator_id(
	u32 InProxyIndex
) const {
	assert(true == exists(generator_ids_, InProxyIndex));
	return generator_ids_[InProxyIndex];
}

void proxy_properties_soa::debug_set_generator_id(
	u32 InProxyIndex,
	u32 gid
) {
	add_or_insert(generator_ids_, InProxyIndex, gid);
}
