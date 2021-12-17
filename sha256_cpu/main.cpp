#include <cstdio>
#include <iostream>
#include <boost/format.hpp>
#include <chrono>

#include "sha256_cpu.h"

/*
g++ main.cpp sha256_cpu.cpp -o main
*/

int main(int argc, char** argv)
{   //<=5<=7 works 
    const char* message = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";//"1234567891234567891234567891234567891234567891234567891";//works only for 56 symbols
    std::cout << boost::format("char=%02x") % static_cast<int>(message[0]);

    const uint8_t nbytes = 32;
    uint8_t* hash = new uint8_t[nbytes];  // == 256 [bits]
    
    SHA256_CPU sha256_cpu = SHA256_CPU();

    // get hash of the string with time measurement
    using ns = std::chrono::nanoseconds;
    auto start = std::chrono::high_resolution_clock::now();
    sha256_cpu.get_msg_hash(hash, (uint8_t*) message, std::strlen(message));
    auto finish = std::chrono::high_resolution_clock::now();
    
    std::cout << boost::format(">\tInput message: `%1%`\n\thash:\t") % message;
    for (int i = 0; i < nbytes; i++)
        std::cout << boost::format("%02x") % static_cast<int>(hash[i]);// << " ";
    std::cout << boost::format("\tDuration: %1% [ns]\n") % std::chrono::duration_cast<ns>(finish - start).count();

    // free memory
    delete[] hash; 
}