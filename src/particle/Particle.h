#pragma once
#include "types.h"
#include "vectors/Vector2.h"

#include <optional>

class Particle {
public:
	~Particle();

	Particle(const FVector2&, const FVector2&, f64, f64, f64, f64, u32);
	Particle(const Particle&);
	Particle(Particle&&) noexcept;
	
	Particle& operator=(const Particle&);
	Particle& operator=(Particle&&) noexcept;

	void update();
	void register_events();
private:

	Particle() = delete;

	void on_proxy_updated();
	
	bool constructed_ = false;
	bool registered_events_ = false;
	
public:
	std::optional<u32> ProxyID;
	
	FVector2 VisualPosition;
};
