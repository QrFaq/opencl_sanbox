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
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    LogInfo("Selected device type is CL_DEVICE_TYPE_GPU\n");
    
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
        LogInfo("> Input string (hash %i):`", ind_hash);
        h_in_buf[ind_hash].length = 32;// N bytes
        for (size_t ind = 0; ind < N_BUFFER_sz; ind++)
        {
            h_in_buf[ind_hash].buffer[ind] = 0x61616161;//write 'aaaa'
            //h_out_buf[n_hashes].buffer[ind] = 0xffffffff;
            LogInfo("%c", h_in_buf[ind_hash].buffer[ind]);
        }
        LogInfo("`\n");
    }


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
    err = clEnqueueReadBuffer(ocl.commandQueue, ocl.out_buf, CL_TRUE, 0, sizeof(s_outbuf)*N_BUFFER_sz * n_hashes, h_out_buf, 0, NULL, NULL);
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
        for (int ind = 0; ind < N_BUFFER_sz; ind++)
        {   
            unsigned int little_end = //h_out_buf[ind_hash].buffer[ind];
                ((h_out_buf[ind_hash].buffer[ind]>>24)&0xff) |
                ((h_out_buf[ind_hash].buffer[ind]<<8)&0xff0000) |
                ((h_out_buf[ind_hash].buffer[ind]>>8)&0xff00) |
                ((h_out_buf[ind_hash].buffer[ind]<<24)&0xff000000);
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