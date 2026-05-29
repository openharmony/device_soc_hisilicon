Notice:
If the module parameter "mem_process_isolation" of the MMZ is set to "1", the process isolation attribute of the MMZ buffer is enabled.
When you run the sample and then run the tools, tools may fail to run due to process isolation problems. In consideration of this,
the "share all" operation is reserved in the sample, you can set "MEM_SHARE" to "y" during compilation to enable this function,
such as "make MEM_SHARE=y".
