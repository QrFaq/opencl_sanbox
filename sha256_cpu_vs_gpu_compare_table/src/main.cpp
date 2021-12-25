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
#include "TextTable.h"
#include "sha256_cpu.h"
#include "include/sha256_opencl.h"
#include <random>
#include <boost/format.hpp>

std::string convert_int(int n)
{
   std::stringstream ss;
   ss << n;
   return ss.str();
}

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

std::string get_str_from_osstream(std::ostringstream* oss)
{
    std::string str = oss->str();//.c_str();
    oss->str("");
    oss->clear();
    return str;
}

void generate_output_table(std::string res_table_str[][4], const unsigned int n_hashes)
{
    ////
    // Generate output table
    LogError("> Draw table : start\n");
    TextTable t( '-', '|', '+' );

    t.add( "Hardware" ); t.add( "Input string" ); t.add( "Hash" );t.add( "Time, [ms]" );t.endOfRow();
    for (unsigned int hash_ind=0; hash_ind < n_hashes; hash_ind++)
    {
        //std::string res_table_str[n_hashes * 2][4];
        t.add(res_table_str[hash_ind * 2][0]);
        t.add(res_table_str[hash_ind * 2][1]);
        t.add(res_table_str[hash_ind * 2][2]);
        t.add(res_table_str[hash_ind * 2][3]);
        t.endOfRow();

        //res_table_str[hash_ind * 2 + 1][0] = "s";
        //res_table_str[hash_ind * 2 + 1][1] = "s";
        //res_table_str[hash_ind * 2 + 1][2] = "s";
        //res_table_str[hash_ind * 2 + 1][3] = "s";

        t.add(res_table_str[hash_ind * 2 + 1][0]);
        t.add(res_table_str[hash_ind * 2 + 1][1]);
        t.add(res_table_str[hash_ind * 2 + 1][2]);
        t.add(res_table_str[hash_ind * 2 + 1][3]);
        t.endOfRow();
    }

    t.setAlignment( 2, TextTable::Alignment::RIGHT );
    std::cout << t;
    LogError("> Draw table : end\n");
}

