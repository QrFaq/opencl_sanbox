#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <boost/format.hpp>
#include "SHA256.h"

/*
g++ main.cpp sha256.cpp -o main
*/

int main(int argc, char** argv)
{
    const char* message = "message";

    const unsigned int hash_length = 256;
    uint8_t* hash = new uint8_t[hash_length];

    std::cout << boost::format("> Input message: `%1%`\nhash:\n") % message;
    for (int i=0;i<hash_length;i++)
        std::cout << +hash[i] << " ";
    std::cout << std::endl;
}