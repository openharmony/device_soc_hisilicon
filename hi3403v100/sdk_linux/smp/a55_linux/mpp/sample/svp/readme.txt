如下文件因为大于50M，被压缩成多块，需要合并后解压才能使用

1、svp/ive/data/input/gmm2/gmm2_352x288_sp400_frm1000.yuv，解压命令为
cd ive/data/input/gmm2/
cat gmm2_352x288_sp400_frm1000.tar.gz.0* > gmm2_352x288_sp400_frm1000.tar.gz
tar zxf gmm2_352x288_sp400_frm1000.tar.gz

2、svp_npu/data/model/rfcn.om，解压命令为
cd svp_npu/data/model/
cat rfcn.om.tar.gz.0* > rfcn.om.tar.gz
tar zxf rfcn.om.tar.gz