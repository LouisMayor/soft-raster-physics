#include "Particle.h"

#include "physics/physics_helpers.h"
#include "physics/marshal.h"
#include "physics/solver.h"

extern marshal sys_marshal;
extern solver sys_solver;

// Particle::Particle() {
// 	construct(FVector2{0.0, 0.0}, FVector2{0.0, 0.0}, 1.0, 1.0, Physics::air_density, 1.0);
// }

Particle::~Particle() {
	sys_solver.on_proxies_updated.unbind_callback(this, &Particle::on_proxy_updated);
}

Particle::Particle(const FVector2 &position, const FVector2 &initial_velocity, f64 Mass, f64 DragCoefficient, f64 MediumDensity, f64 SurfaceArea, u32 DEBUG_gen_id) {
	if (const std::optional<u32> v = sys_marshal.register_proxy(position, initial_velocity, Mass, DragCoefficient, MediumDensity, SurfaceArea, DEBUG_gen_id)) {
		ProxyID = v.value();
	}
	constructed_ = true;
}

Particle::Particle(const Particle &other) {
	VisualPosition = other.VisualPosition;
	ProxyID = other.ProxyID;
}

Particle::Particle(Particle &&other) noexcept {
	VisualPosition = other.VisualPosition;
	ProxyID = other.ProxyID;
}

Particle& Particle::operator=(const Particle &other) {
	return *this;
}

Particle& Particle::operator=(Particle &&other) noexcept {
	return *this;
}

void Particle::update() {
}

void Particle::register_events() {
	sys_solver.on_proxies_updated.bind_callback(this, &Particle::on_proxy_updated);
	registered_events_ = true;
}

void Particle::on_proxy_updated() {
	if (!ProxyID.has_value()) {
		return;
	}
	
	if (const std::optional<ProxyValues> &values = sys_marshal.get_proxy_values(ProxyID.value())) {
		const ProxyValues proxy_values = values.value();
		VisualPosition = proxy_values.P;
	}
}