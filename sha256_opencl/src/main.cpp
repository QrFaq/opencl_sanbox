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

#include "../3rd_parties/utils.h"
#include <sys/time.h>

#define N_BUFFER_sz 32/4

typedef struct s_inbuf {
	unsigned int length;
	unsigned int buffer[N_BUFFER_sz];
};

typedef struct s_outbuf {
	unsigned int buffer[N_BUFFER_sz];
};

/* Create and build OpenCL program from its source code */
int CreateAndBuildProgram(ocl_args_d_t *ocl, const char * kernel_fpath)
{
    cl_int err = CL_SUCCESS;

    // Upload the OpenCL C source code from the input file to source
    char* source = NULL;
    size_t src_size = 0;
    err = ReadSourceFromFile(kernel_fpath, &source, &src_size);
    if (CL_SUCCESS != err)
    {
        LogError("Error: ReadSourceFromFile returned %s.\n", TranslateOpenCLError(err));
        goto Finish;
    }

    // create OpenCL program object
    ocl->program = clCreateProgramWithSource(ocl->context, 1, (const char**)&source, &src_size, &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateProgramWithSource returned %s.\n", TranslateOpenCLError(err));
        goto Finish;
    }

    // Build the program
    err = clBuildProgram(ocl->program, 1, &ocl->device, "", NULL, NULL);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clBuildProgram() for source program returned %s.\n", TranslateOpenCLError(err));

        // In case of error print the build log to the standard output
        // First check the size of the log
        // Then allocate the memory and obtain the log from the program
        if (err == CL_BUILD_PROGRAM_FAILURE)
        {
            size_t log_size = 0;
            clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

            std::vector<char> build_log(log_size);
            clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, log_size, &build_log[0], NULL);

            LogError("Error happened during the build of OpenCL program.\nBuild log:%s", &build_log[0]);
        }
    }

Finish:
    if (source)
    {
        delete[] source;
        source = NULL;
    }

    return err;
}

/* Execute the kernel */
cl_uint ExecuteAddKernel(ocl_args_d_t *ocl, const size_t n_hashes)
{
    cl_int err = CL_SUCCESS;

    // execute kernel
    err = clEnqueueNDRangeKernel(ocl->commandQueue, ocl->kernel, 1, NULL, &n_hashes, NULL, 0, NULL, NULL);
    if (CL_SUCCESS != err)
    {
        LogError("Error: Failed to run kernel, return %s\n", TranslateOpenCLError(err));
        return err;
    }

    // Wait until the queued kernel is completed by the device
    err = clFinish(ocl->commandQueue);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clFinish return %s\n", TranslateOpenCLError(err));
        return err;
    }

    return CL_SUCCESS;
}

/* OpenCL platform, device, context, and command queue */
int SetupOpenCL(ocl_args_d_t *ocl, cl_device_type deviceType, std::string& platformStr)
{
    // The following variable stores return codes for all OpenCL calls.
    cl_int err = CL_SUCCESS;

    // Query for all available OpenCL platforms on the system
    // Here you enumerate all platforms and pick one which name has preferredPlatform as a sub-string
    cl_platform_id platformId = FindOpenCLPlatform("Intel", deviceType, platformStr);
    if (NULL == platformId)
    {
        LogError("Error: Failed to find OpenCL platform.\n");
        return CL_INVALID_VALUE;
    }

    // Create context with device of specified type.
    // Required device type is passed as function argument deviceType.
    // So you may use this function to create context for any CPU or GPU OpenCL device.
    // The creation is synchronized (pfn_notify is NULL) and NULL user_data
    cl_context_properties contextProperties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platformId,
        0
    };
    ocl->context = clCreateContextFromType(contextProperties, deviceType, NULL, NULL, &err);
    if ((CL_SUCCESS != err) || (NULL == ocl->context))
    {
        LogError("Couldn't create a context, clCreateContextFromType() returned '%s'.\n", TranslateOpenCLError(err));
        return err;
    }

    // Query for OpenCL device which was used for context creation
    err = clGetContextInfo(ocl->context, CL_CONTEXT_DEVICES, sizeof(cl_device_id), &ocl->device, NULL);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clGetContextInfo() to get list of devices returned %s.\n", TranslateOpenCLError(err));
        return err;
    }

    // Read the OpenCL platform's version and the device OpenCL and OpenCL C versions
    GetPlatformAndDeviceVersion(platformId, ocl);

    // Create command queue.
    // OpenCL kernels are enqueued for execution to a particular device through special objects called command queues.
    // Command queue guarantees some ordering between calls and other OpenCL commands.
    // Here you create a simple in-order OpenCL command queue that doesn't allow execution of two kernels in parallel on a target device.
#if defined(CL_VERSION_2_0) || defined(CL_VERSION_2_1)
    if (OPENCL_VERSION_2_0 == ocl->deviceVersion || OPENCL_VERSION_2_1 == ocl->deviceVersion)
    {
        const cl_command_queue_properties properties[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0};
        ocl->commandQueue = clCreateCommandQueueWithProperties(ocl->context, ocl->device, properties, &err);
    } 
    else {
        // default behavior: OpenCL 1.2
        cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
        ocl->commandQueue = clCreateCommandQueue(ocl->context, ocl->device, properties, &err);
    } 
#else
    // default behavior: OpenCL 1.2
    cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
    ocl->commandQueue = clCreateCommandQueue(ocl->context, ocl->device, properties, &err);
#endif
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateCommandQueue() returned %s.\n", TranslateOpenCLError(err));
        return err;
    }

    return CL_SUCCESS;
}

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
    ocl.out_buf = clCreateBuffer(ocl.context,  CL_MEM_WRITE_ONLY,  sizeof(s_outbuf)*N_BUFFER_sz * n_hashes, NULL, &err);
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

    // Regularly you wish to use OpenCL in your application to achieve greater performance results
    // that are hard to achieve in other ways.
    // To understand those performance benefits you may want to measure time your application spent in OpenCL kernel execution.
    // The recommended way to obtain this time is to measure interval between two moments:
    //   - just before clEnqueueNDRangeKernel is called, and
    //   - just after clFinish is called
    // clFinish is necessary to measure entire time spending in the kernel, measuring just clEnqueueNDRangeKernel is not enough,
    // because this call doesn't guarantees that kernel is finished.
    // clEnqueueNDRangeKernel is just enqueue new command in OpenCL command queue and doesn't wait until it ends.
    // clFinish waits until all commands in command queue are finished, that suits your need to measure time.
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