#include "proxy.h"
#include "marshal.h"
#include "proxy_properties_soa.h"

extern marshal sys_marshal;

proxy::proxy(u32 ID, const FVector2& Position, const FVector2& Velocity, f64 Mass, f64 DragCoefficient, f64 MediumDensity, f64 SurfaceArea, u32 gid) {
	ParticleID = ID;
	set_position(Position);
	set_velocity(Velocity);
	set_mass(Mass);
	set_drag_coefficient(DragCoefficient);
	set_medium_density(MediumDensity);
	set_surface_area(SurfaceArea);
	debug_set_generator_id(gid);
}

f64& proxy::get_mass() {
	return sys_marshal.properties_soa->get_mass(ParticleID);
}

f64& proxy::get_drag_coefficient() {
	return sys_marshal.properties_soa->get_drag_coefficient(ParticleID);
}

f64& proxy::get_surface_area() {
	return sys_marshal.properties_soa->get_surface_area(ParticleID);
}

f64& proxy::get_medium_density() {
	return sys_marshal.properties_soa->get_medium_density(ParticleID);
}

f64 proxy::get_drag_coefficient() const {
	return sys_marshal.properties_soa->get_drag_coefficient(ParticleID);
}

f64 proxy::get_surface_area() const {
	return sys_marshal.properties_soa->get_surface_area(ParticleID);
}

f64 proxy::get_medium_density() const {
	return sys_marshal.properties_soa->get_medium_density(ParticleID);
}

FVector2& proxy::get_position() {
	return sys_marshal.properties_soa->get_position(ParticleID);
}

FVector2& proxy::get_velocity() {
	return sys_marshal.properties_soa->get_velocity(ParticleID);
}

f64 proxy::get_mass() const {
	return sys_marshal.properties_soa->get_mass(ParticleID);
}

FVector2 proxy::get_position() const {
	return sys_marshal.properties_soa->get_position(ParticleID);
}

FVector2 proxy::get_velocity() const {
	return sys_marshal.properties_soa->get_velocity(ParticleID);
}

void proxy::set_mass(f64 m) const {
	sys_marshal.properties_soa->set_mass(ParticleID, m);
}

void proxy::set_drag_coefficient(f64 cd) const {
	sys_marshal.properties_soa->set_drag_coefficient(ParticleID, cd);
}

void proxy::set_surface_area(f64 a) const {
	sys_marshal.properties_soa->set_surface_area(ParticleID, a);
}

void proxy::set_medium_density(f64 d) const {
	sys_marshal.properties_soa->set_medium_density(ParticleID, d);
}
 
void proxy::set_position(const FVector2 &p) const {
	sys_marshal.properties_soa->set_position(ParticleID, p);
}

void proxy::set_velocity(const FVector2 &v) const {
	sys_marshal.properties_soa->set_velocity(ParticleID, v);
}

void proxy::debug_set_generator_id(u32 gid) const {
	sys_marshal.properties_soa->debug_set_generator_id(ParticleID, gid);
}

u32 proxy::debug_get_generator_id() const {
	return sys_marshal.properties_soa->debug_get_generator_id(ParticleID);
}
