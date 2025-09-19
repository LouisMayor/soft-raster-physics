#pragma once

#include "Particle.h"
#include "vectors/Vector2.h"
#include "vectors/Vector4.h"

#include <memory>
#include <vector>

// General description of the particles the generator will spawn
struct GeneratorDesc {
	FVector ParticleColour = FVector{1.0, 1.0, 1.0, 1.0};
	FVector2 GeneratorPosition = FVector2{0.0, 0.0};
	f64 Mass;
	f64 DragCoefficient;
	f64 MediumDensity;
	f64 SurfaceArea;
	bool Persistent = false;
	f64 Lifetime = 1.0;
	u32 NumOfParticles = 1;
	f64 SpawnDelay = 1.0;
};

// Particle generator, will use the GeneratorDesc to spawn N amount of particles
class Generator {
public:
	Generator(const GeneratorDesc&);
	Particle* spawn();
	void tick(f64 dt);

	// can create a system which process particles, rather than the generator storing them
	// if needed, the generator can reference them by id and 'parent' e.g the Generator
	std::vector<std::unique_ptr<Particle>> Particles;
	
private:
	Generator() = delete;
	Generator(const Generator&) = delete;
	Generator(const Generator&&) noexcept = delete;
	
	Generator& operator=(const Generator&) = delete;
	Generator& operator=(Generator&&) noexcept = delete;

	GeneratorDesc desc_;

	void spawn_update(f64 dt);

	u32 spawn_count_ = 0;
	f64 spawn_time_ = 0.0;
	bool reached_target_ = false;

	inline static u32 generator_id_ = 0;
};
