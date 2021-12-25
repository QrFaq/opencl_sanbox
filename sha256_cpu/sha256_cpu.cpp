#include "sha256_cpu.h"
#include <stdio.h>

/*
    w, word-length = 32 (SHA256_CPU)
*/
SHA256_CPU::SHA256_CPU(){}

void SHA256_CPU::get_msg_hash(uint8_t* hash, uint8_t* msg, const size_t msg_length)
{
    SHA256_CPU::hash = hash;
    SHA256_CPU::msg_length = msg_length;

    ////
    // Preprocessing
    ////

    // padding calculation to 512-bits size
    // Message structure: [message [bytes], 1, padding bits, 64-bit as message size]
    size_t bits = (msg_length * 8 ) + 1 + 64;//TODO: may be here a problem
    size_t padding_size_bits = (512 - (bits % 512)) % 512;
    bits += padding_size_bits;

    // Calculate message blocks number:
    //M[0], M[1], .., M[N]
    SHA256_CPU::blocks_number = bits / 512;
    SHA256_CPU::remainig_blocks_n = blocks_number;

    ////
    // Hasing
    ////
    std::memcpy(&(SHA256_CPU::h), &(SHA256_CPU::h0), 32); // rst

    int remain_sz_msg = msg_length;

    uint8_t* msg_block = new uint8_t[64]; // == 512-bits
    for (size_t i = 0; i < SHA256_CPU::blocks_number; i++)
    {        
        if (remain_sz_msg > 0)
        {
            std::memset(msg_block, 0, 64);
			size_t cp_size = (remain_sz_msg  < 64) ? remain_sz_msg : 64;  // last iteration check
			std::memcpy(msg_block, msg, cp_size);
        }

        SHA256_CPU::remainig_blocks_n--;
        SHA256_CPU::process_block(msg_block);
        remain_sz_msg -= 64;
    }

    // store final result
    for (size_t i = 0; i < 8; i++)
    {
        uint32_t* hash32 = (uint32_t*) SHA256_CPU::hash;
        hash32[i] = __builtin_bswap32(SHA256_CPU::h[i]);
    }

    delete[] msg_block;
}

void SHA256_CPU::process_block(uint8_t* block)
{
    bool isTime2placeBit =
        (SHA256_CPU::blocks_number - SHA256_CPU::remainig_blocks_n) * 64 > SHA256_CPU::msg_length &&
        !SHA256_CPU::bit_placed;
    if (isTime2placeBit)
    {
        block[SHA256_CPU::msg_length % 64] = 0x80;
        SHA256_CPU::bit_placed = true;
    }

    if (SHA256_CPU::remainig_blocks_n == 0)
    {
        // write msg length into the last 64-bits
        // in big endian format
        uint64_t* last64 = (uint64_t*) &block[56];
        *last64 = __builtin_bswap64(SHA256_CPU::msg_length * 8);
    }

    // read data as 32-bit message blocks, M[i]
    uint32_t* pM = (uint32_t*) block;

    ////
    // prepare message schedule

    // w[t], for t in [0, 63]
    uint32_t w[64] = {0};

    // fill t=0..15 as M<t, i>
    for (size_t t = 0; t < 16; t++)
    {
        w[t] = __builtin_bswap32(*pM);
        pM++;
    }
    printf(">   W[0]:%8x W[1]:%8x W[2]:%8x W[3]:%8x W[4]:%8x W[5]:%8x W[6]:%8x W[7]:%8x\n", w[0x0], w[0x1], w[0x2], w[0x3], w[0x4], w[0x5], w[0x6], w[0x7]);
    printf(">   W[8]:%8x W[9]:%8x W[10]:%8x W[11]:%8x W[12]:%8x W[13]:%8x W[14]:%8x W[15]:%8x\n", w[0x8], w[0x9], w[0xA], w[0xB], w[0xC], w[0xD], w[0xE], w[0xF]);

    // fill t=16..63 as M<t, i>
    for (size_t t = 16; t < 64; t++)
    {
        w[t] =  SHA256_CPU::sigma_from_one(w[t - 2]) +
                w[t - 7] + 
                SHA256_CPU::sigma_from_zero(w[t - 15]) + 
                w[t - 16];
    }//+

    ////
    // Hashing
    ////
    // init working variables
    std::memcpy(&abcdefgh, &h, 32); // store H(i-1)

    // Update working variables main loop
    for (size_t t = 0; t < 64; t++)
    {
        uint32_t T1 =   abcdefgh[7] +
                        SHA256_CPU::sum_from_one(abcdefgh[4]) +
                        SHA256_CPU::Ch(abcdefgh[4], abcdefgh[5], abcdefgh[6]) +
                        SHA256_CPU::k[t] + w[t];//+
        uint32_t T2 =   SHA256_CPU::sum_from_zero(abcdefgh[0]) +
                        SHA256_CPU::Maj(abcdefgh[0], abcdefgh[1], abcdefgh[2]);//+
        
        abcdefgh[7] = abcdefgh[6];
        abcdefgh[6] = abcdefgh[5];
        abcdefgh[5] = abcdefgh[4];
        abcdefgh[4] = abcdefgh[3] + T1;
        abcdefgh[3] = abcdefgh[2];
        abcdefgh[2] = abcdefgh[1];
        abcdefgh[1] = abcdefgh[0];
        abcdefgh[0] = T1 + T2;//+
    }

    // compute i-th intermidiate value H(i)
    for (size_t i = 0; i < 8; i++)
        SHA256_CPU::h[i] += SHA256_CPU::abcdefgh[i];
}
		
void SHA256_CPU::reset()
{
    SHA256_CPU::hash = nullptr;
    SHA256_CPU::blocks_number = -1;
    SHA256_CPU::remainig_blocks_n = -1;
    SHA256_CPU::msg_length = -1;
    SHA256_CPU::bit_placed = false;

    for (size_t i = 0; i < 8; i++)
    {
        SHA256_CPU::h[i] = 0;
        SHA256_CPU::abcdefgh[i] = 0xffffffff;
    }
}



/*
    Logical operands
*/
uint32_t SHA256_CPU::Ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (~x & z);
}
uint32_t SHA256_CPU::Maj(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}
uint32_t SHA256_CPU::sum_from_zero(uint32_t x)
{
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}
uint32_t SHA256_CPU::sum_from_one(uint32_t x)
{
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}
uint32_t SHA256_CPU::sigma_from_zero(uint32_t x)
{
    return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3);
}
uint32_t SHA256_CPU::sigma_from_one(uint32_t x)
{
    return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10);
}

uint32_t SHA256_CPU::rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));   //w = 32
}

uint32_t SHA256_CPU::shr(uint32_t x, uint32_t n)
{
    return x >> n;
}