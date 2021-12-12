#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <boost/format.hpp>

int main(int argc, char** argv)
{
    const char* message = "message";

    const unsigned int hash_length = 256;
    //uint8_t* hash = new uint8_t[hash_length];
    uint8_t* hash = new uint8_t[3];
    hash[0] = 1;
    hash[2] = 2;
    hash[0] = 3;

    std::cout << boost::format("> Input message: `%1%`\nhash:") % message;
    for (int i=0;i<3;i++)
        std::cout << +hash[i] << " ";
    std::cout << std::endl;
    
}