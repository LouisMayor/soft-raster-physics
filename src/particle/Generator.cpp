#include "Generator.h"

generator::generator(
	const generator_desc &InDesc
) {
	desc_ = InDesc;
	generator_id_++;
}

particle *generator::spawn() {
	spawn_count_++;
	auto p = std::make_unique<particle>(desc_.GeneratorPosition, FVector2{0.0, 0.0}, desc_.Mass, desc_.DragCoefficient, desc_.MediumDensity, desc_.SurfaceArea, generator_id_);
	return Particles.emplace_back(std::move(p)).get();
}

void generator::tick(
	f64 dt
) {
	if (!reached_target_) {
		spawn_update(dt);
	}
}

void generator::spawn_update(
	f64 dt
) {
	if (!reached_target_) {
		spawn_time_ += dt;
		if (spawn_time_ >= desc_.SpawnDelay) {
			spawn_time_ = 0.0;
			auto p = spawn();
			p->register_events();
		}
	}
	reached_target_ = spawn_count_ >= desc_.NumOfParticles;
}
