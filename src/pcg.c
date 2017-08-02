#include "pcg.h"

static pcg32_random_t pcg32_global = PCG32_INIT;

void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate, uint64_t initseq)
{
  rng->state = 0U;
  rng->seq = (initseq << 1U) | 1U;
  pcg32_random_r(rng);
  rng->state += initstate;
  pcg32_random_r(rng);
}

void pcg32_srandom(uint64_t seed, uint64_t seq)
{
  pcg32_srandom_r(&pcg32_global, seed, seq);
}

uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->seq;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint32_t pcg32_random(void)
{
    return pcg32_random_r(&pcg32_global);
}

uint32_t pcg32_boundedrand_r(pcg32_random_t* rng, uint32_t bound)
{
  uint32_t threshold = -bound % bound;
  for (;;)
  {
    uint32_t r = pcg32_random_r(rng);
    if (r >= threshold)
    {
      return r % bound;
    }
  }
}

uint32_t pcg32_boundedrand(uint32_t bound)
{
    return pcg32_boundedrand_r(&pcg32_global, bound);
}
