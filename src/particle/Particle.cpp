#include "Particle.h"

#include "physics/marshal.h"
#include "physics/physics_helpers.h"
#include "physics/solver.h"

extern marshal sys_marshal;
extern solver sys_solver;

// Particle::Particle() {
// 	construct(FVector2{0.0, 0.0}, FVector2{0.0, 0.0}, 1.0, 1.0, Physics::air_density, 1.0);
// }

particle::~particle() {
	sys_solver.on_proxies_updated.unbind_callback(this, &particle::on_proxy_updated);
}

particle::particle(
	const FVector2 &position,
	const FVector2 &initial_velocity,
	f64 Mass,
	f64 DragCoefficient,
	f64 MediumDensity,
	f64 SurfaceArea,
	u32 DEBUG_gen_id
) {
	if (const std::optional<u32> v = sys_marshal.register_proxy(position, initial_velocity, Mass, DragCoefficient, MediumDensity, SurfaceArea, DEBUG_gen_id)) {
		ProxyID = v.value();
	}
	constructed_ = true;
}

particle::particle(
	const particle &other
) {
	VisualPosition = other.VisualPosition;
	ProxyID = other.ProxyID;
}

particle::particle(
	particle &&other
) noexcept {
	VisualPosition = other.VisualPosition;
	ProxyID = other.ProxyID;
}

particle &particle::operator=(
	const particle &other
) {
	return *this;
}

particle &particle::operator=(
	particle &&other
) noexcept {
	return *this;
}

void particle::update() {
}

void particle::register_events() {
	sys_solver.on_proxies_updated.bind_callback(this, &particle::on_proxy_updated);
	registered_events_ = true;
}

void particle::on_proxy_updated() {
	if (!ProxyID.has_value()) {
		return;
	}

	if (const std::optional<ProxyValues> &values = sys_marshal.get_proxy_values(ProxyID.value())) {
		const ProxyValues proxy_values = values.value();
		VisualPosition = proxy_values.P;
	}
}
