#include <stdint.h>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <boost/format.hpp>

#include "SHA256.h"

/*
g++ main.cpp sha256.cpp -o main
*/

int main(int argc, char** argv)
{
    const char* message = "message";

    const uint8_t nbytes = 32;
    uint8_t* hash = new uint8_t[nbytes];  // == 256 [bits]
    
    SHA256 sha256 = SHA256();
    sha256.get_msg_hash(hash, (uint8_t*) message, std::strlen(message));
    
    std::cout << boost::format("> Input message: `%1%`\nhash:\t") % message;
    for (int i = 0; i < nbytes; i++)
        std::cout << +hash[i] << " ";
    std::cout << std::endl;

    // free memory
    delete[] hash; 
}