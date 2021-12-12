#include <stdint.h>

class SHA256
{
// public:
//     SHA256();
public://TODO: private
    // logical function
    uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
    uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);
    uint32_t sum_from_zero(uint32_t x);
    uint32_t sum_from_one(uint32_t x);
    uint32_t sigma_from_zero(uint32_t x);
    uint32_t sigma_from_one(uint32_t x);

    uint32_t rotr(uint32_t x, uint32_t n);
    uint32_t shr(uint32_t x);
};