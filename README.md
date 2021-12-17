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
> CPU/GPU In-msg[ 0] (0.2hex):`617665504965474d5a4337324575516666304779694a685472426b7856686b47`
> CPU/GPU In-msg[ 1] (0.2hex):`736d31367341343157375639514b4a684852614e69714f6c3138427175514177`
> CPU/GPU In-msg[ 2] (0.2hex):`325a39703130426a5a4a306174706138654d303359556e6963304e4731517653`
> CPU/GPU In-msg[ 3] (0.2hex):`45765a6256326a774470377a3456505037663169373363426944306c64786150`
> CPU/GPU In-msg[ 4] (0.2hex):`51724f6d45477a6a38676176625731483931547171394b4b4f396c3455714f69`
> CPU/GPU In-msg[ 5] (0.2hex):`6369306746445341534d4f6d657a33344d73366e486a6273414146535750534b`
> CPU/GPU In-msg[ 6] (0.2hex):`6f436d334d4b514a6e576b745267527153555072535276616558333063495a64`
> CPU/GPU In-msg[ 7] (0.2hex):`30514d5067335668454133613163485a6c416a30304f437530616438536e4d35`
> CPU/GPU In-msg[ 8] (0.2hex):`4138516d61394c786b6e4f57364d546571773172364e57533174734148324e67`
> CPU/GPU In-msg[ 9] (0.2hex):`4d445477315464626d4c54774b6a334c7a3231434f4364666f774c7970617346`
> CPU/GPU In-msg[10] (0.2hex):`5959487663666f455957653877397378576b383343525678425473356b44316f`
> CPU/GPU In-msg[11] (0.2hex):`6a78344577454f7552476139394554624e363168485458715a54686b62714b6f`
> CPU/GPU In-msg[12] (0.2hex):`7632517a696b7263473233566a75624d797255786271786831416c6f31687777`
> Hash string CPU[0]:`08e8de446a2e9616591741784d2dafc1b0873e95d5c4f7fe7f189d284a519074
	Duration: % [ns]
> Hash string CPU[1]:`a18ccea758360089a80fd36762bac2093467fdd825da7daee4032ed7982394da
	Duration: % [ns]
> Hash string CPU[2]:`99276711c8a45cd68e40cf990ddb4ee1ddac825809ccd32653ed5bbba55d0874
	Duration: % [ns]
> Hash string CPU[3]:`fd5785fea86e166d5a670ec5737b3d16007a08ac74bc5c14778a57793bb18dc2
	Duration: % [ns]
> Hash string CPU[4]:`b7f2924ed344f79eb65554495806e7efaacfc39758b10ecdb2a2ca366b5caec0
	Duration: % [ns]
> Hash string CPU[5]:`2b5b30f74e0738b0c8204df3922f50ca6e64290dafae906c3cae4f7e7d5e67c7
	Duration: % [ns]
> Hash string CPU[6]:`96d71d3fc13965d1b8b998ecaa572d03d28df4f54b09502a79ba762cd9ab88af
	Duration: % [ns]
> Hash string CPU[7]:`1c88620da3d2da44772e8e1a6f1a92a594c179d47847adfdff346377cffefbc8
	Duration: % [ns]
> Hash string CPU[8]:`43d22fbcd3f88645b9517a066149cd7f947c9913c203179a32183738459e5dae
	Duration: % [ns]
> Hash string CPU[9]:`06b9d76acdf5b248d96384a8fb4594a000454a3c8c55674aba3934d5c5e6bc0c
	Duration: % [ns]
> Hash string CPU[10]:`ad81d9e2df9788ff8f28e290d56006cfbe8fbf7ae2538e34ee6f6ba4dbfa8522
	Duration: % [ns]
> Hash string CPU[11]:`f755fd6ba6847e80e351e04557f7b868a5d9156164904c1f030479d38662c5f0
	Duration: % [ns]
> Hash string CPU[12]:`a76185027e278d47642fdf67fb09f4e75280c946e7a95a2ad153e5b7ac078b98
	Duration: % [ns]
> Hash string GPU[0]:`08e8de446a2e9616591741784d2dafc1b0873e95d5c4f7fe7f189d284a519074`
> Hash string GPU[1]:`a18ccea758360089a80fd36762bac2093467fdd825da7daee4032ed7982394da`
> Hash string GPU[2]:`99276711c8a45cd68e40cf990ddb4ee1ddac825809ccd32653ed5bbba55d0874`
> Hash string GPU[3]:`fd5785fea86e166d5a670ec5737b3d16007a08ac74bc5c14778a57793bb18dc2`
> Hash string GPU[4]:`b7f2924ed344f79eb65554495806e7efaacfc39758b10ecdb2a2ca366b5caec0`
> Hash string GPU[5]:`2b5b30f74e0738b0c8204df3922f50ca6e64290dafae906c3cae4f7e7d5e67c7`
> Hash string GPU[6]:`96d71d3fc13965d1b8b998ecaa572d03d28df4f54b09502a79ba762cd9ab88af`
> Hash string GPU[7]:`1c88620da3d2da44772e8e1a6f1a92a594c179d47847adfdff346377cffefbc8`
> Hash string GPU[8]:`43d22fbcd3f88645b9517a066149cd7f947c9913c203179a32183738459e5dae`
> Hash string GPU[9]:`06b9d76acdf5b248d96384a8fb4594a000454a3c8c55674aba3934d5c5e6bc0c`
> Hash string GPU[10]:`ad81d9e2df9788ff8f28e290d56006cfbe8fbf7ae2538e34ee6f6ba4dbfa8522`
> Hash string GPU[11]:`f755fd6ba6847e80e351e04557f7b868a5d9156164904c1f030479d38662c5f0`
> Hash string GPU[12]:`a76185027e278d47642fdf67fb09f4e75280c946e7a95a2ad153e5b7ac078b98`
	Duration: 1.819000 [ms]
> End of calculations
```