# opencl_sanbox


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