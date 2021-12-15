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