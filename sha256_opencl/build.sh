g++ \
    -L/opt/intel/system_studio_2020/opencl/SDK/lib64 \
    -I/opt/intel/system_studio_2020/opencl/SDK/include:./3rd_parties \
    src/*.cpp 3rd_parties/*.cpp \
    -Wno-deprecated-declarations \
    -w \
    -lOpenCL \
    -o main

./main "src/sha256.cl"