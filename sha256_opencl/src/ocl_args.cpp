#include "3rd_parties/utils.h"

ocl_args_d_t::ocl_args_d_t():
        context(NULL),
        device(NULL),
        commandQueue(NULL),
        program(NULL),
        kernel(NULL),
        platformVersion(OPENCL_VERSION_1_2),
        deviceVersion(OPENCL_VERSION_1_2),
        compilerVersion(OPENCL_VERSION_1_2),
        in_buf(NULL),
        out_buf(NULL)
{
}

ocl_args_d_t::~ocl_args_d_t()
{
    cl_int err = CL_SUCCESS;

    if (kernel)
    {
        err = clReleaseKernel(kernel);
        if (CL_SUCCESS != err)
        {
            LogError("Error: clReleaseKernel returned '%s'.\n", TranslateOpenCLError(err));
        }
    }
    if (program)
    {
        err = clReleaseProgram(program);
        if (CL_SUCCESS != err)
        {
            LogError("Error: clReleaseProgram returned '%s'.\n", TranslateOpenCLError(err));
        }
    }
    if (in_buf)
    {
        err = clReleaseMemObject(in_buf);
        if (CL_SUCCESS != err)
        {
            LogError("Error: clReleaseMemObject returned '%s'.\n", TranslateOpenCLError(err));
        }
    }
    if (out_buf)
    {
        err = clReleaseMemObject(out_buf);
        if (CL_SUCCESS != err)
        {
            LogError("Error: clReleaseMemObject returned '%s'.\n", TranslateOpenCLError(err));
        }
    }
    if (commandQueue)
    {
        err = clReleaseCommandQueue(commandQueue);
        if (CL_SUCCESS != err)
        {
            LogError("Error: clReleaseCommandQueue returned '%s'.\n", TranslateOpenCLError(err));
        }
    }
    if (device)
    {
        err = clReleaseDevice(device);
        if (CL_SUCCESS != err)
        {
            LogError("Error: clReleaseDevice returned '%s'.\n", TranslateOpenCLError(err));
        }
    }
    if (context)
    {
        err = clReleaseContext(context);
        if (CL_SUCCESS != err)
        {
            LogError("Error: clReleaseContext returned '%s'.\n", TranslateOpenCLError(err));
        }
    }
}
