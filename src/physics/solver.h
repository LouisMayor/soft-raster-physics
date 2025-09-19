#pragma once

#include "proxy.h"
#include "types.h"
#include "delegate/delegate.h"

#include <memory>
#include <optional>
#include <vector>

struct ProxyValues {
	FVector2 P;
};

// https://itscai.us/blog/post/ue-physics-framework/#the-grand-stage

CREATE_DELEGATE(OnProxiesUpdated);

class solver {
public:
	void advance(const FVector2&, f64);
	bool create_proxy(u32, const FVector2&, const FVector2&, f64, f64, f64, f64, u32);

	OnProxiesUpdated on_proxies_updated;

	[[nodiscard]] std::optional<ProxyValues> get_proxy_values(u32) const;

private:
	void advance_step(std::vector<std::unique_ptr<proxy>>&, const FVector2&, const f64) const;

	[[nodiscard]] FVector2 solve_forces(const FVector2&, f64, f64, f64, f64, f64) const;
	bool solve_world_collision(const FVector2&, const FVector2&, OUT FVector2&) const;
	void solve_particle_collision(const proxy*, const proxy*, const FVector2&, OUT FVector2&, OUT FVector2&) const;

	[[nodiscard]] FVector2 get_force_from_world_collision(const FVector2&) const;
	[[nodiscard]] std::tuple<FVector2, FVector2> get_force_from_particle_collision(const proxy*, const proxy*) const;

	std::vector<std::unique_ptr<proxy>> proxies_;
};

/* TODOs */

/* Needs some spatial reasoning (probably quad-trees)
 *	- e.g. want 'local force effectors', e.g. wind on layers of the world. DON'T want to have to consider every particle that gets advanced...
 *	- want some way of querying, so that solve_forces(...) returns with some FVector2 value, plus gravity, plus some force that's affecting the particle.
 */

/* https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods
 * Fictious Particles
 * https://physics.weber.edu/schroeder/javacourse/LatticeBoltzmann.pdf
 */

/* computational physics papers
 * Energy Stability and Fracture for Frame Rate Rigid Body Simulations - http://physbam.stanford.edu/~fedkiw/papers/stanford2009-02.pdf
 */

/* timestep reference
 * https://gafferongames.com/post/fix_your_timestep/
 * todo: implement fixed timestep (sub-stepping)
 */