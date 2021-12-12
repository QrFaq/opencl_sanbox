#include "SHA256.h"
#include <iostream>
/*
    w, word-length = 32 (SHA256)
*/

uint32_t SHA256::Ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (~x & z);
}
uint32_t SHA256::Maj(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}
uint32_t SHA256::sum_from_zero(uint32_t x)
{
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}
uint32_t SHA256::sum_from_one(uint32_t x)
{
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}
uint32_t SHA256::sigma_from_zero(uint32_t x)
{
    return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3);
}
uint32_t SHA256::sigma_from_one(uint32_t x)
{
    return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10);
}

uint32_t SHA256::rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));   //w = 32
}

uint32_t SHA256::shr(uint32_t x, uint32_t n)
{
    return x >> n;
}