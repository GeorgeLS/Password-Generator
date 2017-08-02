#ifndef PCG_32
#define PCG_32

#include <inttypes.h>
#define PCG32_INIT { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

typedef struct pcg_state_seq_64
{
  uint64_t state;
  uint64_t seq;
} pcg32_random_t;

void pcg32_srandom(uint64_t, uint64_t);
void pcg32_srandom_r(pcg32_random_t*, uint64_t, uint64_t);
uint32_t pcg32_random(void);
uint32_t pcg32_random_r(pcg32_random_t*);
uint32_t pcg32_boundedrand(uint32_t);
uint32_t pcg32_boundedrand_r(pcg32_random_t*, uint32_t);

#endif