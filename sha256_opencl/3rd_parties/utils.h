/*****************************************************************************
 * Copyright (c) 2013-2018 Intel Corporation
 * All rights reserved.
 *
 * This software and the related documents are Intel copyrighted materials, and
 * your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise, you may
 * not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 *
 * WARRANTY DISCLAIMER
 *
 * THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
 * MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Intel Corporation is the author of the Materials, and requests that all
 * problem reports or change requests be submitted to it directly
 *****************************************************************************/

#include "include/ocl_args.h"
#include <vector>
#include <string>

#ifndef __TEMPLATE_UTILS_CL1234
#define __TEMPLATE_UTILS_CL1234
#include "CL/cl.h"

// Print useful information to the default output. Same usage as with printf
void LogInfo(const char* str, ...);

// Print error notification to the default output. Same usage as with printf
void LogError(const char* str, ...);

// Read OpenCL source code from fileName and store it in source. The number of read bytes returns in sourceSize
int ReadSourceFromFile(const char* fileName, char** source, size_t* sourceSize);

#endif //__TEMPLATE_UTILSCL1234

// Macros for OpenCL versions
#define OPENCL_VERSION_1_2  1.2f
#define OPENCL_VERSION_2_0  2.0f
#define OPENCL_VERSION_2_1  2.1f
//for deprecated command queue usage if necessary
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

//for timesub
#ifndef _BSD_SOURCE
    #define _BSD_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
    #define _DEFAULT_SOURCE
#endif


/* This function helps to create informative messages in
 * case when OpenCL errors occur. It returns a string
 * representation for an OpenCL error code.
 * (E.g. "CL_DEVICE_NOT_FOUND" instead of just -1.)
 */
const char* TranslateOpenCLError(cl_int errorCode);

/*
    Automatic platform pick-ups
*/
int GetPlatformAndDeviceVersion (cl_platform_id platformId, ocl_args_d_t *ocl);
bool CheckPreferredPlatformMatch(cl_platform_id platform, const char* preferredPlatform, std::string& platformStr);
cl_platform_id FindOpenCLPlatform(const char* preferredPlatform, cl_device_type deviceType, std::string& platformStr);