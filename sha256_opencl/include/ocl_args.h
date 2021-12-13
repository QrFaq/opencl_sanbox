#include "CL/cl.h"


typedef struct s_inbuf{
    cl_uint length;
    // unsigned int length;
    cl_mem buffer;//unsigned int buffer[32/4];
} t_inbuf;

typedef struct s_outbuf{
    cl_mem buffer;//unsigned int buffer[32/4];
} t_outbuf;

struct ocl_args_d_t
{
    ocl_args_d_t();
    ~ocl_args_d_t();

    // Regular OpenCL objects:
    cl_context       context;           // hold the context handler
    cl_device_id     device;            // hold the selected device handler
    cl_command_queue commandQueue;      // hold the commands-queue handler
    cl_program       program;           // hold the program handler
    cl_kernel        kernel;            // hold the kernel handler
    float            platformVersion;   // hold the OpenCL platform version (default 1.2)
    float            deviceVersion;     // hold the OpenCL device version (default. 1.2)
    float            compilerVersion;   // hold the device OpenCL C version (default. 1.2)
    
    // Objects that are specific for algorithm implemented in this sample
    cl_mem           srcA;              // hold first source buffer
    cl_mem           srcB;              // hold second source buffer
    cl_mem           dstMem;            // hold destination buffer

    t_inbuf inbuf;
    t_outbuf outbuf;
};