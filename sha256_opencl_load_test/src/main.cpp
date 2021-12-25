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
#include <chrono>
#include <iostream>
#include "sha256_cpu.h"
#include "include/sha256_opencl.h"
#include <random>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <limits.h>  // for INT_MAX, INT_MIN

typedef struct cl_s_inbuf {
	cl_uint length;
	cl_uint buffer[N_BUFFER_sz];
};

typedef struct cl_s_outbuf {
	cl_uint buffer[N_BUFFER_sz];
};

std::string gen_random_str(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distributionInteger(0, 63);

    time_t now;
    for (int i = 0; i < len; ++i) {
        time(&now);
        srand((unsigned int)now);
        tmp_s += alphanum[ distributionInteger(generator) % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

void generateInput(cl_uint* inputArray, unsigned int array_size, unsigned int seed)
{
    srand(seed);

    // random initialization of input
    for (unsigned int i = 0; i < array_size; ++i)
        inputArray[i] = rand() / 100;
}

void generateRandInputBatch(cl_s_inbuf* batchArray, unsigned int batch_size, unsigned int seed)
{
    // random initialization of input
    for (unsigned int i = 0; i < batch_size; ++i)
    {
        batchArray->length = MAX_STR_LENGTH_BYTES;
        generateInput(batchArray->buffer, batch_size, seed + i);
    }
}

std::string get_device_name_by_id(cl_device_id device_id)
{
    size_t valueSize;
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, 0, NULL, &valueSize);
    char* value;
    value = (char*) malloc(valueSize);
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, valueSize, value, NULL);
    std::string hw_attribute(value);
    free(value);
    return hw_attribute;
}

std::string get_device_hardware_version_by_id(cl_device_id device_id)
{
    size_t valueSize;
    char* value;
    cl_uint maxComputeUnits;
    clGetDeviceInfo(device_id, CL_DEVICE_VERSION, 0, NULL, &valueSize);
    value = (char*) malloc(valueSize);
    clGetDeviceInfo(device_id, CL_DEVICE_VERSION, valueSize, value, NULL);
    std::string hw_attribute(value);
    free(value);
    return hw_attribute;
}

std::string get_device_software_driver_version_by_id(cl_device_id device_id)
{
    size_t valueSize;
    char* value;
    clGetDeviceInfo(device_id, CL_DRIVER_VERSION, 0, NULL, &valueSize);
    value = (char*) malloc(valueSize);
    clGetDeviceInfo(device_id, CL_DRIVER_VERSION, valueSize, value, NULL);
    std::string hw_attribute(value);
    free(value);
    return hw_attribute;
}

std::string get_device_openclc_vers_by_id(cl_device_id device_id)
{
    size_t valueSize;
    char* value;
    clGetDeviceInfo(device_id, CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
    value = (char*) malloc(valueSize);
    clGetDeviceInfo(device_id, CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
    std::string hw_attribute(value);
    free(value);
    return hw_attribute;
}

cl_uint get_device_nparalle_comp_units_by_id(cl_device_id device_id)
{
    cl_uint maxComputeUnits;
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS,
            sizeof(maxComputeUnits), &maxComputeUnits, NULL);
    return maxComputeUnits;
}

cl_uint get_device_max_work_group_size_by_id(cl_device_id device_id)
{
    size_t maxWorkGroupSz;
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE,
            sizeof(maxWorkGroupSz), &maxWorkGroupSz, NULL);
    return maxWorkGroupSz;
}

cl_uint get_kernel_work_group_size_by_id(cl_device_id device, cl_kernel kernel)
{
    size_t maxKernelWorkGroupSz;
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_WORK_GROUP_SIZE,
            sizeof(maxKernelWorkGroupSz), &maxKernelWorkGroupSz, NULL);
    return maxKernelWorkGroupSz;
}

