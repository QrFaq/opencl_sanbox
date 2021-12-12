#include "SHA256.h"
#include <iostream>
/*
    w, word-length = 32 (SHA256)
*/

uint32_t SHA256::Ch(uint32_t x, uint32_t y, uint32_t z){}
uint32_t SHA256::Maj(uint32_t x, uint32_t y, uint32_t z){}
uint32_t SHA256::sum_from_zero(uint32_t x){}
uint32_t SHA256::sum_from_one(uint32_t x){}
uint32_t SHA256::sigma_from_zero(uint32_t x){}
uint32_t SHA256::sigma_from_one(uint32_t x){}

uint32_t SHA256::rotr(uint32_t x, uint32_t n) {
    std::cout << x << "," << n << "\n";
    return (x << n) | (x >> (32 - n));   //w = 32
}
uint32_t SHA256::shr(uint32_t x){}