g++ \
    -L/opt/intel/system_studio_2020/opencl/SDK/lib64 \
    -l../sha256_opencl \
    -I/opt/intel/system_studio_2020/opencl/SDK/include:../sha256_opencl \
     \
    ./src/*.cpp \
    -Wno-deprecated-declarations \
    -w \
    -shared \
    -lOpenCL \
    -o main



g++ \
    -L /opt/intel/system_studio_2020/opencl/SDK/lib64 \
    -I /opt/intel/system_studio_2020/opencl/SDK/include:.:./sha256_opencl \
    -iquote /home/iri/Documents/cuda_tuts/opencl_sanbox \
    sha256_opencl/src/ocl_args.cpp sha256_opencl/3rd_parties/*.cpp ./src/main.cpp \
    -Wno-deprecated-declarations \
    -w \
    -lOpenCL \
    -o main

./main "../sha256_opencl/src/sha256.cl"