#include "marshal.h"

#include "proxy_properties_soa.h"
#include "solver.h"

extern solver sys_solver;

marshal::marshal() {
	properties_soa = new proxy_properties_soa();
}

marshal::~marshal() {
	delete properties_soa;
}

std::optional<u32> marshal::register_proxy(const FVector2& position, const FVector2& initial_velocity, f64 Mass, f64 DragCoefficient, f64 MediumDensity, f64 SurfaceArea, u32 debug_gen_id) {
	std::optional<u32> res;
	const BindingIDs& IDs = bind_objects();
	
	// bind the ParticleID with an ProxyID to associate the new proxy and it's Particle
	if (sys_solver.create_proxy(IDs.Particle, position, initial_velocity, Mass, DragCoefficient, MediumDensity, SurfaceArea, debug_gen_id)) {
		particle_proxy_map_[IDs.Particle] = IDs.Proxy;
		proxy_particle_map_[IDs.Proxy] = IDs.Particle;
		res = IDs.Proxy;
	}
	
	return res;
}

bool marshal::unregister_proxy(u32) {
	return false;
}

std::optional<ProxyValues> marshal::get_proxy_values(u32 InParticleID) const {
	return sys_solver.get_proxy_values(InParticleID);
}

BindingIDs marshal::bind_objects() {
	return BindingIDs{id_counter_, id_counter_++};
}
