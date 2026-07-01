sample usage:

Step 1: set LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=xxx/mpp/out/lib:$LD_LIBRARY_PATH
    xxx is sdk package path.

Step 2: compile sample code
    make;

Step 3: run sample
    ./sample_svp_npu_main