/*
    Test Kernel time measurement based on generated
    random strings of `MAX_STR_LENGTH_BYTES` size
    with such parameters as:
        - `t_interval_s` : test duration time, [s],
        - deviceType : {CL_DEVICE_TYPE_CPU, CL_DEVICE_TYPE_GPU},
        - gpu_batch_sz : number of processing hashes per one kernel
*/
int main(int argc, char* argv[])
{
    ////
    // Parse cmd arguments
    boost::program_options::variables_map vm;
    boost::program_options::options_description desc{"Options"};
    desc.add_options()
    ("help,h", "Help")
    ("k", boost::program_options::value<std::string>()->default_value("path to .cl file"), "kernel_path")
    ("t", boost::program_options::value<int64_t>()->default_value(1), "t_interval")
    ("g", boost::program_options::value<size_t>()->default_value(1), "global_work_sz==gpu_batch_sz")
    ("l", boost::program_options::value<size_t>()->default_value(1), "local_work_sz")
    ;
    store(parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << '\n';
        return 0;
    }

    std::string kernel_path = vm["k"].as<std::string>();
    int64_t t_interval_s = vm["t"].as<int64_t>();
    size_t global_work_size = vm["g"].as<size_t>();
    size_t local_work_size = vm["l"].as<size_t>();
    const size_t gpu_batch_sz = global_work_size;
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;

    ////
    // GPU configuration
    
    cl_int err;
    ocl_args_d_t ocl;
    
    // initialize Open CL objects (context, queue, etc.)
    std::string platformStr;
    if (CL_SUCCESS != SetupOpenCL(&ocl, deviceType, platformStr))// <- platform selection
        return -1;
    
    // host inputs
    cl_uint in_optimizedSize = ((sizeof(cl_s_inbuf) * gpu_batch_sz - 1)/64 + 1) * 64;
    cl_uint out_optimizedSize = ((sizeof(cl_s_outbuf) * gpu_batch_sz - 1)/64 + 1) * 64;
    cl_s_inbuf* h_in_buf;//[gpu_batch_sz];     // place to store all generated hashes
    cl_s_outbuf* h_out_buf;//[gpu_batch_sz];   // place to store gpu batch

    if(posix_memalign((void**)&h_in_buf, 4096, in_optimizedSize) != 0)
    {
        LogError("Error: posix_memalign failed to allocate buffer.\n");
        return -1;
    }
    if(posix_memalign((void**)&h_out_buf, 4096, out_optimizedSize) != 0)
    {
        LogError("Error: posix_memalign failed to allocate buffer.\n");
        return -1;
    }


    // Create and build the OpenCL program
    if (CL_SUCCESS != CreateAndBuildProgram(&ocl, (const char *) kernel_path.c_str()))
        return -1;

    // Program consists of kernels.
    ocl.kernel = clCreateKernel(ocl.program, "func_sha256", &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateKernel returned %s\n", TranslateOpenCLError(err));
        return -1;
    }
    
    // create buffers
    ocl.in_buf = clCreateBuffer(ocl.context,  CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(cl_s_inbuf) * gpu_batch_sz, h_in_buf, &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateBuffer for `ocl.in_length` returned %s\n", TranslateOpenCLError(err));
        return -1;
    }
    ocl.out_buf = clCreateBuffer(ocl.context,  CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(cl_s_outbuf) * gpu_batch_sz, h_out_buf, &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateBuffer for `ocl.out_buf` returned %s\n", TranslateOpenCLError(err));
        return -1;
    }

    // End of configuration
    ////

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Execute (enqueue) the kernel
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Execute with time profiling
    bool queueProfilingEnable = true;
    struct timeval performanceCountNDRangeStart;
    struct timeval performanceCountNDRangeStop;
    float total_gpu_calc_time = 0.0;
    unsigned int loop_counter = 0;
    LogInfo("> Load test : start\n");
    for (
        auto start = std::chrono::steady_clock::now(),
        now = start; now < start + std::chrono::seconds{t_interval_s};
        now = std::chrono::steady_clock::now()
        )
    {
        generateRandInputBatch(h_in_buf, gpu_batch_sz, loop_counter);
        //LogInfo("> loop_counter=%d\n", loop_counter);

        // Start time point of kernel time measurement
        if (queueProfilingEnable)
            gettimeofday(&performanceCountNDRangeStart, NULL);

        // Passing arguments into OpenCL kernel
        err  = clSetKernelArg(ocl.kernel, 0, sizeof(cl_mem), &ocl.in_buf);
        if (CL_SUCCESS != err)
        {
            LogError("Error: Failed to set argument `in_buf`, returned %s\n", TranslateOpenCLError(err));
            return -1;
        }
        err  = clSetKernelArg(ocl.kernel, 1, sizeof(cl_mem), &ocl.out_buf);
        if (CL_SUCCESS != err)
        {
            LogError("Error: Failed to set argument `out_buf`, returned %s\n", TranslateOpenCLError(err));
            return -1;
        }

        // Execute (enqueue) the kernel
        err = clEnqueueNDRangeKernel(
            ocl.commandQueue, ocl.kernel, 1,
            // *global_work_offset, *global_work_size, *local_work_size,
            NULL, &global_work_size, &local_work_size,
            0, NULL, NULL
        );
        if (CL_SUCCESS != err)
        {
            LogError("Error: Failed to run kernel, return %s\n", TranslateOpenCLError(err));
            return -1;
        }

        // Wait until the queued kernel is completed by the device
        err = clFinish(ocl.commandQueue);
        if (CL_SUCCESS != err)
        {
            LogError("Error: clFinish return %s\n", TranslateOpenCLError(err));
            return -1;
        }

        // End time point of kernel time measurement
        if (queueProfilingEnable)
        {
            gettimeofday(&performanceCountNDRangeStop, NULL);
            struct timeval runtime;
            timersub(&performanceCountNDRangeStop, &performanceCountNDRangeStart, &runtime);
            total_gpu_calc_time += (runtime.tv_sec * 1000.f + runtime.tv_usec / 1000.f);
        }
        loop_counter++;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LogError("> Load test : end\n");
    
    // retrieve performance counter frequency
    if (queueProfilingEnable)
    {
        LogInfo("\nTest result information:\n");
        unsigned int total_numb_calc_hashes = static_cast<unsigned int>(gpu_batch_sz) * loop_counter;
        float avg_gpu_calc_time_ms = static_cast<float>(total_numb_calc_hashes) / total_gpu_calc_time;
        LogInfo("\tLoop kernal launch time: %d [s]\n", t_interval_s);
        LogInfo("\tTotal CPU/GPU working duration: %f [ms]\n", total_gpu_calc_time);
        LogInfo("\tKernel calculation time per batch: %f [hash/s]\n", avg_gpu_calc_time_ms * 1000.f);

        LogInfo("\tTotal number of calculated hashes: %d\n", total_numb_calc_hashes);
        LogInfo("Selected processing platform:\n\t%s\n", platformStr.c_str());       
        LogInfo("Tested Hardware information:\n");
        LogInfo("\tTested hardware device: %s\n", get_device_name_by_id(ocl.device).c_str());
        LogInfo("\tHardware version: %s\n", get_device_hardware_version_by_id(ocl.device).c_str());
        LogInfo("\tSoftware version: %s\n", get_device_software_driver_version_by_id(ocl.device).c_str());
        LogInfo("\tOpenCL C version: %s\n", get_device_openclc_vers_by_id(ocl.device).c_str());
        LogInfo("\tParallel compute units: %d\n", get_device_nparalle_comp_units_by_id(ocl.device));

        LogInfo("\tDevice max work-group: %d\n", get_device_max_work_group_size_by_id(ocl.device));
        LogInfo("\tKernel work-group size: %d\n", get_kernel_work_group_size_by_id(ocl.device, ocl.kernel));
    }
    
    return 0;
}