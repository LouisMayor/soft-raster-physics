#include "solver.h"

#include "physics_helpers.h"
#include "proxy.h"

#include <algorithm>
#include <execution>

void solver::advance(const FVector2& WorldBounds, const f64 DeltaTime) {
	advance_step(proxies_, WorldBounds, DeltaTime);
	on_proxies_updated.execute();
}

bool solver::create_proxy(u32 ParticleID, const FVector2& Position, const FVector2& Initialvelocity, f64 Mass, f64 DragCoefficient, f64 MediumDensity, f64 SurfaceArea, u32 gid) {
	// todo: a better way to assign values when constructed?
	proxies_.emplace_back(std::make_unique<proxy>(ParticleID, Position, Initialvelocity, Mass, DragCoefficient, MediumDensity, SurfaceArea, gid));
	return true;
}

std::optional<ProxyValues> solver::get_proxy_values(u32 InParticleID) const {
	for (const std::unique_ptr<proxy>& proxy_ptr : proxies_) {
		if (proxy_ptr->ParticleID == InParticleID) {
			return ProxyValues{proxy_ptr->get_position()};
		}
	}
	return {};
}

void solver::advance_step(std::vector<std::unique_ptr<proxy>>& InProxies, const FVector2& WorldBounds, f64 dt) const {
	std::for_each(std::execution::par_unseq, std::begin(InProxies), std::end(InProxies), [&](const std::unique_ptr<proxy>& proxy_instance) {
		proxy_instance->get_velocity() += solve_forces(proxy_instance->get_velocity(), proxy_instance->get_mass(), proxy_instance->get_drag_coefficient(), proxy_instance->get_medium_density(), proxy_instance->get_surface_area(), dt);

		FVector2 OutLastValidLocation;
		if (solve_world_collision(proxy_instance->get_position() + proxy_instance->get_velocity() * dt, WorldBounds, OutLastValidLocation)) {
			proxy_instance->get_velocity() = get_force_from_world_collision(proxy_instance->get_velocity());
			proxy_instance->get_position() = OutLastValidLocation + proxy_instance->get_velocity() * dt;
			return;
		}

		// lazily check for other particle collisions
		bool collided = false;
		std::for_each(std::execution::par_unseq, std::begin(InProxies), std::end(InProxies), [&](const std::unique_ptr<proxy>& other_proxy_instance) {
			// can't stop execution, so will keep skipping elements
			if (collided) {
				return;
			}
			
			if (proxy_instance->ParticleID == other_proxy_instance->ParticleID) {
				return;
			}

			const FVector2& NextPos = proxy_instance->get_position() + proxy_instance->get_velocity() * dt;
			const FVector2& OtherParticlePos = other_proxy_instance->get_position();
			const f64 Distance = FVector2::Distance(NextPos, OtherParticlePos);
			if (Distance <= 1.0 + std::numeric_limits<double>::epsilon()) {
				FVector2 OutParticleAVelocity, OutParticleBVelocity;
				solve_particle_collision(proxy_instance.get(), other_proxy_instance.get(), NextPos - OtherParticlePos, OutParticleAVelocity, OutParticleBVelocity);
				proxy_instance->get_velocity() = OutParticleAVelocity * dt;
				other_proxy_instance->get_velocity() = OutParticleBVelocity * dt;
				collided = true;
			}
		});

		proxy_instance->get_position() += proxy_instance->get_velocity() * dt;
	});
}

FVector2 solver::solve_forces(const FVector2& v, f64 m, f64 cd, f64 d, f64 a, f64 dt) const {
	// todo: use Physics::drag()
	return Physics::acceleration(m, cd, a, d) * dt;
}

bool solver::solve_world_collision(const FVector2& NextPosition, const FVector2& WorldBounds, OUT FVector2& WorldCollisionLocation) const {
	bool no_hit = true;
	for (i32 i = 0; i < FVector2::N; ++i) {
		if (NextPosition.XY[i] > WorldBounds.XY[i]) {
			WorldCollisionLocation.XY[i] = NextPosition.XY[i];
			no_hit &= false;
		} else if (NextPosition.XY[i] < 0.) {
			WorldCollisionLocation.XY[i] = 0.;
			no_hit &= false;
		} else {
			WorldCollisionLocation.XY[i] = NextPosition.XY[i];
		}
	}
	return !no_hit;
}

void solver::solve_particle_collision(const proxy* const ParticleA, const proxy* const ParticleB, const FVector2& ImpactDirection, OUT FVector2& OutParticleAVelocity, OUT FVector2& OutParticleBVelocity) const {
	const std::tuple<FVector2, FVector2> &velocities = get_force_from_particle_collision(ParticleA, ParticleB);
	OutParticleAVelocity = ImpactDirection + std::get<0>(velocities);
	OutParticleBVelocity = -ImpactDirection + std::get<1>(velocities);
}

FVector2 solver::get_force_from_world_collision(const FVector2& Velocity) const {
	return Velocity.Abs() * 0.25;
}

std::tuple<FVector2, FVector2> solver::get_force_from_particle_collision(const proxy* const ParticleA, const proxy* const ParticleB) const {
	FVector2 A, B;
	Physics::elastic_collision(ParticleA->get_velocity(), ParticleB->get_velocity(), ParticleA->get_mass(), ParticleB->get_mass(), A, B);
	return std::make_tuple(A, B);
}

// constexpr f64 air_power = 1000.0;
// const FVector2 air_turbine = Physics::wind(Physics::air_density, 100.0, air_power, FVector2{cx, sy}) * time;
// const FVector2 air_drag = Physics::drag(1.05, 1.0, Physics::air_density, particle.Velocity) * time;
// particle.update(air_turbine + g - air_drag, bounds, time);

// constexpr f64 water_power = 100.0;
// const FVector2 water_turbine = Physics::wind(Physics::water_density, 10000.0, water_power, FVector2{cx, sy}) * time;
// const FVector2 water_drag = Physics::drag(1.05, 1.0, Physics::water_density, particle.Velocity) * time;
// particle.update(water_turbine + g - water_drag, bounds, time);