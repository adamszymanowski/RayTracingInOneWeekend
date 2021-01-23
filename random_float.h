#pragma once
#include <random>

// I don't really wan't to separate this, but I guess I have to, given how it's used in the book
inline float // taken from the updated version: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects
random_float()
{
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	float rand = distribution(generator);
	return rand;
}