int main(int argc, char* argv[])
{
    ////
    // Hash configuraions
    ////
    const unsigned int n_hashes = 10;                // cpu
    const size_t gen_str_len = MAX_STR_LENGTH_BYTES;// parameter shall be setted in sha256_opencl.h
    const size_t n_hashes_gpu_process = n_hashes;   // gpu
    
    std::string res_table_str[n_hashes * 2][4];
    std::ostringstream oss;

    /// 
    // CPU-version configuration
    const uint8_t nbytes = 32;
    SHA256_CPU sha256_cpu = SHA256_CPU();

    ////
    // GPU configuration
    
    cl_int err;
    ocl_args_d_t ocl;
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    LogInfo("Selected device type is CL_DEVICE_TYPE_GPU\n");
    
    // initialize Open CL objects (context, queue, etc.)
    std::string platformStr;
    if (CL_SUCCESS != SetupOpenCL(&ocl, deviceType, platformStr))// <- platform selection
        return -1;
    
    // host inputs
    s_inbuf h_in_buf[n_hashes_gpu_process];
    s_outbuf h_out_buf[n_hashes_gpu_process];

    // Create and build the OpenCL program
    if (CL_SUCCESS != CreateAndBuildProgram(&ocl, (const char *)argv[1]))
        return -1;

    // Program consists of kernels.
    ocl.kernel = clCreateKernel(ocl.program, "func_sha256", &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateKernel returned %s\n", TranslateOpenCLError(err));
        return -1;
    }
    
    // create buffers
    ocl.in_buf = clCreateBuffer(ocl.context,  CL_MEM_READ_ONLY,  sizeof(s_inbuf) * n_hashes_gpu_process, NULL, &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateImage for `ocl.in_length` returned %s\n", TranslateOpenCLError(err));
        return -1;
    }
    ocl.out_buf = clCreateBuffer(ocl.context,  CL_MEM_WRITE_ONLY,  sizeof(s_outbuf) * n_hashes_gpu_process, NULL, &err);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clCreateImage for `ocl.out_buf` returned %s\n", TranslateOpenCLError(err));
        return -1;
    }

    // End of configuration
    ////

    // create data
    std::vector<std::string> input_messages = {};
    for (unsigned int hash_ind=0; hash_ind < n_hashes; hash_ind++)
    {
        // generate string
        auto message_str = gen_random_str(gen_str_len);
        // const char* message_debug = "123456789123456789123456789123456789123456789123";//"1234567891234567891234567891234567891234567891234567891";//"1RYFEhBZ6fcl1jqo48PYbMezcF1sieGdKtxsL7RYqCuSzXTu";//"test_str";
        // std::string message_str(message_debug);


        input_messages.push_back(message_str);
        const char * message = input_messages[hash_ind].c_str();
        LogInfo("> CPU/GPU In-msg[%2i] (0.2hex):`", hash_ind);
        for (int i = 0; i < std::strlen(message); i++)
        {
            if (i%4==0 && i!=0) LogInfo(" ");
            LogInfo("%02x", message[i]);
            oss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(message[i]);
        }
        LogInfo("`\n");
        
        // fill table array
        res_table_str[hash_ind * 2][0] = std::string("cpu");
        res_table_str[hash_ind * 2 + 1][0] = std::string("gpu");
        std::string inp_msg_str = get_str_from_osstream(&oss);
        res_table_str[hash_ind * 2][1] = inp_msg_str;
        res_table_str[hash_ind * 2 + 1][1] = inp_msg_str;
    }


    // Main table generation loop CPU
    unsigned long total_cpu_calc_time = 0;
    for (unsigned int hash_ind=0; hash_ind < n_hashes; hash_ind++)
    {
        const char * message = input_messages[hash_ind].c_str();
        
        ////
        // Calculate hash with CPU
        // & time the code
        ////
        //hash = {0};//<-gives segm fault

        // // get hash of the string with time measurement
        uint8_t* hash = new uint8_t[nbytes];  // == 256 [bits]
        using ns = std::chrono::nanoseconds;
        auto start = std::chrono::high_resolution_clock::now();
        sha256_cpu.get_msg_hash(hash, (uint8_t*) message, std::strlen(message));
        auto finish = std::chrono::high_resolution_clock::now();

        LogInfo("> Hash string CPU[%i]:`", hash_ind);
        for (int i = 0; i < nbytes; i++)
        {
            LogInfo("%02x", static_cast<int>(hash[i]));
            oss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(hash[i]);
        }
        LogInfo("\n");

        // fill table array
        std::string buf_str = get_str_from_osstream(&oss);  // hash is here
        res_table_str[hash_ind * 2][2] = buf_str;

        sha256_cpu.reset();

        // calculate unit\total processing time & add to the table
        unsigned long cpu_calc_time = std::chrono::duration_cast<ns>(finish - start).count();
        oss << std::dec << cpu_calc_time/1000000.f;
        buf_str = get_str_from_osstream(&oss);  // unit processing time is here
        res_table_str[hash_ind * 2][3] = buf_str;        
        total_cpu_calc_time = total_cpu_calc_time + cpu_calc_time;

        // free memory of CPU code
        delete[] hash, message; 
    }
    LogInfo("\tDuration: %d [ns]\n", total_cpu_calc_time);

    ////
    // Calculate hash with GPU
    // & time the code
    ////
    // convert char message to unsigned int
    // Convert to data-type
    for (unsigned int hash_ind=0; hash_ind < n_hashes; hash_ind++)
    {
        h_in_buf[hash_ind].length = gen_str_len;// N bytes
        for (size_t ind = 0; ind < N_BUFFER_sz; ind++)
            h_in_buf[hash_ind].buffer[ind] = 0;
        const char * message = input_messages[hash_ind].c_str();
        memcpy(h_in_buf[hash_ind].buffer, message, gen_str_len);
    }

    // GPU main start

    // Write a and b vectors into compute device memory
    cl_event ev[3];
    err = clEnqueueWriteBuffer(ocl.commandQueue, ocl.in_buf, CL_TRUE, 0, sizeof(s_inbuf) * n_hashes_gpu_process, h_in_buf, 0, NULL, &ev[0]);
    if (CL_SUCCESS != err)
    {
        LogError("Error:Copying `h_in_buf` to device at device `out_buf` %s\n", TranslateOpenCLError(err));
        return -1;
    }

    // Passing arguments into OpenCL kernel.
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


    ////
    // Execute with time profiling
    ////
    struct timeval performanceCountNDRangeStart;
    bool queueProfilingEnable = true;
    if (queueProfilingEnable)
        gettimeofday(&performanceCountNDRangeStart, NULL);

    // Execute (enqueue) the kernel
    err = clEnqueueNDRangeKernel(ocl.commandQueue, ocl.kernel, 1, NULL, &n_hashes_gpu_process, NULL, 0, NULL, &ev[1]);
    // size_t local_sz[1] = {1};
    // err = clEnqueueNDRangeKernel(ocl.commandQueue, ocl.kernel, 1, NULL, &n_hashes_gpu_process, local_sz, 0, NULL, &ev[1]);
    clRetainKernel(ocl.kernel);
    if (CL_SUCCESS != err)
    {
        LogError("Error: Failed to run kernel, return %s\n", TranslateOpenCLError(err));
        return err;
    }

    // Wait until the queued kernel is completed by the device
    err = clFinish(ocl.commandQueue);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clFinish return %s\n", TranslateOpenCLError(err));
        return err;
    }
    if (CL_SUCCESS != ExecuteAddKernel(&ocl, n_hashes_gpu_process))
        return -1;

    struct timeval performanceCountNDRangeStop;
    if (queueProfilingEnable)
        gettimeofday(&performanceCountNDRangeStop, NULL);


    ////
    // Get results from Device to Host
    ////
    err = clEnqueueReadBuffer(ocl.commandQueue, ocl.out_buf, CL_TRUE, 0, sizeof(s_outbuf) * n_hashes_gpu_process, h_out_buf, 0, 0, &ev[2]);
    if (CL_SUCCESS != err)
    {
        LogError("Error: clEnqueueReadBuffer returned %s\n", TranslateOpenCLError(err));
        return false;
    }
    

    err = clWaitForEvents(3,ev);
    // err = clFlush(ocl.commandQueue);
    // if (CL_SUCCESS != err)
    // {
    //     LogError("Error: clFinish return %s\n", TranslateOpenCLError(err));
    //     return err;
    // }




    for (int hash_ind = 0; hash_ind < n_hashes_gpu_process; hash_ind++)
    {
        LogInfo("> Hash string GPU[%i]:`", hash_ind);
        h_in_buf[hash_ind].length = n_hashes_gpu_process;
        for (int ind = 0; ind < 8; ind++)
        {   
            unsigned int little_end = //h_out_buf[hash_ind].buffer[ind];
                ((h_out_buf[hash_ind].buffer[ind]>>24)&0xff) |
                ((h_out_buf[hash_ind].buffer[ind]<<8)&0xff0000) |
                ((h_out_buf[hash_ind].buffer[ind]>>8)&0xff00) |
                ((h_out_buf[hash_ind].buffer[ind]<<24)&0xff000000);
            LogInfo("%0.8x", little_end);
            oss << std::hex << std::setfill('0') << std::setw(8) << static_cast<int>(little_end);
        }
        LogInfo("`\n");

        // fill table array
        std::string buf_str = get_str_from_osstream(&oss);  // hash is here
        res_table_str[hash_ind * 2 + 1][2] = buf_str;
    }

    // retrieve performance counter frequency
    if (queueProfilingEnable)
    {
        struct timeval runtime;
        timersub(&performanceCountNDRangeStop, &performanceCountNDRangeStart, &runtime);
        float gpu_calc_time = (runtime.tv_sec * 1000.f + runtime.tv_usec / 1000.f);
        LogInfo("\tDuration: %f [ms]\n", gpu_calc_time);

        for (unsigned int hash_ind=0; hash_ind < n_hashes; hash_ind++)
        {
            oss << std::dec << gpu_calc_time / n_hashes;
            std::string buf_str = get_str_from_osstream(&oss);  // init processing time is here
            res_table_str[hash_ind * 2+1][3] = buf_str;
        }

    }
    //err = clFlush(ocl.commandQueue);


    // clReleaseMemObject(ocl.in_buf);
    // clReleaseMemObject(ocl.out_buf)  
    LogError("> End of calculations\n");

    generate_output_table(res_table_str, n_hashes);
    


    return 0;
}