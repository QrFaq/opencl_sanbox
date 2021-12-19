# Minimal version of CMake
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
 
project(main)

#set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "../${CMAKE_SOURCE_DIR}/")


set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.65.1 COMPONENTS program_options REQUIRED) 

set(MAIN_DIR ${CMAKE_SOURCE_DIR}/..)
set(DIR_LOAD_PROJ ${MAIN_DIR}/sha256_opencl_load_test)
set(DIR_GPU_PROJ ${MAIN_DIR}/sha256_opencl)
set(DIR_CPU_PROJ ${MAIN_DIR}/sha256_cpu)

set(OPENCL_INCLUDE_DIR /opt/intel/system_studio_2020/opencl/SDK/include)
set(OPENCL_LIBRARIES /opt/intel/system_studio_2020/opencl/SDK/lib64)
include_directories(
	${OPENCL_INCLUDE_DIR}
	${DIR_GPU_PROJ}
	${DIR_CPU_PROJ}
	${Boost_INCLUDE_DIR}
	)

set(LOAD_PROJ_SRC_DIR ${DIR_LOAD_PROJ}/src)
set(GPU_PROJ_SRC_DIR ${DIR_GPU_PROJ}/src)
set(GPU_PROJ_3RDPARTIES_DIR ${DIR_GPU_PROJ}/3rd_parties)
set(SOURCES ${GPU_PROJ_SRC_DIR}/ocl_args.cpp
			${GPU_PROJ_3RDPARTIES_DIR}/utils.cpp
			${LOAD_PROJ_SRC_DIR}/main.cpp
			${DIR_CPU_PROJ}/sha256_cpu.cpp
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -w")

# Set up executable
if(Boost_FOUND)
	add_executable (main ${SOURCES})
	target_link_libraries(main ${OPENCL_LIBRARIES} OpenCL ${Boost_LIBRARIES})
endif()