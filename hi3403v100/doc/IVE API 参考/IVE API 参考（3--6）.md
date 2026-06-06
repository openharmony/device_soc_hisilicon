# 数据类型和数据结构<a name="ZH-CN_TOPIC_0000002503971229"></a>

SVP相关数据类型、数据结构定义如下：

-   [ot\_svp\_img\_type](ot_svp_img_type.md)：定义二维广义图像支持的图像类型。
-   [ot\_svp\_img](ot_svp_img.md)：定义二维广义图像信息。
-   [ot\_svp\_src\_img](ot_svp_src_img.md)：定义源图像。
-   [ot\_svp\_dst\_img](ot_svp_dst_img.md)：定义输出图像。
-   [OT\_SVP\_IMG\_ADDR\_NUM](OT_SVP_IMG_ADDR_NUM.md)：定义地址通道数目。

定点数据类型：

-   [ot\_svp\_data](ot_svp_data.md)：定义以byte为单位的二维图像信息。
-   [ot\_svp\_src\_data](ot_svp_src_data.md)：定义以byte为单位的二维源数据信息。
-   [ot\_svp\_dst\_data](ot_svp_dst_data.md)：定义byte为单位的二维输出数据信息。
-   [ot\_svp\_8bit](ot_svp_8bit.md)：定义8bit数据联合体。
-   [ot\_svp\_point\_u16](ot_svp_point_u16.md)：定义u16bit的点信息结构体。
-   [ot\_svp\_point\_s16](ot_svp_point_s16.md)：定义s16bit的点信息结构体。
-   [ot\_svp\_point\_s25q7](ot_svp_point_s25q7.md)：定义s25q7表示的点信息结构体。
-   [ot\_svp\_point\_u14q2](ot_svp_point_u14q2.md)：定义u14q2表示的点信息结构体。
-   [ot\_svp\_rect\_u32](ot_svp_rect_u32.md)：定义u16表示的矩形信息结构体。
-   [ot\_svp\_rect\_u16](ot_svp_rect_u16.md)：定义u32表示的矩形信息结构体。
-   [ot\_svp\_rect\_s24q8](ot_svp_rect_s24q8.md)：定义s24q8表示的矩形信息结构体。
-   [ot\_svp\_lut](ot_svp_lut.md)：定义查找表结构体。

IVE相关数据类型、数据结构定义如下：

-   [ot\_ive\_handle](ot_ive_handle.md)：定义IVE句柄。
-   [OT\_IVE\_HIST\_NUM](OT_IVE_HIST_NUM.md)：定义直方图统计bin数目。
-   [OT\_IVE\_MAP\_NUM](OT_IVE_MAP_NUM.md)：定义映射查找表项数目。
-   [OT\_IVE\_MAX\_RGN\_NUM](OT_IVE_MAX_RGN_NUM.md)：定义最大连通区域数目。
-   [OT\_IVE\_ST\_MAX\_CORNER\_NUM](OT_IVE_ST_MAX_CORNER_NUM.md)：定义Shi-Tomasi-like角点最大数目。
-   [OT\_IVE\_MASK\_NUM](OT_IVE_MASK_NUM.md)：掩码mask数组长度
-   [OT\_IVE\_ARR\_RESERVED\_NUM\_TWO](OT_IVE_ARR_RESERVED_NUM_TWO.md)：保留字段数组长度2
-   [OT\_IVE\_ARR\_RESERVED\_NUM\_THREE](OT_IVE_ARR_RESERVED_NUM_THREE.md)：保留字段数组长度3
-   [OT\_IVE\_ARR\_RESERVED\_NUM\_EIGHT](OT_IVE_ARR_RESERVED_NUM_EIGHT.md)：保留字段数组长度8
-   [OT\_IVE\_ARR\_RESERVED\_NUM\_TWELVE](OT_IVE_ARR_RESERVED_NUM_TWELVE.md)：保留字段数组长度12
-   [OT\_IVE\_ARR\_RESERVED\_NUM\_FOURTEEN](OT_IVE_ARR_RESERVED_NUM_FOURTEEN.md)：保留字段数组长度14
-   [OT\_IVE\_ARR\_NUM\_THREE](OT_IVE_ARR_NUM_THREE.md)：数组长度3
-   [OT\_IVE\_ARR\_NUM\_EIGHT](OT_IVE_ARR_NUM_EIGHT.md)：数组长度8
-   [OT\_IVE\_DEV\_NAME\_LENGTH](OT_IVE_DEV_NAME_LENGTH.md)：IVE设备名字的长度。
-   [OT\_IVE\_DEV\_DEFAULT\_NODE\_NUM](OT_IVE_DEV_DEFAULT_NODE_NUM.md)：默认的IVE节点个数。
-   [ot\_ive\_mod\_param](ot_ive_mod_param.md)：IVE模块相关参数定义。
-   [ot\_ive\_err\_code](ot_ive_err_code.md)：定义错误码。
-   [ot\_ive\_dma\_mode](ot_ive_dma_mode.md)：定义DMA运算模式。
-   [ot\_ive\_dma\_ctrl](ot_ive_dma_ctrl.md)：定义DMA控制信息。
-   [ot\_ive\_filter\_ctrl](ot_ive_filter_ctrl.md)：定义模板滤波控制信息。
-   [ot\_ive\_csc\_mode](ot_ive_csc_mode.md)：定义色彩空间转换模式。
-   [ot\_ive\_csc\_ctrl](ot_ive_csc_ctrl.md)：定义色彩空间转换控制信息。
-   [ot\_ive\_filter\_and\_csc\_ctrl](ot_ive_filter_and_csc_ctrl.md)：定义模板滤波加色彩空间转换复合功能控制信息。
-   [ot\_ive\_sobel\_out\_ctrl](ot_ive_sobel_out_ctrl.md)：定义sobel输出控制信息。
-   [ot\_ive\_sobel\_ctrl](ot_ive_sobel_ctrl.md)：定义sobel边缘提取控制信息。
-   [ot\_ive\_mag\_and\_ang\_out\_ctrl](ot_ive_mag_and_ang_out_ctrl.md)：定义canny边缘幅值与角度计算的输出格式。
-   [ot\_ive\_mag\_and\_ang\_ctrl](ot_ive_mag_and_ang_ctrl.md)：定义canny边缘幅值和幅角计算的控制信息。
-   [ot\_ive\_dilate\_ctrl](ot_ive_dilate_ctrl.md)：定义膨胀控制信息。
-   [ot\_ive\_erode\_ctrl](ot_ive_erode_ctrl.md)：定义腐蚀控制信息。
-   [ot\_ive\_threshold\_mode](ot_ive_threshold_mode.md)：定义图像二值化输出格式。
-   [ot\_ive\_threshold\_ctrl](ot_ive_threshold_ctrl.md)：定义图像二值化控制信息。
-   [ot\_ive\_sub\_mode](ot_ive_sub_mode.md)：定义两图像相减输出格式。
-   [ot\_ive\_sub\_ctrl](ot_ive_sub_ctrl.md)：定义两图像相减控制参数。
-   [ot\_ive\_integ\_out\_ctrl](ot_ive_integ_out_ctrl.md)：定义积分图输出控制参数。
-   [ot\_ive\_integ\_ctrl](ot_ive_integ_ctrl.md)：定义积分图计算控制参数。
-   [ot\_ive\_threshold\_s16\_mode](ot_ive_threshold_s16_mode.md)：定义16bit有符号图像的阈值化模式。
-   [ot\_ive\_threshold\_s16\_ctrl](ot_ive_threshold_s16_ctrl.md)：定义16bit有符号图像的阈值化控制参数。
-   [ot\_ive\_threshold\_u16\_mode](ot_ive_threshold_u16_mode.md)：定义16bti无符号图像的阈值化模式。
-   [ot\_ive\_threshold\_u16\_ctrl](ot_ive_threshold_u16_ctrl.md)：定义16bit无符号图像的阈值化控制参数。
-   [ot\_ive\_16bit\_to\_8bit\_mode](ot_ive_16bit_to_8bit_mode.md)：定义16bit图像到8bit图像的转化模式。
-   [ot\_ive\_16bit\_to\_8bit\_ctrl](ot_ive_16bit_to_8bit_ctrl.md)：定义16bit图像到8bit图像的转化控制参数。
-   [ot\_ive\_order\_stats\_filter\_mode](ot_ive_order_stats_filter_mode.md)：定义顺序统计量滤波模式。
-   [ot\_ive\_order\_stats\_filter\_ctrl](ot_ive_order_stats_filter_ctrl.md)：定义顺序统计量滤波控制参数。
-   [ot\_ive\_map\_u8bit\_lut\_mem](ot_ive_map_u8bit_lut_mem.md)：定义Map U8C1→U8C1的查找表内存。
-   [ot\_ive\_map\_u16bit\_lut\_mem](ot_ive_map_u16bit_lut_mem.md)：定义Map U8C1→U16C1的查找表内存。
-   [ot\_ive\_map\_s16bit\_lut\_mem](ot_ive_map_s16bit_lut_mem.md)：定义Map U8C1→S16C1的查找表内存。
-   [ot\_ive\_map\_mode](ot_ive_map_mode.md)：定义Map的模式。
-   [ot\_ive\_map\_ctrl](ot_ive_map_ctrl.md)：定义Map控制参数。
-   [ot\_ive\_equalize\_hist\_ctrl\_mem](ot_ive_equalize_hist_ctrl_mem.md)：定义直方图均衡化辅助内存。
-   [ot\_ive\_equalize\_hist\_ctrl](ot_ive_equalize_hist_ctrl.md)：定义直方图均衡化控制参数。
-   [ot\_ive\_add\_ctrl](ot_ive_add_ctrl.md)：定义两图像的加权加控制参数。
-   [ot\_ive\_ncc\_dst\_mem](ot_ive_ncc_dst_mem.md)：定义NCC的输出内存信息。
-   [ot\_ive\_rgn](ot_ive_rgn.md)：定义连通区域信息。
-   [ot\_ive\_ccblob](ot_ive_ccblob.md)：定义连通区域标记的输出信息。
-   [ot\_ive\_ccl\_mode](ot_ive_ccl_mode.md)：定义连通区域模式。
-   [ot\_ive\_ccl\_ctrl](ot_ive_ccl_ctrl.md)：定义连通区域标记控制参数。
-   [ot\_ive\_gmm\_ctrl](ot_ive_gmm_ctrl.md)：定义GMM背景建模的控制参数。
-   [ot\_ive\_gmm2\_sns\_factor\_mode](ot_ive_gmm2_sns_factor_mode.md)：定义灵敏度系数模式。
-   [ot\_ive\_gmm2\_life\_update\_factor\_mode](ot_ive_gmm2_life_update_factor_mode.md)：定义模型时长参数更新模式。
-   [ot\_ive\_gmm2\_ctrl](ot_ive_gmm2_ctrl.md)：定义GMM2背景建模的控制参数。
-   [ot\_ive\_canny\_stack\_size](ot_ive_canny_stack_size.md)：定义Canny边缘前半部分计算时强边缘点栈大小结构体。
-   [ot\_ive\_canny\_hys\_edge\_ctrl](ot_ive_canny_hys_edge_ctrl.md)：定义Canny边缘前半部分计算任务的控制参数。
-   [ot\_ive\_lbp\_compare\_mode](ot_ive_lbp_compare_mode.md)：定义LBP纹理计算控制参数。
-   [ot\_ive\_lbp\_ctrl](ot_ive_lbp_ctrl.md)：定义LBP纹理计算控制参数。
-   [ot\_ive\_norm\_grad\_out\_ctrl](ot_ive_norm_grad_out_ctrl.md)：定义归一化梯度信息计算任务输出控制枚举类型。
-   [ot\_ive\_norm\_grad\_ctrl](ot_ive_norm_grad_ctrl.md)：定义归一化梯度信息计算控制参数。
-   [ot\_ive\_lk\_optical\_flow\_pyr\_out\_mode](ot_ive_lk_optical_flow_pyr_out_mode.md)：定义金字塔LK光流计算输出模式。
-   [ot\_ive\_lk\_optical\_flow\_pyr\_ctrl](ot_ive_lk_optical_flow_pyr_ctrl.md)：定义金字塔LK光流计算控制参数。
-   [ot\_ive\_st\_max\_eig\_val](ot_ive_st_max_eig_val.md)：定义Shi-Tomas-like角点计算时最大角点响应值结构体。
-   [ot\_ive\_st\_cand\_corner\_ctrl](ot_ive_st_cand_corner_ctrl.md)：定义Shi-Tomas-like候选角点计算控制参数。
-   [ot\_ive\_st\_corner\_info](ot_ive_st_corner_info.md)：定义Shi-Tomas-like角点计算输出的角点信息结构体。
-   [ot\_ive\_st\_corner\_ctrl](ot_ive_st_corner_ctrl.md)：定义Shi-Tomas-like角点筛选控制参数。
-   [ot\_ive\_sad\_mode](ot_ive_sad_mode.md)：定义SAD计算模式。
-   [ot\_ive\_sad\_out\_ctrl](ot_ive_sad_out_ctrl.md)：定义SAD输出控制模式。
-   [ot\_ive\_sad\_ctrl](ot_ive_sad_ctrl.md)：定义SAD控制参数。
-   [ot\_ive\_resize\_mode](ot_ive_resize_mode.md)：定义Resize的模式。
-   [ot\_ive\_resize\_ctrl](ot_ive_resize_ctrl.md)：定义Resize控制参数。
-   [ot\_ive\_grad\_fg\_mode](ot_ive_grad_fg_mode.md)：定义梯度前景计算模式。
-   [ot\_ive\_grad\_fg\_ctrl](ot_ive_grad_fg_ctrl.md)：定义计算梯度前景控制参数。
-   [ot\_ive\_cand\_bg\_pixel](ot_ive_cand_bg_pixel.md)：定义候选背景模型数据。
-   [ot\_ive\_wrok\_bg\_pixel](ot_ive_wrok_bg_pixel.md)：定义工作背景模型数据。
-   [ot\_ive\_bg\_life](ot_ive_bg_life.md)：定义背景生命力数据。
-   [ot\_ive\_bg\_model\_pixel](ot_ive_bg_model_pixel.md)：定义背景模型数据。
-   [ot\_ive\_fg\_status\_data](ot_ive_fg_status_data.md)：定义前景状态数据。
-   [ot\_ive\_bg\_status\_data](ot_ive_bg_status_data.md)：定义背景状态数据。
-   [ot\_ive\_match\_bg\_model\_ctrl](ot_ive_match_bg_model_ctrl.md)：定义背景匹配控制参数。
-   [ot\_ive\_update\_bg\_model\_ctrl](ot_ive_update_bg_model_ctrl.md)：定义背景更新控制参数。
-   [ot\_ive\_ann\_mlp\_accurate](ot_ive_ann_mlp_accurate.md)：定义ann\_mlp输入特征向量类型。
-   [ot\_ive\_ann\_mlp\_actv\_func](ot_ive_ann_mlp_actv_func.md)：定义ann\_mlp激活函数枚举类型。
-   [ot\_ive\_ann\_mlp\_model](ot_ive_ann_mlp_model.md)：定义ann\_mlp模型数据结构体。
-   [ot\_ive\_svm\_type](ot_ive_svm_type.md)：定义SVM类型。
-   [ot\_ive\_svm\_kernel\_type](ot_ive_svm_kernel_type.md)：定义SVM核函数类型。
-   [ot\_ive\_svm\_model](ot_ive_svm_model.md)：定义SVM模型数据结构体。
-   [ot\_ive\_cnn\_actv\_func](ot_ive_cnn_actv_func.md)：定义CNN激活函数枚举类型。
-   [ot\_ive\_cnn\_pooling](ot_ive_cnn_pooling.md)：定义CNN汇聚操作枚举类型。
-   [ot\_ive\_cnn\_conv\_pooling](ot_ive_cnn_conv_pooling.md)：定义CNN单层Conv-ReLU-Pooling运算包参数结构体。
-   [ot\_ive\_cnn\_fc\_info](ot_ive_cnn_fc_info.md)：定义CNN全链接网络参数结构体。
-   [ot\_ive\_cnn\_model](ot_ive_cnn_model.md)：定义CNN模型参数结构体。
-   [ot\_ive\_cnn\_ctrl](ot_ive_cnn_ctrl.md)：定义CNN预测任务的控制参数。
-   [ot\_ive\_cnn\_result](ot_ive_cnn_result.md)：定义CNN单个样本预测结果结构体。
-   [ot\_ive\_persp\_trans\_point\_pair](ot_ive_persp_trans_point_pair.md)：定义透视变换点对结构体。
-   [ot\_ive\_persp\_trans\_alg\_mode](ot_ive_persp_trans_alg_mode.md)：定义透视变换算法模式枚举。
-   [ot\_ive\_persp\_trans\_csc\_mode](ot_ive_persp_trans_csc_mode.md)：定义透视变换色彩空间转换模式。
-   [ot\_ive\_kcf\_core\_id](ot_ive_kcf_core_id.md)：定义KCF的核ID。
-   [ot\_ive\_persp\_trans\_ctrl](ot_ive_persp_trans_ctrl.md)：定义透视变换控制参数。
-   [ot\_ive\_roi\_info](ot_ive_roi_info.md)：定义目前区域信息参数。
-   [ot\_ive\_kcf\_proc\_ctrl](ot_ive_kcf_proc_ctrl.md)：定义跟踪处理控制参数。
-   [ot\_ive\_list\_head](ot_ive_list_head.md)：定义链表头结构体参数。
-   [ot\_ive\_kcf\_obj](ot_ive_kcf_obj.md)：定义目标信息结构体参数。
-   [ot\_ive\_kcf\_obj\_node](ot_ive_kcf_obj_node.md)：定义目标链表节点参数。
-   [ot\_ive\_kcf\_list\_state](ot_ive_kcf_list_state.md)：定义目标链表状态枚举类型。
-   [ot\_ive\_kcf\_obj\_list](ot_ive_kcf_obj_list.md)：定义目标链表结构参数。
-   [ot\_ive\_kcf\_bbox](ot_ive_kcf_bbox.md)：定义目标区域信息参数。
-   [ot\_ive\_kcf\_bbox\_ctrl](ot_ive_kcf_bbox_ctrl.md)：定义目标区域信息控制参数。
-   [ot\_ive\_hog\_mode](ot_ive_hog_mode.md)：定义HOG\(Histogram of Oriented Gradient\)特征存储模式枚举类型。
-   [ot\_ive\_hog\_ctrl](ot_ive_hog_ctrl.md)：定义计算HOG\(Histogram of Oriented Gradient\)特征控制参数。




## SVP相关数据类型、数据结构<a name="ZH-CN_TOPIC_0000002471091256"></a>







### ot\_svp\_img\_type<a name="ZH-CN_TOPIC_0000002471091210"></a>

【说明】

定义二维广义图像支持的图像类型。

【定义】

```
/* Img type */
typedef enum {
    OT_SVP_IMG_TYPE_U8C1           =  0x0,
    OT_SVP_IMG_TYPE_S8C1           =  0x1,
 
    OT_SVP_IMG_TYPE_YUV420SP       =  0x2,       /* YUV420 SemiPlanar */
    OT_SVP_IMG_TYPE_YUV422SP       =  0x3,       /* YUV422 SemiPlanar */
    OT_SVP_IMG_TYPE_YUV420P        =  0x4,       /* YUV420 Planar */
    OT_SVP_IMG_TYPE_YUV422P        =  0x5,       /* YUV422 planar */
 
    OT_SVP_IMG_TYPE_S8C2_PACKAGE   =  0x6,
    OT_SVP_IMG_TYPE_S8C2_PLANAR    =  0x7,
 
    OT_SVP_IMG_TYPE_S16C1          =  0x8,
    OT_SVP_IMG_TYPE_U16C1          =  0x9,
 
    OT_SVP_IMG_TYPE_U8C3_PACKAGE   =  0xa,
    OT_SVP_IMG_TYPE_U8C3_PLANAR    =  0xb,
 
    OT_SVP_IMG_TYPE_S32C1          =  0xc,
    OT_SVP_IMG_TYPE_U32C1          =  0xd,
 
    OT_SVP_IMG_TYPE_S64C1          =  0xe,
    OT_SVP_IMG_TYPE_U64C1          =  0xf,
    OT_SVP_IMG_TYPE_BUTT
 
} ot_svp_img_type;
```

【成员】

<a name="table15961mcpsimp"></a>
<table><thead align="left"><tr id="row15966mcpsimp"><th class="cellrowborder" valign="top" width="43%" id="mcps1.1.3.1.1"><p id="p15968mcpsimp"><a name="p15968mcpsimp"></a><a name="p15968mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="56.99999999999999%" id="mcps1.1.3.1.2"><p id="p15970mcpsimp"><a name="p15970mcpsimp"></a><a name="p15970mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row15972mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p15974mcpsimp"><a name="p15974mcpsimp"></a><a name="p15974mcpsimp"></a>OT_SVP_IMG_TYPE_U8C1</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p15976mcpsimp"><a name="p15976mcpsimp"></a><a name="p15976mcpsimp"></a>每个像素用1个8bit无符号数据表示的单通道图像。请参见OT_SVP_IMG_TYPE_U8C1 \ OT_SVP_IMG_TYPE_S8C1 \ OT_SVP_IMG_TYPE_S16C1 \ OT_SVP_IMG_TYPE_U16C1 \ OT_SVP_IMG_TYPE_S32C1 \ OT_SVP_IMG_TYPE_U32C1 \ OT_SVP_IMG_TYPE_S64C1 \ OT_SVP_IMG_TYPE_U64C1 类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row15978mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p15980mcpsimp"><a name="p15980mcpsimp"></a><a name="p15980mcpsimp"></a>OT_SVP_IMG_TYPE_S8C1</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p15982mcpsimp"><a name="p15982mcpsimp"></a><a name="p15982mcpsimp"></a>每个像素用1个8bit有符号数据表示的单通道图像。请参见 OT_SVP_IMG_TYPE_U8C1 \ OT_SVP_IMG_TYPE_S8C1 \ OT_SVP_IMG_TYPE_S16C1 \ OT_SVP_IMG_TYPE_U16C1 \ OT_SVP_IMG_TYPE_S32C1 \ OT_SVP_IMG_TYPE_U32C1 \ OT_SVP_IMG_TYPE_S64C1 \ OT_SVP_IMG_TYPE_U64C1 类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row15984mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p15986mcpsimp"><a name="p15986mcpsimp"></a><a name="p15986mcpsimp"></a>OT_SVP_IMG_TYPE_YUV420SP</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p15988mcpsimp"><a name="p15988mcpsimp"></a><a name="p15988mcpsimp"></a>YUV420 Semiplanar格式的图像。</p>
<p id="p15989mcpsimp"><a name="p15989mcpsimp"></a><a name="p15989mcpsimp"></a>请参见OT_SVP_IMG_TYPE_YUV420SP类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row15991mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p15993mcpsimp"><a name="p15993mcpsimp"></a><a name="p15993mcpsimp"></a>OT_SVP_IMG_TYPE_YUV422SP</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p15995mcpsimp"><a name="p15995mcpsimp"></a><a name="p15995mcpsimp"></a>YUV422 Semiplanar格式的图像。</p>
<p id="p15996mcpsimp"><a name="p15996mcpsimp"></a><a name="p15996mcpsimp"></a>请参见OT_SVP_IMG_TYPE_YUV422SP类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row15998mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16000mcpsimp"><a name="p16000mcpsimp"></a><a name="p16000mcpsimp"></a>OT_SVP_IMG_TYPE_YUV420P</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16002mcpsimp"><a name="p16002mcpsimp"></a><a name="p16002mcpsimp"></a>YUV420 Planar格式的图像。请参见OT_SVP_IMG_TYPE_YUV420P类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16004mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16006mcpsimp"><a name="p16006mcpsimp"></a><a name="p16006mcpsimp"></a>OT_SVP_IMG_TYPE_YUV422P</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16008mcpsimp"><a name="p16008mcpsimp"></a><a name="p16008mcpsimp"></a>YUV422 Planar格式的图像。请参见OT_SVP_IMG_TYPE_YUV422P类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16010mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16012mcpsimp"><a name="p16012mcpsimp"></a><a name="p16012mcpsimp"></a>OT_SVP_IMG_TYPE_S8C2_PACKAGE</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16014mcpsimp"><a name="p16014mcpsimp"></a><a name="p16014mcpsimp"></a>每个像素用2个8bit有符号数据表示，且以package格式存储2通道图像。</p>
<p id="p16015mcpsimp"><a name="p16015mcpsimp"></a><a name="p16015mcpsimp"></a>请参见OT_SVP_IMG_TYPE_S8C2_PACKAGE类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16017mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16019mcpsimp"><a name="p16019mcpsimp"></a><a name="p16019mcpsimp"></a>OT_SVP_IMG_TYPE_S8C2_PLANAR</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16021mcpsimp"><a name="p16021mcpsimp"></a><a name="p16021mcpsimp"></a>每个像素用2个8bit有符号数据表示，且以planar格式存储2通道图像。</p>
<p id="p16022mcpsimp"><a name="p16022mcpsimp"></a><a name="p16022mcpsimp"></a>请参见OT_SVP_IMG_TYPE_S8C2_PLANAR类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16024mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16026mcpsimp"><a name="p16026mcpsimp"></a><a name="p16026mcpsimp"></a>OT_SVP_IMG_TYPE_S16C1</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16028mcpsimp"><a name="p16028mcpsimp"></a><a name="p16028mcpsimp"></a>每个像素用1个16bit有符号数据表示单通道图像。请参见OT_SVP_IMG_TYPE_U8C1 \ OT_SVP_IMG_TYPE_S8C1 \ OT_SVP_IMG_TYPE_S16C1 \ OT_SVP_IMG_TYPE_U16C1 \ OT_SVP_IMG_TYPE_S32C1 \ OT_SVP_IMG_TYPE_U32C1 \ OT_SVP_IMG_TYPE_S64C1 \ OT_SVP_IMG_TYPE_U64C1 类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16030mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16032mcpsimp"><a name="p16032mcpsimp"></a><a name="p16032mcpsimp"></a>OT_SVP_IMG_TYPE_U16C1</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16034mcpsimp"><a name="p16034mcpsimp"></a><a name="p16034mcpsimp"></a>每个像素用1个16bit无符号数据表示单通道图像。请参见 OT_SVP_IMG_TYPE_U8C1 \ OT_SVP_IMG_TYPE_S8C1 \ OT_SVP_IMG_TYPE_S16C1 \ OT_SVP_IMG_TYPE_U16C1 \ OT_SVP_IMG_TYPE_S32C1 \ OT_SVP_IMG_TYPE_U32C1 \ OT_SVP_IMG_TYPE_S64C1 \ OT_SVP_IMG_TYPE_U64C1 类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16036mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16038mcpsimp"><a name="p16038mcpsimp"></a><a name="p16038mcpsimp"></a>OT_SVP_IMG_TYPE_U8C3_PACKAGE</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16040mcpsimp"><a name="p16040mcpsimp"></a><a name="p16040mcpsimp"></a>每个像素用3个8bit无符号数据表示且以Package格式存储3通道图像。</p>
<p id="p16041mcpsimp"><a name="p16041mcpsimp"></a><a name="p16041mcpsimp"></a>请参见OT_SVP_IMG_TYPE_U8C3_PACKAGE类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16043mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16045mcpsimp"><a name="p16045mcpsimp"></a><a name="p16045mcpsimp"></a>OT_SVP_IMG_TYPE_U8C3_PLANAR</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16047mcpsimp"><a name="p16047mcpsimp"></a><a name="p16047mcpsimp"></a>每个像素用3个8bit无符号数据表示1个像素的3通道图像，且以planar格式存储。</p>
<p id="p16048mcpsimp"><a name="p16048mcpsimp"></a><a name="p16048mcpsimp"></a>请参见 OT_SVP_IMG_TYPE_U8C3_PLANAR类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16050mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16052mcpsimp"><a name="p16052mcpsimp"></a><a name="p16052mcpsimp"></a>OT_SVP_IMG_TYPE_S32C1</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16054mcpsimp"><a name="p16054mcpsimp"></a><a name="p16054mcpsimp"></a>每个像素用1个32bit有符号数据表示单通道图像。请参见OT_SVP_IMG_TYPE_U8C1 \ OT_SVP_IMG_TYPE_S8C1 \ OT_SVP_IMG_TYPE_S16C1 \ OT_SVP_IMG_TYPE_U16C1 \ OT_SVP_IMG_TYPE_S32C1 \ OT_SVP_IMG_TYPE_U32C1 \ OT_SVP_IMG_TYPE_S64C1 \ OT_SVP_IMG_TYPE_U64C1 类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16056mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16058mcpsimp"><a name="p16058mcpsimp"></a><a name="p16058mcpsimp"></a>OT_SVP_IMG_TYPE_U32C1</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16060mcpsimp"><a name="p16060mcpsimp"></a><a name="p16060mcpsimp"></a>每个像素用1个32bit无符号数据表示单通道图像。请参见 OT_SVP_IMG_TYPE_U8C1 \ OT_SVP_IMG_TYPE_S8C1 \ OT_SVP_IMG_TYPE_S16C1 \ OT_SVP_IMG_TYPE_U16C1 \ OT_SVP_IMG_TYPE_S32C1 \ OT_SVP_IMG_TYPE_U32C1 \ OT_SVP_IMG_TYPE_S64C1 \ OT_SVP_IMG_TYPE_U64C1 类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16062mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16064mcpsimp"><a name="p16064mcpsimp"></a><a name="p16064mcpsimp"></a>OT_SVP_IMG_TYPE_S64C1</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16066mcpsimp"><a name="p16066mcpsimp"></a><a name="p16066mcpsimp"></a>每个像素用1个64bit有符号数据表示单通道图像。请参见OT_SVP_IMG_TYPE_U8C1 \ OT_SVP_IMG_TYPE_S8C1 \ OT_SVP_IMG_TYPE_S16C1 \ OT_SVP_IMG_TYPE_U16C1 \ OT_SVP_IMG_TYPE_S32C1 \ OT_SVP_IMG_TYPE_U32C1 \ OT_SVP_IMG_TYPE_S64C1 \ OT_SVP_IMG_TYPE_U64C1 类型的ot_svp_img图像图。</p>
</td>
</tr>
<tr id="row16068mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16070mcpsimp"><a name="p16070mcpsimp"></a><a name="p16070mcpsimp"></a>OT_SVP_IMG_TYPE_U64C1</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16072mcpsimp"><a name="p16072mcpsimp"></a><a name="p16072mcpsimp"></a>每个像素用1个64bit无符号数据表示单通道图像。请参见 OT_SVP_IMG_TYPE_U8C1 \ OT_SVP_IMG_TYPE_S8C1 \ OT_SVP_IMG_TYPE_S16C1 \ OT_SVP_IMG_TYPE_U16C1 \ OT_SVP_IMG_TYPE_S32C1 \ OT_SVP_IMG_TYPE_U32C1 \ OT_SVP_IMG_TYPE_S64C1 \ OT_SVP_IMG_TYPE_U64C1 类型的ot_svp_img图像图。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_svp\_img](#ot_svp_img)
-   [ot\_svp\_src\_img](#ot_svp_src_img)
-   [ot\_svp\_dst\_img](#ot_svp_dst_img)

### ot\_svp\_img<a name="ZH-CN_TOPIC_0000002504091103"></a>

【说明】

定义二维广义图像信息。

【定义】

```
typedef struct {
    td_u64  phys_addr[OT_SVP_IMG_ADDR_NUM]; /* RW;The physical address of the image */
    td_u64  virt_addr[OT_SVP_IMG_ADDR_NUM]; /* RW;The virtual address of the image */
    td_u32  stride[OT_SVP_IMG_STRIDE_NUM]; /* RW;The stride of the image */
    td_u32  width;                           /* RW;The width of the image */
    td_u32  height;                          /* RW;The height of the image */
    ot_svp_img_type  type;                   /* RW;The type of the image */
} ot_svp_img;
```

【成员】

<a name="table7778mcpsimp"></a>
<table><thead align="left"><tr id="row7783mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p7785mcpsimp"><a name="p7785mcpsimp"></a><a name="p7785mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p7787mcpsimp"><a name="p7787mcpsimp"></a><a name="p7787mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7789mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p7791mcpsimp"><a name="p7791mcpsimp"></a><a name="p7791mcpsimp"></a><span xml:lang="da-DK" id="ph7792mcpsimp"><a name="ph7792mcpsimp"></a><a name="ph7792mcpsimp"></a>phys_addr</span>[<a href="OT_SVP_IMG_ADDR_NUM.md">OT_SVP_IMG_ADDR_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p7795mcpsimp"><a name="p7795mcpsimp"></a><a name="p7795mcpsimp"></a>广义图像的物理地址数组。</p>
</td>
</tr>
<tr id="row7796mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p7798mcpsimp"><a name="p7798mcpsimp"></a><a name="p7798mcpsimp"></a><span xml:lang="da-DK" id="ph7799mcpsimp"><a name="ph7799mcpsimp"></a><a name="ph7799mcpsimp"></a>virt_addr</span>[<a href="OT_SVP_IMG_ADDR_NUM.md">OT_SVP_IMG_ADDR_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p7802mcpsimp"><a name="p7802mcpsimp"></a><a name="p7802mcpsimp"></a>广义图像的虚拟地址数组。</p>
</td>
</tr>
<tr id="row7803mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p7805mcpsimp"><a name="p7805mcpsimp"></a><a name="p7805mcpsimp"></a><span xml:lang="da-DK" id="ph7806mcpsimp"><a name="ph7806mcpsimp"></a><a name="ph7806mcpsimp"></a>stride</span>[<a href="OT_SVP_IMG_STRIDE_NUM.md">OT_SVP_IMG_STRIDE_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p7809mcpsimp"><a name="p7809mcpsimp"></a><a name="p7809mcpsimp"></a>广义图像的跨度。</p>
</td>
</tr>
<tr id="row7810mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p7812mcpsimp"><a name="p7812mcpsimp"></a><a name="p7812mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p7814mcpsimp"><a name="p7814mcpsimp"></a><a name="p7814mcpsimp"></a>广义图像的宽度。</p>
</td>
</tr>
<tr id="row7815mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p7817mcpsimp"><a name="p7817mcpsimp"></a><a name="p7817mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p7819mcpsimp"><a name="p7819mcpsimp"></a><a name="p7819mcpsimp"></a>广义图像的高度。</p>
</td>
</tr>
<tr id="row7820mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p7822mcpsimp"><a name="p7822mcpsimp"></a><a name="p7822mcpsimp"></a>type</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p7824mcpsimp"><a name="p7824mcpsimp"></a><a name="p7824mcpsimp"></a>广义图像的类型。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

每种type下面的图像示意图请参考OT\_SVP\_IMG\_TYPE\_U8C1 \\ OT\_SVP\_IMG\_TYPE\_S8C1 \\ OT\_SVP\_IMG\_TYPE\_S16C1 \\ OT\_SVP\_IMG\_TYPE\_U16C1 \\ OT\_SVP\_IMG\_TYPE\_S32C1 \\ OT\_SVP\_IMG\_TYPE\_U32C1 \\ OT\_SVP\_IMG\_TYPE\_S64C1 \\ OT\_SVP\_IMG\_TYPE\_U64C1 类型的ot\_svp\_img图像图\~ OT\_SVP\_IMG\_TYPE\_U8C3\_PLANAR类型的ot\_svp\_img图像图。

【相关数据及接口】

-   [ot\_svp\_img\_type](#ot_svp_img_type)
-   [ot\_svp\_src\_img](#ot_svp_src_img)
-   [ot\_svp\_dst\_img](#ot_svp_dst_img)

### ot\_svp\_src\_img<a name="ZH-CN_TOPIC_0000002471091254"></a>

【说明】

定义源图像。

【定义】

```
typedef ot_svp_img ot_svp_src_img;
```

【成员】

无

【注意事项】

无

【相关数据及接口】

-   [ot\_svp\_img\_type](#ot_svp_img_type)
-   [ot\_svp\_dst\_img](#ot_svp_dst_img)

### ot\_svp\_dst\_img<a name="ZH-CN_TOPIC_0000002471091286"></a>

【说明】

定义输出图像。

【定义】

```
typedef ot_svp_img ot_svp_dst_img;
```

【成员】

无

【注意事项】

无

【相关数据及接口】

-   [ot\_svp\_img\_type](#ot_svp_img_type)
-   [ot\_svp\_src\_img](#ot_svp_src_img)

### OT\_SVP\_IMG\_ADDR\_NUM<a name="ZH-CN_TOPIC_0000002470931272"></a>

【说明】

定义地址通道数目。

【定义】

```
#define OT_SVP_IMAE_ADDR_NUM    3
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

无。

### OT\_SVP\_IMG\_STRIDE\_NUM<a name="ZH-CN_TOPIC_0000002470931232"></a>

【说明】

定义跨度数组长度。

【定义】

```
#define  OT_SVP_IMG_STRIDE_NUM    3
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

无。

## 定点数据类型<a name="ZH-CN_TOPIC_0000002471091272"></a>

【说明】

定义定点化的数据类型。

【定义】

```
typedef unsigned char td_u0q8;
typedef unsigned char td_u1q7;
typedef unsigned char td_u5q3;
typedef unsigned char td_u3q5;
 
typedef unsigned short td_u0q16;
typedef unsigned short td_u4q12;
typedef unsigned short td_u6q10;
typedef unsigned short td_u8q8;
typedef unsigned short td_u9q7;
typedef unsigned short td_u12q4;
typedef unsigned short td_u14q2;
typedef unsigned short td_u5q11;
typedef unsigned short td_u1q15;
typedef unsigned short td_u2q14;
typedef td_u6q10 td_ufp16;
typedef short td_s9q7;
typedef short td_s14q2;
typedef short td_s1q15;
 
typedef unsigned int td_u22q10;
typedef unsigned int td_u25q7;
typedef unsigned int td_u21q11;
typedef unsigned int td_u14q18;
typedef unsigned int td_u8q24;
typedef unsigned int td_u4q28;
 
typedef int td_s25q7;
typedef int td_s16q16;
typedef int td_s14q18;
typedef int td_s20q12;
typedef int td_s24q8;
 
typedef unsigned short td_u8q4f4;
```

【成员】

<a name="table11614mcpsimp"></a>
<table><thead align="left"><tr id="row11619mcpsimp"><th class="cellrowborder" valign="top" width="18%" id="mcps1.1.3.1.1"><p id="p11621mcpsimp"><a name="p11621mcpsimp"></a><a name="p11621mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="82%" id="mcps1.1.3.1.2"><p id="p11623mcpsimp"><a name="p11623mcpsimp"></a><a name="p11623mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row11625mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11627mcpsimp"><a name="p11627mcpsimp"></a><a name="p11627mcpsimp"></a>td_u0q8</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11629mcpsimp"><a name="p11629mcpsimp"></a><a name="p11629mcpsimp"></a>用0bit表示整数部分，8bit表示小数部分。文档中用UQ0.8来表示。</p>
</td>
</tr>
<tr id="row11630mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11632mcpsimp"><a name="p11632mcpsimp"></a><a name="p11632mcpsimp"></a>td_u1q7</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11634mcpsimp"><a name="p11634mcpsimp"></a><a name="p11634mcpsimp"></a>用高1bit无符号数据表示整数部分，低7bit表示小数部分。文档中用UQ1.7来表示。</p>
</td>
</tr>
<tr id="row11635mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11637mcpsimp"><a name="p11637mcpsimp"></a><a name="p11637mcpsimp"></a>td_u5q3</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11639mcpsimp"><a name="p11639mcpsimp"></a><a name="p11639mcpsimp"></a>用高5bit无符号数据表示整数部分，低3bit表示小数部分。文档中用UQ5.3来表示。</p>
</td>
</tr>
<tr id="row11640mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11642mcpsimp"><a name="p11642mcpsimp"></a><a name="p11642mcpsimp"></a>td_u3q5</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11644mcpsimp"><a name="p11644mcpsimp"></a><a name="p11644mcpsimp"></a>用高3bit无符号数据表示整数部分，低5bit表示小数部分。文档中用UQ3.5来表示。</p>
</td>
</tr>
<tr id="row11645mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11647mcpsimp"><a name="p11647mcpsimp"></a><a name="p11647mcpsimp"></a>td_u0q16</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11649mcpsimp"><a name="p11649mcpsimp"></a><a name="p11649mcpsimp"></a>用0bit表示整数部分，16bit表示小数部分。文档中用UQ0.16来表示。</p>
</td>
</tr>
<tr id="row11650mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11652mcpsimp"><a name="p11652mcpsimp"></a><a name="p11652mcpsimp"></a>td_u4q12</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11654mcpsimp"><a name="p11654mcpsimp"></a><a name="p11654mcpsimp"></a>用高4bit无符号数据表示整数部分，低12bit表示小数部分。文档中用UQ4.12来表示。</p>
</td>
</tr>
<tr id="row11655mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11657mcpsimp"><a name="p11657mcpsimp"></a><a name="p11657mcpsimp"></a>td_u6q10</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11659mcpsimp"><a name="p11659mcpsimp"></a><a name="p11659mcpsimp"></a>用高6bit无符号数据表示整数部分，低10bit表示小数部分。文档中用UQ6.10来表示。</p>
</td>
</tr>
<tr id="row11660mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11662mcpsimp"><a name="p11662mcpsimp"></a><a name="p11662mcpsimp"></a>td_u8q8</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11664mcpsimp"><a name="p11664mcpsimp"></a><a name="p11664mcpsimp"></a>用高8bit无符号数据表示整数部分，低8bit表示小数部分。文档中用UQ8.8来表示。</p>
</td>
</tr>
<tr id="row11665mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11667mcpsimp"><a name="p11667mcpsimp"></a><a name="p11667mcpsimp"></a>td_u9q7</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11669mcpsimp"><a name="p11669mcpsimp"></a><a name="p11669mcpsimp"></a>用高9bit无符号数据表示整数部分，低7bit表示小数部分。文档中用UQ9.7来表示。</p>
</td>
</tr>
<tr id="row11670mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11672mcpsimp"><a name="p11672mcpsimp"></a><a name="p11672mcpsimp"></a>td_u12q4</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11674mcpsimp"><a name="p11674mcpsimp"></a><a name="p11674mcpsimp"></a>用高12bit无符号数据表示整数部分，低4bit表示小数部分。文档中用UQ12.4来表示。</p>
</td>
</tr>
<tr id="row11675mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11677mcpsimp"><a name="p11677mcpsimp"></a><a name="p11677mcpsimp"></a>td_u14q2</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11679mcpsimp"><a name="p11679mcpsimp"></a><a name="p11679mcpsimp"></a>用高14bit无符号数据表示整数部分，低2bit表示小数部分。文档中用UQ14.2来表示。</p>
</td>
</tr>
<tr id="row11680mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11682mcpsimp"><a name="p11682mcpsimp"></a><a name="p11682mcpsimp"></a>td_u5q11</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11684mcpsimp"><a name="p11684mcpsimp"></a><a name="p11684mcpsimp"></a>用高5bit无符号数据表示整数部分，低11bit表示小数部分。文档中用UQ5.11来表示。</p>
</td>
</tr>
<tr id="row11685mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11687mcpsimp"><a name="p11687mcpsimp"></a><a name="p11687mcpsimp"></a>td_u1q15</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11689mcpsimp"><a name="p11689mcpsimp"></a><a name="p11689mcpsimp"></a>用高1bit无符号数据表示整数部分，低15bit表示小数部分。文档中用UQ1.15来表示。</p>
</td>
</tr>
<tr id="row11690mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11692mcpsimp"><a name="p11692mcpsimp"></a><a name="p11692mcpsimp"></a>td_u2q14</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11694mcpsimp"><a name="p11694mcpsimp"></a><a name="p11694mcpsimp"></a>用高2bit无符号数据表示整数部分，低14bit表示小数部分。文档中用UQ2.14来表示。</p>
</td>
</tr>
<tr id="row11695mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11697mcpsimp"><a name="p11697mcpsimp"></a><a name="p11697mcpsimp"></a>td_ufp16</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11699mcpsimp"><a name="p11699mcpsimp"></a><a name="p11699mcpsimp"></a>用高6bit无符号数据表示整数部分，低10bit表示小数部分。文档中用UQ6.10来表示。</p>
</td>
</tr>
<tr id="row11700mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11702mcpsimp"><a name="p11702mcpsimp"></a><a name="p11702mcpsimp"></a>td_s9q7</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11704mcpsimp"><a name="p11704mcpsimp"></a><a name="p11704mcpsimp"></a>用高9bit有符号数据表示整数部分，低7bit表示小数部分。文档中用SQ9.7来表示。</p>
</td>
</tr>
<tr id="row11705mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11707mcpsimp"><a name="p11707mcpsimp"></a><a name="p11707mcpsimp"></a>td_s14q2</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11709mcpsimp"><a name="p11709mcpsimp"></a><a name="p11709mcpsimp"></a>用高14bit有符号数据表示整数部分，低2bit表示小数部分。文档中用SQ14.2来表示。</p>
</td>
</tr>
<tr id="row11710mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11712mcpsimp"><a name="p11712mcpsimp"></a><a name="p11712mcpsimp"></a>td_s1q15</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11714mcpsimp"><a name="p11714mcpsimp"></a><a name="p11714mcpsimp"></a>用高1bit有符号数据表示整数部分，低15bit表示小数部分。文档中用SQ1.15来表示。</p>
</td>
</tr>
<tr id="row11715mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11717mcpsimp"><a name="p11717mcpsimp"></a><a name="p11717mcpsimp"></a>td_u22q10</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11719mcpsimp"><a name="p11719mcpsimp"></a><a name="p11719mcpsimp"></a>用高22bit无符号数据表示整数部分，低10bit表示小数部分。文档中用UQ22.10来表示。</p>
</td>
</tr>
<tr id="row11720mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11722mcpsimp"><a name="p11722mcpsimp"></a><a name="p11722mcpsimp"></a>td_u25q7</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11724mcpsimp"><a name="p11724mcpsimp"></a><a name="p11724mcpsimp"></a>用高25bit无符号数据表示整数部分，低7bit表示小数部分。文档中用UQ25.7来表示。</p>
</td>
</tr>
<tr id="row11725mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11727mcpsimp"><a name="p11727mcpsimp"></a><a name="p11727mcpsimp"></a>td_u21q11</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11729mcpsimp"><a name="p11729mcpsimp"></a><a name="p11729mcpsimp"></a>用高21bit无符号数据表示整数部分，低11bit表示小数部分。文档中用UQ21.11来表示。</p>
</td>
</tr>
<tr id="row11730mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11732mcpsimp"><a name="p11732mcpsimp"></a><a name="p11732mcpsimp"></a>td_u14q18</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11734mcpsimp"><a name="p11734mcpsimp"></a><a name="p11734mcpsimp"></a>用高14bit无符号数据表示整数部分，低18bit表示小数部分。文档中用UQ14.18来表示。</p>
</td>
</tr>
<tr id="row11735mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11737mcpsimp"><a name="p11737mcpsimp"></a><a name="p11737mcpsimp"></a>td_u8q24</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11739mcpsimp"><a name="p11739mcpsimp"></a><a name="p11739mcpsimp"></a>用高8bit无符号数据表示整数部分，低24bit表示小数部分。文档中用UQ8.24来表示。</p>
</td>
</tr>
<tr id="row11740mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11742mcpsimp"><a name="p11742mcpsimp"></a><a name="p11742mcpsimp"></a>td_u4q28</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11744mcpsimp"><a name="p11744mcpsimp"></a><a name="p11744mcpsimp"></a>用高4bit无符号数据表示整数部分，低28bit表示小数部分。文档中用UQ4.28来表示。</p>
</td>
</tr>
<tr id="row11745mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11747mcpsimp"><a name="p11747mcpsimp"></a><a name="p11747mcpsimp"></a>td_s25q7</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11749mcpsimp"><a name="p11749mcpsimp"></a><a name="p11749mcpsimp"></a>用高25bit有符号数据表示整数部分，低7bit表示小数部分。文档中用SQ25.7来表示。</p>
</td>
</tr>
<tr id="row11750mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11752mcpsimp"><a name="p11752mcpsimp"></a><a name="p11752mcpsimp"></a>td_s16q16</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11754mcpsimp"><a name="p11754mcpsimp"></a><a name="p11754mcpsimp"></a>用高16bit有符号数据表示整数部分，低16bit表示小数部分。文档中用SQ16.16来表示。</p>
</td>
</tr>
<tr id="row11755mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11757mcpsimp"><a name="p11757mcpsimp"></a><a name="p11757mcpsimp"></a>td_s14q18</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11759mcpsimp"><a name="p11759mcpsimp"></a><a name="p11759mcpsimp"></a>用高14bit有符号数据表示整数部分，低18bit表示小数部分。文档中用SQ14.18来表示。</p>
</td>
</tr>
<tr id="row11760mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11762mcpsimp"><a name="p11762mcpsimp"></a><a name="p11762mcpsimp"></a>td_s20q12</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11764mcpsimp"><a name="p11764mcpsimp"></a><a name="p11764mcpsimp"></a>用高20bit有符号数据表示整数部分，低12bit表示小数部分。文档中用SQ20.12来表示。</p>
</td>
</tr>
<tr id="row11765mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11767mcpsimp"><a name="p11767mcpsimp"></a><a name="p11767mcpsimp"></a>td_s24q8</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11769mcpsimp"><a name="p11769mcpsimp"></a><a name="p11769mcpsimp"></a>用高24bit有符号数据表示整数部分，低8bit表示小数部分。文档中用SQ24.8来表示。</p>
</td>
</tr>
<tr id="row11770mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.3.1.1 "><p id="p11772mcpsimp"><a name="p11772mcpsimp"></a><a name="p11772mcpsimp"></a>td_u8q4f4</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.3.1.2 "><p id="p11774mcpsimp"><a name="p11774mcpsimp"></a><a name="p11774mcpsimp"></a>用高8bit无符号数据表示整数部分，中间4bit表示小数部分，低4bit表示标志位。文档中用UQF8.4.4来表示。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

td\_uxqyfz\\td\_sxqy：

-   u后面的数字x表示是用x bit无符号数据表示整数部分；
-   s后面的数字x表示用x bit有符号数据表示整数部分；
-   q后面的数字y表示用y bit 数据表示小数部分；
-   f后面的数字z表示用z bit 来表示标志位；
-   从左到右依次表示高bit位到低bit位。

【相关数据类型及接口】

无。













### ot\_svp\_data<a name="ZH-CN_TOPIC_0000002470931244"></a>

【说明】

定义以byte为单位的二维数据信息。

【定义】

```
typedef struct {
    td_u64 phys_addr; /* RW;The physical address of the data */
    td_u64 virt_addr; /* RW;The virtaul address of the data */
    td_u32 stride;   /* RW;The stride of 2D data by byte */
    td_u32 width;    /* RW;The width of 2D data by byte */
    td_u32 height;   /* RW;The height of 2D data by byte */
    td_u32 reserved;
} ot_svp_data;
```

【成员】

<a name="table1382mcpsimp"></a>
<table><thead align="left"><tr id="row1387mcpsimp"><th class="cellrowborder" valign="top" width="31%" id="mcps1.1.3.1.1"><p id="p1389mcpsimp"><a name="p1389mcpsimp"></a><a name="p1389mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="69%" id="mcps1.1.3.1.2"><p id="p1391mcpsimp"><a name="p1391mcpsimp"></a><a name="p1391mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1393mcpsimp"><td class="cellrowborder" valign="top" width="31%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p1395mcpsimp"><a name="p1395mcpsimp"></a><a name="p1395mcpsimp"></a>phys_addr</p>
</td>
<td class="cellrowborder" valign="top" width="69%" headers="mcps1.1.3.1.2 "><p id="p1397mcpsimp"><a name="p1397mcpsimp"></a><a name="p1397mcpsimp"></a>图像物理地址。</p>
</td>
</tr>
<tr id="row1398mcpsimp"><td class="cellrowborder" valign="top" width="31%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p1400mcpsimp"><a name="p1400mcpsimp"></a><a name="p1400mcpsimp"></a>virt_addr</p>
</td>
<td class="cellrowborder" valign="top" width="69%" headers="mcps1.1.3.1.2 "><p id="p1402mcpsimp"><a name="p1402mcpsimp"></a><a name="p1402mcpsimp"></a>图像虚拟地址。</p>
</td>
</tr>
<tr id="row1403mcpsimp"><td class="cellrowborder" valign="top" width="31%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p1405mcpsimp"><a name="p1405mcpsimp"></a><a name="p1405mcpsimp"></a>stride</p>
</td>
<td class="cellrowborder" valign="top" width="69%" headers="mcps1.1.3.1.2 "><p id="p1407mcpsimp"><a name="p1407mcpsimp"></a><a name="p1407mcpsimp"></a>图像跨度。</p>
</td>
</tr>
<tr id="row1408mcpsimp"><td class="cellrowborder" valign="top" width="31%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p1410mcpsimp"><a name="p1410mcpsimp"></a><a name="p1410mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" width="69%" headers="mcps1.1.3.1.2 "><p id="p1412mcpsimp"><a name="p1412mcpsimp"></a><a name="p1412mcpsimp"></a>图像高度。</p>
</td>
</tr>
<tr id="row1413mcpsimp"><td class="cellrowborder" valign="top" width="31%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p1415mcpsimp"><a name="p1415mcpsimp"></a><a name="p1415mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" width="69%" headers="mcps1.1.3.1.2 "><p id="p1417mcpsimp"><a name="p1417mcpsimp"></a><a name="p1417mcpsimp"></a>图像宽度。</p>
</td>
</tr>
<tr id="row1418mcpsimp"><td class="cellrowborder" valign="top" width="31%" headers="mcps1.1.3.1.1 "><p id="p1420mcpsimp"><a name="p1420mcpsimp"></a><a name="p1420mcpsimp"></a>reserved</p>
</td>
<td class="cellrowborder" valign="top" width="69%" headers="mcps1.1.3.1.2 "><p id="p1422mcpsimp"><a name="p1422mcpsimp"></a><a name="p1422mcpsimp"></a>保留位。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

表示以byte为单位的二维数据；可以与[ot\_svp\_img](ot_svp_img.md)图像进行转换。

【相关数据类型及接口】

无。

### ot\_svp\_src\_data<a name="ZH-CN_TOPIC_0000002503971213"></a>

【说明】

定义以byte为单位的二维源数据信息。

【定义】

```
typedef ot_svp_data ot_svp_src_data;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_svp\_img](#ot_svp_img)
-   [ot\_svp\_dst\_data](#ot_svp_dst_data)

### ot\_svp\_dst\_data<a name="ZH-CN_TOPIC_0000002470931290"></a>

【说明】

定义byte为单位的二维输出数据信息。

【定义】

```
typedef ot_svp_data ot_svp_dst_data;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_svp\_img](#ot_svp_img)
-   [ot\_svp\_src\_img](#ot_svp_src_img)

### ot\_svp\_8bit<a name="ZH-CN_TOPIC_0000002503971231"></a>

【说明】

定义8bit数据联合体。

【定义】

```
typedef union {
    td_s8 s8_val;
    td_u8 u8_val;
} ot_svp_8bit;
```

【成员】

<a name="table15205mcpsimp"></a>
<table><thead align="left"><tr id="row15210mcpsimp"><th class="cellrowborder" valign="top" width="31%" id="mcps1.1.3.1.1"><p id="p15212mcpsimp"><a name="p15212mcpsimp"></a><a name="p15212mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="69%" id="mcps1.1.3.1.2"><p id="p15214mcpsimp"><a name="p15214mcpsimp"></a><a name="p15214mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row15216mcpsimp"><td class="cellrowborder" valign="top" width="31%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p15218mcpsimp"><a name="p15218mcpsimp"></a><a name="p15218mcpsimp"></a>s8_val</p>
</td>
<td class="cellrowborder" valign="top" width="69%" headers="mcps1.1.3.1.2 "><p id="p15220mcpsimp"><a name="p15220mcpsimp"></a><a name="p15220mcpsimp"></a>有符号8bit值。</p>
</td>
</tr>
<tr id="row15221mcpsimp"><td class="cellrowborder" valign="top" width="31%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p15223mcpsimp"><a name="p15223mcpsimp"></a><a name="p15223mcpsimp"></a>u8_val</p>
</td>
<td class="cellrowborder" valign="top" width="69%" headers="mcps1.1.3.1.2 "><p id="p15225mcpsimp"><a name="p15225mcpsimp"></a><a name="p15225mcpsimp"></a>无符号8bit值。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

### ot\_svp\_point\_u16<a name="ZH-CN_TOPIC_0000002503971259"></a>

【说明】

定义u16表示的点信息结构体。

【定义】

```
typedef struct {
    td_u16 x; /* RW;The X coordinate of the point */
    td_u16 y; /* RW;The Y coordinate of the point */
} ot_svp_point_u16;
```

【成员】

<a name="table10459mcpsimp"></a>
<table><thead align="left"><tr id="row10464mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p10466mcpsimp"><a name="p10466mcpsimp"></a><a name="p10466mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p10468mcpsimp"><a name="p10468mcpsimp"></a><a name="p10468mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10470mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p10472mcpsimp"><a name="p10472mcpsimp"></a><a name="p10472mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p10474mcpsimp"><a name="p10474mcpsimp"></a><a name="p10474mcpsimp"></a>点的x坐标。</p>
</td>
</tr>
<tr id="row10475mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p10477mcpsimp"><a name="p10477mcpsimp"></a><a name="p10477mcpsimp"></a>y</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p10479mcpsimp"><a name="p10479mcpsimp"></a><a name="p10479mcpsimp"></a>点的y坐标。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

### ot\_svp\_point\_s16<a name="ZH-CN_TOPIC_0000002504091193"></a>

【说明】

定义s16表示的点信息结构体。

【定义】

```
typedef struct {
    td_s16 x; /* RW;The X coordinate of the point */
    td_s16 y; /* RW;The Y coordinate of the point */
} ot_svp_point_s16;
```

【成员】

<a name="table2917mcpsimp"></a>
<table><thead align="left"><tr id="row2922mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p2924mcpsimp"><a name="p2924mcpsimp"></a><a name="p2924mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p2926mcpsimp"><a name="p2926mcpsimp"></a><a name="p2926mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2928mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2930mcpsimp"><a name="p2930mcpsimp"></a><a name="p2930mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2932mcpsimp"><a name="p2932mcpsimp"></a><a name="p2932mcpsimp"></a>点的x坐标。</p>
</td>
</tr>
<tr id="row2933mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2935mcpsimp"><a name="p2935mcpsimp"></a><a name="p2935mcpsimp"></a>y</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2937mcpsimp"><a name="p2937mcpsimp"></a><a name="p2937mcpsimp"></a>点的y坐标。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

### ot\_svp\_point\_s25q7<a name="ZH-CN_TOPIC_0000002503971257"></a>

【说明】

定义s25q7表示的点信息结构体。

【定义】

```
typedef struct {
    td_s25q7 x; /* RW;The X coordinate of the point */
    td_s25q7 y; /* RW;The Y coordinate of the point */
} ot_svp_point_s25q7;
```

【成员】

<a name="table16735mcpsimp"></a>
<table><thead align="left"><tr id="row16740mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p16742mcpsimp"><a name="p16742mcpsimp"></a><a name="p16742mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p16744mcpsimp"><a name="p16744mcpsimp"></a><a name="p16744mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16746mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p16748mcpsimp"><a name="p16748mcpsimp"></a><a name="p16748mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p16750mcpsimp"><a name="p16750mcpsimp"></a><a name="p16750mcpsimp"></a>点的x坐标，以SQ25.7表示。</p>
</td>
</tr>
<tr id="row16751mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p16753mcpsimp"><a name="p16753mcpsimp"></a><a name="p16753mcpsimp"></a>y</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p16755mcpsimp"><a name="p16755mcpsimp"></a><a name="p16755mcpsimp"></a>点的y坐标，以SQ25.7表示。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

### ot\_svp\_point\_u14q2<a name="ZH-CN_TOPIC_0000002504091195"></a>

【说明】

定义u14q2表示的点信息结构体。

【定义】

```
typedef struct {
    td_u14q2 x;
    td_u14q2 y;
} ot_svp_point_u14q2;
```

【成员】

<a name="table12099mcpsimp"></a>
<table><thead align="left"><tr id="row12104mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p12106mcpsimp"><a name="p12106mcpsimp"></a><a name="p12106mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p12108mcpsimp"><a name="p12108mcpsimp"></a><a name="p12108mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row12110mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p12112mcpsimp"><a name="p12112mcpsimp"></a><a name="p12112mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p12114mcpsimp"><a name="p12114mcpsimp"></a><a name="p12114mcpsimp"></a>点的x坐标。</p>
</td>
</tr>
<tr id="row12115mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p12117mcpsimp"><a name="p12117mcpsimp"></a><a name="p12117mcpsimp"></a>y</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p12119mcpsimp"><a name="p12119mcpsimp"></a><a name="p12119mcpsimp"></a>点的y坐标。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

### ot\_svp\_rect\_u32<a name="ZH-CN_TOPIC_0000002470931266"></a>

【说明】

定义u32表示的矩形信息结构体。

【定义】

```
typedef struct {
    td_u32 x;      /* RW;The location of X axis of the rectangle */
    td_u32 y;      /* RW;The location of Y axis of the rectangle */
    td_u32 width;  /* RW;The width of the rectangle */
    td_u32 height; /* RW;The height of the rectangle */
} ot_svp_rect_u32;
```

【成员】

<a name="table10092mcpsimp"></a>
<table><thead align="left"><tr id="row10097mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p10099mcpsimp"><a name="p10099mcpsimp"></a><a name="p10099mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p10101mcpsimp"><a name="p10101mcpsimp"></a><a name="p10101mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10103mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p10105mcpsimp"><a name="p10105mcpsimp"></a><a name="p10105mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p10107mcpsimp"><a name="p10107mcpsimp"></a><a name="p10107mcpsimp"></a>矩形相对于坐标原点最近点的x坐标。</p>
</td>
</tr>
<tr id="row10108mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p10110mcpsimp"><a name="p10110mcpsimp"></a><a name="p10110mcpsimp"></a>y</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p10112mcpsimp"><a name="p10112mcpsimp"></a><a name="p10112mcpsimp"></a>矩形相对于坐标原点最近点的y坐标。</p>
</td>
</tr>
<tr id="row10113mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p10115mcpsimp"><a name="p10115mcpsimp"></a><a name="p10115mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p10117mcpsimp"><a name="p10117mcpsimp"></a><a name="p10117mcpsimp"></a>矩形的宽。</p>
</td>
</tr>
<tr id="row10118mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p10120mcpsimp"><a name="p10120mcpsimp"></a><a name="p10120mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p10122mcpsimp"><a name="p10122mcpsimp"></a><a name="p10122mcpsimp"></a>矩形的高。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及结构】

无

### ot\_svp\_rect\_u16<a name="ZH-CN_TOPIC_0000002470931214"></a>

【说明】

定义u16表示的矩形信息结构体。

【定义】

```
typedef struct {
    td_u16 x;      /* RW;The location of X axis of the rectangle */
    td_u16 y;      /* RW;The location of Y axis of the rectangle */
    td_u16 width;  /* RW;The width of the rectangle */
    td_u16 height; /* RW;The height of the rectangle */
} ot_svp_rect_u16;
```

【成员】

<a name="table7481mcpsimp"></a>
<table><thead align="left"><tr id="row7486mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p7488mcpsimp"><a name="p7488mcpsimp"></a><a name="p7488mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p7490mcpsimp"><a name="p7490mcpsimp"></a><a name="p7490mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7492mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p7494mcpsimp"><a name="p7494mcpsimp"></a><a name="p7494mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p7496mcpsimp"><a name="p7496mcpsimp"></a><a name="p7496mcpsimp"></a>矩形相对于坐标原点最近点的x坐标。</p>
</td>
</tr>
<tr id="row7497mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p7499mcpsimp"><a name="p7499mcpsimp"></a><a name="p7499mcpsimp"></a>y</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p7501mcpsimp"><a name="p7501mcpsimp"></a><a name="p7501mcpsimp"></a>矩形相对于坐标原点最近点的y坐标。</p>
</td>
</tr>
<tr id="row7502mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p7504mcpsimp"><a name="p7504mcpsimp"></a><a name="p7504mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p7506mcpsimp"><a name="p7506mcpsimp"></a><a name="p7506mcpsimp"></a>矩形的宽。</p>
</td>
</tr>
<tr id="row7507mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p7509mcpsimp"><a name="p7509mcpsimp"></a><a name="p7509mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p7511mcpsimp"><a name="p7511mcpsimp"></a><a name="p7511mcpsimp"></a>矩形的高。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及结构】

无

### ot\_svp\_rect\_s24q8<a name="ZH-CN_TOPIC_0000002503971197"></a>

【说明】

定义s24q8表示的矩形信息结构体。

【定义】

```
typedef struct {
    td_s24q8 x;
    td_s24q8 y;
    td_u32 width;
    td_u32 height;
} ot_svp_rect_s24q8;
```

【成员】

<a name="table1775mcpsimp"></a>
<table><thead align="left"><tr id="row1780mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p1782mcpsimp"><a name="p1782mcpsimp"></a><a name="p1782mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p1784mcpsimp"><a name="p1784mcpsimp"></a><a name="p1784mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1786mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p1788mcpsimp"><a name="p1788mcpsimp"></a><a name="p1788mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p1790mcpsimp"><a name="p1790mcpsimp"></a><a name="p1790mcpsimp"></a>矩形相对于坐标原点最近点的x坐标。</p>
</td>
</tr>
<tr id="row1791mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p1793mcpsimp"><a name="p1793mcpsimp"></a><a name="p1793mcpsimp"></a>y</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p1795mcpsimp"><a name="p1795mcpsimp"></a><a name="p1795mcpsimp"></a>矩形相对于坐标原点最近点的y坐标。</p>
</td>
</tr>
<tr id="row1796mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p1798mcpsimp"><a name="p1798mcpsimp"></a><a name="p1798mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p1800mcpsimp"><a name="p1800mcpsimp"></a><a name="p1800mcpsimp"></a>矩形的宽。</p>
</td>
</tr>
<tr id="row1801mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p1803mcpsimp"><a name="p1803mcpsimp"></a><a name="p1803mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p1805mcpsimp"><a name="p1805mcpsimp"></a><a name="p1805mcpsimp"></a>矩形的高。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及结构】

无

### ot\_svp\_lut<a name="ZH-CN_TOPIC_0000002503971221"></a>

【说明】

定义查找表结构体。

【定义】

```
typedef struct {
    ot_svp_mem_info table;
    td_u16 elem_num; /* RW;LUT's elements number */
 
    td_u8 table_in_precision;
    td_u8 table_out_norm;
 
    td_s32 table_in_lower; /* RW;LUT's original input lower limit */
    td_s32 table_in_upper; /* RW;LUT's original input upper limit */
} ot_svp_lut;
```

【成员】

<a name="table5017mcpsimp"></a>
<table><thead align="left"><tr id="row5022mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p5024mcpsimp"><a name="p5024mcpsimp"></a><a name="p5024mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p5026mcpsimp"><a name="p5026mcpsimp"></a><a name="p5026mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5028mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p5030mcpsimp"><a name="p5030mcpsimp"></a><a name="p5030mcpsimp"></a>table</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p5032mcpsimp"><a name="p5032mcpsimp"></a><a name="p5032mcpsimp"></a>查找表建立后的数据内存块信息。</p>
</td>
</tr>
<tr id="row5033mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p5035mcpsimp"><a name="p5035mcpsimp"></a><a name="p5035mcpsimp"></a>elem_num</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p5037mcpsimp"><a name="p5037mcpsimp"></a><a name="p5037mcpsimp"></a>查找表元素个数。</p>
</td>
</tr>
<tr id="row5038mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p5040mcpsimp"><a name="p5040mcpsimp"></a><a name="p5040mcpsimp"></a>table_in_precision</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p5042mcpsimp"><a name="p5042mcpsimp"></a><a name="p5042mcpsimp"></a>建立查找表的数值范围的下限。</p>
</td>
</tr>
<tr id="row5043mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p5045mcpsimp"><a name="p5045mcpsimp"></a><a name="p5045mcpsimp"></a>table_out_norm</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p5047mcpsimp"><a name="p5047mcpsimp"></a><a name="p5047mcpsimp"></a>建立查找表的数值范围的上限。</p>
</td>
</tr>
<tr id="row5048mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p5050mcpsimp"><a name="p5050mcpsimp"></a><a name="p5050mcpsimp"></a>table_in_lower</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p5052mcpsimp"><a name="p5052mcpsimp"></a><a name="p5052mcpsimp"></a>建立查找表的精度，(table_in_upper - table_in_lower)/(1&lt;&lt; table_in_precision)表示建立查找表的间隔</p>
</td>
</tr>
<tr id="row5053mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p5055mcpsimp"><a name="p5055mcpsimp"></a><a name="p5055mcpsimp"></a>table_in_upper</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p5057mcpsimp"><a name="p5057mcpsimp"></a><a name="p5057mcpsimp"></a>表示建立查找表时为将原始数据归一化进行移位的位数或者进行除法的除数。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及结构】

无

## IVE相关数据类型、数据结构<a name="ZH-CN_TOPIC_0000002503971225"></a>






















































































































### ot\_ive\_handle<a name="ZH-CN_TOPIC_0000002471091250"></a>

【说明】

定义IVE句柄。

【定义】

```
typedef td_s32 ot_ive_handle;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

无。

### OT\_IVE\_HIST\_NUM<a name="ZH-CN_TOPIC_0000002470931262"></a>

【说明】

定义直方图统计bin数目。

【定义】

```
#define OT_IVE_HIST_NUM	    256
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

无。

### OT\_IVE\_MAP\_NUM<a name="ZH-CN_TOPIC_0000002471091274"></a>

【说明】

定义映射查找表项数目。

【定义】

```
#define OT_IVE_MAP_NUM	    256
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

无。

### OT\_IVE\_MAX\_RGN\_NUM<a name="ZH-CN_TOPIC_0000002471091214"></a>

【说明】

定义最大连通区域数目。

【定义】

```
#define OT_IVE_MAX_RGN_NUM    254
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

无。

### OT\_IVE\_ST\_MAX\_CORNER\_NUM<a name="ZH-CN_TOPIC_0000002503971193"></a>

【说明】

定义Shi-Tomasi-like角点最大数目。

【定义】

```
#define OT_IVE_ST_MAX_CORNER_NUM    500
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

无。

### OT\_IVE\_MASK\_NUM<a name="ZH-CN_TOPIC_0000002471091310"></a>

【说明】

掩码mask对应的数组长度。

【定义】

```
#define OT_IVE_MASK_NUM                25
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_ARR\_RESERVED\_NUM\_TWO<a name="ZH-CN_TOPIC_0000002503971159"></a>

【说明】

保留字段数组长度2。

【定义】

```
#define OT_IVE_ARR_RESERVED_NUM_TWO         2
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_ARR\_RESERVED\_NUM\_THREE<a name="ZH-CN_TOPIC_0000002504091161"></a>

【说明】

保留字段数组长度3。

【定义】

```
#define OT_IVE_ARR_RESERVED_NUM_THREE       3
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_ARR\_RESERVED\_NUM\_EIGHT<a name="ZH-CN_TOPIC_0000002470931298"></a>

【说明】

保留字段数组长度8。

【定义】

```
#define OT_IVE_ARR_RESERVED_NUM_EIGHT       8
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_ARR\_RESERVED\_NUM\_TWELVE<a name="ZH-CN_TOPIC_0000002470931230"></a>

【说明】

保留字段数组长度12。

【定义】

```
#define OT_IVE_ARR_RESERVED_NUM_TWELVE      12
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_ARR\_RESERVED\_NUM\_FOURTEEN<a name="ZH-CN_TOPIC_0000002470931216"></a>

【说明】

保留字段数组长度14。

【定义】

```
#define OT_IVE_ARR_RESERVED_NUM_FOURTEEN    14
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_ARR\_NUM\_THREE<a name="ZH-CN_TOPIC_0000002503971175"></a>

【说明】

数组长度3。

【定义】

```
#define OT_IVE_ARR_NUM_THREE           3
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_ARR\_NUM\_EIGHT<a name="ZH-CN_TOPIC_0000002503971255"></a>

【说明】

数组长度8。

【定义】

```
#define OT_IVE_ARR_NUM_EIGHT           8
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_DEV\_NAME\_LENGTH<a name="ZH-CN_TOPIC_0000002503971217"></a>

【说明】

IVE设备名字的长度。

【定义】

```
#define OT_IVE_DEV_NAME_LENGTH           10
```

【成员】

无

【相关数据类型及接口】

无

### OT\_IVE\_DEV\_DEFAULT\_NODE\_NUM<a name="ZH-CN_TOPIC_0000002504091127"></a>

【说明】

默认的IVE节点个数。

【定义】

```
#define OT_IVE_DEFAULT_NODE_NUM           512
```

【成员】

无

【相关数据类型及接口】

无

### ot\_ive\_mod\_param<a name="ZH-CN_TOPIC_0000002504091147"></a>

【说明】

IVE模块相关参数定义。

【定义】

```
typedef struct {
    td_u16 mod_node_num;
    td_u8 power_save_en;
} ot_ive_mod_param;
```

【成员】

<a name="table16634mcpsimp"></a>
<table><thead align="left"><tr id="row16639mcpsimp"><th class="cellrowborder" valign="top" width="43%" id="mcps1.1.3.1.1"><p id="p16641mcpsimp"><a name="p16641mcpsimp"></a><a name="p16641mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="56.99999999999999%" id="mcps1.1.3.1.2"><p id="p16643mcpsimp"><a name="p16643mcpsimp"></a><a name="p16643mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16645mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16647mcpsimp"><a name="p16647mcpsimp"></a><a name="p16647mcpsimp"></a>mod_node_num</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16649mcpsimp"><a name="p16649mcpsimp"></a><a name="p16649mcpsimp"></a>IVE节点个数，取值范围[20, 512]。</p>
</td>
</tr>
<tr id="row16650mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p16652mcpsimp"><a name="p16652mcpsimp"></a><a name="p16652mcpsimp"></a>power_save_en</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p16654mcpsimp"><a name="p16654mcpsimp"></a><a name="p16654mcpsimp"></a>低功耗标志，取值范围[0, 1]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

ive\_std\_mod\_init

### ot\_ive\_err\_code<a name="ZH-CN_TOPIC_0000002504091141"></a>

【说明】

定义错误码。

【定义】

```
typedef enum {
    OT_IVE_ERR_SYS_TIMEOUT      = 0x40,             /* IVE process timeout */
    OT_IVE_ERR_QUERY_TIMEOUT    = 0x41,             /* IVE query timeout */
    OT_IVE_ERR_BUS_ERR          = 0x42,             /* IVE BUS error */
    OT_IVE_ERR_OPEN_FILE        = 0x43,             /* IVE open file error */
    OT_IVE_ERR_READ_FILE        = 0x44,             /* IVE read file error */
    OT_IVE_ERR_BUTT
} ot_ive_err_code;
```

【成员】

<a name="table14929mcpsimp"></a>
<table><thead align="left"><tr id="row14934mcpsimp"><th class="cellrowborder" valign="top" width="43%" id="mcps1.1.3.1.1"><p id="p14936mcpsimp"><a name="p14936mcpsimp"></a><a name="p14936mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="56.99999999999999%" id="mcps1.1.3.1.2"><p id="p14938mcpsimp"><a name="p14938mcpsimp"></a><a name="p14938mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14940mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p14942mcpsimp"><a name="p14942mcpsimp"></a><a name="p14942mcpsimp"></a>OT_IVE_ERR_SYS_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p14944mcpsimp"><a name="p14944mcpsimp"></a><a name="p14944mcpsimp"></a>系统超时。</p>
</td>
</tr>
<tr id="row14945mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p14947mcpsimp"><a name="p14947mcpsimp"></a><a name="p14947mcpsimp"></a>OT_IVE_ERR_QUERY_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p14949mcpsimp"><a name="p14949mcpsimp"></a><a name="p14949mcpsimp"></a>Query查询超时。</p>
</td>
</tr>
<tr id="row14950mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p14952mcpsimp"><a name="p14952mcpsimp"></a><a name="p14952mcpsimp"></a>OT_IVE_ERR_BUS_ERR</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p14954mcpsimp"><a name="p14954mcpsimp"></a><a name="p14954mcpsimp"></a>总线错误。</p>
</td>
</tr>
<tr id="row14955mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p14957mcpsimp"><a name="p14957mcpsimp"></a><a name="p14957mcpsimp"></a>OT_IVE_ERR_OPEN_FILE</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p14959mcpsimp"><a name="p14959mcpsimp"></a><a name="p14959mcpsimp"></a>打开文件失败。</p>
</td>
</tr>
<tr id="row14960mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p14962mcpsimp"><a name="p14962mcpsimp"></a><a name="p14962mcpsimp"></a>OT_IVE_ERR_READ_FILE</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p14964mcpsimp"><a name="p14964mcpsimp"></a><a name="p14964mcpsimp"></a>读文件失败。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_dma\_mode<a name="ZH-CN_TOPIC_0000002470931310"></a>

【说明】

定义dma操作模式。

【定义】

```
typedef enum {
    OT_IVE_DMA_MODE_DIRECT_COPY = 0x0,
    OT_IVE_DMA_MODE_INTERVAL_COPY = 0x1,
    OT_IVE_DMA_MODE_SET_3BYTE = 0x2,
    OT_IVE_DMA_MODE_SET_8BYTE = 0x3,
    OT_IVE_DMA_MODE_BUTT
} ot_ive_dma_mode;
```

【成员】

<a name="table12800mcpsimp"></a>
<table><thead align="left"><tr id="row12805mcpsimp"><th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.1"><p id="p12807mcpsimp"><a name="p12807mcpsimp"></a><a name="p12807mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.2"><p id="p12809mcpsimp"><a name="p12809mcpsimp"></a><a name="p12809mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row12811mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p12813mcpsimp"><a name="p12813mcpsimp"></a><a name="p12813mcpsimp"></a><span xml:lang="fr-FR" id="ph12814mcpsimp"><a name="ph12814mcpsimp"></a><a name="ph12814mcpsimp"></a>OT_IVE</span>_DMA_MODE_DIRECT_COPY</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p12816mcpsimp"><a name="p12816mcpsimp"></a><a name="p12816mcpsimp"></a>直接快速拷贝模式。</p>
</td>
</tr>
<tr id="row12817mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p12819mcpsimp"><a name="p12819mcpsimp"></a><a name="p12819mcpsimp"></a><span xml:lang="fr-FR" id="ph12820mcpsimp"><a name="ph12820mcpsimp"></a><a name="ph12820mcpsimp"></a>OT_IVE</span>_DMA_MODE_INTERVAL_COPY</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p12822mcpsimp"><a name="p12822mcpsimp"></a><a name="p12822mcpsimp"></a>间隔拷贝模式，请参见ss_mpi_ive_dma【注意】说明。</p>
</td>
</tr>
<tr id="row12824mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p12826mcpsimp"><a name="p12826mcpsimp"></a><a name="p12826mcpsimp"></a><span xml:lang="fr-FR" id="ph12827mcpsimp"><a name="ph12827mcpsimp"></a><a name="ph12827mcpsimp"></a>OT_IVE</span>_DMA_MODE_SET_3BYTE</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p12829mcpsimp"><a name="p12829mcpsimp"></a><a name="p12829mcpsimp"></a>3byte赋值模式，请参见ss_mpi_ive_dma【注意】说明。</p>
</td>
</tr>
<tr id="row12831mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p12833mcpsimp"><a name="p12833mcpsimp"></a><a name="p12833mcpsimp"></a><span xml:lang="fr-FR" id="ph12834mcpsimp"><a name="ph12834mcpsimp"></a><a name="ph12834mcpsimp"></a>OT_IVE</span>_DMA_MODE_SET_8BYTE</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p12836mcpsimp"><a name="p12836mcpsimp"></a><a name="p12836mcpsimp"></a>8byte赋值模式，请参见ss_mpi_ive_dma【注意】说明。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_dma\_ctrl](#ot_ive_dma_ctrl)

### ot\_ive\_dma\_ctrl<a name="ZH-CN_TOPIC_0000002504091157"></a>

【说明】

定义dma控制信息。

【定义】

```
typedef struct {
    ot_ive_dma_mode mode;
    td_u64 val;
    td_u8  hor_seg_size;
    td_u8 elem_size;
    td_u8 ver_seg_rows;
} ot_ive_dma_ctrl
```

【成员】

<a name="table7716mcpsimp"></a>
<table><thead align="left"><tr id="row7721mcpsimp"><th class="cellrowborder" valign="top" width="26%" id="mcps1.1.3.1.1"><p id="p7723mcpsimp"><a name="p7723mcpsimp"></a><a name="p7723mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="74%" id="mcps1.1.3.1.2"><p id="p7725mcpsimp"><a name="p7725mcpsimp"></a><a name="p7725mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7727mcpsimp"><td class="cellrowborder" valign="top" width="26%" headers="mcps1.1.3.1.1 "><p id="p7729mcpsimp"><a name="p7729mcpsimp"></a><a name="p7729mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="74%" headers="mcps1.1.3.1.2 "><p id="p7731mcpsimp"><a name="p7731mcpsimp"></a><a name="p7731mcpsimp"></a>dma操作模式。</p>
</td>
</tr>
<tr id="row7732mcpsimp"><td class="cellrowborder" valign="top" width="26%" headers="mcps1.1.3.1.1 "><p id="p7734mcpsimp"><a name="p7734mcpsimp"></a><a name="p7734mcpsimp"></a>val</p>
</td>
<td class="cellrowborder" valign="top" width="74%" headers="mcps1.1.3.1.2 "><p id="p7736mcpsimp"><a name="p7736mcpsimp"></a><a name="p7736mcpsimp"></a>仅赋值模式使用，用于对内存赋值，3byte赋值模式用低3byte保存。</p>
</td>
</tr>
<tr id="row7737mcpsimp"><td class="cellrowborder" valign="top" width="26%" headers="mcps1.1.3.1.1 "><p id="p7739mcpsimp"><a name="p7739mcpsimp"></a><a name="p7739mcpsimp"></a>hor_seg_size</p>
</td>
<td class="cellrowborder" valign="top" width="74%" headers="mcps1.1.3.1.2 "><p id="p7741mcpsimp"><a name="p7741mcpsimp"></a><a name="p7741mcpsimp"></a>仅间隔拷贝模式使用，水平方向将源图像一行分割的段大小。</p>
<p id="p7742mcpsimp"><a name="p7742mcpsimp"></a><a name="p7742mcpsimp"></a>取值范围：{2, 3, 4, 8, 16}。</p>
</td>
</tr>
<tr id="row7743mcpsimp"><td class="cellrowborder" valign="top" width="26%" headers="mcps1.1.3.1.1 "><p id="p7745mcpsimp"><a name="p7745mcpsimp"></a><a name="p7745mcpsimp"></a>elem_size</p>
</td>
<td class="cellrowborder" valign="top" width="74%" headers="mcps1.1.3.1.2 "><p id="p7747mcpsimp"><a name="p7747mcpsimp"></a><a name="p7747mcpsimp"></a>仅间隔拷贝模式使用，分割的每一段中前elem_sizebyte为有效的拷贝字段。</p>
<p id="p7748mcpsimp"><a name="p7748mcpsimp"></a><a name="p7748mcpsimp"></a>取值范围：[1, hor_seg_size-1]。</p>
</td>
</tr>
<tr id="row7749mcpsimp"><td class="cellrowborder" valign="top" width="26%" headers="mcps1.1.3.1.1 "><p id="p7751mcpsimp"><a name="p7751mcpsimp"></a><a name="p7751mcpsimp"></a>ver_seg_rows</p>
</td>
<td class="cellrowborder" valign="top" width="74%" headers="mcps1.1.3.1.2 "><p id="p7753mcpsimp"><a name="p7753mcpsimp"></a><a name="p7753mcpsimp"></a>仅间隔拷贝模式使用，将每ver_seg_rows行中第一行数据分割为hor_seg_size大小的段，拷贝每段中的前elem_size大小的字节</p>
<p id="p7754mcpsimp"><a name="p7754mcpsimp"></a><a name="p7754mcpsimp"></a>取值范围：[1, min{65535/src_stride, src_height}]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_dma\_mode](#ot_ive_dma_mode)

### ot\_ive\_filter\_ctrl<a name="ZH-CN_TOPIC_0000002503971267"></a>

【说明】

定义模板滤波控制信息。

【定义】

```
typedef struct {
    td_s8 mask[OT_IVE_MASK_NUM];     /* Template parameter filter coefficient */
    td_u8 norm;         /* Normalization parameter, by right shift */
} ot_ive_filter_ctrl
```

【成员】

<a name="table16288mcpsimp"></a>
<table><thead align="left"><tr id="row16293mcpsimp"><th class="cellrowborder" valign="top" width="36%" id="mcps1.1.3.1.1"><p id="p16295mcpsimp"><a name="p16295mcpsimp"></a><a name="p16295mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.3.1.2"><p id="p16297mcpsimp"><a name="p16297mcpsimp"></a><a name="p16297mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16299mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p16301mcpsimp"><a name="p16301mcpsimp"></a><a name="p16301mcpsimp"></a>mask[<a href="OT_IVE_MASK_NUM.md">OT_IVE_MASK_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p16305mcpsimp"><a name="p16305mcpsimp"></a><a name="p16305mcpsimp"></a>5x5模板系数，外围系数设为0可实现3x3模板滤波。</p>
</td>
</tr>
<tr id="row16306mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p16308mcpsimp"><a name="p16308mcpsimp"></a><a name="p16308mcpsimp"></a>norm</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p16310mcpsimp"><a name="p16310mcpsimp"></a><a name="p16310mcpsimp"></a>归一化参数。</p>
<p id="p16311mcpsimp"><a name="p16311mcpsimp"></a><a name="p16311mcpsimp"></a>取值范围：[0, 13]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

通过配置不同的模板系数可以达到不同的滤波效果。

【相关数据类型及接口】

无。

### ot\_ive\_csc\_mode<a name="ZH-CN_TOPIC_0000002470931222"></a>

【说明】

定义色彩空间转换模式。

【定义】

```
typedef enum {
    OT_IVE_CSC_MODE_VIDEO_BT601_YUV_TO_RGB = 0x0,     /* CSC: YUV_TO_RGB, video transfer mode, RGB value range [16, 235] */
    OT_IVE_CSC_MODE_VIDEO_BT709_YUV_TO_RGB = 0x1,     /* CSC: YUV_To_RGB, video transfer mode, RGB value range [16, 235] */
    OT_IVE_CSC_MODE_PIC_BT601_YUV_TO_RGB = 0x2,       /* CSC: YUV_TO_RGB, picture transfer mode, RGB value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT709_YUV_TO_RGB = 0x3,       /* CSC: YUV_TO_RGB, picture transfer mode, RGB value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT601_YUV_TO_HSV = 0x4,       /* CSC: YUV_TO_HSV, picture transfer mode, HSV value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT709_YUV_TO_HSV = 0x5,       /* CSC: YUV_TO_HSV, picture transfer mode, HSV value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT601_YUV_TO_LAB = 0x6,       /* CSC: YUV_TO_LAB, picture transfer mode, Lab value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT709_YUV_TO_LAB = 0x7,       /* CSC: YUV_TO_LAB, picture transfer mode, Lab value range [0, 255] */
    OT_IVE_CSC_MODE_VIDEO_BT601_RGB_TO_YUV = 0x8,     /* CSC: RGB_TO_YUV, video transfer mode, YUV value range [0, 255] */
    OT_IVE_CSC_MODE_VIDEO_BT709_RGB_TO_2YUV = 0x9,     /* CSC: RGB_TO_YUV, video transfer mode, YUV value range [0, 255] */
    OT_IVE_CSC_MODE_PIC_BT601_RGB_TO_YUV = 0xa,       /* CSC: RGB_TO_YUV, picture transfer mode, Y:[16, 235],U\V:[16, 240] */
    OT_IVE_CSC_MODE_PIC_BT709_RGB_TO_YUV = 0xb,       /* CSC: RGB_TO_YUV, picture transfer mode, Y:[16, 235],U\V:[16, 240] */
    OT_IVE_CSC_MODE_BUTT
} ot_ive_csc_mode
```

【成员】

<a name="table4136mcpsimp"></a>
<table><thead align="left"><tr id="row4141mcpsimp"><th class="cellrowborder" valign="top" width="67%" id="mcps1.1.3.1.1"><p id="p4143mcpsimp"><a name="p4143mcpsimp"></a><a name="p4143mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="33%" id="mcps1.1.3.1.2"><p id="p4145mcpsimp"><a name="p4145mcpsimp"></a><a name="p4145mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4147mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4149mcpsimp"><a name="p4149mcpsimp"></a><a name="p4149mcpsimp"></a><span xml:lang="fr-FR" id="ph4150mcpsimp"><a name="ph4150mcpsimp"></a><a name="ph4150mcpsimp"></a>OT_IVE</span>_CSC_MODE_VIDEO_BT601_YUV_TO_RGB</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4152mcpsimp"><a name="p4152mcpsimp"></a><a name="p4152mcpsimp"></a>BT601的YUV_TO_RGB视频变换。</p>
</td>
</tr>
<tr id="row4153mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4155mcpsimp"><a name="p4155mcpsimp"></a><a name="p4155mcpsimp"></a><span xml:lang="fr-FR" id="ph4156mcpsimp"><a name="ph4156mcpsimp"></a><a name="ph4156mcpsimp"></a>OT_IVE</span>_CSC_MODE_VIDEO_BT709_YUV_TO_RGB</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4158mcpsimp"><a name="p4158mcpsimp"></a><a name="p4158mcpsimp"></a>BT709的YUV_TO_RGB视频变换。</p>
</td>
</tr>
<tr id="row4159mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4161mcpsimp"><a name="p4161mcpsimp"></a><a name="p4161mcpsimp"></a><span xml:lang="fr-FR" id="ph4162mcpsimp"><a name="ph4162mcpsimp"></a><a name="ph4162mcpsimp"></a>OT_IVE</span>_CSC_MODE_PIC_BT601_YUV_TO_RGB</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4164mcpsimp"><a name="p4164mcpsimp"></a><a name="p4164mcpsimp"></a>BT601的YUV_TO_RGB图像变换。</p>
</td>
</tr>
<tr id="row4165mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4167mcpsimp"><a name="p4167mcpsimp"></a><a name="p4167mcpsimp"></a><span xml:lang="fr-FR" id="ph4168mcpsimp"><a name="ph4168mcpsimp"></a><a name="ph4168mcpsimp"></a>OT_IVE</span>_CSC_MODE_PIC_BT709_YUV_TO_RGB</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4170mcpsimp"><a name="p4170mcpsimp"></a><a name="p4170mcpsimp"></a>BT709的YUV_TO_RGB图像变换。</p>
</td>
</tr>
<tr id="row4171mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4173mcpsimp"><a name="p4173mcpsimp"></a><a name="p4173mcpsimp"></a><span xml:lang="fr-FR" id="ph4174mcpsimp"><a name="ph4174mcpsimp"></a><a name="ph4174mcpsimp"></a>OT_IVE</span>_CSC_MODE_PIC_BT601_YUV_TO_HSV</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4176mcpsimp"><a name="p4176mcpsimp"></a><a name="p4176mcpsimp"></a>BT601的YUV_TO_HSV图像变换。</p>
</td>
</tr>
<tr id="row4177mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4179mcpsimp"><a name="p4179mcpsimp"></a><a name="p4179mcpsimp"></a><span xml:lang="fr-FR" id="ph4180mcpsimp"><a name="ph4180mcpsimp"></a><a name="ph4180mcpsimp"></a>OT_IVE</span>_CSC_MODE_PIC_BT709_YUV_TO_HSV</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4182mcpsimp"><a name="p4182mcpsimp"></a><a name="p4182mcpsimp"></a>BT709的YUV_TO_HSV图像变换。</p>
</td>
</tr>
<tr id="row4183mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4185mcpsimp"><a name="p4185mcpsimp"></a><a name="p4185mcpsimp"></a><span xml:lang="fr-FR" id="ph4186mcpsimp"><a name="ph4186mcpsimp"></a><a name="ph4186mcpsimp"></a>OT_IVE</span>_CSC_MODE_PIC_BT601_YUV_TO_LAB</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4188mcpsimp"><a name="p4188mcpsimp"></a><a name="p4188mcpsimp"></a>BT601的YUV_TO_LAB图像变换。</p>
</td>
</tr>
<tr id="row4189mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4191mcpsimp"><a name="p4191mcpsimp"></a><a name="p4191mcpsimp"></a><span xml:lang="fr-FR" id="ph4192mcpsimp"><a name="ph4192mcpsimp"></a><a name="ph4192mcpsimp"></a>OT_IVE</span>_CSC_MODE_PIC_BT709_YUV_TO_LAB</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4194mcpsimp"><a name="p4194mcpsimp"></a><a name="p4194mcpsimp"></a>BT709的YUV_TO_LAB图像变换。</p>
</td>
</tr>
<tr id="row4195mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4197mcpsimp"><a name="p4197mcpsimp"></a><a name="p4197mcpsimp"></a><span xml:lang="fr-FR" id="ph4198mcpsimp"><a name="ph4198mcpsimp"></a><a name="ph4198mcpsimp"></a>OT_IVE</span>_CSC_MODE_VIDEO_BT601_RGB_TO_YUV</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4200mcpsimp"><a name="p4200mcpsimp"></a><a name="p4200mcpsimp"></a>BT601的RGB_TO_YUV视频变换。</p>
</td>
</tr>
<tr id="row4201mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4203mcpsimp"><a name="p4203mcpsimp"></a><a name="p4203mcpsimp"></a><span xml:lang="fr-FR" id="ph4204mcpsimp"><a name="ph4204mcpsimp"></a><a name="ph4204mcpsimp"></a>OT_IVE</span>_CSC_MODE_VIDEO_BT709_RGB_TO_YUV</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4206mcpsimp"><a name="p4206mcpsimp"></a><a name="p4206mcpsimp"></a>BT709的RGB_TO_YUV视频变换。</p>
</td>
</tr>
<tr id="row4207mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4209mcpsimp"><a name="p4209mcpsimp"></a><a name="p4209mcpsimp"></a><span xml:lang="fr-FR" id="ph4210mcpsimp"><a name="ph4210mcpsimp"></a><a name="ph4210mcpsimp"></a>OT_IVE</span>_CSC_MODE_PIC_BT601_RGB_TO_YUV</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4212mcpsimp"><a name="p4212mcpsimp"></a><a name="p4212mcpsimp"></a>BT601的RGB_TO_YUV图像变换。</p>
</td>
</tr>
<tr id="row4213mcpsimp"><td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.3.1.1 "><p id="p4215mcpsimp"><a name="p4215mcpsimp"></a><a name="p4215mcpsimp"></a><span xml:lang="fr-FR" id="ph4216mcpsimp"><a name="ph4216mcpsimp"></a><a name="ph4216mcpsimp"></a>OT_IVE</span>_CSC_MODE_PIC_BT709_RGB_TO_YUV</p>
</td>
<td class="cellrowborder" valign="top" width="33%" headers="mcps1.1.3.1.2 "><p id="p4218mcpsimp"><a name="p4218mcpsimp"></a><a name="p4218mcpsimp"></a>BT709的RGB_TO_YUV图像变换。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   OT\_IVE\_CSC\_MODE\_VIDEO\_BT601\_YUV\_TO\_RGB和OT\_IVE\_CSC\_MODE\_VIDEO\_BT709\_YUV\_TO\_RGB模式，输出满足16≤R、G、B≤235。
-   OT\_IVE\_CSC\_MODE\_PIC\_BT601\_YUV\_TO\_RGB和OT\_IVE\_CSC\_MODE\_PIC\_BT709\_YUV\_TO\_RGB模式，输出满足0≤R、G、B≤255。
-   OT\_IVE\_CSC\_MODE\_PIC\_BT601\_YUV\_TO\_HSV和OT\_IVE\_CSC\_MODE\_PIC\_BT709\_YUV\_TO\_HSV模式，输出满足0≤H、S、V≤255。
-   OT\_IVE\_CSC\_MODE\_PIC\_BT601\_YUV\_TO\_LAB和OT\_IVE\_CSC\_MODE\_PIC\_BT709\_YUV\_TO\_LAB模式，输出满足0≤L、A、B≤255。
-   OT\_IVE\_CSC\_MODE\_VIDEO\_BT601\_RGB\_TO\_YUV和OT\_IVE\_CSC\_MODE\_VIDEO\_BT709\_RGB\_TO\_YUV模式，输出满足0≤Y、U、V≤255。
-   OT\_IVE\_CSC\_MODE\_PIC\_BT601\_RGB\_TO\_YUV和OT\_IVE\_CSC\_MODE\_PIC\_BT709\_RGB\_TO\_YUV模式，输出满足0≤Y≤235，0≤U、V≤240。

【相关数据类型及接口】

-   [ot\_ive\_csc\_ctrl](#ot_ive_csc_ctrl)
-   [ot\_ive\_filter\_and\_csc\_ctrl](#ot_ive_filter_and_csc_ctrl)

### ot\_ive\_csc\_ctrl<a name="ZH-CN_TOPIC_0000002504091137"></a>

【说明】

定义色彩空间转换控制信息。

【定义】

```
typedef struct {
    ot_ive_csc_mode mode; /* Working mode */
} ot_ive_csc_ctrl
```

【成员】

<a name="table8630mcpsimp"></a>
<table><thead align="left"><tr id="row8635mcpsimp"><th class="cellrowborder" valign="top" width="23%" id="mcps1.1.3.1.1"><p id="p8637mcpsimp"><a name="p8637mcpsimp"></a><a name="p8637mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="77%" id="mcps1.1.3.1.2"><p id="p8639mcpsimp"><a name="p8639mcpsimp"></a><a name="p8639mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row8641mcpsimp"><td class="cellrowborder" valign="top" width="23%" headers="mcps1.1.3.1.1 "><p id="p8643mcpsimp"><a name="p8643mcpsimp"></a><a name="p8643mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="77%" headers="mcps1.1.3.1.2 "><p id="p8645mcpsimp"><a name="p8645mcpsimp"></a><a name="p8645mcpsimp"></a>工作模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_csc\_mode](#ot_ive_csc_mode)

### ot\_ive\_filter\_and\_csc\_ctrl<a name="ZH-CN_TOPIC_0000002471091222"></a>

【说明】

定义模板滤波加色彩空间转换复合功能控制信息。

【定义】

```
typedef struct {
    ot_ive_csc_mode mode;   /* CSC working mode */
    td_s8 mask[OT_IVE_MASK_NUM];         /* Template parameter filter coefficient */
    td_u8 norm;             /* Normalization parameter, by right shift */
} ot_ive_filter_and_csc_ctrl ;
```

【成员】

<a name="table114mcpsimp"></a>
<table><thead align="left"><tr id="row119mcpsimp"><th class="cellrowborder" valign="top" width="37%" id="mcps1.1.3.1.1"><p id="p121mcpsimp"><a name="p121mcpsimp"></a><a name="p121mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.3.1.2"><p id="p123mcpsimp"><a name="p123mcpsimp"></a><a name="p123mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row125mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p id="p127mcpsimp"><a name="p127mcpsimp"></a><a name="p127mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p129mcpsimp"><a name="p129mcpsimp"></a><a name="p129mcpsimp"></a>工作模式。</p>
</td>
</tr>
<tr id="row130mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p id="p132mcpsimp"><a name="p132mcpsimp"></a><a name="p132mcpsimp"></a>mask[<a href="OT_IVE_MASK_NUM.md">OT_IVE_MASK_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p136mcpsimp"><a name="p136mcpsimp"></a><a name="p136mcpsimp"></a>5x5模板系数。</p>
</td>
</tr>
<tr id="row137mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p id="p139mcpsimp"><a name="p139mcpsimp"></a><a name="p139mcpsimp"></a>norm</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p141mcpsimp"><a name="p141mcpsimp"></a><a name="p141mcpsimp"></a>归一化参数。</p>
<p id="p142mcpsimp"><a name="p142mcpsimp"></a><a name="p142mcpsimp"></a>取值范围：[0, 13]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

仅支持YUV2RGB的4种模式。

【相关数据类型及接口】

[ot\_ive\_csc\_mode](#ot_ive_csc_mode)

### ot\_ive\_sobel\_out\_ctrl<a name="ZH-CN_TOPIC_0000002503971149"></a>

【说明】

定义sobel输出控制信息。

【定义】

```
typedef enum {
    OT_IVE_SOBEL_OUT_CTRL_BOTH = 0x0, /* Output horizontal and vertical */
    OT_IVE_SOBEL_OUT_CTRL_HOR = 0x1,  /* Output horizontal */
    OT_IVE_SOBEL_OUT_CTRL_VER = 0x2,  /* Output vertical */
    OT_IVE_SOBEL_OUT_CTRL_BUTT
} ot_ive_sobel_out_ctrl;
```

【成员】

<a name="table16823mcpsimp"></a>
<table><thead align="left"><tr id="row16828mcpsimp"><th class="cellrowborder" valign="top" width="47%" id="mcps1.1.3.1.1"><p id="p16830mcpsimp"><a name="p16830mcpsimp"></a><a name="p16830mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="53%" id="mcps1.1.3.1.2"><p id="p16832mcpsimp"><a name="p16832mcpsimp"></a><a name="p16832mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16834mcpsimp"><td class="cellrowborder" valign="top" width="47%" headers="mcps1.1.3.1.1 "><p id="p16836mcpsimp"><a name="p16836mcpsimp"></a><a name="p16836mcpsimp"></a><span xml:lang="fr-FR" id="ph16837mcpsimp"><a name="ph16837mcpsimp"></a><a name="ph16837mcpsimp"></a>OT_IVE</span>_SOBEL_OUT_CTRL_BOTH</p>
</td>
<td class="cellrowborder" valign="top" width="53%" headers="mcps1.1.3.1.2 "><p id="p16839mcpsimp"><a name="p16839mcpsimp"></a><a name="p16839mcpsimp"></a>同时输出用模板和转置模板滤波的结果。</p>
</td>
</tr>
<tr id="row16840mcpsimp"><td class="cellrowborder" valign="top" width="47%" headers="mcps1.1.3.1.1 "><p id="p16842mcpsimp"><a name="p16842mcpsimp"></a><a name="p16842mcpsimp"></a><span xml:lang="fr-FR" id="ph16843mcpsimp"><a name="ph16843mcpsimp"></a><a name="ph16843mcpsimp"></a>OT_IVE</span>_SOBEL_OUT_CTRL_HOR</p>
</td>
<td class="cellrowborder" valign="top" width="53%" headers="mcps1.1.3.1.2 "><p id="p16845mcpsimp"><a name="p16845mcpsimp"></a><a name="p16845mcpsimp"></a>仅输出用模板直接滤波的结果。</p>
</td>
</tr>
<tr id="row16846mcpsimp"><td class="cellrowborder" valign="top" width="47%" headers="mcps1.1.3.1.1 "><p id="p16848mcpsimp"><a name="p16848mcpsimp"></a><a name="p16848mcpsimp"></a><span xml:lang="fr-FR" id="ph16849mcpsimp"><a name="ph16849mcpsimp"></a><a name="ph16849mcpsimp"></a>OT_IVE</span>_SOBEL_OUT_CTRL_VER</p>
</td>
<td class="cellrowborder" valign="top" width="53%" headers="mcps1.1.3.1.2 "><p id="p16851mcpsimp"><a name="p16851mcpsimp"></a><a name="p16851mcpsimp"></a>仅输出用转置模板滤波的结果。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_sobel\_ctrl](#ot_ive_sobel_ctrl)

### ot\_ive\_sobel\_ctrl<a name="ZH-CN_TOPIC_0000002470931226"></a>

【说明】

定义sobel-like梯度计算控制信息。

【定义】

```
typedef struct {
    ot_ive_sobel_out_ctrl out_ctrl; /* Output format */
    td_s8 mask[OT_IVE_MASK_NUM];                 /* Template parameter */
} ot_ive_sobel_ctrl;
```

【成员】

<a name="table3633mcpsimp"></a>
<table><thead align="left"><tr id="row3638mcpsimp"><th class="cellrowborder" valign="top" width="36%" id="mcps1.1.3.1.1"><p id="p3640mcpsimp"><a name="p3640mcpsimp"></a><a name="p3640mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.3.1.2"><p id="p3642mcpsimp"><a name="p3642mcpsimp"></a><a name="p3642mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3644mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p3646mcpsimp"><a name="p3646mcpsimp"></a><a name="p3646mcpsimp"></a>out_ctrl</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p3648mcpsimp"><a name="p3648mcpsimp"></a><a name="p3648mcpsimp"></a>输出控制枚举参数。</p>
</td>
</tr>
<tr id="row3649mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p3651mcpsimp"><a name="p3651mcpsimp"></a><a name="p3651mcpsimp"></a>mask[<a href="OT_IVE_MASK_NUM.md">OT_IVE_MASK_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p3655mcpsimp"><a name="p3655mcpsimp"></a><a name="p3655mcpsimp"></a>5x5模板系数。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_sobel\_out\_ctrl](#ot_ive_sobel_out_ctrl)

### ot\_ive\_mag\_and\_ang\_out\_ctrl<a name="ZH-CN_TOPIC_0000002471091234"></a>

【说明】

定义梯度幅值与角度计算的输出格式。

【定义】

```
typedef enum {
    OT_IVE_MAG_AND_ANG_OUT_CTRL_MAG = 0x0,/* Only the magnitude is output.*/
    OT_IVE_MAG_AND_ANG_OUT_CTRL_MAG_AND_ANG = 0x1, /* The magnitude and angle are output.*/
    OT_IVE_MAG_AND_ANG_OUT_CTRL_BUTT
} ot_ive_mag_and_ang_out_ctrl;
```

【成员】

<a name="table1441mcpsimp"></a>
<table><thead align="left"><tr id="row1446mcpsimp"><th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.1"><p id="p1448mcpsimp"><a name="p1448mcpsimp"></a><a name="p1448mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.2"><p id="p1450mcpsimp"><a name="p1450mcpsimp"></a><a name="p1450mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1452mcpsimp"><td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.1 "><p id="p1454mcpsimp"><a name="p1454mcpsimp"></a><a name="p1454mcpsimp"></a><span xml:lang="fr-FR" id="ph1455mcpsimp"><a name="ph1455mcpsimp"></a><a name="ph1455mcpsimp"></a>OT_IVE</span>_MAG_AND_ANG_OUT_CTRL_MAG</p>
</td>
<td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.2 "><p id="p1457mcpsimp"><a name="p1457mcpsimp"></a><a name="p1457mcpsimp"></a>仅输出幅值。</p>
</td>
</tr>
<tr id="row1458mcpsimp"><td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.1 "><p id="p1460mcpsimp"><a name="p1460mcpsimp"></a><a name="p1460mcpsimp"></a><span xml:lang="fr-FR" id="ph1461mcpsimp"><a name="ph1461mcpsimp"></a><a name="ph1461mcpsimp"></a>OT_IVE</span>_MAG_AND_ANG_OUT_CTRL_MAG_AND_ANG</p>
</td>
<td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.2 "><p id="p1463mcpsimp"><a name="p1463mcpsimp"></a><a name="p1463mcpsimp"></a>同时输出幅值和角度值。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_mag\_and\_ang\_ctrl](#ot_ive_mag_and_ang_ctrl)

### ot\_ive\_mag\_and\_ang\_ctrl<a name="ZH-CN_TOPIC_0000002470931264"></a>

【说明】

定义梯度幅值和幅角计算的控制信息。

【定义】

```
typedef struct {
    ot_ive_mag_and_ang_out_ctrl out_ctrl;
    td_u16 threshld;
    td_s8 mask[OT_IVE_MASK_NUM]; /* Template parameter. */
} ot_ive_mag_and_ang_ctrl;
```

【成员】

<a name="table16872mcpsimp"></a>
<table><thead align="left"><tr id="row16877mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p16879mcpsimp"><a name="p16879mcpsimp"></a><a name="p16879mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p16881mcpsimp"><a name="p16881mcpsimp"></a><a name="p16881mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16883mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p16885mcpsimp"><a name="p16885mcpsimp"></a><a name="p16885mcpsimp"></a>out_ctrl</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p16887mcpsimp"><a name="p16887mcpsimp"></a><a name="p16887mcpsimp"></a>输出格式。</p>
</td>
</tr>
<tr id="row16888mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p16890mcpsimp"><a name="p16890mcpsimp"></a><a name="p16890mcpsimp"></a>threshold</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p16892mcpsimp"><a name="p16892mcpsimp"></a><a name="p16892mcpsimp"></a>用于对幅值进行阈值化的阈值。</p>
</td>
</tr>
<tr id="row16893mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p16895mcpsimp"><a name="p16895mcpsimp"></a><a name="p16895mcpsimp"></a>mask[<a href="OT_IVE_MASK_NUM.md">OT_IVE_MASK_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p16899mcpsimp"><a name="p16899mcpsimp"></a><a name="p16899mcpsimp"></a>5x5模板系数。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_mag\_and\_ang\_out\_ctrl](#ot_ive_mag_and_ang_out_ctrl)

### ot\_ive\_dilate\_ctrl<a name="ZH-CN_TOPIC_0000002504091109"></a>

【说明】

定义膨胀控制信息。

【定义】

```
typedef struct {
    td_u8 mask[OT_IVE_MASK_NUM]; /* The template parameter value must be 0 or 255. */
} ot_ive_dilate_ctrl;
```

【成员】

<a name="table5490mcpsimp"></a>
<table><thead align="left"><tr id="row5495mcpsimp"><th class="cellrowborder" valign="top" width="47%" id="mcps1.1.3.1.1"><p id="p5497mcpsimp"><a name="p5497mcpsimp"></a><a name="p5497mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="53%" id="mcps1.1.3.1.2"><p id="p5499mcpsimp"><a name="p5499mcpsimp"></a><a name="p5499mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5501mcpsimp"><td class="cellrowborder" valign="top" width="47%" headers="mcps1.1.3.1.1 "><p id="p5503mcpsimp"><a name="p5503mcpsimp"></a><a name="p5503mcpsimp"></a>mask[<a href="OT_IVE_MASK_NUM.md">OT_IVE_MASK_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="53%" headers="mcps1.1.3.1.2 "><p id="p5507mcpsimp"><a name="p5507mcpsimp"></a><a name="p5507mcpsimp"></a>5x5模板系数。</p>
<p id="p5508mcpsimp"><a name="p5508mcpsimp"></a><a name="p5508mcpsimp"></a>取值范围：0或255。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_erode\_ctrl<a name="ZH-CN_TOPIC_0000002470931312"></a>

【说明】

定义腐蚀控制信息。

【定义】

```
typedef struct {
    td_u8 mask[OT_IVE_MASK_NUM]; /* The template parameter value must be 0 or 255. */
} ot_ive_erode_ctrl;
```

【成员】

<a name="table2628mcpsimp"></a>
<table><thead align="left"><tr id="row2633mcpsimp"><th class="cellrowborder" valign="top" width="47%" id="mcps1.1.3.1.1"><p id="p2635mcpsimp"><a name="p2635mcpsimp"></a><a name="p2635mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="53%" id="mcps1.1.3.1.2"><p id="p2637mcpsimp"><a name="p2637mcpsimp"></a><a name="p2637mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2639mcpsimp"><td class="cellrowborder" valign="top" width="47%" headers="mcps1.1.3.1.1 "><p id="p2641mcpsimp"><a name="p2641mcpsimp"></a><a name="p2641mcpsimp"></a>mask[<a href="OT_IVE_MASK_NUM.md">OT_IVE_MASK_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="53%" headers="mcps1.1.3.1.2 "><p id="p2645mcpsimp"><a name="p2645mcpsimp"></a><a name="p2645mcpsimp"></a>5x5模板系数。</p>
<p id="p2646mcpsimp"><a name="p2646mcpsimp"></a><a name="p2646mcpsimp"></a>取值：0或255。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_threshold\_mode<a name="ZH-CN_TOPIC_0000002504091197"></a>

【说明】

定义图像二值化输出格式。

【定义】

```
typedef enum {
    OT_IVE_THRESHOLD_MODE_BINARY = 0x0,       /* src_val <= low_thr, dst_val = min_val; src_val > low_threshold, dst_val = max_val. */
    OT_IVE_THRESHOLD_MODE_TRUNC = 0x1,        /* src_val <= low_threshold, dst_val = src_val; src_val > low_threshold, dst_val = max_val. */
    OT_IVE_THRESHOLD_MODE_TO_MIN_VAL = 0x2,    /* src_val <= low_threshold, dst_val = min_val; src_val > low_threshold, dst_val = src_val. */
    OT_IVE_THRESHOLD_MODE_MIN_MID_MAX = 0x3,  /* src_val <= low_threshold, dst_val = min_val;  low_threshold < src_val <= high_threshold, dst_val = mid_val; src_val > high_threshold, dst_val = max_val. */
    OT_IVE_THRESHOLD_MODE_ORIG_MID_MAX = 0x4,  /* src_val <= low_threshold, dst_val = src_val;  low_threshold < src_val <= high_threshold, dst_val = mid_val; src_val > high_threshold, dst_val = max_val. */
    OT_IVE_THRESHOLD_MODE_MIN_MID_ORI = 0x5,  /* src_val <= low_threshold, dst_val = min_val;  low_threshold < src_val <= high_threshold, dst_val = mid_val; src_val > high_threshold, dst_val = src_val. */
    OT_IVE_THRESHOLD_MODE_MIN_ORIG_MAX = 0x6,  /* src_val <= low_threshold, dst_val = min_val;  low_threshold < src_val <= high_threshold, dst_val = src_val; src_val > high_threshold, dst_val = max_val. */
    OT_IVE_THRESHOLD_MODE_ORI_MID_ORIG = 0x7,  /* src_val <= low_threshold, dst_val = src_val;  low_threshold < src_val <= high_threshold, dst_val = mid_val; src_val > high_threshold, dst_val = src_val. */
    OT_IVE_THRESHOLD_MODE_BUTT
} ot_ive_threshold_mode;
```

【成员】

<a name="table975mcpsimp"></a>
<table><thead align="left"><tr id="row980mcpsimp"><th class="cellrowborder" valign="top" width="49.1%" id="mcps1.1.3.1.1"><p id="p982mcpsimp"><a name="p982mcpsimp"></a><a name="p982mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="50.9%" id="mcps1.1.3.1.2"><p id="p984mcpsimp"><a name="p984mcpsimp"></a><a name="p984mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row986mcpsimp"><td class="cellrowborder" valign="top" width="49.1%" headers="mcps1.1.3.1.1 "><p id="p988mcpsimp"><a name="p988mcpsimp"></a><a name="p988mcpsimp"></a>OT_IVE_THRESHOLD_MODE_BINARY</p>
</td>
<td class="cellrowborder" valign="top" width="50.9%" headers="mcps1.1.3.1.2 "><p id="p990mcpsimp"><a name="p990mcpsimp"></a><a name="p990mcpsimp"></a>src_val ≤ low_threshold, dst_val = min_val; src_val &gt; low_threshold, dst_val = max_val。</p>
</td>
</tr>
<tr id="row991mcpsimp"><td class="cellrowborder" valign="top" width="49.1%" headers="mcps1.1.3.1.1 "><p id="p993mcpsimp"><a name="p993mcpsimp"></a><a name="p993mcpsimp"></a>OT_IVE_THRESHOLD_MODE_TRUNC</p>
</td>
<td class="cellrowborder" valign="top" width="50.9%" headers="mcps1.1.3.1.2 "><p id="p995mcpsimp"><a name="p995mcpsimp"></a><a name="p995mcpsimp"></a>src_val ≤ low_threshold, dst_val = src_val;</p>
<p id="p996mcpsimp"><a name="p996mcpsimp"></a><a name="p996mcpsimp"></a>src_val &gt; low_threshold, dst_val = max_val。</p>
</td>
</tr>
<tr id="row997mcpsimp"><td class="cellrowborder" valign="top" width="49.1%" headers="mcps1.1.3.1.1 "><p id="p999mcpsimp"><a name="p999mcpsimp"></a><a name="p999mcpsimp"></a>OT_IVE_THRESHOLD_MODE_TO_MIN_VAL</p>
</td>
<td class="cellrowborder" valign="top" width="50.9%" headers="mcps1.1.3.1.2 "><p id="p1001mcpsimp"><a name="p1001mcpsimp"></a><a name="p1001mcpsimp"></a>src_val ≤low_threshold, dst_val = min_val;</p>
<p id="p1002mcpsimp"><a name="p1002mcpsimp"></a><a name="p1002mcpsimp"></a>src_val &gt; low_threshold, dst_val = src_val。</p>
</td>
</tr>
<tr id="row1003mcpsimp"><td class="cellrowborder" valign="top" width="49.1%" headers="mcps1.1.3.1.1 "><p id="p1005mcpsimp"><a name="p1005mcpsimp"></a><a name="p1005mcpsimp"></a>OT_IVE_THRESHOLD_MODE_MIN_MID_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="50.9%" headers="mcps1.1.3.1.2 "><p id="p1007mcpsimp"><a name="p1007mcpsimp"></a><a name="p1007mcpsimp"></a>src_val ≤ low_threshold, dst_val = min_val; low_threshold &lt; src_val ≤ high_threshold,</p>
<p id="p1008mcpsimp"><a name="p1008mcpsimp"></a><a name="p1008mcpsimp"></a>dst_val = mid_val;</p>
<p id="p1009mcpsimp"><a name="p1009mcpsimp"></a><a name="p1009mcpsimp"></a>src_val &gt; high_threshold, dst_val = max_val。</p>
</td>
</tr>
<tr id="row1010mcpsimp"><td class="cellrowborder" valign="top" width="49.1%" headers="mcps1.1.3.1.1 "><p id="p1012mcpsimp"><a name="p1012mcpsimp"></a><a name="p1012mcpsimp"></a>OT_IVE_THRESHOLD_MODE_ORIG_MID_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="50.9%" headers="mcps1.1.3.1.2 "><p id="p1014mcpsimp"><a name="p1014mcpsimp"></a><a name="p1014mcpsimp"></a>src_val ≤low_threshold, dst_val = src_val;</p>
<p id="p1015mcpsimp"><a name="p1015mcpsimp"></a><a name="p1015mcpsimp"></a>low_threshold &lt; src_val ≤ high_threshold,</p>
<p id="p1016mcpsimp"><a name="p1016mcpsimp"></a><a name="p1016mcpsimp"></a>dst_val = mid_val;</p>
<p id="p1017mcpsimp"><a name="p1017mcpsimp"></a><a name="p1017mcpsimp"></a>src_val &gt; high_threshold, dst_val = max_val。</p>
</td>
</tr>
<tr id="row1018mcpsimp"><td class="cellrowborder" valign="top" width="49.1%" headers="mcps1.1.3.1.1 "><p id="p1020mcpsimp"><a name="p1020mcpsimp"></a><a name="p1020mcpsimp"></a>OT_IVE_THRESHOLD_MODE_MIN_MID_ORI</p>
</td>
<td class="cellrowborder" valign="top" width="50.9%" headers="mcps1.1.3.1.2 "><p id="p1022mcpsimp"><a name="p1022mcpsimp"></a><a name="p1022mcpsimp"></a>src_val ≤low_threshold, dst_val = min_val;</p>
<p id="p1023mcpsimp"><a name="p1023mcpsimp"></a><a name="p1023mcpsimp"></a>low_threshold &lt; src_val ≤high_threshold,</p>
<p id="p1024mcpsimp"><a name="p1024mcpsimp"></a><a name="p1024mcpsimp"></a>dst_val = mid_val;</p>
<p id="p1025mcpsimp"><a name="p1025mcpsimp"></a><a name="p1025mcpsimp"></a>src_val &gt; high_threshold, dst_val = src_val。</p>
</td>
</tr>
<tr id="row1026mcpsimp"><td class="cellrowborder" valign="top" width="49.1%" headers="mcps1.1.3.1.1 "><p id="p1028mcpsimp"><a name="p1028mcpsimp"></a><a name="p1028mcpsimp"></a>OT_IVE_THRESHOLD_MODE_MIN_ORIG_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="50.9%" headers="mcps1.1.3.1.2 "><p id="p1030mcpsimp"><a name="p1030mcpsimp"></a><a name="p1030mcpsimp"></a>src_val ≤ low_threshold, dst_val = min_val;  low_threshold &lt; src_val ≤ high_threshold,</p>
<p id="p1031mcpsimp"><a name="p1031mcpsimp"></a><a name="p1031mcpsimp"></a>dst_val = src_val;</p>
<p id="p1032mcpsimp"><a name="p1032mcpsimp"></a><a name="p1032mcpsimp"></a>src_val &gt; high_threshold, dst_val = max_val。</p>
</td>
</tr>
<tr id="row1033mcpsimp"><td class="cellrowborder" valign="top" width="49.1%" headers="mcps1.1.3.1.1 "><p id="p1035mcpsimp"><a name="p1035mcpsimp"></a><a name="p1035mcpsimp"></a>OT_IVE_THRESHOLD_MODE_ORI_MID_ORIG</p>
</td>
<td class="cellrowborder" valign="top" width="50.9%" headers="mcps1.1.3.1.2 "><p id="p1037mcpsimp"><a name="p1037mcpsimp"></a><a name="p1037mcpsimp"></a>src_val≤ low_threshold, dst_val = src_val;</p>
<p id="p1038mcpsimp"><a name="p1038mcpsimp"></a><a name="p1038mcpsimp"></a>low_threshold &lt; src_val ≤ high_threshold,</p>
<p id="p1039mcpsimp"><a name="p1039mcpsimp"></a><a name="p1039mcpsimp"></a>dst_val = mid_val;</p>
<p id="p1040mcpsimp"><a name="p1040mcpsimp"></a><a name="p1040mcpsimp"></a>src_val &gt; high_threshold, dst_val = src_val。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式请参见ss\_mpi\_ive\_threshold中的【注意】，示意图请参见threshold 8种阈值化模式示意图。

【相关数据类型及接口】

[ot\_ive\_threshold\_ctrl](#ot_ive_threshold_ctrl)

### ot\_ive\_threshold\_ctrl<a name="ZH-CN_TOPIC_0000002504091163"></a>

【说明】

定义图像二值化控制信息。

【定义】

```
typedef struct {
    ot_ive_threshold_mode mode;
    td_u8 low_threshold;  /* user-defined threshold,  0<=u8LowThr<=255 */
    td_u8 high_threshold; /* user-defined threshold, if mode<OT_IVE_THRESHOLD_MODE_MIN_MID_MAX, high_threshold is not used,
                      else 0<=low_threshold<= high_threshold <=255; */
    td_u8 min_val;  /* Minimum value when tri-level thresholding */
    td_u8 mid_val;  /* Middle value when tri-level thresholding, if enMode<2, u32MidVal is not used; */
    td_u8 max_val;  /* Maxmum value when tri-level thresholding */
} ot_ive_thresh_ctrl;
```

【成员】

<a name="table829mcpsimp"></a>
<table><thead align="left"><tr id="row834mcpsimp"><th class="cellrowborder" valign="top" width="32%" id="mcps1.1.3.1.1"><p id="p836mcpsimp"><a name="p836mcpsimp"></a><a name="p836mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.3.1.2"><p id="p838mcpsimp"><a name="p838mcpsimp"></a><a name="p838mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row840mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p842mcpsimp"><a name="p842mcpsimp"></a><a name="p842mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p844mcpsimp"><a name="p844mcpsimp"></a><a name="p844mcpsimp"></a>阈值化运算模式。</p>
</td>
</tr>
<tr id="row845mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p847mcpsimp"><a name="p847mcpsimp"></a><a name="p847mcpsimp"></a>low_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p849mcpsimp"><a name="p849mcpsimp"></a><a name="p849mcpsimp"></a>低阈值。</p>
<p id="p850mcpsimp"><a name="p850mcpsimp"></a><a name="p850mcpsimp"></a>取值范围：[0,255]</p>
</td>
</tr>
<tr id="row851mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p853mcpsimp"><a name="p853mcpsimp"></a><a name="p853mcpsimp"></a>high_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p855mcpsimp"><a name="p855mcpsimp"></a><a name="p855mcpsimp"></a>高阈值。</p>
<p id="p856mcpsimp"><a name="p856mcpsimp"></a><a name="p856mcpsimp"></a>0≤low_thresholdesh≤high_thresholdesh≤255。</p>
</td>
</tr>
<tr id="row857mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p859mcpsimp"><a name="p859mcpsimp"></a><a name="p859mcpsimp"></a>min_val</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p861mcpsimp"><a name="p861mcpsimp"></a><a name="p861mcpsimp"></a>最小值。</p>
<p id="p862mcpsimp"><a name="p862mcpsimp"></a><a name="p862mcpsimp"></a>取值范围：[0,255]</p>
</td>
</tr>
<tr id="row863mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p865mcpsimp"><a name="p865mcpsimp"></a><a name="p865mcpsimp"></a>mid_val</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p867mcpsimp"><a name="p867mcpsimp"></a><a name="p867mcpsimp"></a>中间值。</p>
<p id="p868mcpsimp"><a name="p868mcpsimp"></a><a name="p868mcpsimp"></a>取值范围：[0,255]</p>
</td>
</tr>
<tr id="row869mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p871mcpsimp"><a name="p871mcpsimp"></a><a name="p871mcpsimp"></a>max_val</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p873mcpsimp"><a name="p873mcpsimp"></a><a name="p873mcpsimp"></a>最大值。</p>
<p id="p874mcpsimp"><a name="p874mcpsimp"></a><a name="p874mcpsimp"></a>取值范围：[0,255]</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_threshold\_mode](#ot_ive_threshold_mode)

### ot\_ive\_sub\_mode<a name="ZH-CN_TOPIC_0000002470931336"></a>

【说明】

定义两图像相减输出格式。

【定义】

```
typedef enum {
    OT_IVE_SUB_MODE_ABS = 0x0,   /* Absolute value of the difference */
    OT_IVE_SUB_MODE_SHIFT = 0x1, /* The output result is obtained by shifting   the result one digit right to reserve the signed bit. */
    OT_IVE_SUB_MODE_BUTT
} ot_ive_sub_mode;
```

【成员】

<a name="table17120mcpsimp"></a>
<table><thead align="left"><tr id="row17125mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p17127mcpsimp"><a name="p17127mcpsimp"></a><a name="p17127mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p17129mcpsimp"><a name="p17129mcpsimp"></a><a name="p17129mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row17131mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p17133mcpsimp"><a name="p17133mcpsimp"></a><a name="p17133mcpsimp"></a>OT_IVE_SUB_MODE_ABS</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p17135mcpsimp"><a name="p17135mcpsimp"></a><a name="p17135mcpsimp"></a>取差的绝对值。</p>
</td>
</tr>
<tr id="row17136mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p17138mcpsimp"><a name="p17138mcpsimp"></a><a name="p17138mcpsimp"></a>OT_IVE_SUB_MODE_SHIFT</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p17140mcpsimp"><a name="p17140mcpsimp"></a><a name="p17140mcpsimp"></a>将结果右移一位输出，保留符号位。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_sub\_ctrl](#ot_ive_sub_ctrl)

### ot\_ive\_sub\_ctrl<a name="ZH-CN_TOPIC_0000002471091290"></a>

【说明】

定义两图像相减控制参数。

【定义】

```
typedef struct {
    ot_ive_sub_mode mode;
} ot_ive_sub_ctrl;
```

【成员】

<a name="table10341mcpsimp"></a>
<table><thead align="left"><tr id="row10346mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p10348mcpsimp"><a name="p10348mcpsimp"></a><a name="p10348mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p10350mcpsimp"><a name="p10350mcpsimp"></a><a name="p10350mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10352mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p10354mcpsimp"><a name="p10354mcpsimp"></a><a name="p10354mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p10356mcpsimp"><a name="p10356mcpsimp"></a><a name="p10356mcpsimp"></a>两图像相减模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_sub\_mode](#ot_ive_sub_mode)

### ot\_ive\_integ\_out\_ctrl<a name="ZH-CN_TOPIC_0000002504091081"></a>

【说明】

定义积分图输出控制参数。

【定义】

```
typedef enum ot_ive_integ_out_ctrl {
    OT_IVE_INTEG_OUT_CTRL_COMBINE  =  0x0,
    OT_IVE_INTEG_OUT_CTRL_SUM	     =  0x1,
    OT_IVE_INTEG_OUT_CTRL_SQRT_SUM    =  0x2,
    OT_IVE_INTEG_OUT_CTRL_BUTT
}ot_ive_integ_out_ctrl;
```

【成员】

<a name="table1337mcpsimp"></a>
<table><thead align="left"><tr id="row1342mcpsimp"><th class="cellrowborder" valign="top" width="53%" id="mcps1.1.3.1.1"><p id="p1344mcpsimp"><a name="p1344mcpsimp"></a><a name="p1344mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="47%" id="mcps1.1.3.1.2"><p id="p1346mcpsimp"><a name="p1346mcpsimp"></a><a name="p1346mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1348mcpsimp"><td class="cellrowborder" valign="top" width="53%" headers="mcps1.1.3.1.1 "><p id="p1350mcpsimp"><a name="p1350mcpsimp"></a><a name="p1350mcpsimp"></a>OT_IVE_INTEG_OUT_CTRL_COMBINE</p>
</td>
<td class="cellrowborder" valign="top" width="47%" headers="mcps1.1.3.1.2 "><p id="p1352mcpsimp"><a name="p1352mcpsimp"></a><a name="p1352mcpsimp"></a>和、平方和积分图组合输出，如积分图（OT_SVP_IMG_TYPE_U64C1）组合输出示意图。</p>
</td>
</tr>
<tr id="row1354mcpsimp"><td class="cellrowborder" valign="top" width="53%" headers="mcps1.1.3.1.1 "><p id="p1356mcpsimp"><a name="p1356mcpsimp"></a><a name="p1356mcpsimp"></a>OT_IVE_INTEG_OUT_CTRL_SUM</p>
</td>
<td class="cellrowborder" valign="top" width="47%" headers="mcps1.1.3.1.2 "><p id="p1358mcpsimp"><a name="p1358mcpsimp"></a><a name="p1358mcpsimp"></a>仅和积分图输出。</p>
</td>
</tr>
<tr id="row1359mcpsimp"><td class="cellrowborder" valign="top" width="53%" headers="mcps1.1.3.1.1 "><p id="p1361mcpsimp"><a name="p1361mcpsimp"></a><a name="p1361mcpsimp"></a>OT_IVE_INTEG_OUT_CTRL_SQRT_SUM</p>
</td>
<td class="cellrowborder" valign="top" width="47%" headers="mcps1.1.3.1.2 "><p id="p1363mcpsimp"><a name="p1363mcpsimp"></a><a name="p1363mcpsimp"></a>仅平方和积分图输出。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_integ\_ctrl](#ot_ive_integ_ctrl)

### ot\_ive\_integ\_ctrl<a name="ZH-CN_TOPIC_0000002504091115"></a>

【说明】

定义积分图计算控制参数。

【定义】

```
typedef struct {
    ot_ive_integ_out_ctrl out_ctrl;
} ot_ive_integ_ctrl;
```

【成员】

<a name="table5522mcpsimp"></a>
<table><thead align="left"><tr id="row5527mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p5529mcpsimp"><a name="p5529mcpsimp"></a><a name="p5529mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p5531mcpsimp"><a name="p5531mcpsimp"></a><a name="p5531mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5533mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p5535mcpsimp"><a name="p5535mcpsimp"></a><a name="p5535mcpsimp"></a>out_ctrl</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p5537mcpsimp"><a name="p5537mcpsimp"></a><a name="p5537mcpsimp"></a>积分图输出控制参数</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_integ\_out\_ctrl](#ot_ive_integ_out_ctrl)

### ot\_ive\_threshold\_s16\_mode<a name="ZH-CN_TOPIC_0000002504091143"></a>

【说明】

定义16bit有符号图像的阈值化模式。

【定义】

```
typedef enum {
    OT_IVE_THRESHOLD_S16_MODE_S16_TO_S8_MIN_MID_MAX = 0x0,
    OT_IVE_THRESHOLD_S16_MODE_S16_TO_S8_MIN_ORIG_MAX = 0x1,
    OT_IVE_THRESHOLD_S16_MODE_S16_TO_U8_MIN_MID_MAX = 0x2,
    OT_IVE_THRESHOLD_S16_MODE_S16_TO_U8_MIN_ORIG_MAX = 0x3,
    OT_IVE_THRESHOLD_S16_MODE_BUTT
} ot_ive_threshold_s16_mode;
```

【成员】

<a name="table892mcpsimp"></a>
<table><thead align="left"><tr id="row897mcpsimp"><th class="cellrowborder" valign="top" width="41%" id="mcps1.1.3.1.1"><p id="p899mcpsimp"><a name="p899mcpsimp"></a><a name="p899mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="59%" id="mcps1.1.3.1.2"><p id="p901mcpsimp"><a name="p901mcpsimp"></a><a name="p901mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row903mcpsimp"><td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.1 "><p id="p905mcpsimp"><a name="p905mcpsimp"></a><a name="p905mcpsimp"></a>OT_IVE_THRESHOLD_S16_MODE_S16_TO_S8_MIN_MID_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.2 "><p id="p907mcpsimp"><a name="p907mcpsimp"></a><a name="p907mcpsimp"></a>src_val ≤ low_threshold,</p>
<p id="p908mcpsimp"><a name="p908mcpsimp"></a><a name="p908mcpsimp"></a>dst_val = min_val;</p>
<p id="p909mcpsimp"><a name="p909mcpsimp"></a><a name="p909mcpsimp"></a>low_threshold &lt; src_val ≤high_threshold,</p>
<p id="p910mcpsimp"><a name="p910mcpsimp"></a><a name="p910mcpsimp"></a>dst_val = mid_val;</p>
<p id="p911mcpsimp"><a name="p911mcpsimp"></a><a name="p911mcpsimp"></a>src_val &gt; high_threshold,</p>
<p id="p912mcpsimp"><a name="p912mcpsimp"></a><a name="p912mcpsimp"></a>dst_val = max_val;</p>
</td>
</tr>
<tr id="row913mcpsimp"><td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.1 "><p id="p915mcpsimp"><a name="p915mcpsimp"></a><a name="p915mcpsimp"></a>OT_IVE_THRESHOLD_S16_MODE_S16_TO_S8_MIN_ORIG_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.2 "><p id="p917mcpsimp"><a name="p917mcpsimp"></a><a name="p917mcpsimp"></a>src_val ≤ low_threshold,</p>
<p id="p918mcpsimp"><a name="p918mcpsimp"></a><a name="p918mcpsimp"></a>dst_val = min_val;</p>
<p id="p919mcpsimp"><a name="p919mcpsimp"></a><a name="p919mcpsimp"></a>low_threshold &lt; src_val ≤high_threshold,</p>
<p id="p920mcpsimp"><a name="p920mcpsimp"></a><a name="p920mcpsimp"></a>dst_val = src_val;</p>
<p id="p921mcpsimp"><a name="p921mcpsimp"></a><a name="p921mcpsimp"></a>src_val &gt; high_threshold,</p>
<p id="p922mcpsimp"><a name="p922mcpsimp"></a><a name="p922mcpsimp"></a>dst_val = max_val;</p>
</td>
</tr>
<tr id="row923mcpsimp"><td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.1 "><p id="p925mcpsimp"><a name="p925mcpsimp"></a><a name="p925mcpsimp"></a>OT_IVE_THRESHOLD_S16_MODE_S16_TO_U8_MIN_MID_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.2 "><p id="p927mcpsimp"><a name="p927mcpsimp"></a><a name="p927mcpsimp"></a>src_val ≤ low_threshold,</p>
<p id="p928mcpsimp"><a name="p928mcpsimp"></a><a name="p928mcpsimp"></a>dst_val = min_val;</p>
<p id="p929mcpsimp"><a name="p929mcpsimp"></a><a name="p929mcpsimp"></a>low_threshold &lt; src_val ≤high_threshold,</p>
<p id="p930mcpsimp"><a name="p930mcpsimp"></a><a name="p930mcpsimp"></a>dst_val = mid_val;</p>
<p id="p931mcpsimp"><a name="p931mcpsimp"></a><a name="p931mcpsimp"></a>src_val &gt; high_threshold,</p>
<p id="p932mcpsimp"><a name="p932mcpsimp"></a><a name="p932mcpsimp"></a>dst_val = max_val;</p>
</td>
</tr>
<tr id="row933mcpsimp"><td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.1 "><p id="p935mcpsimp"><a name="p935mcpsimp"></a><a name="p935mcpsimp"></a>OT_IVE_THRESHOLD_S16_MODE_S16_TO_U8_MIN_ORIG_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.2 "><p id="p937mcpsimp"><a name="p937mcpsimp"></a><a name="p937mcpsimp"></a>src_val ≤ low_threshold,</p>
<p id="p938mcpsimp"><a name="p938mcpsimp"></a><a name="p938mcpsimp"></a>dst_val = min_val;</p>
<p id="p939mcpsimp"><a name="p939mcpsimp"></a><a name="p939mcpsimp"></a>low_threshold &lt; src_val ≤high_threshold,</p>
<p id="p940mcpsimp"><a name="p940mcpsimp"></a><a name="p940mcpsimp"></a>dst_val = src_val;</p>
<p id="p941mcpsimp"><a name="p941mcpsimp"></a><a name="p941mcpsimp"></a>src_val &gt; high_threshold,</p>
<p id="p942mcpsimp"><a name="p942mcpsimp"></a><a name="p942mcpsimp"></a>dst_val = max_val;</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式请参见ss\_mpi\_ive\_threshold\_s16中的【注意】，示意图请参见 threshold\_s16 4种阈值化模式示意图。

【相关数据类型及接口】

[ot\_ive\_threshold\_s16\_ctrl](#ot_ive_threshold_s16_ctrl)

### ot\_ive\_threshold\_s16\_ctrl<a name="ZH-CN_TOPIC_0000002504091125"></a>

【说明】

定义16bit有符号图像的阈值化控制参数。

【定义】

```
typedef struct {
    ot_ive_threshold_s16_mode mode;
    td_s16 low_threshold;         /* User-defined threshold */
    td_s16 high_threshold;        /* User-defined threshold */
    ot_svp_8bit min_val;   /* Minimum value when tri-level thresholding */
    ot_svp_8bit mid_val;    /* Middle value when tri-level thresholding */
    ot_svp_8bit max_val;    /* Maxmum value when tri-level thresholding */
} ot_ive_threshold_s16_ctrl;
```

【成员】

<a name="table2850mcpsimp"></a>
<table><thead align="left"><tr id="row2855mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p2857mcpsimp"><a name="p2857mcpsimp"></a><a name="p2857mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p2859mcpsimp"><a name="p2859mcpsimp"></a><a name="p2859mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2861mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2863mcpsimp"><a name="p2863mcpsimp"></a><a name="p2863mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2865mcpsimp"><a name="p2865mcpsimp"></a><a name="p2865mcpsimp"></a>阈值化运算模式。</p>
</td>
</tr>
<tr id="row2866mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2868mcpsimp"><a name="p2868mcpsimp"></a><a name="p2868mcpsimp"></a>low_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2870mcpsimp"><a name="p2870mcpsimp"></a><a name="p2870mcpsimp"></a>低阈值。</p>
</td>
</tr>
<tr id="row2871mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2873mcpsimp"><a name="p2873mcpsimp"></a><a name="p2873mcpsimp"></a>high_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2875mcpsimp"><a name="p2875mcpsimp"></a><a name="p2875mcpsimp"></a>高阈值。</p>
</td>
</tr>
<tr id="row2876mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2878mcpsimp"><a name="p2878mcpsimp"></a><a name="p2878mcpsimp"></a>min_val</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2880mcpsimp"><a name="p2880mcpsimp"></a><a name="p2880mcpsimp"></a>最小值。</p>
</td>
</tr>
<tr id="row2881mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2883mcpsimp"><a name="p2883mcpsimp"></a><a name="p2883mcpsimp"></a>mid_val</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2885mcpsimp"><a name="p2885mcpsimp"></a><a name="p2885mcpsimp"></a>中间值。</p>
</td>
</tr>
<tr id="row2886mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2888mcpsimp"><a name="p2888mcpsimp"></a><a name="p2888mcpsimp"></a>max_val</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2890mcpsimp"><a name="p2890mcpsimp"></a><a name="p2890mcpsimp"></a>最大值。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式请参见ss\_mpi\_ive\_threshold\_s16中的【注意】，示意图请参见threshold\_s16 4种阈值化模式示意图。

【相关数据类型及接口】

[ot\_ive\_threshold\_s16\_mode](#ot_ive_threshold_s16_mode)

### ot\_ive\_threshold\_u16\_mode<a name="ZH-CN_TOPIC_0000002503971223"></a>

【说明】

定义16bti无符号图像的阈值化模式。

【定义】

```
typedef enum {
    OT_IVE_THRESHOLD_U16_MODE_U16_TO_U8_MIN_MID_MAX = 0x0,
    OT_IVE_THRESHOLD_U16_MODE_U16_TO_U8_MIN_ORIG_MAX = 0x1,
    OT_IVE_THRESHOLD_U16_MODE_BUTT
 } ot_ive_threshold_u16_mode;
```

【成员】

<a name="table16770mcpsimp"></a>
<table><thead align="left"><tr id="row16775mcpsimp"><th class="cellrowborder" valign="top" width="41%" id="mcps1.1.3.1.1"><p id="p16777mcpsimp"><a name="p16777mcpsimp"></a><a name="p16777mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="59%" id="mcps1.1.3.1.2"><p id="p16779mcpsimp"><a name="p16779mcpsimp"></a><a name="p16779mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16781mcpsimp"><td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.1 "><p id="p16783mcpsimp"><a name="p16783mcpsimp"></a><a name="p16783mcpsimp"></a>OT_IVE_THRESHOLD_U16_MODE_U16_TO_U8_MIN_MID_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.2 "><p id="p16785mcpsimp"><a name="p16785mcpsimp"></a><a name="p16785mcpsimp"></a>src_val ≤ low_threshold,</p>
<p id="p16786mcpsimp"><a name="p16786mcpsimp"></a><a name="p16786mcpsimp"></a>dst_val = min_val;</p>
<p id="p16787mcpsimp"><a name="p16787mcpsimp"></a><a name="p16787mcpsimp"></a>low_threshold &lt; src_val ≤high_threshold,</p>
<p id="p16788mcpsimp"><a name="p16788mcpsimp"></a><a name="p16788mcpsimp"></a>dst_val = mid_val;</p>
<p id="p16789mcpsimp"><a name="p16789mcpsimp"></a><a name="p16789mcpsimp"></a>src_val &gt; high_threshold,</p>
<p id="p16790mcpsimp"><a name="p16790mcpsimp"></a><a name="p16790mcpsimp"></a>dst_val = max_val;</p>
</td>
</tr>
<tr id="row16791mcpsimp"><td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.1 "><p id="p16793mcpsimp"><a name="p16793mcpsimp"></a><a name="p16793mcpsimp"></a>OT_IVE_THRESHOLD_U16_MODE_U16_TO_U8_MIN_ORIG_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.2 "><p id="p16795mcpsimp"><a name="p16795mcpsimp"></a><a name="p16795mcpsimp"></a>src_val ≤ low_threshold,</p>
<p id="p16796mcpsimp"><a name="p16796mcpsimp"></a><a name="p16796mcpsimp"></a>dst_val = min_val;</p>
<p id="p16797mcpsimp"><a name="p16797mcpsimp"></a><a name="p16797mcpsimp"></a>low_threshold &lt; src_val ≤high_threshold,</p>
<p id="p16798mcpsimp"><a name="p16798mcpsimp"></a><a name="p16798mcpsimp"></a>dst_val = src_val;</p>
<p id="p16799mcpsimp"><a name="p16799mcpsimp"></a><a name="p16799mcpsimp"></a>src_val &gt; high_threshold,</p>
<p id="p16800mcpsimp"><a name="p16800mcpsimp"></a><a name="p16800mcpsimp"></a>dst_val = max_val;</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式请参见ss\_mpi\_ive\_threshold\_u16中的【注意】，示意图请参见threshold\_u16 2种阈值化模式示意图。

【相关数据类型及接口】

[ot\_ive\_threshold\_u16\_ctrl](#ot_ive_threshold_u16_ctrl)

### ot\_ive\_threshold\_u16\_ctrl<a name="ZH-CN_TOPIC_0000002504091089"></a>

【说明】

定义16bit无符号图像的阈值化控制参数。

【定义】

```
typedef struct {
    ot_ive_threshold_u16_mode mode;
    td_u16 low_threshold;
    td_u16 high_threshold;
    td_u8 min_val;
    td_u8 mid_val;
    td_u8 max_val;
} ot_ive_threshold_u16_ctrl;
```

【成员】

<a name="table11528mcpsimp"></a>
<table><thead align="left"><tr id="row11533mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p11535mcpsimp"><a name="p11535mcpsimp"></a><a name="p11535mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p11537mcpsimp"><a name="p11537mcpsimp"></a><a name="p11537mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row11539mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p11541mcpsimp"><a name="p11541mcpsimp"></a><a name="p11541mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p11543mcpsimp"><a name="p11543mcpsimp"></a><a name="p11543mcpsimp"></a>阈值化运算模式。</p>
</td>
</tr>
<tr id="row11544mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p11546mcpsimp"><a name="p11546mcpsimp"></a><a name="p11546mcpsimp"></a>low_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p11548mcpsimp"><a name="p11548mcpsimp"></a><a name="p11548mcpsimp"></a>低阈值。</p>
</td>
</tr>
<tr id="row11549mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p11551mcpsimp"><a name="p11551mcpsimp"></a><a name="p11551mcpsimp"></a>high_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p11553mcpsimp"><a name="p11553mcpsimp"></a><a name="p11553mcpsimp"></a>高阈值。</p>
</td>
</tr>
<tr id="row11554mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p11556mcpsimp"><a name="p11556mcpsimp"></a><a name="p11556mcpsimp"></a>min_val</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p11558mcpsimp"><a name="p11558mcpsimp"></a><a name="p11558mcpsimp"></a>最小值。</p>
<p id="p11559mcpsimp"><a name="p11559mcpsimp"></a><a name="p11559mcpsimp"></a>取值范围：[0,255]</p>
</td>
</tr>
<tr id="row11560mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p11562mcpsimp"><a name="p11562mcpsimp"></a><a name="p11562mcpsimp"></a>mid_val</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p11564mcpsimp"><a name="p11564mcpsimp"></a><a name="p11564mcpsimp"></a>中间值。</p>
<p id="p11565mcpsimp"><a name="p11565mcpsimp"></a><a name="p11565mcpsimp"></a>取值范围：[0,255]</p>
</td>
</tr>
<tr id="row11566mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p11568mcpsimp"><a name="p11568mcpsimp"></a><a name="p11568mcpsimp"></a>max_val</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p11570mcpsimp"><a name="p11570mcpsimp"></a><a name="p11570mcpsimp"></a>最大值。</p>
<p id="p11571mcpsimp"><a name="p11571mcpsimp"></a><a name="p11571mcpsimp"></a>取值范围：[0,255]</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式请参见ss\_mpi\_ive\_threshold\_u16中的【注意】，示意图请参见threshold\_u16 2种阈值化模式示意图。

【相关数据类型及接口】

[ot\_ive\_threshold\_u16\_mode](#ot_ive_threshold_u16_mode)

### ot\_ive\_16bit\_to\_8bit\_mode<a name="ZH-CN_TOPIC_0000002471091260"></a>

【说明】

定义16bit图像数据到8bit图像数据的的转化模式。

【定义】

```
typedef enum {
    OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_S8 = 0x0,
    OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_ABS = 0x1,
    OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_BIAS = 0x2,
    OT_IVE_16BIT_TO_8BIT_MODE_U16_TO_U8 = 0x3,
    OT_IVE_16BIT_TO_8BIT_MODE_BUTT
} ot_ive_16bit_to_8bit_mode;
```

【成员】

<a name="table10244mcpsimp"></a>
<table><thead align="left"><tr id="row10249mcpsimp"><th class="cellrowborder" valign="top" width="66%" id="mcps1.1.3.1.1"><p id="p10251mcpsimp"><a name="p10251mcpsimp"></a><a name="p10251mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="34%" id="mcps1.1.3.1.2"><p id="p10253mcpsimp"><a name="p10253mcpsimp"></a><a name="p10253mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10255mcpsimp"><td class="cellrowborder" valign="top" width="66%" headers="mcps1.1.3.1.1 "><p id="p10257mcpsimp"><a name="p10257mcpsimp"></a><a name="p10257mcpsimp"></a>OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_S8</p>
</td>
<td class="cellrowborder" valign="top" width="34%" headers="mcps1.1.3.1.2 "><p id="p10259mcpsimp"><a name="p10259mcpsimp"></a><a name="p10259mcpsimp"></a>S16数据到S8数据的线性变换。</p>
</td>
</tr>
<tr id="row10260mcpsimp"><td class="cellrowborder" valign="top" width="66%" headers="mcps1.1.3.1.1 "><p id="p10262mcpsimp"><a name="p10262mcpsimp"></a><a name="p10262mcpsimp"></a>OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_ABS</p>
</td>
<td class="cellrowborder" valign="top" width="34%" headers="mcps1.1.3.1.2 "><p id="p10264mcpsimp"><a name="p10264mcpsimp"></a><a name="p10264mcpsimp"></a>S16数据线性变换到S8数据后取绝对值得到S8数据。</p>
</td>
</tr>
<tr id="row10265mcpsimp"><td class="cellrowborder" valign="top" width="66%" headers="mcps1.1.3.1.1 "><p id="p10267mcpsimp"><a name="p10267mcpsimp"></a><a name="p10267mcpsimp"></a>OT_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_BIAS</p>
</td>
<td class="cellrowborder" valign="top" width="34%" headers="mcps1.1.3.1.2 "><p id="p10269mcpsimp"><a name="p10269mcpsimp"></a><a name="p10269mcpsimp"></a>S16数据线性变换到S8数据且平移后截断到U8数据。</p>
</td>
</tr>
<tr id="row10270mcpsimp"><td class="cellrowborder" valign="top" width="66%" headers="mcps1.1.3.1.1 "><p id="p10272mcpsimp"><a name="p10272mcpsimp"></a><a name="p10272mcpsimp"></a>OT_IVE_16BIT_TO_8BIT_MODE_U16_TO_U8</p>
</td>
<td class="cellrowborder" valign="top" width="34%" headers="mcps1.1.3.1.2 "><p id="p10274mcpsimp"><a name="p10274mcpsimp"></a><a name="p10274mcpsimp"></a>U16数据线性变换到U8数据。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式请参见ss\_mpi\_ive\_16bit\_to\_8bit中的【注意】，示意图请参见16bit\_to\_8bit 4种转换模式示意图。

【相关数据类型及接口】

[ot\_ive\_16bit\_to\_8bit\_ctrl](#ot_ive_16bit_to_8bit_ctrl)

### ot\_ive\_16bit\_to\_8bit\_ctrl<a name="ZH-CN_TOPIC_0000002470931316"></a>

【说明】

定义16bit图像数据到8bit图像数据的转化控制参数。

【定义】

```
typedef struct {
    ot_ive_16bit_to_8bit_mode mode;
    td_u16 denominator;
    td_u8 num;
    td_s8 bias;
} ot_ive_16bit_to_8bit_ctrl;
```

【成员】

<a name="table1596mcpsimp"></a>
<table><thead align="left"><tr id="row1601mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p1603mcpsimp"><a name="p1603mcpsimp"></a><a name="p1603mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p1605mcpsimp"><a name="p1605mcpsimp"></a><a name="p1605mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1607mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p1609mcpsimp"><a name="p1609mcpsimp"></a><a name="p1609mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p1611mcpsimp"><a name="p1611mcpsimp"></a><a name="p1611mcpsimp"></a>16bit数据到8bit数据的转换模式。</p>
</td>
</tr>
<tr id="row1612mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p1614mcpsimp"><a name="p1614mcpsimp"></a><a name="p1614mcpsimp"></a>denominator</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p1616mcpsimp"><a name="p1616mcpsimp"></a><a name="p1616mcpsimp"></a>线性变换中的分母。</p>
<p id="p1617mcpsimp"><a name="p1617mcpsimp"></a><a name="p1617mcpsimp"></a>取值范围：[max{1,num}, 65535]</p>
</td>
</tr>
<tr id="row1618mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p1620mcpsimp"><a name="p1620mcpsimp"></a><a name="p1620mcpsimp"></a>num</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p1622mcpsimp"><a name="p1622mcpsimp"></a><a name="p1622mcpsimp"></a>线性变换中的分子。</p>
<p id="p1623mcpsimp"><a name="p1623mcpsimp"></a><a name="p1623mcpsimp"></a>取值范围：[0,255]。</p>
</td>
</tr>
<tr id="row1624mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p1626mcpsimp"><a name="p1626mcpsimp"></a><a name="p1626mcpsimp"></a>bias</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p1628mcpsimp"><a name="p1628mcpsimp"></a><a name="p1628mcpsimp"></a>线性变换中的平移项。</p>
<p id="p1629mcpsimp"><a name="p1629mcpsimp"></a><a name="p1629mcpsimp"></a>取值范围：[-128,127]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   计算公式请参见ss\_mpi\_ive\_16bit\_to\_8bit中的【注意】，示意图请参见16bit\_to\_8bit 4种转换模式示意图。
-   num≤denominator，且den≠0；

【相关数据类型及接口】

[ot\_ive\_16bit\_to\_8bit\_mode](#ot_ive_16bit_to_8bit_mode)

### ot\_ive\_order\_stats\_filter\_mode<a name="ZH-CN_TOPIC_0000002470931238"></a>

【说明】

定义顺序统计量滤波模式。

【定义】

```
typedef enum {
    OT_IVE_ORDER_STATS_FILTER_MODE_MEDIAN = 0x0,
    OT_IVE_ORDER_STATS_FILTER_MODE_MAX = 0x1,
    OT_IVE_ORDER_STATS_FILTER_MODE_MIN = 0x2,
    OT_IVE_ORD_STAT_FILTER_MODE_BUTT
} ot_ive_order_stats_filter_mode;
```

【成员】

<a name="table14561mcpsimp"></a>
<table><thead align="left"><tr id="row14566mcpsimp"><th class="cellrowborder" valign="top" width="65%" id="mcps1.1.3.1.1"><p id="p14568mcpsimp"><a name="p14568mcpsimp"></a><a name="p14568mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="35%" id="mcps1.1.3.1.2"><p id="p14570mcpsimp"><a name="p14570mcpsimp"></a><a name="p14570mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14572mcpsimp"><td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.3.1.1 "><p id="p14574mcpsimp"><a name="p14574mcpsimp"></a><a name="p14574mcpsimp"></a>OT_IVE_ORDER_STATS_FILTER_MODE_MEDIAN</p>
</td>
<td class="cellrowborder" valign="top" width="35%" headers="mcps1.1.3.1.2 "><p id="p14576mcpsimp"><a name="p14576mcpsimp"></a><a name="p14576mcpsimp"></a>中值滤波。</p>
</td>
</tr>
<tr id="row14577mcpsimp"><td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.3.1.1 "><p id="p14579mcpsimp"><a name="p14579mcpsimp"></a><a name="p14579mcpsimp"></a>OT_IVE_ORDER_STATS_FILTER_MODE_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="35%" headers="mcps1.1.3.1.2 "><p id="p14581mcpsimp"><a name="p14581mcpsimp"></a><a name="p14581mcpsimp"></a>最大值滤波，等价于灰度图的膨胀。</p>
</td>
</tr>
<tr id="row14582mcpsimp"><td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.3.1.1 "><p id="p14584mcpsimp"><a name="p14584mcpsimp"></a><a name="p14584mcpsimp"></a>OT_IVE_ORDER_STATS_FILTER_MODE_MIN</p>
</td>
<td class="cellrowborder" valign="top" width="35%" headers="mcps1.1.3.1.2 "><p id="p14586mcpsimp"><a name="p14586mcpsimp"></a><a name="p14586mcpsimp"></a>最小值滤波，等价于灰度图的腐蚀。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_order\_stats\_filter\_ctrl](#ot_ive_order_stats_filter_ctrl)

### ot\_ive\_order\_stats\_filter\_ctrl<a name="ZH-CN_TOPIC_0000002471091240"></a>

【说明】

定义顺序统计量滤波控制参数。

【定义】

```
typedef struct {
    ot_ive_order_stats_filter_mode mode;
} ot_ive_order_stats_filter_ctrl;
```

【成员】

<a name="table8153mcpsimp"></a>
<table><thead align="left"><tr id="row8158mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p8160mcpsimp"><a name="p8160mcpsimp"></a><a name="p8160mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p8162mcpsimp"><a name="p8162mcpsimp"></a><a name="p8162mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row8164mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p8166mcpsimp"><a name="p8166mcpsimp"></a><a name="p8166mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p8168mcpsimp"><a name="p8168mcpsimp"></a><a name="p8168mcpsimp"></a>顺序统计量滤波模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_order\_stats\_filter\_mode](#ot_ive_order_stats_filter_mode)

### ot\_ive\_map\_u8bit\_lut\_mem<a name="ZH-CN_TOPIC_0000002471091206"></a>

【说明】

定义map U8C1→U8C1的查找表内存。

【定义】

```
typedef struct {
    td_u8 map[OT_IVE_MAP_NUM];
} ot_ive_map_u8bit_lut_mem;
```

【成员】

<a name="table17155mcpsimp"></a>
<table><thead align="left"><tr id="row17160mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p17162mcpsimp"><a name="p17162mcpsimp"></a><a name="p17162mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p17164mcpsimp"><a name="p17164mcpsimp"></a><a name="p17164mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row17166mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p17168mcpsimp"><a name="p17168mcpsimp"></a><a name="p17168mcpsimp"></a>map[<a href="OT_IVE_MAP_NUM.md">OT_IVE_MAP_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p17171mcpsimp"><a name="p17171mcpsimp"></a><a name="p17171mcpsimp"></a>map查找表数组。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_map\_u16bit\_lut\_mem<a name="ZH-CN_TOPIC_0000002504091091"></a>

【说明】

定义map U8C1→U16C1的查找表内存。

【定义】

```
typedef struct {
    td_u16 map[OT_IVE_MAP_NUM];
} ot_ive_map_u16bit_lut_mem;
```

【成员】

<a name="table2812mcpsimp"></a>
<table><thead align="left"><tr id="row2817mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p2819mcpsimp"><a name="p2819mcpsimp"></a><a name="p2819mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p2821mcpsimp"><a name="p2821mcpsimp"></a><a name="p2821mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2823mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2825mcpsimp"><a name="p2825mcpsimp"></a><a name="p2825mcpsimp"></a>map[<a href="OT_IVE_MAP_NUM.md">OT_IVE_MAP_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2828mcpsimp"><a name="p2828mcpsimp"></a><a name="p2828mcpsimp"></a>map查找表数组。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_map\_s16bit\_lut\_mem<a name="ZH-CN_TOPIC_0000002504091173"></a>

【说明】

定义map U8C1→U16C1的查找表内存。

【定义】

```
typedef struct {
    td_s16 map[OT_IVE_MAP_NUM];
} ot_ive_map_s16bit_lut_mem;
```

【成员】

<a name="table16096mcpsimp"></a>
<table><thead align="left"><tr id="row16101mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p16103mcpsimp"><a name="p16103mcpsimp"></a><a name="p16103mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p16105mcpsimp"><a name="p16105mcpsimp"></a><a name="p16105mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16107mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p16109mcpsimp"><a name="p16109mcpsimp"></a><a name="p16109mcpsimp"></a>map[<a href="OT_IVE_MAP_NUM.md">OT_IVE_MAP_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p16112mcpsimp"><a name="p16112mcpsimp"></a><a name="p16112mcpsimp"></a>map查找表数组。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_map\_mode<a name="ZH-CN_TOPIC_0000002470931320"></a>

【说明】

定义map的模式。

【定义】

```
typedef enum {
    OT_IVE_MAP_MODE_U8 = 0x0,
    OT_IVE_MAP_MODE_S16 = 0x1,
    OT_IVE_MAP_MODE_U16 = 0x2,
    OT_IVE_MAP_MODE_BUTT
} ot_ive_map_mode;
```

【成员】

<a name="table10138mcpsimp"></a>
<table><thead align="left"><tr id="row10143mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p10145mcpsimp"><a name="p10145mcpsimp"></a><a name="p10145mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p10147mcpsimp"><a name="p10147mcpsimp"></a><a name="p10147mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10149mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p10151mcpsimp"><a name="p10151mcpsimp"></a><a name="p10151mcpsimp"></a>OT_IVE_MAP_MODE_U8</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p10153mcpsimp"><a name="p10153mcpsimp"></a><a name="p10153mcpsimp"></a>U8C1→U8C1 Map模式。</p>
</td>
</tr>
<tr id="row10154mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p10156mcpsimp"><a name="p10156mcpsimp"></a><a name="p10156mcpsimp"></a>OT_IVE_MAP_MODE_S16</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p10158mcpsimp"><a name="p10158mcpsimp"></a><a name="p10158mcpsimp"></a>U8C1→S16C1 Map模式。</p>
</td>
</tr>
<tr id="row10159mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p10161mcpsimp"><a name="p10161mcpsimp"></a><a name="p10161mcpsimp"></a>OT_IVE_MAP_MODE_U16</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p10163mcpsimp"><a name="p10163mcpsimp"></a><a name="p10163mcpsimp"></a>U8C1→U16C1 Map 模式</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_map\_ctrl<a name="ZH-CN_TOPIC_0000002471091224"></a>

【说明】

定义map控制参数。

【定义】

```
typedef struct {
    ot_ive_map_mode mode;
} ot_ive_map_ctrl;
```

【成员】

<a name="table15337mcpsimp"></a>
<table><thead align="left"><tr id="row15342mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p15344mcpsimp"><a name="p15344mcpsimp"></a><a name="p15344mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p15346mcpsimp"><a name="p15346mcpsimp"></a><a name="p15346mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row15348mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p15350mcpsimp"><a name="p15350mcpsimp"></a><a name="p15350mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p15352mcpsimp"><a name="p15352mcpsimp"></a><a name="p15352mcpsimp"></a>Map模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_equalize\_hist\_ctrl\_mem<a name="ZH-CN_TOPIC_0000002471091324"></a>

【说明】

定义直方图均衡化辅助内存。

【定义】

```
typedef struct {
    td_u32 hist[OT_IVE_HIST_NUM];
    td_u8 map[OT_IVE_MAP_NUM];
} ot_ive_equalize_hist_ctrl_mem;
```

【成员】

<a name="table1995mcpsimp"></a>
<table><thead align="left"><tr id="row2000mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p2002mcpsimp"><a name="p2002mcpsimp"></a><a name="p2002mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p2004mcpsimp"><a name="p2004mcpsimp"></a><a name="p2004mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2006mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2008mcpsimp"><a name="p2008mcpsimp"></a><a name="p2008mcpsimp"></a>hist[<a href="OT_IVE_HIST_NUM.md">OT_IVE_HIST_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2011mcpsimp"><a name="p2011mcpsimp"></a><a name="p2011mcpsimp"></a>直方图统计的输出。</p>
</td>
</tr>
<tr id="row2012mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p2014mcpsimp"><a name="p2014mcpsimp"></a><a name="p2014mcpsimp"></a>map[<a href="OT_IVE_MAP_NUM.md">OT_IVE_MAP_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p2017mcpsimp"><a name="p2017mcpsimp"></a><a name="p2017mcpsimp"></a>根据统计直方图计算得到的map查找表。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_equalize\_hist\_ctrl](#ot_ive_equalize_hist_ctrl)

### ot\_ive\_equalize\_hist\_ctrl<a name="ZH-CN_TOPIC_0000002503971189"></a>

【说明】

定义直方图均衡化控制参数。

【定义】

```
typedef struct {
    ot_svp_mem_info mem;
} ot_ive_equalize_hist_ctrl;
```

【成员】

<a name="table8122mcpsimp"></a>
<table><thead align="left"><tr id="row8127mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.3.1.1"><p id="p8129mcpsimp"><a name="p8129mcpsimp"></a><a name="p8129mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="80%" id="mcps1.1.3.1.2"><p id="p8131mcpsimp"><a name="p8131mcpsimp"></a><a name="p8131mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row8133mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.3.1.1 "><p id="p8135mcpsimp"><a name="p8135mcpsimp"></a><a name="p8135mcpsimp"></a>mem</p>
</td>
<td class="cellrowborder" valign="top" width="80%" headers="mcps1.1.3.1.2 "><p id="p8137mcpsimp"><a name="p8137mcpsimp"></a><a name="p8137mcpsimp"></a>需开辟sizeof(<a href="ot_ive_equalize_hist_ctrl_mem.md">ot_ive_equalize_hist_ctrl_mem</a>)字节大小的内存。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_equalize\_hist\_ctrl\_mem](#ot_ive_equalize_hist_ctrl_mem)

### ot\_ive\_add\_ctrl<a name="ZH-CN_TOPIC_0000002503971153"></a>

【说明】

定义两图像的加权加控制参数。

【定义】

```
typedef struct {
    td_u0q16 x; /* x of "xA+yB" */
    td_u0q16 y; /* y of "xA+yB" */
} ot_ive_add_ctrl;
```

【成员】

<a name="table7445mcpsimp"></a>
<table><thead align="left"><tr id="row7450mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p7452mcpsimp"><a name="p7452mcpsimp"></a><a name="p7452mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p7454mcpsimp"><a name="p7454mcpsimp"></a><a name="p7454mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7456mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p7458mcpsimp"><a name="p7458mcpsimp"></a><a name="p7458mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p7460mcpsimp"><a name="p7460mcpsimp"></a><a name="p7460mcpsimp"></a>加权加“xA+yB”中的权重“x”。</p>
</td>
</tr>
<tr id="row7461mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p7463mcpsimp"><a name="p7463mcpsimp"></a><a name="p7463mcpsimp"></a>y</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p7465mcpsimp"><a name="p7465mcpsimp"></a><a name="p7465mcpsimp"></a>加权加“xA+yB”中的权重“y”。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_ncc\_dst\_mem<a name="ZH-CN_TOPIC_0000002503971173"></a>

【说明】

定义ncc的输出内存信息。

【定义】

```
typedef struct {
    td_u64 num;
    td_u64 quad_sum1;
    td_u64 quad_sum2;
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_EIGHT];
} ot_ive_ncc_dst_mem;
```

【成员】

<a name="table3709mcpsimp"></a>
<table><thead align="left"><tr id="row3714mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p3716mcpsimp"><a name="p3716mcpsimp"></a><a name="p3716mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p3718mcpsimp"><a name="p3718mcpsimp"></a><a name="p3718mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3720mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p3722mcpsimp"><a name="p3722mcpsimp"></a><a name="p3722mcpsimp"></a>num</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p class="msonormal" id="p3724mcpsimp"><a name="p3724mcpsimp"></a><a name="p3724mcpsimp"></a>NCC计算公式的分子--<a name="image159281555102516"></a><a name="image159281555102516"></a><span><img class="mathml" id="image159281555102516" src="figures/zh-cn_formulaimage_0000002503971347.png"></span></p>
</td>
</tr>
<tr id="row3725mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p3727mcpsimp"><a name="p3727mcpsimp"></a><a name="p3727mcpsimp"></a>quad_sum1</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p class="msonormal" id="p16204756268"><a name="p16204756268"></a><a name="p16204756268"></a>NCC计算公式的分母--根号内部分：</p>
<p id="p1020311552617"><a name="p1020311552617"></a><a name="p1020311552617"></a>NCC计算公式的分子--<a name="image89161940183311"></a><a name="image89161940183311"></a><span><img class="mathml" id="image89161940183311" src="figures/zh-cn_formulaimage_0000002503971349.png"></span></p>
</td>
</tr>
<tr id="row3730mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p3732mcpsimp"><a name="p3732mcpsimp"></a><a name="p3732mcpsimp"></a>quad_sum2</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p class="msonormal" id="p3734mcpsimp"><a name="p3734mcpsimp"></a><a name="p3734mcpsimp"></a>NCC计算公式的分母--根号内部分：</p>
<p id="p173405772616"><a name="p173405772616"></a><a name="p173405772616"></a>NCC计算公式的分子--<a name="image20114173123411"></a><a name="image20114173123411"></a><span><img class="mathml" id="image20114173123411" src="figures/zh-cn_formulaimage_0000002471091414.png"></span></p>
</td>
</tr>
<tr id="row3735mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p xml:lang="de-DE" id="p3737mcpsimp"><a name="p3737mcpsimp"></a><a name="p3737mcpsimp"></a><span xml:lang="en-US" id="ph3738mcpsimp"><a name="ph3738mcpsimp"></a><a name="ph3738mcpsimp"></a>reserved[</span><a href="OT_IVE_ARR_RESERVED_NUM_EIGHT.md">OT_IVE_ARR_RESERVED_NUM_EIGHT</a><span xml:lang="en-US" id="ph3741mcpsimp"><a name="ph3741mcpsimp"></a><a name="ph3741mcpsimp"></a>]</span></p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p3743mcpsimp"><a name="p3743mcpsimp"></a><a name="p3743mcpsimp"></a>保留字段</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式请参见ss\_mpi\_ive\_ncc中的【注意】。

【相关数据类型及接口】

无。

### ot\_ive\_rgn<a name="ZH-CN_TOPIC_0000002471091252"></a>

【说明】

定义连通区域信息。

【定义】

```
typedef struct {
    td_u32 area;   /* Represented by the pixel number */
    td_u16 left;   /* Circumscribed rectangle left border */
    td_u16 right;  /* Circumscribed rectangle right border */
    td_u16 top;    /* Circumscribed rectangle top border */
    td_u16 bottom; /* Circumscribed rectangle bottom border */
} ot_ive_rgn;
```

【成员】

<a name="table4588mcpsimp"></a>
<table><thead align="left"><tr id="row4593mcpsimp"><th class="cellrowborder" valign="top" width="36%" id="mcps1.1.3.1.1"><p id="p4595mcpsimp"><a name="p4595mcpsimp"></a><a name="p4595mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.3.1.2"><p id="p4597mcpsimp"><a name="p4597mcpsimp"></a><a name="p4597mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4599mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p4601mcpsimp"><a name="p4601mcpsimp"></a><a name="p4601mcpsimp"></a>area</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p4603mcpsimp"><a name="p4603mcpsimp"></a><a name="p4603mcpsimp"></a>连通区域面积，以连通区域像素数目表示。</p>
</td>
</tr>
<tr id="row4604mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p4606mcpsimp"><a name="p4606mcpsimp"></a><a name="p4606mcpsimp"></a>left</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p4608mcpsimp"><a name="p4608mcpsimp"></a><a name="p4608mcpsimp"></a>连通区域外接矩形的最左边坐标。</p>
</td>
</tr>
<tr id="row4609mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p4611mcpsimp"><a name="p4611mcpsimp"></a><a name="p4611mcpsimp"></a>right</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p4613mcpsimp"><a name="p4613mcpsimp"></a><a name="p4613mcpsimp"></a>连通区域外接矩形的最右边坐标。</p>
</td>
</tr>
<tr id="row4614mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p4616mcpsimp"><a name="p4616mcpsimp"></a><a name="p4616mcpsimp"></a>top</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p4618mcpsimp"><a name="p4618mcpsimp"></a><a name="p4618mcpsimp"></a>连通区域外接矩形的最上面坐标。</p>
</td>
</tr>
<tr id="row4619mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p4621mcpsimp"><a name="p4621mcpsimp"></a><a name="p4621mcpsimp"></a>bottom</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p4623mcpsimp"><a name="p4623mcpsimp"></a><a name="p4623mcpsimp"></a>连通区域外接矩形的最下面坐标。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_ccblob](#ot_ive_ccblob)

### ot\_ive\_ccblob\_info<a name="ZH-CN_TOPIC_0000002503971211"></a>

【说明】

定义连通区域统计信息在Memory中的存储信息。

【定义】

```
typedef union {
    struct {
        td_u32 cur_area_threshold: 23;/*Threshold of the result rgns' area*/
        td_u32 label_status: 1;  /*1: Labeled failed ; 0: Labeled successfully*/
        td_u32 rgn_num: 8;  /*Number of valid rgn, non-continuous stored*/
    }bits;
    td_u32 u32;
} ot_ive_ccblob_info;
```

【成员】

<a name="table5961mcpsimp"></a>
<table><thead align="left"><tr id="row5966mcpsimp"><th class="cellrowborder" valign="top" width="36%" id="mcps1.1.3.1.1"><p id="p5968mcpsimp"><a name="p5968mcpsimp"></a><a name="p5968mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.3.1.2"><p id="p5970mcpsimp"><a name="p5970mcpsimp"></a><a name="p5970mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5972mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p5974mcpsimp"><a name="p5974mcpsimp"></a><a name="p5974mcpsimp"></a>cur_area_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p5976mcpsimp"><a name="p5976mcpsimp"></a><a name="p5976mcpsimp"></a>有效连通区域的面积阈值(23bit表示)，rgn中面积小于这个阈值的都被置为0</p>
</td>
</tr>
<tr id="row5977mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p5979mcpsimp"><a name="p5979mcpsimp"></a><a name="p5979mcpsimp"></a>label_status</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p5981mcpsimp"><a name="p5981mcpsimp"></a><a name="p5981mcpsimp"></a>连通区域标记是否成功(1bit表示)。</p>
<p id="p5982mcpsimp"><a name="p5982mcpsimp"></a><a name="p5982mcpsimp"></a>-1：标记失败；</p>
<p id="p5983mcpsimp"><a name="p5983mcpsimp"></a><a name="p5983mcpsimp"></a>0：标记成功。</p>
</td>
</tr>
<tr id="row5984mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p5986mcpsimp"><a name="p5986mcpsimp"></a><a name="p5986mcpsimp"></a>rgn_num</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p5988mcpsimp"><a name="p5988mcpsimp"></a><a name="p5988mcpsimp"></a>有效连通区域个数(8bit表示)。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   cur\_area\_threshold：32bit中的前23bit表示有效区域的面积阈值。
-   label\_status：32bit中的第24bit表示的是连通区域是否标记成功。
-   rgn\_num：32bit中后8bit表示是有效连通区域的个数。

【相关数据类型及接口】

-   [ot\_ive\_ccblob](#ot_ive_ccblob)
-   ss\_mpi\_ive\_ccl

### ot\_ive\_ccblob<a name="ZH-CN_TOPIC_0000002503971207"></a>

【说明】

定义连通区域标记的输出信息。

【定义】

```
typedef struct {
    ot_ive_ccblob_info info;
    ot_ive_rgn rgn[OT_IVE_MAX_RGN_NUM];  /* Valid rgns with 'cur_area_thr>0' and 'label = array_index+1' */
} ot_ive_ccblob;
```

【成员】

<a name="table1479mcpsimp"></a>
<table><thead align="left"><tr id="row1484mcpsimp"><th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.1"><p id="p1486mcpsimp"><a name="p1486mcpsimp"></a><a name="p1486mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.2"><p id="p1488mcpsimp"><a name="p1488mcpsimp"></a><a name="p1488mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1490mcpsimp"><td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.1 "><p id="p1492mcpsimp"><a name="p1492mcpsimp"></a><a name="p1492mcpsimp"></a>info</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.2 "><p id="p1494mcpsimp"><a name="p1494mcpsimp"></a><a name="p1494mcpsimp"></a>描述连通区域的相关信息。</p>
</td>
</tr>
<tr id="row1495mcpsimp"><td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.1 "><p id="p1497mcpsimp"><a name="p1497mcpsimp"></a><a name="p1497mcpsimp"></a>rgn[<a href="OT_IVE_MAX_RGN_NUM.md">OT_IVE_MAX_RGN_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.2 "><p id="p1500mcpsimp"><a name="p1500mcpsimp"></a><a name="p1500mcpsimp"></a>连通区域信息：有效的连通区域其面积大于0，对应标记为数组下标加1。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_ive\_ccblob\_info](#ot_ive_ccblob_info)
-   ss\_mpi\_ive\_ccl

### ot\_ive\_ccl\_mode<a name="ZH-CN_TOPIC_0000002470931332"></a>

【说明】

定义连通区域模式。

【定义】

```
typedef enum {
    OT_IVE_CCL_MODE_4C = 0x0, /* 4-connected */
    OT_IVE_CCL_MODE_8C = 0x1, /* 8-connected */
    OT_IVE_CCL_MODE_BUTT
} ot_ive_ccl_mode;
```

【成员】

<a name="table4551mcpsimp"></a>
<table><thead align="left"><tr id="row4556mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p4558mcpsimp"><a name="p4558mcpsimp"></a><a name="p4558mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p4560mcpsimp"><a name="p4560mcpsimp"></a><a name="p4560mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4562mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p4564mcpsimp"><a name="p4564mcpsimp"></a><a name="p4564mcpsimp"></a>OT_IVE_CCL_MODE_4C</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p4566mcpsimp"><a name="p4566mcpsimp"></a><a name="p4566mcpsimp"></a>4-连通。</p>
</td>
</tr>
<tr id="row4567mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p4569mcpsimp"><a name="p4569mcpsimp"></a><a name="p4569mcpsimp"></a>OT_IVE_CCL_MODE_8C</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p4571mcpsimp"><a name="p4571mcpsimp"></a><a name="p4571mcpsimp"></a>8-连通。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_ccl\_ctrl<a name="ZH-CN_TOPIC_0000002471091318"></a>

【说明】

定义连通区域标记控制参数。

【定义】

```
typedef struct {
    ot_ive_ccl_mode mode;   /* Mode */
    td_u16 init_area_threshold;   /* Init threshold of rgn area */
    td_u16 step;            /* Increase area step for once */
} ot_ive_ccl_ctrl;
```

【成员】

<a name="table4952mcpsimp"></a>
<table><thead align="left"><tr id="row4957mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p4959mcpsimp"><a name="p4959mcpsimp"></a><a name="p4959mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p4961mcpsimp"><a name="p4961mcpsimp"></a><a name="p4961mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4963mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p4965mcpsimp"><a name="p4965mcpsimp"></a><a name="p4965mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p4967mcpsimp"><a name="p4967mcpsimp"></a><a name="p4967mcpsimp"></a>连通区域模式。</p>
</td>
</tr>
<tr id="row4968mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p4970mcpsimp"><a name="p4970mcpsimp"></a><a name="p4970mcpsimp"></a>init_area_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p4972mcpsimp"><a name="p4972mcpsimp"></a><a name="p4972mcpsimp"></a>初始面积阈值。</p>
<p id="p4973mcpsimp"><a name="p4973mcpsimp"></a><a name="p4973mcpsimp"></a>取值范围：[0, 65535]</p>
<p id="p4974mcpsimp"><a name="p4974mcpsimp"></a><a name="p4974mcpsimp"></a>参考取值：4。</p>
</td>
</tr>
<tr id="row4975mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p4977mcpsimp"><a name="p4977mcpsimp"></a><a name="p4977mcpsimp"></a>step</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p4979mcpsimp"><a name="p4979mcpsimp"></a><a name="p4979mcpsimp"></a>面积阈值增长步长。</p>
<p id="p4980mcpsimp"><a name="p4980mcpsimp"></a><a name="p4980mcpsimp"></a>取值范围：[1,65535]</p>
<p id="p4981mcpsimp"><a name="p4981mcpsimp"></a><a name="p4981mcpsimp"></a>参考取值：2。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_ccblob](#ot_ive_ccblob)

### ot\_ive\_gmm\_ctrl<a name="ZH-CN_TOPIC_0000002471091236"></a>

【说明】

定义GMM背景建模的控制参数。

【定义】

```
typedef struct {
    td_u22q10 noise_var;    /* Initial noise Variance */
    td_u22q10 max_var;      /* Max  Variance */
    td_u22q10 min_var;      /* Min  Variance */
    td_u0q16 learn_rate;    /* Learning rate */
    td_u0q16 bg_ratio;      /* Background ratio */
    td_u8q8 var_threshold;        /* Variance Threshold */
    td_u0q16 init_wgt;   /* Initial Weight */
    td_u8 model_num;        /* Model number: 3 or 5 */
} ot_ive_gmm_ctrl;
```

【成员】

<a name="table5077mcpsimp"></a>
<table><thead align="left"><tr id="row5082mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p5084mcpsimp"><a name="p5084mcpsimp"></a><a name="p5084mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p5086mcpsimp"><a name="p5086mcpsimp"></a><a name="p5086mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5088mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p5090mcpsimp"><a name="p5090mcpsimp"></a><a name="p5090mcpsimp"></a>noise_ver</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p5092mcpsimp"><a name="p5092mcpsimp"></a><a name="p5092mcpsimp"></a>初始噪声方差。</p>
<p id="p5093mcpsimp"><a name="p5093mcpsimp"></a><a name="p5093mcpsimp"></a>取值范围：[0x1, 0xFFFFFF]。</p>
<p id="p5094mcpsimp"><a name="p5094mcpsimp"></a><a name="p5094mcpsimp"></a>对灰度的GMM，对应OpenCV MOG中灰度模型中的noiseSigma * noiseSigma。</p>
<p id="p5095mcpsimp"><a name="p5095mcpsimp"></a><a name="p5095mcpsimp"></a>参考取值：15*15*(1&lt;&lt;10)。</p>
<p id="p5096mcpsimp"><a name="p5096mcpsimp"></a><a name="p5096mcpsimp"></a>对RGB的GMM，对应OpenCV MOG中RGB模型中的 3 * noiseSigma * noiseSigma。</p>
<p id="p5097mcpsimp"><a name="p5097mcpsimp"></a><a name="p5097mcpsimp"></a>参考取值：3*15*15*(1&lt;&lt;10)。</p>
</td>
</tr>
<tr id="row5098mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p5100mcpsimp"><a name="p5100mcpsimp"></a><a name="p5100mcpsimp"></a>max_var</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p5102mcpsimp"><a name="p5102mcpsimp"></a><a name="p5102mcpsimp"></a>模型方差的最大值。</p>
<p id="p5103mcpsimp"><a name="p5103mcpsimp"></a><a name="p5103mcpsimp"></a>取值范围；[0x1, 0xFFFFFF]。</p>
<p id="p5104mcpsimp"><a name="p5104mcpsimp"></a><a name="p5104mcpsimp"></a>对应OpenCV MOG2中fVarMax。</p>
<p id="p5105mcpsimp"><a name="p5105mcpsimp"></a><a name="p5105mcpsimp"></a>参考取值：3*4000&lt;&lt;10 (RGB)，2000&lt;&lt;10（灰度）。</p>
</td>
</tr>
<tr id="row5106mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p5108mcpsimp"><a name="p5108mcpsimp"></a><a name="p5108mcpsimp"></a>min_var</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p5110mcpsimp"><a name="p5110mcpsimp"></a><a name="p5110mcpsimp"></a>模型方差的最小值。</p>
<p id="p5111mcpsimp"><a name="p5111mcpsimp"></a><a name="p5111mcpsimp"></a>取值范围：[0x1, max_var]。</p>
<p id="p5112mcpsimp"><a name="p5112mcpsimp"></a><a name="p5112mcpsimp"></a>对应OpenCV MOG2中fVarMin。</p>
<p id="p5113mcpsimp"><a name="p5113mcpsimp"></a><a name="p5113mcpsimp"></a>参考取值：600&lt;&lt;10 (RGB)，200&lt;&lt;10 (灰度)。</p>
</td>
</tr>
<tr id="row5114mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p5116mcpsimp"><a name="p5116mcpsimp"></a><a name="p5116mcpsimp"></a>learn_rate</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p5118mcpsimp"><a name="p5118mcpsimp"></a><a name="p5118mcpsimp"></a>学习速率。</p>
<p id="p5119mcpsimp"><a name="p5119mcpsimp"></a><a name="p5119mcpsimp"></a>取值范围：[1, 65535]。</p>
<p id="p5120mcpsimp"><a name="p5120mcpsimp"></a><a name="p5120mcpsimp"></a>对应OpenCV  MOG2中learningRate。</p>
<p id="p5121mcpsimp"><a name="p5121mcpsimp"></a><a name="p5121mcpsimp"></a>参考取值： if (frameNum&lt;500) (1/frameNum)*((1&lt;&lt;16)-1)；else ((1/500)*((1&lt;&lt;16)-1)。</p>
</td>
</tr>
<tr id="row5122mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p5124mcpsimp"><a name="p5124mcpsimp"></a><a name="p5124mcpsimp"></a>bg_ratio</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p5126mcpsimp"><a name="p5126mcpsimp"></a><a name="p5126mcpsimp"></a>背景比例阈值。</p>
<p id="p5127mcpsimp"><a name="p5127mcpsimp"></a><a name="p5127mcpsimp"></a>取值范围：[1, 65535]。</p>
<p id="p5128mcpsimp"><a name="p5128mcpsimp"></a><a name="p5128mcpsimp"></a>对应OpenCV MOG中backgroundRatio。</p>
<p id="p5129mcpsimp"><a name="p5129mcpsimp"></a><a name="p5129mcpsimp"></a>参考取值：0.8*((1&lt;&lt;16)-1)。</p>
</td>
</tr>
<tr id="row5130mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p5132mcpsimp"><a name="p5132mcpsimp"></a><a name="p5132mcpsimp"></a>var_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p5134mcpsimp"><a name="p5134mcpsimp"></a><a name="p5134mcpsimp"></a>方差阈值。</p>
<p id="p5135mcpsimp"><a name="p5135mcpsimp"></a><a name="p5135mcpsimp"></a>取值范围：[1, 65535]。</p>
<p id="p5136mcpsimp"><a name="p5136mcpsimp"></a><a name="p5136mcpsimp"></a>对应OpenCV MOG中varThreshold，用于决定一个像素是否命中当前模型。</p>
<p id="p5137mcpsimp"><a name="p5137mcpsimp"></a><a name="p5137mcpsimp"></a>参考取值：6.25*(1&lt;&lt;8)。</p>
</td>
</tr>
<tr id="row5138mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p5140mcpsimp"><a name="p5140mcpsimp"></a><a name="p5140mcpsimp"></a>init_wgt</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p5142mcpsimp"><a name="p5142mcpsimp"></a><a name="p5142mcpsimp"></a>初始权重。</p>
<p id="p5143mcpsimp"><a name="p5143mcpsimp"></a><a name="p5143mcpsimp"></a>取值范围：[1, 65535]。</p>
<p id="p5144mcpsimp"><a name="p5144mcpsimp"></a><a name="p5144mcpsimp"></a>对应OpenCV MOG中的defaultInitialWeight。</p>
<p id="p5145mcpsimp"><a name="p5145mcpsimp"></a><a name="p5145mcpsimp"></a>参考取值：0.05*((1&lt;&lt;16)-1)。</p>
</td>
</tr>
<tr id="row5146mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p5148mcpsimp"><a name="p5148mcpsimp"></a><a name="p5148mcpsimp"></a>model_num</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p5150mcpsimp"><a name="p5150mcpsimp"></a><a name="p5150mcpsimp"></a>模型个数。</p>
<p id="p5151mcpsimp"><a name="p5151mcpsimp"></a><a name="p5151mcpsimp"></a>取值范围：{3,5}。</p>
<p id="p5152mcpsimp"><a name="p5152mcpsimp"></a><a name="p5152mcpsimp"></a>对应OpenCV MOG中nmixtures。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无

### ot\_ive\_gmm2\_sns\_factor\_mode<a name="ZH-CN_TOPIC_0000002471091300"></a>

【说明】

定义灵敏度系数模式。

【定义】

```
typedef enum {
    OT_IVE_GMM2_SNS_FACTOR_MODE_GLOBAL = 0x0, /* Global sensitivity factor mode */
    OT_IVE_GMM2_SNS_FACTOR_MODE_PIXEL = 0x1, /* Pixel sensitivity factor mode */
    OT_IVE_GMM2_SNS_FACTOR_MODE_BUTT
} ot_ive_gmm2_sns_factor_mode;
```

【成员】

<a name="table14300mcpsimp"></a>
<table><thead align="left"><tr id="row14305mcpsimp"><th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.1"><p id="p14307mcpsimp"><a name="p14307mcpsimp"></a><a name="p14307mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.2"><p id="p14309mcpsimp"><a name="p14309mcpsimp"></a><a name="p14309mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14311mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p14313mcpsimp"><a name="p14313mcpsimp"></a><a name="p14313mcpsimp"></a>OT_IVE_GMM2_SNS_FACTOR_MODE_GLOBAL</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p14315mcpsimp"><a name="p14315mcpsimp"></a><a name="p14315mcpsimp"></a>全局灵敏度系数模式，每个像素在模型匹配过程中，方差灵敏度使用<a href="ot_ive_gmm2_ctrl.md">ot_ive_gmm2_ctrl</a>的global_sns_factor。</p>
</td>
</tr>
<tr id="row14317mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p14319mcpsimp"><a name="p14319mcpsimp"></a><a name="p14319mcpsimp"></a>OT_IVE_GMM2_SNS_FACTOR_MODE_PIXEL</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p14321mcpsimp"><a name="p14321mcpsimp"></a><a name="p14321mcpsimp"></a>像素级灵敏度系数模式，每个像素在模型匹配过程中，方差灵敏度使用factor的灵敏度系数。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_gmm2\_life\_update\_factor\_mode<a name="ZH-CN_TOPIC_0000002503971203"></a>

【说明】

定义模型时长参数更新模式。

【定义】

```
typedef enum {
    OT_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_GLOBAL = 0x0, /* Global life update factor mode */
    OT_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_PIXEL = 0x1, /* Pixel life update factor mode */
    OT_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_BUTT
} ot_ive_gmm2_life_update_factor_mode;
```

【成员】

<a name="table14094mcpsimp"></a>
<table><thead align="left"><tr id="row14099mcpsimp"><th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.1"><p id="p14101mcpsimp"><a name="p14101mcpsimp"></a><a name="p14101mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.2"><p id="p14103mcpsimp"><a name="p14103mcpsimp"></a><a name="p14103mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14105mcpsimp"><td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.1 "><p id="p14107mcpsimp"><a name="p14107mcpsimp"></a><a name="p14107mcpsimp"></a>OT_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_GLOBAL</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.2 "><p id="p14109mcpsimp"><a name="p14109mcpsimp"></a><a name="p14109mcpsimp"></a>模型时长参数全局更新模式，每个像素模型时长参数在更新时使用<a href="ot_ive_gmm2_ctrl.md">ot_ive_gmm2_ctrl</a>的global_life_update_factor。</p>
</td>
</tr>
<tr id="row14111mcpsimp"><td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.1 "><p id="p14113mcpsimp"><a name="p14113mcpsimp"></a><a name="p14113mcpsimp"></a>OT_IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_PIXEL</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.2 "><p id="p14115mcpsimp"><a name="p14115mcpsimp"></a><a name="p14115mcpsimp"></a>模型时长参数像素级更新模式，每个像素模型时长在更新时使用factor的模型更新参数。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_gmm2\_ctrl<a name="ZH-CN_TOPIC_0000002504091119"></a>

【说明】

定义GMM2背景建模的控制参数。

【定义】

```
typedef struct {
    ot_ive_gmm2_sns_factor_mode sns_factor_mode; /* Sensitivity factor mode */
    ot_ive_gmm2_life_update_factor_mode life_update_factor_mode; /* Life update factor mode */
    td_u16 global_life_update_factor; /* Global life update factor (default: 4) */
    td_u16 life_threshold; /* Life threshold (default: 5000) */
    td_u16 freq_init_val; /* Initial frequency (default: 20000) */
    td_u16 freq_reduce_factor; /* Frequency reduction factor (default: 0xFF00) */
    td_u16 freq_add_factor; /* Frequency adding factor (default: 0xEF) */
    td_u16 freq_threshold; /* Frequency threshold (default: 12000) */
    td_u16 var_rate; /* Variation update rate (default: 1) */
    td_u9q7 max_var; /* Max variation (default: (16 * 16)<<7) */
    td_u9q7 min_var; /* Min variation (default: ( 8 *  8)<<7) */
    td_u8 global_sns_factor; /* Global sensitivity factor (default: 8) */
    td_u8 model_num; /* Model number (range: 1~5, default: 3) */
} ot_ive_gmm2_ctrl;
```

【成员】

<a name="table12380mcpsimp"></a>
<table><thead align="left"><tr id="row12385mcpsimp"><th class="cellrowborder" valign="top" width="32%" id="mcps1.1.3.1.1"><p id="p12387mcpsimp"><a name="p12387mcpsimp"></a><a name="p12387mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.3.1.2"><p id="p12389mcpsimp"><a name="p12389mcpsimp"></a><a name="p12389mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row12391mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12393mcpsimp"><a name="p12393mcpsimp"></a><a name="p12393mcpsimp"></a>sns_factor_mode</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12395mcpsimp"><a name="p12395mcpsimp"></a><a name="p12395mcpsimp"></a>灵敏度模式，默认全局模式。全局模式使用global_sns_factor作为灵敏度系数；像素模式使用ss_mpi_ive_gmm2中factor的低8bit值作为灵敏度系数。</p>
</td>
</tr>
<tr id="row12397mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12399mcpsimp"><a name="p12399mcpsimp"></a><a name="p12399mcpsimp"></a>life_update_factor_mode</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12401mcpsimp"><a name="p12401mcpsimp"></a><a name="p12401mcpsimp"></a>模型时长更新模式，默认全局模式。全局模式使用glb_life_update_factor作为前进模型更新参数；像素模式使用ss_mpi_ive_gmm2中factor的高8bit值作为前进模型时长更新参数。</p>
</td>
</tr>
<tr id="row12403mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12405mcpsimp"><a name="p12405mcpsimp"></a><a name="p12405mcpsimp"></a>global_life_update_factor</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12407mcpsimp"><a name="p12407mcpsimp"></a><a name="p12407mcpsimp"></a>全局模型更新参数，范围：[0, 65535]，默认：4。</p>
</td>
</tr>
<tr id="row12408mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12410mcpsimp"><a name="p12410mcpsimp"></a><a name="p12410mcpsimp"></a>life_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12412mcpsimp"><a name="p12412mcpsimp"></a><a name="p12412mcpsimp"></a>背景模型生成时间，表示一个模型从前景模型转成背景模型需要的时间，范围：[0, 65535]，默认：5000。</p>
</td>
</tr>
<tr id="row12413mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12415mcpsimp"><a name="p12415mcpsimp"></a><a name="p12415mcpsimp"></a>freq_init_val</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12417mcpsimp"><a name="p12417mcpsimp"></a><a name="p12417mcpsimp"></a>初始频率，范围：[0, 65535]，默认：20000。</p>
</td>
</tr>
<tr id="row12418mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12420mcpsimp"><a name="p12420mcpsimp"></a><a name="p12420mcpsimp"></a>freq_redu_factor</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12422mcpsimp"><a name="p12422mcpsimp"></a><a name="p12422mcpsimp"></a>频率衰减系数，范围：[0, 65535]，默认：0xFF00。</p>
</td>
</tr>
<tr id="row12423mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12425mcpsimp"><a name="p12425mcpsimp"></a><a name="p12425mcpsimp"></a>freq_add_factor</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12427mcpsimp"><a name="p12427mcpsimp"></a><a name="p12427mcpsimp"></a>模型匹配频率增加系数，范围：[0, 65535]，默认：0xEF。</p>
</td>
</tr>
<tr id="row12428mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12430mcpsimp"><a name="p12430mcpsimp"></a><a name="p12430mcpsimp"></a>freq_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12432mcpsimp"><a name="p12432mcpsimp"></a><a name="p12432mcpsimp"></a>模型失效频率阈值，范围：[0, 65535]，默认：12000。</p>
</td>
</tr>
<tr id="row12433mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12435mcpsimp"><a name="p12435mcpsimp"></a><a name="p12435mcpsimp"></a>var_rate</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12437mcpsimp"><a name="p12437mcpsimp"></a><a name="p12437mcpsimp"></a>方差更新率，范围：[0, 65535]，默认：1。</p>
</td>
</tr>
<tr id="row12438mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12440mcpsimp"><a name="p12440mcpsimp"></a><a name="p12440mcpsimp"></a>max_var</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12442mcpsimp"><a name="p12442mcpsimp"></a><a name="p12442mcpsimp"></a>方差最大值，范围：[0, 65535]，默认：(16x16)&lt;&lt;7。</p>
</td>
</tr>
<tr id="row12443mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12445mcpsimp"><a name="p12445mcpsimp"></a><a name="p12445mcpsimp"></a>min_var</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12447mcpsimp"><a name="p12447mcpsimp"></a><a name="p12447mcpsimp"></a>方差最小值，范围：0-方差最大值，默认：(8x8)&lt;&lt;7，必须小于max_var。</p>
</td>
</tr>
<tr id="row12448mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12450mcpsimp"><a name="p12450mcpsimp"></a><a name="p12450mcpsimp"></a>global_sns_factor</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12452mcpsimp"><a name="p12452mcpsimp"></a><a name="p12452mcpsimp"></a>全局灵敏度参数，范围：[0, 255]，默认：8。</p>
</td>
</tr>
<tr id="row12453mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p12455mcpsimp"><a name="p12455mcpsimp"></a><a name="p12455mcpsimp"></a>model_num</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p12457mcpsimp"><a name="p12457mcpsimp"></a><a name="p12457mcpsimp"></a>模型数量，范围：[1, 5]，默认：3。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_canny\_stack\_size<a name="ZH-CN_TOPIC_0000002504091083"></a>

【说明】

定义Canny边缘前半部分计算时强边缘点栈大小结构体。

【定义】

```
typedef struct {
    td_u32 stack_size;  /* Stack size for output */
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_TWELVE]; /* For 16 byte align */
} ot_ive_canny_stack_size;
```

【成员】

<a name="table8662mcpsimp"></a>
<table><thead align="left"><tr id="row8667mcpsimp"><th class="cellrowborder" valign="top" width="56.99999999999999%" id="mcps1.1.3.1.1"><p id="p8669mcpsimp"><a name="p8669mcpsimp"></a><a name="p8669mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="43%" id="mcps1.1.3.1.2"><p id="p8671mcpsimp"><a name="p8671mcpsimp"></a><a name="p8671mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row8673mcpsimp"><td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p8675mcpsimp"><a name="p8675mcpsimp"></a><a name="p8675mcpsimp"></a>stack_size</p>
</td>
<td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.2 "><p id="p8677mcpsimp"><a name="p8677mcpsimp"></a><a name="p8677mcpsimp"></a>栈大小(强边缘点的个数)。</p>
</td>
</tr>
<tr id="row8678mcpsimp"><td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.1 "><p xml:lang="de-DE" id="p8680mcpsimp"><a name="p8680mcpsimp"></a><a name="p8680mcpsimp"></a><span xml:lang="en-US" id="ph8681mcpsimp"><a name="ph8681mcpsimp"></a><a name="ph8681mcpsimp"></a>reserved</span> <span xml:lang="en-US" id="ph8682mcpsimp"><a name="ph8682mcpsimp"></a><a name="ph8682mcpsimp"></a>[</span><a href="OT_IVE_ARR_RESERVED_NUM_TWELVE.md">OT_IVE_ARR_RESERVED_NUM_TWELVE</a><span xml:lang="en-US" id="ph8685mcpsimp"><a name="ph8685mcpsimp"></a><a name="ph8685mcpsimp"></a>]</span></p>
</td>
<td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.2 "><p id="p8687mcpsimp"><a name="p8687mcpsimp"></a><a name="p8687mcpsimp"></a>保留字段。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_canny\_hys\_edge\_ctrl<a name="ZH-CN_TOPIC_0000002503971183"></a>

【说明】

定义Canny边缘前半部分计算任务的控制参数。

【定义】

```
typedef struct {
    ot_svp_mem_info mem;
    td_u16 low_threshold;
    td_u16 high_threshold;
    td_s8 mask[OT_IVE_MASK_NUM];
} ot_ive_canny_hys_edge_ctrl;
```

【成员】

<a name="table3842mcpsimp"></a>
<table><thead align="left"><tr id="row3847mcpsimp"><th class="cellrowborder" valign="top" width="37%" id="mcps1.1.3.1.1"><p id="p3849mcpsimp"><a name="p3849mcpsimp"></a><a name="p3849mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.3.1.2"><p id="p3851mcpsimp"><a name="p3851mcpsimp"></a><a name="p3851mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3853mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p id="p3855mcpsimp"><a name="p3855mcpsimp"></a><a name="p3855mcpsimp"></a>mem</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p3857mcpsimp"><a name="p3857mcpsimp"></a><a name="p3857mcpsimp"></a>辅助内存。内存配置大小说明见ss_mpi_ive_canny_hys_edge的【注意】。</p>
<p id="p3859mcpsimp"><a name="p3859mcpsimp"></a><a name="p3859mcpsimp"></a>详细定义请见《SVPx.0 API参考》1.4小节。</p>
</td>
</tr>
<tr id="row3860mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p id="p3862mcpsimp"><a name="p3862mcpsimp"></a><a name="p3862mcpsimp"></a>low_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p3864mcpsimp"><a name="p3864mcpsimp"></a><a name="p3864mcpsimp"></a>低阈值。</p>
<p id="p3865mcpsimp"><a name="p3865mcpsimp"></a><a name="p3865mcpsimp"></a>取值范围：[0,255]</p>
</td>
</tr>
<tr id="row3866mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p id="p3868mcpsimp"><a name="p3868mcpsimp"></a><a name="p3868mcpsimp"></a>high_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p3870mcpsimp"><a name="p3870mcpsimp"></a><a name="p3870mcpsimp"></a>高阈值。</p>
<p id="p3871mcpsimp"><a name="p3871mcpsimp"></a><a name="p3871mcpsimp"></a>取值范围：[low_threshold,255]</p>
</td>
</tr>
<tr id="row3872mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p id="p3874mcpsimp"><a name="p3874mcpsimp"></a><a name="p3874mcpsimp"></a>mask[<a href="OT_IVE_MASK_NUM.md">OT_IVE_MASK_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p3878mcpsimp"><a name="p3878mcpsimp"></a><a name="p3878mcpsimp"></a>用于计算梯度的参数模板。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_lbp\_compare\_mode<a name="ZH-CN_TOPIC_0000002470931250"></a>

【说明】

定义lbp计算的比较模式。

【定义】

```
typedef enum {
    OT_IVE_LBP_COMPARE_MODE_NORMAL= 0x0, /* P(x)-P(center)>= bit_thr.s8Val, s(x)=1; else s(x)=0; */
    OT_IVE_LBP_COMPARE_MODE_ABS= 0x1,    /* Abs(P(x)-P(center))>=bit_thr.u8Val, s(x)=1; else s(x)=0; */
    OT_IVE_LBP_COMPARE_MODE_BUTT
} ot_ive_lbp_compare_mode;
```

【成员】

<a name="table15284mcpsimp"></a>
<table><thead align="left"><tr id="row15289mcpsimp"><th class="cellrowborder" valign="top" width="56.00000000000001%" id="mcps1.1.3.1.1"><p id="p15291mcpsimp"><a name="p15291mcpsimp"></a><a name="p15291mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="44%" id="mcps1.1.3.1.2"><p id="p15293mcpsimp"><a name="p15293mcpsimp"></a><a name="p15293mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row15295mcpsimp"><td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p15297mcpsimp"><a name="p15297mcpsimp"></a><a name="p15297mcpsimp"></a>OT_IVE_LBP_COMPARE_MODE_NORMAL</p>
</td>
<td class="cellrowborder" valign="top" width="44%" headers="mcps1.1.3.1.2 "><p id="p15299mcpsimp"><a name="p15299mcpsimp"></a><a name="p15299mcpsimp"></a>LBP简单比较模式。</p>
</td>
</tr>
<tr id="row15300mcpsimp"><td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p15302mcpsimp"><a name="p15302mcpsimp"></a><a name="p15302mcpsimp"></a>OT_IVE_LBP_COMPARE_MODE_ABS</p>
</td>
<td class="cellrowborder" valign="top" width="44%" headers="mcps1.1.3.1.2 "><p id="p15304mcpsimp"><a name="p15304mcpsimp"></a><a name="p15304mcpsimp"></a>LBP绝对值比较模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式参考ss\_mpi\_ive\_lbp中的【注意】，示意图请参考lbp计算公式示意图。

【相关数据类型及接口】

[ot\_ive\_lbp\_ctrl](#ot_ive_lbp_ctrl)

### ot\_ive\_lbp\_ctrl<a name="ZH-CN_TOPIC_0000002471091226"></a>

【说明】

定义lbp纹理计算控制参数。

【定义】

```
typedef struct {
    ot_ive_lbp_compare_mode mode;
    ot_svp_8bit bit_threshold;
} ot_ive_lbp_ctrl;
```

【成员】

<a name="table13570mcpsimp"></a>
<table><thead align="left"><tr id="row13575mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.3.1.1"><p id="p13577mcpsimp"><a name="p13577mcpsimp"></a><a name="p13577mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="80%" id="mcps1.1.3.1.2"><p id="p13579mcpsimp"><a name="p13579mcpsimp"></a><a name="p13579mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row13581mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.3.1.1 "><p id="p13583mcpsimp"><a name="p13583mcpsimp"></a><a name="p13583mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="80%" headers="mcps1.1.3.1.2 "><p id="p13585mcpsimp"><a name="p13585mcpsimp"></a><a name="p13585mcpsimp"></a>LBP比较模式。</p>
</td>
</tr>
<tr id="row13586mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.3.1.1 "><p id="p13588mcpsimp"><a name="p13588mcpsimp"></a><a name="p13588mcpsimp"></a>bit_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="80%" headers="mcps1.1.3.1.2 "><p id="p13590mcpsimp"><a name="p13590mcpsimp"></a><a name="p13590mcpsimp"></a>LBP比较阈值。</p>
<p id="p13591mcpsimp"><a name="p13591mcpsimp"></a><a name="p13591mcpsimp"></a>OT_IVE_LBP_COMPARE_MODE_NORMAL下的取值范围：[-128,127]。</p>
<p id="p13592mcpsimp"><a name="p13592mcpsimp"></a><a name="p13592mcpsimp"></a>OT_IVE_LBP_COMPARE_MODE_ABS下的取值范围：[0,255]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

计算公式参考ss\_mpi\_ive\_lbp中的【注意】，示意图请参考lbp计算公式示意图。

【相关数据类型及接口】

-   [ot\_ive\_lbp\_compare\_mode](#ot_ive_lbp_compare_mode)
-   [ot\_svp\_8bit](#ot_svp_8bit)

### ot\_ive\_norm\_grad\_out\_ctrl<a name="ZH-CN_TOPIC_0000002503971179"></a>

【说明】

定义归一化梯度信息计算任务输出控制枚举类型。

【定义】

```
typedef enum {
    OT_IVE_NORM_GRAD_OUT_CTRL_HOR_AND_VER = 0x0,
    OT_IVE_NORM_GRAD_OUT_CTRL_HOR = 0x1,
    OT_IVE_NORM_GRAD_OUT_CTRL_VER = 0x2,
    OT_IVE_NORM_GRAD_OUT_CTRL_COMBINE = 0x3,
    OT_IVE_NORM_GRAD_OUT_CTRL_BUTT
} ot_ive_norm_grad_out_ctrl;
```

【成员】

<a name="table10294mcpsimp"></a>
<table><thead align="left"><tr id="row10299mcpsimp"><th class="cellrowborder" valign="top" width="43%" id="mcps1.1.3.1.1"><p id="p10301mcpsimp"><a name="p10301mcpsimp"></a><a name="p10301mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="56.99999999999999%" id="mcps1.1.3.1.2"><p id="p10303mcpsimp"><a name="p10303mcpsimp"></a><a name="p10303mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10305mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p10307mcpsimp"><a name="p10307mcpsimp"></a><a name="p10307mcpsimp"></a>OT_IVE_NORM_GRAD_OUT_CTRL_HOR_AND_VER</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p10309mcpsimp"><a name="p10309mcpsimp"></a><a name="p10309mcpsimp"></a>同时输出梯度信息的H、V分量图(H、V定义见ss_mpi_ive_norm_grad的【参数】)。</p>
</td>
</tr>
<tr id="row10311mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p10313mcpsimp"><a name="p10313mcpsimp"></a><a name="p10313mcpsimp"></a>OT_IVE_NORM_GRAD_OUT_CTRL_HOR</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p10315mcpsimp"><a name="p10315mcpsimp"></a><a name="p10315mcpsimp"></a>仅输出梯度信息的H分量图。</p>
</td>
</tr>
<tr id="row10316mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p10318mcpsimp"><a name="p10318mcpsimp"></a><a name="p10318mcpsimp"></a>OT_IVE_NORM_GRAD_OUT_CTRL_VER</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p10320mcpsimp"><a name="p10320mcpsimp"></a><a name="p10320mcpsimp"></a>仅输出梯度信息的V分量图。</p>
</td>
</tr>
<tr id="row10321mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p10323mcpsimp"><a name="p10323mcpsimp"></a><a name="p10323mcpsimp"></a>OT_IVE_NORM_GRAD_OUT_CTRL_COMBINE</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p10325mcpsimp"><a name="p10325mcpsimp"></a><a name="p10325mcpsimp"></a>输出梯度信息以package 存储（如OT_SVP_IMG_TYPE_S8C2_PACKAGE类型的ot_svp_img图像图）的HV图。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_norm\_grad\_ctrl](#ot_ive_norm_grad_ctrl)

### ot\_ive\_norm\_grad\_ctrl<a name="ZH-CN_TOPIC_0000002470931288"></a>

【说明】

定义归一化梯度信息计算控制参数。

【定义】

```
typedef struct {
    ot_ive_norm_grad_out_ctrl out_ctrl;
    td_s8 mask[OT_IVE_MASK_NUM];
    td_u8 norm;
} ot_ive_norm_grad_ctrl;
```

【成员】

<a name="table8586mcpsimp"></a>
<table><thead align="left"><tr id="row8591mcpsimp"><th class="cellrowborder" valign="top" width="36%" id="mcps1.1.3.1.1"><p id="p8593mcpsimp"><a name="p8593mcpsimp"></a><a name="p8593mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.3.1.2"><p id="p8595mcpsimp"><a name="p8595mcpsimp"></a><a name="p8595mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row8597mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p8599mcpsimp"><a name="p8599mcpsimp"></a><a name="p8599mcpsimp"></a>out_ctrl</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p8601mcpsimp"><a name="p8601mcpsimp"></a><a name="p8601mcpsimp"></a>梯度信息输出控制模式。</p>
</td>
</tr>
<tr id="row8602mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p8604mcpsimp"><a name="p8604mcpsimp"></a><a name="p8604mcpsimp"></a>mask[<a href="OT_IVE_MASK_NUM.md">OT_IVE_MASK_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p8608mcpsimp"><a name="p8608mcpsimp"></a><a name="p8608mcpsimp"></a>计算梯度需要的模板。</p>
</td>
</tr>
<tr id="row8609mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p8611mcpsimp"><a name="p8611mcpsimp"></a><a name="p8611mcpsimp"></a>norm</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p8613mcpsimp"><a name="p8613mcpsimp"></a><a name="p8613mcpsimp"></a>归一化参数。</p>
<p id="p8614mcpsimp"><a name="p8614mcpsimp"></a><a name="p8614mcpsimp"></a>取值范围：[0, 13]</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_norm\_grad\_out\_ctrl](#ot_ive_norm_grad_out_ctrl)

### ot\_ive\_lk\_optical\_flow\_pyr\_out\_mode<a name="ZH-CN_TOPIC_0000002504091077"></a>

【说明】

定义金字塔LK光流计算输出模式。

【定义】

```
typedef enum {
    OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_NONE = 0,   /* Output none */
    OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_STATUS = 1, /* Output status */
    OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH = 2, /* Output status and err */
    OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BUTT
} ot_ive_lk_optical_flow_pyr_out_mode;
```

【成员】

<a name="table9674mcpsimp"></a>
<table><thead align="left"><tr id="row9679mcpsimp"><th class="cellrowborder" valign="top" width="74%" id="mcps1.1.3.1.1"><p id="p9681mcpsimp"><a name="p9681mcpsimp"></a><a name="p9681mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="26%" id="mcps1.1.3.1.2"><p id="p9683mcpsimp"><a name="p9683mcpsimp"></a><a name="p9683mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row9685mcpsimp"><td class="cellrowborder" valign="top" width="74%" headers="mcps1.1.3.1.1 "><p id="p9687mcpsimp"><a name="p9687mcpsimp"></a><a name="p9687mcpsimp"></a>OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_NONE</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.1.3.1.2 "><p id="p9689mcpsimp"><a name="p9689mcpsimp"></a><a name="p9689mcpsimp"></a>status以及err均不输出。</p>
</td>
</tr>
<tr id="row9690mcpsimp"><td class="cellrowborder" valign="top" width="74%" headers="mcps1.1.3.1.1 "><p id="p9692mcpsimp"><a name="p9692mcpsimp"></a><a name="p9692mcpsimp"></a>OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_STATUS</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.1.3.1.2 "><p id="p9694mcpsimp"><a name="p9694mcpsimp"></a><a name="p9694mcpsimp"></a>仅输出status。</p>
</td>
</tr>
<tr id="row9695mcpsimp"><td class="cellrowborder" valign="top" width="74%" headers="mcps1.1.3.1.1 "><p id="p9697mcpsimp"><a name="p9697mcpsimp"></a><a name="p9697mcpsimp"></a>OT_IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.1.3.1.2 "><p id="p9699mcpsimp"><a name="p9699mcpsimp"></a><a name="p9699mcpsimp"></a>同时输出status和err。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_lk\_optical\_flow\_pyr\_ctrl<a name="ZH-CN_TOPIC_0000002503971181"></a>

【说明】

定义金字塔LK光流计算控制参数。

【定义】

```
typedef struct {
    ot_ive_lk_optical_flow_pyr_out_mode out_mode;
    td_bool use_init_flow;   /* where to use initial flow */
    td_u16 points_num;          /* Number of the feature points,<=500 */
    td_u8 max_level;         /* 0<=max_level<=3 */
    td_u0q8 min_eig_val_threshold;     /* Minimum eigenvalue threshold */
    td_u8 iter_cnt;          /* Maximum iteration times, <=20 */
    td_u0q8 eps;             /* Used for exit criteria: dx^2 + dy^2 < eps */
} ot_ive_lk_optical_flow_pyr_ctrl;
```

【成员】

<a name="table7531mcpsimp"></a>
<table><thead align="left"><tr id="row7536mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p7538mcpsimp"><a name="p7538mcpsimp"></a><a name="p7538mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p7540mcpsimp"><a name="p7540mcpsimp"></a><a name="p7540mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7542mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p7544mcpsimp"><a name="p7544mcpsimp"></a><a name="p7544mcpsimp"></a>out_mode</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p7546mcpsimp"><a name="p7546mcpsimp"></a><a name="p7546mcpsimp"></a>status以及err的输出模式控制。</p>
</td>
</tr>
<tr id="row7547mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p7549mcpsimp"><a name="p7549mcpsimp"></a><a name="p7549mcpsimp"></a>use_init_flow</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p7551mcpsimp"><a name="p7551mcpsimp"></a><a name="p7551mcpsimp"></a>是否使用初始光流计算（next_pts是否需要初始化）：TD_TRUE表示使用初始光流，TD_FALSE表示不适用初始光流。</p>
</td>
</tr>
<tr id="row7552mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p7554mcpsimp"><a name="p7554mcpsimp"></a><a name="p7554mcpsimp"></a>points_num</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p7556mcpsimp"><a name="p7556mcpsimp"></a><a name="p7556mcpsimp"></a>prev_pts、next_pts中特征点个数，同时也是status和err的数组大小。</p>
<p id="p7557mcpsimp"><a name="p7557mcpsimp"></a><a name="p7557mcpsimp"></a>取值范围：[1, 500]。</p>
</td>
</tr>
<tr id="row7558mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p7560mcpsimp"><a name="p7560mcpsimp"></a><a name="p7560mcpsimp"></a>max_level</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p7562mcpsimp"><a name="p7562mcpsimp"></a><a name="p7562mcpsimp"></a>max_level+1为金字塔层数相关。</p>
<p id="p7563mcpsimp"><a name="p7563mcpsimp"></a><a name="p7563mcpsimp"></a>取值范围：[0, 3]，对应金字塔层数[1, 4]。</p>
<p id="p7564mcpsimp"><a name="p7564mcpsimp"></a><a name="p7564mcpsimp"></a>参考取值：2。</p>
</td>
</tr>
<tr id="row7565mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p7567mcpsimp"><a name="p7567mcpsimp"></a><a name="p7567mcpsimp"></a>min_eig_val_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p7569mcpsimp"><a name="p7569mcpsimp"></a><a name="p7569mcpsimp"></a>最小特征值阈值。</p>
<p id="p7570mcpsimp"><a name="p7570mcpsimp"></a><a name="p7570mcpsimp"></a>取值范围：[1, 255]。</p>
</td>
</tr>
<tr id="row7571mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p7573mcpsimp"><a name="p7573mcpsimp"></a><a name="p7573mcpsimp"></a>iter_cnt</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p7575mcpsimp"><a name="p7575mcpsimp"></a><a name="p7575mcpsimp"></a>最大迭代次数。</p>
<p id="p7576mcpsimp"><a name="p7576mcpsimp"></a><a name="p7576mcpsimp"></a>取值范围：[1, 20]。</p>
</td>
</tr>
<tr id="row7577mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p7579mcpsimp"><a name="p7579mcpsimp"></a><a name="p7579mcpsimp"></a>eps</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p7581mcpsimp"><a name="p7581mcpsimp"></a><a name="p7581mcpsimp"></a>迭代收敛条件：dx^2 + dy^2 &lt; eps。</p>
<p id="p7582mcpsimp"><a name="p7582mcpsimp"></a><a name="p7582mcpsimp"></a>取值范围：[1, 255]。</p>
<p id="p7583mcpsimp"><a name="p7583mcpsimp"></a><a name="p7583mcpsimp"></a>参考取值：2。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_st\_max\_eig\_val<a name="ZH-CN_TOPIC_0000002504091183"></a>

【说明】

定义Shi-Tomas-like角点计算时最大角点响应值结构体。

【定义】

```
typedef struct {
    td_u16 max_eig_val;     /* Shi-Tomasi second step output MaxEig */
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_FOURTEEN]; /* For 16 byte align */
} ot_ive_st_max_eig_val;
```

【成员】

<a name="table11223mcpsimp"></a>
<table><thead align="left"><tr id="row11228mcpsimp"><th class="cellrowborder" valign="top" width="60%" id="mcps1.1.3.1.1"><p id="p11230mcpsimp"><a name="p11230mcpsimp"></a><a name="p11230mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="40%" id="mcps1.1.3.1.2"><p id="p11232mcpsimp"><a name="p11232mcpsimp"></a><a name="p11232mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row11234mcpsimp"><td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.3.1.1 "><p id="p11236mcpsimp"><a name="p11236mcpsimp"></a><a name="p11236mcpsimp"></a>max_eig_val</p>
</td>
<td class="cellrowborder" valign="top" width="40%" headers="mcps1.1.3.1.2 "><p id="p11238mcpsimp"><a name="p11238mcpsimp"></a><a name="p11238mcpsimp"></a>最大角点响应值。</p>
</td>
</tr>
<tr id="row11239mcpsimp"><td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.3.1.1 "><p xml:lang="de-DE" id="p11241mcpsimp"><a name="p11241mcpsimp"></a><a name="p11241mcpsimp"></a><span xml:lang="en-US" id="ph11242mcpsimp"><a name="ph11242mcpsimp"></a><a name="ph11242mcpsimp"></a>reserved</span> <span xml:lang="en-US" id="ph11243mcpsimp"><a name="ph11243mcpsimp"></a><a name="ph11243mcpsimp"></a>[</span><a href="OT_IVE_ARR_RESERVED_NUM_FOURTEEN.md">OT_IVE_ARR_RESERVED_NUM_FOURTEEN</a><span xml:lang="en-US" id="ph11246mcpsimp"><a name="ph11246mcpsimp"></a><a name="ph11246mcpsimp"></a>]</span></p>
</td>
<td class="cellrowborder" valign="top" width="40%" headers="mcps1.1.3.1.2 "><p id="p11248mcpsimp"><a name="p11248mcpsimp"></a><a name="p11248mcpsimp"></a>保留位。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

### ot\_ive\_st\_cand\_corner\_ctrl<a name="ZH-CN_TOPIC_0000002504091117"></a>

【说明】

定义Shi-Tomas-like候选角点计算控制参数。

【定义】

```
typedef struct {
    ot_svp_mem_info mem;
    td_u0q8 quality_level;
} ot_ive_st_cand_corner_ctrl;
```

【成员】

<a name="table6023mcpsimp"></a>
<table><thead align="left"><tr id="row6028mcpsimp"><th class="cellrowborder" valign="top" width="21%" id="mcps1.1.3.1.1"><p id="p6030mcpsimp"><a name="p6030mcpsimp"></a><a name="p6030mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="79%" id="mcps1.1.3.1.2"><p id="p6032mcpsimp"><a name="p6032mcpsimp"></a><a name="p6032mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6034mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.3.1.1 "><p id="p6036mcpsimp"><a name="p6036mcpsimp"></a><a name="p6036mcpsimp"></a>mem</p>
</td>
<td class="cellrowborder" valign="top" width="79%" headers="mcps1.1.3.1.2 "><p id="p6038mcpsimp"><a name="p6038mcpsimp"></a><a name="p6038mcpsimp"></a>辅助内存。内存配置大小见ss_mpi_ive_st_cand_corner的【注意】。</p>
<p id="p6040mcpsimp"><a name="p6040mcpsimp"></a><a name="p6040mcpsimp"></a>详细定义请见《SVPx.0 API参考》</p>
</td>
</tr>
<tr id="row6041mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.3.1.1 "><p id="p6043mcpsimp"><a name="p6043mcpsimp"></a><a name="p6043mcpsimp"></a>quality_level</p>
</td>
<td class="cellrowborder" valign="top" width="79%" headers="mcps1.1.3.1.2 "><p id="p6045mcpsimp"><a name="p6045mcpsimp"></a><a name="p6045mcpsimp"></a>ShiTomasi角点质量控制参数，角点响应值小于 “quality_level *最大角点响应值”的点将直接被确认为非角点。</p>
<p id="p6046mcpsimp"><a name="p6046mcpsimp"></a><a name="p6046mcpsimp"></a>取值范围：[1, 255]。</p>
<p id="p6047mcpsimp"><a name="p6047mcpsimp"></a><a name="p6047mcpsimp"></a>参考取值：25。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无

### ot\_ive\_st\_corner\_info<a name="ZH-CN_TOPIC_0000002503971265"></a>

【说明】

定义Shi-Tomas-like角点计算输出的角点信息结构体。

【定义】

```
typedef struct {
    td_u16 corner_num;
    ot_svp_point_u16 corner[OT_IVE_ST_MAX_CORNER_NUM];
} ot_ive_st_corner_info;
```

【成员】

<a name="table4641mcpsimp"></a>
<table><thead align="left"><tr id="row4646mcpsimp"><th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.1"><p id="p4648mcpsimp"><a name="p4648mcpsimp"></a><a name="p4648mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.2"><p id="p4650mcpsimp"><a name="p4650mcpsimp"></a><a name="p4650mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4652mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p4654mcpsimp"><a name="p4654mcpsimp"></a><a name="p4654mcpsimp"></a>corner_num</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p4656mcpsimp"><a name="p4656mcpsimp"></a><a name="p4656mcpsimp"></a>有效角点数目。</p>
</td>
</tr>
<tr id="row4657mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p4659mcpsimp"><a name="p4659mcpsimp"></a><a name="p4659mcpsimp"></a>corner [<a href="OT_IVE_ST_MAX_CORNER_NUM.md">OT_IVE_ST_MAX_CORNER_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p4662mcpsimp"><a name="p4662mcpsimp"></a><a name="p4662mcpsimp"></a>角点坐标数组。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

### ot\_ive\_st\_corner\_ctrl<a name="ZH-CN_TOPIC_0000002471091220"></a>

【说明】

定义Shi-Tomas-like角点筛选控制参数。

【定义】

```
typedef struct {
    td_u16 max_corner_num;
    td_u16 min_dist;
} ot_ive_st_corner_ctrl;
```

【成员】

<a name="table7633mcpsimp"></a>
<table><thead align="left"><tr id="row7638mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p7640mcpsimp"><a name="p7640mcpsimp"></a><a name="p7640mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p7642mcpsimp"><a name="p7642mcpsimp"></a><a name="p7642mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7644mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p7646mcpsimp"><a name="p7646mcpsimp"></a><a name="p7646mcpsimp"></a>max_corner_num</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p7648mcpsimp"><a name="p7648mcpsimp"></a><a name="p7648mcpsimp"></a>最大角点数目。</p>
<p id="p7649mcpsimp"><a name="p7649mcpsimp"></a><a name="p7649mcpsimp"></a>取值范围：[1, 500]。</p>
</td>
</tr>
<tr id="row7650mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p7652mcpsimp"><a name="p7652mcpsimp"></a><a name="p7652mcpsimp"></a>min_dist</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p7654mcpsimp"><a name="p7654mcpsimp"></a><a name="p7654mcpsimp"></a>相邻角点最小距离。</p>
<p id="p7655mcpsimp"><a name="p7655mcpsimp"></a><a name="p7655mcpsimp"></a>取值范围：[1, 255]。</p>
<p id="p7656mcpsimp"><a name="p7656mcpsimp"></a><a name="p7656mcpsimp"></a>参考取值：10。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_sad\_mode<a name="ZH-CN_TOPIC_0000002470931274"></a>

【说明】

定义SAD计算模式。

【定义】

```
typedef enum {
    OT_IVE_SAD_MODE_MB_4X4 = 0x0,   /* 4x4 */
    OT_IVE_SAD_MODE_MB_8X8 = 0x1,   /* 8x8 */
    OT_IVE_SAD_MODE_MB_16X16 = 0x2, /* 16x16 */
    OT_IVE_SAD_MODE_BUTT
} ot_ive_sad_mode;
```

【成员】

<a name="table6803mcpsimp"></a>
<table><thead align="left"><tr id="row6808mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p6810mcpsimp"><a name="p6810mcpsimp"></a><a name="p6810mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p6812mcpsimp"><a name="p6812mcpsimp"></a><a name="p6812mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6814mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p6816mcpsimp"><a name="p6816mcpsimp"></a><a name="p6816mcpsimp"></a>OT_IVE_SAD_MODE_MB_4X4</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p6818mcpsimp"><a name="p6818mcpsimp"></a><a name="p6818mcpsimp"></a>按4x4像素块计算SAD。</p>
</td>
</tr>
<tr id="row6819mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p6821mcpsimp"><a name="p6821mcpsimp"></a><a name="p6821mcpsimp"></a>OT_IVE_SAD_MODE_MB_8X8</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p6823mcpsimp"><a name="p6823mcpsimp"></a><a name="p6823mcpsimp"></a>按8x8像素块计算SAD。</p>
</td>
</tr>
<tr id="row6824mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p6826mcpsimp"><a name="p6826mcpsimp"></a><a name="p6826mcpsimp"></a>OT_IVE_SAD_MODE_MB_16X16</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p6828mcpsimp"><a name="p6828mcpsimp"></a><a name="p6828mcpsimp"></a>按16x16像素块计算SAD。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_sad\_ctrl](#ot_ive_sad_ctrl)

### ot\_ive\_sad\_out\_ctrl<a name="ZH-CN_TOPIC_0000002471091278"></a>

【说明】

定义SAD输出控制模式。

【定义】

```
typedef enum {
    OT_IVE_SAD_OUT_CTRL_16BIT_BOTH = 0x0, /* Output 16 bit sad and thresh */
    OT_IVE_SAD_OUT_CTRL_8BIT_BOTH = 0x1,  /* Output 8 bit sad and thresh */
    OT_IVE_SAD_OUT_CTRL_16BIT_SAD = 0x2,  /* Output 16 bit sad */
    OT_IVE_SAD_OUT_CTRL_8BIT_SAD = 0x3,   /* Output 8 bit sad */
    OT_IVE_SAD_OUT_CTRL_THRESHOLD = 0x4,   /* Output thresh,16 bits sad */
    OT_IVE_SAD_OUT_CTRL_BUTT
} ot_ive_sad_out_ctrl;
```

【成员】

<a name="table13518mcpsimp"></a>
<table><thead align="left"><tr id="row13523mcpsimp"><th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.1"><p id="p13525mcpsimp"><a name="p13525mcpsimp"></a><a name="p13525mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.2"><p id="p13527mcpsimp"><a name="p13527mcpsimp"></a><a name="p13527mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row13529mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p13531mcpsimp"><a name="p13531mcpsimp"></a><a name="p13531mcpsimp"></a>OT_IVE_SAD_OUT_CTRL_16BIT_BOTH</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p13533mcpsimp"><a name="p13533mcpsimp"></a><a name="p13533mcpsimp"></a>16 bit SAD图和阈值化图输出模式。</p>
</td>
</tr>
<tr id="row13534mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p13536mcpsimp"><a name="p13536mcpsimp"></a><a name="p13536mcpsimp"></a>OT_IVE_SAD_OUT_CTRL_8BIT_BOTH</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p13538mcpsimp"><a name="p13538mcpsimp"></a><a name="p13538mcpsimp"></a>8 bit SAD图和阈值化图输出模式。</p>
</td>
</tr>
<tr id="row13539mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p13541mcpsimp"><a name="p13541mcpsimp"></a><a name="p13541mcpsimp"></a>OT_IVE_SAD_OUT_CTRL_16BIT_SAD</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p13543mcpsimp"><a name="p13543mcpsimp"></a><a name="p13543mcpsimp"></a>16 bit SAD图输出模式。</p>
</td>
</tr>
<tr id="row13544mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p13546mcpsimp"><a name="p13546mcpsimp"></a><a name="p13546mcpsimp"></a>OT_IVE_SAD_OUT_CTRL_8BIT_SAD</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p13548mcpsimp"><a name="p13548mcpsimp"></a><a name="p13548mcpsimp"></a>8 bit SAD图输出模式。</p>
</td>
</tr>
<tr id="row13549mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p13551mcpsimp"><a name="p13551mcpsimp"></a><a name="p13551mcpsimp"></a>OT_IVE_SAD_OUT_CTRL_THRESHOLD</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p13553mcpsimp"><a name="p13553mcpsimp"></a><a name="p13553mcpsimp"></a>阈值化图输出模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_sad\_ctrl](#ot_ive_sad_ctrl)

### ot\_ive\_sad\_ctrl<a name="ZH-CN_TOPIC_0000002471091212"></a>

【说明】

定义SAD控制参数。

【定义】

```
typedef struct {
    ot_ive_sad_mode mode;
    ot_ive_sad_out_ctrl out_ctrl;
    td_u16 threshold;    /* src_val <= thr, dst_val = min_val; src_val > thr, dst_val = max_val. */
    td_u8 min_val;      /* Min value */
    td_u8 max_val;      /* Max value */
} ot_ive_sad_ctrl;
```

【成员】

<a name="table9090mcpsimp"></a>
<table><thead align="left"><tr id="row9095mcpsimp"><th class="cellrowborder" valign="top" width="36%" id="mcps1.1.3.1.1"><p id="p9097mcpsimp"><a name="p9097mcpsimp"></a><a name="p9097mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.3.1.2"><p id="p9099mcpsimp"><a name="p9099mcpsimp"></a><a name="p9099mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row9101mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p9103mcpsimp"><a name="p9103mcpsimp"></a><a name="p9103mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p9105mcpsimp"><a name="p9105mcpsimp"></a><a name="p9105mcpsimp"></a>SAD计算模式。</p>
</td>
</tr>
<tr id="row9106mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p9108mcpsimp"><a name="p9108mcpsimp"></a><a name="p9108mcpsimp"></a>out_ctrl</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p9110mcpsimp"><a name="p9110mcpsimp"></a><a name="p9110mcpsimp"></a>SAD输出控制模式。</p>
</td>
</tr>
<tr id="row9111mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p9113mcpsimp"><a name="p9113mcpsimp"></a><a name="p9113mcpsimp"></a>threshold</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p9115mcpsimp"><a name="p9115mcpsimp"></a><a name="p9115mcpsimp"></a>对计算的SAD图进行阈值化的阈值。</p>
<p id="p9116mcpsimp"><a name="p9116mcpsimp"></a><a name="p9116mcpsimp"></a>取值范围依赖out_ctrl：</p>
<a name="ul9117mcpsimp"></a><a name="ul9117mcpsimp"></a><ul id="ul9117mcpsimp"><li>OT_IVE_SAD_OUT_CTRL_8BIT_BOTH，取值[0, 255]</li><li>OT_IVE_SAD_OUT_CTRL_16BIT_BOTH和OT_IVE_SAD_OUT_CTRL_THRESHOLD，取值[0, 65535]</li></ul>
</td>
</tr>
<tr id="row9120mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p9122mcpsimp"><a name="p9122mcpsimp"></a><a name="p9122mcpsimp"></a>min_val</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p9124mcpsimp"><a name="p9124mcpsimp"></a><a name="p9124mcpsimp"></a>阈值化不超过thr时的取值。</p>
</td>
</tr>
<tr id="row9125mcpsimp"><td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.1 "><p id="p9127mcpsimp"><a name="p9127mcpsimp"></a><a name="p9127mcpsimp"></a>max_val</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.2 "><p id="p9129mcpsimp"><a name="p9129mcpsimp"></a><a name="p9129mcpsimp"></a>阈值化超过thr时的取值。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

-   [ot\_ive\_sad\_mode](#ot_ive_sad_mode)
-   [ot\_ive\_sad\_out\_ctrl](#ot_ive_sad_out_ctrl)

### ot\_ive\_resize\_mode<a name="ZH-CN_TOPIC_0000002503971219"></a>

【说明】

定义resize的模式。

【定义】

```
typedef enum {
    OT_IVE_RESIZE_MODE_LINEAR = 0x0, /* Bilinear interpolation */
    OT_IVE_RESIZE_MODE_AREA = 0x1,
    OT_IVE_RESIZE_MODE_BUTT
} ot_ive_resize_mode;
```

【成员】

<a name="table14891mcpsimp"></a>
<table><thead align="left"><tr id="row14896mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p14898mcpsimp"><a name="p14898mcpsimp"></a><a name="p14898mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p14900mcpsimp"><a name="p14900mcpsimp"></a><a name="p14900mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14902mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p14904mcpsimp"><a name="p14904mcpsimp"></a><a name="p14904mcpsimp"></a>OT_IVE_RESIZE_MODE_LINEAR</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p14906mcpsimp"><a name="p14906mcpsimp"></a><a name="p14906mcpsimp"></a>双向性插值缩放模式。</p>
</td>
</tr>
<tr id="row14907mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p14909mcpsimp"><a name="p14909mcpsimp"></a><a name="p14909mcpsimp"></a>OT_IVE_RESIZE_MODE_AREA</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p14911mcpsimp"><a name="p14911mcpsimp"></a><a name="p14911mcpsimp"></a>区域插值缩放模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_resize\_ctrl<a name="ZH-CN_TOPIC_0000002471091258"></a>

【说明】

定义resize控制参数。

【定义】

```
typedef struct {
    ot_ive_resize_mode mode;
    ot_svp_mem_info mem;
    td_u16 num;
} ot_ive_resize_ctrl;
```

【成员】

<a name="table14980mcpsimp"></a>
<table><thead align="left"><tr id="row14985mcpsimp"><th class="cellrowborder" valign="top" width="32%" id="mcps1.1.3.1.1"><p id="p14987mcpsimp"><a name="p14987mcpsimp"></a><a name="p14987mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.3.1.2"><p id="p14989mcpsimp"><a name="p14989mcpsimp"></a><a name="p14989mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14991mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p14993mcpsimp"><a name="p14993mcpsimp"></a><a name="p14993mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p14995mcpsimp"><a name="p14995mcpsimp"></a><a name="p14995mcpsimp"></a>缩放模式。</p>
</td>
</tr>
<tr id="row14996mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p14998mcpsimp"><a name="p14998mcpsimp"></a><a name="p14998mcpsimp"></a>mem</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p15000mcpsimp"><a name="p15000mcpsimp"></a><a name="p15000mcpsimp"></a>辅助内存。开辟参照ss_mpi_ive_resize中【注意】。</p>
<p id="p15002mcpsimp"><a name="p15002mcpsimp"></a><a name="p15002mcpsimp"></a>详细定义请见《SVPx.0 API参考》</p>
</td>
</tr>
<tr id="row15003mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p15005mcpsimp"><a name="p15005mcpsimp"></a><a name="p15005mcpsimp"></a>num</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p15007mcpsimp"><a name="p15007mcpsimp"></a><a name="p15007mcpsimp"></a>图像数目，取值范围：[1, 64]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_grad\_fg\_mode<a name="ZH-CN_TOPIC_0000002503971161"></a>

【说明】

定义梯度前景计算模式。

【定义】

```
typedef enum {
    OT_IVE_GRAD_FG_MODE_USE_CUR_GRAD = 0x0,
    OT_IVE_GRAD_FG_MODE_FIND_MIN_GRAD = 0x1,
   OT_IVE_GRAD_FG_MODE_BUTT
} ot_ive_grad_fg_mode;
```

【成员】

<a name="table11889mcpsimp"></a>
<table><thead align="left"><tr id="row11894mcpsimp"><th class="cellrowborder" valign="top" width="59%" id="mcps1.1.3.1.1"><p id="p11896mcpsimp"><a name="p11896mcpsimp"></a><a name="p11896mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="41%" id="mcps1.1.3.1.2"><p id="p11898mcpsimp"><a name="p11898mcpsimp"></a><a name="p11898mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row11900mcpsimp"><td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.1 "><p id="p11902mcpsimp"><a name="p11902mcpsimp"></a><a name="p11902mcpsimp"></a>OT_IVE_GRAD_FG_MODE_USE_CUR_GRAD</p>
</td>
<td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.2 "><p id="p11904mcpsimp"><a name="p11904mcpsimp"></a><a name="p11904mcpsimp"></a>当前位置梯度计算模式。</p>
</td>
</tr>
<tr id="row11905mcpsimp"><td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.1 "><p id="p11907mcpsimp"><a name="p11907mcpsimp"></a><a name="p11907mcpsimp"></a>OT_IVE_GRAD_FG_MODE_FIND_MIN_GRAD</p>
</td>
<td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.2 "><p id="p11909mcpsimp"><a name="p11909mcpsimp"></a><a name="p11909mcpsimp"></a>周边最小梯度计算模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_grad\_fg\_ctrl](#ot_ive_grad_fg_ctrl)

### ot\_ive\_grad\_fg\_ctrl<a name="ZH-CN_TOPIC_0000002504091111"></a>

【说明】

定义计算梯度前景控制参数。

【定义】

```
typedef struct {
    ot_ive_grad_fg_mode mode;  /* Calculation mode */
    td_u16 edw_factor;         /* Edge width adjustment factor (range: 500 to 2000; default: 1000) */
    td_u8 crl_coef_threshold;       /* Gradient vector correlation coefficient threshold
                                (ranges: 50 to 100; default: 80) */
    td_u8 mag_crl_threshold;       /* Gradient amplitude threshold (range: 0 to 20; default: 4) */
    td_u8 min_mag_diff;        /* Gradient magnitude difference threshold (range: 2 to 8; default: 2) */
    td_u8 noise_val;           /* Gradient amplitude noise threshold (range: 1 to 8; default: 1) */
    td_u8 edw_dark;            /* Black pixels enable flag (range: 0 (no), 1 (yes); default: 1) */
} ot_ive_grad_fg_ctrl;
```

【成员】

<a name="table208mcpsimp"></a>
<table><thead align="left"><tr id="row213mcpsimp"><th class="cellrowborder" valign="top" width="27%" id="mcps1.1.3.1.1"><p id="p215mcpsimp"><a name="p215mcpsimp"></a><a name="p215mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="73%" id="mcps1.1.3.1.2"><p id="p217mcpsimp"><a name="p217mcpsimp"></a><a name="p217mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row219mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.3.1.1 "><p id="p221mcpsimp"><a name="p221mcpsimp"></a><a name="p221mcpsimp"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="73%" headers="mcps1.1.3.1.2 "><p id="p223mcpsimp"><a name="p223mcpsimp"></a><a name="p223mcpsimp"></a>梯度前景计算模式。参考<a href="ot_ive_grad_fg_mode.md">ot_ive_grad_fg_mode</a>。</p>
</td>
</tr>
<tr id="row225mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.3.1.1 "><p id="p227mcpsimp"><a name="p227mcpsimp"></a><a name="p227mcpsimp"></a>edw_factor</p>
</td>
<td class="cellrowborder" valign="top" width="73%" headers="mcps1.1.3.1.2 "><p id="p229mcpsimp"><a name="p229mcpsimp"></a><a name="p229mcpsimp"></a>边缘宽度调节因子。</p>
<p id="p230mcpsimp"><a name="p230mcpsimp"></a><a name="p230mcpsimp"></a>取值范围：[500, 2000]</p>
<p id="p231mcpsimp"><a name="p231mcpsimp"></a><a name="p231mcpsimp"></a>参考取值：1000</p>
</td>
</tr>
<tr id="row232mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.3.1.1 "><p id="p234mcpsimp"><a name="p234mcpsimp"></a><a name="p234mcpsimp"></a>crl_coef_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="73%" headers="mcps1.1.3.1.2 "><p id="p236mcpsimp"><a name="p236mcpsimp"></a><a name="p236mcpsimp"></a>梯度向量相关系数阈值。</p>
<p id="p237mcpsimp"><a name="p237mcpsimp"></a><a name="p237mcpsimp"></a>取值范围：[50, 100]</p>
<p id="p238mcpsimp"><a name="p238mcpsimp"></a><a name="p238mcpsimp"></a>参考取值：80</p>
</td>
</tr>
<tr id="row239mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.3.1.1 "><p id="p241mcpsimp"><a name="p241mcpsimp"></a><a name="p241mcpsimp"></a>mag_crl_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="73%" headers="mcps1.1.3.1.2 "><p id="p243mcpsimp"><a name="p243mcpsimp"></a><a name="p243mcpsimp"></a>梯度幅值阈值。</p>
<p id="p244mcpsimp"><a name="p244mcpsimp"></a><a name="p244mcpsimp"></a>取值范围：[0, 20]</p>
<p id="p245mcpsimp"><a name="p245mcpsimp"></a><a name="p245mcpsimp"></a>参考取值：4</p>
</td>
</tr>
<tr id="row246mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.3.1.1 "><p id="p248mcpsimp"><a name="p248mcpsimp"></a><a name="p248mcpsimp"></a>min_mag_diff</p>
</td>
<td class="cellrowborder" valign="top" width="73%" headers="mcps1.1.3.1.2 "><p id="p250mcpsimp"><a name="p250mcpsimp"></a><a name="p250mcpsimp"></a>梯度幅值差值阈值。</p>
<p id="p251mcpsimp"><a name="p251mcpsimp"></a><a name="p251mcpsimp"></a>取值范围：[2, 8]</p>
<p id="p252mcpsimp"><a name="p252mcpsimp"></a><a name="p252mcpsimp"></a>参考取值：2</p>
</td>
</tr>
<tr id="row253mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.3.1.1 "><p id="p255mcpsimp"><a name="p255mcpsimp"></a><a name="p255mcpsimp"></a>noise_val</p>
</td>
<td class="cellrowborder" valign="top" width="73%" headers="mcps1.1.3.1.2 "><p id="p257mcpsimp"><a name="p257mcpsimp"></a><a name="p257mcpsimp"></a>梯度幅值噪声阈值。</p>
<p id="p258mcpsimp"><a name="p258mcpsimp"></a><a name="p258mcpsimp"></a>取值范围：[1, 8]</p>
<p id="p259mcpsimp"><a name="p259mcpsimp"></a><a name="p259mcpsimp"></a>参考取值：1</p>
</td>
</tr>
<tr id="row260mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.3.1.1 "><p id="p262mcpsimp"><a name="p262mcpsimp"></a><a name="p262mcpsimp"></a>edw_dark</p>
</td>
<td class="cellrowborder" valign="top" width="73%" headers="mcps1.1.3.1.2 "><p id="p264mcpsimp"><a name="p264mcpsimp"></a><a name="p264mcpsimp"></a>黑像素使能标志，0表示不开启，1表示开启。</p>
<p id="p265mcpsimp"><a name="p265mcpsimp"></a><a name="p265mcpsimp"></a>参考取值：1</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_grad\_fg\_mode](#ot_ive_grad_fg_mode)

### ot\_ive\_cand\_bg\_pixel<a name="ZH-CN_TOPIC_0000002503971261"></a>

【说明】

定义候选背景模型数据。

【定义】

```
typedef struct {
    td_u8q4f4 mean;         /* Candidate background grays value */
    td_u16 start_time;      /* Candidate Background start time */
    td_u16 sum_access_time; /* Candidate Background cumulative access time */
    td_u16 short_keep_time; /* Candidate background short hold time */
    td_u8 chg_condition;    /* Time condition for candidate background into the changing state */
    td_u8 poten_bg_life;    /* Potential background cumulative access time */
} ot_ive_cand_bg_pixel;
```

【成员】

<a name="table6458mcpsimp"></a>
<table><thead align="left"><tr id="row6463mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p6465mcpsimp"><a name="p6465mcpsimp"></a><a name="p6465mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p6467mcpsimp"><a name="p6467mcpsimp"></a><a name="p6467mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6469mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p6471mcpsimp"><a name="p6471mcpsimp"></a><a name="p6471mcpsimp"></a>mean</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p6473mcpsimp"><a name="p6473mcpsimp"></a><a name="p6473mcpsimp"></a>候选背景灰度均值，高12位为候选背景像素值，低4位为状态标识。</p>
</td>
</tr>
<tr id="row6474mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p6476mcpsimp"><a name="p6476mcpsimp"></a><a name="p6476mcpsimp"></a>start_time</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p6478mcpsimp"><a name="p6478mcpsimp"></a><a name="p6478mcpsimp"></a>候选背景起始时间。</p>
</td>
</tr>
<tr id="row6479mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p6481mcpsimp"><a name="p6481mcpsimp"></a><a name="p6481mcpsimp"></a>sum_access_time</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p6483mcpsimp"><a name="p6483mcpsimp"></a><a name="p6483mcpsimp"></a>候选背景累计访问时间。</p>
</td>
</tr>
<tr id="row6484mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p6486mcpsimp"><a name="p6486mcpsimp"></a><a name="p6486mcpsimp"></a>short_keep_time</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p6488mcpsimp"><a name="p6488mcpsimp"></a><a name="p6488mcpsimp"></a>候选背景短时保持时间。</p>
</td>
</tr>
<tr id="row6489mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p6491mcpsimp"><a name="p6491mcpsimp"></a><a name="p6491mcpsimp"></a>chg_cond</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p6493mcpsimp"><a name="p6493mcpsimp"></a><a name="p6493mcpsimp"></a>候选背景进入变换状态时间条件。</p>
</td>
</tr>
<tr id="row6494mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p6496mcpsimp"><a name="p6496mcpsimp"></a><a name="p6496mcpsimp"></a>poten_bg_life</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p6498mcpsimp"><a name="p6498mcpsimp"></a><a name="p6498mcpsimp"></a>潜在背景生命值。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

-   [ot\_ive\_wrok\_bg\_pixel](#ot_ive_wrok_bg_pixel)
-   [ot\_ive\_bg\_model\_pixel](#ot_ive_bg_model_pixel)

### ot\_ive\_wrok\_bg\_pixel<a name="ZH-CN_TOPIC_0000002503971171"></a>

【说明】

定义工作背景模型数据。

【定义】

```
typedef struct {
    td_u8q4f4 mean;     /* 0# background grays value */
    td_u16 access_time;    /* Background cumulative access time */
    td_u8 prev_gray;     /* Gray value of last pixel */
    td_u5q3 diff_threshold;   /* Differential threshold */
    td_u8 access_flag;     /* Background access flag */
    td_u8 bg_gray[OT_IVE_ARR_NUM_THREE];   /* 1# ~ 3# background grays value */
} ot_ive_wrok_bg_pixel;
```

【成员】

<a name="table1825mcpsimp"></a>
<table><thead align="left"><tr id="row1830mcpsimp"><th class="cellrowborder" valign="top" width="39%" id="mcps1.1.3.1.1"><p id="p1832mcpsimp"><a name="p1832mcpsimp"></a><a name="p1832mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="61%" id="mcps1.1.3.1.2"><p id="p1834mcpsimp"><a name="p1834mcpsimp"></a><a name="p1834mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1836mcpsimp"><td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.1 "><p id="p1838mcpsimp"><a name="p1838mcpsimp"></a><a name="p1838mcpsimp"></a>mean</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.2 "><p id="p1840mcpsimp"><a name="p1840mcpsimp"></a><a name="p1840mcpsimp"></a>0号工作背景灰度值，高12位为0号工作背景像素值，低4位为工作背景状态标识。</p>
</td>
</tr>
<tr id="row1841mcpsimp"><td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.1 "><p id="p1843mcpsimp"><a name="p1843mcpsimp"></a><a name="p1843mcpsimp"></a>access_time</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.2 "><p id="p1845mcpsimp"><a name="p1845mcpsimp"></a><a name="p1845mcpsimp"></a>用于工作背景有效性检查的背景累计访问时间。</p>
</td>
</tr>
<tr id="row1846mcpsimp"><td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.1 "><p id="p1848mcpsimp"><a name="p1848mcpsimp"></a><a name="p1848mcpsimp"></a>prev_gray</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.2 "><p id="p1850mcpsimp"><a name="p1850mcpsimp"></a><a name="p1850mcpsimp"></a>前一帧像素灰度值。</p>
</td>
</tr>
<tr id="row1851mcpsimp"><td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.1 "><p id="p1853mcpsimp"><a name="p1853mcpsimp"></a><a name="p1853mcpsimp"></a>diff_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.2 "><p id="p1855mcpsimp"><a name="p1855mcpsimp"></a><a name="p1855mcpsimp"></a>差分阈值。</p>
</td>
</tr>
<tr id="row1856mcpsimp"><td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.1 "><p id="p1858mcpsimp"><a name="p1858mcpsimp"></a><a name="p1858mcpsimp"></a>access_flag</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.2 "><p id="p1860mcpsimp"><a name="p1860mcpsimp"></a><a name="p1860mcpsimp"></a>工作背景访问标识。</p>
</td>
</tr>
<tr id="row1861mcpsimp"><td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.1 "><p id="p1863mcpsimp"><a name="p1863mcpsimp"></a><a name="p1863mcpsimp"></a>bg_gray [<a href="OT_IVE_ARR_NUM_THREE.md">OT_IVE_ARR_NUM_THREE</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.2 "><p id="p1867mcpsimp"><a name="p1867mcpsimp"></a><a name="p1867mcpsimp"></a>1～3号背景灰度值。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_ive\_cand\_bg\_pixel](#ot_ive_cand_bg_pixel)
-   [ot\_ive\_bg\_model\_pixel](#ot_ive_bg_model_pixel)

### ot\_ive\_bg\_life<a name="ZH-CN_TOPIC_0000002471091208"></a>

【说明】

定义背景生命力数据。

【定义】

```
typedef struct {
    td_u8 work_bg_life[OT_IVE_ARR_NUM_THREE]; /* 1# ~ 3# background vitality */
    td_u8 cand_bg_life;   /* Candidate background vitality */
} ot_ive_bg_life;
```

【成员】

<a name="table6518mcpsimp"></a>
<table><thead align="left"><tr id="row6523mcpsimp"><th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.3.1.1"><p id="p6525mcpsimp"><a name="p6525mcpsimp"></a><a name="p6525mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="45%" id="mcps1.1.3.1.2"><p id="p6527mcpsimp"><a name="p6527mcpsimp"></a><a name="p6527mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6529mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p6531mcpsimp"><a name="p6531mcpsimp"></a><a name="p6531mcpsimp"></a>work_bg_life [<a href="OT_IVE_ARR_NUM_THREE.md">OT_IVE_ARR_NUM_THREE</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p6535mcpsimp"><a name="p6535mcpsimp"></a><a name="p6535mcpsimp"></a>1～3号工作背景生命力。</p>
</td>
</tr>
<tr id="row6536mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p6538mcpsimp"><a name="p6538mcpsimp"></a><a name="p6538mcpsimp"></a>cand_bg_life</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p6540mcpsimp"><a name="p6540mcpsimp"></a><a name="p6540mcpsimp"></a>候选背景生命力。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ot\_ive\_bg\_model\_pixel](#ot_ive_bg_model_pixel)

### ot\_ive\_bg\_model\_pixel<a name="ZH-CN_TOPIC_0000002503971177"></a>

【说明】

定义背景模型数据。

【定义】

```
typedef struct {
    ot_ive_wrok_bg_pixel work_bg_pixel;   /* Working background */
    ot_ive_cand_bg_pixel cand_pixel;    /* Candidate background */
    ot_ive_bg_life bg_life;             /* Background vitality */
} ot_ive_bg_model_pixel;
```

【成员】

<a name="table10575mcpsimp"></a>
<table><thead align="left"><tr id="row10580mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p10582mcpsimp"><a name="p10582mcpsimp"></a><a name="p10582mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p10584mcpsimp"><a name="p10584mcpsimp"></a><a name="p10584mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10586mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p10588mcpsimp"><a name="p10588mcpsimp"></a><a name="p10588mcpsimp"></a>work_bg_pixel</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p10590mcpsimp"><a name="p10590mcpsimp"></a><a name="p10590mcpsimp"></a>工作背景数据。</p>
</td>
</tr>
<tr id="row10591mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p10593mcpsimp"><a name="p10593mcpsimp"></a><a name="p10593mcpsimp"></a>cand_pixel</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p10595mcpsimp"><a name="p10595mcpsimp"></a><a name="p10595mcpsimp"></a>候选背景数据。</p>
</td>
</tr>
<tr id="row10596mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p10598mcpsimp"><a name="p10598mcpsimp"></a><a name="p10598mcpsimp"></a>bg_life</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p10600mcpsimp"><a name="p10600mcpsimp"></a><a name="p10600mcpsimp"></a>背景生命力。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

-   [ot\_ive\_cand\_bg\_pixel](#ot_ive_cand_bg_pixel)
-   [ot\_ive\_wrok\_bg\_pixel](#ot_ive_wrok_bg_pixel)
-   [ot\_ive\_bg\_life](#ot_ive_bg_life)

### ot\_ive\_fg\_status\_data<a name="ZH-CN_TOPIC_0000002503971239"></a>

【说明】

定义前景状态数据。

【定义】

```
typedef struct {
    td_u32 pixel_num;
    td_u32 sum_luma;
    td_u8 reserved[OT_IVE_ARR_NUM_EIGHT];
} ot_ive_fg_status_data;
```

【成员】

<a name="table9150mcpsimp"></a>
<table><thead align="left"><tr id="row9155mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p9157mcpsimp"><a name="p9157mcpsimp"></a><a name="p9157mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p9159mcpsimp"><a name="p9159mcpsimp"></a><a name="p9159mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row9161mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p9163mcpsimp"><a name="p9163mcpsimp"></a><a name="p9163mcpsimp"></a>pixel_num</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p9165mcpsimp"><a name="p9165mcpsimp"></a><a name="p9165mcpsimp"></a>前景像素数目。</p>
</td>
</tr>
<tr id="row9166mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p9168mcpsimp"><a name="p9168mcpsimp"></a><a name="p9168mcpsimp"></a>sum_luma</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p9170mcpsimp"><a name="p9170mcpsimp"></a><a name="p9170mcpsimp"></a>输入图像的所有像素亮度累加和。</p>
</td>
</tr>
<tr id="row9171mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p9173mcpsimp"><a name="p9173mcpsimp"></a><a name="p9173mcpsimp"></a>reserved[<a href="OT_IVE_ARR_NUM_EIGHT.md">OT_IVE_ARR_NUM_EIGHT</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p9177mcpsimp"><a name="p9177mcpsimp"></a><a name="p9177mcpsimp"></a>保留字段。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_bg\_status\_data<a name="ZH-CN_TOPIC_0000002503971253"></a>

【说明】

定义背景状态数据。

【定义】

```
typedef struct {
    td_u32 pixel_num;
    td_u32 sum_luma;
    td_u8 reserved[OT_IVE_ARR_NUM_EIGHT];
} ot_ive_bg_status_data;
```

【成员】

<a name="table15242mcpsimp"></a>
<table><thead align="left"><tr id="row15247mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p15249mcpsimp"><a name="p15249mcpsimp"></a><a name="p15249mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p15251mcpsimp"><a name="p15251mcpsimp"></a><a name="p15251mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row15253mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p15255mcpsimp"><a name="p15255mcpsimp"></a><a name="p15255mcpsimp"></a>pixel_num</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p15257mcpsimp"><a name="p15257mcpsimp"></a><a name="p15257mcpsimp"></a>背景像素数目。</p>
</td>
</tr>
<tr id="row15258mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p15260mcpsimp"><a name="p15260mcpsimp"></a><a name="p15260mcpsimp"></a>sum_luma</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p15262mcpsimp"><a name="p15262mcpsimp"></a><a name="p15262mcpsimp"></a>背景图像的所有像素亮度累加和。</p>
</td>
</tr>
<tr id="row15263mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p15265mcpsimp"><a name="p15265mcpsimp"></a><a name="p15265mcpsimp"></a>reserved[<a href="OT_IVE_ARR_NUM_EIGHT.md">OT_IVE_ARR_NUM_EIGHT</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p15269mcpsimp"><a name="p15269mcpsimp"></a><a name="p15269mcpsimp"></a>保留字段。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_match\_bg\_model\_ctrl<a name="ZH-CN_TOPIC_0000002503971249"></a>

【说明】

定义背景匹配控制参数。

【定义】

```
typedef struct {
    td_u32 cur_frame_num;   /* Current frame timestamp, in frame units */
    td_u32 prev_frame_num;   /* Previous frame timestamp, in frame units */
    td_u16 time_threshold; /* Potential background replacement time threshold range: 2 to 100 frames; default: 20) */
    td_u8 diff_threshold_crl_coef;    /* Correlation coefficients between differential threshold and gray value (range: 0 to 5; default: 0) */
    td_u8 diff_max_threshold;         /* Maximum of background differential threshold (range: 3 to 15; default: 6) */
    td_u8 diff_min_threshold;         /* Minimum of background differential threshold (range: 3 to 15; default: 4) */
    td_u8 diff_threshold_inc;         /* Dynamic Background differential threshold increment (range: 0 to 6; default: 0) */
    td_u8 fast_learn_rate;      /* Quick background learning rate (range: 0 to 4; default: 2) */
    td_u8 detected_chg_rgn;       /* Whether to detect change rgn (range: 0 (no), 1 (yes); default: 0) */
} ot_ive_match_bg_model_ctrl;
```

【成员】

<a name="table13138mcpsimp"></a>
<table><thead align="left"><tr id="row13143mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p13145mcpsimp"><a name="p13145mcpsimp"></a><a name="p13145mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p13147mcpsimp"><a name="p13147mcpsimp"></a><a name="p13147mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row13149mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13151mcpsimp"><a name="p13151mcpsimp"></a><a name="p13151mcpsimp"></a>cur_frame_num</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13153mcpsimp"><a name="p13153mcpsimp"></a><a name="p13153mcpsimp"></a>当前帧时间。</p>
</td>
</tr>
<tr id="row13154mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13156mcpsimp"><a name="p13156mcpsimp"></a><a name="p13156mcpsimp"></a>prev_frame_num</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13158mcpsimp"><a name="p13158mcpsimp"></a><a name="p13158mcpsimp"></a>前一帧时间。</p>
<p id="p13159mcpsimp"><a name="p13159mcpsimp"></a><a name="p13159mcpsimp"></a>要求prev_frame_num&lt; cur_frame_num。</p>
</td>
</tr>
<tr id="row13160mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13162mcpsimp"><a name="p13162mcpsimp"></a><a name="p13162mcpsimp"></a>time_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13164mcpsimp"><a name="p13164mcpsimp"></a><a name="p13164mcpsimp"></a>潜在背景替换时间阈值。</p>
<p id="p13165mcpsimp"><a name="p13165mcpsimp"></a><a name="p13165mcpsimp"></a>取值范围：[2,100]。</p>
<p id="p13166mcpsimp"><a name="p13166mcpsimp"></a><a name="p13166mcpsimp"></a>参考取值：20</p>
</td>
</tr>
<tr id="row13167mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13169mcpsimp"><a name="p13169mcpsimp"></a><a name="p13169mcpsimp"></a>diff_threshold_crl_coef</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13171mcpsimp"><a name="p13171mcpsimp"></a><a name="p13171mcpsimp"></a>差分阈值与灰度值相关系数。</p>
<p id="p13172mcpsimp"><a name="p13172mcpsimp"></a><a name="p13172mcpsimp"></a>取值范围：[0,5]。</p>
<p id="p13173mcpsimp"><a name="p13173mcpsimp"></a><a name="p13173mcpsimp"></a>参考取值：0</p>
</td>
</tr>
<tr id="row13174mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13176mcpsimp"><a name="p13176mcpsimp"></a><a name="p13176mcpsimp"></a>diff_max_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13178mcpsimp"><a name="p13178mcpsimp"></a><a name="p13178mcpsimp"></a>背景差分阈值调整上限。</p>
<p id="p13179mcpsimp"><a name="p13179mcpsimp"></a><a name="p13179mcpsimp"></a>取值范围：[3,15]。</p>
<p id="p13180mcpsimp"><a name="p13180mcpsimp"></a><a name="p13180mcpsimp"></a>参考取值：6</p>
</td>
</tr>
<tr id="row13181mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13183mcpsimp"><a name="p13183mcpsimp"></a><a name="p13183mcpsimp"></a>diff_min_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13185mcpsimp"><a name="p13185mcpsimp"></a><a name="p13185mcpsimp"></a>背景差分阈值调整下限。</p>
<p id="p13186mcpsimp"><a name="p13186mcpsimp"></a><a name="p13186mcpsimp"></a>取值范围：[3, diff_max_threshold]。</p>
<p id="p13187mcpsimp"><a name="p13187mcpsimp"></a><a name="p13187mcpsimp"></a>参考取值：4</p>
</td>
</tr>
<tr id="row13188mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13190mcpsimp"><a name="p13190mcpsimp"></a><a name="p13190mcpsimp"></a>diff_threshold_inc</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13192mcpsimp"><a name="p13192mcpsimp"></a><a name="p13192mcpsimp"></a>动态背景下差分阈值增量。</p>
<p id="p13193mcpsimp"><a name="p13193mcpsimp"></a><a name="p13193mcpsimp"></a>取值范围：[0,6]。</p>
<p id="p13194mcpsimp"><a name="p13194mcpsimp"></a><a name="p13194mcpsimp"></a>参考取值：0</p>
</td>
</tr>
<tr id="row13195mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13197mcpsimp"><a name="p13197mcpsimp"></a><a name="p13197mcpsimp"></a>fast_learn_rate</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13199mcpsimp"><a name="p13199mcpsimp"></a><a name="p13199mcpsimp"></a>快速背景学习速率。</p>
<p id="p13200mcpsimp"><a name="p13200mcpsimp"></a><a name="p13200mcpsimp"></a>取值范围：[0,4]。</p>
<p id="p13201mcpsimp"><a name="p13201mcpsimp"></a><a name="p13201mcpsimp"></a>参考取值：2</p>
</td>
</tr>
<tr id="row13202mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p13204mcpsimp"><a name="p13204mcpsimp"></a><a name="p13204mcpsimp"></a>detected_chg_rgn</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p13206mcpsimp"><a name="p13206mcpsimp"></a><a name="p13206mcpsimp"></a>是否检测变化区域。</p>
<p id="p13207mcpsimp"><a name="p13207mcpsimp"></a><a name="p13207mcpsimp"></a>取值范围：{0,1}，0表示不检测，1表示检测。</p>
<p id="p13208mcpsimp"><a name="p13208mcpsimp"></a><a name="p13208mcpsimp"></a>参考取值：0</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

### ot\_ive\_update\_bg\_model\_ctrl<a name="ZH-CN_TOPIC_0000002471091248"></a>

【说明】

定义背景更新控制参数。

【定义】

```
typedef struct {
    td_u32 cur_frame_num;         /* Current frame timestamp, in frame units */
    td_u32 prev_check_time;        /* The last time when background status is checked */
    td_u32 frame_check_period;      /* Background status checking period (range: 0 to 2000 frames; default: 50) */
 
    td_u32 init_min_time;       /* Background initialization shortest time
                                (range: 20 to 6000 frames; default: 100) */
    td_u32 steady_bg_min_blend_time; /* Steady background integration shortest time (range: 20 to 6000 frames; default: 200) */
    td_u32 steady_bg_max_blend_time; /* Steady background integration longest time (range: 20 to 40000 frames; default: 1500) */
    td_u32 dynamic_bg_min_blend_time; /* Dynamic background integration shortest time (range: 0 to 6000 frames; default: 0) */
    td_u32 static_detection_min_time;  /* Still detection shortest time (range: 20 to 6000 frames; default: 80) */
    td_u16 fg_max_fade_time;     /* Foreground disappearing longest time
                                (range: 1 to 255 seconds; default: 15) */
    td_u16 bg_max_fade_time;     /* Background disappearing longest time
                                (range: 1 to 255  seconds ; default: 60) */
 
    td_u8 steady_bg_access_time_rate_thr; /* Steady background access time ratio threshold
                                (range: 10 to 100; default: 80) */
    td_u8 chg_bg_access_time_rate_thr; /* Change background access time ratio threshold (range: 10 to 100; default: 60) */
    td_u8 dynamic_bg_access_time_threshold;     /* Dynamic background access time ratio threshold (range: 0 to 50; default: 0) */
    td_u8 dynamic_bg_depth;          /* Dynamic background depth (range: 0 to 3; default: 3) */
    td_u8 bg_eff_state_rate_threshold;     /* Background state time ratio threshold when initializing (range: 90 to 100; default: 90) */
 
    td_u8 acc_bg_learn;  /* Whether to accelerate background learning (range: 0 (no), 1 (yes); default: 0) */
    td_u8 detected_chg_rgn; /* Whether to detect change rgn (range: 0 (no), 1 (yes); default: 0) */
} ot_ive_update_bg_model_ctrl;
```

【成员】

<a name="table4356mcpsimp"></a>
<table><thead align="left"><tr id="row4361mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p4363mcpsimp"><a name="p4363mcpsimp"></a><a name="p4363mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p4365mcpsimp"><a name="p4365mcpsimp"></a><a name="p4365mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4367mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4369mcpsimp"><a name="p4369mcpsimp"></a><a name="p4369mcpsimp"></a>cur_frame_num</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4371mcpsimp"><a name="p4371mcpsimp"></a><a name="p4371mcpsimp"></a>当前帧时间。</p>
</td>
</tr>
<tr id="row4372mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4374mcpsimp"><a name="p4374mcpsimp"></a><a name="p4374mcpsimp"></a>prev_check_time</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4376mcpsimp"><a name="p4376mcpsimp"></a><a name="p4376mcpsimp"></a>上次进行背景状态检查的时间点。</p>
</td>
</tr>
<tr id="row4377mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4379mcpsimp"><a name="p4379mcpsimp"></a><a name="p4379mcpsimp"></a>frame_check_period</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4381mcpsimp"><a name="p4381mcpsimp"></a><a name="p4381mcpsimp"></a>背景状态检查时间周期。</p>
<p id="p4382mcpsimp"><a name="p4382mcpsimp"></a><a name="p4382mcpsimp"></a>取值范围：[0, 2000]。</p>
<p id="p4383mcpsimp"><a name="p4383mcpsimp"></a><a name="p4383mcpsimp"></a>参考取值：50</p>
</td>
</tr>
<tr id="row4384mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4386mcpsimp"><a name="p4386mcpsimp"></a><a name="p4386mcpsimp"></a>init_min_time</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4388mcpsimp"><a name="p4388mcpsimp"></a><a name="p4388mcpsimp"></a>背景初始化最短时间。</p>
<p id="p4389mcpsimp"><a name="p4389mcpsimp"></a><a name="p4389mcpsimp"></a>取值范围：[20, 6000]。</p>
<p id="p4390mcpsimp"><a name="p4390mcpsimp"></a><a name="p4390mcpsimp"></a>参考取值：100</p>
</td>
</tr>
<tr id="row4391mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4393mcpsimp"><a name="p4393mcpsimp"></a><a name="p4393mcpsimp"></a>steady_bg_min_blend_time</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4395mcpsimp"><a name="p4395mcpsimp"></a><a name="p4395mcpsimp"></a>稳态背景融入最短时间。</p>
<p id="p4396mcpsimp"><a name="p4396mcpsimp"></a><a name="p4396mcpsimp"></a>取值范围：[init_min_time, 6000]。</p>
<p id="p4397mcpsimp"><a name="p4397mcpsimp"></a><a name="p4397mcpsimp"></a>参考取值：200</p>
</td>
</tr>
<tr id="row4398mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4400mcpsimp"><a name="p4400mcpsimp"></a><a name="p4400mcpsimp"></a>steady_bg_max_blend_time</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4402mcpsimp"><a name="p4402mcpsimp"></a><a name="p4402mcpsimp"></a>稳态背景融入最长时间。</p>
<p id="p4403mcpsimp"><a name="p4403mcpsimp"></a><a name="p4403mcpsimp"></a>取值范围：[steady_bg_max_blend_time, 40000]。</p>
<p id="p4404mcpsimp"><a name="p4404mcpsimp"></a><a name="p4404mcpsimp"></a>参考取值：1500</p>
</td>
</tr>
<tr id="row4405mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4407mcpsimp"><a name="p4407mcpsimp"></a><a name="p4407mcpsimp"></a>dynamic_bg_min_blend_time</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4409mcpsimp"><a name="p4409mcpsimp"></a><a name="p4409mcpsimp"></a>动态背景融入最短时间。</p>
<p id="p4410mcpsimp"><a name="p4410mcpsimp"></a><a name="p4410mcpsimp"></a>取值范围：[0, 6000]。</p>
<p id="p4411mcpsimp"><a name="p4411mcpsimp"></a><a name="p4411mcpsimp"></a>参考取值：0</p>
</td>
</tr>
<tr id="row4412mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4414mcpsimp"><a name="p4414mcpsimp"></a><a name="p4414mcpsimp"></a>static_detection_min_time</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4416mcpsimp"><a name="p4416mcpsimp"></a><a name="p4416mcpsimp"></a>静物检测最短时间。</p>
<p id="p4417mcpsimp"><a name="p4417mcpsimp"></a><a name="p4417mcpsimp"></a>取值范围：[20, 6000]。</p>
<p id="p4418mcpsimp"><a name="p4418mcpsimp"></a><a name="p4418mcpsimp"></a>参考取值：80</p>
</td>
</tr>
<tr id="row4419mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4421mcpsimp"><a name="p4421mcpsimp"></a><a name="p4421mcpsimp"></a>fg_max_fade_time</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4423mcpsimp"><a name="p4423mcpsimp"></a><a name="p4423mcpsimp"></a>前景持续消失最长时间。</p>
<p id="p4424mcpsimp"><a name="p4424mcpsimp"></a><a name="p4424mcpsimp"></a>取值范围：[1, 255]。</p>
<p id="p4425mcpsimp"><a name="p4425mcpsimp"></a><a name="p4425mcpsimp"></a>参考取值：15</p>
</td>
</tr>
<tr id="row4426mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4428mcpsimp"><a name="p4428mcpsimp"></a><a name="p4428mcpsimp"></a>bg_max_fade_time</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4430mcpsimp"><a name="p4430mcpsimp"></a><a name="p4430mcpsimp"></a>背景持续消失最长时间。</p>
<p id="p4431mcpsimp"><a name="p4431mcpsimp"></a><a name="p4431mcpsimp"></a>取值范围：[1, 255]。</p>
<p id="p4432mcpsimp"><a name="p4432mcpsimp"></a><a name="p4432mcpsimp"></a>参考取值：60</p>
</td>
</tr>
<tr id="row4433mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4435mcpsimp"><a name="p4435mcpsimp"></a><a name="p4435mcpsimp"></a>steady_bg_access_time_rate_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4437mcpsimp"><a name="p4437mcpsimp"></a><a name="p4437mcpsimp"></a>稳态背景访问时间比率阈值。</p>
<p id="p4438mcpsimp"><a name="p4438mcpsimp"></a><a name="p4438mcpsimp"></a>取值范围：[10, 100]。</p>
<p id="p4439mcpsimp"><a name="p4439mcpsimp"></a><a name="p4439mcpsimp"></a>参考取值：80</p>
</td>
</tr>
<tr id="row4440mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4442mcpsimp"><a name="p4442mcpsimp"></a><a name="p4442mcpsimp"></a>chg_bg_access_time_rate_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4444mcpsimp"><a name="p4444mcpsimp"></a><a name="p4444mcpsimp"></a>变化背景访问时间比率阈值。</p>
<p id="p4445mcpsimp"><a name="p4445mcpsimp"></a><a name="p4445mcpsimp"></a>取值范围：[10, 100]。</p>
<p id="p4446mcpsimp"><a name="p4446mcpsimp"></a><a name="p4446mcpsimp"></a>参考取值：60</p>
</td>
</tr>
<tr id="row4447mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4449mcpsimp"><a name="p4449mcpsimp"></a><a name="p4449mcpsimp"></a>dynamic_bg_access_time_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4451mcpsimp"><a name="p4451mcpsimp"></a><a name="p4451mcpsimp"></a>动态背景访问时间比率阈值。</p>
<p id="p4452mcpsimp"><a name="p4452mcpsimp"></a><a name="p4452mcpsimp"></a>取值范围：[0, 50]。</p>
<p id="p4453mcpsimp"><a name="p4453mcpsimp"></a><a name="p4453mcpsimp"></a>参考取值：0</p>
</td>
</tr>
<tr id="row4454mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4456mcpsimp"><a name="p4456mcpsimp"></a><a name="p4456mcpsimp"></a>dynamic_bg_depth</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4458mcpsimp"><a name="p4458mcpsimp"></a><a name="p4458mcpsimp"></a>动态背景深度。</p>
<p id="p4459mcpsimp"><a name="p4459mcpsimp"></a><a name="p4459mcpsimp"></a>取值范围：[0, 3]。</p>
<p id="p4460mcpsimp"><a name="p4460mcpsimp"></a><a name="p4460mcpsimp"></a>参考取值：3</p>
</td>
</tr>
<tr id="row4461mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4463mcpsimp"><a name="p4463mcpsimp"></a><a name="p4463mcpsimp"></a>bg_eff_state_rate_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4465mcpsimp"><a name="p4465mcpsimp"></a><a name="p4465mcpsimp"></a>景初始化阶段背景状态时间比率阈值。</p>
<p id="p4466mcpsimp"><a name="p4466mcpsimp"></a><a name="p4466mcpsimp"></a>取值范围：[90, 100]。</p>
<p id="p4467mcpsimp"><a name="p4467mcpsimp"></a><a name="p4467mcpsimp"></a>参考取值：90</p>
</td>
</tr>
<tr id="row4468mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4470mcpsimp"><a name="p4470mcpsimp"></a><a name="p4470mcpsimp"></a>acc_bg_learn</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4472mcpsimp"><a name="p4472mcpsimp"></a><a name="p4472mcpsimp"></a>是否加速背景学习。</p>
<p id="p4473mcpsimp"><a name="p4473mcpsimp"></a><a name="p4473mcpsimp"></a>取值范围：{0, 1}，0表示不加速，1表示加速。</p>
<p id="p4474mcpsimp"><a name="p4474mcpsimp"></a><a name="p4474mcpsimp"></a>参考取值：0</p>
</td>
</tr>
<tr id="row4475mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4477mcpsimp"><a name="p4477mcpsimp"></a><a name="p4477mcpsimp"></a>detected_chg_rgn</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4479mcpsimp"><a name="p4479mcpsimp"></a><a name="p4479mcpsimp"></a>是否检测变化区域。</p>
<p id="p4480mcpsimp"><a name="p4480mcpsimp"></a><a name="p4480mcpsimp"></a>取值范围：{0, 1}，0表示不检测，1表示检测。</p>
<p id="p4481mcpsimp"><a name="p4481mcpsimp"></a><a name="p4481mcpsimp"></a>参考取值：0</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

要求init\_min\_time≤ steady\_bg\_min\_blend\_time≤ steady\_bg\_max\_blend\_time。

【相关数据类型及接口】

无。

### ot\_ive\_ann\_mlp\_accurate<a name="ZH-CN_TOPIC_0000002504091185"></a>

【说明】

定义ann\_mlp输入特征向量类型。

【定义】

```
typedef enum {
    OT_IVE_ann_mlp_ACCURATE_SRC16_WGT16 = 0x0, /* input decimals' accurate 16 bit, weight 16bit */
    OT_IVE_ann_mlp_ACCURATE_SRC14_WGT20 = 0x1, /* input decimals' accurate 14 bit, weight 20bit */
    OT_IVE_ann_mlp_ACCURATE_BUTT
} ot_ive_ann_mlp_accurate;
```

【成员】

<a name="table13859mcpsimp"></a>
<table><thead align="left"><tr id="row13864mcpsimp"><th class="cellrowborder" valign="top" width="60%" id="mcps1.1.3.1.1"><p id="p13866mcpsimp"><a name="p13866mcpsimp"></a><a name="p13866mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="40%" id="mcps1.1.3.1.2"><p id="p13868mcpsimp"><a name="p13868mcpsimp"></a><a name="p13868mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row13870mcpsimp"><td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p13872mcpsimp"><a name="p13872mcpsimp"></a><a name="p13872mcpsimp"></a>OT_IVE_ann_mlp_ACCURATE_SRC16_WGT16</p>
</td>
<td class="cellrowborder" valign="top" width="40%" headers="mcps1.1.3.1.2 "><p id="p13874mcpsimp"><a name="p13874mcpsimp"></a><a name="p13874mcpsimp"></a>输入特征向量类型为SQ16.16。</p>
</td>
</tr>
<tr id="row13875mcpsimp"><td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p13877mcpsimp"><a name="p13877mcpsimp"></a><a name="p13877mcpsimp"></a>OT_IVE_ann_mlp_ACCURATE_SRC14_WGT20</p>
</td>
<td class="cellrowborder" valign="top" width="40%" headers="mcps1.1.3.1.2 "><p id="p13879mcpsimp"><a name="p13879mcpsimp"></a><a name="p13879mcpsimp"></a>输入特征向量类型为SQ18.14。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_ann\_mlp\_actv\_func<a name="ZH-CN_TOPIC_0000002503971151"></a>

【说明】

定义ann\_mlp激活函数枚举类型。

【定义】

```
typedef enum {
    OT_IVE_ANN_MLP_ACTV_FUNC_IDENTITY = 0x0,
    OT_IVE_ANN_MLP_ACTV_FUNC_SIGMOID_SYM = 0x1,
    OT_IVE_ANN_MLP_ACTV_FUNC_GAUSSIAN = 0x2,
    OT_IVE_ANN_MLP_ACTV_FUNC_BUTT
} ot_ive_ann_mlp_actv_func;
```

【成员】

<a name="table7672mcpsimp"></a>
<table><thead align="left"><tr id="row7677mcpsimp"><th class="cellrowborder" valign="top" width="64%" id="mcps1.1.3.1.1"><p id="p7679mcpsimp"><a name="p7679mcpsimp"></a><a name="p7679mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="36%" id="mcps1.1.3.1.2"><p id="p7681mcpsimp"><a name="p7681mcpsimp"></a><a name="p7681mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7683mcpsimp"><td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p7685mcpsimp"><a name="p7685mcpsimp"></a><a name="p7685mcpsimp"></a>OT_IVE_ANN_MLP_ACTV_FUNC_IDENTITY</p>
</td>
<td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.2 "><p id="p7687mcpsimp"><a name="p7687mcpsimp"></a><a name="p7687mcpsimp"></a>Identity激活函数。</p>
</td>
</tr>
<tr id="row7688mcpsimp"><td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p7690mcpsimp"><a name="p7690mcpsimp"></a><a name="p7690mcpsimp"></a>OT_IVE_ANN_MLP_ACTV_FUNC_SIGMOID_SYM</p>
</td>
<td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.2 "><p id="p7692mcpsimp"><a name="p7692mcpsimp"></a><a name="p7692mcpsimp"></a>Sigmoid对称激活函数。</p>
</td>
</tr>
<tr id="row7693mcpsimp"><td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p7695mcpsimp"><a name="p7695mcpsimp"></a><a name="p7695mcpsimp"></a>OT_IVE_ANN_MLP_ACTV_FUNC_GAUSSIAN</p>
</td>
<td class="cellrowborder" valign="top" width="36%" headers="mcps1.1.3.1.2 "><p id="p7697mcpsimp"><a name="p7697mcpsimp"></a><a name="p7697mcpsimp"></a>Gaussian激活函数。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

对应函数定义参见ss\_mpi\_ive\_ann\_mlp\_predict中【注意】。

【相关数据类型及接口】

无。

### ot\_ive\_ann\_mlp\_model<a name="ZH-CN_TOPIC_0000002503971233"></a>

【说明】

定义ann\_mlp模型数据结构体。

【定义】

```
typedef struct {
    ot_ive_ann_mlp_actv_func actv_func;
    ot_ive_ann_mlp_accurate accurate;
    ot_svp_mem_info wgt;
    td_u32 total_wgt_size;
    td_u16 layer_cnt[OT_IVE_ARR_NUM_EIGHT];      /* 8 layers, including input and output layer */
    td_u16 max_cnt;           /* MaxCount<=1024 */
    td_u8 layer_num;            /* 2<layerNum<=8 */
    td_u8 reserved;
} ot_ive_ann_mlp_model;
```

【成员】

<a name="table1071mcpsimp"></a>
<table><thead align="left"><tr id="row1076mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.1"><p id="p1078mcpsimp"><a name="p1078mcpsimp"></a><a name="p1078mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.2"><p id="p1080mcpsimp"><a name="p1080mcpsimp"></a><a name="p1080mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1082mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p1084mcpsimp"><a name="p1084mcpsimp"></a><a name="p1084mcpsimp"></a>actv_func</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p1086mcpsimp"><a name="p1086mcpsimp"></a><a name="p1086mcpsimp"></a>激活函数类型。</p>
</td>
</tr>
<tr id="row1087mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p1089mcpsimp"><a name="p1089mcpsimp"></a><a name="p1089mcpsimp"></a>accurate</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p1091mcpsimp"><a name="p1091mcpsimp"></a><a name="p1091mcpsimp"></a>输入特征向量类型。</p>
</td>
</tr>
<tr id="row1092mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p1094mcpsimp"><a name="p1094mcpsimp"></a><a name="p1094mcpsimp"></a>wgt</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p1096mcpsimp"><a name="p1096mcpsimp"></a><a name="p1096mcpsimp"></a>模型数据权重。</p>
<p id="p1097mcpsimp"><a name="p1097mcpsimp"></a><a name="p1097mcpsimp"></a>详细定义请见《SVPx.0 API参考》</p>
</td>
</tr>
<tr id="row1098mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p1100mcpsimp"><a name="p1100mcpsimp"></a><a name="p1100mcpsimp"></a>total_wgt_size</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p1102mcpsimp"><a name="p1102mcpsimp"></a><a name="p1102mcpsimp"></a>总的权重的大小</p>
</td>
</tr>
<tr id="row1103mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p1105mcpsimp"><a name="p1105mcpsimp"></a><a name="p1105mcpsimp"></a>layer_cnt[<a href="OT_IVE_ARR_NUM_EIGHT.md">OT_IVE_ARR_NUM_EIGHT</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p1109mcpsimp"><a name="p1109mcpsimp"></a><a name="p1109mcpsimp"></a>输入层1层-&gt;隐含层若干层（至少1层，最多6层）-&gt;输出层1层，分别存储输入、输出层的特征维度（取值范围：[1, 256]），各隐藏层的神经元数目（取值范围：[2,256]）。</p>
</td>
</tr>
<tr id="row1110mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p1112mcpsimp"><a name="p1112mcpsimp"></a><a name="p1112mcpsimp"></a>max_cnt</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p class="msonormal" id="p1114mcpsimp"><a name="p1114mcpsimp"></a><a name="p1114mcpsimp"></a>所有层中神经元数目或者特征维度的最大值：<a name="image108374142236"></a><a name="image108374142236"></a><span><img class="mathml" id="image108374142236" src="figures/zh-cn_formulaimage_0000002471091532.png"></span></p>
</td>
</tr>
<tr id="row1115mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p1117mcpsimp"><a name="p1117mcpsimp"></a><a name="p1117mcpsimp"></a>layer_num</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p1119mcpsimp"><a name="p1119mcpsimp"></a><a name="p1119mcpsimp"></a>ann_mlp系统中的层数。</p>
<p id="p1120mcpsimp"><a name="p1120mcpsimp"></a><a name="p1120mcpsimp"></a>取值范围：[3, 8]。</p>
</td>
</tr>
<tr id="row1121mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.1 "><p id="p1123mcpsimp"><a name="p1123mcpsimp"></a><a name="p1123mcpsimp"></a>reserved</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.2 "><p id="p1125mcpsimp"><a name="p1125mcpsimp"></a><a name="p1125mcpsimp"></a>保留字段</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_ive\_ann\_mlp\_actv\_func](#ot_ive_ann_mlp_actv_func)
-   [ot\_ive\_ann\_mlp\_accurate](#ot_ive_ann_mlp_accurate)

### ot\_ive\_svm\_type<a name="ZH-CN_TOPIC_0000002471091314"></a>

【说明】

定义svm类型。

【定义】

```
typedef enum {
    OT_IVE_SVM_TYPE_C_SVC = 0x0,
    OT_IVE_SVM_TYPE_NU_SVC = 0x1,
    OT_IVE_SVM_TYPE_BUTT
} ot_ive_svm_type;
```

【成员】

<a name="table16915mcpsimp"></a>
<table><thead align="left"><tr id="row16920mcpsimp"><th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.1"><p id="p16922mcpsimp"><a name="p16922mcpsimp"></a><a name="p16922mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.2"><p id="p16924mcpsimp"><a name="p16924mcpsimp"></a><a name="p16924mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16926mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p16928mcpsimp"><a name="p16928mcpsimp"></a><a name="p16928mcpsimp"></a>OT_IVE_SVM_TYPE_C_SVC</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p16930mcpsimp"><a name="p16930mcpsimp"></a><a name="p16930mcpsimp"></a>分类模式。</p>
</td>
</tr>
<tr id="row16931mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p16933mcpsimp"><a name="p16933mcpsimp"></a><a name="p16933mcpsimp"></a>OT_IVE_SVM_TYPE_NU_SVC</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p16935mcpsimp"><a name="p16935mcpsimp"></a><a name="p16935mcpsimp"></a>回归模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_svm\_kernel\_type<a name="ZH-CN_TOPIC_0000002471091268"></a>

【说明】

定义svm核函数类型。

【定义】

```
typedef enum {
    OT_IVE_SVM_KERNEL_TYPE_LINEAR = 0x0,
    OT_IVE_SVM_KERNEL_TYPE_POLY = 0x1,
    OT_IVE_SVM_KERNEL_TYPE_RBF = 0x2,
    OT_IVE_SVM_KERNEL_TYPE_SIGMOID = 0x3,
    OT_IVE_SVM_KERNEL_TYPE_BUTT
} ot_ive_svm_kernel_type;
```

【成员】

<a name="table4257mcpsimp"></a>
<table><thead align="left"><tr id="row4262mcpsimp"><th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.1"><p id="p4264mcpsimp"><a name="p4264mcpsimp"></a><a name="p4264mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.2"><p id="p4266mcpsimp"><a name="p4266mcpsimp"></a><a name="p4266mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4268mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p4270mcpsimp"><a name="p4270mcpsimp"></a><a name="p4270mcpsimp"></a>OT_IVE_SVM_KERNEL_TYPE_LINEAR</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p4272mcpsimp"><a name="p4272mcpsimp"></a><a name="p4272mcpsimp"></a>线性核函数。</p>
</td>
</tr>
<tr id="row4273mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p4275mcpsimp"><a name="p4275mcpsimp"></a><a name="p4275mcpsimp"></a>OT_IVE_SVM_KERNEL_TYPE_POLY</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p4277mcpsimp"><a name="p4277mcpsimp"></a><a name="p4277mcpsimp"></a>多项式核函数。</p>
</td>
</tr>
<tr id="row4278mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p4280mcpsimp"><a name="p4280mcpsimp"></a><a name="p4280mcpsimp"></a>OT_IVE_SVM_KERNEL_TYPE_RBF</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p4282mcpsimp"><a name="p4282mcpsimp"></a><a name="p4282mcpsimp"></a>径向基核函数。</p>
</td>
</tr>
<tr id="row4283mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p xml:lang="da-DK" id="p4285mcpsimp"><a name="p4285mcpsimp"></a><a name="p4285mcpsimp"></a>OT_IVE_SVM_KERNEL_TYPE_SIGMOID</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p4287mcpsimp"><a name="p4287mcpsimp"></a><a name="p4287mcpsimp"></a>Sigmoid核函数。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

对应核函数定义参见ss\_mpi\_ive\_svm\_predict中【注意】。

【相关数据类型及接口】

无。

### ot\_ive\_svm\_model<a name="ZH-CN_TOPIC_0000002470931338"></a>

【说明】

定义svm模型数据结构体。

【定义】

```
typedef struct {
    ot_ive_svm_type type;
    ot_ive_svm_kernel_type kernel_type;
    ot_svp_mem_info sv;   /* SV memory */
    ot_svp_mem_info df;   /* Decision functions memory */
    td_u32 total_df_size; /* All decision functions coef size in byte */
    td_u16 feature_dim;
    td_u16 sv_total;
    td_u8 class_cnt;
} ot_ive_svm_model;
```

【成员】

<a name="table4876mcpsimp"></a>
<table><thead align="left"><tr id="row4881mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p4883mcpsimp"><a name="p4883mcpsimp"></a><a name="p4883mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p4885mcpsimp"><a name="p4885mcpsimp"></a><a name="p4885mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4887mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4889mcpsimp"><a name="p4889mcpsimp"></a><a name="p4889mcpsimp"></a>type</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4891mcpsimp"><a name="p4891mcpsimp"></a><a name="p4891mcpsimp"></a>svm类型，当前仅支持分类IVE_SVM_TYPE_C_SVC模式。</p>
</td>
</tr>
<tr id="row4892mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4894mcpsimp"><a name="p4894mcpsimp"></a><a name="p4894mcpsimp"></a>kernel_type</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4896mcpsimp"><a name="p4896mcpsimp"></a><a name="p4896mcpsimp"></a>svm核函数类型，参考ot_ive_svm_kernel_type。</p>
</td>
</tr>
<tr id="row4898mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4900mcpsimp"><a name="p4900mcpsimp"></a><a name="p4900mcpsimp"></a>sv</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4902mcpsimp"><a name="p4902mcpsimp"></a><a name="p4902mcpsimp"></a>模型数据中的支持向量，其内存排布参看ss_mpi_ive_svm_predict的【注意】。</p>
</td>
</tr>
<tr id="row4904mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4906mcpsimp"><a name="p4906mcpsimp"></a><a name="p4906mcpsimp"></a>df</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4908mcpsimp"><a name="p4908mcpsimp"></a><a name="p4908mcpsimp"></a>模型数据中的判决函数参数部分。</p>
</td>
</tr>
<tr id="row4909mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4911mcpsimp"><a name="p4911mcpsimp"></a><a name="p4911mcpsimp"></a>total_df_size</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4913mcpsimp"><a name="p4913mcpsimp"></a><a name="p4913mcpsimp"></a>所有判决函数参数总字节数。</p>
</td>
</tr>
<tr id="row4914mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4916mcpsimp"><a name="p4916mcpsimp"></a><a name="p4916mcpsimp"></a>feature_dim</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4918mcpsimp"><a name="p4918mcpsimp"></a><a name="p4918mcpsimp"></a>输入特征维数。</p>
<p id="p4919mcpsimp"><a name="p4919mcpsimp"></a><a name="p4919mcpsimp"></a>取值范围：[1, 256]或[1, 1024]，跟ss_mpi_ive_cnn_predict 中的输入特征向量维度一致。</p>
</td>
</tr>
<tr id="row4921mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4923mcpsimp"><a name="p4923mcpsimp"></a><a name="p4923mcpsimp"></a>sv_total</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4925mcpsimp"><a name="p4925mcpsimp"></a><a name="p4925mcpsimp"></a>总支持向量个数。</p>
<p id="p4926mcpsimp"><a name="p4926mcpsimp"></a><a name="p4926mcpsimp"></a>取值范围：[1, 3000]。</p>
</td>
</tr>
<tr id="row4927mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4929mcpsimp"><a name="p4929mcpsimp"></a><a name="p4929mcpsimp"></a>class_cnt</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4931mcpsimp"><a name="p4931mcpsimp"></a><a name="p4931mcpsimp"></a>类别数目。</p>
<p id="p4932mcpsimp"><a name="p4932mcpsimp"></a><a name="p4932mcpsimp"></a>取值范围：[2, 80]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_ive\_svm\_type](#ot_ive_svm_type)
-   [ot\_ive\_svm\_kernel\_type](#ot_ive_svm_kernel_type)

### ot\_ive\_cnn\_actv\_func<a name="ZH-CN_TOPIC_0000002470931254"></a>

【说明】

定义CNN 激活函数枚举类型。

【定义】

```
typedef enum {
    OT_IVE_CNN_ACTV_FUNC_NONE = 0x0,    /* Do not taking a activation, equivalent f(x)=x */
    OT_IVE_CNN_ACTV_FUNC_RELU = 0x1,    /* f(x)=max(0, x) */
    OT_IVE_CNN_ACTV_FUNC_SIGMOID = 0x2, /* f(x)=1/(1+exp(-x)), not support */
    OT_IVE_CNN_ACTV_FUNC_BUTT
} ot_ive_cnn_actv_func;
```

【成员】

<a name="table159mcpsimp"></a>
<table><thead align="left"><tr id="row164mcpsimp"><th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.1"><p id="p166mcpsimp"><a name="p166mcpsimp"></a><a name="p166mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.2"><p id="p168mcpsimp"><a name="p168mcpsimp"></a><a name="p168mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row170mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p172mcpsimp"><a name="p172mcpsimp"></a><a name="p172mcpsimp"></a><span xml:lang="da-DK" id="ph173mcpsimp"><a name="ph173mcpsimp"></a><a name="ph173mcpsimp"></a>OT_IVE</span>_CNN_ACTV_FUNC_NONE</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p175mcpsimp"><a name="p175mcpsimp"></a><a name="p175mcpsimp"></a>不使用激活函数。</p>
</td>
</tr>
<tr id="row176mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p178mcpsimp"><a name="p178mcpsimp"></a><a name="p178mcpsimp"></a><span xml:lang="da-DK" id="ph179mcpsimp"><a name="ph179mcpsimp"></a><a name="ph179mcpsimp"></a>OT_IVE</span>_CNN_ACTV_FUNC_RELU</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p181mcpsimp"><a name="p181mcpsimp"></a><a name="p181mcpsimp"></a>使用ReLU激活函数。</p>
</td>
</tr>
<tr id="row182mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p184mcpsimp"><a name="p184mcpsimp"></a><a name="p184mcpsimp"></a><span xml:lang="da-DK" id="ph185mcpsimp"><a name="ph185mcpsimp"></a><a name="ph185mcpsimp"></a>OT_IVE</span>_CNN_ACTV_FUNC_SIGMOID</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p187mcpsimp"><a name="p187mcpsimp"></a><a name="p187mcpsimp"></a>使用Sigmoid激活函数，暂不支持。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_cnn\_pooling<a name="ZH-CN_TOPIC_0000002471091230"></a>

【说明】

定义CNN汇聚操作枚举类型。

【定义】

```
typedef enum {
    OT_IVE_CNN_POOLING_NONE = 0x0, /* Do not taking a pooling action */
    OT_IVE_CNN_POOLING_MAX = 0x1,  /* Using max value of every pooling area */
    OT_IVE_CNN_POOLING_AVG = 0x2,  /* Using average value of every pooling area */
 
    OT_IVE_CNN_POOLING_BUTT
} ot_ive_cnn_pooling;
```

【成员】

<a name="table10179mcpsimp"></a>
<table><thead align="left"><tr id="row10184mcpsimp"><th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.1"><p id="p10186mcpsimp"><a name="p10186mcpsimp"></a><a name="p10186mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.2"><p id="p10188mcpsimp"><a name="p10188mcpsimp"></a><a name="p10188mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10190mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p10192mcpsimp"><a name="p10192mcpsimp"></a><a name="p10192mcpsimp"></a><span xml:lang="da-DK" id="ph10193mcpsimp"><a name="ph10193mcpsimp"></a><a name="ph10193mcpsimp"></a>OT_IVE</span>_CNN_POOLING_NONE</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p10195mcpsimp"><a name="p10195mcpsimp"></a><a name="p10195mcpsimp"></a>不执行图像汇聚。</p>
</td>
</tr>
<tr id="row10196mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p10198mcpsimp"><a name="p10198mcpsimp"></a><a name="p10198mcpsimp"></a><span xml:lang="da-DK" id="ph10199mcpsimp"><a name="ph10199mcpsimp"></a><a name="ph10199mcpsimp"></a>OT_IVE</span>_CNN_POOLING_MAX</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p10201mcpsimp"><a name="p10201mcpsimp"></a><a name="p10201mcpsimp"></a>对汇聚范围的特征点求最大值。</p>
</td>
</tr>
<tr id="row10202mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p10204mcpsimp"><a name="p10204mcpsimp"></a><a name="p10204mcpsimp"></a><span xml:lang="da-DK" id="ph10205mcpsimp"><a name="ph10205mcpsimp"></a><a name="ph10205mcpsimp"></a>OT_IVE</span>_CNN_POOLING_AVG</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p10207mcpsimp"><a name="p10207mcpsimp"></a><a name="p10207mcpsimp"></a>对汇聚范围的特征点求平均值。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_cnn\_conv\_pooling<a name="ZH-CN_TOPIC_0000002471091306"></a>

【说明】

定义CNN单层Conv-ReLU-Pooling运算包参数结构体。

【定义】

```
typedef struct {
    ot_ive_cnn_actv_func actv_func;   /* Type of activation function */
    ot_ive_cnn_pooling pooling;         /* Mode of pooling method */
 
    td_u8 feature_map_num;  /* Number of feature maps */
    td_u8 kernel_size;  /* Kernel size, only support 3 currently */
    td_u8 conv_step;     /* Convolution step, only support 1 currently */
 
    td_u8 pool_size;     /* Pooling size, only support 2 currently */
    td_u8 pool_step;     /* Pooling step, only support 2 currently */
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_THREE];
} ot_ive_cnn_conv_pooling;
```

【成员】

<a name="table10501mcpsimp"></a>
<table><thead align="left"><tr id="row10506mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p10508mcpsimp"><a name="p10508mcpsimp"></a><a name="p10508mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p10510mcpsimp"><a name="p10510mcpsimp"></a><a name="p10510mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10512mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p10514mcpsimp"><a name="p10514mcpsimp"></a><a name="p10514mcpsimp"></a>actv_func</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p10516mcpsimp"><a name="p10516mcpsimp"></a><a name="p10516mcpsimp"></a>激活函数类型。</p>
</td>
</tr>
<tr id="row10517mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p10519mcpsimp"><a name="p10519mcpsimp"></a><a name="p10519mcpsimp"></a>pooling</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p10521mcpsimp"><a name="p10521mcpsimp"></a><a name="p10521mcpsimp"></a>汇聚操作类型。</p>
</td>
</tr>
<tr id="row10522mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p10524mcpsimp"><a name="p10524mcpsimp"></a><a name="p10524mcpsimp"></a>feature_map_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p10526mcpsimp"><a name="p10526mcpsimp"></a><a name="p10526mcpsimp"></a>卷积运算包中输出特征图的数目。支持范围：1~50。</p>
</td>
</tr>
<tr id="row10527mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p10529mcpsimp"><a name="p10529mcpsimp"></a><a name="p10529mcpsimp"></a>kernel_size</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p10531mcpsimp"><a name="p10531mcpsimp"></a><a name="p10531mcpsimp"></a>卷积运算包中卷积核的尺寸。仅支持3x3。</p>
</td>
</tr>
<tr id="row10532mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p10534mcpsimp"><a name="p10534mcpsimp"></a><a name="p10534mcpsimp"></a>conv_step</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p10536mcpsimp"><a name="p10536mcpsimp"></a><a name="p10536mcpsimp"></a>卷积运算包中卷积核移动的步长。仅支持1。</p>
</td>
</tr>
<tr id="row10537mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p10539mcpsimp"><a name="p10539mcpsimp"></a><a name="p10539mcpsimp"></a>pool_size</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p10541mcpsimp"><a name="p10541mcpsimp"></a><a name="p10541mcpsimp"></a>汇聚操作的窗口大小。仅支持2x2。</p>
</td>
</tr>
<tr id="row10542mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p10544mcpsimp"><a name="p10544mcpsimp"></a><a name="p10544mcpsimp"></a>pool_step</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p10546mcpsimp"><a name="p10546mcpsimp"></a><a name="p10546mcpsimp"></a>汇聚操作窗口移动的步长。仅支持2。</p>
</td>
</tr>
<tr id="row10547mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p10549mcpsimp"><a name="p10549mcpsimp"></a><a name="p10549mcpsimp"></a>reserved</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p10551mcpsimp"><a name="p10551mcpsimp"></a><a name="p10551mcpsimp"></a>保留</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_ive\_cnn\_actv\_func](#ot_ive_cnn_actv_func)
-   [ot\_ive\_cnn\_pooling](#ot_ive_cnn_pooling)

### ot\_ive\_cnn\_fc\_info<a name="ZH-CN_TOPIC_0000002470931296"></a>

【说明】

定义CNN全链接网络参数结构体。

【定义】

```
typedef struct {
    td_u16 layer_cnt[OT_IVE_ARR_NUM_EIGHT];    /* Neuron number of every fully connected layers */
    td_u16 max_cnt;         /* Max neuron number in all fully connected layers */
    td_u8 layer_num;        /* Number of fully connected layer */
    td_u8 reserved;
} ot_ive_cnn_fc_info;
```

【成员】

<a name="table14513mcpsimp"></a>
<table><thead align="left"><tr id="row14518mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p14520mcpsimp"><a name="p14520mcpsimp"></a><a name="p14520mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p14522mcpsimp"><a name="p14522mcpsimp"></a><a name="p14522mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14524mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p14526mcpsimp"><a name="p14526mcpsimp"></a><a name="p14526mcpsimp"></a>layer_cnt[<a href="OT_IVE_ARR_NUM_EIGHT.md">OT_IVE_ARR_NUM_EIGHT</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p14530mcpsimp"><a name="p14530mcpsimp"></a><a name="p14530mcpsimp"></a>全连接各层神经元节点数目。输入层（即Conv-ReLU-Pooling的输出）支持范围：[1, 1024]；中间隐藏层支持范围：[2, 256]；输出层支持范围：[1, 256]。</p>
</td>
</tr>
<tr id="row14531mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p14533mcpsimp"><a name="p14533mcpsimp"></a><a name="p14533mcpsimp"></a>max_cnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p14535mcpsimp"><a name="p14535mcpsimp"></a><a name="p14535mcpsimp"></a>全连接各层节点数目的最大值。</p>
</td>
</tr>
<tr id="row14536mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p14538mcpsimp"><a name="p14538mcpsimp"></a><a name="p14538mcpsimp"></a>layer_num</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p14540mcpsimp"><a name="p14540mcpsimp"></a><a name="p14540mcpsimp"></a>全连接层数。支持范围：[3, 8]。</p>
</td>
</tr>
<tr id="row14541mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p14543mcpsimp"><a name="p14543mcpsimp"></a><a name="p14543mcpsimp"></a>reserved</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p14545mcpsimp"><a name="p14545mcpsimp"></a><a name="p14545mcpsimp"></a>保留</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_cnn\_model<a name="ZH-CN_TOPIC_0000002504091131"></a>

【说明】

定义CNN模型参数结构体。

【定义】

```
typedef struct {
    ot_ive_cnn_conv_pooling conv_pool[OT_IVE_ARR_NUM_EIGHT];  /* Conv-ReLU-Pooling layers info */
    ot_ive_cnn_fc_info fc_info;  /* Fully connected layers info */
    ot_svp_mem_info conv_kernel_bias;      /* Conv-ReLU-Pooling layers' kernels and bias */
    td_u32 conv_kernel_bias_size;          /* Size of Conv-ReLU-Pooling layer' kernels and bias */
    ot_svp_mem_info fc_wgt_bias;          /* Fully Connection Layers' weights and bias */
    td_u32 fc_wgt_bias_size;              /* Size of fully connection layers weights and bias */
    td_u32 total_mem_size;                 /* Total memory size of all kernels, weights, bias */
    ot_svp_img_type type;                /* Image type used for the CNN model */
    td_u32 width;                       /* Image width used for the model */
    td_u32 height;                      /* Image height used for the model */
    td_u16 class_cnt;                    /* Number of classes */
    td_u8 conv_pool_layer_num;             /* Number of Conv-ReLU-Pooling layers */
    td_u8 reserved;
} ot_ive_cnn_model;
```

【成员】

<a name="table295mcpsimp"></a>
<table><thead align="left"><tr id="row300mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p302mcpsimp"><a name="p302mcpsimp"></a><a name="p302mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p304mcpsimp"><a name="p304mcpsimp"></a><a name="p304mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row306mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p308mcpsimp"><a name="p308mcpsimp"></a><a name="p308mcpsimp"></a>conv_pool[<a href="OT_IVE_ARR_NUM_EIGHT.md">OT_IVE_ARR_NUM_EIGHT</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p312mcpsimp"><a name="p312mcpsimp"></a><a name="p312mcpsimp"></a>各层卷积运算包参数配置。</p>
</td>
</tr>
<tr id="row313mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p315mcpsimp"><a name="p315mcpsimp"></a><a name="p315mcpsimp"></a>full_connect</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p317mcpsimp"><a name="p317mcpsimp"></a><a name="p317mcpsimp"></a>全连接运算包参数配置。</p>
</td>
</tr>
<tr id="row318mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p320mcpsimp"><a name="p320mcpsimp"></a><a name="p320mcpsimp"></a>conv_kernel_bias</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p322mcpsimp"><a name="p322mcpsimp"></a><a name="p322mcpsimp"></a>所有卷积运算包中卷积核系数和偏置系数。</p>
</td>
</tr>
<tr id="row323mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p325mcpsimp"><a name="p325mcpsimp"></a><a name="p325mcpsimp"></a>conv_kernel_bias_size</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p327mcpsimp"><a name="p327mcpsimp"></a><a name="p327mcpsimp"></a>所有卷积运算包中卷积核系数和偏置系数的字节数。</p>
</td>
</tr>
<tr id="row328mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p330mcpsimp"><a name="p330mcpsimp"></a><a name="p330mcpsimp"></a>fc_wgt_bias</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p332mcpsimp"><a name="p332mcpsimp"></a><a name="p332mcpsimp"></a>全连接运算包中权重系数和偏置系数。</p>
<p id="p333mcpsimp"><a name="p333mcpsimp"></a><a name="p333mcpsimp"></a>详细定义请见《SVPx.0 API参考》1.4小节。</p>
</td>
</tr>
<tr id="row334mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p336mcpsimp"><a name="p336mcpsimp"></a><a name="p336mcpsimp"></a>fc_wgt_bias_size</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p338mcpsimp"><a name="p338mcpsimp"></a><a name="p338mcpsimp"></a>全连接运算包中权重系数和偏置系数的字节数。</p>
</td>
</tr>
<tr id="row339mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p341mcpsimp"><a name="p341mcpsimp"></a><a name="p341mcpsimp"></a>total_mem_size</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p343mcpsimp"><a name="p343mcpsimp"></a><a name="p343mcpsimp"></a>CNN网络计算中所需要的包括卷积核、权重、偏置系数等的内存字节数。</p>
</td>
</tr>
<tr id="row344mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p346mcpsimp"><a name="p346mcpsimp"></a><a name="p346mcpsimp"></a>type</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p348mcpsimp"><a name="p348mcpsimp"></a><a name="p348mcpsimp"></a>输入CNN模型的图像类型。仅支持U8C1灰度图或者RGB_PLANAR彩色图。</p>
</td>
</tr>
<tr id="row349mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p351mcpsimp"><a name="p351mcpsimp"></a><a name="p351mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p353mcpsimp"><a name="p353mcpsimp"></a><a name="p353mcpsimp"></a>输入CNN模型的图像宽度。w支持范围：16~80。</p>
</td>
</tr>
<tr id="row354mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p356mcpsimp"><a name="p356mcpsimp"></a><a name="p356mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p358mcpsimp"><a name="p358mcpsimp"></a><a name="p358mcpsimp"></a>输入CNN模型的图像高度。h支持范围：16~(1280/w)。</p>
</td>
</tr>
<tr id="row359mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p361mcpsimp"><a name="p361mcpsimp"></a><a name="p361mcpsimp"></a>class_cnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p363mcpsimp"><a name="p363mcpsimp"></a><a name="p363mcpsimp"></a>CNN模型分类任务的类别数目（与全连接最后一层输出一致）。支持范围：1~256。</p>
</td>
</tr>
<tr id="row364mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p366mcpsimp"><a name="p366mcpsimp"></a><a name="p366mcpsimp"></a>conv_pool_layer_num</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p368mcpsimp"><a name="p368mcpsimp"></a><a name="p368mcpsimp"></a>CNN模型卷积运算包的数目。支持范围：1~8。</p>
</td>
</tr>
<tr id="row369mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p371mcpsimp"><a name="p371mcpsimp"></a><a name="p371mcpsimp"></a>reserved</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p373mcpsimp"><a name="p373mcpsimp"></a><a name="p373mcpsimp"></a>保留</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

CNN网络模型结构参考ss\_mpi\_ive\_cnn\_predict中的【注意】。

【相关数据类型及接口】

-   [ot\_ive\_cnn\_actv\_func](#ot_ive_cnn_actv_func)
-   [ot\_ive\_cnn\_pooling](#ot_ive_cnn_pooling)
-   [ot\_ive\_cnn\_conv\_pooling](#ot_ive_cnn_conv_pooling)
-   [ot\_ive\_cnn\_fc\_info](#ot_ive_cnn_fc_info)

### ot\_ive\_cnn\_ctrl<a name="ZH-CN_TOPIC_0000002471091238"></a>

【说明】

定义CNN预测任务的控制参数。

【定义】

```
typedef struct {
    ot_svp_mem_info  mem;    /* Assist memory */
    td_u32 num;             /* Input img number */
} ot_ive_cnn_ctrl;
```

【成员】

<a name="table7597mcpsimp"></a>
<table><thead align="left"><tr id="row7602mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p7604mcpsimp"><a name="p7604mcpsimp"></a><a name="p7604mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p7606mcpsimp"><a name="p7606mcpsimp"></a><a name="p7606mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7608mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p7610mcpsimp"><a name="p7610mcpsimp"></a><a name="p7610mcpsimp"></a>mem</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p7612mcpsimp"><a name="p7612mcpsimp"></a><a name="p7612mcpsimp"></a>CNN预测计算过程的辅助内存，所需内存大小参考ss_mpi_ive_cnn_predict中的【注意】。</p>
<p id="p7614mcpsimp"><a name="p7614mcpsimp"></a><a name="p7614mcpsimp"></a>详细定义请见《SVPx.0 API参考》</p>
</td>
</tr>
<tr id="row7615mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p7617mcpsimp"><a name="p7617mcpsimp"></a><a name="p7617mcpsimp"></a>num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p7619mcpsimp"><a name="p7619mcpsimp"></a><a name="p7619mcpsimp"></a>输入CNN模型的图像数目。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_cnn\_result<a name="ZH-CN_TOPIC_0000002503971157"></a>

【说明】

定义CNN单个样本预测结果结构体。

【定义】

```
typedef struct {
    td_s32 class_idx;       /* The most possible index of the classification */
    td_s32 conf;      /* The confidence of the classification */
} ot_ive_cnn_result;
```

【成员】

<a name="table6420mcpsimp"></a>
<table><thead align="left"><tr id="row6425mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p6427mcpsimp"><a name="p6427mcpsimp"></a><a name="p6427mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p6429mcpsimp"><a name="p6429mcpsimp"></a><a name="p6429mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6431mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p6433mcpsimp"><a name="p6433mcpsimp"></a><a name="p6433mcpsimp"></a>class_idx</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p6435mcpsimp"><a name="p6435mcpsimp"></a><a name="p6435mcpsimp"></a>CNN模型的预测类别索引。</p>
</td>
</tr>
<tr id="row6436mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p6438mcpsimp"><a name="p6438mcpsimp"></a><a name="p6438mcpsimp"></a>conf</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p6440mcpsimp"><a name="p6440mcpsimp"></a><a name="p6440mcpsimp"></a>CNN模型所预测类别的置信度。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_persp\_trans\_point\_pair<a name="ZH-CN_TOPIC_0000002470931326"></a>

【说明】

定义透视变换点对结构体。

【定义】

```
typedef struct {
    ot_svp_point_u14q2 src_point; /* Source point */
    ot_svp_point_u14q2 dst_point; /* Destination point */
} ot_ive_persp_trans_point_pair;
```

【成员】

<a name="table14855mcpsimp"></a>
<table><thead align="left"><tr id="row14860mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p14862mcpsimp"><a name="p14862mcpsimp"></a><a name="p14862mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p14864mcpsimp"><a name="p14864mcpsimp"></a><a name="p14864mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14866mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p14868mcpsimp"><a name="p14868mcpsimp"></a><a name="p14868mcpsimp"></a>src_point</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p xml:lang="fr-FR" id="p14870mcpsimp"><a name="p14870mcpsimp"></a><a name="p14870mcpsimp"></a>源图坐标<span xml:lang="en-US" id="ph14871mcpsimp"><a name="ph14871mcpsimp"></a><a name="ph14871mcpsimp"></a>。</span></p>
</td>
</tr>
<tr id="row14872mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p14874mcpsimp"><a name="p14874mcpsimp"></a><a name="p14874mcpsimp"></a>dst_point</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p14876mcpsimp"><a name="p14876mcpsimp"></a><a name="p14876mcpsimp"></a>目的坐标。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_persp\_trans\_alg\_mode<a name="ZH-CN_TOPIC_0000002471091242"></a>

【说明】

定义透视变换算法模式枚举。

【定义】

```
typedef enum {
    OT_IVE_PERSP_TRANS_ALG_MODE_NR_SIM = 0x0, /* Non-reflective similarity transform mode */
    OT_IVE_PERSP_TRANS_ALG_MODE_SIM = 0x1,    /* Reflective similarity transform mode */
    OT_IVE_PERSP_TRANS_ALG_MODE_AFFINE = 0x2, /* Affine transform mode */
    OT_IVE_PERSP_TRANS_ALG_MODE_BUTT
} ot_ive_persp_trans_alg_mode;
```

【成员】

<a name="table7849mcpsimp"></a>
<table><thead align="left"><tr id="row7854mcpsimp"><th class="cellrowborder" valign="top" width="61%" id="mcps1.1.3.1.1"><p id="p7856mcpsimp"><a name="p7856mcpsimp"></a><a name="p7856mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="39%" id="mcps1.1.3.1.2"><p id="p7858mcpsimp"><a name="p7858mcpsimp"></a><a name="p7858mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7860mcpsimp"><td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.1 "><p id="p7862mcpsimp"><a name="p7862mcpsimp"></a><a name="p7862mcpsimp"></a>OT_IVE_PERSP_TRANS_ALG_MODE_NR_SIM</p>
</td>
<td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.2 "><p id="p7864mcpsimp"><a name="p7864mcpsimp"></a><a name="p7864mcpsimp"></a>非反射相似变换。</p>
</td>
</tr>
<tr id="row7865mcpsimp"><td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.1 "><p id="p7867mcpsimp"><a name="p7867mcpsimp"></a><a name="p7867mcpsimp"></a>OT_IVE_PERSP_TRANS_ALG_MODE_SIM</p>
</td>
<td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.2 "><p id="p7869mcpsimp"><a name="p7869mcpsimp"></a><a name="p7869mcpsimp"></a>相似变换。</p>
</td>
</tr>
<tr id="row7870mcpsimp"><td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.1 "><p id="p7872mcpsimp"><a name="p7872mcpsimp"></a><a name="p7872mcpsimp"></a>OT_IVE_PERSP_TRANS_ALG_MODE_AFFINE</p>
</td>
<td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.2 "><p id="p7874mcpsimp"><a name="p7874mcpsimp"></a><a name="p7874mcpsimp"></a>仿射变换。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_persp\_trans\_csc\_mode<a name="ZH-CN_TOPIC_0000002470931228"></a>

【说明】

定义透视变换色彩空间转换模式。

【定义】

```
typedef enum {
    OT_IVE_PERSP_TRANS_CSC_MODE_NONE = 0x0,                /* No do csc */
    OT_IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT601_YUV_TO_RGB = 0x1, /* CSC: YUV2RGB, video transfer mode, RGB value range [16, 235] */
    OT_IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT709_YUV_TO_RGB = 0x2, /* CSC: YUV2RGB, video transfer mode, RGB value range [16, 235] */
    OT_IVE_PERSP_TRANS_CSC_MODE_PIC_BT601_YUV_TO_RGB = 0x3,   /* CSC: YUV2RGB, picture transfer mode, RGB value range [0, 255] */
    OT_IVE_PERSP_TRANS_CSC_MODE_PIC_BT709_YUV_TO_RGB = 0x4,   /* CSC: YUV2RGB, picture transfer mode, RGB value range [0, 255] */
    OT_IVE_PERSP_TRANS_CSC_MODE_BUTT
} ot_ive_persp_trans_csc_mode;
```

【成员】

<a name="table4499mcpsimp"></a>
<table><thead align="left"><tr id="row4504mcpsimp"><th class="cellrowborder" valign="top" width="59%" id="mcps1.1.3.1.1"><p id="p4506mcpsimp"><a name="p4506mcpsimp"></a><a name="p4506mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="41%" id="mcps1.1.3.1.2"><p id="p4508mcpsimp"><a name="p4508mcpsimp"></a><a name="p4508mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4510mcpsimp"><td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.1 "><p id="p4512mcpsimp"><a name="p4512mcpsimp"></a><a name="p4512mcpsimp"></a>OT_IVE_PERSP_TRANS_CSC_MODE_NONE</p>
</td>
<td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.2 "><p id="p4514mcpsimp"><a name="p4514mcpsimp"></a><a name="p4514mcpsimp"></a>不做色彩空间转换。</p>
</td>
</tr>
<tr id="row4515mcpsimp"><td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.1 "><p id="p4517mcpsimp"><a name="p4517mcpsimp"></a><a name="p4517mcpsimp"></a>OT_IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT601_YUV_TO_RGB</p>
</td>
<td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.2 "><p id="p4519mcpsimp"><a name="p4519mcpsimp"></a><a name="p4519mcpsimp"></a>BT601的YUV2RGB视频变换。</p>
</td>
</tr>
<tr id="row4520mcpsimp"><td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.1 "><p id="p4522mcpsimp"><a name="p4522mcpsimp"></a><a name="p4522mcpsimp"></a>OT_IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT709_YUV_TO_RGB</p>
</td>
<td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.2 "><p id="p4524mcpsimp"><a name="p4524mcpsimp"></a><a name="p4524mcpsimp"></a>BT709的YUV2RGB视频变换。</p>
</td>
</tr>
<tr id="row4525mcpsimp"><td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.1 "><p id="p4527mcpsimp"><a name="p4527mcpsimp"></a><a name="p4527mcpsimp"></a>OT_IVE_PERSP_TRANS_CSC_MODE_PIC_BT601_YUV_TO_RGB</p>
</td>
<td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.2 "><p id="p4529mcpsimp"><a name="p4529mcpsimp"></a><a name="p4529mcpsimp"></a>BT601的YUV2RGB图像变换。</p>
</td>
</tr>
<tr id="row4530mcpsimp"><td class="cellrowborder" valign="top" width="59%" headers="mcps1.1.3.1.1 "><p id="p4532mcpsimp"><a name="p4532mcpsimp"></a><a name="p4532mcpsimp"></a>OT_IVE_PERSP_TRANS_CSC_MODE_PIC_BT709_YUV_TO_RGB</p>
</td>
<td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.3.1.2 "><p id="p4534mcpsimp"><a name="p4534mcpsimp"></a><a name="p4534mcpsimp"></a>BT709的YUV2RGB图像变换。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   OT\_IVE\_PERSP\_TRANS\_CSC\_MODE\_VIDEO\_BT601\_YUV\_TO\_RGB和OT\_IVE\_PERSP\_TRANS\_CSC\_MODE\_VIDEO\_BT709\_YUV\_TO\_RGB模式，输出满足16≤R、G、B≤235。
-   OT\_IVE\_PERSP\_TRANS\_CSC\_MODE\_PIC\_BT601\_YUV\_TO\_RGB和OT\_IVE\_PERSP\_TRANS\_CSC\_MODE\_PIC\_BT709\_YUV\_TO\_RGB模式，输出满足0≤R、G、B≤255。

【相关数据类型及接口】

无。

### ot\_ive\_persp\_trans\_ctrl<a name="ZH-CN_TOPIC_0000002471091262"></a>

【说明】

定义透视变换控制参数。

【定义】

```
typedef struct {
    ot_ive_persp_trans_alg_mode alg_mode;   /* Alg mode */
    ot_ive_persp_trans_csc_mode csc_mode;   /* CSC mode */
    td_u16 roi_num;                         /* Roi number */
    td_u16 point_pair_num;                  /* Point pair number  */
} ot_ive_persp_trans_ctrl;
```

【成员】

<a name="table5798mcpsimp"></a>
<table><thead align="left"><tr id="row5803mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p5805mcpsimp"><a name="p5805mcpsimp"></a><a name="p5805mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p5807mcpsimp"><a name="p5807mcpsimp"></a><a name="p5807mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5809mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p5811mcpsimp"><a name="p5811mcpsimp"></a><a name="p5811mcpsimp"></a>alg_mode</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5813mcpsimp"><a name="p5813mcpsimp"></a><a name="p5813mcpsimp"></a>透视变换算法模式。</p>
</td>
</tr>
<tr id="row5814mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p5816mcpsimp"><a name="p5816mcpsimp"></a><a name="p5816mcpsimp"></a>csc_mode</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5818mcpsimp"><a name="p5818mcpsimp"></a><a name="p5818mcpsimp"></a>透视变换色彩空间转换模式。</p>
</td>
</tr>
<tr id="row5819mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p5821mcpsimp"><a name="p5821mcpsimp"></a><a name="p5821mcpsimp"></a>roi_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5823mcpsimp"><a name="p5823mcpsimp"></a><a name="p5823mcpsimp"></a>区域数目。取值范围：[1, 64]。</p>
</td>
</tr>
<tr id="row5824mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p5826mcpsimp"><a name="p5826mcpsimp"></a><a name="p5826mcpsimp"></a>point_pair_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5828mcpsimp"><a name="p5828mcpsimp"></a><a name="p5828mcpsimp"></a>点对数目。</p>
<a name="ul5829mcpsimp"></a><a name="ul5829mcpsimp"></a><ul id="ul5829mcpsimp"><li xml:lang="fr-FR">非反射相似变换<span xml:lang="en-US" id="ph5831mcpsimp"><a name="ph5831mcpsimp"></a><a name="ph5831mcpsimp"></a>取值范围</span>：[2, 68]；</li><li xml:lang="fr-FR">其他算法<span xml:lang="en-US" id="ph5833mcpsimp"><a name="ph5833mcpsimp"></a><a name="ph5833mcpsimp"></a>取值范围</span>：[3, 68]<span xml:lang="en-US" id="ph5834mcpsimp"><a name="ph5834mcpsimp"></a><a name="ph5834mcpsimp"></a>。</span></li></ul>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_kcf\_core\_id<a name="ZH-CN_TOPIC_0000002504091191"></a>

【说明】

定义KCF的核ID。

【定义】

定义1：

```
typedef enum {
    OT_IVE_KCF_CORE0 = 0x0,
    OT_IVE_KCF_CORE_BUTT
} ot_ive_kcf_core_id;
```

定义2：

```
typedef enum {
    OT_IVE_KCF_CORE0 = 0x0,
    OT_IVE_KCF_CORE1 = 0x1,
    OT_IVE_KCF_CORE_BUTT
} ot_ive_kcf_core_id;
```

【成员】

<a name="table6377mcpsimp"></a>
<table><thead align="left"><tr id="row6382mcpsimp"><th class="cellrowborder" valign="top" width="45%" id="mcps1.1.3.1.1"><p id="p6384mcpsimp"><a name="p6384mcpsimp"></a><a name="p6384mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.3.1.2"><p id="p6386mcpsimp"><a name="p6386mcpsimp"></a><a name="p6386mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6388mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p6390mcpsimp"><a name="p6390mcpsimp"></a><a name="p6390mcpsimp"></a>OT_IVE_KCF_CORE0</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p6392mcpsimp"><a name="p6392mcpsimp"></a><a name="p6392mcpsimp"></a>KCF的核零</p>
</td>
</tr>
<tr id="row6393mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p6395mcpsimp"><a name="p6395mcpsimp"></a><a name="p6395mcpsimp"></a>OT_IVE_KCF_CORE1</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p6397mcpsimp"><a name="p6397mcpsimp"></a><a name="p6397mcpsimp"></a>KCF的核一</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

不同解决方案Kcf核数不一样，请具体参考发布包中ot\_common\_ive.h头文件中ot\_ive\_kcf\_core\_id定义。

【相关数据类型及接口】

-   [ot\_ive\_kcf\_proc\_ctrl](#ot_ive_kcf_proc_ctrl)
-   [ot\_ive\_hog\_ctrl](#ot_ive_hog_ctrl)
-   ss\_mpi\_ive\_kcf\_proc
-   ss\_mpi\_ive\_hog

### ot\_ive\_roi\_info<a name="ZH-CN_TOPIC_0000002503971155"></a>

【说明】

定义目前区域信息参数。

【定义】

```
typedef struct {
    ot_svp_rect_s24q8 roi;
    td_u32 roi_id;
} ot_ive_roi_info;
```

【成员】

<a name="table15911mcpsimp"></a>
<table><thead align="left"><tr id="row15916mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p15918mcpsimp"><a name="p15918mcpsimp"></a><a name="p15918mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p15920mcpsimp"><a name="p15920mcpsimp"></a><a name="p15920mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row15922mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p15924mcpsimp"><a name="p15924mcpsimp"></a><a name="p15924mcpsimp"></a>roi</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p15926mcpsimp"><a name="p15926mcpsimp"></a><a name="p15926mcpsimp"></a>区域信息。</p>
</td>
</tr>
<tr id="row15927mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p15929mcpsimp"><a name="p15929mcpsimp"></a><a name="p15929mcpsimp"></a>roi_id</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p15931mcpsimp"><a name="p15931mcpsimp"></a><a name="p15931mcpsimp"></a>区域索引。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_kcf\_proc\_ctrl<a name="ZH-CN_TOPIC_0000002471091264"></a>

【说明】

定义跟踪处理控制参数。

【定义】

```
typedef struct {
    ot_ive_csc_mode csc_mode;   /* Only support:
    IVE_CSC_MODE_VIDEO_BT601_YUV_TO_RGB  CSC: YUV2RGB, video transfer mode,RGB value range [16, 235]
    IVE_CSC_MODE_VIDEO_BT709_YUV_TO_RGB  CSC: YUV2RGB, video transfer mode,RGB value range [16, 235]
    IVE_CSC_MODE_PIC_BT601_YUV_TO_RGB  CSC: YUV2RGB, picture transfer mode,RGB value range [0, 255]
    IVE_CSC_MODE_PIC_BT709_YUV_TO_RGB  CSC: YUV2RGB, picture transfer mode,RGB value range [0, 255] */
    ot_svp_mem_info tmp_buf;
    td_u1q15 interp_factor; /* Blend coefficient. [0, 32768] */
    td_u0q16 lamda;        /* The regularization coefficient. [0, 65535] */
    td_u4q12 norm_trunc_alfa;   /* The normalization thresh. [0, 4095] */
    td_u0q8 sigma;         /* The gaussian kernel bandwidth. [0, 255]  */
    td_u8 resp_threshold;
    ot_ive_kcf_core_id core_id;
} ot_ive_kcf_proc_ctrl;
```

【成员】

<a name="table2328mcpsimp"></a>
<table><thead align="left"><tr id="row2333mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p2335mcpsimp"><a name="p2335mcpsimp"></a><a name="p2335mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p2337mcpsimp"><a name="p2337mcpsimp"></a><a name="p2337mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2339mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2341mcpsimp"><a name="p2341mcpsimp"></a><a name="p2341mcpsimp"></a>csc_mode</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2343mcpsimp"><a name="p2343mcpsimp"></a><a name="p2343mcpsimp"></a>色度空间转换模式。取值范围：[0, 3]</p>
</td>
</tr>
<tr id="row2344mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2346mcpsimp"><a name="p2346mcpsimp"></a><a name="p2346mcpsimp"></a>tmp_buf</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2348mcpsimp"><a name="p2348mcpsimp"></a><a name="p2348mcpsimp"></a>辅助内存。</p>
</td>
</tr>
<tr id="row2349mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2351mcpsimp"><a name="p2351mcpsimp"></a><a name="p2351mcpsimp"></a>interp_factor</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2353mcpsimp"><a name="p2353mcpsimp"></a><a name="p2353mcpsimp"></a>The blend coefficient。取值范围：[0, 32768]</p>
</td>
</tr>
<tr id="row2354mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2356mcpsimp"><a name="p2356mcpsimp"></a><a name="p2356mcpsimp"></a>lamda</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2358mcpsimp"><a name="p2358mcpsimp"></a><a name="p2358mcpsimp"></a>The regularization coefficient. 取值范围：[0, 65535]</p>
</td>
</tr>
<tr id="row2359mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2361mcpsimp"><a name="p2361mcpsimp"></a><a name="p2361mcpsimp"></a>norm_trunc_alfa</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2363mcpsimp"><a name="p2363mcpsimp"></a><a name="p2363mcpsimp"></a>归一化阈值。取值范围：[0, 4095]</p>
</td>
</tr>
<tr id="row2364mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2366mcpsimp"><a name="p2366mcpsimp"></a><a name="p2366mcpsimp"></a>sigma</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2368mcpsimp"><a name="p2368mcpsimp"></a><a name="p2368mcpsimp"></a>高斯核带宽。取值范围：[0, 255]</p>
</td>
</tr>
<tr id="row2369mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2371mcpsimp"><a name="p2371mcpsimp"></a><a name="p2371mcpsimp"></a>resp_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2373mcpsimp"><a name="p2373mcpsimp"></a><a name="p2373mcpsimp"></a>响应阈值。取值范围：[0, 255]</p>
</td>
</tr>
<tr id="row2374mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2376mcpsimp"><a name="p2376mcpsimp"></a><a name="p2376mcpsimp"></a>core_id</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2378mcpsimp"><a name="p2378mcpsimp"></a><a name="p2378mcpsimp"></a>指定KCF的核ID</p>
</td>
</tr>
</tbody>
</table>

【解决方案差异】

<a name="table2380mcpsimp"></a>
<table><thead align="left"><tr id="row2385mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p2387mcpsimp"><a name="p2387mcpsimp"></a><a name="p2387mcpsimp"></a>解决方案名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p2389mcpsimp"><a name="p2389mcpsimp"></a><a name="p2389mcpsimp"></a>差异</p>
</th>
</tr>
</thead>
<tbody><tr id="row2411mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2413mcpsimp"><a name="p2413mcpsimp"></a><a name="p2413mcpsimp"></a>SS928V100</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2415mcpsimp"><a name="p2415mcpsimp"></a><a name="p2415mcpsimp"></a>KCF在IVE中只有一个核，默认用核0，配置其他参数无效</p>
</td>
</tr>
<tr id="row181113452368"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p1823012815334"><a name="p1823012815334"></a><a name="p1823012815334"></a>SS927V100</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p192301028123318"><a name="p192301028123318"></a><a name="p192301028123318"></a>KCF在IVE中只有一个核，默认用核0，配置其他参数无效</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_list\_head<a name="ZH-CN_TOPIC_0000002504091189"></a>

【说明】

定义链表头结构体参数。

【定义】

```
typedef struct tag_ot_ive_list_head {
    struct tag_ot_ive_list_head *next, *prev;
} ot_ive_list_head;
```

【成员】

<a name="table2593mcpsimp"></a>
<table><thead align="left"><tr id="row2598mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p2600mcpsimp"><a name="p2600mcpsimp"></a><a name="p2600mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p2602mcpsimp"><a name="p2602mcpsimp"></a><a name="p2602mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2604mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2606mcpsimp"><a name="p2606mcpsimp"></a><a name="p2606mcpsimp"></a>next</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2608mcpsimp"><a name="p2608mcpsimp"></a><a name="p2608mcpsimp"></a>指向下一个节点指针。</p>
</td>
</tr>
<tr id="row2609mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p2611mcpsimp"><a name="p2611mcpsimp"></a><a name="p2611mcpsimp"></a>prev</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p2613mcpsimp"><a name="p2613mcpsimp"></a><a name="p2613mcpsimp"></a>指向前一个节点指针。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_kcf\_obj<a name="ZH-CN_TOPIC_0000002471091292"></a>

【说明】

定义目标信息结构体参数。

【定义】

```
typedef struct {
    ot_ive_roi_info roi_info;
    ot_svp_mem_info cos_win_x;
    ot_svp_mem_info cos_win_y;
    ot_svp_mem_info gauss_peak;
    ot_svp_mem_info hog_feature;
    ot_svp_mem_info alpha;
    ot_svp_mem_info dst;
    td_u3q5 padding; /* [48, 160]  */
    td_u8 reserved[OT_IVE_ARR_RESERVED_NUM_THREE];
} ot_ive_kcf_obj;
```

【成员】

<a name="table10385mcpsimp"></a>
<table><thead align="left"><tr id="row10390mcpsimp"><th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.3.1.1"><p id="p10392mcpsimp"><a name="p10392mcpsimp"></a><a name="p10392mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="45%" id="mcps1.1.3.1.2"><p id="p10394mcpsimp"><a name="p10394mcpsimp"></a><a name="p10394mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10396mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p10398mcpsimp"><a name="p10398mcpsimp"></a><a name="p10398mcpsimp"></a>roi_info</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10400mcpsimp"><a name="p10400mcpsimp"></a><a name="p10400mcpsimp"></a>目标区域信息。</p>
</td>
</tr>
<tr id="row10401mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p10403mcpsimp"><a name="p10403mcpsimp"></a><a name="p10403mcpsimp"></a>cos_win_x</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10405mcpsimp"><a name="p10405mcpsimp"></a><a name="p10405mcpsimp"></a>水平方向汉宁窗。</p>
</td>
</tr>
<tr id="row10406mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p10408mcpsimp"><a name="p10408mcpsimp"></a><a name="p10408mcpsimp"></a>cos_win_y</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10410mcpsimp"><a name="p10410mcpsimp"></a><a name="p10410mcpsimp"></a>垂直方向汉宁窗。</p>
</td>
</tr>
<tr id="row10411mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p10413mcpsimp"><a name="p10413mcpsimp"></a><a name="p10413mcpsimp"></a>gauss_peak</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10415mcpsimp"><a name="p10415mcpsimp"></a><a name="p10415mcpsimp"></a>高斯峰值表。</p>
</td>
</tr>
<tr id="row10416mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p10418mcpsimp"><a name="p10418mcpsimp"></a><a name="p10418mcpsimp"></a>hog_feature</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10420mcpsimp"><a name="p10420mcpsimp"></a><a name="p10420mcpsimp"></a>HOG特征。</p>
</td>
</tr>
<tr id="row10421mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p10423mcpsimp"><a name="p10423mcpsimp"></a><a name="p10423mcpsimp"></a>alpha</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10425mcpsimp"><a name="p10425mcpsimp"></a><a name="p10425mcpsimp"></a>Alpha值。</p>
</td>
</tr>
<tr id="row10426mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p10428mcpsimp"><a name="p10428mcpsimp"></a><a name="p10428mcpsimp"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10430mcpsimp"><a name="p10430mcpsimp"></a><a name="p10430mcpsimp"></a>跟踪结果。</p>
</td>
</tr>
<tr id="row10431mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p10433mcpsimp"><a name="p10433mcpsimp"></a><a name="p10433mcpsimp"></a>padding</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10435mcpsimp"><a name="p10435mcpsimp"></a><a name="p10435mcpsimp"></a>目标区域放大倍数。</p>
</td>
</tr>
<tr id="row10436mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p xml:lang="de-DE" id="p10438mcpsimp"><a name="p10438mcpsimp"></a><a name="p10438mcpsimp"></a><span xml:lang="en-US" id="ph10439mcpsimp"><a name="ph10439mcpsimp"></a><a name="ph10439mcpsimp"></a>reserved</span> <span xml:lang="en-US" id="ph10440mcpsimp"><a name="ph10440mcpsimp"></a><a name="ph10440mcpsimp"></a>[</span><a href="OT_IVE_ARR_RESERVED_NUM_THREE.md">OT_IVE_ARR_RESERVED_NUM_THREE</a><span xml:lang="en-US" id="ph10443mcpsimp"><a name="ph10443mcpsimp"></a><a name="ph10443mcpsimp"></a>]</span></p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p10445mcpsimp"><a name="p10445mcpsimp"></a><a name="p10445mcpsimp"></a>保留字段。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_kcf\_obj\_node<a name="ZH-CN_TOPIC_0000002470931324"></a>

【说明】

定义目标链表节点参数。

【定义】

```
typedef struct {
    ot_ive_list_head list;
    ot_ive_kcf_obj kcf_obj;
} ot_ive_kcf_obj_node;
```

【成员】

<a name="table4834mcpsimp"></a>
<table><thead align="left"><tr id="row4839mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p4841mcpsimp"><a name="p4841mcpsimp"></a><a name="p4841mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p4843mcpsimp"><a name="p4843mcpsimp"></a><a name="p4843mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4845mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p4847mcpsimp"><a name="p4847mcpsimp"></a><a name="p4847mcpsimp"></a>list</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4849mcpsimp"><a name="p4849mcpsimp"></a><a name="p4849mcpsimp"></a>节点链表头信息。</p>
</td>
</tr>
<tr id="row4850mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p4852mcpsimp"><a name="p4852mcpsimp"></a><a name="p4852mcpsimp"></a>kcf_obj</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4854mcpsimp"><a name="p4854mcpsimp"></a><a name="p4854mcpsimp"></a>目标信息。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_kcf\_list\_state<a name="ZH-CN_TOPIC_0000002504091113"></a>

【说明】

定义目标链表状态枚举类型。

【定义】

```
typedef enum {
    OT_IVE_KCF_LIST_STATE_CREATE = 0x1,
    OT_IVE_KCF_LIST_STATE_DESTORY = 0x2,
    OT_IVE_KCF_LIST_STATE_BUTT
} ot_ive_kcf_list_state;
```

【成员】

<a name="table15023mcpsimp"></a>
<table><thead align="left"><tr id="row15028mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p15030mcpsimp"><a name="p15030mcpsimp"></a><a name="p15030mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p15032mcpsimp"><a name="p15032mcpsimp"></a><a name="p15032mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row15034mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p15036mcpsimp"><a name="p15036mcpsimp"></a><a name="p15036mcpsimp"></a>OT_IVE_KCF_LIST_STATE_CREATE</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p15038mcpsimp"><a name="p15038mcpsimp"></a><a name="p15038mcpsimp"></a>创建链表。</p>
</td>
</tr>
<tr id="row15039mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p15041mcpsimp"><a name="p15041mcpsimp"></a><a name="p15041mcpsimp"></a>OT_IVE_KCF_LIST_STATE_DESTORY</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p15043mcpsimp"><a name="p15043mcpsimp"></a><a name="p15043mcpsimp"></a>删除链表。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_kcf\_obj\_list<a name="ZH-CN_TOPIC_0000002470931236"></a>

【说明】

定义目标链表结构参数。

【定义】

```
typedef struct {
    ot_ive_kcf_obj_node *obj_node_buf;  /* The object list node address */
    ot_ive_list_head free_obj_list;     /* The free list of object list */
    ot_ive_list_head train_obj_list;    /* The training list of object list */
    ot_ive_list_head track_obj_list;    /* The tracking list of object list */
    td_u32 free_obj_num;                /* The numbers of free list */
    td_u32 train_obj_num;               /* The numbers of training list */
    td_u32 track_obj_num;               /* The numbers of tracking list */
    td_u32 max_obj_num;                 /* The maximum numbers of object list */
    ot_ive_kcf_list_state list_state;   /* The object list state */
    td_u8 *tmp_buf;                     /* Assist buffer */
    td_u32 width;                       /* image width */
    td_u32 height;                      /* image height */
} ot_ive_kcf_obj_list;
```

【成员】

<a name="table413mcpsimp"></a>
<table><thead align="left"><tr id="row418mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p420mcpsimp"><a name="p420mcpsimp"></a><a name="p420mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p422mcpsimp"><a name="p422mcpsimp"></a><a name="p422mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row424mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p426mcpsimp"><a name="p426mcpsimp"></a><a name="p426mcpsimp"></a>obj_node_buf</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p428mcpsimp"><a name="p428mcpsimp"></a><a name="p428mcpsimp"></a>目标节点地址。</p>
</td>
</tr>
<tr id="row429mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p431mcpsimp"><a name="p431mcpsimp"></a><a name="p431mcpsimp"></a>free_obj_list</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p433mcpsimp"><a name="p433mcpsimp"></a><a name="p433mcpsimp"></a>空闲节点链表。</p>
</td>
</tr>
<tr id="row434mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p436mcpsimp"><a name="p436mcpsimp"></a><a name="p436mcpsimp"></a>train_obj_list</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p438mcpsimp"><a name="p438mcpsimp"></a><a name="p438mcpsimp"></a>训练节点链表。</p>
</td>
</tr>
<tr id="row439mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p441mcpsimp"><a name="p441mcpsimp"></a><a name="p441mcpsimp"></a>track_obj_list</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p443mcpsimp"><a name="p443mcpsimp"></a><a name="p443mcpsimp"></a>跟踪节点链表。</p>
</td>
</tr>
<tr id="row444mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p446mcpsimp"><a name="p446mcpsimp"></a><a name="p446mcpsimp"></a>free_obj_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p448mcpsimp"><a name="p448mcpsimp"></a><a name="p448mcpsimp"></a>空闲节点链表节点数。</p>
</td>
</tr>
<tr id="row449mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p451mcpsimp"><a name="p451mcpsimp"></a><a name="p451mcpsimp"></a>train_obj_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p453mcpsimp"><a name="p453mcpsimp"></a><a name="p453mcpsimp"></a>训练节点链表节点数。</p>
</td>
</tr>
<tr id="row454mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p456mcpsimp"><a name="p456mcpsimp"></a><a name="p456mcpsimp"></a>track_obj_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p458mcpsimp"><a name="p458mcpsimp"></a><a name="p458mcpsimp"></a>跟踪节点链表节点数。</p>
</td>
</tr>
<tr id="row459mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p461mcpsimp"><a name="p461mcpsimp"></a><a name="p461mcpsimp"></a>max_obj_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p463mcpsimp"><a name="p463mcpsimp"></a><a name="p463mcpsimp"></a>目标节点链表最大节点数。</p>
</td>
</tr>
<tr id="row464mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p466mcpsimp"><a name="p466mcpsimp"></a><a name="p466mcpsimp"></a>list_state</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p468mcpsimp"><a name="p468mcpsimp"></a><a name="p468mcpsimp"></a>目标节点链表状态。</p>
</td>
</tr>
<tr id="row469mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p471mcpsimp"><a name="p471mcpsimp"></a><a name="p471mcpsimp"></a>tmp_buf</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p473mcpsimp"><a name="p473mcpsimp"></a><a name="p473mcpsimp"></a>链表辅助内存。</p>
</td>
</tr>
<tr id="row474mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p476mcpsimp"><a name="p476mcpsimp"></a><a name="p476mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p478mcpsimp"><a name="p478mcpsimp"></a><a name="p478mcpsimp"></a>跟踪图像的宽。</p>
</td>
</tr>
<tr id="row479mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p481mcpsimp"><a name="p481mcpsimp"></a><a name="p481mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p483mcpsimp"><a name="p483mcpsimp"></a><a name="p483mcpsimp"></a>跟踪图像的高。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_kcf\_bbox<a name="ZH-CN_TOPIC_0000002504091101"></a>

【说明】

定义目标区域信息参数。

【定义】

```
typedef struct {
    ot_ive_kcf_obj_node *node;
    td_s32 response; /* Bbox Response value. */
    ot_ive_roi_info roi_info;
    td_bool is_track_ok;
    td_bool is_roi_refresh;
} ot_ive_kcf_bbox;
```

【成员】

<a name="table16685mcpsimp"></a>
<table><thead align="left"><tr id="row16690mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p16692mcpsimp"><a name="p16692mcpsimp"></a><a name="p16692mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p16694mcpsimp"><a name="p16694mcpsimp"></a><a name="p16694mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16696mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p16698mcpsimp"><a name="p16698mcpsimp"></a><a name="p16698mcpsimp"></a>node</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p16700mcpsimp"><a name="p16700mcpsimp"></a><a name="p16700mcpsimp"></a>节点信息指针。</p>
</td>
</tr>
<tr id="row16701mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p16703mcpsimp"><a name="p16703mcpsimp"></a><a name="p16703mcpsimp"></a>response</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p16705mcpsimp"><a name="p16705mcpsimp"></a><a name="p16705mcpsimp"></a>目标响应值。</p>
</td>
</tr>
<tr id="row16706mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p16708mcpsimp"><a name="p16708mcpsimp"></a><a name="p16708mcpsimp"></a>roi_info</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p16710mcpsimp"><a name="p16710mcpsimp"></a><a name="p16710mcpsimp"></a>目标区域信息。</p>
</td>
</tr>
<tr id="row16711mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p16713mcpsimp"><a name="p16713mcpsimp"></a><a name="p16713mcpsimp"></a>is_track_ok</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p16715mcpsimp"><a name="p16715mcpsimp"></a><a name="p16715mcpsimp"></a>跟踪状态标志。</p>
</td>
</tr>
<tr id="row16716mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p16718mcpsimp"><a name="p16718mcpsimp"></a><a name="p16718mcpsimp"></a>is_roi_refresh</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p16720mcpsimp"><a name="p16720mcpsimp"></a><a name="p16720mcpsimp"></a>目标区域刷新标志。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_kcf\_bbox\_ctrl<a name="ZH-CN_TOPIC_0000002470931278"></a>

【说明】

定义目标区域信息控制参数。

【定义】

```
typedef struct {
    td_u32 max_bbox_num;    /* The member numbers of Bbox Array. */
    td_s32 response_threshold;        /* Select Bbox when Bbox'Response value is greater than or equal to RespThr. */
} ot_ive_kcf_bbox_ctrl;
```

【成员】

<a name="table4303mcpsimp"></a>
<table><thead align="left"><tr id="row4308mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p4310mcpsimp"><a name="p4310mcpsimp"></a><a name="p4310mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p4312mcpsimp"><a name="p4312mcpsimp"></a><a name="p4312mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4314mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4316mcpsimp"><a name="p4316mcpsimp"></a><a name="p4316mcpsimp"></a>max_bbox_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4318mcpsimp"><a name="p4318mcpsimp"></a><a name="p4318mcpsimp"></a>目标区域信息数组最大成员数。</p>
</td>
</tr>
<tr id="row4319mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p4321mcpsimp"><a name="p4321mcpsimp"></a><a name="p4321mcpsimp"></a>resp_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p4323mcpsimp"><a name="p4323mcpsimp"></a><a name="p4323mcpsimp"></a>目标响应阈值。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_ive\_hog\_mode<a name="ZH-CN_TOPIC_0000002504091097"></a>

【说明】

定义HOG\(Histogram of Oriented Gradient\)特征存储模式枚举类型。

【定义】

```
typedef enum {
    OT_IVE_HOG_MODE_VER_TANGENT_PLANE = 0x1,
    OT_IVE_HOG_MODE_HOR_TANGENT_PLANE = 0x2,
    OT_IVE_HOG_MODE_BUTT
} ot_ive_hog_mode;
```

【成员】

<a name="table6598mcpsimp"></a>
<table><thead align="left"><tr id="row6603mcpsimp"><th class="cellrowborder" valign="top" width="61%" id="mcps1.1.3.1.1"><p id="p6605mcpsimp"><a name="p6605mcpsimp"></a><a name="p6605mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="39%" id="mcps1.1.3.1.2"><p id="p6607mcpsimp"><a name="p6607mcpsimp"></a><a name="p6607mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6609mcpsimp"><td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.1 "><p id="p6611mcpsimp"><a name="p6611mcpsimp"></a><a name="p6611mcpsimp"></a>OT_IVE_HOG_MODE_VER_TANGENT_PLANE</p>
</td>
<td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.2 "><p id="p6613mcpsimp"><a name="p6613mcpsimp"></a><a name="p6613mcpsimp"></a>垂直切面模式。</p>
</td>
</tr>
<tr id="row6614mcpsimp"><td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.3.1.1 "><p id="p6616mcpsimp"><a name="p6616mcpsimp"></a><a name="p6616mcpsimp"></a>OT_IVE_HOG_MODE_HOR_TANGENT_PLANE</p>
</td>
<td class="cellrowborder" valign="top" width="39%" headers="mcps1.1.3.1.2 "><p id="p6618mcpsimp"><a name="p6618mcpsimp"></a><a name="p6618mcpsimp"></a>水平切面模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

ss\_mpi\_ive\_hog

### ot\_ive\_hog\_ctrl<a name="ZH-CN_TOPIC_0000002470931248"></a>

【说明】

定义计算HOG\(Histogram of Oriented Gradient\)特征控制参数。

【定义】

```
typedef struct {
    ot_ive_csc_mode csc_mode;   /* Only support:
                                IVE_CSC_MODE_VIDEO_BT601_YUV_TO_RGB  CSC: YUV2RGB, video transfer mode,
                                RGB value range [16, 235]
                                IVE_CSC_MODE_VIDEO_BT709_YUV_TO_RGB  CSC: YUV2RGB, video transfer mode,
                                RGB value range [16, 235]
                                IVE_CSC_MODE_PIC_BT601_YUV_TORGB  CSC: YUV2RGB, picture transfer mode,
                                RGB value range [0, 255]
                                IVE_CSC_MODE_PIC_BT709_YUV_TO_RGB  CSC: YUV2RGB, picture transfer mode,
                                RGB value range [0, 255] */
    ot_ive_hog_mode hog_mode;   /* Hog mode */
    td_u32 roi_num;             /* Roi number. [1, 64] */
    td_u4q12 norm_trunc_alfa;    /* The normalization thresh. [0, 4095] */
    ot_ive_kcf_core_id core_id;
   } ot_ive_hog_ctrl;
```

【成员】

<a name="table5865mcpsimp"></a>
<table><thead align="left"><tr id="row5870mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p5872mcpsimp"><a name="p5872mcpsimp"></a><a name="p5872mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p5874mcpsimp"><a name="p5874mcpsimp"></a><a name="p5874mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5876mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p5878mcpsimp"><a name="p5878mcpsimp"></a><a name="p5878mcpsimp"></a>csc_mode</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5880mcpsimp"><a name="p5880mcpsimp"></a><a name="p5880mcpsimp"></a>色度空间转换模式。取值范围：[0, 3]</p>
</td>
</tr>
<tr id="row5881mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p5883mcpsimp"><a name="p5883mcpsimp"></a><a name="p5883mcpsimp"></a>hog_mode</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5885mcpsimp"><a name="p5885mcpsimp"></a><a name="p5885mcpsimp"></a>HOG特征模式。</p>
</td>
</tr>
<tr id="row5886mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p5888mcpsimp"><a name="p5888mcpsimp"></a><a name="p5888mcpsimp"></a>roi_num</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5890mcpsimp"><a name="p5890mcpsimp"></a><a name="p5890mcpsimp"></a>区域数目。取值范围：[1, 64]</p>
</td>
</tr>
<tr id="row5891mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p5893mcpsimp"><a name="p5893mcpsimp"></a><a name="p5893mcpsimp"></a>norm_trunc_alfa</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5895mcpsimp"><a name="p5895mcpsimp"></a><a name="p5895mcpsimp"></a>归一化截断阈值。取值范围：[0, 4095]</p>
</td>
</tr>
<tr id="row5896mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p5898mcpsimp"><a name="p5898mcpsimp"></a><a name="p5898mcpsimp"></a>core_id</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5900mcpsimp"><a name="p5900mcpsimp"></a><a name="p5900mcpsimp"></a>指定KCF的核ID。</p>
</td>
</tr>
</tbody>
</table>

【解决方案差异】

<a name="table5902mcpsimp"></a>
<table><thead align="left"><tr id="row5907mcpsimp"><th class="cellrowborder" valign="top" width="25%" id="mcps1.1.3.1.1"><p id="p5909mcpsimp"><a name="p5909mcpsimp"></a><a name="p5909mcpsimp"></a>解决方案名称</p>
</th>
<th class="cellrowborder" valign="top" width="75%" id="mcps1.1.3.1.2"><p id="p5911mcpsimp"><a name="p5911mcpsimp"></a><a name="p5911mcpsimp"></a>差异</p>
</th>
</tr>
</thead>
<tbody><tr id="row5933mcpsimp"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p5935mcpsimp"><a name="p5935mcpsimp"></a><a name="p5935mcpsimp"></a>SS928V100</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p5937mcpsimp"><a name="p5937mcpsimp"></a><a name="p5937mcpsimp"></a>KCF在IVE中只有一个核，默认用核0，配置其他参数无效</p>
</td>
</tr>
<tr id="row9671651133611"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.3.1.1 "><p id="p1823012815334"><a name="p1823012815334"></a><a name="p1823012815334"></a>SS927V100</p>
</td>
<td class="cellrowborder" valign="top" width="75%" headers="mcps1.1.3.1.2 "><p id="p192301028123318"><a name="p192301028123318"></a><a name="p192301028123318"></a>KCF在IVE中只有一个核，默认用核0，配置其他参数无效</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

# 错误码<a name="ZH-CN_TOPIC_0000002471091270"></a>


## IVE错误码<a name="ZH-CN_TOPIC_0000002503971191"></a>

_识别_加速引擎API错误码如下所示。

**表 1** _识别_加速引擎API错误码

<a name="_Ref248310770"></a>
<table><thead align="left"><tr id="row16952mcpsimp"><th class="cellrowborder" valign="top" width="18.81188118811881%" id="mcps1.2.4.1.1"><p id="p16954mcpsimp"><a name="p16954mcpsimp"></a><a name="p16954mcpsimp"></a>错误代码</p>
</th>
<th class="cellrowborder" valign="top" width="43.56435643564357%" id="mcps1.2.4.1.2"><p id="p16956mcpsimp"><a name="p16956mcpsimp"></a><a name="p16956mcpsimp"></a>宏定义</p>
</th>
<th class="cellrowborder" valign="top" width="37.62376237623762%" id="mcps1.2.4.1.3"><p id="p16958mcpsimp"><a name="p16958mcpsimp"></a><a name="p16958mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row16960mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p16962mcpsimp"><a name="p16962mcpsimp"></a><a name="p16962mcpsimp"></a>0xa01d8001</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p16964mcpsimp"><a name="p16964mcpsimp"></a><a name="p16964mcpsimp"></a>OT_ERR_IVE_INVALID_DEV_ID</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p16966mcpsimp"><a name="p16966mcpsimp"></a><a name="p16966mcpsimp"></a>设备<span xml:lang="fr-FR" id="ph16967mcpsimp"><a name="ph16967mcpsimp"></a><a name="ph16967mcpsimp"></a>ID</span>超出合法范围</p>
</td>
</tr>
<tr id="row16968mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p16970mcpsimp"><a name="p16970mcpsimp"></a><a name="p16970mcpsimp"></a>0xa01d8003</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p16972mcpsimp"><a name="p16972mcpsimp"></a><a name="p16972mcpsimp"></a>OT_ERR_IVE_INVALID_CHN_ID</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p xml:lang="fr-FR" id="p16974mcpsimp"><a name="p16974mcpsimp"></a><a name="p16974mcpsimp"></a><span xml:lang="en-US" id="ph16975mcpsimp"><a name="ph16975mcpsimp"></a><a name="ph16975mcpsimp"></a>通道</span>组号错误或无效区域句柄</p>
</td>
</tr>
<tr id="row16976mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p16978mcpsimp"><a name="p16978mcpsimp"></a><a name="p16978mcpsimp"></a>0xa01d8007</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p xml:lang="it-IT" id="p16980mcpsimp"><a name="p16980mcpsimp"></a><a name="p16980mcpsimp"></a>OT_ERR_IVE_ILLEGAL_PARAM</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p16982mcpsimp"><a name="p16982mcpsimp"></a><a name="p16982mcpsimp"></a>参数超出合法范围</p>
</td>
</tr>
<tr id="row16983mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p16985mcpsimp"><a name="p16985mcpsimp"></a><a name="p16985mcpsimp"></a>0xa01d8008</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p xml:lang="it-IT" id="p16987mcpsimp"><a name="p16987mcpsimp"></a><a name="p16987mcpsimp"></a>OT_ERR_IVE_EXIST</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p16989mcpsimp"><a name="p16989mcpsimp"></a><a name="p16989mcpsimp"></a>重复创建已存在的设备、通道或资源</p>
</td>
</tr>
<tr id="row16990mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p16992mcpsimp"><a name="p16992mcpsimp"></a><a name="p16992mcpsimp"></a>0xa01d8009</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p16994mcpsimp"><a name="p16994mcpsimp"></a><a name="p16994mcpsimp"></a>OT_ERR_IVE_UNEXIST</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p16996mcpsimp"><a name="p16996mcpsimp"></a><a name="p16996mcpsimp"></a>试图使用或者销毁不存在的设备、通道或者资源</p>
</td>
</tr>
<tr id="row16997mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p16999mcpsimp"><a name="p16999mcpsimp"></a><a name="p16999mcpsimp"></a>0xa01d800a</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p xml:lang="de-DE" id="p17001mcpsimp"><a name="p17001mcpsimp"></a><a name="p17001mcpsimp"></a>OT_ERR_IVE_NULL_PTR</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17003mcpsimp"><a name="p17003mcpsimp"></a><a name="p17003mcpsimp"></a>函数参数中有空指针</p>
</td>
</tr>
<tr id="row17004mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17006mcpsimp"><a name="p17006mcpsimp"></a><a name="p17006mcpsimp"></a>0xa01d800b</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17008mcpsimp"><a name="p17008mcpsimp"></a><a name="p17008mcpsimp"></a>OT_ERR_IVE_NOT_CFG</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17010mcpsimp"><a name="p17010mcpsimp"></a><a name="p17010mcpsimp"></a>模块没有配置</p>
</td>
</tr>
<tr id="row17011mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17013mcpsimp"><a name="p17013mcpsimp"></a><a name="p17013mcpsimp"></a>0xa01d800c</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17015mcpsimp"><a name="p17015mcpsimp"></a><a name="p17015mcpsimp"></a>OT_ERR_IVE_NOT_SUPPORT</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17017mcpsimp"><a name="p17017mcpsimp"></a><a name="p17017mcpsimp"></a>不支持的参数或者功能</p>
</td>
</tr>
<tr id="row17018mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17020mcpsimp"><a name="p17020mcpsimp"></a><a name="p17020mcpsimp"></a>0xa01d800d</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17022mcpsimp"><a name="p17022mcpsimp"></a><a name="p17022mcpsimp"></a>OT_ERR_IVE_NOT_PERM</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17024mcpsimp"><a name="p17024mcpsimp"></a><a name="p17024mcpsimp"></a>该操作不允许<span xml:lang="fr-FR" id="ph17025mcpsimp"><a name="ph17025mcpsimp"></a><a name="ph17025mcpsimp"></a>，</span>如试图修改静态配置参数</p>
</td>
</tr>
<tr id="row17026mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17028mcpsimp"><a name="p17028mcpsimp"></a><a name="p17028mcpsimp"></a>0xa01d8014</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17030mcpsimp"><a name="p17030mcpsimp"></a><a name="p17030mcpsimp"></a>OT_ERR_IVE_NO_MEM</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17032mcpsimp"><a name="p17032mcpsimp"></a><a name="p17032mcpsimp"></a>分配内存失败，如系统内存不足</p>
</td>
</tr>
<tr id="row17033mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17035mcpsimp"><a name="p17035mcpsimp"></a><a name="p17035mcpsimp"></a>0xa01d8015</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17037mcpsimp"><a name="p17037mcpsimp"></a><a name="p17037mcpsimp"></a>OT_ERR_IVE_NO_BUF</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17039mcpsimp"><a name="p17039mcpsimp"></a><a name="p17039mcpsimp"></a>分配缓存失败，如申请的图像缓冲区太大</p>
</td>
</tr>
<tr id="row17040mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17042mcpsimp"><a name="p17042mcpsimp"></a><a name="p17042mcpsimp"></a>0xa01d8016</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p xml:lang="de-DE" id="p17044mcpsimp"><a name="p17044mcpsimp"></a><a name="p17044mcpsimp"></a>OT_ERR_IVE_BUF_EMPTY</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17046mcpsimp"><a name="p17046mcpsimp"></a><a name="p17046mcpsimp"></a>缓冲区中无图像</p>
</td>
</tr>
<tr id="row17047mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17049mcpsimp"><a name="p17049mcpsimp"></a><a name="p17049mcpsimp"></a>0xa01d8017</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17051mcpsimp"><a name="p17051mcpsimp"></a><a name="p17051mcpsimp"></a>OT_ERR_IVE_BUF_FULL</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17053mcpsimp"><a name="p17053mcpsimp"></a><a name="p17053mcpsimp"></a>缓冲区中图像满</p>
</td>
</tr>
<tr id="row17054mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17056mcpsimp"><a name="p17056mcpsimp"></a><a name="p17056mcpsimp"></a>0xa01d8018</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17058mcpsimp"><a name="p17058mcpsimp"></a><a name="p17058mcpsimp"></a>OT_ERR_IVE_NOT_READY</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17060mcpsimp"><a name="p17060mcpsimp"></a><a name="p17060mcpsimp"></a>系统没有初始化或没有加载相应模块</p>
</td>
</tr>
<tr id="row17061mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17063mcpsimp"><a name="p17063mcpsimp"></a><a name="p17063mcpsimp"></a>0xa01d8021</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17065mcpsimp"><a name="p17065mcpsimp"></a><a name="p17065mcpsimp"></a>OT_ERR_IVE_BAD_ADDR</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17067mcpsimp"><a name="p17067mcpsimp"></a><a name="p17067mcpsimp"></a>地址非法</p>
</td>
</tr>
<tr id="row17068mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17070mcpsimp"><a name="p17070mcpsimp"></a><a name="p17070mcpsimp"></a>0xa01d8022</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17072mcpsimp"><a name="p17072mcpsimp"></a><a name="p17072mcpsimp"></a>OT_ERR_IVE_BUSY</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17074mcpsimp"><a name="p17074mcpsimp"></a><a name="p17074mcpsimp"></a>系统忙</p>
</td>
</tr>
<tr id="row17075mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17077mcpsimp"><a name="p17077mcpsimp"></a><a name="p17077mcpsimp"></a>0xa01d8040</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17079mcpsimp"><a name="p17079mcpsimp"></a><a name="p17079mcpsimp"></a>OT_ERR_IVE_SYS_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17081mcpsimp"><a name="p17081mcpsimp"></a><a name="p17081mcpsimp"></a>系统超时</p>
</td>
</tr>
<tr id="row17082mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17084mcpsimp"><a name="p17084mcpsimp"></a><a name="p17084mcpsimp"></a>0xa01d8041</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17086mcpsimp"><a name="p17086mcpsimp"></a><a name="p17086mcpsimp"></a>OT_ERR_IVE_QUERY_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17088mcpsimp"><a name="p17088mcpsimp"></a><a name="p17088mcpsimp"></a>Query查询超时</p>
</td>
</tr>
<tr id="row17089mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17091mcpsimp"><a name="p17091mcpsimp"></a><a name="p17091mcpsimp"></a>0xa01d8042</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17093mcpsimp"><a name="p17093mcpsimp"></a><a name="p17093mcpsimp"></a>OT_ERR_IVE_BUS_ERR</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17095mcpsimp"><a name="p17095mcpsimp"></a><a name="p17095mcpsimp"></a>总线错误</p>
</td>
</tr>
<tr id="row17096mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17098mcpsimp"><a name="p17098mcpsimp"></a><a name="p17098mcpsimp"></a>0xa01d8043</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17100mcpsimp"><a name="p17100mcpsimp"></a><a name="p17100mcpsimp"></a>OT_ERR_IVE_OPEN_FILE</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17102mcpsimp"><a name="p17102mcpsimp"></a><a name="p17102mcpsimp"></a>打开文件失败</p>
</td>
</tr>
<tr id="row17103mcpsimp"><td class="cellrowborder" valign="top" width="18.81188118811881%" headers="mcps1.2.4.1.1 "><p id="p17105mcpsimp"><a name="p17105mcpsimp"></a><a name="p17105mcpsimp"></a>0xa01d8044</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.2.4.1.2 "><p id="p17107mcpsimp"><a name="p17107mcpsimp"></a><a name="p17107mcpsimp"></a>OT_ERR_IVE_READ_FILE</p>
</td>
<td class="cellrowborder" valign="top" width="37.62376237623762%" headers="mcps1.2.4.1.3 "><p id="p17109mcpsimp"><a name="p17109mcpsimp"></a><a name="p17109mcpsimp"></a>读文件失败</p>
</td>
</tr>
</tbody>
</table>

# Proc调试信息<a name="ZH-CN_TOPIC_0000002470931256"></a>



## 概述<a name="ZH-CN_TOPIC_0000002504091199"></a>

调试信息采用了Linux下的proc文件系统，可实时反映当前系统的运行状态，所记录的信息可供问题定位及分析时使用。

【文件目录】

/proc/umap

【信息查看方法】

-   在控制台上可以使用cat命令查看信息，cat /proc/umap/ive；也可以使用其他常用的文件操作命令，例如 cp /proc/umap/ive ./，将文件拷贝到当前目录。
-   在应用程序中可以将上述文件当作普通只读文件进行读操作，例如fopen、fread等。

>![](public_sys-resources/icon-note.gif) **说明：** 
>参数在描述时有以下2种情况需要注意：
>-   取值为\{0, 1\}的参数，如未列出具体取值和含义的对应关系，则参数为1时表示肯定，为0时表示否定。
>-   取值为\{aaa, bbb, ccc\}的参数，未列出具体取值和含义的对应关系，但可直接根据取值aaa、bbb或ccc判断参数含义

## Proc信息说明<a name="ZH-CN_TOPIC_0000002503971199"></a>

【调试信息】

```
~ # cat /proc/umap/ive
 
[IVE]  Version: [Vx.x.x.x B0xx Release],   Build Time[Aug 2 2019, 10:08:05] 
 
-------------------------------moudle param------------------------------
         save_power     max_node_num
              0               20
 
-------------------------------ive queue info----------------------------
      wait    busy  wait_cur_id  wait_end_id  busy_cur_id busy_end_id
         1      -1         0              0         0                 0
 
--------------------------------ive task info----------------------------
       handle   task_finish    last_id    task_id   handle_wrap   finish_wrap
             5               5            0           0               0              0
 
-----------------------------------ive runtime info---------------------
last_inst     cnt_per_sec     max_cnt_per_sec      total_int_cnt_last_sec   
          0                 1                      2                               4
total_int_cnt      qt_cnt                st_cnt
              5            0                       0
 
cost_tm          m_cost_tm      cost_tm_per_sec           m_cost_tm_per_sec 
       7                   17                      17                             17
total_int_cost_tm               last_task_hw_tm                        run_tm
                  64                               20                             20
----------------------------------------ive invoke info-----------------------------------------------------------------

       dma          filter        csc     filter_csc       sobel      mag_ang          dilate           erode
         0               0          0              0           0            0               0               0

 threshold             and        sub             or       integ         hist      thresh_s16      thresh_u16
         0               0          0              0           0            0               0               0

     16to8 ord_stat_filter    bernsen            map  equal_hist          add             xor             ncc
         0               0          0              0           0            0               0               0

       ccl             gmm      canny            lbp   norm_grad           lk       shitomasi         grad_fg
         0               0          0              0           0            0               0               0

 match_mod      update_mod      radon            ann         svm      adp_thr   line_filter_h  noise_remove_h
         0               0          0              0           0            0               0               0

plate_char             sad       gmm2         resize         cnn  persp_trans
         0               0          0              0           0            0

----------------------------------------kcf module param----------------------------------------------------------------
                          save_power                         max_node_num
                                   1                                  128

----------------------------------------kcf queue info------------------------------------------------------------------
   core_id         wait      busy     wait_cur_id   wait_end_id   busy_cur_id   busy_end_id
         0           0        -1             0             0             0             0
         1           0        -1             0             0             0             0

----------------------------------------kcf task info-------------------------------------------------------------------
   core_id        handle     task_finish       last_id     task_id     handle_wrap     finish_wrap
         0         0             384             0          63               0               0
         1         0              0              0           0               0               0

----------------------------------------kcf runtime info----------------------------------------------------------------
   core_id    last_inst    cnt_per_sec   max_cnt_per_sec    total_int_cnt_last_sec      total_int_cnt           qt_cnt
         0            1              1                 1                         6                  6                0
         1            0              0                 0                         0                  0                0

    st_cnt      cost_tm      m_cost_tm   cost_tm_per_sec         m_cost_tm_per_sec  total_int_cost_tm  last_task_hw_tm
         0           12             13                12                        13                 65            47566
         0            0              0                 0                         0                  0                0

    run_tm
        61
        61

----------------------------------------kcf invoke info-----------------------------------------------------------------

   core_id             kcf             hog
         0               6               0
         1               0               0
----------------------------------------ive utili info---------------------------------------------------
  utili  
   26
```

【调试信息分析】

记录当前IVE工作状态资源信息和算子调用信息。

【参数说明】

<a name="table7920mcpsimp"></a>
<table><thead align="left"><tr id="row7926mcpsimp"><th class="cellrowborder" colspan="2" valign="top" id="mcps1.1.4.1.1"><p id="p7928mcpsimp"><a name="p7928mcpsimp"></a><a name="p7928mcpsimp"></a>参数</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.1.4.1.2"><p id="p7930mcpsimp"><a name="p7930mcpsimp"></a><a name="p7930mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row7932mcpsimp"><td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p7934mcpsimp"><a name="p7934mcpsimp"></a><a name="p7934mcpsimp"></a>module param 模块参数</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p7936mcpsimp"><a name="p7936mcpsimp"></a><a name="p7936mcpsimp"></a>save_power</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p7938mcpsimp"><a name="p7938mcpsimp"></a><a name="p7938mcpsimp"></a>低功耗标志。</p>
<p id="p7939mcpsimp"><a name="p7939mcpsimp"></a><a name="p7939mcpsimp"></a>0：关闭低功耗；</p>
<p id="p7940mcpsimp"><a name="p7940mcpsimp"></a><a name="p7940mcpsimp"></a>1：打开低功耗。</p>
</td>
</tr>
<tr id="row7941mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7943mcpsimp"><a name="p7943mcpsimp"></a><a name="p7943mcpsimp"></a>max_node_num</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7945mcpsimp"><a name="p7945mcpsimp"></a><a name="p7945mcpsimp"></a>队列最大节点数。</p>
<p id="p7946mcpsimp"><a name="p7946mcpsimp"></a><a name="p7946mcpsimp"></a>取值范围[20, 512]。</p>
</td>
</tr>
<tr id="row7947mcpsimp"><td class="cellrowborder" rowspan="6" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p7949mcpsimp"><a name="p7949mcpsimp"></a><a name="p7949mcpsimp"></a>ive queue info</p>
<p id="p7950mcpsimp"><a name="p7950mcpsimp"></a><a name="p7950mcpsimp"></a>IVE队列信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p7952mcpsimp"><a name="p7952mcpsimp"></a><a name="p7952mcpsimp"></a>wait</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p7954mcpsimp"><a name="p7954mcpsimp"></a><a name="p7954mcpsimp"></a>等待队列编号（0或1）。</p>
</td>
</tr>
<tr id="row7955mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7957mcpsimp"><a name="p7957mcpsimp"></a><a name="p7957mcpsimp"></a>busy</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7959mcpsimp"><a name="p7959mcpsimp"></a><a name="p7959mcpsimp"></a>正在调度队列编号（0，1或-1），</p>
<p id="p7960mcpsimp"><a name="p7960mcpsimp"></a><a name="p7960mcpsimp"></a>-1表示IVE硬件空闲。</p>
</td>
</tr>
<tr id="row7961mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7963mcpsimp"><a name="p7963mcpsimp"></a><a name="p7963mcpsimp"></a>wait_cur_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7965mcpsimp"><a name="p7965mcpsimp"></a><a name="p7965mcpsimp"></a>等待队列的首个有效任务ID。</p>
</td>
</tr>
<tr id="row7966mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7968mcpsimp"><a name="p7968mcpsimp"></a><a name="p7968mcpsimp"></a>wait_end_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7970mcpsimp"><a name="p7970mcpsimp"></a><a name="p7970mcpsimp"></a>等待队列的末尾有效任务ID + 1。</p>
</td>
</tr>
<tr id="row7971mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7973mcpsimp"><a name="p7973mcpsimp"></a><a name="p7973mcpsimp"></a>busy_cur_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7975mcpsimp"><a name="p7975mcpsimp"></a><a name="p7975mcpsimp"></a>调度队列的首个有效任务ID。</p>
</td>
</tr>
<tr id="row7976mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7978mcpsimp"><a name="p7978mcpsimp"></a><a name="p7978mcpsimp"></a>busy_end_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7980mcpsimp"><a name="p7980mcpsimp"></a><a name="p7980mcpsimp"></a>调度队列的末尾有效任务ID + 1。</p>
</td>
</tr>
<tr id="row7981mcpsimp"><td class="cellrowborder" rowspan="6" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p7983mcpsimp"><a name="p7983mcpsimp"></a><a name="p7983mcpsimp"></a>ive task info</p>
<p id="p7984mcpsimp"><a name="p7984mcpsimp"></a><a name="p7984mcpsimp"></a>IVE TASK相关信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p7986mcpsimp"><a name="p7986mcpsimp"></a><a name="p7986mcpsimp"></a>handle</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p7988mcpsimp"><a name="p7988mcpsimp"></a><a name="p7988mcpsimp"></a>当前可分配的任务handle号。</p>
</td>
</tr>
<tr id="row7989mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7991mcpsimp"><a name="p7991mcpsimp"></a><a name="p7991mcpsimp"></a>task_finish</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7993mcpsimp"><a name="p7993mcpsimp"></a><a name="p7993mcpsimp"></a>当前已完成任务的个数。</p>
</td>
</tr>
<tr id="row7994mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7996mcpsimp"><a name="p7996mcpsimp"></a><a name="p7996mcpsimp"></a>last_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p7998mcpsimp"><a name="p7998mcpsimp"></a><a name="p7998mcpsimp"></a>上一次中断完成的任务ID。</p>
</td>
</tr>
<tr id="row7999mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8001mcpsimp"><a name="p8001mcpsimp"></a><a name="p8001mcpsimp"></a>task_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8003mcpsimp"><a name="p8003mcpsimp"></a><a name="p8003mcpsimp"></a>本次中断完成的任务ID。</p>
</td>
</tr>
<tr id="row8004mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8006mcpsimp"><a name="p8006mcpsimp"></a><a name="p8006mcpsimp"></a>handle_wrap</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8008mcpsimp"><a name="p8008mcpsimp"></a><a name="p8008mcpsimp"></a>用户handle号分配发生回写的次数。</p>
</td>
</tr>
<tr id="row8009mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8011mcpsimp"><a name="p8011mcpsimp"></a><a name="p8011mcpsimp"></a>finish_wrap</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8013mcpsimp"><a name="p8013mcpsimp"></a><a name="p8013mcpsimp"></a>完成任务数发生回写的次数。</p>
</td>
</tr>
<tr id="row8014mcpsimp"><td class="cellrowborder" rowspan="14" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p8016mcpsimp"><a name="p8016mcpsimp"></a><a name="p8016mcpsimp"></a>ive runtime info</p>
<p id="p8017mcpsimp"><a name="p8017mcpsimp"></a><a name="p8017mcpsimp"></a>IVE运行时相关信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p8019mcpsimp"><a name="p8019mcpsimp"></a><a name="p8019mcpsimp"></a>last_inst</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p8021mcpsimp"><a name="p8021mcpsimp"></a><a name="p8021mcpsimp"></a>用户最后一次提交任务时传入的is_instant值。</p>
</td>
</tr>
<tr id="row8022mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8024mcpsimp"><a name="p8024mcpsimp"></a><a name="p8024mcpsimp"></a>cnt_per_sec</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8026mcpsimp"><a name="p8026mcpsimp"></a><a name="p8026mcpsimp"></a>最近一次的1秒内中断执行次数。</p>
</td>
</tr>
<tr id="row8027mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8029mcpsimp"><a name="p8029mcpsimp"></a><a name="p8029mcpsimp"></a>max_cnt_per_sec</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8031mcpsimp"><a name="p8031mcpsimp"></a><a name="p8031mcpsimp"></a>历史上的1秒内最大的中断执行次数。</p>
</td>
</tr>
<tr id="row8032mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8034mcpsimp"><a name="p8034mcpsimp"></a><a name="p8034mcpsimp"></a>total_int_cnt_last_sec</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8036mcpsimp"><a name="p8036mcpsimp"></a><a name="p8036mcpsimp"></a>上一秒上报中断总次数。</p>
</td>
</tr>
<tr id="row8037mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8039mcpsimp"><a name="p8039mcpsimp"></a><a name="p8039mcpsimp"></a>total_int_cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8041mcpsimp"><a name="p8041mcpsimp"></a><a name="p8041mcpsimp"></a>IVE产生中断的总次数。</p>
</td>
</tr>
<tr id="row8042mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8044mcpsimp"><a name="p8044mcpsimp"></a><a name="p8044mcpsimp"></a>qt_cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8046mcpsimp"><a name="p8046mcpsimp"></a><a name="p8046mcpsimp"></a>查询IVE链表超时次数。</p>
</td>
</tr>
<tr id="row8047mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8049mcpsimp"><a name="p8049mcpsimp"></a><a name="p8049mcpsimp"></a>st_cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8051mcpsimp"><a name="p8051mcpsimp"></a><a name="p8051mcpsimp"></a>IVE系统超时次数。</p>
</td>
</tr>
<tr id="row8052mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8054mcpsimp"><a name="p8054mcpsimp"></a><a name="p8054mcpsimp"></a>cost_tm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8056mcpsimp"><a name="p8056mcpsimp"></a><a name="p8056mcpsimp"></a>最近一次执行中断的执行耗时。</p>
<p id="p8057mcpsimp"><a name="p8057mcpsimp"></a><a name="p8057mcpsimp"></a>单位：us</p>
</td>
</tr>
<tr id="row8058mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8060mcpsimp"><a name="p8060mcpsimp"></a><a name="p8060mcpsimp"></a>m_cost_tm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8062mcpsimp"><a name="p8062mcpsimp"></a><a name="p8062mcpsimp"></a>执行一次中断的最大耗时。</p>
<p id="p8063mcpsimp"><a name="p8063mcpsimp"></a><a name="p8063mcpsimp"></a>单位：us</p>
</td>
</tr>
<tr id="row8064mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8066mcpsimp"><a name="p8066mcpsimp"></a><a name="p8066mcpsimp"></a>cost_tm_per_sec</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8068mcpsimp"><a name="p8068mcpsimp"></a><a name="p8068mcpsimp"></a>最近一秒执行中断的执行耗时。</p>
<p id="p8069mcpsimp"><a name="p8069mcpsimp"></a><a name="p8069mcpsimp"></a>单位：us</p>
</td>
</tr>
<tr id="row8070mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8072mcpsimp"><a name="p8072mcpsimp"></a><a name="p8072mcpsimp"></a>m_cost_tm_per_sec</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8074mcpsimp"><a name="p8074mcpsimp"></a><a name="p8074mcpsimp"></a>历史上一秒执行中断的最大执行耗时。</p>
<p id="p8075mcpsimp"><a name="p8075mcpsimp"></a><a name="p8075mcpsimp"></a>单位：us</p>
</td>
</tr>
<tr id="row8076mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8078mcpsimp"><a name="p8078mcpsimp"></a><a name="p8078mcpsimp"></a>total_int_cost_tm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8080mcpsimp"><a name="p8080mcpsimp"></a><a name="p8080mcpsimp"></a>中断处理总时间。</p>
<p id="p8081mcpsimp"><a name="p8081mcpsimp"></a><a name="p8081mcpsimp"></a>单位：us</p>
</td>
</tr>
<tr id="row8082mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8084mcpsimp"><a name="p8084mcpsimp"></a><a name="p8084mcpsimp"></a>last_task_hw_tm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8086mcpsimp"><a name="p8086mcpsimp"></a><a name="p8086mcpsimp"></a>最近IVE算子硬件运行时间</p>
<p id="p8087mcpsimp"><a name="p8087mcpsimp"></a><a name="p8087mcpsimp"></a>单位：us</p>
</td>
</tr>
<tr id="row8088mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8090mcpsimp"><a name="p8090mcpsimp"></a><a name="p8090mcpsimp"></a>run_tm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p8092mcpsimp"><a name="p8092mcpsimp"></a><a name="p8092mcpsimp"></a>IVE运行总时间。</p>
<p id="p8093mcpsimp"><a name="p8093mcpsimp"></a><a name="p8093mcpsimp"></a>单位：s</p>
</td>
</tr>
<tr id="row8094mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p8096mcpsimp"><a name="p8096mcpsimp"></a><a name="p8096mcpsimp"></a>ive invoke info</p>
<p id="p8097mcpsimp"><a name="p8097mcpsimp"></a><a name="p8097mcpsimp"></a>IVE调用信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p8099mcpsimp"><a name="p8099mcpsimp"></a><a name="p8099mcpsimp"></a>dma</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p8101mcpsimp"><a name="p8101mcpsimp"></a><a name="p8101mcpsimp"></a>dma的调用次数。其它不再一一列出。</p>
</td>
</tr>
<tr id="row11561145203317"><td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p3561152113316"><a name="p3561152113316"></a><a name="p3561152113316"></a>kcf module param</p>
<p id="p043616911344"><a name="p043616911344"></a><a name="p043616911344"></a>kcf模块参数</p>
<p id="p84160288342"><a name="p84160288342"></a><a name="p84160288342"></a></p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p1584874283410"><a name="p1584874283410"></a><a name="p1584874283410"></a>save_power</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p784804273412"><a name="p784804273412"></a><a name="p784804273412"></a>低功耗标志。</p>
<p id="p8848174216345"><a name="p8848174216345"></a><a name="p8848174216345"></a>0：关闭低功耗；</p>
<p id="p178481542173417"><a name="p178481542173417"></a><a name="p178481542173417"></a>1：打开低功耗。</p>
</td>
</tr>
<tr id="row15415328153418"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p11848642133411"><a name="p11848642133411"></a><a name="p11848642133411"></a>max_node_num</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p17848194273416"><a name="p17848194273416"></a><a name="p17848194273416"></a>队列最大节点数。</p>
<p id="p4848184218341"><a name="p4848184218341"></a><a name="p4848184218341"></a>取值范围[20, 128]。</p>
</td>
</tr>
<tr id="row189632711437"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p1296192714310"><a name="p1296192714310"></a><a name="p1296192714310"></a>kcf queue info</p>
<p id="p43251844114311"><a name="p43251844114311"></a><a name="p43251844114311"></a>KCF队列信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p19962279439"><a name="p19962279439"></a><a name="p19962279439"></a>core_id</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p2961127114319"><a name="p2961127114319"></a><a name="p2961127114319"></a>核编号（0或1）。其他同ive queue info，不再一一列出。</p>
</td>
</tr>
<tr id="row286418994517"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p2864109144510"><a name="p2864109144510"></a><a name="p2864109144510"></a>kcf task info</p>
<p id="p19453171912453"><a name="p19453171912453"></a><a name="p19453171912453"></a>KCF TASK相关信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p986419104513"><a name="p986419104513"></a><a name="p986419104513"></a>core_id</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p1986417920459"><a name="p1986417920459"></a><a name="p1986417920459"></a>核编号（0或1）。其他同ive task info，不再一一列出。</p>
</td>
</tr>
<tr id="row5479029174018"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p114801229164012"><a name="p114801229164012"></a><a name="p114801229164012"></a>kcf runtime info</p>
<p id="p0525191462"><a name="p0525191462"></a><a name="p0525191462"></a>KCF运行时相关信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p9480829144010"><a name="p9480829144010"></a><a name="p9480829144010"></a>core_id</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p948052918400"><a name="p948052918400"></a><a name="p948052918400"></a>核编号（0或1）。其他同ive runtime info，不再一一列出。</p>
</td>
</tr>
<tr id="row127883904712"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p278819911479"><a name="p278819911479"></a><a name="p278819911479"></a>kcf invoke info</p>
<p id="p12917246184713"><a name="p12917246184713"></a><a name="p12917246184713"></a>KCF调用信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p19788099478"><a name="p19788099478"></a><a name="p19788099478"></a>core_id</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p1978859194717"><a name="p1978859194717"></a><a name="p1978859194717"></a>核编号（0或1）。</p>
</td>
</tr>
<tr id="row9917446174715"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p14917154604718"><a name="p14917154604718"></a><a name="p14917154604718"></a>kcf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p0917144604712"><a name="p0917144604712"></a><a name="p0917144604712"></a>kcf的调用次数。</p>
</td>
</tr>
<tr id="row12435116474"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p104145118478"><a name="p104145118478"></a><a name="p104145118478"></a>hog</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p641751104719"><a name="p641751104719"></a><a name="p641751104719"></a>hog的调用次数。</p>
</td>
</tr>
<tr id="row8102mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p8104mcpsimp"><a name="p8104mcpsimp"></a><a name="p8104mcpsimp"></a>ive utili info IVE硬件利用率信息</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p8106mcpsimp"><a name="p8106mcpsimp"></a><a name="p8106mcpsimp"></a>utili</p>
</td>
<td class="cellrowborder" valign="top" width="56.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p8108mcpsimp"><a name="p8108mcpsimp"></a><a name="p8108mcpsimp"></a>IVE硬件利用率。</p>
<p id="p8109mcpsimp"><a name="p8109mcpsimp"></a><a name="p8109mcpsimp"></a>单位：%</p>
</td>
</tr>
</tbody>
</table>

【注意】

-   建议代码调试阶段关闭低功耗，调试完成再打开低功耗。
-   在非低功耗场景时，才会统计IVE硬件利用率。

# FAQ<a name="ZH-CN_TOPIC_0000002504091085"></a>






## 使用PC端IVE Clib与板端IVESDK开发算法的差异<a name="ZH-CN_TOPIC_0000002504091167"></a>

**表 1**  使用PC端IVE Clib与板端IVE SDK开发算法的差异

<a name="table1661mcpsimp"></a>
<table><thead align="left"><tr id="row1672mcpsimp"><th class="cellrowborder" valign="top" width="6.9306930693069315%" id="mcps1.2.5.1.1"><p xml:lang="fr-FR" id="p1674mcpsimp"><a name="p1674mcpsimp"></a><a name="p1674mcpsimp"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="13.861386138613863%" id="mcps1.2.5.1.2"><p id="p1676mcpsimp"><a name="p1676mcpsimp"></a><a name="p1676mcpsimp"></a>关键词</p>
</th>
<th class="cellrowborder" valign="top" width="36.633663366336634%" id="mcps1.2.5.1.3"><p xml:lang="fr-FR" id="p1678mcpsimp"><a name="p1678mcpsimp"></a><a name="p1678mcpsimp"></a>PC端IVE Clib</p>
</th>
<th class="cellrowborder" valign="top" width="42.57425742574258%" id="mcps1.2.5.1.4"><p xml:lang="fr-FR" id="p1680mcpsimp"><a name="p1680mcpsimp"></a><a name="p1680mcpsimp"></a>板端SDK</p>
</th>
</tr>
</thead>
<tbody><tr id="row1682mcpsimp"><td class="cellrowborder" valign="top" width="6.9306930693069315%" headers="mcps1.2.5.1.1 "><p xml:lang="fr-FR" id="p1684mcpsimp"><a name="p1684mcpsimp"></a><a name="p1684mcpsimp"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="13.861386138613863%" headers="mcps1.2.5.1.2 "><p xml:lang="fr-FR" id="p1686mcpsimp"><a name="p1686mcpsimp"></a><a name="p1686mcpsimp"></a>handle</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.2.5.1.3 "><p xml:lang="fr-FR" id="p1688mcpsimp"><a name="p1688mcpsimp"></a><a name="p1688mcpsimp"></a>无效</p>
</td>
<td class="cellrowborder" valign="top" width="42.57425742574258%" headers="mcps1.2.5.1.4 "><p xml:lang="fr-FR" id="p1690mcpsimp"><a name="p1690mcpsimp"></a><a name="p1690mcpsimp"></a>当有需要时与ss_mpi_ive_query配合查询算子是否完成。详细参考“重要概念”的“<span xml:lang="en-US" id="ph12929916407"><a name="ph12929916407"></a><a name="ph12929916407"></a>句柄</span>(handle)”</p>
</td>
</tr>
<tr id="row1697mcpsimp"><td class="cellrowborder" valign="top" width="6.9306930693069315%" headers="mcps1.2.5.1.1 "><p xml:lang="fr-FR" id="p1699mcpsimp"><a name="p1699mcpsimp"></a><a name="p1699mcpsimp"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="13.861386138613863%" headers="mcps1.2.5.1.2 "><p xml:lang="fr-FR" id="p1701mcpsimp"><a name="p1701mcpsimp"></a><a name="p1701mcpsimp"></a>is_instant</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.2.5.1.3 "><p xml:lang="fr-FR" id="p1703mcpsimp"><a name="p1703mcpsimp"></a><a name="p1703mcpsimp"></a>无效</p>
</td>
<td class="cellrowborder" valign="top" width="42.57425742574258%" headers="mcps1.2.5.1.4 "><p xml:lang="fr-FR" id="p1705mcpsimp"><a name="p1705mcpsimp"></a><a name="p1705mcpsimp"></a>根据算法设置可减少中断，提升性能。详细参考“重要概念”的“<span xml:lang="en-US" id="ph1777353014017"><a name="ph1777353014017"></a><a name="ph1777353014017"></a>及时返回结果标志</span>”</p>
</td>
</tr>
<tr id="row1711mcpsimp"><td class="cellrowborder" valign="top" width="6.9306930693069315%" headers="mcps1.2.5.1.1 "><p xml:lang="fr-FR" id="p1713mcpsimp"><a name="p1713mcpsimp"></a><a name="p1713mcpsimp"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="13.861386138613863%" headers="mcps1.2.5.1.2 "><p xml:lang="fr-FR" id="p1715mcpsimp"><a name="p1715mcpsimp"></a><a name="p1715mcpsimp"></a>query</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.2.5.1.3 "><p xml:lang="fr-FR" id="p1717mcpsimp"><a name="p1717mcpsimp"></a><a name="p1717mcpsimp"></a>不需要查询，查询永远返回成功</p>
</td>
<td class="cellrowborder" valign="top" width="42.57425742574258%" headers="mcps1.2.5.1.4 "><p xml:lang="fr-FR" id="p1719mcpsimp"><a name="p1719mcpsimp"></a><a name="p1719mcpsimp"></a>当用户需要使用IVE硬算子的结果时，必须查询任务是否完成。详细参考“重要概念”的“<span xml:lang="en-US" id="ph175614454018"><a name="ph175614454018"></a><a name="ph175614454018"></a>查询(query)</span>”以及ss_mpi_ive_query接口说明。</p>
</td>
</tr>
<tr id="row1727mcpsimp"><td class="cellrowborder" valign="top" width="6.9306930693069315%" headers="mcps1.2.5.1.1 "><p xml:lang="fr-FR" id="p1729mcpsimp"><a name="p1729mcpsimp"></a><a name="p1729mcpsimp"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="13.861386138613863%" headers="mcps1.2.5.1.2 "><p xml:lang="fr-FR" id="p1731mcpsimp"><a name="p1731mcpsimp"></a><a name="p1731mcpsimp"></a>内存开辟、物理地址、虚拟地址</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.2.5.1.3 "><p xml:lang="fr-FR" id="p1733mcpsimp"><a name="p1733mcpsimp"></a><a name="p1733mcpsimp"></a>内存使用malloc开辟。由于malloc出来均为虚拟地址，所以赋值给虚拟地址。为模拟IVE硬件使用物理地址的特性，Clib也使用物理地址，所以物理地址必须赋值，且必须赋值为虚拟地址的td_u32强制类转化。</p>
</td>
<td class="cellrowborder" valign="top" width="42.57425742574258%" headers="mcps1.2.5.1.4 "><p xml:lang="fr-FR" id="p1735mcpsimp"><a name="p1735mcpsimp"></a><a name="p1735mcpsimp"></a>IVE硬件使用物理地址。内存使用ss_mpi_sys_mmz_malloc/ss_mpi_sys_mmz_alloc_cached接口详细信息请参见《MPP 媒体处理软件<span xml:lang="en-US" id="ph1736mcpsimp"><a name="ph1736mcpsimp"></a><a name="ph1736mcpsimp"></a>Vx.y</span> 开发参考》）开辟，物理地址和虚拟地址由此生成；或者使用其他模块的VB内存。</p>
</td>
</tr>
<tr id="row1737mcpsimp"><td class="cellrowborder" valign="top" width="6.9306930693069315%" headers="mcps1.2.5.1.1 "><p xml:lang="fr-FR" id="p1739mcpsimp"><a name="p1739mcpsimp"></a><a name="p1739mcpsimp"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="13.861386138613863%" headers="mcps1.2.5.1.2 "><p xml:lang="fr-FR" id="p1741mcpsimp"><a name="p1741mcpsimp"></a><a name="p1741mcpsimp"></a>地址对齐</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.2.5.1.3 "><p xml:lang="fr-FR" id="p1743mcpsimp"><a name="p1743mcpsimp"></a><a name="p1743mcpsimp"></a>Clib不要求地址对齐</p>
</td>
<td class="cellrowborder" valign="top" width="42.57425742574258%" headers="mcps1.2.5.1.4 "><p xml:lang="fr-FR" id="p1745mcpsimp"><a name="p1745mcpsimp"></a><a name="p1745mcpsimp"></a>硬件地址按要求对齐。</p>
</td>
</tr>
<tr id="row1746mcpsimp"><td class="cellrowborder" valign="top" width="6.9306930693069315%" headers="mcps1.2.5.1.1 "><p xml:lang="fr-FR" id="p1748mcpsimp"><a name="p1748mcpsimp"></a><a name="p1748mcpsimp"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="13.861386138613863%" headers="mcps1.2.5.1.2 "><p xml:lang="fr-FR" id="p1750mcpsimp"><a name="p1750mcpsimp"></a><a name="p1750mcpsimp"></a>解决方案差异</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.2.5.1.3 "><p xml:lang="fr-FR" id="p1752mcpsimp"><a name="p1752mcpsimp"></a><a name="p1752mcpsimp"></a>Clib是功能全集，接口会更新到最新版。</p>
</td>
<td class="cellrowborder" valign="top" width="42.57425742574258%" headers="mcps1.2.5.1.4 "><p xml:lang="fr-FR" id="p1754mcpsimp"><a name="p1754mcpsimp"></a><a name="p1754mcpsimp"></a>解决方案根据需求，支持的功能是Clib中的子集。某些接口由于升级可能与最新版Clib不一致。</p>
</td>
</tr>
<tr id="row1755mcpsimp"><td class="cellrowborder" valign="top" width="6.9306930693069315%" headers="mcps1.2.5.1.1 "><p xml:lang="fr-FR" id="p1757mcpsimp"><a name="p1757mcpsimp"></a><a name="p1757mcpsimp"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="13.861386138613863%" headers="mcps1.2.5.1.2 "><p xml:lang="fr-FR" id="p1759mcpsimp"><a name="p1759mcpsimp"></a><a name="p1759mcpsimp"></a>异步、同步、并行、串行</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.2.5.1.3 "><p xml:lang="fr-FR" id="p1761mcpsimp"><a name="p1761mcpsimp"></a><a name="p1761mcpsimp"></a>Clib的执行与算法软件均在CPU中串行执行，不存在异步问题。</p>
</td>
<td class="cellrowborder" valign="top" width="42.57425742574258%" headers="mcps1.2.5.1.4 "><p xml:lang="fr-FR" id="p1763mcpsimp"><a name="p1763mcpsimp"></a><a name="p1763mcpsimp"></a>IVE硬件与CPU异步执行，可以此特性让IVE与CPU并行工作，提高性能。但是在CPU需要使用IVE的结果数据时，需要同步。</p>
</td>
</tr>
</tbody>
</table>

## 使用IVE与OpenCV开发算法的区别<a name="ZH-CN_TOPIC_0000002503971169"></a>

1.  IVE与CPU异步，CPU必须查询IVE任务是否完成；用OpenCV开发算法不需要；
2.  IVE的参数一般是定点化的，内部计算也是定点的；OpenCV一般是浮点的参数和计算；故同样的功能，IVE相比OpenCV有范围和精度上的限制。
3.  IVE使用物理地址，且对起始地址以及跨度有对齐要求；OpenCV不需要物理地址，也无对齐要求；
4.  IVE有软硬件分工，有些算子硬件实现一部分，软件实现一部分，也就是会有硬件+软件多个接口实现一个OpenCV算子的情况。
5.  IVE算子的控制参数尽可能与OpenCV算子对齐，方便客户从OpenCV算子适配到IVE。例如金字塔LK光流计算控制参数ot\_ive\_lk\_optical\_flow\_pyr\_ctrl结构体内的成员变量与对应的OpenCV算子输入参数变量的含义是一一对齐的；对应的浮点转定点方式可以通过IVE算子参数变量的类型\(例如td\_u0q8\)和sample样例中参数变量的赋值\(例如0.1 \* 256\)来分析其转换关系。
6.  若从OpenCV算子适配到IVE算子的开发过程中报错导致无法运行或结果与预期不一致，建议参考sample代码先检查IVE算子的输入是否配置完备、规格和数值范围是否符合规格要求；根据检查代码的报错打印和错误码确认问题范围和原因等。

## ANN/SVM查找表的建立<a name="ZH-CN_TOPIC_0000002471091298"></a>

>![](public_sys-resources/icon-note.gif) **说明：** 
>以函数f\(u\)建立查找表为例，建立查找表的步骤如下\(下面提到的table\_in\_lower，table\_in\_upper，table\_in\_precision，elem\_num，table\_out\_norm，table请参ot\_svp\_lut结构体说明\).

1.  明确自变量u的范围：若u在\[a, b\]之间，假设r=b-a; a、b分别对应table\_in\_lower，table\_in\_upper；（ANN建表时u即为自变量u；svm建表时u对应![](figures/zh-cn_formulaimage_0000002504091435.png)或者![](figures/zh-cn_formulaimage_0000002470931574.png)，具体公式可参考ss\_mpi\_ive\_ann\_mlp\_predict和ss\_mpi\_ive\_svm\_predict的【注意】）
2.  明确自变量1个单位的采样数g，则g=1<< table\_in\_precision；整个查找表的元素个数elem\_num =r\*g=r<< table\_in\_precision; \(ANN和SVM均对查找表有最大数目限制要求，具体参考ss\_mpi\_ive\_ann\_mlp\_predict和ss\_mpi\_ive\_svm\_predict的【注意】\)
3.  明确值域f\(u\)的范围，由于一般需要将值域约束到\[-1, 1\]，这时候，可以使用table\_out\_norm或者1<<table\_out\_norm做除数来对f\(u\)做归一化;（ANN仅支持1<<table\_out\_norm做除数；SVM支持2种，所以ive\_xml2bin\_ui.exe中对svm的模型转换时要求输入divisor，且divisor必须与建立查找表的归一化除数一致）
4.  根据f\(u\)的具体公式以及u的采样值，生成对应的f\(u\)查找表，保存在table中。

## Cache内存的使用<a name="ZH-CN_TOPIC_0000002504091187"></a>

内存开辟是否带cache，与算法软件对这片内存的使用主体相关。由于IVE是直接读取DDR内存数据，若此时使用的内存带有cache，必须刷cache来保证数据的一致性。所以若使用主体为IVE，CPU不使用或者仅使用一次，那么建议这片内存不带cache；若CPU是使用主体，建议这片内存带cache。

## 打开/关闭低功耗方法<a name="ZH-CN_TOPIC_0000002504091175"></a>

IVE低功耗开关是通过模块参数传递。在加载IVE ko时把模块参数save\_power值设为1（save\_power=1），即可打开低功耗，反之为关闭低功耗。默认情况下是打开低功耗。

