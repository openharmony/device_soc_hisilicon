1. The kernel directory must be specified for KERN_DIR in the built-in.sh file.
2. The mbedtls directory must be specified for MBEDTLS_DIR in the built-in.sh file.
2. The CIPHER module depends on basic ko files such as osal, sysconfig, base, and sys. The *.c files of CIPHER can be copied to the specified directory only after adaptation.
