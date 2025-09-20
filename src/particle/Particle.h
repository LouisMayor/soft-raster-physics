#pragma once
#include "types.h"
#include "vectors/Vector2.h"

#include <optional>

class particle {
  public:
	~particle();

	particle(
		const FVector2 &,
		const FVector2 &,
		f64,
		f64,
		f64,
		f64,
		u32
	);
	particle(const particle &);
	particle(particle &&) noexcept;

	particle &operator=(const particle &);
	particle &operator=(particle &&) noexcept;

	void update();
	void register_events();

  private:
	particle() = delete;

	void on_proxy_updated();

	bool constructed_ = false;
	bool registered_events_ = false;

  public:
	std::optional<u32> ProxyID;

	FVector2 VisualPosition;
};
