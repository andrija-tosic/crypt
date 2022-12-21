#include "RNG.h"

uint32_t RNG::xorshift128()
{
	struct xorshift128_state* state = &this->state;
	uint32_t t = state->x[3];

	uint32_t s = state->x[0];
	state->x[3] = state->x[2];
	state->x[2] = state->x[1];
	state->x[1] = s;

	t ^= t << 11;
	t ^= t >> 8;
	return state->x[0] = t ^ s ^ (s >> 19);
}

uint64_t RNG::splitmix64() {
	struct splitmix64_state* state = &this->splitmix64_state;

	uint64_t result = (state->s += 0x9E3779B97f4A7C15);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}

void RNG::xorshift128_init(uint64_t seed) {
	struct xorshift128_state* state = &this->state;

	RNG::splitmix64_state = { seed };

	uint64_t tmp = splitmix64();
	state->x[0] = (uint32_t)tmp;
	state->x[1] = (uint32_t)(tmp >> 32);

	tmp = splitmix64();
	state->x[2] = (uint32_t)tmp;
	state->x[3] = (uint32_t)(tmp >> 32);
}

RNG::RNG() {
	static std::random_device rd;
	std::mt19937 mt_rand(rd());

	xorshift128_init(mt_rand());
}

uint64_t RNG::next() {
	return this->xorshift128();
}
