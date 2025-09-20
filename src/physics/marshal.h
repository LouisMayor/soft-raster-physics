#pragma once
#include "solver.h"
#include "types.h"
#include "vectors/Vector2.h"

#include <optional>
#include <unordered_map>

class proxy_properties_soa;
class proxy;

struct BindingIDs {
	u32 Proxy;
	u32 Particle;
};

class marshal {
  public:
	marshal();
	~marshal();

	std::optional<u32> register_proxy(
		const FVector2 &,
		const FVector2 &,
		f64,
		f64,
		f64,
		f64,
		u32
	);
	bool unregister_proxy(u32);

	[[nodiscard]] std::optional<ProxyValues> get_proxy_values(u32) const;

	// todo: restructure so this doesn't exist here, and then we can remove circular dependencies
	proxy_properties_soa *properties_soa;

  private:
	BindingIDs bind_objects();

	// bi-directional communication between particle and proxy objects.
	// e.g. 'game' world and 'simulation' world
	std::unordered_map<u32, u32> particle_proxy_map_;
	std::unordered_map<u32, u32> proxy_particle_map_;

	u32 id_counter_ = 0;
};
