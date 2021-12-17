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
> CPU/GPU In-msg[ 0] (0.2hex):`556a7471746c3956676c46314e59544639644f4c707935645478355945304635`
> CPU/GPU In-msg[ 1] (0.2hex):`7030705850777a42725078545077454e45554e5467543975744a694b4f496157`
> CPU/GPU In-msg[ 2] (0.2hex):`313557395a755249554256327148356349356568756f59314958754276695249`
> Hash string CPU[0]:`572f68a70ccd06e6935c934081fa407c71a1cb09569d2c19fa2bed97188dbbb3
> Hash string CPU[1]:`c88c97ea745dec26199b34f618e88f12ab46f0bd97a0760ad7a48efb7611b56a
> Hash string CPU[2]:`9d5f741df85d6b616f05e538e6d8f7447ef99f9f051073587bfa8f626962fee7
	Duration: 7124 [ns]
> Hash string GPU[0]:`572f68a70ccd06e6935c934081fa407c71a1cb09569d2c19fa2bed97188dbbb3`
> Hash string GPU[1]:`c88c97ea745dec26199b34f618e88f12ab46f0bd97a0760ad7a48efb7611b56a`
> Hash string GPU[2]:`9d5f741df85d6b616f05e538e6d8f7447ef99f9f051073587bfa8f626962fee7`
	Duration: 2.080000 [ms]
> End of calculations
> Draw table : start
+--------+----------------------------------------------------------------+----------------------------------------------------------------+----------+
|Hardware|Input string                                                    |                                                            Hash|Time, [ms]|
+--------+----------------------------------------------------------------+----------------------------------------------------------------+----------+
|cpu     |556a7471746c3956676c46314e59544639644f4c707935645478355945304635|572f68a70ccd06e6935c934081fa407c71a1cb09569d2c19fa2bed97188dbbb3|0.00281   |
+--------+----------------------------------------------------------------+----------------------------------------------------------------+----------+
|gpu     |556a7471746c3956676c46314e59544639644f4c707935645478355945304635|572f68a70ccd06e6935c934081fa407c71a1cb09569d2c19fa2bed97188dbbb3|0.693333  |
+--------+----------------------------------------------------------------+----------------------------------------------------------------+----------+
|cpu     |7030705850777a42725078545077454e45554e5467543975744a694b4f496157|c88c97ea745dec26199b34f618e88f12ab46f0bd97a0760ad7a48efb7611b56a|0.00228   |
+--------+----------------------------------------------------------------+----------------------------------------------------------------+----------+
|gpu     |7030705850777a42725078545077454e45554e5467543975744a694b4f496157|c88c97ea745dec26199b34f618e88f12ab46f0bd97a0760ad7a48efb7611b56a|0.693333  |
+--------+----------------------------------------------------------------+----------------------------------------------------------------+----------+
|cpu     |313557395a755249554256327148356349356568756f59314958754276695249|9d5f741df85d6b616f05e538e6d8f7447ef99f9f051073587bfa8f626962fee7|0.002034  |
+--------+----------------------------------------------------------------+----------------------------------------------------------------+----------+
|gpu     |313557395a755249554256327148356349356568756f59314958754276695249|9d5f741df85d6b616f05e538e6d8f7447ef99f9f051073587bfa8f626962fee7|0.693333  |
+--------+----------------------------------------------------------------+----------------------------------------------------------------+----------+
> Draw table : end

```
