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

bool hit_sphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - 4*a*c;
	return (discriminant > 0);
}

vec3 color(const ray& r)
{
	if (hit_sphere(vec3(0, 0, -1.0), 0.5, r))
	{
		return vec3(1, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5f, 0.7f, 1.0f); // without f compliler throws a warning
}
