/*
    Template of the main wrapper was taken
    from the public Intel snippest example
    `ocl-tools-walkthrough` TemplateGFX example.
    
    Everything is used for non-commercial purposes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include <string>

#include <sys/time.h>
#include "include/sha256_opencl.h"

int main(int argc, char* argv[])
{
    cl_int err;
    ocl_args_d_t ocl;
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;//CL_DEVICE_TYPE_GPU;//
    //LogInfo("Selected device type is CL_DEVICE_TYPE_GPU\n");
    
    ////
    // initialize Open CL objects (context, queue, etc.)
    ////
    std::string platformStr;
    if (CL_SUCCESS != SetupOpenCL(&ocl, deviceType, platformStr))// <- platform selection
        return -1;
    
    ////
    // host inputs
    ////
    const size_t n_hashes = 1;
    // unsigned int buffer[N_BUFFER_sz];
    s_inbuf h_in_buf[n_hashes];// = h_inbuf[];// = {1}
    s_outbuf h_out_buf[n_hashes];



    // init host data, example
    for (int ind_hash = 0; ind_hash < n_hashes; ind_hash++)
    {
        h_in_buf[ind_hash].length = MAX_STR_LENGTH_BYTES;// N bytes
        LogInfo("> Input string (hash %i), len=%d:\n`", ind_hash, h_in_buf[ind_hash].length);
        // for (size_t ind = 0; ind < MAX_STR_LENGTH_BYTES; ind+=4)
        // {   
        //     h_in_buf[ind_hash].buffer[ind/4] = 0;
        //     //write 'aaaa'
        //     if (ind + 0 < MAX_STR_LENGTH_BYTES)
        //         h_in_buf[ind_hash].buffer[ind/4] |= 0x61000000;
        //     if (ind + 1 < MAX_STR_LENGTH_BYTES)
        //         h_in_buf[ind_hash].buffer[ind/4] |= 0x00620000;
        //     if (ind + 2 < MAX_STR_LENGTH_BYTES)
        //         h_in_buf[ind_hash].buffer[ind/4] |= 0x00006300;
        //     if (ind + 3 < MAX_STR_LENGTH_BYTES)
        //         h_in_buf[ind_hash].buffer[ind/4] |= 0x00000064;
        //     //h_out_buf[n_hashes].buffer[ind] = 0xffffffff;
        //     LogInfo(
        //         "%d, %c, %8x \n",
        //         ind/4,
        //         h_in_buf[ind_hash].buffer[ind/4],
        //         h_in_buf[ind_hash].buffer[ind/4]
        //     );
        // }
        LogInfo("`\n");
        h_in_buf[ind_hash].buffer[0]=0x00f24d69;
        h_in_buf[ind_hash].buffer[1]=0x6e65fc61;
        h_in_buf[ind_hash].buffer[2]=0xc71d0e34;
        h_in_buf[ind_hash].buffer[3]=0x517c7bdf;
        h_in_buf[ind_hash].buffer[4]=0x5187c55a;
        h_in_buf[ind_hash].buffer[5]=0xf4f8b61f;
        h_in_buf[ind_hash].buffer[6]=0xdc321588;
        h_in_buf[ind_hash].buffer[7]=0xc7ab768d;
        h_in_buf[ind_hash].buffer[8]=0xee24b006;
        h_in_buf[ind_hash].buffer[9]=0xdf291064;
        h_in_buf[ind_hash].buffer[10]=0x58d7cf00;
        h_in_buf[ind_hash].buffer[11]=0x01020304;
        h_in_buf[ind_hash].buffer[12]=0x05060708;
        h_in_buf[ind_hash].buffer[13]=0x090a0b0c;
        h_in_buf[ind_hash].buffer[14]=0x0d0e0f10;
        h_in_buf[ind_hash].buffer[15]=0x11121314;
        h_in_buf[ind_hash].buffer[16]=0x15161718;
        h_in_buf[ind_hash].buffer[17]=0x191a1b20;
        h_in_buf[ind_hash].buffer[18]=0x11e3998e;
        h_in_buf[ind_hash].buffer[19]=0xadce88ad;
        h_in_buf[ind_hash].buffer[20]=0x28e847f8;
        h_in_buf[ind_hash].buffer[21]=0x057522cb;
        h_in_buf[ind_hash].buffer[22]=0xcc7c4300;
        h_in_buf[ind_hash].buffer[23]=0x01020304;
        h_in_buf[ind_hash].buffer[24]=0x05060708;
        h_in_buf[ind_hash].buffer[25]=0x090a0b0c;
        h_in_buf[ind_hash].buffer[26]=0x0d0e0f10;
        h_in_buf[ind_hash].buffer[27]=0x11121314;
        h_in_buf[ind_hash].buffer[28]=0x15161718;
        h_in_buf[ind_hash].buffer[29]=0x191a1b20;
        h_in_buf[ind_hash].buffer[30]=0x11e39900;



    }
    //00f24d696e65fc61c71d0e34517c7bdf5187c55af4f8b61fdc321588c7ab768dee24b006df29106458d7cf000102030405060708090a0b0c0d0e0f101112131415161718191a1b2011e3998eadce88ad28e847f8057522cbcc7c43000102030405060708090a0b0c0d0e0f101112131415161718191a1b2011e399


    ////
    // Create and build the OpenCL program
    ////
    if (CL_SUCCESS != CreateAndBuildProgram(&ocl, (const char *)argv[1]))
        return -1;

    // Program consists of kernels.
    ocl.kernel = clCreateKernel(ocl.program, "func_sha256", &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateKernel returned %s\n", TranslateOpenCLError(err));
        return -1;
    }


    ////
    // Create & write the data & set args
    // OpenCL Device buffers
    ////
    // create buffers
    ocl.in_buf = clCreateBuffer(ocl.context,  CL_MEM_READ_ONLY,  sizeof(s_inbuf) * n_hashes, NULL, &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateImage for `ocl.in_length` returned %s\n", TranslateOpenCLError(err));
        return -1;
    }
    ocl.out_buf = clCreateBuffer(ocl.context,  CL_MEM_WRITE_ONLY,  sizeof(s_outbuf) * n_hashes, NULL, &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateImage for `ocl.out_buf` returned %s\n", TranslateOpenCLError(err));
        return -1;
    }

    // Write a and b vectors into compute device memory
    err = clEnqueueWriteBuffer(ocl.commandQueue, ocl.in_buf, CL_TRUE, 0, sizeof(s_inbuf) * n_hashes, h_in_buf, 0, NULL, NULL);
    if (CL_SUCCESS != err)
    {
        LogError("Error:Copying `h_in_buf` to device at device `out_buf` %s\n", TranslateOpenCLError(err));
        return -1;
    }

    // Passing arguments into OpenCL kernel.
    err  = clSetKernelArg(ocl.kernel, 0, sizeof(cl_mem), (void *)&ocl.in_buf);
    if (CL_SUCCESS != err)
    {
        LogError("Error: Failed to set argument `in_buf`, returned %s\n", TranslateOpenCLError(err));
        return -1;
    }
    err  = clSetKernelArg(ocl.kernel, 1, sizeof(cl_mem), (void *)&ocl.out_buf);
    if (CL_SUCCESS != err)
    {
        LogError("Error: Failed to set argument `out_buf`, returned %s\n", TranslateOpenCLError(err));
        return -1;
    }


    ////
    // Execute with time profiling
    ////
    struct timeval performanceCountNDRangeStart;
    bool queueProfilingEnable = true;
    if (queueProfilingEnable)
        gettimeofday(&performanceCountNDRangeStart, NULL);

    // Execute (enqueue) the kernel
    if (CL_SUCCESS != ExecuteAddKernel(&ocl, n_hashes))
        return -1;

    struct timeval performanceCountNDRangeStop;
    if (queueProfilingEnable)
        gettimeofday(&performanceCountNDRangeStop, NULL);


    ////
    // Get results from Device to Host
    ////
    err = clEnqueueReadBuffer(ocl.commandQueue, ocl.out_buf, CL_TRUE, 0, sizeof(s_outbuf) * n_hashes, h_out_buf, 0, NULL, NULL);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clEnqueueReadBuffer returned %s\n", TranslateOpenCLError(err));
        return false;
    }


    // retrieve performance counter frequency
    if (queueProfilingEnable)
    {
        struct timeval runtime;
        timersub(&performanceCountNDRangeStop, &performanceCountNDRangeStart, &runtime);
        LogInfo("> NDRange performance counter time:\t%f [ms]\n", (runtime.tv_sec * 1000.f + runtime.tv_usec / 1000.f));
    }


    for (int ind_hash = 0; ind_hash < n_hashes; ind_hash++)
    {
        LogInfo("> Hash string:`");
        h_in_buf[ind_hash].length = n_hashes;
        for (int ind = 0; ind < 8; ind++)
        {   
            uint32_t little_end = h_out_buf[ind_hash].buffer[ind];
                // ((h_out_buf[ind_hash].buffer[ind]>>24)&0xff) |
                // ((h_out_buf[ind_hash].buffer[ind]<<8)&0xff0000) |
                // ((h_out_buf[ind_hash].buffer[ind]>>8)&0xff00) |
                // ((h_out_buf[ind_hash].buffer[ind]<<24)&0xff000000);
            unsigned char bytes[4] = {
                (little_end >> 24) & 0xFF,
                (little_end >> 16) & 0xFF,
                (little_end >> 8) & 0xFF,
                little_end & 0xFF
            };
            //sprintf(buffer,"%02X ",onebyte);
            // LogInfo("%02x%02x%02x%02x", bytes[0],bytes[1],bytes[2],bytes[3]);//works
            LogInfo("%0.8x", little_end);
        }
        LogInfo("`\n");
    }
    LogError("> End of calculations\n");

    return 0;
}