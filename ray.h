#pragma once
#include "vec3.h"

class ray {
public:
	ray() {};
	ray(const vec3& a, const vec3& b) { A = a; B = b; }
	vec3 origin() const		{ return A; }
	vec3 direction() const  { return B; }
	vec3 point_at_parameter(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
};

vec3 color(const ray& r)
{
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5f, 0.7f, 1.0f); // without f compliler throws a warning
}
