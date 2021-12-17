# opencl_sanbox

[Algorithm documentation link](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)

## how to launch
* build script makes automatic launch
    ```>$ cd <project folder>```
- To launch a program & hash-calculation time only:
    ```>$ sh ./build.sh```
OR
- To calculate program time & launch:
    ```>$ time ./build.sh```

## CPU result with a small input string
```
>$ time ./build.sh
>	Input message: `1234567891234567891234567891234567891234567891234567891`
	hash:	dfe655f881efc69168756ce900408823ba18906d30dd0b41ff42edce879cec03	Duration: 3467 [ns]

real	0m0,686s
user	0m0,604s
sys	0m0,082s
```
### Hash results were compared with Python `hashlib`
```
import hashhlib
>>> input_str = "1234567891234567891234567891234567891234567891234567891"
>>> hash_to_check = "dfe655f881efc69168756ce900408823ba18906d30dd0b41ff42edce879cec03"
>>> hashlib.sha256(input_str.encode('utf-8')).hexdigest() == hash_to_check
True
```

## GPU result with a small input string
```
Selected device type is CL_DEVICE_TYPE_GPU
Number of available platforms: 3
Selected platform: Intel(R) CPU Runtime for OpenCL(TM) Applications
clGetDeviceIDs() returned CL_DEVICE_NOT_FOUND for platform Intel(R) CPU Runtime for OpenCL(TM) Applications.
Selected platform: Intel(R) OpenCL HD Graphics
> Input string (hash 0):`aaaaaaaa`
> NDRange performance counter time:	1.728000 [ms]
> Hash string:`3ba3f5f43b92602683c19aee62a20342b084dd5971ddd33808d81a328879a547`
> End of calculations
```
### Hash results were compared with Python `hashlib`
```
import hashhlib
>>> input_str = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
>>> hash_to_check = "3ba3f5f43b92602683c19aee62a20342b084dd5971ddd33808d81a328879a547"
>>> hashlib.sha256(input_str.encode()).hexdigest() == hash_to_check
True
```

## CPU/GPU hash check listing with code example snippet
```
cd <project folder>
mkdir build
cd build
cmake .
make
./main </path/to>/sha256_opencl/src/sha256.cl
```

### Listing result
```
Selected device type is CL_DEVICE_TYPE_GPU
Number of available platforms: 3
Selected platform: Intel(R) CPU Runtime for OpenCL(TM) Applications
clGetDeviceIDs() returned CL_DEVICE_NOT_FOUND for platform Intel(R) CPU Runtime for OpenCL(TM) Applications.
Selected platform: Intel(R) OpenCL HD Graphics
> CPU/GPU In-msg[ 0] (0.2hex):`697665716163314c6b3079455a6643336d764c5237514e5852687265664c4176`
> CPU/GPU In-msg[ 1] (0.2hex):`305761497877504c4c58537637414f4930304a59383331514373777530444d71`
> CPU/GPU In-msg[ 2] (0.2hex):`44317a553253394c53394468504239586279724f6e5161624e716f50524b476c`
> Hash string CPU[0]:`e4f7a77dc5a411a42f8856fd42d600e40b882d4f578a154459113042971ff22e
> Hash string CPU[1]:`9009435b54c08ab301d695d1b288cff7521df823de88a0bb65d82e12bae0ed42
> Hash string CPU[2]:`fa8ac7a4182e4520c5d3c686ec88710664a3e9612724924fbc8a79cd313fcb60
	Duration: 7003 [ns]
> Hash string GPU[0]:`e4f7a77dc5a411a42f8856fd42d600e40b882d4f578a154459113042971ff22e`
> Hash string GPU[1]:`9009435b54c08ab301d695d1b288cff7521df823de88a0bb65d82e12bae0ed42`
> Hash string GPU[2]:`fa8ac7a4182e4520c5d3c686ec88710664a3e9612724924fbc8a79cd313fcb60`
	Duration: 2.155000 [ms]
> End of calculations
```