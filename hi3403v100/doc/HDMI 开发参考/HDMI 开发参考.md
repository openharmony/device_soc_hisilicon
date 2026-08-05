# 前言<a name="ZH-CN_TOPIC_0000002441654049"></a>

**概述<a name="section5493mcpsimp"></a>**

解决方案的内置HDMI（High-Definition MultiMedia Interface）模块支持视频的HDMI输出。

>![](public_sys-resources/icon-note.gif) **说明：** 
>本文未有特殊说明，SS625V100、SS522V101、SS522V100与SS524V100完全一致；SS927V100与SS928V100内容完全一致。

**产品版本<a name="section5496mcpsimp"></a>**

与本文档相对应的产品版本如下。

<a name="table5499mcpsimp"></a>
<table><thead align="left"><tr id="row5504mcpsimp"><th class="cellrowborder" valign="top" width="32%" id="mcps1.1.3.1.1"><p id="p5506mcpsimp"><a name="p5506mcpsimp"></a><a name="p5506mcpsimp"></a>产品名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.3.1.2"><p id="p5508mcpsimp"><a name="p5508mcpsimp"></a><a name="p5508mcpsimp"></a>产品版本</p>
</th>
</tr>
</thead>
<tbody><tr id="row5510mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5512mcpsimp"><a name="p5512mcpsimp"></a><a name="p5512mcpsimp"></a>SS928</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5514mcpsimp"><a name="p5514mcpsimp"></a><a name="p5514mcpsimp"></a>V100</p>
</td>
</tr>
<tr id="row5515mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5517mcpsimp"><a name="p5517mcpsimp"></a><a name="p5517mcpsimp"></a>SS626</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5519mcpsimp"><a name="p5519mcpsimp"></a><a name="p5519mcpsimp"></a>V100</p>
</td>
</tr>
<tr id="row181205115816"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p881081984715"><a name="p881081984715"></a><a name="p881081984715"></a>SS524</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p34921898474"><a name="p34921898474"></a><a name="p34921898474"></a>V100</p>
</td>
</tr>
<tr id="row54017581329"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p114110581217"><a name="p114110581217"></a><a name="p114110581217"></a>SS522</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p20412585210"><a name="p20412585210"></a><a name="p20412585210"></a>V100</p>
</td>
</tr>
<tr id="row202519210319"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p8106102017315"><a name="p8106102017315"></a><a name="p8106102017315"></a>SS522</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p1610617201536"><a name="p1610617201536"></a><a name="p1610617201536"></a>V101</p>
</td>
</tr>
<tr id="row20654192435218"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p6427175519594"><a name="p6427175519594"></a><a name="p6427175519594"></a>SS528</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p64271955205915"><a name="p64271955205915"></a><a name="p64271955205915"></a>V100</p>
</td>
</tr>
<tr id="row760814334812"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p187350151849"><a name="p187350151849"></a><a name="p187350151849"></a>SS625</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p46095339816"><a name="p46095339816"></a><a name="p46095339816"></a>V100</p>
</td>
</tr>
<tr id="row153105414511"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p8622349102117"><a name="p8622349102117"></a><a name="p8622349102117"></a>SS927</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p9185184311112"><a name="p9185184311112"></a><a name="p9185184311112"></a>V100</p>
</td>
</tr>
</tbody>
</table>

**读者对象<a name="section5520mcpsimp"></a>**

本文档（本指南）主要适用于以下工程师：

-   技术支持工程师
-   软件开发工程师

**符号约定<a name="section5526mcpsimp"></a>**

在本文中可能出现下列标志，它们所代表的含义如下。

<a name="table5529mcpsimp"></a>
<table><thead align="left"><tr id="row5534mcpsimp"><th class="cellrowborder" valign="top" width="21%" id="mcps1.1.3.1.1"><p id="p5536mcpsimp"><a name="p5536mcpsimp"></a><a name="p5536mcpsimp"></a>符号</p>
</th>
<th class="cellrowborder" valign="top" width="79%" id="mcps1.1.3.1.2"><p id="p5538mcpsimp"><a name="p5538mcpsimp"></a><a name="p5538mcpsimp"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row5540mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.3.1.1 "><p class="msonormal" id="p5542mcpsimp"><a name="p5542mcpsimp"></a><a name="p5542mcpsimp"></a><a name="image167"></a><a name="image167"></a><span><img id="image167" src="figures/zh-cn_image_0000002441693901.png" height="27.93" width="75.81"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79%" headers="mcps1.1.3.1.2 "><p id="p5544mcpsimp"><a name="p5544mcpsimp"></a><a name="p5544mcpsimp"></a>表示如不避免则将会导致死亡或严重伤害的具有高等级风险的危害。</p>
</td>
</tr>
<tr id="row5545mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.3.1.1 "><p class="msonormal" id="p5547mcpsimp"><a name="p5547mcpsimp"></a><a name="p5547mcpsimp"></a><a name="image168"></a><a name="image168"></a><span><img id="image168" src="figures/zh-cn_image_0000002408254650.png" height="27.93" width="75.81"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79%" headers="mcps1.1.3.1.2 "><p id="p5549mcpsimp"><a name="p5549mcpsimp"></a><a name="p5549mcpsimp"></a>表示如不避免则可能导致死亡或严重伤害的具有中等级风险的危害。</p>
</td>
</tr>
<tr id="row5550mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.3.1.1 "><p class="msonormal" id="p5552mcpsimp"><a name="p5552mcpsimp"></a><a name="p5552mcpsimp"></a><a name="image169"></a><a name="image169"></a><span><img id="image169" src="figures/zh-cn_image_0000002441654065.png" height="27.93" width="75.81"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79%" headers="mcps1.1.3.1.2 "><p id="p5554mcpsimp"><a name="p5554mcpsimp"></a><a name="p5554mcpsimp"></a>表示如不避免则可能导致轻微或中度伤害的具有低等级风险的危害。</p>
</td>
</tr>
<tr id="row5555mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.3.1.1 "><p class="msonormal" id="p5557mcpsimp"><a name="p5557mcpsimp"></a><a name="p5557mcpsimp"></a><a name="image170"></a><a name="image170"></a><span><img id="image170" src="figures/zh-cn_image_0000002408094738.png" height="27.93" width="75.81"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79%" headers="mcps1.1.3.1.2 "><p id="p5559mcpsimp"><a name="p5559mcpsimp"></a><a name="p5559mcpsimp"></a>用于传递设备或环境安全警示信息。如不避免则可能会导致设备损坏、数据丢失、设备性能降低或其它不可预知的结果。</p>
<p id="p5560mcpsimp"><a name="p5560mcpsimp"></a><a name="p5560mcpsimp"></a>“须知”不涉及人身伤害。</p>
</td>
</tr>
<tr id="row5561mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.3.1.1 "><p class="msonormal" id="p5563mcpsimp"><a name="p5563mcpsimp"></a><a name="p5563mcpsimp"></a><a name="image171"></a><a name="image171"></a><span><img id="image171" src="figures/zh-cn_image_0000002441654061.png" height="27.93" width="75.81"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79%" headers="mcps1.1.3.1.2 "><p id="p5565mcpsimp"><a name="p5565mcpsimp"></a><a name="p5565mcpsimp"></a>对正文中重点信息的补充说明。</p>
<p id="p5566mcpsimp"><a name="p5566mcpsimp"></a><a name="p5566mcpsimp"></a>“说明”不是安全警示信息，不涉及人身、设备及环境伤害信息。</p>
</td>
</tr>
</tbody>
</table>

**修订记录<a name="section5666mcpsimp"></a>**

修订记录累积了每次文档更新的说明。最新版本的文档包含以前所有文档版本的更新内容。

<a name="table2161mcpsimp"></a>
<table><thead align="left"><tr id="row2167mcpsimp"><th class="cellrowborder" valign="top" width="17.24%" id="mcps1.1.4.1.1"><p id="p2169mcpsimp"><a name="p2169mcpsimp"></a><a name="p2169mcpsimp"></a>文档版本</p>
</th>
<th class="cellrowborder" valign="top" width="24.27%" id="mcps1.1.4.1.2"><p id="p2171mcpsimp"><a name="p2171mcpsimp"></a><a name="p2171mcpsimp"></a>发布日期</p>
</th>
<th class="cellrowborder" valign="top" width="58.489999999999995%" id="mcps1.1.4.1.3"><p id="p2173mcpsimp"><a name="p2173mcpsimp"></a><a name="p2173mcpsimp"></a>修改说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row1117162275616"><td class="cellrowborder" valign="top" width="17.24%" headers="mcps1.1.4.1.1 "><p id="p68017313562"><a name="p68017313562"></a><a name="p68017313562"></a>00B02</p>
</td>
<td class="cellrowborder" valign="top" width="24.27%" headers="mcps1.1.4.1.2 "><p id="p280203125614"><a name="p280203125614"></a><a name="p280203125614"></a>2026-02-25</p>
</td>
<td class="cellrowborder" valign="top" width="58.489999999999995%" headers="mcps1.1.4.1.3 "><p id="p780103195613"><a name="p780103195613"></a><a name="p780103195613"></a>第2次临时版本发布。</p>
<p id="p285332213578"><a name="p285332213578"></a><a name="p285332213578"></a>“Debug调试”小节涉及修改。</p>
</td>
</tr>
<tr id="row2175mcpsimp"><td class="cellrowborder" valign="top" width="17.24%" headers="mcps1.1.4.1.1 "><p id="p2177mcpsimp"><a name="p2177mcpsimp"></a><a name="p2177mcpsimp"></a>00B01</p>
</td>
<td class="cellrowborder" valign="top" width="24.27%" headers="mcps1.1.4.1.2 "><p id="p2179mcpsimp"><a name="p2179mcpsimp"></a><a name="p2179mcpsimp"></a>2025-09-15</p>
</td>
<td class="cellrowborder" valign="top" width="58.489999999999995%" headers="mcps1.1.4.1.3 "><p id="p2181mcpsimp"><a name="p2181mcpsimp"></a><a name="p2181mcpsimp"></a>第1次临时版本发布。</p>
</td>
</tr>
</tbody>
</table>

# 概述<a name="ZH-CN_TOPIC_0000002408254646"></a>

-   **[重要概念](#ZH-CN_TOPIC_0000002441693893)**  

-   **[规格说明](#ZH-CN_TOPIC_0000002408254638)**  

## 重要概念<a name="ZH-CN_TOPIC_0000002441693893"></a>

HDMI的音频不能单独输出，必须依赖于视频输出，且HDMI的时钟来源于VO的时钟，因此接口调用顺序上需要先使能VO，再调用HDMI接口，然后配置音视频输出。

## 规格说明<a name="ZH-CN_TOPIC_0000002408254638"></a>

文档中未特别注明为HDMI2.0规格，默认只支持HDMI1.4规格的产品。

>![](public_sys-resources/icon-notice.gif) **须知：** 
>所有解决方案采用的是同一套API接口，但只支持HDMI1.4 API接口的解决方案，不支持HDMI2.0相关参数。

解决方案HDMI规格说明，如[表1](#_Ref472517318)所示。未明确列出的规格表示不支持。

**表 1**  解决方案HDMI规格说明

<a name="_Ref472517318"></a>
<table><thead align="left"><tr id="row3918mcpsimp"><th class="cellrowborder" valign="top" width="48%" id="mcps1.2.3.1.1"><p id="p3920mcpsimp"><a name="p3920mcpsimp"></a><a name="p3920mcpsimp"></a>产品名称</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.2.3.1.2"><p id="p3922mcpsimp"><a name="p3922mcpsimp"></a><a name="p3922mcpsimp"></a>HDMI规格</p>
</th>
</tr>
</thead>
<tbody><tr id="row3924mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.2.3.1.1 "><p id="p3926mcpsimp"><a name="p3926mcpsimp"></a><a name="p3926mcpsimp"></a>SS528V100</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.3.1.2 "><p id="p3928mcpsimp"><a name="p3928mcpsimp"></a><a name="p3928mcpsimp"></a>HDMI2.0</p>
</td>
</tr>
<tr id="row3929mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.2.3.1.1 "><p id="p3931mcpsimp"><a name="p3931mcpsimp"></a><a name="p3931mcpsimp"></a>SS625V100</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.3.1.2 "><p id="p3933mcpsimp"><a name="p3933mcpsimp"></a><a name="p3933mcpsimp"></a>HDMI1.4</p>
</td>
</tr>
<tr id="row3934mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.2.3.1.1 "><p id="p3936mcpsimp"><a name="p3936mcpsimp"></a><a name="p3936mcpsimp"></a>SS524V100</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.3.1.2 "><p id="p3938mcpsimp"><a name="p3938mcpsimp"></a><a name="p3938mcpsimp"></a>HDMI1.4</p>
</td>
</tr>
<tr id="row3939mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.2.3.1.1 "><p id="p3941mcpsimp"><a name="p3941mcpsimp"></a><a name="p3941mcpsimp"></a>SS522V101</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.3.1.2 "><p id="p3943mcpsimp"><a name="p3943mcpsimp"></a><a name="p3943mcpsimp"></a>HDMI1.4</p>
</td>
</tr>
<tr id="row3944mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.2.3.1.1 "><p id="p3946mcpsimp"><a name="p3946mcpsimp"></a><a name="p3946mcpsimp"></a>SS928V100</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.3.1.2 "><p id="p3948mcpsimp"><a name="p3948mcpsimp"></a><a name="p3948mcpsimp"></a>HDMI2.0</p>
</td>
</tr>
<tr id="row3949mcpsimp"><td class="cellrowborder" valign="top" width="48%" headers="mcps1.2.3.1.1 "><p id="p3951mcpsimp"><a name="p3951mcpsimp"></a><a name="p3951mcpsimp"></a>SS626V100</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.3.1.2 "><p id="p3953mcpsimp"><a name="p3953mcpsimp"></a><a name="p3953mcpsimp"></a>双路HDMI2.0</p>
</td>
</tr>
</tbody>
</table>

# API参考<a name="ZH-CN_TOPIC_0000002441693889"></a>

该功能模块提供以下MPI。

-   [ss\_mpi\_hdmi\_init](#ZH-CN_TOPIC_0000002408094734)：初始化HDMI。
-   [ss\_mpi\_hdmi\_deinit](#ZH-CN_TOPIC_0000002408094726)：去初始化HDMI。
-   [ss\_mpi\_hdmi\_open](#ZH-CN_TOPIC_0000002441654053)：打开HDMI。
-   [ss\_mpi\_hdmi\_close](#ZH-CN_TOPIC_0000002408254642)：关闭HDMI。
-   [ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)：设置HDMI属性。
-   [ss\_mpi\_hdmi\_get\_attr](#ZH-CN_TOPIC_0000002408094710)：获取HDMI属性。
-   [ss\_mpi\_hdmi\_start](#ZH-CN_TOPIC_0000002408094694)：启动HDMI输出。
-   [ss\_mpi\_hdmi\_stop](#ZH-CN_TOPIC_0000002441693837)：停止HDMI输出。
-   [ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)：获取HDMI Sink端的能力集。
-   [ss\_mpi\_hdmi\_force\_get\_edid](#ZH-CN_TOPIC_0000002441693877)：获取HDMI的EDID原始数据。
-   [ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)：注册事件回调函数
-   [ss\_mpi\_hdmi\_unregister\_callback](#ZH-CN_TOPIC_0000002408254562)：撤销事件回调函数
-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)：设置信息帧。
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)：获取信息帧。
-   [ss\_mpi\_hdmi\_set\_hw\_spec](#ZH-CN_TOPIC_0000002408254618)：设置指标参数。
-   [ss\_mpi\_hdmi\_get\_hw\_spec](#ZH-CN_TOPIC_0000002408094690)：获取指标参数。
-   [ss\_mpi\_hdmi\_set\_avmute](#ZH-CN_TOPIC_0000002441693833)：设置AVMUTE。
-   [ss\_mpi\_hdmi\_set\_mod\_param](#ZH-CN_TOPIC_0000002408094658): HDMI模块参数设置。
-   [ss\_mpi\_hdmi\_get\_mod\_param](#ZH-CN_TOPIC_0000002441653985): 获取HDMI模块参数。

## ss\_mpi\_hdmi\_init<a name="ZH-CN_TOPIC_0000002408094734"></a>

【描述】

初始化HDMI。

【语法】

```
td_s32 ss_mpi_hdmi_init(td_void);
```

【参数】

无。

【返回值】

<a name="table2614mcpsimp"></a>
<table><thead align="left"><tr id="row2619mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p2621mcpsimp"><a name="p2621mcpsimp"></a><a name="p2621mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p2623mcpsimp"><a name="p2623mcpsimp"></a><a name="p2623mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2625mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p2627mcpsimp"><a name="p2627mcpsimp"></a><a name="p2627mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p2629mcpsimp"><a name="p2629mcpsimp"></a><a name="p2629mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row2630mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p2632mcpsimp"><a name="p2632mcpsimp"></a><a name="p2632mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p2634mcpsimp"><a name="p2634mcpsimp"></a><a name="p2634mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

无

【举例】

注意：举例时未检查API返回值，建议用户使用时请检查返回值做相应处理，下同。

```
ot_hdmi_attr      st_attr;
 
/* 初始化HDMI */
ss_mpi_hdmi_init();
/* 打开HDMI */
ss_mpi_hdmi_open(OT_HDMI_ID_0);
/* 获取HDMI属性 */
ss_mpi_hdmi_get_attr(OT_HDMI_ID_0, &st_attr);
/* 设置HDMI属性 */
st_attr. enable_hdmi = TD_TRUE;
st_attr. video_format = OT_HDMI_VIDEO_FORMAT_720P_60;
st_attr. deep_color_mode = OT_HDMI_DEEP_COLOR_OFF;
st_attr. enable_audio = TD_TRUE;
st_attr. sample_rate  = OT_HDMI_SAMPLE_RATE_48K;
st_attr. bit_depth = OT_HDMI_BIT_DEPTH_16;
ss_mpi_hdmi_set_attr(OT_HDMI_ID_0, &st_attr);
/* 启动HDMI */
ss_mpi_hdmi_start(OT_HDMI_ID_0);
 
/*以下，为使用结束后退出流程*/
/* 停止HDMI */
ss_mpi_hdmi_stop(OT_HDMI_ID_0);
/* 关闭HDMI */
ss_mpi_hdmi_close(OT_HDMI_ID_0);
/* 去初始化HDMI */
ss_mpi_hdmi_deinit();
```

【相关主题】

[ss\_mpi\_hdmi\_deinit](#ZH-CN_TOPIC_0000002408094726)

## ss\_mpi\_hdmi\_deinit<a name="ZH-CN_TOPIC_0000002408094726"></a>

【描述】

去初始化HDMI。

【语法】

```
td_s32 ss_mpi_hdmi_deinit(td_void);
```

【参数】

无。

【返回值】

<a name="table3397mcpsimp"></a>
<table><thead align="left"><tr id="row3402mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p3404mcpsimp"><a name="p3404mcpsimp"></a><a name="p3404mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p3406mcpsimp"><a name="p3406mcpsimp"></a><a name="p3406mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3408mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3410mcpsimp"><a name="p3410mcpsimp"></a><a name="p3410mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3412mcpsimp"><a name="p3412mcpsimp"></a><a name="p3412mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row3413mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3415mcpsimp"><a name="p3415mcpsimp"></a><a name="p3415mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3417mcpsimp"><a name="p3417mcpsimp"></a><a name="p3417mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   若已初始化成功，程序出现异常需要退出时需要调用此接口。
-   未初始化就去初始化或重复去初始化返回成功。

【举例】

请参见[ss\_mpi\_hdmi\_init](#ZH-CN_TOPIC_0000002408094734)和[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)的举例。

【相关主题】

无。

## ss\_mpi\_hdmi\_open<a name="ZH-CN_TOPIC_0000002441654053"></a>

【描述】

打开HDMI。

【语法】

```
td_s32 ss_mpi_hdmi_open(ot_hdmi_id hdmi);
```

【参数】

<a name="table6469mcpsimp"></a>
<table><thead align="left"><tr id="row6475mcpsimp"><th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.1"><p id="p6477mcpsimp"><a name="p6477mcpsimp"></a><a name="p6477mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="69.7%" id="mcps1.1.4.1.2"><p id="p6479mcpsimp"><a name="p6479mcpsimp"></a><a name="p6479mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.3"><p id="p6481mcpsimp"><a name="p6481mcpsimp"></a><a name="p6481mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row6483mcpsimp"><td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.1 "><p id="p6485mcpsimp"><a name="p6485mcpsimp"></a><a name="p6485mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="69.7%" headers="mcps1.1.4.1.2 "><p id="p6487mcpsimp"><a name="p6487mcpsimp"></a><a name="p6487mcpsimp"></a>HDMI接口号。</p>
<p id="p6488mcpsimp"><a name="p6488mcpsimp"></a><a name="p6488mcpsimp"></a>取值：0。参见<a href="#ZH-CN_TOPIC_0000002441653977">ot_hdmi_id</a>保留将来扩展多个HDMI设备支持，下同。</p>
</td>
<td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.3 "><p id="p6491mcpsimp"><a name="p6491mcpsimp"></a><a name="p6491mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table6493mcpsimp"></a>
<table><thead align="left"><tr id="row6498mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p6500mcpsimp"><a name="p6500mcpsimp"></a><a name="p6500mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p6502mcpsimp"><a name="p6502mcpsimp"></a><a name="p6502mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6504mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p6506mcpsimp"><a name="p6506mcpsimp"></a><a name="p6506mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p6508mcpsimp"><a name="p6508mcpsimp"></a><a name="p6508mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row6509mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p6511mcpsimp"><a name="p6511mcpsimp"></a><a name="p6511mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p6513mcpsimp"><a name="p6513mcpsimp"></a><a name="p6513mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   ss\_mpi\_hdmi\_open之前须确保HDMI已被初始化，否则将返回[OT\_ERR\_HDMI\_NOT\_INIT](#OT_ERR_HDMI_NOT_INIT)。
-   重复打开HDMI返回成功。
-   调用该接口时，如果处于未接入hdmi线的状态下，该接口调度耗时相对较长（正常1\~3s）。如果是正常接hdmi线且sink端状态正常，该接口耗时为1s左右。

【举例】

请参见[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)的举例。

【相关主题】

[ss\_mpi\_hdmi\_close](#ZH-CN_TOPIC_0000002408254642)

## ss\_mpi\_hdmi\_close<a name="ZH-CN_TOPIC_0000002408254642"></a>

【描述】

关闭HDMI。

【语法】

```
td_s32 ss_mpi_hdmi_close(ot_hdmi_id hdmi);
```

【参数】

<a name="table6397mcpsimp"></a>
<table><thead align="left"><tr id="row6403mcpsimp"><th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.1"><p id="p6405mcpsimp"><a name="p6405mcpsimp"></a><a name="p6405mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="69.7%" id="mcps1.1.4.1.2"><p id="p6407mcpsimp"><a name="p6407mcpsimp"></a><a name="p6407mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.3"><p id="p6409mcpsimp"><a name="p6409mcpsimp"></a><a name="p6409mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row6411mcpsimp"><td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.1 "><p id="p6413mcpsimp"><a name="p6413mcpsimp"></a><a name="p6413mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="69.7%" headers="mcps1.1.4.1.2 "><p id="p6415mcpsimp"><a name="p6415mcpsimp"></a><a name="p6415mcpsimp"></a>HDMI接口号。</p>
<p id="p6416mcpsimp"><a name="p6416mcpsimp"></a><a name="p6416mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.3 "><p id="p6418mcpsimp"><a name="p6418mcpsimp"></a><a name="p6418mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table6420mcpsimp"></a>
<table><thead align="left"><tr id="row6425mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p6427mcpsimp"><a name="p6427mcpsimp"></a><a name="p6427mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p6429mcpsimp"><a name="p6429mcpsimp"></a><a name="p6429mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6431mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p6433mcpsimp"><a name="p6433mcpsimp"></a><a name="p6433mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p6435mcpsimp"><a name="p6435mcpsimp"></a><a name="p6435mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row6436mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p6438mcpsimp"><a name="p6438mcpsimp"></a><a name="p6438mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p6440mcpsimp"><a name="p6440mcpsimp"></a><a name="p6440mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   HDMI open成功之后，若应用发生异常，需要退出，需要调用此接口及[ss\_mpi\_hdmi\_deinit](#ZH-CN_TOPIC_0000002408094726)释放HDMI资源。

【举例】

请参见[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)的举例。

【相关主题】

[ss\_mpi\_hdmi\_open](#ZH-CN_TOPIC_0000002441654053)

## ss\_mpi\_hdmi\_set\_attr<a name="ZH-CN_TOPIC_0000002441693897"></a>

【描述】

设置HDMI属性。

【语法】

```
td_s32 ss_mpi_hdmi_set_attr(ot_hdmi_id hdmi, const ot_hdmi_attr *attr);
```

【参数】

<a name="table3511mcpsimp"></a>
<table><thead align="left"><tr id="row3517mcpsimp"><th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.1"><p id="p3519mcpsimp"><a name="p3519mcpsimp"></a><a name="p3519mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="69.7%" id="mcps1.1.4.1.2"><p id="p3521mcpsimp"><a name="p3521mcpsimp"></a><a name="p3521mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.3"><p id="p3523mcpsimp"><a name="p3523mcpsimp"></a><a name="p3523mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row3525mcpsimp"><td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.1 "><p id="p3527mcpsimp"><a name="p3527mcpsimp"></a><a name="p3527mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="69.7%" headers="mcps1.1.4.1.2 "><p id="p3529mcpsimp"><a name="p3529mcpsimp"></a><a name="p3529mcpsimp"></a>HDMI接口号。</p>
<p id="p3530mcpsimp"><a name="p3530mcpsimp"></a><a name="p3530mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.3 "><p id="p3532mcpsimp"><a name="p3532mcpsimp"></a><a name="p3532mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row3533mcpsimp"><td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.1 "><p id="p3535mcpsimp"><a name="p3535mcpsimp"></a><a name="p3535mcpsimp"></a>attr</p>
</td>
<td class="cellrowborder" valign="top" width="69.7%" headers="mcps1.1.4.1.2 "><p id="p3537mcpsimp"><a name="p3537mcpsimp"></a><a name="p3537mcpsimp"></a>HDMI属性结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.3 "><p id="p3539mcpsimp"><a name="p3539mcpsimp"></a><a name="p3539mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table3541mcpsimp"></a>
<table><thead align="left"><tr id="row3546mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p3548mcpsimp"><a name="p3548mcpsimp"></a><a name="p3548mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p3550mcpsimp"><a name="p3550mcpsimp"></a><a name="p3550mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3552mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3554mcpsimp"><a name="p3554mcpsimp"></a><a name="p3554mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3556mcpsimp"><a name="p3556mcpsimp"></a><a name="p3556mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row3557mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3559mcpsimp"><a name="p3559mcpsimp"></a><a name="p3559mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3561mcpsimp"><a name="p3561mcpsimp"></a><a name="p3561mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   用户需在启动之前设置HDMI属性；若HDMI已启动，则应先停止HDMI，设置属性后再重新启动。不遵循该流程使用的行为是未定义的。
-   部分属性SSxx中暂时不支持，见数据类型[ot\_hdmi\_attr](#ZH-CN_TOPIC_0000002408094686)说明。
-   若只设置部分属性，设置前应先获取属性，赋值该部分属性后再设置。
-   部分显示设备的EDID可能不够准确，如有的显示设备具备显示4kP60的能力，但其EDID中申明并不支持4kP60和SCDC，此时调用此接口设置4kP60输出时，为了避免引入兼容性问题，HDMI驱动不会强制设置4kP60输出，电视不会正常显示（串口会有如下提示：sink is not support scdc）。针对类似情况，若确定显示设备支持4kP60，可将接口参数中auth\_mode设置为TD\_TRUE，HDMI驱动将强制按照4kP60输出。但存在引入各种兼容性问题的风险，如电视花屏、闪屏，甚至死机等。
-   SS625V100、SS524V100、SS522V101只支持到HDMI1.4，属性中的video\_format取值最大只能到OT\_HDMI\_VIDEO\_FORMAT\_3840X2160P\_30。
-   此接口为同步接口，在调用过程中，驱动可能会通过SCDC与显示设备交互，此过程耗时较长（正常约3\~5s）。
-   设置属性前应用必须参考对端能力，特殊情况如对端不支持HDMI时必须先调用VDP的mpi接口[ss\_mpi\_vo\_set\_hdmi\_param](#ss_mpi_vo_set_hdmi_param)将HDMI的color space设置成RGB，然后再通过本接口将模式设置成DVI模式，否则驱动会有错误打印。
-   关于VDP的[ss\_mpi\_vo\_set\_hdmi\_param](#ss_mpi_vo_set_hdmi_param)，使用时需要先调用HDMI的[ss\_mpi\_hdmi\_stop](#ZH-CN_TOPIC_0000002441693837)接口将HDMI的信号关断然后再调用，设置完成后需再调用HDMI的[ss\_mpi\_hdmi\_start](#ZH-CN_TOPIC_0000002408094694)接口打开信号输出。
-   在调用此接口前一定要先调用[ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)接口获取对端的能力然后合理的配置属性，特别的情况如对端不支持HDMI时需要先调用VO的[ss\_mpi\_vo\_set\_hdmi\_param](#ss_mpi_vo_set_hdmi_param)接口将颜色空间配置成RGB，然后才能调用此接口来配置HDMI的其他属性，否则驱动会有报错。
-   有开机画面且非平滑过渡的场景下，应用上电后第一次设置属性前一定要先调用一次[ss\_mpi\_hdmi\_stop](#ZH-CN_TOPIC_0000002441693837)，否则可能会导致兼容性问题。

备注：当auth\_mode设置为TD\_TRUE时驱动会强制SCDC操作，若显示设备不支持或不插HDMI线，此接口耗时可能更长。

【举例】

请参见[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)的举例。

【相关主题】

[ss\_mpi\_hdmi\_get\_attr](#ZH-CN_TOPIC_0000002408094710)

## ss\_mpi\_hdmi\_get\_attr<a name="ZH-CN_TOPIC_0000002408094710"></a>

【描述】

获取HDMI属性。

【语法】

```
td_s32 ss_mpi_hdmi_get_attr(ot_hdmi_id hdmi, ot_hdmi_attr *attr);
```

【参数】

<a name="table5234mcpsimp"></a>
<table><thead align="left"><tr id="row5240mcpsimp"><th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.1"><p id="p5242mcpsimp"><a name="p5242mcpsimp"></a><a name="p5242mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="69.7%" id="mcps1.1.4.1.2"><p id="p5244mcpsimp"><a name="p5244mcpsimp"></a><a name="p5244mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.3"><p id="p5246mcpsimp"><a name="p5246mcpsimp"></a><a name="p5246mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row5248mcpsimp"><td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.1 "><p id="p5250mcpsimp"><a name="p5250mcpsimp"></a><a name="p5250mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="69.7%" headers="mcps1.1.4.1.2 "><p id="p5252mcpsimp"><a name="p5252mcpsimp"></a><a name="p5252mcpsimp"></a>HDMI接口号。</p>
<p id="p5253mcpsimp"><a name="p5253mcpsimp"></a><a name="p5253mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.3 "><p id="p5255mcpsimp"><a name="p5255mcpsimp"></a><a name="p5255mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row5256mcpsimp"><td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.1 "><p id="p5258mcpsimp"><a name="p5258mcpsimp"></a><a name="p5258mcpsimp"></a>attr</p>
</td>
<td class="cellrowborder" valign="top" width="69.7%" headers="mcps1.1.4.1.2 "><p id="p5260mcpsimp"><a name="p5260mcpsimp"></a><a name="p5260mcpsimp"></a>HDMI属性结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.3 "><p id="p5262mcpsimp"><a name="p5262mcpsimp"></a><a name="p5262mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table5264mcpsimp"></a>
<table><thead align="left"><tr id="row5269mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p5271mcpsimp"><a name="p5271mcpsimp"></a><a name="p5271mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p5273mcpsimp"><a name="p5273mcpsimp"></a><a name="p5273mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5275mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p5277mcpsimp"><a name="p5277mcpsimp"></a><a name="p5277mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p5279mcpsimp"><a name="p5279mcpsimp"></a><a name="p5279mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row5280mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p5282mcpsimp"><a name="p5282mcpsimp"></a><a name="p5282mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p5284mcpsimp"><a name="p5284mcpsimp"></a><a name="p5284mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   若只设置部分属性，设置前应先获取属性，赋值需要修改的属性后再设置。

【举例】

请参见[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)的举例。

【相关主题】

[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)

## ss\_mpi\_hdmi\_start<a name="ZH-CN_TOPIC_0000002408094694"></a>

【描述】

启动HDMI输出。

【语法】

```
td_s32 ss_mpi_hdmi_start(ot_hdmi_id hdmi);
```

【参数】

<a name="table588mcpsimp"></a>
<table><thead align="left"><tr id="row594mcpsimp"><th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.1"><p id="p596mcpsimp"><a name="p596mcpsimp"></a><a name="p596mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="69.7%" id="mcps1.1.4.1.2"><p id="p598mcpsimp"><a name="p598mcpsimp"></a><a name="p598mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.3"><p id="p600mcpsimp"><a name="p600mcpsimp"></a><a name="p600mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row602mcpsimp"><td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.1 "><p id="p604mcpsimp"><a name="p604mcpsimp"></a><a name="p604mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="69.7%" headers="mcps1.1.4.1.2 "><p id="p606mcpsimp"><a name="p606mcpsimp"></a><a name="p606mcpsimp"></a>HDMI接口号。</p>
<p id="p607mcpsimp"><a name="p607mcpsimp"></a><a name="p607mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.3 "><p id="p609mcpsimp"><a name="p609mcpsimp"></a><a name="p609mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table611mcpsimp"></a>
<table><thead align="left"><tr id="row616mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p618mcpsimp"><a name="p618mcpsimp"></a><a name="p618mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p620mcpsimp"><a name="p620mcpsimp"></a><a name="p620mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row622mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p624mcpsimp"><a name="p624mcpsimp"></a><a name="p624mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p626mcpsimp"><a name="p626mcpsimp"></a><a name="p626mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row627mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p629mcpsimp"><a name="p629mcpsimp"></a><a name="p629mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p631mcpsimp"><a name="p631mcpsimp"></a><a name="p631mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   调用该接口时输出4K60且sink端能力支持，此时该接口耗时会在1s左右。如果输出4K30及以下分辨率，该接口耗时会在1s以内。

【举例】

请参见[ss\_mpi\_hdmi\_init](#ZH-CN_TOPIC_0000002408094734)和[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)的举例。

【相关主题】

[ss\_mpi\_hdmi\_stop](#ZH-CN_TOPIC_0000002441693837)

## ss\_mpi\_hdmi\_stop<a name="ZH-CN_TOPIC_0000002441693837"></a>

【描述】

停止HDMI输出。

【语法】

```
td_s32 ss_mpi_hdmi_stop(ot_hdmi_id hdmi);
```

【参数】

<a name="table6213mcpsimp"></a>
<table><thead align="left"><tr id="row6219mcpsimp"><th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.1"><p id="p6221mcpsimp"><a name="p6221mcpsimp"></a><a name="p6221mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="69.7%" id="mcps1.1.4.1.2"><p id="p6223mcpsimp"><a name="p6223mcpsimp"></a><a name="p6223mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15.150000000000002%" id="mcps1.1.4.1.3"><p id="p6225mcpsimp"><a name="p6225mcpsimp"></a><a name="p6225mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row6227mcpsimp"><td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.1 "><p id="p6229mcpsimp"><a name="p6229mcpsimp"></a><a name="p6229mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="69.7%" headers="mcps1.1.4.1.2 "><p id="p6231mcpsimp"><a name="p6231mcpsimp"></a><a name="p6231mcpsimp"></a>HDMI接口号。</p>
<p id="p6232mcpsimp"><a name="p6232mcpsimp"></a><a name="p6232mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15.150000000000002%" headers="mcps1.1.4.1.3 "><p id="p6234mcpsimp"><a name="p6234mcpsimp"></a><a name="p6234mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table6236mcpsimp"></a>
<table><thead align="left"><tr id="row6241mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p6243mcpsimp"><a name="p6243mcpsimp"></a><a name="p6243mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p6245mcpsimp"><a name="p6245mcpsimp"></a><a name="p6245mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6247mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p6249mcpsimp"><a name="p6249mcpsimp"></a><a name="p6249mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p6251mcpsimp"><a name="p6251mcpsimp"></a><a name="p6251mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row6252mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p6254mcpsimp"><a name="p6254mcpsimp"></a><a name="p6254mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p6256mcpsimp"><a name="p6256mcpsimp"></a><a name="p6256mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。

【举例】

请参见[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)的举例。

【相关主题】

无

## ss\_mpi\_hdmi\_get\_sink\_capability<a name="ZH-CN_TOPIC_0000002408254566"></a>

【描述】

获取HDMI Sink端的能力集。

【语法】

```
td_s32 ss_mpi_hdmi_get_sink_capability(ot_hdmi_id hdmi, ot_hdmi_sink_capability *capability);
```

【参数】

<a name="table2875mcpsimp"></a>
<table><thead align="left"><tr id="row2881mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.4.1.1"><p id="p2883mcpsimp"><a name="p2883mcpsimp"></a><a name="p2883mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="65%" id="mcps1.1.4.1.2"><p id="p2885mcpsimp"><a name="p2885mcpsimp"></a><a name="p2885mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p2887mcpsimp"><a name="p2887mcpsimp"></a><a name="p2887mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row2889mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p2891mcpsimp"><a name="p2891mcpsimp"></a><a name="p2891mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.4.1.2 "><p id="p2893mcpsimp"><a name="p2893mcpsimp"></a><a name="p2893mcpsimp"></a>HDMI接口号。</p>
<p id="p2894mcpsimp"><a name="p2894mcpsimp"></a><a name="p2894mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p2896mcpsimp"><a name="p2896mcpsimp"></a><a name="p2896mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row2897mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p2899mcpsimp"><a name="p2899mcpsimp"></a><a name="p2899mcpsimp"></a>capability</p>
</td>
<td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.4.1.2 "><p id="p2901mcpsimp"><a name="p2901mcpsimp"></a><a name="p2901mcpsimp"></a>HDMI Sink端能力集结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p2903mcpsimp"><a name="p2903mcpsimp"></a><a name="p2903mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table2905mcpsimp"></a>
<table><thead align="left"><tr id="row2910mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p2912mcpsimp"><a name="p2912mcpsimp"></a><a name="p2912mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p2914mcpsimp"><a name="p2914mcpsimp"></a><a name="p2914mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2916mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p2918mcpsimp"><a name="p2918mcpsimp"></a><a name="p2918mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p2920mcpsimp"><a name="p2920mcpsimp"></a><a name="p2920mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row2921mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p2923mcpsimp"><a name="p2923mcpsimp"></a><a name="p2923mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p2925mcpsimp"><a name="p2925mcpsimp"></a><a name="p2925mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   应在HDMI启动且插入线缆之后调用。
-   驱动会在应用调用[ss\_mpi\_hdmi\_open](#ZH-CN_TOPIC_0000002441654053)后读取显示设备的EDID，读取和解析过程需要一定时间。因此，若应用注册HDMI事件回调，则建议在检测到插入事件后调用；若应用不注册回调，则在open HDMI后延时1\~2秒再调用此接口（若不延时，则可能出现获取失败）。

【举例】

```
ot_hdmi_edid st_edid_data;
ot_hdmi_sink_capability st_sink_cap;
/* 初始化HDMI */
ss_mpi_hdmi_init();
/* 打开HDMI */
ss_mpi_hdmi_open(OT_HDMI_ID_0);
/*设置属性*/
…
/* 启动HDMI */
ss_mpi_hdmi_start(OT_HDMI_ID_0);
…
sleep(2);
/* 获取EDID */
ss_mpi_hdmi_force_get_edid(OT_HDMI_ID_0, &st_edid_data);
/*获取能力集合*/
ss_mpi_hdmi_get_sink_capability(OT_HDMI_ID_0, &st_sink_cap);
```

【相关主题】

[ss\_mpi\_hdmi\_force\_get\_edid](#ZH-CN_TOPIC_0000002441693877)

## ss\_mpi\_hdmi\_force\_get\_edid<a name="ZH-CN_TOPIC_0000002441693877"></a>

【描述】

获取HDMI的EDID原始数据。

【语法】

```
td_s32 ss_mpi_hdmi_force_get_edid(ot_hdmi_id hdmi, ot_hdmi_edid *edid_data);
```

【参数】

<a name="table6320mcpsimp"></a>
<table><thead align="left"><tr id="row6326mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.4.1.1"><p id="p6328mcpsimp"><a name="p6328mcpsimp"></a><a name="p6328mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.4.1.2"><p id="p6330mcpsimp"><a name="p6330mcpsimp"></a><a name="p6330mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="18%" id="mcps1.1.4.1.3"><p id="p6332mcpsimp"><a name="p6332mcpsimp"></a><a name="p6332mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row6334mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p6336mcpsimp"><a name="p6336mcpsimp"></a><a name="p6336mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.4.1.2 "><p id="p6338mcpsimp"><a name="p6338mcpsimp"></a><a name="p6338mcpsimp"></a>HDMI接口号。</p>
<p id="p6339mcpsimp"><a name="p6339mcpsimp"></a><a name="p6339mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.4.1.3 "><p id="p6341mcpsimp"><a name="p6341mcpsimp"></a><a name="p6341mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row6342mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p6344mcpsimp"><a name="p6344mcpsimp"></a><a name="p6344mcpsimp"></a>edid_data</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.4.1.2 "><p id="p6346mcpsimp"><a name="p6346mcpsimp"></a><a name="p6346mcpsimp"></a>HDMI的EDID信息。</p>
</td>
<td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.4.1.3 "><p id="p6348mcpsimp"><a name="p6348mcpsimp"></a><a name="p6348mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table6350mcpsimp"></a>
<table><thead align="left"><tr id="row6355mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p6357mcpsimp"><a name="p6357mcpsimp"></a><a name="p6357mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p6359mcpsimp"><a name="p6359mcpsimp"></a><a name="p6359mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6361mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p6363mcpsimp"><a name="p6363mcpsimp"></a><a name="p6363mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p6365mcpsimp"><a name="p6365mcpsimp"></a><a name="p6365mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row6366mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p6368mcpsimp"><a name="p6368mcpsimp"></a><a name="p6368mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p6370mcpsimp"><a name="p6370mcpsimp"></a><a name="p6370mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   HDMI内部在线缆插入后已从Sink获取EDID。该API为强制获取EDID，一般情况下不需使用。

【举例】

请参见[ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)。

【相关主题】

[ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)

## ss\_mpi\_hdmi\_register\_callback<a name="ZH-CN_TOPIC_0000002408094654"></a>

【描述】

注册HDMI事件回调函数。

【语法】

```
td_s32 ss_mpi_hdmi_register_callback(ot_hdmi_id hdmi, const ot_hdmi_callback_func *callback_func);
```

【参数】

<a name="table880mcpsimp"></a>
<table><thead align="left"><tr id="row886mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.4.1.1"><p id="p888mcpsimp"><a name="p888mcpsimp"></a><a name="p888mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="65%" id="mcps1.1.4.1.2"><p id="p890mcpsimp"><a name="p890mcpsimp"></a><a name="p890mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p892mcpsimp"><a name="p892mcpsimp"></a><a name="p892mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row894mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p896mcpsimp"><a name="p896mcpsimp"></a><a name="p896mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.4.1.2 "><p id="p898mcpsimp"><a name="p898mcpsimp"></a><a name="p898mcpsimp"></a>HDMI接口号。</p>
<p id="p899mcpsimp"><a name="p899mcpsimp"></a><a name="p899mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p901mcpsimp"><a name="p901mcpsimp"></a><a name="p901mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row902mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p904mcpsimp"><a name="p904mcpsimp"></a><a name="p904mcpsimp"></a>callback_func</p>
</td>
<td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.4.1.2 "><p id="p906mcpsimp"><a name="p906mcpsimp"></a><a name="p906mcpsimp"></a>HDMI 回调函数结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p908mcpsimp"><a name="p908mcpsimp"></a><a name="p908mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table910mcpsimp"></a>
<table><thead align="left"><tr id="row915mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p917mcpsimp"><a name="p917mcpsimp"></a><a name="p917mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p919mcpsimp"><a name="p919mcpsimp"></a><a name="p919mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row921mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p923mcpsimp"><a name="p923mcpsimp"></a><a name="p923mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p925mcpsimp"><a name="p925mcpsimp"></a><a name="p925mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row926mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p928mcpsimp"><a name="p928mcpsimp"></a><a name="p928mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p930mcpsimp"><a name="p930mcpsimp"></a><a name="p930mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   建议用户注册HDMI事件回调函数。例如，当产生热插拔事件时，可以通过注册的回调函数读取热插拔后产生的能力集信息为依据更改HDMI的属性，然后重新启动HDMI，使HDMI的属性适配新插入的对端显示器/电视。若用户不注册事件回调函数，则当事件产生时，HDMI内部会采取默认的处理方式。
-   若用户注册了事件回调函数，则退出HDMI前应调用[ss\_mpi\_hdmi\_unregister\_callback](#ZH-CN_TOPIC_0000002408254562)注销该回调函数，否则会有内存泄漏的问题。
-   若用户没有注册回调函数，则驱动报的事件会走内核回调处理。
-   内核事件回调函数只在用户start HDMI之后才能正常响应拔插事件。
-   内核回调函数中若事件为HPD事件驱动会自动start HDMI，若事件为UNHPD事件驱动会自动stop HDMI。
-   重复注册同一回调函数与参数将返回[OT\_ERR\_HDMI\_CALLBACK\_ALREADY](#OT_HDMI_CALLBACK_ALREADY)。
-   建议用户最多同时注册10个不同的回调函数，注册超过10个的回调函数将会影响之前注册的回调函数使用。
-   若同时注册了多个不同的事件回调函数，当一个事件产生时，HDMI会按事件回调函数注册先后顺序，遍历事件池，响应用户注册的回调函数。

【举例】

```
#define HDMI_PRINT       printf("[HDMI]%s[%d]:\t",__func__,__LINE__);printf
#define HDMI_CHK_FAILURE_NORET(s32_ret) do{\
   if(s32_ret!=TD_SUCCESS)\
   {\
      HDMI_PRINT("s32_ret=%d is not expected TD_SUCCESS!\n",s32_ret);\
   }\
}while(0);
#define HDMI_CHK_FAILURE_RET(s32_ret) do{\
   if(s32_ret!=TD_SUCCESS)\
   {\
      HDMI_PRINT("s32_ret=%d is not expected TD_SUCCESS!\n",s32_ret);\
      return TD_FAILURE;\
   }\
}while(0);
#define HDMI_CHK_FAILURE_GOTO(res,lable) do{\
   if(TD_FAILURE==res)\
   {HDMI_PRINT("return failure!\n");goto lable;}\
}while(0);
typedef struct hdmi_args_s
{
   OT_HDMI_ID_E          en_hdmi;
   OT_HDMI_VIDEO_FORMAT_E e_force_format;
}hdmi_args_s;
static ot_s32 hdmi_unplug_proc(TD_VOID *p_private_data)
{
   ot_s32          s32_ret = TD_SUCCESS;
   hdmi_args_s     *p_args  = (hdmi_args_s *)p_private_data;
   ot_hdmi_id    h_hdmi   =  p_args->en_hdmi;
   HDMI_PRINT("\n --- unplug event handling. --- \n");
   s32_ret = ss_mpi_hdmi_stop(h_hdmi);
   HDMI_CHK_FAILURE_RET(s32_ret);
   return s32_ret;
}
static  ot_s32 hdmi_hot_plug_proc(TD_VOID *p_private_data)
{
   ot_s32                    s32_ret = TD_SUCCESS;
   hdmi_args_s    *p_args  = (hdmi_args_s *)p_private_data;
   ot_hdmi_id                h_hdmi   =  p_args->en_hdmi;
   ot_hdmi_attr              st_hdmi_attr;
   ot_hdmi_sink_capability   st_sink_cap;
   HDMI_PRINT("\n --- hotplug event handling --- \n");
   s32_ret = ss_mpi_hdmi_get_attr (h_hdmi, &st_hdmi_attr);
   HDMI_CHK_FAILURE_RET(s32_ret);
   s32_ret = ss_mpi_hdmi_get_sink_capability (h_hdmi, &st_sink_cap);
   HDMI_CHK_FAILURE_RET(s32_ret);
   if (TD_FALSE == st_sink_cap.connected )
   {
      HDMI_PRINT("st_sink_cap.connected is TD_FALSE!\n");
      return TD_FAILURE;
   }
   if(TD_TRUE == st_sink_cap.support_hdmi)
   {
      st_hdmi_attr.enable_hdmi = TD_TRUE;
   }
   else
   {
      st_hdmi_attr.enable_hdmi = TD_FALSE;
   }
   if(TD_TRUE == st_hdmi_attr.enable_hdmi)
   {
       st_hdmi_attr.enable_hdmi = TD_TRUE;
       st_hdmi_attr.deep_color_mode = OT_HDMI_DEEP_COLOR_OFF;
       st_hdmi_attr.enable_audio = TD_TRUE;
       st_hdmi_attr.bit_depth = OT_HDMI_BIT_DEPTH_16;
   }
   else
   {
      st_hdmi_attr.enable_hdmi = TD_FALSE;
      st_hdmi_attr.enable_audio = TD_FALSE;
   }
   if (p_args->e_force_format >= OT_HDMI_VIDEO_FORMAT_1080P_60
       &&   p_args->e_force_format < OT_HDMI_VIDEO_FORMAT_BUTT 
       &&   st_sink_cap.video_format_supported[p_args->e_force_format] )
   {
        HDMI_PRINT("set force format=%d\n",p_args->e_force_format);
        st_hdmi_attr.video_format = p_args->e_force_format;
   }
   else 
   {
        HDMI_PRINT("not support expected format=%d, we set native format=%d\n",p_args->e_force_format,st_sink_cap. native_video_format);
        st_hdmi_attr. video_format= st_sink_cap. native_video_format;
   }
         
   s32_ret = ss_mpi_hdmi_set_attr (h_hdmi, &st_hdmi_attr);
   HDMI_CHK_FAILURE_RET(s32_ret);
 
   /* ss_mpi_hdmi_set_attr must before ss_mpi_hdmi_start! */
   s32_ret = ss_mpi_hdmi_start(h_hdmi);
   HDMI_CHK_FAILURE_RET(s32_ret);
   return s32_ret;
}
 
TD_VOID hdmi_event_proc(ot_hdmi_event_type event, TD_VOID *p_private_data)
{
    switch (event)
    {
     case OT_HDMI_EVENT_HOTPLUG:
        hdmi_hot_plug_proc(p_private_data);
        break;
     case OT_HDMI_EVENT_NO_PLUG:
        hdmi_unplug_proc(p_private_data);
        break;
     case OT_HDMI_EVENT_EDID_FAIL:
        break;
     default:
        HDMI_PRINT("un-known event:%d\n",event);
        return;
    }
    return;
}
ot_hdmi_callback_func g_st_callback_func;
hdmi_args_s g_st_hdmi_args;
/* 初始化HDMI流程 */
 ot_s32 ss_adp_hdmi_init(ot_hdmi_id en_hdmi_id, OT_HDMI_VIDEO_FORMAT_E e_force_format)
{
   ot_s32 s32_ret = TD_FAILURE;
   s32_ret = ss_mpi_hdmi_init();
   HDMI_CHK_FAILURE_RET(s32_ret);
 
   g_st_hdmi_args.en_hdmi                 = en_hdmi_id;
   g_st_hdmi_args.e_force_format          = e_force_format;
 
   g_st_callback_func.pfn_hdmi_event_callback = hdmi_event_proc;
   g_st_callback_func.private_data = &g_st_hdmi_args;
   s32_ret = ss_mpi_hdmi_open(g_st_hdmi_args.en_hdmi);
   HDMI_CHK_FAILURE_GOTO(s32_ret, ERROR2);
   s32_ret = ss_mpi_hdmi_register_callback(g_st_hdmi_args.en_hdmi, &g_st_callback_func);
   HDMI_CHK_FAILURE_GOTO(s32_ret, ERROR1);
   return TD_SUCCESS;
 
   ERROR1:
   s32_ret |= ss_mpi_hdmi_close(g_st_hdmi_args.en_hdmi);
 
   ERROR2:
   s32_ret |= ss_mpi_hdmi_deinit();
   return s32_ret;
}
 
/* 退出HDMI流程 */
 ot_s32 ss_adp_hdmi_deinit(ot_hdmi_id en_hdmi_id)
{
   ot_s32 s32_ret = TD_FAILURE;
   s32_ret = ss_mpi_hdmi_stop(en_hdmi_id);
   HDMI_CHK_FAILURE_NORET(s32_ret);
   g_st_hdmi_args.en_hdmi= en_hdmi_id;
   g_st_callback_func.pfn_hdmi_event_callback = hdmi_event_proc;
   g_st_callback_func.private_data = &g_st_hdmi_args;
   s32_ret = ss_mpi_hdmi_unregister_callback(en_hdmi_id, &g_st_callback_func);
   HDMI_CHK_FAILURE_NORET(s32_ret);
   s32_ret = ss_mpi_hdmi_close(en_hdmi_id);
   HDMI_CHK_FAILURE_NORET(s32_ret);
   s32_ret = ss_mpi_hdmi_deinit();
   HDMI_CHK_FAILURE_NORET(s32_ret);
   return s32_ret;
}
```

【相关主题】

[ss\_mpi\_hdmi\_unregister\_callback](#ZH-CN_TOPIC_0000002408254562)

## ss\_mpi\_hdmi\_unregister\_callback<a name="ZH-CN_TOPIC_0000002408254562"></a>

【描述】

撤销HDMI事件回调函数。

【语法】

```
td_s32 ss_mpi_hdmi_unregister_callback(ot_hdmi_id hdmi, const ot_hdmi_callback_func *callback_func);
```

【参数】

<a name="table3810mcpsimp"></a>
<table><thead align="left"><tr id="row3816mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.4.1.1"><p id="p3818mcpsimp"><a name="p3818mcpsimp"></a><a name="p3818mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="65%" id="mcps1.1.4.1.2"><p id="p3820mcpsimp"><a name="p3820mcpsimp"></a><a name="p3820mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p3822mcpsimp"><a name="p3822mcpsimp"></a><a name="p3822mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row3824mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p3826mcpsimp"><a name="p3826mcpsimp"></a><a name="p3826mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.4.1.2 "><p id="p3828mcpsimp"><a name="p3828mcpsimp"></a><a name="p3828mcpsimp"></a>HDMI接口号。</p>
<p id="p3829mcpsimp"><a name="p3829mcpsimp"></a><a name="p3829mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p3831mcpsimp"><a name="p3831mcpsimp"></a><a name="p3831mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row3832mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p3834mcpsimp"><a name="p3834mcpsimp"></a><a name="p3834mcpsimp"></a>callback_func</p>
</td>
<td class="cellrowborder" valign="top" width="65%" headers="mcps1.1.4.1.2 "><p id="p3836mcpsimp"><a name="p3836mcpsimp"></a><a name="p3836mcpsimp"></a>HDMI 回调函数结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p3838mcpsimp"><a name="p3838mcpsimp"></a><a name="p3838mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table3840mcpsimp"></a>
<table><thead align="left"><tr id="row3845mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p3847mcpsimp"><a name="p3847mcpsimp"></a><a name="p3847mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p3849mcpsimp"><a name="p3849mcpsimp"></a><a name="p3849mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3851mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3853mcpsimp"><a name="p3853mcpsimp"></a><a name="p3853mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3855mcpsimp"><a name="p3855mcpsimp"></a><a name="p3855mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row3856mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3858mcpsimp"><a name="p3858mcpsimp"></a><a name="p3858mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3860mcpsimp"><a name="p3860mcpsimp"></a><a name="p3860mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   若用户注册了回调函数，则退出HDMI前，应使用撤销HDMI事件回调函数。
-   重复撤销或未注册就撤销将返回[OT\_ERR\_HDMI\_CALLBACK\_NOT\_REGISTER](#OT_HDMI_CALLBACK_NOT_REGISTER)。

【举例】

见[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)。

【相关主题】

[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)

## ss\_mpi\_hdmi\_set\_infoframe<a name="ZH-CN_TOPIC_0000002441693881"></a>

【描述】

设置信息帧。

【语法】

```
td_s32 ss_mpi_hdmi_set_infoframe(ot_hdmi_id hdmi, const ot_hdmi_infoframe *infoframe);
```

【参数】

<a name="table3210mcpsimp"></a>
<table><thead align="left"><tr id="row3216mcpsimp"><th class="cellrowborder" valign="top" width="18%" id="mcps1.1.4.1.1"><p id="p3218mcpsimp"><a name="p3218mcpsimp"></a><a name="p3218mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="66%" id="mcps1.1.4.1.2"><p id="p3220mcpsimp"><a name="p3220mcpsimp"></a><a name="p3220mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p3222mcpsimp"><a name="p3222mcpsimp"></a><a name="p3222mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row3224mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.4.1.1 "><p id="p3226mcpsimp"><a name="p3226mcpsimp"></a><a name="p3226mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="66%" headers="mcps1.1.4.1.2 "><p id="p3228mcpsimp"><a name="p3228mcpsimp"></a><a name="p3228mcpsimp"></a>HDMI接口号。</p>
<p id="p3229mcpsimp"><a name="p3229mcpsimp"></a><a name="p3229mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p3231mcpsimp"><a name="p3231mcpsimp"></a><a name="p3231mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row3232mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.1.4.1.1 "><p id="p3234mcpsimp"><a name="p3234mcpsimp"></a><a name="p3234mcpsimp"></a>infoframe</p>
</td>
<td class="cellrowborder" valign="top" width="66%" headers="mcps1.1.4.1.2 "><p id="p3236mcpsimp"><a name="p3236mcpsimp"></a><a name="p3236mcpsimp"></a>HDMI信息帧结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p3238mcpsimp"><a name="p3238mcpsimp"></a><a name="p3238mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table3240mcpsimp"></a>
<table><thead align="left"><tr id="row3245mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p3247mcpsimp"><a name="p3247mcpsimp"></a><a name="p3247mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p3249mcpsimp"><a name="p3249mcpsimp"></a><a name="p3249mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3251mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3253mcpsimp"><a name="p3253mcpsimp"></a><a name="p3253mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3255mcpsimp"><a name="p3255mcpsimp"></a><a name="p3255mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row3256mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3258mcpsimp"><a name="p3258mcpsimp"></a><a name="p3258mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3260mcpsimp"><a name="p3260mcpsimp"></a><a name="p3260mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   该接口属于高级接口，一般不需要调用。若用户使用，则应根据已设置的音视频相关属性（如enVideoFORMAT），及遵从《High-Definition Multimedia Interface Specification Version 1.4b》、《High-Definition Multimedia Interface Specification Version 2.0》、《CEA-861-D》与《CEA-861-F》标准基础上设置信息帧，不依据音视频属性遵从标准发送信息帧的行为是未定义的。调用此接口，可能会造成显示异常。

【举例】

```
ot_hdmi_infoframe st_infoframe;
/* 初始化HDMI */
ss_mpi_hdmi_init ();
/* 打开HDMI */
ss_mpi_hdmi_open (OT_HDMI_ID_0);
/* 设置属性 */
…
/* 启动HDMI */
ss_mpi_hdmi_start (OT_HDMI_ID_0);
…
/*设置AVI的有效宽高比*/
ss_mpi_hdmi_get_infoframe(OT_HDMI_ID_0, OT_INFOFRAME_TYPE_AVI, &st_infoframe);
st_infoframe.infoframe_type = OT_INFOFRAME_TYPE_AVI;
st_infoframe.infoframe_unit.avi_infoframe.active_aspect_ratio = OT_HDMI_PIC_ASP_RATIO_16TO9;
ss_mpi_hdmi_set_infoframe(OT_HDMI_ID_0, &st_infoframe);
```

【相关主题】

[ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ss\_mpi\_hdmi\_get\_infoframe<a name="ZH-CN_TOPIC_0000002441693885"></a>

【描述】

获取信息帧。

【语法】

```
td_s32 ss_mpi_hdmi_get_infoframe(ot_hdmi_id hdmi, ot_hdmi_infoframe_type infoframe_type, ot_hdmi_infoframe *infoframe);
```

【参数】

<a name="table2681mcpsimp"></a>
<table><thead align="left"><tr id="row2687mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.4.1.1"><p id="p2689mcpsimp"><a name="p2689mcpsimp"></a><a name="p2689mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.4.1.2"><p id="p2691mcpsimp"><a name="p2691mcpsimp"></a><a name="p2691mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p2693mcpsimp"><a name="p2693mcpsimp"></a><a name="p2693mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row2695mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p2697mcpsimp"><a name="p2697mcpsimp"></a><a name="p2697mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.4.1.2 "><p id="p2699mcpsimp"><a name="p2699mcpsimp"></a><a name="p2699mcpsimp"></a>HDMI接口号。</p>
<p id="p2700mcpsimp"><a name="p2700mcpsimp"></a><a name="p2700mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p2702mcpsimp"><a name="p2702mcpsimp"></a><a name="p2702mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row2703mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p2705mcpsimp"><a name="p2705mcpsimp"></a><a name="p2705mcpsimp"></a>infoframe_type</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.4.1.2 "><p id="p2707mcpsimp"><a name="p2707mcpsimp"></a><a name="p2707mcpsimp"></a>信息帧类型。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p2709mcpsimp"><a name="p2709mcpsimp"></a><a name="p2709mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row2710mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p2712mcpsimp"><a name="p2712mcpsimp"></a><a name="p2712mcpsimp"></a>infoframe</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.4.1.2 "><p id="p2714mcpsimp"><a name="p2714mcpsimp"></a><a name="p2714mcpsimp"></a>HDMI信息帧结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p2716mcpsimp"><a name="p2716mcpsimp"></a><a name="p2716mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table2718mcpsimp"></a>
<table><thead align="left"><tr id="row2723mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p2725mcpsimp"><a name="p2725mcpsimp"></a><a name="p2725mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p2727mcpsimp"><a name="p2727mcpsimp"></a><a name="p2727mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2729mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p2731mcpsimp"><a name="p2731mcpsimp"></a><a name="p2731mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p2733mcpsimp"><a name="p2733mcpsimp"></a><a name="p2733mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row2734mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p2736mcpsimp"><a name="p2736mcpsimp"></a><a name="p2736mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p2738mcpsimp"><a name="p2738mcpsimp"></a><a name="p2738mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   该接口属于高级接口，一般不需要调用。
-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   调用前须确保单板上电后已调用过[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)，否则通过此接口获取到的部分参数可能是非法值。

【举例】

请参考[ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)

【相关主题】

[ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)

## ss\_mpi\_hdmi\_set\_hw\_spec<a name="ZH-CN_TOPIC_0000002408254618"></a>

【描述】

设置HDMI指标参数。

【语法】

```
td_s32 ss_mpi_hdmi_set_hw_spec(ot_hdmi_id hdmi, const ot_hdmi_hw_spec *hw_spec);
```

【参数】

<a name="table4085mcpsimp"></a>
<table><thead align="left"><tr id="row4091mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p4093mcpsimp"><a name="p4093mcpsimp"></a><a name="p4093mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p4095mcpsimp"><a name="p4095mcpsimp"></a><a name="p4095mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p4097mcpsimp"><a name="p4097mcpsimp"></a><a name="p4097mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row4099mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p4101mcpsimp"><a name="p4101mcpsimp"></a><a name="p4101mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p4103mcpsimp"><a name="p4103mcpsimp"></a><a name="p4103mcpsimp"></a>HDMI接口号。</p>
<p id="p4104mcpsimp"><a name="p4104mcpsimp"></a><a name="p4104mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p4106mcpsimp"><a name="p4106mcpsimp"></a><a name="p4106mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row4107mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p4109mcpsimp"><a name="p4109mcpsimp"></a><a name="p4109mcpsimp"></a>hw_spec</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p4111mcpsimp"><a name="p4111mcpsimp"></a><a name="p4111mcpsimp"></a>HDMI指标参数结构体。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p4113mcpsimp"><a name="p4113mcpsimp"></a><a name="p4113mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table4115mcpsimp"></a>
<table><thead align="left"><tr id="row4120mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p4122mcpsimp"><a name="p4122mcpsimp"></a><a name="p4122mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p4124mcpsimp"><a name="p4124mcpsimp"></a><a name="p4124mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4126mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4128mcpsimp"><a name="p4128mcpsimp"></a><a name="p4128mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4130mcpsimp"><a name="p4130mcpsimp"></a><a name="p4130mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row4131mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p4133mcpsimp"><a name="p4133mcpsimp"></a><a name="p4133mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p4135mcpsimp"><a name="p4135mcpsimp"></a><a name="p4135mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   正常情况请勿使用该接口。若必要情况下需要设置HDMI硬件指标参数，请务必确保参数设置后HDMI电气特性符合协议标准，否则可能出现不可预知的兼容性问题。
-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   若需要设置HDMI硬件指标参数，请务必在调用[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)后调用此接口。
-   调用此接口前请务必先调用[ss\_mpi\_hdmi\_get\_hw\_spec](#ZH-CN_TOPIC_0000002408094690)获取当前所有频段的指标参数。根据实际需求调整对应频段的具体参数\(参考[ot\_hdmi\_hw\_spec](#ZH-CN_TOPIC_0000002441693849)\)。
-   根据不同分辨率，色深\(Deep Color\)以及颜色空间\(Color Space\)可划分为四个频段，如[表1](#_Ref528253079)所示。

调用该接口**调试**硬件指标时，分辨率，色深以及颜色空间必须按下表设置，其中\[25M, 100M\]TMDS时钟频段需要下表两种配置都要调试。同时必须将[ot\_hdmi\_attr](#ZH-CN_TOPIC_0000002408094686)成员auth\_mode\_en设置成TD\_TRUE。若调试结束后，**正常使用**该接口设置指标参数时，请必须将[ot\_hdmi\_attr](#ZH-CN_TOPIC_0000002408094686)成员auth\_mode\_en设置成TD\_FALSE。

**表 1**  色深\(Deep Color\)以及颜色空间\(Color Space\)频段表

<a name="_Ref528253079"></a>
<table><thead align="left"><tr id="row4170mcpsimp"><th class="cellrowborder" valign="top" width="18%" id="mcps1.2.5.1.1"><p id="p4172mcpsimp"><a name="p4172mcpsimp"></a><a name="p4172mcpsimp"></a>TMDS时钟频段</p>
</th>
<th class="cellrowborder" valign="top" width="31%" id="mcps1.2.5.1.2"><p id="p4174mcpsimp"><a name="p4174mcpsimp"></a><a name="p4174mcpsimp"></a>分辨率(video_format)</p>
</th>
<th class="cellrowborder" valign="top" width="26%" id="mcps1.2.5.1.3"><p id="p4176mcpsimp"><a name="p4176mcpsimp"></a><a name="p4176mcpsimp"></a>色深(deep_color_mode)</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p4178mcpsimp"><a name="p4178mcpsimp"></a><a name="p4178mcpsimp"></a>颜色空间(enVidOutMode)</p>
</th>
</tr>
</thead>
<tbody><tr id="row4180mcpsimp"><td class="cellrowborder" rowspan="2" valign="top" width="18%" headers="mcps1.2.5.1.1 "><p id="p4182mcpsimp"><a name="p4182mcpsimp"></a><a name="p4182mcpsimp"></a>[25M, 100M]</p>
</td>
<td class="cellrowborder" valign="top" width="31%" headers="mcps1.2.5.1.2 "><p id="p4184mcpsimp"><a name="p4184mcpsimp"></a><a name="p4184mcpsimp"></a>OT_HDMI_VIDEO_FORMAT_480P_60</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.5.1.3 "><p id="p4186mcpsimp"><a name="p4186mcpsimp"></a><a name="p4186mcpsimp"></a>OT_HDMI_DEEP_COLOR_24BIT</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p4188mcpsimp"><a name="p4188mcpsimp"></a><a name="p4188mcpsimp"></a>OT_HDMI_VIDEO_MODE_RGB444</p>
</td>
</tr>
<tr id="row4189mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4191mcpsimp"><a name="p4191mcpsimp"></a><a name="p4191mcpsimp"></a>OT_HDMI_VIDEO_FORMAT_1080P_30</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4193mcpsimp"><a name="p4193mcpsimp"></a><a name="p4193mcpsimp"></a>OT_HDMI_DEEP_COLOR_24BIT</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p4195mcpsimp"><a name="p4195mcpsimp"></a><a name="p4195mcpsimp"></a>OT_HDMI_VIDEO_MODE_RGB444</p>
</td>
</tr>
<tr id="row4196mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.5.1.1 "><p id="p4198mcpsimp"><a name="p4198mcpsimp"></a><a name="p4198mcpsimp"></a>(100M, 165M]</p>
</td>
<td class="cellrowborder" valign="top" width="31%" headers="mcps1.2.5.1.2 "><p id="p4200mcpsimp"><a name="p4200mcpsimp"></a><a name="p4200mcpsimp"></a>OT_HDMI_VIDEO_FORMAT_1080P_60</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.5.1.3 "><p id="p4202mcpsimp"><a name="p4202mcpsimp"></a><a name="p4202mcpsimp"></a>OT_HDMI_DEEP_COLOR_24BIT</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p4204mcpsimp"><a name="p4204mcpsimp"></a><a name="p4204mcpsimp"></a>OT_HDMI_VIDEO_MODE_RGB444</p>
</td>
</tr>
<tr id="row4205mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.5.1.1 "><p id="p4207mcpsimp"><a name="p4207mcpsimp"></a><a name="p4207mcpsimp"></a>(165M, 340M]</p>
</td>
<td class="cellrowborder" valign="top" width="31%" headers="mcps1.2.5.1.2 "><p id="p4209mcpsimp"><a name="p4209mcpsimp"></a><a name="p4209mcpsimp"></a>OT_HDMI_VIDEO_FORMAT_3840X2160P_30</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.5.1.3 "><p id="p4211mcpsimp"><a name="p4211mcpsimp"></a><a name="p4211mcpsimp"></a>OT_HDMI_DEEP_COLOR_24BIT</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p4213mcpsimp"><a name="p4213mcpsimp"></a><a name="p4213mcpsimp"></a>OT_HDMI_VIDEO_MODE_RGB444</p>
</td>
</tr>
<tr id="row4214mcpsimp"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.5.1.1 "><p id="p4216mcpsimp"><a name="p4216mcpsimp"></a><a name="p4216mcpsimp"></a>(340M, 600M]</p>
<p id="p4217mcpsimp"><a name="p4217mcpsimp"></a><a name="p4217mcpsimp"></a><strong id="b4218mcpsimp"><a name="b4218mcpsimp"></a><a name="b4218mcpsimp"></a>注意：只在支持HDMI2.0的情况下，需要配置</strong></p>
</td>
<td class="cellrowborder" valign="top" width="31%" headers="mcps1.2.5.1.2 "><p id="p4220mcpsimp"><a name="p4220mcpsimp"></a><a name="p4220mcpsimp"></a>OT_HDMI_VIDEO_FORMAT_3840X2160P_60</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.5.1.3 "><p id="p4222mcpsimp"><a name="p4222mcpsimp"></a><a name="p4222mcpsimp"></a>OT_HDMI_DEEP_COLOR_24BIT</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p4224mcpsimp"><a name="p4224mcpsimp"></a><a name="p4224mcpsimp"></a>OT_HDMI_VIDEO_MODE_RGB444</p>
</td>
</tr>
</tbody>
</table>

-   其他注意事项请参考[ot\_hdmi\_hw\_spec](#ZH-CN_TOPIC_0000002441693849)。
-   color space需要通过VO的ss\_mpi\_vo\_set\_hdmi\_param接口设置。此接口请参考《MPP 媒体处理软件V5.0 开发参考》。
-   如果用户有开机画面场景，且用户使用此接口自行配置了HDMI硬件指标参数，请在开机画面场景下使用相关的函数设置相同的HDMI硬件指标参数。

【举例】

```
ot_hdmi_attr      st_attr;
ot_hdmi_hw_spec  st_hw_spec;
/* 初始化HDMI */
ss_mpi_hdmi_init();
/* 打开HDMI */
ss_mpi_hdmi_open(OT_HDMI_ID_0);
/* 获取HDMI属性 */
ss_mpi_hdmi_get_attr(OT_HDMI_ID_0, &st_attr);
/* 设置HDMI属性 */
st_attr.video_format = OT_HDMI_VIDEO_FORMAT_720P_60;
st_attr.deep_color_mode = OT_HDMI_DEEP_COLOR_24BIT;
st_attr.audio_en = TD_TRUE;
st_attr.sample_rate  = OT_HDMI_SAMPLE_RATE_48K;
st_attr.bit_depth = OT_HDMI_BIT_DEPTH_16;
ss_mpi_hdmi_set_attr(OT_HDMI_ID_0, &st_attr);
 
//获取HDMI硬件指标参数
ss_mpi_hdmi_get_hw_spec(OT_HDMI_ID_0, &st_hw_spec);
//根据需求配置TMDS时钟频段[25M, 100M]的硬件指标参数, 若不需要配置请勿修改。
st_hw_spec.hw_param[0].i_de_main_clk = 18;
st_hw_spec.hw_param[0].i_de_main_data = 18;
st_hw_spec.hw_param[0].i_main_clk= 28;
st_hw_spec.hw_param[0].i_main_data= 29;
//根据需求配置TMDS时钟频段(100M, 165M]的硬件指标参数, 若不需要配置请勿修改。
st_hw_spec.hw_param[1].i_de_main_clk = 19;
st_hw_spec.hw_param[1].i_de_main_data = 19;
st_hw_spec.hw_param[1].i_main_clk= 30;
st_hw_spec.hw_param[1].i_main_data= 31;
//根据需求配置TMDS时钟频段(165M, 340M]的硬件指标参数, 若不需要配置请勿修改。
st_hw_spec.hw_param[2].i_de_main_clk = 20;
st_hw_spec.hw_param[2].i_de_main_data = 20;
st_hw_spec.hw_param[2].i_main_clk= 32;
st_hw_spec.hw_param[2].i_main_data= 33;
//根据需求配置TMDS时钟频段(340M, 600M]的硬件指标参数, 若不需要配置请勿修改。
st_hw_spec.hw_param[3].i_de_main_clk = 21;
st_hw_spec.hw_param[3].i_de_main_data = 21;
st_hw_spec.hw_param[3].i_main_clk= 34;
st_hw_spec.hw_param[3].i_main_data= 35;
//设置HDMI硬件指标参数
ss_mpi_hdmi_set_hw_spec (OT_HDMI_ID_0, &st_hw_spec);
 
/* 启动HDMI */
ss_mpi_hdmi_start(OT_HDMI_ID_0);
 
/*以下，为使用结束后退出流程*/
/* 停止HDMI */
ss_mpi_hdmi_stop(OT_HDMI_ID_0);
 
/* 关闭HDMI */
ss_mpi_hdmi_close(OT_HDMI_ID_0);
 
/* 去初始化HDMI */
ss_mpi_hdmi_deinit();
```

【相关主题】

[ss\_mpi\_hdmi\_get\_hw\_spec](#ZH-CN_TOPIC_0000002408094690)

## ss\_mpi\_hdmi\_get\_hw\_spec<a name="ZH-CN_TOPIC_0000002408094690"></a>

【描述】

获取HDMI指标参数。

【语法】

```
td_s32 ss_mpi_hdmi_get_hw_spec(ot_hdmi_id hdmi, ot_hdmi_hw_spec *hw_spec);
```

【参数】

<a name="table3074mcpsimp"></a>
<table><thead align="left"><tr id="row3080mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p3082mcpsimp"><a name="p3082mcpsimp"></a><a name="p3082mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p3084mcpsimp"><a name="p3084mcpsimp"></a><a name="p3084mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p3086mcpsimp"><a name="p3086mcpsimp"></a><a name="p3086mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row3088mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p3090mcpsimp"><a name="p3090mcpsimp"></a><a name="p3090mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p3092mcpsimp"><a name="p3092mcpsimp"></a><a name="p3092mcpsimp"></a>HDMI接口号。</p>
<p id="p3093mcpsimp"><a name="p3093mcpsimp"></a><a name="p3093mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p3095mcpsimp"><a name="p3095mcpsimp"></a><a name="p3095mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row3096mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p3098mcpsimp"><a name="p3098mcpsimp"></a><a name="p3098mcpsimp"></a>hw_spec</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p3100mcpsimp"><a name="p3100mcpsimp"></a><a name="p3100mcpsimp"></a>HDMI指标参数结构体。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p3102mcpsimp"><a name="p3102mcpsimp"></a><a name="p3102mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table3104mcpsimp"></a>
<table><thead align="left"><tr id="row3109mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p3111mcpsimp"><a name="p3111mcpsimp"></a><a name="p3111mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p3113mcpsimp"><a name="p3113mcpsimp"></a><a name="p3113mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3115mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3117mcpsimp"><a name="p3117mcpsimp"></a><a name="p3117mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3119mcpsimp"><a name="p3119mcpsimp"></a><a name="p3119mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row3120mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p3122mcpsimp"><a name="p3122mcpsimp"></a><a name="p3122mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3124mcpsimp"><a name="p3124mcpsimp"></a><a name="p3124mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。

【举例】

参考[ss\_mpi\_hdmi\_set\_hw\_spec](#ZH-CN_TOPIC_0000002408254618)。

【相关主题】

[ss\_mpi\_hdmi\_set\_hw\_spec](#ZH-CN_TOPIC_0000002408254618)

## ss\_mpi\_hdmi\_set\_avmute<a name="ZH-CN_TOPIC_0000002441693833"></a>

【描述】

设置AVMUTE。

【语法】

```
td_s32 ss_mpi_hdmi_set_avmute(ot_hdmi_id hdmi, td_bool avmute_en);
```

【参数】

<a name="table5927mcpsimp"></a>
<table><thead align="left"><tr id="row5933mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p5935mcpsimp"><a name="p5935mcpsimp"></a><a name="p5935mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p5937mcpsimp"><a name="p5937mcpsimp"></a><a name="p5937mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p5939mcpsimp"><a name="p5939mcpsimp"></a><a name="p5939mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row5941mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p5943mcpsimp"><a name="p5943mcpsimp"></a><a name="p5943mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p5945mcpsimp"><a name="p5945mcpsimp"></a><a name="p5945mcpsimp"></a>HDMI接口号。</p>
<p id="p5946mcpsimp"><a name="p5946mcpsimp"></a><a name="p5946mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p5948mcpsimp"><a name="p5948mcpsimp"></a><a name="p5948mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row5949mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p5951mcpsimp"><a name="p5951mcpsimp"></a><a name="p5951mcpsimp"></a>avmute_en</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p5953mcpsimp"><a name="p5953mcpsimp"></a><a name="p5953mcpsimp"></a>AVMUTE使能标志。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p5955mcpsimp"><a name="p5955mcpsimp"></a><a name="p5955mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table5957mcpsimp"></a>
<table><thead align="left"><tr id="row5962mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p5964mcpsimp"><a name="p5964mcpsimp"></a><a name="p5964mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p5966mcpsimp"><a name="p5966mcpsimp"></a><a name="p5966mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5968mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p5970mcpsimp"><a name="p5970mcpsimp"></a><a name="p5970mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p5972mcpsimp"><a name="p5972mcpsimp"></a><a name="p5972mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row5973mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p5975mcpsimp"><a name="p5975mcpsimp"></a><a name="p5975mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p5977mcpsimp"><a name="p5977mcpsimp"></a><a name="p5977mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   由于不同设备对AVMUTE包有不同的响应，在有些设备上有可能会导致黑屏、绿屏等异常现象，建议不要使用此接口。
-   AVMUTE的设置操作在start、stop接口中均有涉及，其中在[ss\_mpi\_hdmi\_start](#ZH-CN_TOPIC_0000002408094694)接口中驱动会clean avmute，在[ss\_mpi\_hdmi\_stop](#ZH-CN_TOPIC_0000002441693837)接口中驱动会set avmute。
-   用户通过此接口设置了AVMUTE之后，如果后续有stop、start等操作时，AVMUTE的状态会被改变，最终状态可能会与用户前面设置的值不一样。
-   没有注册回调函数时用户不调用[ss\_mpi\_hdmi\_start](#ZH-CN_TOPIC_0000002408094694)或[ss\_mpi\_hdmi\_stop](#ZH-CN_TOPIC_0000002441693837)AVMUTE状态也可能被改变，详细说明请参看[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)接口的注意事项。
-   请在对端设备稳定时调用该接口，可能存在启动业务时对端设备HPD不稳定从而重新start hdmi，最终会导致set avmute失败的情况。

【举例】

```
/* 初始化HDMI */
ss_mpi_hdmi_init();
/* 打开HDMI */
ss_mpi_hdmi_open(OT_HDMI_ID_0);
/* 启动HDMI */
ss_mpi_hdmi_start(OT_HDMI_ID_0);
/* avmute set */
ss_mpi_hdmi_set_avmute(OT_HDMI_ID_0, TD_TRUE);
/*以下，为使用结束后退出流程*/
/* avmute clear */
ss_mpi_hdmi_set_avmute(OT_HDMI_ID_0, TD_FALSE);
/* 停止HDMI */
ss_mpi_hdmi_stop(OT_HDMI_ID_0);
/* 关闭HDMI */
ss_mpi_hdmi_close(OT_HDMI_ID_0);
/* 去初始化HDMI */
ss_mpi_hdmi_deinit();
```

【相关主题】

[ss\_mpi\_hdmi\_set\_hw\_spec](#ZH-CN_TOPIC_0000002408254618)

## ss\_mpi\_hdmi\_set\_mod\_param<a name="ZH-CN_TOPIC_0000002408094658"></a>

【描述】

HDMI模块参数设置。

【语法】

```
td_s32 ss_mpi_hdmi_set_mod_param(ot_hdmi_id hdmi, const ot_hdmi_mod_param *mod_param);
```

【参数】

<a name="table5677mcpsimp"></a>
<table><thead align="left"><tr id="row5683mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p5685mcpsimp"><a name="p5685mcpsimp"></a><a name="p5685mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p5687mcpsimp"><a name="p5687mcpsimp"></a><a name="p5687mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p5689mcpsimp"><a name="p5689mcpsimp"></a><a name="p5689mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row5691mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p5693mcpsimp"><a name="p5693mcpsimp"></a><a name="p5693mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p5695mcpsimp"><a name="p5695mcpsimp"></a><a name="p5695mcpsimp"></a>HDMI接口号。</p>
<p id="p5696mcpsimp"><a name="p5696mcpsimp"></a><a name="p5696mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p5698mcpsimp"><a name="p5698mcpsimp"></a><a name="p5698mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row5699mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p5701mcpsimp"><a name="p5701mcpsimp"></a><a name="p5701mcpsimp"></a>mod_param</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p5703mcpsimp"><a name="p5703mcpsimp"></a><a name="p5703mcpsimp"></a>模块参数值，包括走线长度和展频开关。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p5705mcpsimp"><a name="p5705mcpsimp"></a><a name="p5705mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table5707mcpsimp"></a>
<table><thead align="left"><tr id="row5712mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p5714mcpsimp"><a name="p5714mcpsimp"></a><a name="p5714mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p5716mcpsimp"><a name="p5716mcpsimp"></a><a name="p5716mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5718mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p5720mcpsimp"><a name="p5720mcpsimp"></a><a name="p5720mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p5722mcpsimp"><a name="p5722mcpsimp"></a><a name="p5722mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row5723mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p5725mcpsimp"><a name="p5725mcpsimp"></a><a name="p5725mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p5727mcpsimp"><a name="p5727mcpsimp"></a><a name="p5727mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

-   调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。
-   若需调用此接口，请在[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)接口之前完成设置。
-   不同走线长度对应不同的指标参数，使用时请根据实际的情况选择正确的参数。
-   枚举值与走线长度的对应关系请参考[ot\_hdmi\_trace\_len](#ZH-CN_TOPIC_0000002441654005)的说明。
-   若不设置则驱动对每个解决方案会有一个默认的尺寸：
    -   SS528V100默认2.5inch
    -   SS625V100默认3.5inch
    -   SS524V100默认3.0inch
    -   SS522V101默认3.0inch
    -   SS928V100默认2.0inch
    -   SS626V100默认2.0inch

-   解决方案默认的指标参数与另外的9套调试参数里相同尺寸的参数不是同一套，也就是说每个解决方案都有总共10套指标参数\(1套默认参数加9套调试参数\)。
-   可以通过将trace\_len设置成OT\_HDMI\_TRACE\_DEFAULT切回到解决方案的默认指标参数配置。
-   当前除了SS528V100、SS625V100、SS524V100、SS522V101、SS928V100、SS626V100之外的其他解决方案都不支持走线长度设置，接口调用后不会返回报错但是会将对应的配置值强制设置成对应的默认走线长度\(OT\_HDMI\_TRACE\_DEFAULT\)。
-   如果用户有开机画面场景，且用户使用此接口自行配置了HDMI硬件指标参数，请在开机画面场景下使用相关的接口设置相同的HDMI硬件指标参数。
-   展频开关驱动默认是不开的，打开展频会对兼容性有影响请谨慎设置。
-   当前除了SS528V100、SS625V100、SS524V100、SS522V101、SS928V100、SS626V100之外的其他解决方案都不支持展频开关设置，接口调用后不会返回报错但是会将对应的配置值强制设置成对应的默认值\(关展频\)。
-   SS928V100开机画面场景已支持走线长度设置，若不设置则默认是与2.0 inch对应的配置。

【举例】

无

【相关主题】

[ss\_mpi\_hdmi\_get\_mod\_param](#ZH-CN_TOPIC_0000002441653985)

## ss\_mpi\_hdmi\_get\_mod\_param<a name="ZH-CN_TOPIC_0000002441653985"></a>

【描述】

获取HDMI模块参数。

【语法】

```
td_s32 ss_mpi_hdmi_get_mod_param(ot_hdmi_id hdmi, ot_hdmi_mod_param *mod_param);
```

【参数】

<a name="table109mcpsimp"></a>
<table><thead align="left"><tr id="row115mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p117mcpsimp"><a name="p117mcpsimp"></a><a name="p117mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p119mcpsimp"><a name="p119mcpsimp"></a><a name="p119mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p121mcpsimp"><a name="p121mcpsimp"></a><a name="p121mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row123mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p125mcpsimp"><a name="p125mcpsimp"></a><a name="p125mcpsimp"></a>hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p127mcpsimp"><a name="p127mcpsimp"></a><a name="p127mcpsimp"></a>HDMI接口号。</p>
<p id="p128mcpsimp"><a name="p128mcpsimp"></a><a name="p128mcpsimp"></a>取值：0</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p130mcpsimp"><a name="p130mcpsimp"></a><a name="p130mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row131mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p133mcpsimp"><a name="p133mcpsimp"></a><a name="p133mcpsimp"></a>mod_param</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p135mcpsimp"><a name="p135mcpsimp"></a><a name="p135mcpsimp"></a>模块参数，包括走线长度和展频开关。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p137mcpsimp"><a name="p137mcpsimp"></a><a name="p137mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table139mcpsimp"></a>
<table><thead align="left"><tr id="row144mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p146mcpsimp"><a name="p146mcpsimp"></a><a name="p146mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p148mcpsimp"><a name="p148mcpsimp"></a><a name="p148mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row150mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p152mcpsimp"><a name="p152mcpsimp"></a><a name="p152mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p154mcpsimp"><a name="p154mcpsimp"></a><a name="p154mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row155mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p157mcpsimp"><a name="p157mcpsimp"></a><a name="p157mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p159mcpsimp"><a name="p159mcpsimp"></a><a name="p159mcpsimp"></a>失败，其值为<a href="#ZH-CN_TOPIC_0000002441693861">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hdmi.h、ot\_comm\_hdmi.h
-   库文件：libss\_hdmi.a

【注意】

调用前须确保HDMI已被open，否则将返回[OT\_ERR\_HDMI\_DEV\_NOT\_OPEN](#OT_ERR_HDMI_DEV_NOT_OPEN)。

【举例】

无

【相关主题】

[ss\_mpi\_hdmi\_set\_mod\_param](#ZH-CN_TOPIC_0000002408094658)

# 数据类型<a name="ZH-CN_TOPIC_0000002441693829"></a>

HDMI相关数据类型定义如下：

-   [ot\_hdmi\_id](#ZH-CN_TOPIC_0000002441653977)：定义HDMI接口号。
-   [ot\_hdmi\_callback](#ZH-CN_TOPIC_0000002441653981)：定义HDMI回调函数指针类型。
-   [ot\_hdmi\_callback\_func](#ZH-CN_TOPIC_0000002441654041)：定义HDMI回调函数结构体。
-   [ot\_hdmi\_event\_type](#ZH-CN_TOPIC_0000002441654025)：定义HDMI事件通知枚举。
-   [ot\_hdmi\_attr](#ZH-CN_TOPIC_0000002408094686)：定义HDMI属性结构体。
-   [ot\_hdmi\_video\_format](#ZH-CN_TOPIC_0000002441653989)：定义HDMI视频制式枚举。
-   [ot\_hdmi\_video\_mode](#ZH-CN_TOPIC_0000002408254594)：定义HDMI颜色空间类型枚举。
-   [ot\_hdmi\_deep\_color](#ZH-CN_TOPIC_0000002408094682)：定义HDMI深色模式枚举。
-   [ot\_hdmi\_sample\_rate](#ZH-CN_TOPIC_0000002441653993)：定义HDMI音频输出采样率枚举。
-   [ot\_hdmi\_bit\_depth](#ZH-CN_TOPIC_0000002408254590)：定义HDMI音频输出采样位宽枚举。
-   [ot\_hdmi\_audio\_format\_code](#ZH-CN_TOPIC_0000002441693825)：定义HDMI音频格式枚举。
-   [ot\_hdmi\_audio\_info](#ZH-CN_TOPIC_0000002441654037)：定义HDMI音频能力集信息。
-   [ot\_hdmi\_timing\_info](#ZH-CN_TOPIC_0000002408094702)：定义HDMI详细时序信息。
-   [ot\_hdmi\_detail\_timing](#ZH-CN_TOPIC_0000002408254598)：定义HDMI详细时序能力集信息。
-   [ot\_hdmi\_sink\_capability](#ZH-CN_TOPIC_0000002408254610)：定义HDMI Sink端能力集结构体。
-   [ot\_hdmi\_edid](#ZH-CN_TOPIC_0000002408094698)：定义HDMI EDID信息结构体。
-   [ot\_hdmi\_infoframe\_type](#ZH-CN_TOPIC_0000002408254602)：定义HDMI信息帧类型枚举。
-   [ot\_hdmi\_infoframe](#ZH-CN_TOPIC_0000002408254606)：定义HDMI信息帧结构体。
-   [ot\_hdmi\_infoframe\_unit](#ZH-CN_TOPIC_0000002441693841)：定义HDMI信息帧单元结构体。
-   [ot\_hdmi\_avi\_infoframe](#ZH-CN_TOPIC_0000002408254570)：定义AVI信息帧（版本2）单元结构体。
-   [ot\_hdmi\_audio\_infoframe](#ZH-CN_TOPIC_0000002408094714)：定义AUDIO信息帧（版本1）单元结构体。
-   [ot\_hdmi\_vendorspec\_infoframe](#ZH-CN_TOPIC_0000002408254614)：vendor信息帧结构体。
-   [ot\_hdmi\_color\_space](#ZH-CN_TOPIC_0000002441654033)：定义颜色空间枚举。
-   [ot\_hdmi\_bar\_info](#ZH-CN_TOPIC_0000002408254622)：定义Bar信息枚举。
-   [ot\_hdmi\_scan\_info](#ZH-CN_TOPIC_0000002441693869)：定义Scan信息枚举。
-   [ot\_hdmi\_colorimetry](#ZH-CN_TOPIC_0000002441693873)：定义Colorimetry信息枚举。
-   [ot\_hdmi\_ex\_colorimetry](#ZH-CN_TOPIC_0000002441693857)：定义扩展Colorimetry信息枚举
-   [ot\_pic\_aspect\_ratio](#ZH-CN_TOPIC_0000002408254586)：定义图像宽高比枚举。
-   [ot\_hdmi\_active\_aspect\_ratio](#ZH-CN_TOPIC_0000002441653973)：定义实际图像宽高比枚举。
-   [ot\_hdmi\_pic\_scaline](#ZH-CN_TOPIC_0000002408254626)：定义图像扫描信息枚举。
-   [ot\_hdmi\_rgb\_quant\_range](#ZH-CN_TOPIC_0000002441693817)：定义RGB量化范围枚举。
-   [ot\_hdmi\_pixel\_repetition](#ZH-CN_TOPIC_0000002441653969)：定义像素复制次数枚举。
-   [ot\_hdmi\_content\_type](#ZH-CN_TOPIC_0000002408254630)：定义内容信息枚举。
-   [ot\_hdmi\_ycc\_quant\_range](#ZH-CN_TOPIC_0000002408254634)：定义YCC量化范围枚举。
-   [ot\_hdmi\_audio\_chn\_cnt](#ZH-CN_TOPIC_0000002441654029)：定义音频通道数枚举。
-   [ot\_hdmi\_coding\_type](#ZH-CN_TOPIC_0000002441693853)：定义音频编码类型枚举。
-   [ot\_hdmi\_audio\_sample\_size](#ZH-CN_TOPIC_0000002408254582)：定义音频采样大小枚举。
-   [ot\_hdmi\_audio\_sample\_freq](#ZH-CN_TOPIC_0000002441693821)：定义音频采样频率枚举。
-   [ot\_hdmi\_level\_shift\_val](#ZH-CN_TOPIC_0000002408094670)：定义音频Shift信息枚举。
-   [ot\_hdmi\_lfe\_playback\_level](#ZH-CN_TOPIC_0000002408254578)：定义音频Playback信息枚举。
-   [ot\_hdmi\_eotf](#ZH-CN_TOPIC_0000002408094662)：定义支持的EOTF能力结构体。
-   [ot\_hdmi\_hdr\_metadata\_type](#ZH-CN_TOPIC_0000002408094666)：定义支持metadata类型结构体。
-   [ot\_hdmi\_hdr\_capability](#ZH-CN_TOPIC_0000002408094722)：定义HDR能力结构体。
-   [ot\_hdmi\_hw\_param](#ZH-CN_TOPIC_0000002408094706)：HDMI指标参数结构体。
-   [ot\_hdmi\_hw\_spec](#ZH-CN_TOPIC_0000002441693849)：HDMI指标参数。
-   [ot\_hdmi\_mod\_param](#ZH-CN_TOPIC_0000002408094674)：HDMI模块参数设置。
-   [ot\_hdmi\_trace\_len](#ZH-CN_TOPIC_0000002441654005)：HDMI信号线在PCB板上的走线长度枚举。
-   [OT\_HDMI\_VENDOR\_USER\_DATA\_MAX\_LEN](#ZH-CN_TOPIC_0000002441693845)：最大自定义vendor数据的长度。
-   [OT\_HDMI\_MAX\_AUDIO\_CAPBILITY\_CNT](#ZH-CN_TOPIC_0000002408094678)：能获取的音频能力个数。
-   [OT\_HDMI\_MAX\_SAMPLE\_RATE\_NUM](#ZH-CN_TOPIC_0000002441654001)：音频采样率个数。
-   [OT\_HDMI\_MAX\_BIT\_DEPTH\_NUM](#ZH-CN_TOPIC_0000002408254574)：音频采样深度个数。
-   [OT\_HDMI\_DETAIL\_TIMING\_MAX](#ZH-CN_TOPIC_0000002408094718)：详细的视频制式信息个数。
-   [OT\_HDMI\_EDID\_RAW\_DATA\_LEN](#ZH-CN_TOPIC_0000002441693865)：EDID原始数据的长度。
-   [OT\_HDMI\_HW\_PARAM\_NUM](#ZH-CN_TOPIC_0000002441654009)：硬件参数的个数。
-   [OT\_HDMI\_MANUFACTURE\_NAME\_LEN](#ZH-CN_TOPIC_0000002441654021)：制造商名称的长度。

注：本节已涵盖各重要的数据类型，部分未列出数据类型请参见ot\_comm\_hdmi.h

## ot\_hdmi\_id<a name="ZH-CN_TOPIC_0000002441653977"></a>

【说明】

定义HDMI接口号。

【定义】

```
typedef enum {
    OT_HDMI_ID_0 = 0,
    OT_HDMI_ID_1 = 1,
    OT_HDMI_ID_BUTT
} ot_hdmi_id;
```

【成员】

<a name="table4441mcpsimp"></a>
<table><thead align="left"><tr id="row4446mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p4448mcpsimp"><a name="p4448mcpsimp"></a><a name="p4448mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p4450mcpsimp"><a name="p4450mcpsimp"></a><a name="p4450mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4452mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p4454mcpsimp"><a name="p4454mcpsimp"></a><a name="p4454mcpsimp"></a>OT_HDMI_ID_0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p4456mcpsimp"><a name="p4456mcpsimp"></a><a name="p4456mcpsimp"></a>HDMI接口0</p>
</td>
</tr>
<tr id="row4457mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p4459mcpsimp"><a name="p4459mcpsimp"></a><a name="p4459mcpsimp"></a>OT_HDMI_ID_1</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p4461mcpsimp"><a name="p4461mcpsimp"></a><a name="p4461mcpsimp"></a>HDMI接口1，仅SS626V100支持</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   保留将来扩展多个HDMI设备支持。
-   设置其他值将返回[OT\_ERR\_HDMI\_INVALID\_PARA](#OT_ERR_HDMI_INVALID_PARA)。

【相关数据类型及接口】

无。

## ot\_hdmi\_callback<a name="ZH-CN_TOPIC_0000002441653981"></a>

【说明】

定义HDMI回调函数指针类型。

【定义】

```
typedef void (*ot_hdmi_callback)(ot_hdmi_event_type event, td_void *private_data);
```

【成员】

<a name="table2554mcpsimp"></a>
<table><thead align="left"><tr id="row2559mcpsimp"><th class="cellrowborder" valign="top" width="34.97%" id="mcps1.1.3.1.1"><p id="p2561mcpsimp"><a name="p2561mcpsimp"></a><a name="p2561mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="65.03%" id="mcps1.1.3.1.2"><p id="p2563mcpsimp"><a name="p2563mcpsimp"></a><a name="p2563mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2565mcpsimp"><td class="cellrowborder" valign="top" width="34.97%" headers="mcps1.1.3.1.1 "><p id="p2567mcpsimp"><a name="p2567mcpsimp"></a><a name="p2567mcpsimp"></a>ot_hdmi_event_type</p>
</td>
<td class="cellrowborder" valign="top" width="65.03%" headers="mcps1.1.3.1.2 "><p xml:lang="fr-FR" id="p2569mcpsimp"><a name="p2569mcpsimp"></a><a name="p2569mcpsimp"></a>HDMI事件通知类型<span xml:lang="en-US" id="ph2570mcpsimp"><a name="ph2570mcpsimp"></a><a name="ph2570mcpsimp"></a>。</span></p>
</td>
</tr>
<tr id="row2571mcpsimp"><td class="cellrowborder" valign="top" width="34.97%" headers="mcps1.1.3.1.1 "><p id="p2573mcpsimp"><a name="p2573mcpsimp"></a><a name="p2573mcpsimp"></a>private_data</p>
</td>
<td class="cellrowborder" valign="top" width="65.03%" headers="mcps1.1.3.1.2 "><p id="p2575mcpsimp"><a name="p2575mcpsimp"></a><a name="p2575mcpsimp"></a>事件私有数据。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_hdmi\_callback\_func](#ZH-CN_TOPIC_0000002441654041)
-   [ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)
-   [ss\_mpi\_hdmi\_unregister\_callback](#ZH-CN_TOPIC_0000002408254562)

## ot\_hdmi\_callback\_func<a name="ZH-CN_TOPIC_0000002441654041"></a>

【说明】

定义HDMI回调函数结构体。

【定义】

```
typedef struct {
    ot_hdmi_callback   hdmi_event_callback;
    td_void             *private_data;
} ot_hdmi_callback_func;
```

【成员】

<a name="table3996mcpsimp"></a>
<table><thead align="left"><tr id="row4001mcpsimp"><th class="cellrowborder" valign="top" width="32%" id="mcps1.1.3.1.1"><p id="p4003mcpsimp"><a name="p4003mcpsimp"></a><a name="p4003mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.3.1.2"><p id="p4005mcpsimp"><a name="p4005mcpsimp"></a><a name="p4005mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4007mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p4009mcpsimp"><a name="p4009mcpsimp"></a><a name="p4009mcpsimp"></a>hdmi_event_callback</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p4011mcpsimp"><a name="p4011mcpsimp"></a><a name="p4011mcpsimp"></a>事件处理回调函数。</p>
</td>
</tr>
<tr id="row4012mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p4014mcpsimp"><a name="p4014mcpsimp"></a><a name="p4014mcpsimp"></a>private_data</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p4016mcpsimp"><a name="p4016mcpsimp"></a><a name="p4016mcpsimp"></a>回调函数参数私有数据。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   建议用户注册事件回调函数，若不注册则HDMI内部采取默认行为。见[ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)。
-   私有数据不能为局部变量，否则可能导致回调函数中不能正常访问。
-   回调函数参数私有数据，一般并不使用，赋值可以为NULL。

【相关数据类型及接口】

-   [ot\_hdmi\_callback](#ZH-CN_TOPIC_0000002441653981)
-   [ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)
-   [ss\_mpi\_hdmi\_unregister\_callback](#ZH-CN_TOPIC_0000002408254562)

## ot\_hdmi\_event\_type<a name="ZH-CN_TOPIC_0000002441654025"></a>

【说明】

定义HDMI事件通知枚举。

【定义】

```
typedef enum {
    OT_HDMI_EVENT_HOTPLUG = 0x10,
    OT_HDMI_EVENT_NO_PLUG,
    OT_HDMI_EVENT_EDID_FAIL,
    OT_HDMI_EVENT_BUTT
} ot_hdmi_event_type;
```

【成员】

<a name="table4043mcpsimp"></a>
<table><thead align="left"><tr id="row4048mcpsimp"><th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.3.1.1"><p id="p4050mcpsimp"><a name="p4050mcpsimp"></a><a name="p4050mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="45%" id="mcps1.1.3.1.2"><p id="p4052mcpsimp"><a name="p4052mcpsimp"></a><a name="p4052mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4054mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p4056mcpsimp"><a name="p4056mcpsimp"></a><a name="p4056mcpsimp"></a>OT_HDMI_EVENT_HOTPLUG</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p4058mcpsimp"><a name="p4058mcpsimp"></a><a name="p4058mcpsimp"></a>HDMI Cable插入事件。</p>
</td>
</tr>
<tr id="row4059mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p4061mcpsimp"><a name="p4061mcpsimp"></a><a name="p4061mcpsimp"></a>OT_HDMI_EVENT_NO_PLUG</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p4063mcpsimp"><a name="p4063mcpsimp"></a><a name="p4063mcpsimp"></a>HDMI Cable拔出事件。</p>
</td>
</tr>
<tr id="row4064mcpsimp"><td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.1 "><p id="p4066mcpsimp"><a name="p4066mcpsimp"></a><a name="p4066mcpsimp"></a>OT_HDMI_EVENT_EDID_FAIL</p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.2 "><p id="p4068mcpsimp"><a name="p4068mcpsimp"></a><a name="p4068mcpsimp"></a>HDMI EDID读取失败事件。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_register\_callback](#ZH-CN_TOPIC_0000002408094654)
-   [ss\_mpi\_hdmi\_unregister\_callback](#ZH-CN_TOPIC_0000002408254562)

## ot\_hdmi\_attr<a name="ZH-CN_TOPIC_0000002408094686"></a>

【说明】

定义HDMI属性结构体。

【定义】

```
typedef struct {
    td_bool                 hdmi_en;
    ot_hdmi_video_format    video_format;
    ot_hdmi_deep_color     deep_color_mode;
    td_bool                 audio_en;
    ot_hdmi_sample_rate    sample_rate;
    ot_hdmi_bit_depth      bit_depth;
    td_bool                 auth_mode_en;
    td_bool                 deep_color_adapt_en;
    td_u32                  pix_clk;
} ot_hdmi_attr;
```

【成员】

<a name="table3666mcpsimp"></a>
<table><thead align="left"><tr id="row3671mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p3673mcpsimp"><a name="p3673mcpsimp"></a><a name="p3673mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p3675mcpsimp"><a name="p3675mcpsimp"></a><a name="p3675mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3677mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3679mcpsimp"><a name="p3679mcpsimp"></a><a name="p3679mcpsimp"></a>hdmi_en</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3681mcpsimp"><a name="p3681mcpsimp"></a><a name="p3681mcpsimp"></a>是否强制HDMI输出。</p>
<p id="p3682mcpsimp"><a name="p3682mcpsimp"></a><a name="p3682mcpsimp"></a>TD_TRUE：强制HDMI输出；</p>
<p id="p3683mcpsimp"><a name="p3683mcpsimp"></a><a name="p3683mcpsimp"></a>TD_FALSE：DVI输出。</p>
</td>
</tr>
<tr id="row3684mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3686mcpsimp"><a name="p3686mcpsimp"></a><a name="p3686mcpsimp"></a>video_format</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3688mcpsimp"><a name="p3688mcpsimp"></a><a name="p3688mcpsimp"></a>视频制式，此参数需要与VO配置的制式保持一致。</p>
<p id="p3689mcpsimp"><a name="p3689mcpsimp"></a><a name="p3689mcpsimp"></a>建议用户设置为Sink能力集支持的制式。</p>
</td>
</tr>
<tr id="row3690mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3692mcpsimp"><a name="p3692mcpsimp"></a><a name="p3692mcpsimp"></a>deep_color_mode</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3694mcpsimp"><a name="p3694mcpsimp"></a><a name="p3694mcpsimp"></a>DeepColor输出模式。</p>
<p id="p3695mcpsimp"><a name="p3695mcpsimp"></a><a name="p3695mcpsimp"></a>OT_HDMI_DEEP_COLOR_24BIT、OT_HDMI_DEEP_COLOR_30BIT、OT_HDMI_DEEP_COLOR_36BIT、</p>
<p id="p3696mcpsimp"><a name="p3696mcpsimp"></a><a name="p3696mcpsimp"></a>默认为OT_HDMI_DEEP_COLOR_24BIT。</p>
<p id="p3697mcpsimp"><a name="p3697mcpsimp"></a><a name="p3697mcpsimp"></a>部分Sink不支持OT_HDMI_DEEP_COLOR_30BIT和OT_HDMI_DEEP_COLOR_36BIT，设置此DeepColor可能引起异常。</p>
</td>
</tr>
<tr id="row3698mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3700mcpsimp"><a name="p3700mcpsimp"></a><a name="p3700mcpsimp"></a>audio_en</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3702mcpsimp"><a name="p3702mcpsimp"></a><a name="p3702mcpsimp"></a>是否使能音频。</p>
</td>
</tr>
<tr id="row3703mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3705mcpsimp"><a name="p3705mcpsimp"></a><a name="p3705mcpsimp"></a>sample_rate</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3707mcpsimp"><a name="p3707mcpsimp"></a><a name="p3707mcpsimp"></a>音频采样率，此参数需要与AO的配置保持一致。</p>
<p id="p3708mcpsimp"><a name="p3708mcpsimp"></a><a name="p3708mcpsimp"></a>SSxx目前不支持采样率小于32KHz的采样率，建议用户设置为Sink能力集支持的采样率。</p>
</td>
</tr>
<tr id="row3709mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3711mcpsimp"><a name="p3711mcpsimp"></a><a name="p3711mcpsimp"></a>bit_depth</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3713mcpsimp"><a name="p3713mcpsimp"></a><a name="p3713mcpsimp"></a>音频位宽，默认为16，此参数需要与AO的配置保持一致。</p>
<p id="p3714mcpsimp"><a name="p3714mcpsimp"></a><a name="p3714mcpsimp"></a>建议用户设置为Sink能力集支持位宽。</p>
</td>
</tr>
<tr id="row3715mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3717mcpsimp"><a name="p3717mcpsimp"></a><a name="p3717mcpsimp"></a>auth_mode_en</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3719mcpsimp"><a name="p3719mcpsimp"></a><a name="p3719mcpsimp"></a>使能该模式，HDMI强制输出，不再去根据显示或认证设备的EDID信息来自适应调整，主要针对认证场景。</p>
<p id="p3720mcpsimp"><a name="p3720mcpsimp"></a><a name="p3720mcpsimp"></a>0：不使能auth_mode；</p>
<p id="p3721mcpsimp"><a name="p3721mcpsimp"></a><a name="p3721mcpsimp"></a>1：使能auth_mode。</p>
<p id="p3722mcpsimp"><a name="p3722mcpsimp"></a><a name="p3722mcpsimp"></a>说明：默认为TD_FALSE，此标志用于调试和HDMI认证测试用，建议用户不要修改，其他注意事项请参考<a href="#ZH-CN_TOPIC_0000002441693897">ss_mpi_hdmi_set_attr</a>。</p>
</td>
</tr>
<tr id="row3724mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3726mcpsimp"><a name="p3726mcpsimp"></a><a name="p3726mcpsimp"></a>deep_color_adapt_en</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3728mcpsimp"><a name="p3728mcpsimp"></a><a name="p3728mcpsimp"></a>是否打开驱动Deepcolor自适应策略。</p>
<p id="p3729mcpsimp"><a name="p3729mcpsimp"></a><a name="p3729mcpsimp"></a>默认为TD_TRUE。建议用户设置为TD_TRUE。</p>
</td>
</tr>
<tr id="row3730mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p3732mcpsimp"><a name="p3732mcpsimp"></a><a name="p3732mcpsimp"></a>pix_clk</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p3734mcpsimp"><a name="p3734mcpsimp"></a><a name="p3734mcpsimp"></a>用户设置自定义时序的像素时钟（单位：KHz）。</p>
<p id="p3735mcpsimp"><a name="p3735mcpsimp"></a><a name="p3735mcpsimp"></a>说明：用户设置自定义时序时一定设置此参数，此参数仅当video_format为OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE时才会生效。</p>
<p id="p284564716581"><a name="p284564716581"></a><a name="p284564716581"></a>注意：当配置自定义时序时，如果配置的pix_clk小于25M，需要在配置VO模块时进行倍频操作，使最终clk配置大于25M。同时需要设置HDMI信息帧中的avi_infoframe，需要把pixel_repetition置为对应倍频倍数。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   用户可以根据建议值设置HDMI属性。
-   用户设置DVI模式输出之前必须先通过VO的接口将color space设置成RGB。
-   对于暂时不支持的属性，请设置为默认值。
-   用户设置自定义时序时，一定要设置该时序对应的像素时钟pix\_clk（其他情况下不需要设置此参数，即使设置也不会生效），否则可能显示设备无法正常显示。此参数的合法性需用户保证，驱动无法识别该参数是否合法，如果参数不合法，也可能导致显示设备无法正常显示。
-   hdmi\_en被设置为TD\_FALSE时，HDMI驱动将工作在DVI模式，此时不输出音频以及所有信息帧。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)
-   [ss\_mpi\_hdmi\_get\_attr](#ZH-CN_TOPIC_0000002408094710)

## ot\_hdmi\_video\_format<a name="ZH-CN_TOPIC_0000002441653989"></a>

【说明】

定义HDMI视频制式枚举。

【定义】

```
typedef enum {
    OT_HDMI_VIDEO_FORMAT_1080P_60,
    OT_HDMI_VIDEO_FORMAT_1080P_50,
    OT_HDMI_VIDEO_FORMAT_1080P_30,
    OT_HDMI_VIDEO_FORMAT_1080P_25,
    OT_HDMI_VIDEO_FORMAT_1080P_24,
    OT_HDMI_VIDEO_FORMAT_1080i_60,
    OT_HDMI_VIDEO_FORMAT_1080i_50,
    OT_HDMI_VIDEO_FORMAT_720P_60,
    OT_HDMI_VIDEO_FORMAT_720P_50,
    OT_HDMI_VIDEO_FORMAT_576P_50,
    OT_HDMI_VIDEO_FORMAT_480P_60,
    OT_HDMI_VIDEO_FORMAT_PAL,                  /* B D G H I PAL */
    OT_HDMI_VIDEO_FORMAT_NTSC,                 /* (M)NTSC       */
    OT_HDMI_VIDEO_FORMAT_861D_640X480_60,
    OT_HDMI_VIDEO_FORMAT_VESA_800X600_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1024X768_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1280X800_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1280X1024_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1366X768_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1440X900_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1400X1050_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1600X1200_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1680X1050_60,
    OT_HDMI_VIDEO_FORMAT_VESA_1920X1200_60,
    OT_HDMI_VIDEO_FORMAT_2560x1440_30,
    OT_HDMI_VIDEO_FORMAT_2560x1440_60,
    OT_HDMI_VIDEO_FORMAT_2560x1600_60,
    OT_HDMI_VIDEO_FORMAT_1920x2160_30,
    OT_HDMI_VIDEO_FORMAT_3840X2160P_24,               
    OT_HDMI_VIDEO_FORMAT_3840X2160P_25,               
    OT_HDMI_VIDEO_FORMAT_3840X2160P_30,               
    OT_HDMI_VIDEO_FORMAT_3840X2160P_50,               
    OT_HDMI_VIDEO_FORMAT_3840X2160P_60,               
    OT_HDMI_VIDEO_FORMAT_4096X2160P_24,               
    OT_HDMI_VIDEO_FORMAT_4096X2160P_25,               
    OT_HDMI_VIDEO_FORMAT_4096X2160P_30,               
    OT_HDMI_VIDEO_FORMAT_4096X2160P_50,               
    OT_HDMI_VIDEO_FORMAT_4096X2160P_60,        
    OT_HDMI_VIDEO_FORMAT_3840X2160P_120,
    OT_HDMI_VIDEO_FORMAT_4096X2160P_120,
    OT_HDMI_VIDEO_FORMAT_7680X4320P_30,
    OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    OT_HDMI_VIDEO_FORMAT_BUTT
} ot_hdmi_video_format;
```

【成员】

无。

【注意事项】

-   需要根据视频输出的制式设置相应的HDMI的制式。

    -   OT\_HDMI\_VIDEO\_FORMAT\_3840X2160P\_50
    -   OT\_HDMI\_VIDEO\_FORMAT\_3840X2160P\_60
    -   OT\_HDMI\_VIDEO\_FORMAT\_4096X2160P\_50
    -   OT\_HDMI\_VIDEO\_FORMAT\_4096X2160P\_60

    **以上四种制式为HDMI2.0规格，仅支持HDMI1.4的产品不支持这几种制式。**

-   暂不支持如下制式：

    -   OT\_HDMI\_VIDEO\_FORMAT\_3840X2160P\_120
    -   OT\_HDMI\_VIDEO\_FORMAT\_4096X2160P\_120
    -   OT\_HDMI\_VIDEO\_FORMAT\_7680X4320P\_30

    设置这些参数将返回[OT\_ERR\_HDMI\_FEATURE\_NO\_SUPPORT](#OT_ERR_HDMI_FEATURE_NO_SUPPORT)。

-   当[ot\_hdmi\_attr](#ZH-CN_TOPIC_0000002408094686)中的hdmi\_en设置成TD\_FALSE\(DVI模式\)时，不支持OT\_HDMI\_VIDEO\_FORMAT\_3840X2160P\_24及以上分辨率，强行设置时会返回[OT\_ERR\_HDMI\_FEATURE\_NO\_SUPPORT](#OT_ERR_HDMI_FEATURE_NO_SUPPORT)。

【相关数据类型及接口】

[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)

## ot\_hdmi\_video\_mode<a name="ZH-CN_TOPIC_0000002408254594"></a>

【说明】

定义HDMI颜色空间类型枚举。

【定义】

```
typedef enum {
    OT_HDMI_VIDEO_MODE_RGB444,
    OT_HDMI_VIDEO_MODE_YCBCR422,
    OT_HDMI_VIDEO_MODE_YCBCR444,
    OT_HDMI_VIDEO_MODE_YCBCR420,
    OT_HDMI_VIDEO_MODE_BUTT
} ot_hdmi_video_mode;
```

【成员】

<a name="table5189mcpsimp"></a>
<table><thead align="left"><tr id="row5194mcpsimp"><th class="cellrowborder" valign="top" width="52%" id="mcps1.1.3.1.1"><p id="p5196mcpsimp"><a name="p5196mcpsimp"></a><a name="p5196mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="48%" id="mcps1.1.3.1.2"><p id="p5198mcpsimp"><a name="p5198mcpsimp"></a><a name="p5198mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5200mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p5202mcpsimp"><a name="p5202mcpsimp"></a><a name="p5202mcpsimp"></a>OT_HDMI_VIDEO_MODE_RGB444</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p5204mcpsimp"><a name="p5204mcpsimp"></a><a name="p5204mcpsimp"></a>RGB444输出模式</p>
</td>
</tr>
<tr id="row5205mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p5207mcpsimp"><a name="p5207mcpsimp"></a><a name="p5207mcpsimp"></a>OT_HDMI_VIDEO_MODE_YCBCR422</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p5209mcpsimp"><a name="p5209mcpsimp"></a><a name="p5209mcpsimp"></a>YCBCR422输出模式</p>
</td>
</tr>
<tr id="row5210mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p5212mcpsimp"><a name="p5212mcpsimp"></a><a name="p5212mcpsimp"></a>OT_HDMI_VIDEO_MODE_YCBCR444</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p5214mcpsimp"><a name="p5214mcpsimp"></a><a name="p5214mcpsimp"></a>YCBCR444输出模式</p>
</td>
</tr>
<tr id="row5215mcpsimp"><td class="cellrowborder" valign="top" width="52%" headers="mcps1.1.3.1.1 "><p id="p5217mcpsimp"><a name="p5217mcpsimp"></a><a name="p5217mcpsimp"></a>OT_HDMI_VIDEO_MODE_YCBCR420</p>
</td>
<td class="cellrowborder" valign="top" width="48%" headers="mcps1.1.3.1.2 "><p id="p5219mcpsimp"><a name="p5219mcpsimp"></a><a name="p5219mcpsimp"></a>YCBCR420输出模式</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

OT\_HDMI\_VIDEO\_MODE\_YCBCR420**为HDMI2.0规格，仅支持HDMI1.4的产品不支持此颜色空间。**

【相关数据类型及接口】

无。

## ot\_hdmi\_deep\_color<a name="ZH-CN_TOPIC_0000002408094682"></a>

【说明】

定义HDMI深色模式枚举。

【定义】

```
typedef enum {
    OT_HDMI_DEEP_COLOR_24BIT,
    OT_HDMI_DEEP_COLOR_30BIT,
    OT_HDMI_DEEP_COLOR_36BIT,
    OT_HDMI_DEEP_COLOR_BUTT
} ot_hdmi_deep_color;
```

【成员】

<a name="table3611mcpsimp"></a>
<table><thead align="left"><tr id="row3616mcpsimp"><th class="cellrowborder" valign="top" width="45%" id="mcps1.1.3.1.1"><p id="p3618mcpsimp"><a name="p3618mcpsimp"></a><a name="p3618mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.3.1.2"><p id="p3620mcpsimp"><a name="p3620mcpsimp"></a><a name="p3620mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3622mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p3624mcpsimp"><a name="p3624mcpsimp"></a><a name="p3624mcpsimp"></a>OT_HDMI_DEEP_COLOR_24BIT</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p3626mcpsimp"><a name="p3626mcpsimp"></a><a name="p3626mcpsimp"></a>HDMI Deep Color 24bit模式</p>
</td>
</tr>
<tr id="row3627mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p3629mcpsimp"><a name="p3629mcpsimp"></a><a name="p3629mcpsimp"></a>OT_HDMI_DEEP_COLOR_30BIT</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p3631mcpsimp"><a name="p3631mcpsimp"></a><a name="p3631mcpsimp"></a>HDMI Deep Color 30bit模式</p>
</td>
</tr>
<tr id="row3632mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p3634mcpsimp"><a name="p3634mcpsimp"></a><a name="p3634mcpsimp"></a>OT_HDMI_DEEP_COLOR_36BIT</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p3636mcpsimp"><a name="p3636mcpsimp"></a><a name="p3636mcpsimp"></a>HDMI Deep Color 36bit模式</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   若获取到对端能力不支持30bit和36bit时，建议不要设置，否则显示设备很有可能无显示。
-   设置OT\_HDMI\_DEEP\_COLOR\_BUTT及枚举以外的值将返回[OT\_ERR\_HDMI\_INVALID\_PARA](#OT_ERR_HDMI_INVALID_PARA)。

【相关数据类型及接口】

[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)

## ot\_hdmi\_sample\_rate<a name="ZH-CN_TOPIC_0000002441653993"></a>

【说明】

定义HDMI音频输出采样率枚举。

【定义】

```
typedef enum {
    OT_HDMI_SAMPLE_RATE_UNKNOWN,
    OT_HDMI_SAMPLE_RATE_8K,
    OT_HDMI_SAMPLE_RATE_11K,
    OT_HDMI_SAMPLE_RATE_12K,
    OT_HDMI_SAMPLE_RATE_16K,
    OT_HDMI_SAMPLE_RATE_22K,
    OT_HDMI_SAMPLE_RATE_24K,
    OT_HDMI_SAMPLE_RATE_32K,
    OT_HDMI_SAMPLE_RATE_44K,
    OT_HDMI_SAMPLE_RATE_48K,
    OT_HDMI_SAMPLE_RATE_88K,
    OT_HDMI_SAMPLE_RATE_96K,
    OT_HDMI_SAMPLE_RATE_176K,
    OT_HDMI_SAMPLE_RATE_192K,
    OT_HDMI_SAMPLE_RATE_768K,
    OT_HDMI_SAMPLE_RATE_BUTT
} ot_hdmi_sample_rate;
```

【成员】

无。

【注意事项】

-   当前仅支持OT\_HDMI\_SAMPLE\_RATE\_32K、OT\_HDMI\_SAMPLE\_RATE\_44K、OT\_HDMI\_SAMPLE\_RATE\_48K。
-   设置OT\_HDMI\_SAMPLE\_RATE\_BUTT及枚举以外的值将返回[OT\_ERR\_HDMI\_INVALID\_PARA](#OT_ERR_HDMI_INVALID_PARA)，设置其他参数将返回[OT\_ERR\_HDMI\_FEATURE\_NO\_SUPPORT](#OT_ERR_HDMI_FEATURE_NO_SUPPORT)。
-   音频模块传递给HDMI的数据，采样率必须与HDMI输出的保持一致。如音频模块采集的音频源为8KHz，而HDMI需要输出48KHz音频数据，此时需要音频模块对源数据进行重采样到48KHz。

【相关数据类型及接口】

[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)

## ot\_hdmi\_bit\_depth<a name="ZH-CN_TOPIC_0000002408254590"></a>

【说明】

定义HDMI音频输出采样位宽枚举。

【定义】

```
typedef enum {
    OT_HDMI_BIT_DEPTH_UNKNOWN,
    OT_HDMI_BIT_DEPTH_8,
    OT_HDMI_BIT_DEPTH_16,
    OT_HDMI_BIT_DEPTH_18,
    OT_HDMI_BIT_DEPTH_20,
    OT_HDMI_BIT_DEPTH_24,
    OT_HDMI_BIT_DEPTH_32,
    OT_HDMI_BIT_DEPTH_BUTT
} ot_hdmi_bit_depth;
```

【成员】

无。

【注意事项】

-   目前只支持设置OT\_HDMI\_BIT\_DEPTH\_16的音频采样位宽。
-   OT\_HDMI\_BIT\_DEPTH\_UNKNOWN、OT\_HDMI\_BIT\_DEPTH\_8、OT\_HDMI\_BIT\_DEPTH\_18、OT\_HDMI\_BIT\_DEPTH\_20、OT\_HDMI\_BIT\_DEPTH\_24、OT\_HDMI\_BIT\_DEPTH\_32暂不支持，设置这些参数时将返回[OT\_ERR\_HDMI\_FEATURE\_NO\_SUPPORT](#OT_ERR_HDMI_FEATURE_NO_SUPPORT)。设置OT\_HDMI\_BIT\_DEPTH\_BUTT及枚举以外的值将返回[OT\_ERR\_HDMI\_INVALID\_PARA](#OT_ERR_HDMI_INVALID_PARA)。

【相关数据类型及接口】

[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)

## ot\_hdmi\_audio\_format\_code<a name="ZH-CN_TOPIC_0000002441693825"></a>

【说明】

定义HDMI音频格式枚举。

【定义】

```
typedef enum {
    OT_HDMI_AUDIO_FORMAT_CODE_RESERVED,
    OT_HDMI_AUDIO_FORMAT_CODE_PCM,
    OT_HDMI_AUDIO_FORMAT_CODE_AC3,
    OT_HDMI_AUDIO_FORMAT_CODE_MPEG1,
    OT_HDMI_AUDIO_FORMAT_CODE_MP3,
    OT_HDMI_AUDIO_FORMAT_CODE_MPEG2,
    OT_HDMI_AUDIO_FORMAT_CODE_AAC,
    OT_HDMI_AUDIO_FORMAT_CODE_DTS,
    OT_HDMI_AUDIO_FORMAT_CODE_ATRAC,
    OT_HDMI_AUDIO_FORMAT_CODE_ONE_BIT,
    OT_HDMI_AUDIO_FORMAT_CODE_DDP,
    OT_HDMI_AUDIO_FORMAT_CODE_DTS_HD,
    OT_HDMI_AUDIO_FORMAT_CODE_MAT,
    OT_HDMI_AUDIO_FORMAT_CODE_DST,
    OT_HDMI_AUDIO_FORMAT_CODE_WMA_PRO,
    OT_HDMI_AUDIO_FORMAT_CODE_BUTT,
} ot_hdmi_audio_format_code;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

[ss\_mpi\_hdmi\_set\_attr](#ZH-CN_TOPIC_0000002441693897)

## ot\_hdmi\_audio\_info<a name="ZH-CN_TOPIC_0000002441654037"></a>

【说明】

定义HDMI音频能力集信息。

【定义】

```
typedef struct {
    ot_hdmi_audio_format_code   audio_format_code;
    ot_hdmi_sample_rate        support_sample_rate[OT_HDMI_MAX_SAMPLE_RATE_NUM];
    td_u8                       audio_chn;
    ot_hdmi_bit_depth           support_bit_depth[OT_HDMI_MAX_BIT_DEPTH_NUM];
    td_u32                      support_bit_depth_num;
    td_u32                      max_bit_rate;
} ot_hdmi_audio_info;
```

【成员】

<a name="table4382mcpsimp"></a>
<table><thead align="left"><tr id="row4387mcpsimp"><th class="cellrowborder" valign="top" width="32%" id="mcps1.1.3.1.1"><p id="p4389mcpsimp"><a name="p4389mcpsimp"></a><a name="p4389mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.3.1.2"><p id="p4391mcpsimp"><a name="p4391mcpsimp"></a><a name="p4391mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4393mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p4395mcpsimp"><a name="p4395mcpsimp"></a><a name="p4395mcpsimp"></a>audio_format_code</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p4397mcpsimp"><a name="p4397mcpsimp"></a><a name="p4397mcpsimp"></a>支持的音频格式。</p>
</td>
</tr>
<tr id="row4398mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p4400mcpsimp"><a name="p4400mcpsimp"></a><a name="p4400mcpsimp"></a>support_sample_rate</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p4402mcpsimp"><a name="p4402mcpsimp"></a><a name="p4402mcpsimp"></a>支持的音频采样率。</p>
</td>
</tr>
<tr id="row4403mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p4405mcpsimp"><a name="p4405mcpsimp"></a><a name="p4405mcpsimp"></a>audio_chn</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p4407mcpsimp"><a name="p4407mcpsimp"></a><a name="p4407mcpsimp"></a>支持的音频通道数。</p>
</td>
</tr>
<tr id="row4408mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p4410mcpsimp"><a name="p4410mcpsimp"></a><a name="p4410mcpsimp"></a>support_bit_depth</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p4412mcpsimp"><a name="p4412mcpsimp"></a><a name="p4412mcpsimp"></a>支持的采样bit深度。</p>
</td>
</tr>
<tr id="row4413mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p4415mcpsimp"><a name="p4415mcpsimp"></a><a name="p4415mcpsimp"></a>support_bit_depth_num</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p4417mcpsimp"><a name="p4417mcpsimp"></a><a name="p4417mcpsimp"></a>支持不同的bit深度种类个数。</p>
</td>
</tr>
<tr id="row4418mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p4420mcpsimp"><a name="p4420mcpsimp"></a><a name="p4420mcpsimp"></a>max_bit_rate</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p4422mcpsimp"><a name="p4422mcpsimp"></a><a name="p4422mcpsimp"></a>最大比特率。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_hdmi\_sink\_capability](#ZH-CN_TOPIC_0000002408254610)
-   [ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)

## ot\_hdmi\_timing\_info<a name="ZH-CN_TOPIC_0000002408094702"></a>

【说明】

定义HDMI Detail Timing信息。

【定义】

```
typedef struct {
    td_u32       vfb;
    td_u32       vbb;
    td_u32       vact;
    td_u32       hfb;
    td_u32       hbb;
    td_u32       hact;
    td_u32       vpw;
    td_u32       hpw;
    td_bool      idv;
    td_bool      ihs;
    td_bool      ivs;
    td_u32       img_width;
    td_u32       img_height;
    td_u32       aspect_ratio_w;
    td_u32       aspect_ratio_h;
    td_bool      interlace;
    td_s32       pixel_clk;
} ot_hdmi_timing_info;
```

【成员】

<a name="table5328mcpsimp"></a>
<table><thead align="left"><tr id="row5333mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p5335mcpsimp"><a name="p5335mcpsimp"></a><a name="p5335mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p5337mcpsimp"><a name="p5337mcpsimp"></a><a name="p5337mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5339mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5341mcpsimp"><a name="p5341mcpsimp"></a><a name="p5341mcpsimp"></a>vfb</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5343mcpsimp"><a name="p5343mcpsimp"></a><a name="p5343mcpsimp"></a>垂直消隐前肩，隔行输出时表示顶场垂直消隐前肩。单位：行。</p>
</td>
</tr>
<tr id="row5344mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5346mcpsimp"><a name="p5346mcpsimp"></a><a name="p5346mcpsimp"></a>vbb</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5348mcpsimp"><a name="p5348mcpsimp"></a><a name="p5348mcpsimp"></a>垂直消隐后肩，隔行输出时表示顶场垂直消隐后肩。单位：行。</p>
</td>
</tr>
<tr id="row5349mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5351mcpsimp"><a name="p5351mcpsimp"></a><a name="p5351mcpsimp"></a>vact</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5353mcpsimp"><a name="p5353mcpsimp"></a><a name="p5353mcpsimp"></a>垂直有效区，隔行输出时表示顶场垂直有效区。单位：行。</p>
</td>
</tr>
<tr id="row5354mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5356mcpsimp"><a name="p5356mcpsimp"></a><a name="p5356mcpsimp"></a>hfb</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5358mcpsimp"><a name="p5358mcpsimp"></a><a name="p5358mcpsimp"></a>水平消隐前肩。单位：像素。</p>
</td>
</tr>
<tr id="row5359mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5361mcpsimp"><a name="p5361mcpsimp"></a><a name="p5361mcpsimp"></a>hbb</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5363mcpsimp"><a name="p5363mcpsimp"></a><a name="p5363mcpsimp"></a>水平消隐后肩。单位：像素。</p>
</td>
</tr>
<tr id="row5364mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5366mcpsimp"><a name="p5366mcpsimp"></a><a name="p5366mcpsimp"></a>hact</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5368mcpsimp"><a name="p5368mcpsimp"></a><a name="p5368mcpsimp"></a>水平有效区。单位：像素。</p>
</td>
</tr>
<tr id="row5369mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5371mcpsimp"><a name="p5371mcpsimp"></a><a name="p5371mcpsimp"></a>vpw</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5373mcpsimp"><a name="p5373mcpsimp"></a><a name="p5373mcpsimp"></a>垂直同步信号的宽度。单位：行。</p>
</td>
</tr>
<tr id="row5374mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5376mcpsimp"><a name="p5376mcpsimp"></a><a name="p5376mcpsimp"></a>hpw</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5378mcpsimp"><a name="p5378mcpsimp"></a><a name="p5378mcpsimp"></a>水平同步信号的宽度。单位：像素。</p>
</td>
</tr>
<tr id="row5379mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5381mcpsimp"><a name="p5381mcpsimp"></a><a name="p5381mcpsimp"></a>idv</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5383mcpsimp"><a name="p5383mcpsimp"></a><a name="p5383mcpsimp"></a>数据有效信号的极性。配置0为高有效，配置1为低有效。</p>
</td>
</tr>
<tr id="row5384mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5386mcpsimp"><a name="p5386mcpsimp"></a><a name="p5386mcpsimp"></a>ihs</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5388mcpsimp"><a name="p5388mcpsimp"></a><a name="p5388mcpsimp"></a>水平有效信号的极性，配置0为高有效，配置1为低有效。</p>
</td>
</tr>
<tr id="row5389mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5391mcpsimp"><a name="p5391mcpsimp"></a><a name="p5391mcpsimp"></a>ivs</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5393mcpsimp"><a name="p5393mcpsimp"></a><a name="p5393mcpsimp"></a>垂直有效信号的极性，配置0为高有效，配置1为低有效。</p>
</td>
</tr>
<tr id="row5394mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5396mcpsimp"><a name="p5396mcpsimp"></a><a name="p5396mcpsimp"></a>img_width</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5398mcpsimp"><a name="p5398mcpsimp"></a><a name="p5398mcpsimp"></a>图像宽度</p>
</td>
</tr>
<tr id="row5399mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5401mcpsimp"><a name="p5401mcpsimp"></a><a name="p5401mcpsimp"></a>img_height</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5403mcpsimp"><a name="p5403mcpsimp"></a><a name="p5403mcpsimp"></a>图像高度</p>
</td>
</tr>
<tr id="row5404mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5406mcpsimp"><a name="p5406mcpsimp"></a><a name="p5406mcpsimp"></a>aspect_ratio_w</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5408mcpsimp"><a name="p5408mcpsimp"></a><a name="p5408mcpsimp"></a>宽高比宽</p>
</td>
</tr>
<tr id="row5409mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5411mcpsimp"><a name="p5411mcpsimp"></a><a name="p5411mcpsimp"></a>aspect_ratio_h</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5413mcpsimp"><a name="p5413mcpsimp"></a><a name="p5413mcpsimp"></a>宽高比高</p>
</td>
</tr>
<tr id="row5414mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5416mcpsimp"><a name="p5416mcpsimp"></a><a name="p5416mcpsimp"></a>interlace</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5418mcpsimp"><a name="p5418mcpsimp"></a><a name="p5418mcpsimp"></a>隔行标志</p>
</td>
</tr>
<tr id="row5419mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5421mcpsimp"><a name="p5421mcpsimp"></a><a name="p5421mcpsimp"></a>pixel_clk</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5423mcpsimp"><a name="p5423mcpsimp"></a><a name="p5423mcpsimp"></a>时序的像素时钟</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_hdmi\_sink\_capability](#ZH-CN_TOPIC_0000002408254610)
-   [ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)

## ot\_hdmi\_detail\_timing<a name="ZH-CN_TOPIC_0000002408254598"></a>

【说明】

定义HDMI Detail Timing能力集信息。

【定义】

```
typedef struct {
    td_u32                    detail_timing_num;
    ot_hdmi_timing_info     detail_timing[OT_HDMI_DETAIL_TIMING_MAX];
} ot_hdmi_detail_timing;
```

【成员】

<a name="table5776mcpsimp"></a>
<table><thead align="left"><tr id="row5781mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p5783mcpsimp"><a name="p5783mcpsimp"></a><a name="p5783mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p5785mcpsimp"><a name="p5785mcpsimp"></a><a name="p5785mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5787mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5789mcpsimp"><a name="p5789mcpsimp"></a><a name="p5789mcpsimp"></a>detail_timing_num</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5791mcpsimp"><a name="p5791mcpsimp"></a><a name="p5791mcpsimp"></a>支持的详细时序个数。</p>
</td>
</tr>
<tr id="row5792mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5794mcpsimp"><a name="p5794mcpsimp"></a><a name="p5794mcpsimp"></a>detail_timing[<a href="#ZH-CN_TOPIC_0000002408094718">OT_HDMI_DETAIL_TIMING_MAX</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5797mcpsimp"><a name="p5797mcpsimp"></a><a name="p5797mcpsimp"></a>支持的详细时序信息。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_hdmi\_sink\_capability](#ZH-CN_TOPIC_0000002408254610)
-   [ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)

## ot\_hdmi\_sink\_capability<a name="ZH-CN_TOPIC_0000002408254610"></a>

【说明】

定义HDMI Sink端能力集结构体。

【定义】

```
typedef struct {
    td_bool             is_connected;
    td_bool             support_hdmi;
    td_bool             is_sink_power_on;
    ot_hdmi_video_format   native_video_format;
    td_bool             support_video_format [OT_HDMI_VIDEO_FORMAT_BUTT];
    td_bool             support_ycbcr;
    td_bool             support_xvycc601;
    td_bool             support_xvycc709;
    td_u8               md_bit;
    td_u32              audio_info_num;
    ot_hdmi_audio_info  audio_info[OT_HDMI_MAX_AUDIO_CAPBILITY_CNT];
    td_bool             speaker[OT_HDMI_AUDIO_SPEAKER_BUTT];
    td_u8               manufacture_name[OT_HDMI_MANUFACTURE_NAME_LEN];
    td_u32              pdt_code;
    td_u32              serial_num;
    td_u32              week_of_manufacture;
    td_u32              year_of_manufacture;
    td_u8               version;
    td_u8               revision;
    td_u8               edid_ex_blk_num;
    td_bool             is_phys_addr_valid;
    td_u8               phys_addr_a;
    td_u8               phys_addr_b;
    td_u8               phys_addr_c;
    td_u8               phys_addr_d;
    td_bool             support_dvi_dual;
    td_bool             support_deepcolor_ycbcr444;
    td_bool             support_deepcolor_30bit;
    td_bool             support_deepcolor_36bit;
    td_bool             support_deepcolor_48bit;
    td_bool             support_ai;
    td_u32              max_tmds_clk;
    td_bool             i_latency_fields_present;
    td_bool             latency_fields_present;
    td_bool             hdmi_video_present;
    td_u8               video_latency;
    td_u8               audio_latency;
    td_u8               interlaced_video_latency;
    td_u8               interlaced_audio_latency;
    td_bool             support_y420_dc_30bit;
    td_bool             support_y420_dc_36bit;
    td_bool             support_y420_dc_48bit;
    td_bool             support_hdmi_2_0;
    td_bool             support_y420_format[OT_HDMI_VIDEO_FORMAT_BUTT];
    td_bool             only_support_y420_format[OT_HDMI_VIDEO_FORMAT_BUTT];
    td_bool             ycc_quant_selectable;
    td_bool             rgb_quant_selectable;
    td_bool             support_hdr;
    ot_hdmi_hdr_capability  hdr;
    ot_hdmi_detail_timing  detailed_timing;
} ot_hdmi_sink_capability;
```

【成员】

<a name="table4827mcpsimp"></a>
<table><thead align="left"><tr id="row4832mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p4834mcpsimp"><a name="p4834mcpsimp"></a><a name="p4834mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p4836mcpsimp"><a name="p4836mcpsimp"></a><a name="p4836mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4838mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4840mcpsimp"><a name="p4840mcpsimp"></a><a name="p4840mcpsimp"></a>is_connected</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4842mcpsimp"><a name="p4842mcpsimp"></a><a name="p4842mcpsimp"></a>设备是否连接</p>
</td>
</tr>
<tr id="row4843mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4845mcpsimp"><a name="p4845mcpsimp"></a><a name="p4845mcpsimp"></a>support_hdmi</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4847mcpsimp"><a name="p4847mcpsimp"></a><a name="p4847mcpsimp"></a>设备是否支持HDMI（默认为HDMI1.4），如果不支持，则为DVI设备。</p>
</td>
</tr>
<tr id="row4848mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4850mcpsimp"><a name="p4850mcpsimp"></a><a name="p4850mcpsimp"></a>is_sink_power_on</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4852mcpsimp"><a name="p4852mcpsimp"></a><a name="p4852mcpsimp"></a>Sink设备是否上电。</p>
</td>
</tr>
<tr id="row4853mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4855mcpsimp"><a name="p4855mcpsimp"></a><a name="p4855mcpsimp"></a>native_video_format</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4857mcpsimp"><a name="p4857mcpsimp"></a><a name="p4857mcpsimp"></a>显示设备物理分辨率。</p>
</td>
</tr>
<tr id="row4858mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4860mcpsimp"><a name="p4860mcpsimp"></a><a name="p4860mcpsimp"></a>support_video_format</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4862mcpsimp"><a name="p4862mcpsimp"></a><a name="p4862mcpsimp"></a>视频能力集。</p>
<p id="p4863mcpsimp"><a name="p4863mcpsimp"></a><a name="p4863mcpsimp"></a>TD_TRUE：支持这种显示格式；</p>
<p id="p4864mcpsimp"><a name="p4864mcpsimp"></a><a name="p4864mcpsimp"></a>TD_FALSE：不支持。</p>
</td>
</tr>
<tr id="row4865mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4867mcpsimp"><a name="p4867mcpsimp"></a><a name="p4867mcpsimp"></a>support_ycbcr</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4869mcpsimp"><a name="p4869mcpsimp"></a><a name="p4869mcpsimp"></a>是否支持YCBCR显示。</p>
<p id="p4870mcpsimp"><a name="p4870mcpsimp"></a><a name="p4870mcpsimp"></a>TD_TRUE：支持YCBCR显示；</p>
<p id="p4871mcpsimp"><a name="p4871mcpsimp"></a><a name="p4871mcpsimp"></a>TD_FALSE：只支持RGB显示。</p>
</td>
</tr>
<tr id="row4872mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4874mcpsimp"><a name="p4874mcpsimp"></a><a name="p4874mcpsimp"></a>support_xvycc601</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4876mcpsimp"><a name="p4876mcpsimp"></a><a name="p4876mcpsimp"></a>是否支持xvYCC601颜色格式。</p>
</td>
</tr>
<tr id="row4877mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4879mcpsimp"><a name="p4879mcpsimp"></a><a name="p4879mcpsimp"></a>support_xvycc709</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4881mcpsimp"><a name="p4881mcpsimp"></a><a name="p4881mcpsimp"></a>是否支持xvYCC709颜色格式。</p>
</td>
</tr>
<tr id="row4882mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4884mcpsimp"><a name="p4884mcpsimp"></a><a name="p4884mcpsimp"></a>md_bit</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4886mcpsimp"><a name="p4886mcpsimp"></a><a name="p4886mcpsimp"></a>xvYCC601支持的传输profile:1:P0,2:P1,4:P2</p>
</td>
</tr>
<tr id="row4887mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4889mcpsimp"><a name="p4889mcpsimp"></a><a name="p4889mcpsimp"></a>audio_info_num</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4891mcpsimp"><a name="p4891mcpsimp"></a><a name="p4891mcpsimp"></a>支持的音频信息个数，取值范围[1~16]</p>
</td>
</tr>
<tr id="row4892mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4894mcpsimp"><a name="p4894mcpsimp"></a><a name="p4894mcpsimp"></a>audio_info</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4896mcpsimp"><a name="p4896mcpsimp"></a><a name="p4896mcpsimp"></a>支持的音频信息。最多支持16组数据。每组音频信息包括:音频编码格式、采样率、Channel个数、采样bit深度及其个数、最大bit率。</p>
<p id="p4897mcpsimp"><a name="p4897mcpsimp"></a><a name="p4897mcpsimp"></a><strong id="b4898mcpsimp"><a name="b4898mcpsimp"></a><a name="b4898mcpsimp"></a>注意</strong>：OT_HDMI_MAX_AUDIO_CAPBILITY_CNT值为16。</p>
<p id="p4899mcpsimp"><a name="p4899mcpsimp"></a><a name="p4899mcpsimp"></a>详细请参考EIA-CEA-861-F 7.5.2 Audio Data Block</p>
</td>
</tr>
<tr id="row4900mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4902mcpsimp"><a name="p4902mcpsimp"></a><a name="p4902mcpsimp"></a>speaker</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4904mcpsimp"><a name="p4904mcpsimp"></a><a name="p4904mcpsimp"></a>扬声器位置，请参考EIA-CEA-861-F中SpeakerDATABlock的定义。</p>
</td>
</tr>
<tr id="row4905mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4907mcpsimp"><a name="p4907mcpsimp"></a><a name="p4907mcpsimp"></a>manufacture_name</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4909mcpsimp"><a name="p4909mcpsimp"></a><a name="p4909mcpsimp"></a>设备厂商标识。</p>
<p id="p4910mcpsimp"><a name="p4910mcpsimp"></a><a name="p4910mcpsimp"></a><strong id="b4911mcpsimp"><a name="b4911mcpsimp"></a><a name="b4911mcpsimp"></a>注意</strong>：OT_HDMI_MANUFACTURE_NAME_LEN值为4。</p>
</td>
</tr>
<tr id="row4912mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4914mcpsimp"><a name="p4914mcpsimp"></a><a name="p4914mcpsimp"></a>pdt_code</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4916mcpsimp"><a name="p4916mcpsimp"></a><a name="p4916mcpsimp"></a>设备ID。</p>
</td>
</tr>
<tr id="row4917mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4919mcpsimp"><a name="p4919mcpsimp"></a><a name="p4919mcpsimp"></a>serial_num</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4921mcpsimp"><a name="p4921mcpsimp"></a><a name="p4921mcpsimp"></a>设备序列号。</p>
</td>
</tr>
<tr id="row4922mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4924mcpsimp"><a name="p4924mcpsimp"></a><a name="p4924mcpsimp"></a>week_of_manufacture</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4926mcpsimp"><a name="p4926mcpsimp"></a><a name="p4926mcpsimp"></a>设备生产日期(周)。</p>
</td>
</tr>
<tr id="row4927mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4929mcpsimp"><a name="p4929mcpsimp"></a><a name="p4929mcpsimp"></a>year_of_manufacture</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4931mcpsimp"><a name="p4931mcpsimp"></a><a name="p4931mcpsimp"></a>设备生产日期(年)。</p>
</td>
</tr>
<tr id="row4932mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4934mcpsimp"><a name="p4934mcpsimp"></a><a name="p4934mcpsimp"></a>version</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4936mcpsimp"><a name="p4936mcpsimp"></a><a name="p4936mcpsimp"></a>设备版本号。</p>
</td>
</tr>
<tr id="row4937mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4939mcpsimp"><a name="p4939mcpsimp"></a><a name="p4939mcpsimp"></a>revision</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4941mcpsimp"><a name="p4941mcpsimp"></a><a name="p4941mcpsimp"></a>设备子版本号。</p>
</td>
</tr>
<tr id="row4942mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4944mcpsimp"><a name="p4944mcpsimp"></a><a name="p4944mcpsimp"></a>edid_ex_blk_num</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4946mcpsimp"><a name="p4946mcpsimp"></a><a name="p4946mcpsimp"></a>EDID扩展块数目。</p>
</td>
</tr>
<tr id="row4947mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4949mcpsimp"><a name="p4949mcpsimp"></a><a name="p4949mcpsimp"></a>is_phys_addr_valid</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4951mcpsimp"><a name="p4951mcpsimp"></a><a name="p4951mcpsimp"></a>CEC物理地址是否有效标志。</p>
</td>
</tr>
<tr id="row4952mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4954mcpsimp"><a name="p4954mcpsimp"></a><a name="p4954mcpsimp"></a>phys_addr_a</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4956mcpsimp"><a name="p4956mcpsimp"></a><a name="p4956mcpsimp"></a>CEC物理地址A。</p>
</td>
</tr>
<tr id="row4957mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4959mcpsimp"><a name="p4959mcpsimp"></a><a name="p4959mcpsimp"></a>phys_addr_b</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4961mcpsimp"><a name="p4961mcpsimp"></a><a name="p4961mcpsimp"></a>CEC物理地址B。</p>
</td>
</tr>
<tr id="row4962mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4964mcpsimp"><a name="p4964mcpsimp"></a><a name="p4964mcpsimp"></a>phys_addr_c</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4966mcpsimp"><a name="p4966mcpsimp"></a><a name="p4966mcpsimp"></a>CEC物理地址C。</p>
</td>
</tr>
<tr id="row4967mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4969mcpsimp"><a name="p4969mcpsimp"></a><a name="p4969mcpsimp"></a>phys_addr_d</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4971mcpsimp"><a name="p4971mcpsimp"></a><a name="p4971mcpsimp"></a>CEC物理地址D。</p>
</td>
</tr>
<tr id="row4972mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4974mcpsimp"><a name="p4974mcpsimp"></a><a name="p4974mcpsimp"></a>support_dvi_dual</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4976mcpsimp"><a name="p4976mcpsimp"></a><a name="p4976mcpsimp"></a>是否支持 DVI dual-link 操作。</p>
</td>
</tr>
<tr id="row4977mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4979mcpsimp"><a name="p4979mcpsimp"></a><a name="p4979mcpsimp"></a>support_deepcolor_ycbcr444</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4981mcpsimp"><a name="p4981mcpsimp"></a><a name="p4981mcpsimp"></a>是否支持 YCBCR 4:4:4 Deep Color模式。</p>
</td>
</tr>
<tr id="row4982mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4984mcpsimp"><a name="p4984mcpsimp"></a><a name="p4984mcpsimp"></a>support_deepcolor_30bit</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4986mcpsimp"><a name="p4986mcpsimp"></a><a name="p4986mcpsimp"></a>是否支持Deep Color 30bit 模式。</p>
</td>
</tr>
<tr id="row4987mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4989mcpsimp"><a name="p4989mcpsimp"></a><a name="p4989mcpsimp"></a>support_deepcolor_36bit</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4991mcpsimp"><a name="p4991mcpsimp"></a><a name="p4991mcpsimp"></a>是否支持Deep Color 36bit 模式。</p>
</td>
</tr>
<tr id="row4992mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4994mcpsimp"><a name="p4994mcpsimp"></a><a name="p4994mcpsimp"></a>support_deepcolor_48bit</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4996mcpsimp"><a name="p4996mcpsimp"></a><a name="p4996mcpsimp"></a>是否支持Deep Color 48bit 模式。</p>
</td>
</tr>
<tr id="row4997mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4999mcpsimp"><a name="p4999mcpsimp"></a><a name="p4999mcpsimp"></a>support_ai</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5001mcpsimp"><a name="p5001mcpsimp"></a><a name="p5001mcpsimp"></a>是否支持 Supports_AI 模式。</p>
</td>
</tr>
<tr id="row5002mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5004mcpsimp"><a name="p5004mcpsimp"></a><a name="p5004mcpsimp"></a>max_tmds_clk</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5006mcpsimp"><a name="p5006mcpsimp"></a><a name="p5006mcpsimp"></a>最大TMDS时钟。</p>
</td>
</tr>
<tr id="row5007mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5009mcpsimp"><a name="p5009mcpsimp"></a><a name="p5009mcpsimp"></a>i_latency_fields_present</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5011mcpsimp"><a name="p5011mcpsimp"></a><a name="p5011mcpsimp"></a>延时标志位。</p>
</td>
</tr>
<tr id="row5012mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5014mcpsimp"><a name="p5014mcpsimp"></a><a name="p5014mcpsimp"></a>latency_fields_present</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5016mcpsimp"><a name="p5016mcpsimp"></a><a name="p5016mcpsimp"></a>Video_Latency和Audio_Latency fields是否存在。</p>
</td>
</tr>
<tr id="row5017mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5019mcpsimp"><a name="p5019mcpsimp"></a><a name="p5019mcpsimp"></a>hdmi_video_present</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5021mcpsimp"><a name="p5021mcpsimp"></a><a name="p5021mcpsimp"></a>特殊的视频格式。</p>
</td>
</tr>
<tr id="row5022mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5024mcpsimp"><a name="p5024mcpsimp"></a><a name="p5024mcpsimp"></a>video_latency</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5026mcpsimp"><a name="p5026mcpsimp"></a><a name="p5026mcpsimp"></a>视频延时。</p>
</td>
</tr>
<tr id="row5027mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5029mcpsimp"><a name="p5029mcpsimp"></a><a name="p5029mcpsimp"></a>audio_latency</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5031mcpsimp"><a name="p5031mcpsimp"></a><a name="p5031mcpsimp"></a>音频延时。</p>
</td>
</tr>
<tr id="row5032mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5034mcpsimp"><a name="p5034mcpsimp"></a><a name="p5034mcpsimp"></a>interlaced_video_latency</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5036mcpsimp"><a name="p5036mcpsimp"></a><a name="p5036mcpsimp"></a>隔行视频模式下的视频延时。</p>
</td>
</tr>
<tr id="row5037mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5039mcpsimp"><a name="p5039mcpsimp"></a><a name="p5039mcpsimp"></a>interlaced_audio_latency</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5041mcpsimp"><a name="p5041mcpsimp"></a><a name="p5041mcpsimp"></a>隔行视频模式下的音频延时。</p>
</td>
</tr>
<tr id="row5042mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5044mcpsimp"><a name="p5044mcpsimp"></a><a name="p5044mcpsimp"></a>support_y420_dc_30bit</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5046mcpsimp"><a name="p5046mcpsimp"></a><a name="p5046mcpsimp"></a>是否支持YCbCr420 的deep-color 30bit模式。</p>
</td>
</tr>
<tr id="row5047mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5049mcpsimp"><a name="p5049mcpsimp"></a><a name="p5049mcpsimp"></a>support_y420_dc_36bit</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5051mcpsimp"><a name="p5051mcpsimp"></a><a name="p5051mcpsimp"></a>是否支持YCbCr420 的deep-color 36bit模式。</p>
</td>
</tr>
<tr id="row5052mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5054mcpsimp"><a name="p5054mcpsimp"></a><a name="p5054mcpsimp"></a>support_y420_dc_48bit</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5056mcpsimp"><a name="p5056mcpsimp"></a><a name="p5056mcpsimp"></a>是否支持YCbCr420 的deep-color 48bit模式。</p>
</td>
</tr>
<tr id="row5057mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5059mcpsimp"><a name="p5059mcpsimp"></a><a name="p5059mcpsimp"></a>support_hdmi_2_0</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5061mcpsimp"><a name="p5061mcpsimp"></a><a name="p5061mcpsimp"></a>是否支持HDMI2.0。</p>
</td>
</tr>
<tr id="row5062mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5064mcpsimp"><a name="p5064mcpsimp"></a><a name="p5064mcpsimp"></a>support_y420_format</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5066mcpsimp"><a name="p5066mcpsimp"></a><a name="p5066mcpsimp"></a>支持YCbCr420的视频制式。</p>
</td>
</tr>
<tr id="row5067mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5069mcpsimp"><a name="p5069mcpsimp"></a><a name="p5069mcpsimp"></a>only_support_y420_format</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5071mcpsimp"><a name="p5071mcpsimp"></a><a name="p5071mcpsimp"></a>只支持YCbCr420的视频制式。</p>
</td>
</tr>
<tr id="row5072mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5074mcpsimp"><a name="p5074mcpsimp"></a><a name="p5074mcpsimp"></a>ycc_quant_selectable</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5076mcpsimp"><a name="p5076mcpsimp"></a><a name="p5076mcpsimp"></a>是否支持YCbCr Quantization可选。</p>
</td>
</tr>
<tr id="row5077mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5079mcpsimp"><a name="p5079mcpsimp"></a><a name="p5079mcpsimp"></a>rgb_quant_selectable</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5081mcpsimp"><a name="p5081mcpsimp"></a><a name="p5081mcpsimp"></a>是否支持RGB Quantization可选。</p>
</td>
</tr>
<tr id="row5082mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5084mcpsimp"><a name="p5084mcpsimp"></a><a name="p5084mcpsimp"></a>support_hdr</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5086mcpsimp"><a name="p5086mcpsimp"></a><a name="p5086mcpsimp"></a>Sink是否支持HDR。</p>
</td>
</tr>
<tr id="row5087mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5089mcpsimp"><a name="p5089mcpsimp"></a><a name="p5089mcpsimp"></a>hdr</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5091mcpsimp"><a name="p5091mcpsimp"></a><a name="p5091mcpsimp"></a>Sink支持HDR能力的详细信息。</p>
</td>
</tr>
<tr id="row5092mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5094mcpsimp"><a name="p5094mcpsimp"></a><a name="p5094mcpsimp"></a>detailed_timing</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5096mcpsimp"><a name="p5096mcpsimp"></a><a name="p5096mcpsimp"></a>支持的详细时序能力集信息。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

能力上报不区分HDMI1.4和HDMI2.0。

【相关数据类型及接口】

[ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)

## ot\_hdmi\_edid<a name="ZH-CN_TOPIC_0000002408094698"></a>

【说明】

定义HDMI EDID信息结构体。

【定义】

```
typedef struct {
    td_bool                 edid_valid;
    td_u32                  edid_len;
    td_u8                   edid[OT_HDMI_EDID_RAW_DATA_LEN];
} ot_hdmi_edid;
```

【成员】

<a name="table4636mcpsimp"></a>
<table><thead align="left"><tr id="row4641mcpsimp"><th class="cellrowborder" valign="top" width="45%" id="mcps1.1.3.1.1"><p id="p4643mcpsimp"><a name="p4643mcpsimp"></a><a name="p4643mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.3.1.2"><p id="p4645mcpsimp"><a name="p4645mcpsimp"></a><a name="p4645mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4647mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p4649mcpsimp"><a name="p4649mcpsimp"></a><a name="p4649mcpsimp"></a>edid_valid</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p4651mcpsimp"><a name="p4651mcpsimp"></a><a name="p4651mcpsimp"></a><span xml:lang="fr-FR" id="ph4652mcpsimp"><a name="ph4652mcpsimp"></a><a name="ph4652mcpsimp"></a>EDID</span>信息是否有效。</p>
</td>
</tr>
<tr id="row4653mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p4655mcpsimp"><a name="p4655mcpsimp"></a><a name="p4655mcpsimp"></a>edid_len</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p4657mcpsimp"><a name="p4657mcpsimp"></a><a name="p4657mcpsimp"></a>EDID信息数据长度。</p>
</td>
</tr>
<tr id="row4658mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p4660mcpsimp"><a name="p4660mcpsimp"></a><a name="p4660mcpsimp"></a>edid[<a href="#ZH-CN_TOPIC_0000002441693865">OT_HDMI_EDID_RAW_DATA_LEN</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p4663mcpsimp"><a name="p4663mcpsimp"></a><a name="p4663mcpsimp"></a>EDID信息数据。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

[ss\_mpi\_hdmi\_force\_get\_edid](#ZH-CN_TOPIC_0000002441693877)

## ot\_hdmi\_infoframe\_type<a name="ZH-CN_TOPIC_0000002408254602"></a>

【说明】

定义HDMI信息帧类型枚举。

【定义】

```
typedef enum {
    OT_INFOFRAME_TYPE_AVI,
    OT_INFOFRAME_TYPE_AUDIO,
    OT_INFOFRAME_TYPE_VENDORSPEC,
    OT_INFOFRAME_TYPE_BUTT
} ot_hdmi_infoframe_type;
```

【成员】

无。

【注意事项】

无

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_infoframe<a name="ZH-CN_TOPIC_0000002408254606"></a>

【说明】

定义HDMI信息帧结构体。

【定义】

```
typedef struct {
    ot_hdmi_infoframe_type  infoframe_type;
    ot_hdmi_infoframe_unit  infoframe_unit;
} ot_hdmi_infoframe;
```

【成员】

<a name="table5443mcpsimp"></a>
<table><thead align="left"><tr id="row5448mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p5450mcpsimp"><a name="p5450mcpsimp"></a><a name="p5450mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p5452mcpsimp"><a name="p5452mcpsimp"></a><a name="p5452mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5454mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5456mcpsimp"><a name="p5456mcpsimp"></a><a name="p5456mcpsimp"></a>infoframe_type</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5458mcpsimp"><a name="p5458mcpsimp"></a><a name="p5458mcpsimp"></a>信息帧类型</p>
</td>
</tr>
<tr id="row5459mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5461mcpsimp"><a name="p5461mcpsimp"></a><a name="p5461mcpsimp"></a>infoframe_unit</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5463mcpsimp"><a name="p5463mcpsimp"></a><a name="p5463mcpsimp"></a>信息帧单元(内容)</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_infoframe\_unit<a name="ZH-CN_TOPIC_0000002441693841"></a>

【说明】

定义HDMI信息帧单元结构体。

【定义】

```
typedef union {
    ot_hdmi_avi_infoframe   avi_infoframe;
    ot_hdmi_audio_infoframe   audio_infoframe;
    ot_hdmi_vendorspec_infoframe vendor_spec_infoframe;
} ot_hdmi_infoframe_unit;
```

【成员】

<a name="table3447mcpsimp"></a>
<table><thead align="left"><tr id="row3452mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p3454mcpsimp"><a name="p3454mcpsimp"></a><a name="p3454mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p3456mcpsimp"><a name="p3456mcpsimp"></a><a name="p3456mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3458mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p3460mcpsimp"><a name="p3460mcpsimp"></a><a name="p3460mcpsimp"></a>avi_infoframe</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p3462mcpsimp"><a name="p3462mcpsimp"></a><a name="p3462mcpsimp"></a>AVI信息帧单元</p>
</td>
</tr>
<tr id="row3463mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p3465mcpsimp"><a name="p3465mcpsimp"></a><a name="p3465mcpsimp"></a>audio_infoframe</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p3467mcpsimp"><a name="p3467mcpsimp"></a><a name="p3467mcpsimp"></a>AUDIO信息帧单元</p>
</td>
</tr>
<tr id="row3468mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p3470mcpsimp"><a name="p3470mcpsimp"></a><a name="p3470mcpsimp"></a>vendor_spec_infoframe</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p3472mcpsimp"><a name="p3472mcpsimp"></a><a name="p3472mcpsimp"></a>VENDOR信息帧单元</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_avi\_infoframe<a name="ZH-CN_TOPIC_0000002408254570"></a>

【说明】

定义HDMI AVI信息帧（版本2）单元结构体。

【定义】

```
typedef struct {
    ot_hdmi_video_format       timing_mode;
    ot_hdmi_color_space        color_space;
    td_bool                    active_info_present;
    ot_hdmi_bar_info           bar_info;
    ot_hdmi_scan_info          scan_info;
    ot_hdmi_colorimetry         colorimetry;
    ot_hdmi_ex_colorimetry     ex_colorimetry;
    ot_pic_aspect_ratio        aspect_ratio;
    ot_hdmi_active_aspect_ratio   active_aspect_ratio;
    ot_hdmi_pic_scaline    pic_scaling;
    ot_hdmi_rgb_quant_range      rgb_quant;
    td_bool                       is_it_content;
    ot_hdmi_pixel_repetition   pixel_repetition;
    ot_hdmi_content_type       content_type;
    ot_hdmi_ycc_quant_range      ycc_quant;
    td_u16                     line_n_end_of_top_bar;
    td_u16                     line_n_start_of_bot_bar;
    td_u16                     pixel_n_end_of_left_bar;
    td_u16                     pixel_n_start_of_right_bar;
} ot_hdmi_avi_infoframe;
```

【成员】

<a name="table4510mcpsimp"></a>
<table><thead align="left"><tr id="row4515mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p4517mcpsimp"><a name="p4517mcpsimp"></a><a name="p4517mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p4519mcpsimp"><a name="p4519mcpsimp"></a><a name="p4519mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4521mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4523mcpsimp"><a name="p4523mcpsimp"></a><a name="p4523mcpsimp"></a>timing_mode</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4525mcpsimp"><a name="p4525mcpsimp"></a><a name="p4525mcpsimp"></a>视频时序</p>
</td>
</tr>
<tr id="row4526mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4528mcpsimp"><a name="p4528mcpsimp"></a><a name="p4528mcpsimp"></a>color_space</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4530mcpsimp"><a name="p4530mcpsimp"></a><a name="p4530mcpsimp"></a>颜色空间</p>
</td>
</tr>
<tr id="row4531mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4533mcpsimp"><a name="p4533mcpsimp"></a><a name="p4533mcpsimp"></a>active_info_present</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4535mcpsimp"><a name="p4535mcpsimp"></a><a name="p4535mcpsimp"></a>Info是否有效</p>
</td>
</tr>
<tr id="row4536mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4538mcpsimp"><a name="p4538mcpsimp"></a><a name="p4538mcpsimp"></a>bar_info</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4540mcpsimp"><a name="p4540mcpsimp"></a><a name="p4540mcpsimp"></a>Bar信息</p>
</td>
</tr>
<tr id="row4541mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4543mcpsimp"><a name="p4543mcpsimp"></a><a name="p4543mcpsimp"></a>scan_info</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4545mcpsimp"><a name="p4545mcpsimp"></a><a name="p4545mcpsimp"></a>Scan信息</p>
</td>
</tr>
<tr id="row4546mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4548mcpsimp"><a name="p4548mcpsimp"></a><a name="p4548mcpsimp"></a>colorimetry</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4550mcpsimp"><a name="p4550mcpsimp"></a><a name="p4550mcpsimp"></a>色域</p>
</td>
</tr>
<tr id="row4551mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4553mcpsimp"><a name="p4553mcpsimp"></a><a name="p4553mcpsimp"></a>ex_colorimetry</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4555mcpsimp"><a name="p4555mcpsimp"></a><a name="p4555mcpsimp"></a>扩展色域</p>
</td>
</tr>
<tr id="row4556mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4558mcpsimp"><a name="p4558mcpsimp"></a><a name="p4558mcpsimp"></a>aspect_ratio</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4560mcpsimp"><a name="p4560mcpsimp"></a><a name="p4560mcpsimp"></a>图像宽高比</p>
</td>
</tr>
<tr id="row4561mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4563mcpsimp"><a name="p4563mcpsimp"></a><a name="p4563mcpsimp"></a>active_aspect_ratio</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4565mcpsimp"><a name="p4565mcpsimp"></a><a name="p4565mcpsimp"></a>有效宽高比</p>
</td>
</tr>
<tr id="row4566mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4568mcpsimp"><a name="p4568mcpsimp"></a><a name="p4568mcpsimp"></a>pic_scaling</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4570mcpsimp"><a name="p4570mcpsimp"></a><a name="p4570mcpsimp"></a>图像均衡</p>
</td>
</tr>
<tr id="row4571mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4573mcpsimp"><a name="p4573mcpsimp"></a><a name="p4573mcpsimp"></a>rgb_quant</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4575mcpsimp"><a name="p4575mcpsimp"></a><a name="p4575mcpsimp"></a>RGB量化</p>
</td>
</tr>
<tr id="row4576mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4578mcpsimp"><a name="p4578mcpsimp"></a><a name="p4578mcpsimp"></a>is_it_content</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4580mcpsimp"><a name="p4580mcpsimp"></a><a name="p4580mcpsimp"></a>ITContent是否有效</p>
</td>
</tr>
<tr id="row4581mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4583mcpsimp"><a name="p4583mcpsimp"></a><a name="p4583mcpsimp"></a>pixel_repetition</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4585mcpsimp"><a name="p4585mcpsimp"></a><a name="p4585mcpsimp"></a>像素加倍</p>
</td>
</tr>
<tr id="row4586mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4588mcpsimp"><a name="p4588mcpsimp"></a><a name="p4588mcpsimp"></a>content_type</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4590mcpsimp"><a name="p4590mcpsimp"></a><a name="p4590mcpsimp"></a>ITContent类型</p>
</td>
</tr>
<tr id="row4591mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4593mcpsimp"><a name="p4593mcpsimp"></a><a name="p4593mcpsimp"></a>ycc_quant</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4595mcpsimp"><a name="p4595mcpsimp"></a><a name="p4595mcpsimp"></a>YCC量化</p>
</td>
</tr>
<tr id="row4596mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4598mcpsimp"><a name="p4598mcpsimp"></a><a name="p4598mcpsimp"></a>line_n_end_of_top_bar</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4600mcpsimp"><a name="p4600mcpsimp"></a><a name="p4600mcpsimp"></a>TopBar终止行数</p>
</td>
</tr>
<tr id="row4601mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4603mcpsimp"><a name="p4603mcpsimp"></a><a name="p4603mcpsimp"></a>line_n_start_of_bot_bar</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4605mcpsimp"><a name="p4605mcpsimp"></a><a name="p4605mcpsimp"></a>BottomBar开始行数</p>
</td>
</tr>
<tr id="row4606mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4608mcpsimp"><a name="p4608mcpsimp"></a><a name="p4608mcpsimp"></a>pixel_n_end_of_left_bar</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4610mcpsimp"><a name="p4610mcpsimp"></a><a name="p4610mcpsimp"></a>LeftBar终止像素数</p>
</td>
</tr>
<tr id="row4611mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4613mcpsimp"><a name="p4613mcpsimp"></a><a name="p4613mcpsimp"></a>pixel_n_start_of_right_bar</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4615mcpsimp"><a name="p4615mcpsimp"></a><a name="p4615mcpsimp"></a>RightBar开始像素数</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

详细描述请参见《High-Definition Multimedia Interface Specification Version 1.4b》、《High-Definition Multimedia Interface Specification Version 2.0》、《CEA-861-D》与《CEA-861-F》

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_audio\_infoframe<a name="ZH-CN_TOPIC_0000002408094714"></a>

【说明】

定义HDMI AUDIO信息帧（版本1）单元结构体。

【定义】

```
typedef struct {
    ot_hdmi_audio_chn_cnt           chn_cnt;
    ot_hdmi_coding_type              coding_type;
    ot_hdmi_audio_sample_size       sample_size;
    ot_hdmi_audio_sample_freq       sampling_freq;
    td_u8                               chn_alloc;
    ot_hdmi_level_shift_val             level_shift;
    ot_hdmi_lfe_playback_level      lfe_playback_level;
    td_bool                             down_mix_inhibit;
} ot_hdmi_audio_infoframe;
```

【成员】

<a name="table2808mcpsimp"></a>
<table><thead align="left"><tr id="row2813mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p2815mcpsimp"><a name="p2815mcpsimp"></a><a name="p2815mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p2817mcpsimp"><a name="p2817mcpsimp"></a><a name="p2817mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2819mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2821mcpsimp"><a name="p2821mcpsimp"></a><a name="p2821mcpsimp"></a>chn_cnt</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2823mcpsimp"><a name="p2823mcpsimp"></a><a name="p2823mcpsimp"></a>音频通道个数</p>
</td>
</tr>
<tr id="row2824mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2826mcpsimp"><a name="p2826mcpsimp"></a><a name="p2826mcpsimp"></a>coding_type</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2828mcpsimp"><a name="p2828mcpsimp"></a><a name="p2828mcpsimp"></a>音频格式</p>
</td>
</tr>
<tr id="row2829mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2831mcpsimp"><a name="p2831mcpsimp"></a><a name="p2831mcpsimp"></a>sample_size</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2833mcpsimp"><a name="p2833mcpsimp"></a><a name="p2833mcpsimp"></a>音频采样深度（位宽）</p>
</td>
</tr>
<tr id="row2834mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2836mcpsimp"><a name="p2836mcpsimp"></a><a name="p2836mcpsimp"></a>sampling_freq</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2838mcpsimp"><a name="p2838mcpsimp"></a><a name="p2838mcpsimp"></a>音频采样率</p>
</td>
</tr>
<tr id="row2839mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2841mcpsimp"><a name="p2841mcpsimp"></a><a name="p2841mcpsimp"></a>chn_alloc</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2843mcpsimp"><a name="p2843mcpsimp"></a><a name="p2843mcpsimp"></a>Channel/Speaker分配</p>
</td>
</tr>
<tr id="row2844mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2846mcpsimp"><a name="p2846mcpsimp"></a><a name="p2846mcpsimp"></a>level_shift</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2848mcpsimp"><a name="p2848mcpsimp"></a><a name="p2848mcpsimp"></a>Level Shift Value，左移值</p>
</td>
</tr>
<tr id="row2849mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2851mcpsimp"><a name="p2851mcpsimp"></a><a name="p2851mcpsimp"></a>lfe_playback_level</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2853mcpsimp"><a name="p2853mcpsimp"></a><a name="p2853mcpsimp"></a>LFE playback level information，LFE 播放等级信息。</p>
</td>
</tr>
<tr id="row2854mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p2856mcpsimp"><a name="p2856mcpsimp"></a><a name="p2856mcpsimp"></a>down_mix_inhibit</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p2858mcpsimp"><a name="p2858mcpsimp"></a><a name="p2858mcpsimp"></a>Down-mix Inhibit 标志位。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

详细描述请参见《High-Definition Multimedia Interface Specification Version 1.4b》、《High-Definition Multimedia Interface Specification Version 2.0》、《CEA-861-D》与《CEA-861-F》

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_vendorspec\_infoframe<a name="ZH-CN_TOPIC_0000002408254614"></a>

【说明】

vendor信息帧结构体。

【定义】

```
typedef struct {
    td_u8 data_len;
    td_u8 user_data[OT_HDMI_VENDOR_USER_DATA_MAX_LEN];
} ot_hdmi_vendorspec_infoframe;
```

【成员】

<a name="table5112mcpsimp"></a>
<table><thead align="left"><tr id="row5117mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p5119mcpsimp"><a name="p5119mcpsimp"></a><a name="p5119mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p5121mcpsimp"><a name="p5121mcpsimp"></a><a name="p5121mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row5123mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5125mcpsimp"><a name="p5125mcpsimp"></a><a name="p5125mcpsimp"></a>data_len</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5127mcpsimp"><a name="p5127mcpsimp"></a><a name="p5127mcpsimp"></a>用户数据的长度</p>
</td>
</tr>
<tr id="row5128mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p5130mcpsimp"><a name="p5130mcpsimp"></a><a name="p5130mcpsimp"></a>user_data[<a href="#ZH-CN_TOPIC_0000002441693845">OT_HDMI_VENDOR_USER_DATA_MAX_LEN</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p5132mcpsimp"><a name="p5132mcpsimp"></a><a name="p5132mcpsimp"></a>用户数据</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

用户可以通过该接口设置自定数据，当data\_len为0时停止发送用户数据。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_color\_space<a name="ZH-CN_TOPIC_0000002441654033"></a>

【说明】

定义颜色空间枚举。

【定义】

```
typedef enum {
    OT_HDMI_COLOR_SPACE_RGB444,
    OT_HDMI_COLOR_SPACE_YCBCR422,
    OT_HDMI_COLOR_SPACE_YCBCR444,
    OT_HDMI_COLOR_SPACE_YCBCR420,
    OT_HDMI_COLOR_SPACE_BUTT
} ot_hdmi_color_space;
```

【成员】

无。

【注意事项】

OT\_HDMI\_COLOR\_SPACE\_YCBCR420**为HDMI2.0规格，仅支持HDMI1.4规格的产品不支持此颜色空间。**

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_bar\_info<a name="ZH-CN_TOPIC_0000002408254622"></a>

【说明】

定义Bar信息枚举。

【定义】

```
typedef enum {
    OT_HDMI_BAR_INFO_NOT_VALID,
    OT_HDMI_BAR_INFO_V,
    OT_HDMI_BAR_INFO_H,
    OT_HDMI_BAR_INFO_VH,
    OT_HDMI_BAR_INFO_BUTT
} ot_hdmi_bar_info;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_scan\_info<a name="ZH-CN_TOPIC_0000002441693869"></a>

【说明】

定义Scan信息枚举。

【定义】

```
typedef enum {
    OT_HDMI_SCAN_INFO_NO_DATA,
    OT_HDMI_SCAN_INFO_OVERSCANNED,
    OT_HDMI_SCAN_INFO_UNDERSCANNED,
    OT_HDMI_SCAN_INFO_BUTT
} ot_hdmi_scan_info;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_colorimetry<a name="ZH-CN_TOPIC_0000002441693873"></a>

【说明】

定义Colorimetry信息枚举。

【定义】

```
typedef enum {
    OT_HDMI_COMMON_COLORIMETRY_NO_DATA,
    OT_HDMI_COMMON_COLORIMETRY_ITU601,
    OT_HDMI_COMMON_COLORIMETRY_ITU709,
    OT_HDMI_COMMON_COLORIMETRY_BUTT
} ot_hdmi_colorimetry;
```

【成员】

无。

【注意事项】

OT\_HDMI\_COMMON\_COLORIMETRY\_BUTT值有效，其含义表示设置为扩展色域，详细描述请参见《CEA-861-D》。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_ex\_colorimetry<a name="ZH-CN_TOPIC_0000002441693857"></a>

【说明】

定义扩展Colorimetry信息枚举。

【定义】

```
typedef enum {
    OT_HDMI_COMMON_COLORIMETRY_XVYCC_601,
    OT_HDMI_COMMON_COLORIMETRY_XVYCC_709,
    OT_HDMI_COMMON_COLORIMETRY_S_YCC_601,
    OT_HDMI_COMMON_COLORIMETRY_ADOBE_YCC_601,
    OT_HDMI_COMMON_COLORIMETRY_ADOBE_RGB,
    OT_HDMI_COMMON_COLORIMETRY_2020_CONST_LUMINOUS,
    OT_HDMI_COMMON_COLORIMETRY_2020_NON_CONST_LUMINOUS,
    OT_HDMI_COMMON_COLORIMETRY_EXT_BUTT
} ot_hdmi_ex_colorimetry;
```

【成员】

无。

【注意事项】

无

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_pic\_aspect\_ratio<a name="ZH-CN_TOPIC_0000002408254586"></a>

【说明】

定义图像宽高比枚举。

【定义】

```
typedef enum {
    OT_HDMI_PIC_ASPECT_RATIO_NO_DATA,
    OT_HDMI_PIC_ASPECT_RATIO_4TO3,
    OT_HDMI_PIC_ASPECT_RATIO_16TO9,
    OT_HDMI_PIC_ASPECT_RATIO_64TO27,
    OT_HDMI_PIC_ASPECT_RATIO_256TO135,
    OT_HDMI_PIC_ASPECT_RATIO_BUTT,
} ot_pic_aspect_ratio;
```

【成员】

无。

【注意事项】

设置枚举以外的值将返回[OT\_ERR\_HDMI\_INVALID\_PARA](#OT_ERR_HDMI_INVALID_PARA)。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_active\_aspect\_ratio<a name="ZH-CN_TOPIC_0000002441653973"></a>

【说明】

定义实际图像宽高比枚举。

【定义】

```
typedef enum {
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_TOP=2,
    OT_HDMI_ACTIVE_ASPECT_RATIO_14TO9_TOP,
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_BOX_CENTER,
    OT_HDMI_ACTIVE_ASPECT_RATIO_SAME_PIC=8,
    OT_HDMI_ACTIVE_ASPECT_RATIO_4TO3_CENTER,
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_CENTER,
    OT_HDMI_ACTIVE_ASPECT_RATIO_14TO9_CENTER,
    OT_HDMI_ACTIVE_ASPECT_RATIO_4TO3_14_9=13,
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_14_9,
    OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_4_3,
    OT_HDMI_ACTIVE_ASPECT_RATIO_BUTT
} ot_hdmi_active_aspect_ratio;
```

【成员】

无。

【注意事项】

无

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_pic\_scaline<a name="ZH-CN_TOPIC_0000002408254626"></a>

【说明】

定义图像扫描信息枚举。

【定义】

```
typedef enum {
    OT_HDMI_PIC_NON_UNIFORM_SCALING,
    OT_HDMI_PIC_SCALING_H,
    OT_HDMI_PIC_SCALING_V,
    OT_HDMI_PIC_SCALING_HV,
    OT_HDMI_PIC_SCALING_BUTT
} ot_hdmi_pic_scaline;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_rgb\_quant\_range<a name="ZH-CN_TOPIC_0000002441693817"></a>

【说明】

定义RGB量化范围枚举。

【定义】

```
typedef enum {
    OT_HDMI_RGB_QUANT_DEFAULT_RANGE,
    OT_HDMI_RGB_QUANT_LIMITED_RANGE,
    OT_HDMI_RGB_QUANT_FULL_RANGE,
    OT_HDMI_RGB_QUANT_FULL_BUTT
} ot_hdmi_rgb_quant_range;
```

【成员】

无。

【注意事项】

无

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_pixel\_repetition<a name="ZH-CN_TOPIC_0000002441653969"></a>

【说明】

定义像素复制次数枚举。

【定义】

```
typedef enum {
    OT_HDMI_PIXEL_REPET_NO,
    OT_HDMI_PIXEL_REPET_2_TIMES,
    OT_HDMI_PIXEL_REPET_3_TIMES,
    OT_HDMI_PIXEL_REPET_4_TIMES,
    OT_HDMI_PIXEL_REPET_5_TIMES,
    OT_HDMI_PIXEL_REPET_6_TIMES,
    OT_HDMI_PIXEL_REPET_7_TIMES,
    OT_HDMI_PIXEL_REPET_8_TIMES,
    OT_HDMI_PIXEL_REPET_9_TIMES,
    OT_HDMI_PIXEL_REPET_10_TIMES,
    OT_HDMI_PIXEL_REPET_BUTT
} ot_hdmi_pixel_repetition;
```

【成员】

无。

【注意事项】

设置枚举以外的值将返回[OT\_ERR\_HDMI\_INVALID\_PARA](#OT_ERR_HDMI_INVALID_PARA)。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_content\_type<a name="ZH-CN_TOPIC_0000002408254630"></a>

【说明】

定义内容信息枚举。

【定义】

```
typedef enum {
    OT_HDMI_CONTNET_GRAPHIC,
    OT_HDMI_CONTNET_PHOTO,
    OT_HDMI_CONTNET_CINEMA,
    OT_HDMI_CONTNET_GAME,
    OT_HDMI_CONTNET_BUTT
} ot_hdmi_content_type;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_ycc\_quant\_range<a name="ZH-CN_TOPIC_0000002408254634"></a>

【说明】

定义YCC量化范围枚举。

【定义】

```
typedef enum {
    OT_HDMI_YCC_QUANT_LIMITED_RANGE,
    OT_HDMI_YCC_QUANT_FULL_RANGE,
    OT_HDMI_YCC_QUANT_BUTT
} ot_hdmi_ycc_quant_range;
```

【成员】

无。

【注意事项】

设置枚举以外的值将返回[OT\_ERR\_HDMI\_INVALID\_PARA](#OT_ERR_HDMI_INVALID_PARA)。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_audio\_chn\_cnt<a name="ZH-CN_TOPIC_0000002441654029"></a>

【说明】

定义音频通道数枚举。

【定义】

```
typedef enum {
    OT_HDMI_AUDIO_CHN_CNT_STREAM,
    OT_HDMI_AUDIO_CHN_CNT_2,
    OT_HDMI_AUDIO_CHN_CNT_3,
    OT_HDMI_AUDIO_CHN_CNT_4,
    OT_HDMI_AUDIO_CHN_CNT_5,
    OT_HDMI_AUDIO_CHN_CNT_6,
    OT_HDMI_AUDIO_CHN_CNT_7,
    OT_HDMI_AUDIO_CHN_CNT_8,
    OT_HDMI_AUDIO_CHN_BUTT
} ot_hdmi_audio_chn_cnt;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_coding\_type<a name="ZH-CN_TOPIC_0000002441693853"></a>

【说明】

定义音频编码类型枚举。

【定义】

```
typedef enum {
    OT_HDMI_AUDIO_CODING_REFER_STREAM_HEAD,
    OT_HDMI_AUDIO_CODING_PCM,
    OT_HDMI_AUDIO_CODING_AC3,
    OT_HDMI_AUDIO_CODING_MPEG1,
    OT_HDMI_AUDIO_CODING_MP3,
    OT_HDMI_AUDIO_CODING_MPEG2,
    OT_HDMI_AUDIO_CODING_AACLC,
    OT_HDMI_AUDIO_CODING_DTS,
    OT_HDMI_AUDIO_CODING_ATRAC,
    OT_HDMI_AUDIO_CODIND_ONE_BIT_AUDIO,
    OT_HDMI_AUDIO_CODING_ENHANCED_AC3,
    OT_HDMI_AUDIO_CODING_DTS_HD,
    OT_HDMI_AUDIO_CODING_MAT,
    OT_HDMI_AUDIO_CODING_DST,
    OT_HDMI_AUDIO_CODING_WMA_PRO,
    OT_HDMI_AUDIO_CODING_BUTT
} ot_hdmi_coding_type;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_audio\_sample\_size<a name="ZH-CN_TOPIC_0000002408254582"></a>

【说明】

定义音频采样大小枚举。

【定义】

```
typedef enum {
    OT_HDMI_AUDIO_SAMPLE_SIZE_STREAM,
    OT_HDMI_AUDIO_SAMPLE_SIZE_16,
    OT_HDMI_AUDIO_SAMPLE_SIZE_20,
    OT_HDMI_AUDIO_SAMPLE_SIZE_24,
    OT_HDMI_AUDIO_SAMPLE_SIZE_BUTT
} ot_hdmi_audio_sample_size;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_audio\_sample\_freq<a name="ZH-CN_TOPIC_0000002441693821"></a>

【说明】

定义音频采样频率枚举。

【定义】

```
typedef enum {
    OT_HDMI_AUDIO_SAMPLE_FREQ_STREAM,
    OT_HDMI_AUDIO_SAMPLE_FREQ_32000,
    OT_HDMI_AUDIO_SAMPLE_FREQ_44100,
    OT_HDMI_AUDIO_SAMPLE_FREQ_48000,
    OT_HDMI_AUDIO_SAMPLE_FREQ_88200,
    OT_HDMI_AUDIO_SAMPLE_FREQ_96000,
    OT_HDMI_AUDIO_SAMPLE_FREQ_176400,
    OT_HDMI_AUDIO_SAMPLE_FREQ_192000,
    OT_HDMI_AUDIO_SAMPLE_FREQ_BUTT
} ot_hdmi_audio_sample_freq;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_level\_shift\_val<a name="ZH-CN_TOPIC_0000002408094670"></a>

【说明】

定义音频Shift信息枚举。

【定义】

```
typedef enum {
    OT_HDMI_LEVEL_SHIFT_VAL_0_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_1_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_2_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_3_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_4_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_5_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_6_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_7_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_8_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_9_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_10_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_11_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_12_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_13_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_14_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_15_DB,
    OT_HDMI_LEVEL_SHIFT_VAL_BUTT
} ot_hdmi_level_shift_val;
```

【成员】

无。

【注意事项】

无。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_lfe\_playback\_level<a name="ZH-CN_TOPIC_0000002408254578"></a>

【说明】

定义音频Playback信息枚举。

【定义】

```
typedef enum {
    OT_HDMI_LFE_PLAYBACK_NO,
    OT_HDMI_LFE_PLAYBACK_0_DB,
    OT_HDMI_LFE_PLAYBACK_10_DB,
    OT_HDMI_LFE_PLAYBACK_BUTT
} ot_hdmi_lfe_playback_level;
```

【成员】

无。

【注意事项】

设置枚举以外的值将返回[OT\_ERR\_HDMI\_INVALID\_PARA](#OT_ERR_HDMI_INVALID_PARA)。

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_infoframe](#ZH-CN_TOPIC_0000002441693881)
-   [ss\_mpi\_hdmi\_get\_infoframe](#ZH-CN_TOPIC_0000002441693885)

## ot\_hdmi\_eotf<a name="ZH-CN_TOPIC_0000002408094662"></a>

【说明】

定义支持的EOTF能力结构体。

【定义】

```
typedef struct {
    td_bool eotf_sdr;
    td_bool eotf_hdr;
    td_bool eotf_smpte_st2084;
    td_bool eotf_hlg;
    td_bool eotf_future;
} ot_hdmi_eotf;
```

【成员】

<a name="table4321mcpsimp"></a>
<table><thead align="left"><tr id="row4326mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p4328mcpsimp"><a name="p4328mcpsimp"></a><a name="p4328mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p4330mcpsimp"><a name="p4330mcpsimp"></a><a name="p4330mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4332mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p4334mcpsimp"><a name="p4334mcpsimp"></a><a name="p4334mcpsimp"></a>eotf_sdr</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p4336mcpsimp"><a name="p4336mcpsimp"></a><a name="p4336mcpsimp"></a>是否支持Traditional Gamma - SDR Luminance Range</p>
</td>
</tr>
<tr id="row4337mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p4339mcpsimp"><a name="p4339mcpsimp"></a><a name="p4339mcpsimp"></a>eotf_hdr</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p4341mcpsimp"><a name="p4341mcpsimp"></a><a name="p4341mcpsimp"></a>是否支持Traditional Gamma - HDR Luminance Range</p>
</td>
</tr>
<tr id="row4342mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p4344mcpsimp"><a name="p4344mcpsimp"></a><a name="p4344mcpsimp"></a>eotf_smpte_st2084</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p4346mcpsimp"><a name="p4346mcpsimp"></a><a name="p4346mcpsimp"></a>是否支持SMPTE ST 2084中定义的EOTF类型</p>
</td>
</tr>
<tr id="row4347mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p4349mcpsimp"><a name="p4349mcpsimp"></a><a name="p4349mcpsimp"></a>eotf_hlg</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p4351mcpsimp"><a name="p4351mcpsimp"></a><a name="p4351mcpsimp"></a>是否支持Hybrid Log-Gamma EOTF类型</p>
</td>
</tr>
<tr id="row4352mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p id="p4354mcpsimp"><a name="p4354mcpsimp"></a><a name="p4354mcpsimp"></a>eotf_future</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p4356mcpsimp"><a name="p4356mcpsimp"></a><a name="p4356mcpsimp"></a>保留</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

-   [ot\_hdmi\_hdr\_capability](#ZH-CN_TOPIC_0000002408094722)
-   [ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)

## ot\_hdmi\_hdr\_metadata\_type<a name="ZH-CN_TOPIC_0000002408094666"></a>

【说明】

定义支持metadata类型结构体。

【定义】

```
typedef struct {
    td_bool descriptor_type1;
} ot_hdmi_hdr_metadata_type;
```

【成员】

<a name="table219mcpsimp"></a>
<table><thead align="left"><tr id="row224mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p226mcpsimp"><a name="p226mcpsimp"></a><a name="p226mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p228mcpsimp"><a name="p228mcpsimp"></a><a name="p228mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row230mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p232mcpsimp"><a name="p232mcpsimp"></a><a name="p232mcpsimp"></a>descriptor_type1</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p234mcpsimp"><a name="p234mcpsimp"></a><a name="p234mcpsimp"></a>是否支持静态metadata 类型1</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

HDR相关能力在SS528V100、SS625V100、SS524V100、SS522V101上不支持。

【相关数据类型及接口】

-   [ot\_hdmi\_hdr\_capability](#ZH-CN_TOPIC_0000002408094722)
-   [ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)

## ot\_hdmi\_hdr\_capability<a name="ZH-CN_TOPIC_0000002408094722"></a>

【说明】

定义HDR能力结构体。

【定义】

```
typedef struct {
    ot_hdmi_eotf                  eotf;
    ot_hdmi_hdr_metadata_type     metadata;
    td_u8                         max_luma_cv;
    td_u8                         avg_luma_cv;
    td_u8                         min_luma_cv;
} ot_hdmi_hdr_capability;
```

【成员】

<a name="table6547mcpsimp"></a>
<table><thead align="left"><tr id="row6552mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p6554mcpsimp"><a name="p6554mcpsimp"></a><a name="p6554mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p6556mcpsimp"><a name="p6556mcpsimp"></a><a name="p6556mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6558mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p6560mcpsimp"><a name="p6560mcpsimp"></a><a name="p6560mcpsimp"></a>eotf</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p6562mcpsimp"><a name="p6562mcpsimp"></a><a name="p6562mcpsimp"></a>Sink支持的EOTF类型</p>
</td>
</tr>
<tr id="row6563mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p6565mcpsimp"><a name="p6565mcpsimp"></a><a name="p6565mcpsimp"></a>metadata</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p6567mcpsimp"><a name="p6567mcpsimp"></a><a name="p6567mcpsimp"></a>Sink支持的metadata类型</p>
</td>
</tr>
<tr id="row6568mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p6570mcpsimp"><a name="p6570mcpsimp"></a><a name="p6570mcpsimp"></a>max_luma_cv</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p6572mcpsimp"><a name="p6572mcpsimp"></a><a name="p6572mcpsimp"></a>Sink支持的最大亮度值</p>
</td>
</tr>
<tr id="row6573mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p6575mcpsimp"><a name="p6575mcpsimp"></a><a name="p6575mcpsimp"></a>avg_luma_cv</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p6577mcpsimp"><a name="p6577mcpsimp"></a><a name="p6577mcpsimp"></a>Sink支持的帧平均亮度值</p>
</td>
</tr>
<tr id="row6578mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p6580mcpsimp"><a name="p6580mcpsimp"></a><a name="p6580mcpsimp"></a>min_luma_cv</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p6582mcpsimp"><a name="p6582mcpsimp"></a><a name="p6582mcpsimp"></a>Sink支持的最小亮度值</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

HDR相关能力在SS528V100、SS625V100、SS524V100、SS522V101上不支持。

【相关数据类型及接口】

[ss\_mpi\_hdmi\_get\_sink\_capability](#ZH-CN_TOPIC_0000002408254566)

## ot\_hdmi\_hw\_param<a name="ZH-CN_TOPIC_0000002408094706"></a>

【说明】

HDMI指标参数结构体。

【定义】

```
typedef struct {
    td_u32 i_de_main_clk;
    td_u32 i_de_main_data;
    td_u32 i_main_clk;
    td_u32 i_main_data;
    td_u32 ft_cap_clk;
    td_u32 ft_cap_data;
} ot_hdmi_hw_param;
```

【成员】

<a name="table4704mcpsimp"></a>
<table><thead align="left"><tr id="row4709mcpsimp"><th class="cellrowborder" valign="top" width="38%" id="mcps1.1.3.1.1"><p id="p4711mcpsimp"><a name="p4711mcpsimp"></a><a name="p4711mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="62%" id="mcps1.1.3.1.2"><p id="p4713mcpsimp"><a name="p4713mcpsimp"></a><a name="p4713mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4715mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4717mcpsimp"><a name="p4717mcpsimp"></a><a name="p4717mcpsimp"></a>i_de_main_clk</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4719mcpsimp"><a name="p4719mcpsimp"></a><a name="p4719mcpsimp"></a>clk线的辅助区电流。</p>
<p id="p4720mcpsimp"><a name="p4720mcpsimp"></a><a name="p4720mcpsimp"></a>取值范围：[0, 43]</p>
</td>
</tr>
<tr id="row4721mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4723mcpsimp"><a name="p4723mcpsimp"></a><a name="p4723mcpsimp"></a>i_de_main_data</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4725mcpsimp"><a name="p4725mcpsimp"></a><a name="p4725mcpsimp"></a>data线的辅助区电流。</p>
<p id="p4726mcpsimp"><a name="p4726mcpsimp"></a><a name="p4726mcpsimp"></a>取值范围：[0, 43]</p>
</td>
</tr>
<tr id="row4727mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4729mcpsimp"><a name="p4729mcpsimp"></a><a name="p4729mcpsimp"></a>i_main_clk</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4731mcpsimp"><a name="p4731mcpsimp"></a><a name="p4731mcpsimp"></a>clk线的主区电流。</p>
<p id="p4732mcpsimp"><a name="p4732mcpsimp"></a><a name="p4732mcpsimp"></a>取值范围：(0, 43]</p>
</td>
</tr>
<tr id="row4733mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4735mcpsimp"><a name="p4735mcpsimp"></a><a name="p4735mcpsimp"></a>i_main_data</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4737mcpsimp"><a name="p4737mcpsimp"></a><a name="p4737mcpsimp"></a>data线的主区电流。</p>
<p id="p4738mcpsimp"><a name="p4738mcpsimp"></a><a name="p4738mcpsimp"></a>取值范围：(0, 43]</p>
</td>
</tr>
<tr id="row4739mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4741mcpsimp"><a name="p4741mcpsimp"></a><a name="p4741mcpsimp"></a>ft_cap_clk</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4743mcpsimp"><a name="p4743mcpsimp"></a><a name="p4743mcpsimp"></a>clk线的上升沿。</p>
<p id="p4744mcpsimp"><a name="p4744mcpsimp"></a><a name="p4744mcpsimp"></a>取值范围：[0, 4]，其中"0"代表关闭上升沿调整，"1", "2", "3", "4"分别代表上升沿抬升距离交叉点的位置为：18ps, 35ps, 45ps, 55ps</p>
</td>
</tr>
<tr id="row4745mcpsimp"><td class="cellrowborder" valign="top" width="38%" headers="mcps1.1.3.1.1 "><p id="p4747mcpsimp"><a name="p4747mcpsimp"></a><a name="p4747mcpsimp"></a>ft_cap_data</p>
</td>
<td class="cellrowborder" valign="top" width="62%" headers="mcps1.1.3.1.2 "><p id="p4749mcpsimp"><a name="p4749mcpsimp"></a><a name="p4749mcpsimp"></a>data线的上升沿。</p>
<p id="p4750mcpsimp"><a name="p4750mcpsimp"></a><a name="p4750mcpsimp"></a>取值范围：[0, 4]，其中"0"代表关闭上升沿调整，"1", "2", "3", "4"分别代表上升沿抬升距离交叉点的位置为：18ps, 35ps, 45ps, 55ps</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   HDMI指标参数中上升沿参数在SS928V100、SS626V100上不建议设置，如果进行设置不会返回失败，但是设置不生效。
-   SS928V100、SS626V100的指标参数取值范围有变化。
    -   辅助区电流  取值范围：\[0, 63\]
    -   主区电流  取值范围：\(0, 63\]

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_hw\_spec](#ZH-CN_TOPIC_0000002408254618)
-   [ot\_hdmi\_hw\_spec](#ZH-CN_TOPIC_0000002441693849)

## ot\_hdmi\_hw\_spec<a name="ZH-CN_TOPIC_0000002441693849"></a>

【说明】

HDMI指标参数。

【定义】

```
typedef struct {
   ot_hdmi_hw_param hw_param[OT_HDMI_HW_PARAM_NUM];
} ot_hdmi_hw_spec;
```

【成员】

<a name="table3758mcpsimp"></a>
<table><thead align="left"><tr id="row3763mcpsimp"><th class="cellrowborder" valign="top" width="54%" id="mcps1.1.3.1.1"><p id="p3765mcpsimp"><a name="p3765mcpsimp"></a><a name="p3765mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="46%" id="mcps1.1.3.1.2"><p id="p3767mcpsimp"><a name="p3767mcpsimp"></a><a name="p3767mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3769mcpsimp"><td class="cellrowborder" valign="top" width="54%" headers="mcps1.1.3.1.1 "><p id="p3771mcpsimp"><a name="p3771mcpsimp"></a><a name="p3771mcpsimp"></a>hw_param[<a href="#ZH-CN_TOPIC_0000002441654009">OT_HDMI_HW_PARAM_NUM</a>]</p>
</td>
<td class="cellrowborder" valign="top" width="46%" headers="mcps1.1.3.1.2 "><p id="p3774mcpsimp"><a name="p3774mcpsimp"></a><a name="p3774mcpsimp"></a>指标参数。</p>
</td>
</tr>
</tbody>
</table>

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_hw\_spec](#ZH-CN_TOPIC_0000002408254618)
-   [ot\_hdmi\_hw\_param](#ZH-CN_TOPIC_0000002408094706)

## ot\_hdmi\_mod\_param<a name="ZH-CN_TOPIC_0000002408094674"></a>

【说明】

HDMI模块参数设置。

【定义】

```
typedef struct {
    ot_hdmi_trace_len  trace_len; /* Phy parameters selection. */
    td_bool emi_en; /* emi enable/disable */
} ot_hdmi_mod_param;
```

【成员】

<a name="table3176mcpsimp"></a>
<table><thead align="left"><tr id="row3181mcpsimp"><th class="cellrowborder" valign="top" width="43%" id="mcps1.1.3.1.1"><p id="p3183mcpsimp"><a name="p3183mcpsimp"></a><a name="p3183mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="56.99999999999999%" id="mcps1.1.3.1.2"><p id="p3185mcpsimp"><a name="p3185mcpsimp"></a><a name="p3185mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row3187mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p3189mcpsimp"><a name="p3189mcpsimp"></a><a name="p3189mcpsimp"></a>trace_len</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p3191mcpsimp"><a name="p3191mcpsimp"></a><a name="p3191mcpsimp"></a>HDMI信号线在PCB板上的走线长度。</p>
</td>
</tr>
<tr id="row3192mcpsimp"><td class="cellrowborder" valign="top" width="43%" headers="mcps1.1.3.1.1 "><p id="p3194mcpsimp"><a name="p3194mcpsimp"></a><a name="p3194mcpsimp"></a>emi_en</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.3.1.2 "><p id="p3196mcpsimp"><a name="p3196mcpsimp"></a><a name="p3196mcpsimp"></a>展频使能开关</p>
</td>
</tr>
</tbody>
</table>

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_mod\_param](#ZH-CN_TOPIC_0000002408094658)
-   [ss\_mpi\_hdmi\_get\_mod\_param](#ZH-CN_TOPIC_0000002441653985)

## ot\_hdmi\_trace\_len<a name="ZH-CN_TOPIC_0000002441654005"></a>

【说明】

HDMI信号线在PCB板上的走线长度枚举。

-   OT\_HDMI\_TRACE\_LEN\_0表示1英寸，后续每个档位按照0.5英寸递增，即OT\_HDMI\_TRACE\_LEN\_1表示1.5英寸。
-   OT\_HDMI\_TRACE\_DEFAULT表示默认配置。
-   当前仅支持1-5英寸走线长度。

【定义】

```
typedef enum {
    OT_HDMI_TRACE_LEN_0,
    OT_HDMI_TRACE_LEN_1,
    OT_HDMI_TRACE_LEN_2,
    OT_HDMI_TRACE_LEN_3,
    OT_HDMI_TRACE_LEN_4,
    OT_HDMI_TRACE_LEN_5,
    OT_HDMI_TRACE_LEN_6,
    OT_HDMI_TRACE_LEN_7,
    OT_HDMI_TRACE_LEN_8,
    OT_HDMI_TRACE_DEFAULT,
    OT_HDMI_TRACE_BUTT
} ot_hdmi_trace_len;
```

【成员】

无

【相关数据类型及接口】

-   [ss\_mpi\_hdmi\_set\_mod\_param](#ZH-CN_TOPIC_0000002408094658)
-   [ss\_mpi\_hdmi\_get\_mod\_param](#ZH-CN_TOPIC_0000002441653985)
-   [ot\_hdmi\_mod\_param](#ZH-CN_TOPIC_0000002408094674)

## OT\_HDMI\_VENDOR\_USER\_DATA\_MAX\_LEN<a name="ZH-CN_TOPIC_0000002441693845"></a>

【说明】

最大自定义vendor数据的长度。

【定义】

```
#define OT_HDMI_VENDOR_USER_DATA_MAX_LEN 22
```

【成员】

无

【相关数据类型及接口】

无

## OT\_HDMI\_MAX\_AUDIO\_CAPBILITY\_CNT<a name="ZH-CN_TOPIC_0000002408094678"></a>

【说明】

能获取的音频能力个数。

【定义】

```
#define OT_HDMI_MAX_AUDIO_CAPBILITY_CNT     16
```

【成员】

无

【相关数据类型及接口】

无

## OT\_HDMI\_MAX\_SAMPLE\_RATE\_NUM<a name="ZH-CN_TOPIC_0000002441654001"></a>

【说明】

音频采样率个数。

【定义】

```
#define OT_HDMI_MAX_SAMPLE_RATE_NUM     8
```

【成员】

无

【相关数据类型及接口】

无

## OT\_HDMI\_MAX\_BIT\_DEPTH\_NUM<a name="ZH-CN_TOPIC_0000002408254574"></a>

【说明】

音频采样深度个数。

【定义】

```
#define OT_HDMI_MAX_BIT_DEPTH_NUM     6
```

【成员】

无

【相关数据类型及接口】

无

## OT\_HDMI\_DETAIL\_TIMING\_MAX<a name="ZH-CN_TOPIC_0000002408094718"></a>

【说明】

详细的视频制式信息个数。

【定义】

```
#define OT_HDMI_DETAIL_TIMING_MAX     10
```

【成员】

无

【相关数据类型及接口】

无

## OT\_HDMI\_EDID\_RAW\_DATA\_LEN<a name="ZH-CN_TOPIC_0000002441693865"></a>

【说明】

EDID原始数据的长度。

【定义】

```
#define OT_HDMI_EDID_RAW_DATA_LEN     512
```

【成员】

无

【相关数据类型及接口】

无

## OT\_HDMI\_HW\_PARAM\_NUM<a name="ZH-CN_TOPIC_0000002441654009"></a>

【说明】

硬件参数的个数。

【定义】

```
#define OT_HDMI_HW_PARAM_NUM     4
```

【成员】

无

【相关数据类型及接口】

无

## OT\_HDMI\_MANUFACTURE\_NAME\_LEN<a name="ZH-CN_TOPIC_0000002441654021"></a>

【说明】

制造商名称的长度。

【定义】

```
#define OT_HDMI_MANUFACTURE_NAME_LEN     4
```

【成员】

无

【相关数据类型及接口】

无

# 错误码<a name="ZH-CN_TOPIC_0000002441693861"></a>

HDMI API错误码如下所示。

**表 1**  HDMI API错误码

<a name="_Ref248309361"></a>
<table><thead align="left"><tr id="row6029mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.4.1.1"><p id="p6031mcpsimp"><a name="p6031mcpsimp"></a><a name="p6031mcpsimp"></a>错误代码</p>
</th>
<th class="cellrowborder" valign="top" width="54%" id="mcps1.2.4.1.2"><p id="p6033mcpsimp"><a name="p6033mcpsimp"></a><a name="p6033mcpsimp"></a>宏定义</p>
</th>
<th class="cellrowborder" valign="top" width="26%" id="mcps1.2.4.1.3"><p id="p6035mcpsimp"><a name="p6035mcpsimp"></a><a name="p6035mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row6037mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6039mcpsimp"><a name="p6039mcpsimp"></a><a name="p6039mcpsimp"></a>0xa0288001</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p xml:lang="pt-BR" id="OT_ERR_HDMI_NOT_INIT"><a name="OT_ERR_HDMI_NOT_INIT"></a><a name="OT_ERR_HDMI_NOT_INIT"></a>OT_ERR_HDMI_NOT_INIT</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6042mcpsimp"><a name="p6042mcpsimp"></a><a name="p6042mcpsimp"></a>HDMI未初始化</p>
</td>
</tr>
<tr id="row6043mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6045mcpsimp"><a name="p6045mcpsimp"></a><a name="p6045mcpsimp"></a>0xa0288002</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p xml:lang="pt-BR" id="OT_ERR_HDMI_INVALID_PARA"><a name="OT_ERR_HDMI_INVALID_PARA"></a><a name="OT_ERR_HDMI_INVALID_PARA"></a>OT_ERR_HDMI_INVALID_PARA</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6048mcpsimp"><a name="p6048mcpsimp"></a><a name="p6048mcpsimp"></a>参数非法</p>
</td>
</tr>
<tr id="row6049mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6051mcpsimp"><a name="p6051mcpsimp"></a><a name="p6051mcpsimp"></a>0xa0288003</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p xml:lang="it-IT" id="p6053mcpsimp"><a name="p6053mcpsimp"></a><a name="p6053mcpsimp"></a>OT_ERR_HDMI_NULL_PTR</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6055mcpsimp"><a name="p6055mcpsimp"></a><a name="p6055mcpsimp"></a>空指针</p>
</td>
</tr>
<tr id="row6056mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6058mcpsimp"><a name="p6058mcpsimp"></a><a name="p6058mcpsimp"></a>0xa0288004</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p xml:lang="it-IT" id="OT_ERR_HDMI_DEV_NOT_OPEN"><a name="OT_ERR_HDMI_DEV_NOT_OPEN"></a><a name="OT_ERR_HDMI_DEV_NOT_OPEN"></a>OT_ERR_HDMI_DEV_NOT_OPEN</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6061mcpsimp"><a name="p6061mcpsimp"></a><a name="p6061mcpsimp"></a>HDMI未打开</p>
</td>
</tr>
<tr id="row6062mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6064mcpsimp"><a name="p6064mcpsimp"></a><a name="p6064mcpsimp"></a>0xa0288005</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6066mcpsimp"><a name="p6066mcpsimp"></a><a name="p6066mcpsimp"></a>OT_ERR_HDMI_DEV_NOT_CONNECT</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6068mcpsimp"><a name="p6068mcpsimp"></a><a name="p6068mcpsimp"></a>HDMI设备未连接</p>
</td>
</tr>
<tr id="row6069mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6071mcpsimp"><a name="p6071mcpsimp"></a><a name="p6071mcpsimp"></a>0xa0288006</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6073mcpsimp"><a name="p6073mcpsimp"></a><a name="p6073mcpsimp"></a>OT_ERR_HDMI_READ_SINK_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6075mcpsimp"><a name="p6075mcpsimp"></a><a name="p6075mcpsimp"></a>HDMI读取Sink端失败</p>
</td>
</tr>
<tr id="row6076mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6078mcpsimp"><a name="p6078mcpsimp"></a><a name="p6078mcpsimp"></a>0xa0288007</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p xml:lang="it-IT" id="p6080mcpsimp"><a name="p6080mcpsimp"></a><a name="p6080mcpsimp"></a>OT_ERR_HDMI_INIT_ALREADY</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6082mcpsimp"><a name="p6082mcpsimp"></a><a name="p6082mcpsimp"></a>HDMI已经初始化</p>
</td>
</tr>
<tr id="row6083mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6085mcpsimp"><a name="p6085mcpsimp"></a><a name="p6085mcpsimp"></a>0xa0288008</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="OT_HDMI_CALLBACK_ALREADY"><a name="OT_HDMI_CALLBACK_ALREADY"></a><a name="OT_HDMI_CALLBACK_ALREADY"></a>OT_ERR_HDMI_CALLBACK_ALREADY</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6088mcpsimp"><a name="p6088mcpsimp"></a><a name="p6088mcpsimp"></a>HDMI回调已注册</p>
</td>
</tr>
<tr id="row6089mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6091mcpsimp"><a name="p6091mcpsimp"></a><a name="p6091mcpsimp"></a>0xa0288009</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p xml:lang="it-IT" id="p6093mcpsimp"><a name="p6093mcpsimp"></a><a name="p6093mcpsimp"></a>OT_ERR_HDMI_INVALID_CALLBACK</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6095mcpsimp"><a name="p6095mcpsimp"></a><a name="p6095mcpsimp"></a>回调函数无效</p>
</td>
</tr>
<tr id="row6096mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6098mcpsimp"><a name="p6098mcpsimp"></a><a name="p6098mcpsimp"></a>0xa028800a</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="OT_ERR_HDMI_FEATURE_NO_SUPPORT"><a name="OT_ERR_HDMI_FEATURE_NO_SUPPORT"></a><a name="OT_ERR_HDMI_FEATURE_NO_SUPPORT"></a>OT_ERR_HDMI_FEATURE_NO_SUPPORT</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6101mcpsimp"><a name="p6101mcpsimp"></a><a name="p6101mcpsimp"></a>功能不支持</p>
</td>
</tr>
<tr id="row6102mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6104mcpsimp"><a name="p6104mcpsimp"></a><a name="p6104mcpsimp"></a>0xa028800b</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6106mcpsimp"><a name="p6106mcpsimp"></a><a name="p6106mcpsimp"></a>OT_ERR_HDMI_BUS_BUSY</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6108mcpsimp"><a name="p6108mcpsimp"></a><a name="p6108mcpsimp"></a>总线忙</p>
</td>
</tr>
<tr id="row6109mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6111mcpsimp"><a name="p6111mcpsimp"></a><a name="p6111mcpsimp"></a>0xa028800c</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6113mcpsimp"><a name="p6113mcpsimp"></a><a name="p6113mcpsimp"></a>OT_ERR_HDMI_READ_EVENT_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6115mcpsimp"><a name="p6115mcpsimp"></a><a name="p6115mcpsimp"></a>事件读失败</p>
</td>
</tr>
<tr id="row6116mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6118mcpsimp"><a name="p6118mcpsimp"></a><a name="p6118mcpsimp"></a>0xa028800d</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6120mcpsimp"><a name="p6120mcpsimp"></a><a name="p6120mcpsimp"></a>OT_ERR_HDMI_NOT_START</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6122mcpsimp"><a name="p6122mcpsimp"></a><a name="p6122mcpsimp"></a>HDMI没有启动</p>
</td>
</tr>
<tr id="row6123mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6125mcpsimp"><a name="p6125mcpsimp"></a><a name="p6125mcpsimp"></a>0xa028800e</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6127mcpsimp"><a name="p6127mcpsimp"></a><a name="p6127mcpsimp"></a>OT_ERR_HDMI_READ_EDID_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6129mcpsimp"><a name="p6129mcpsimp"></a><a name="p6129mcpsimp"></a>HDMI读取EDID失败</p>
</td>
</tr>
<tr id="row6130mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6132mcpsimp"><a name="p6132mcpsimp"></a><a name="p6132mcpsimp"></a>0xa028800f</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6134mcpsimp"><a name="p6134mcpsimp"></a><a name="p6134mcpsimp"></a>OT_ERR_HDMI_INIT_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6136mcpsimp"><a name="p6136mcpsimp"></a><a name="p6136mcpsimp"></a>HDMI初始化失败</p>
</td>
</tr>
<tr id="row6137mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6139mcpsimp"><a name="p6139mcpsimp"></a><a name="p6139mcpsimp"></a>0xa0288010</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6141mcpsimp"><a name="p6141mcpsimp"></a><a name="p6141mcpsimp"></a>OT_ERR_HDMI_CREATE_TESK_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6143mcpsimp"><a name="p6143mcpsimp"></a><a name="p6143mcpsimp"></a>HDMI内核创建任务失败</p>
</td>
</tr>
<tr id="row6144mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6146mcpsimp"><a name="p6146mcpsimp"></a><a name="p6146mcpsimp"></a>0xa0288011</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6148mcpsimp"><a name="p6148mcpsimp"></a><a name="p6148mcpsimp"></a>OT_ERR_HDMI_MALLOC_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6150mcpsimp"><a name="p6150mcpsimp"></a><a name="p6150mcpsimp"></a>HDMI分配内存失败</p>
</td>
</tr>
<tr id="row6151mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6153mcpsimp"><a name="p6153mcpsimp"></a><a name="p6153mcpsimp"></a>0xa0288012</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6155mcpsimp"><a name="p6155mcpsimp"></a><a name="p6155mcpsimp"></a>OT_ERR_HDMI_FREE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6157mcpsimp"><a name="p6157mcpsimp"></a><a name="p6157mcpsimp"></a>HDMI释放内存失败</p>
</td>
</tr>
<tr id="row6158mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6160mcpsimp"><a name="p6160mcpsimp"></a><a name="p6160mcpsimp"></a>0xa0288013</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6162mcpsimp"><a name="p6162mcpsimp"></a><a name="p6162mcpsimp"></a>OT_ERR_HDMI_PTHREAD_CREATE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6164mcpsimp"><a name="p6164mcpsimp"></a><a name="p6164mcpsimp"></a>HDMI创建线程失败</p>
</td>
</tr>
<tr id="row6165mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6167mcpsimp"><a name="p6167mcpsimp"></a><a name="p6167mcpsimp"></a>0xa0288014</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6169mcpsimp"><a name="p6169mcpsimp"></a><a name="p6169mcpsimp"></a>OT_ERR_HDMI_PTHREAD_JOIN_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6171mcpsimp"><a name="p6171mcpsimp"></a><a name="p6171mcpsimp"></a>HDMI等待线程结束失败</p>
</td>
</tr>
<tr id="row6172mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6174mcpsimp"><a name="p6174mcpsimp"></a><a name="p6174mcpsimp"></a>0xa0288015</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6176mcpsimp"><a name="p6176mcpsimp"></a><a name="p6176mcpsimp"></a>OT_ERR_HDMI_STRATEGY_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6178mcpsimp"><a name="p6178mcpsimp"></a><a name="p6178mcpsimp"></a>HDMI内核自适应策略失败</p>
</td>
</tr>
<tr id="row6179mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6181mcpsimp"><a name="p6181mcpsimp"></a><a name="p6181mcpsimp"></a>0xa0288016</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6183mcpsimp"><a name="p6183mcpsimp"></a><a name="p6183mcpsimp"></a>OT_ERR_HDMI_SET_ATTR_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6185mcpsimp"><a name="p6185mcpsimp"></a><a name="p6185mcpsimp"></a>HDMI设置属性失败</p>
</td>
</tr>
<tr id="row6186mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6188mcpsimp"><a name="p6188mcpsimp"></a><a name="p6188mcpsimp"></a>0xa0288017</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="OT_HDMI_CALLBACK_NOT_REGISTER"><a name="OT_HDMI_CALLBACK_NOT_REGISTER"></a><a name="OT_HDMI_CALLBACK_NOT_REGISTER"></a>OT_ERR_HDMI_CALLBACK_NOT_REGISTER</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6191mcpsimp"><a name="p6191mcpsimp"></a><a name="p6191mcpsimp"></a>HDMI回调没有注册</p>
</td>
</tr>
<tr id="row6192mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6194mcpsimp"><a name="p6194mcpsimp"></a><a name="p6194mcpsimp"></a>0xa0288018</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6196mcpsimp"><a name="p6196mcpsimp"></a><a name="p6196mcpsimp"></a>OT_ERR_HDMI_UNKNOWN_COMMAND</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6198mcpsimp"><a name="p6198mcpsimp"></a><a name="p6198mcpsimp"></a>HDMI未知命令</p>
</td>
</tr>
<tr id="row6199mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p6201mcpsimp"><a name="p6201mcpsimp"></a><a name="p6201mcpsimp"></a>0xa0288019</p>
</td>
<td class="cellrowborder" valign="top" width="54%" headers="mcps1.2.4.1.2 "><p id="p6203mcpsimp"><a name="p6203mcpsimp"></a><a name="p6203mcpsimp"></a>OT_ERR_HDMI_MUTEX_LOCK_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="26%" headers="mcps1.2.4.1.3 "><p id="p6205mcpsimp"><a name="p6205mcpsimp"></a><a name="p6205mcpsimp"></a>HDMI加锁失败</p>
</td>
</tr>
</tbody>
</table>

# Proc调试信息<a name="ZH-CN_TOPIC_0000002441654013"></a>

【HDMI软硬件总体状态调试信息】

>![](public_sys-resources/icon-notice.gif) **须知：** 
>解决方案不支持的规格，该部分proc信息无效。

```
# cat /proc/umap/hdmi0

[HDMI] Version: [V1.0.0.0 B010 Release], Build Time[Sep  6 2019, 10:49:13]

HDMI version:   2.0.0.20190620.0

----------------------------- app attr ----------------------------------
hdmie enable        : YES                 audio enable        : YES                 
auth mode enable    : NO                  deep color mode     : 24                 
deep color adapt    : NO                  
----------------------------- sw status ---------------------------------
thread run          : YES                 run status          : OPEN START
tmds mode           : HDMI1.4             kernel cnt          : 0                   
user cnt            : 1                   kernel callback     : NO                 
user callback cnt   : 1                   transit state       : BOOT->APP
emi enable          : NO                  pcb len             : 0                   
----------------------------- hw status ---------------------------------
hot plug            : YES                 rsen                : NO                 
phy output enable   : NO                  phy power enable    : NO                  
tmds mode           : HDMI1.4             avmute              : NO
emi enable          : NO                  src_scramble        : YES                 
sink_scramble       : YES                 tmds_bit_clk_ratio  : 40
---------------------------- detect timming -----------------------------
sync sw enable      : NO                  hsync polarity      : P                   
progressive         : YES                 vsync polarity      : P                  
hsync total         : 1650                hactive cnt         : 1280
vsync total         : 750                 vactive cnt         : 720                 
---------------------- task id=1112 event pool[0] status --------------------------
cnt|err total|hpd|unhpd|edid fail |rsen con|rsen dis
wr:|0      |0  |0    |0       |0      |0       
rd:|0      |0  |0    |0       |0      |0       
memory[wkflg=0 |rdable= 0| rdptr=0 | wrptr=0 ]: 
 
---------------------------- spec info ----------------------------------
                user_seting                    |default_seting   |cur_seting   
stage         |first |second|third |fourth |first |second|third |fourth|
i_de_main_clk |0     |0     |0     |0      |0     |0     |0     |0     |0            
i_de_main_data|1     |1     |1     |1      |1     |1     |1     |1     |1            
i_main_clk    |20    |20    |20    |21     |20    |20    |20    |21    |20           
i_main_data   |27    |27    |28    |33     |27    |27    |28    |33    |28           
ft_cap_clk    |0     |0     |0     |0      |0     |0     |0     |0     |0            
ft_cap_data   |0     |0     |3     |0      |0     |0     |3     |0     |3  
```

【调试信息分析】

记录HDMI输出管理模块信息。

【参数说明】

<a name="table1191mcpsimp"></a>
<table><thead align="left"><tr id="row1197mcpsimp"><th class="cellrowborder" colspan="2" valign="top" id="mcps1.1.4.1.1"><p id="p1199mcpsimp"><a name="p1199mcpsimp"></a><a name="p1199mcpsimp"></a>Parameter</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.1.4.1.2"><p id="p1201mcpsimp"><a name="p1201mcpsimp"></a><a name="p1201mcpsimp"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="row1203mcpsimp"><td class="cellrowborder" rowspan="5" valign="top" width="14.14%" headers="mcps1.1.4.1.1 "><p id="p1205mcpsimp"><a name="p1205mcpsimp"></a><a name="p1205mcpsimp"></a>app attr</p>
</td>
<td class="cellrowborder" valign="top" width="29.26%" headers="mcps1.1.4.1.1 "><p id="p1207mcpsimp"><a name="p1207mcpsimp"></a><a name="p1207mcpsimp"></a>hdmie enable</p>
</td>
<td class="cellrowborder" valign="top" width="56.599999999999994%" headers="mcps1.1.4.1.2 "><p id="p1209mcpsimp"><a name="p1209mcpsimp"></a><a name="p1209mcpsimp"></a>是否使能HDMI模式。</p>
<p id="p1210mcpsimp"><a name="p1210mcpsimp"></a><a name="p1210mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1211mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1213mcpsimp"><a name="p1213mcpsimp"></a><a name="p1213mcpsimp"></a>audio enable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1215mcpsimp"><a name="p1215mcpsimp"></a><a name="p1215mcpsimp"></a>当前用户是否使能音频。</p>
<p id="p1216mcpsimp"><a name="p1216mcpsimp"></a><a name="p1216mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1217mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1219mcpsimp"><a name="p1219mcpsimp"></a><a name="p1219mcpsimp"></a>deep color mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1221mcpsimp"><a name="p1221mcpsimp"></a><a name="p1221mcpsimp"></a>当前用户设置的图像色彩深度。</p>
<p id="p1222mcpsimp"><a name="p1222mcpsimp"></a><a name="p1222mcpsimp"></a>取值：{24, 30, 36, 48, OFF, UNKNOWN}</p>
</td>
</tr>
<tr id="row1223mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1225mcpsimp"><a name="p1225mcpsimp"></a><a name="p1225mcpsimp"></a>deep color adapt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1227mcpsimp"><a name="p1227mcpsimp"></a><a name="p1227mcpsimp"></a>当前用户是否使能DeepColor自适应策略。</p>
<p id="p1228mcpsimp"><a name="p1228mcpsimp"></a><a name="p1228mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1229mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1231mcpsimp"><a name="p1231mcpsimp"></a><a name="p1231mcpsimp"></a>auth mode enable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1233mcpsimp"><a name="p1233mcpsimp"></a><a name="p1233mcpsimp"></a>是否使能认证模式。</p>
<p id="p1234mcpsimp"><a name="p1234mcpsimp"></a><a name="p1234mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1235mcpsimp"><td class="cellrowborder" rowspan="10" valign="top" width="14.14%" headers="mcps1.1.4.1.1 "><p id="p1237mcpsimp"><a name="p1237mcpsimp"></a><a name="p1237mcpsimp"></a>sw status</p>
</td>
<td class="cellrowborder" valign="top" width="29.26%" headers="mcps1.1.4.1.1 "><p id="p1239mcpsimp"><a name="p1239mcpsimp"></a><a name="p1239mcpsimp"></a>thread run</p>
</td>
<td class="cellrowborder" valign="top" width="56.599999999999994%" headers="mcps1.1.4.1.2 "><p id="p1241mcpsimp"><a name="p1241mcpsimp"></a><a name="p1241mcpsimp"></a>当前HDMI驱动线程是否运行。</p>
<p id="p1242mcpsimp"><a name="p1242mcpsimp"></a><a name="p1242mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1243mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1245mcpsimp"><a name="p1245mcpsimp"></a><a name="p1245mcpsimp"></a>run status</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1247mcpsimp"><a name="p1247mcpsimp"></a><a name="p1247mcpsimp"></a>HDMI运行状态。</p>
<p id="p1248mcpsimp"><a name="p1248mcpsimp"></a><a name="p1248mcpsimp"></a>取值：{NONE, OPEN, START, STOP, CLOSE}</p>
</td>
</tr>
<tr id="row1249mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1251mcpsimp"><a name="p1251mcpsimp"></a><a name="p1251mcpsimp"></a>tmds mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1253mcpsimp"><a name="p1253mcpsimp"></a><a name="p1253mcpsimp"></a>当前TMDS工作模式。</p>
<p id="p1254mcpsimp"><a name="p1254mcpsimp"></a><a name="p1254mcpsimp"></a>取值：{NONE, DVI, HDMI1.4, HDMI2.0, AUTO, HDMI2.1, UNKNOWN}</p>
</td>
</tr>
<tr id="row1255mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1257mcpsimp"><a name="p1257mcpsimp"></a><a name="p1257mcpsimp"></a>kernel cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1259mcpsimp"><a name="p1259mcpsimp"></a><a name="p1259mcpsimp"></a>Kernel下打开HDMI设备的计数。</p>
</td>
</tr>
<tr id="row1260mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1262mcpsimp"><a name="p1262mcpsimp"></a><a name="p1262mcpsimp"></a>user cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1264mcpsimp"><a name="p1264mcpsimp"></a><a name="p1264mcpsimp"></a>用户打开HDMI设备的计数。</p>
</td>
</tr>
<tr id="row1265mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1267mcpsimp"><a name="p1267mcpsimp"></a><a name="p1267mcpsimp"></a>kernel callback</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1269mcpsimp"><a name="p1269mcpsimp"></a><a name="p1269mcpsimp"></a>HDMI回调是否被注册。</p>
<p id="p1270mcpsimp"><a name="p1270mcpsimp"></a><a name="p1270mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1271mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1273mcpsimp"><a name="p1273mcpsimp"></a><a name="p1273mcpsimp"></a>user callback cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1275mcpsimp"><a name="p1275mcpsimp"></a><a name="p1275mcpsimp"></a>用户回调次数。</p>
</td>
</tr>
<tr id="row1276mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1278mcpsimp"><a name="p1278mcpsimp"></a><a name="p1278mcpsimp"></a>transit state</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1280mcpsimp"><a name="p1280mcpsimp"></a><a name="p1280mcpsimp"></a>启动过程状态记录。</p>
<p id="p1281mcpsimp"><a name="p1281mcpsimp"></a><a name="p1281mcpsimp"></a>取值：{NONE, BOOT-&gt;MCE, MCE-&gt;APP, BOOT-&gt;APP}</p>
</td>
</tr>
<tr id="row1282mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1284mcpsimp"><a name="p1284mcpsimp"></a><a name="p1284mcpsimp"></a>emi enable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1286mcpsimp"><a name="p1286mcpsimp"></a><a name="p1286mcpsimp"></a>是否使能展频功能。</p>
<p id="p1287mcpsimp"><a name="p1287mcpsimp"></a><a name="p1287mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1288mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1290mcpsimp"><a name="p1290mcpsimp"></a><a name="p1290mcpsimp"></a>pcb len</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1292mcpsimp"><a name="p1292mcpsimp"></a><a name="p1292mcpsimp"></a>HDMI走线参数信息。</p>
<p id="p1293mcpsimp"><a name="p1293mcpsimp"></a><a name="p1293mcpsimp"></a>取值：[0, 9]</p>
</td>
</tr>
<tr id="row1294mcpsimp"><td class="cellrowborder" rowspan="10" valign="top" width="14.14%" headers="mcps1.1.4.1.1 "><p id="p1296mcpsimp"><a name="p1296mcpsimp"></a><a name="p1296mcpsimp"></a>hw status</p>
</td>
<td class="cellrowborder" valign="top" width="29.26%" headers="mcps1.1.4.1.1 "><p id="p1298mcpsimp"><a name="p1298mcpsimp"></a><a name="p1298mcpsimp"></a>hot plug</p>
</td>
<td class="cellrowborder" valign="top" width="56.599999999999994%" headers="mcps1.1.4.1.2 "><p id="p1300mcpsimp"><a name="p1300mcpsimp"></a><a name="p1300mcpsimp"></a>硬件Hotplug状态。</p>
<p id="p1301mcpsimp"><a name="p1301mcpsimp"></a><a name="p1301mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1302mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1304mcpsimp"><a name="p1304mcpsimp"></a><a name="p1304mcpsimp"></a>rsen</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1306mcpsimp"><a name="p1306mcpsimp"></a><a name="p1306mcpsimp"></a>硬件Rsen状态。</p>
<p id="p1307mcpsimp"><a name="p1307mcpsimp"></a><a name="p1307mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1308mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1310mcpsimp"><a name="p1310mcpsimp"></a><a name="p1310mcpsimp"></a>phy output enable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1312mcpsimp"><a name="p1312mcpsimp"></a><a name="p1312mcpsimp"></a>HDMI Phy是否已打开。</p>
<p id="p1313mcpsimp"><a name="p1313mcpsimp"></a><a name="p1313mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1314mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1316mcpsimp"><a name="p1316mcpsimp"></a><a name="p1316mcpsimp"></a>phy power enable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1318mcpsimp"><a name="p1318mcpsimp"></a><a name="p1318mcpsimp"></a>HDMI Phy是否已上电。</p>
<p id="p1319mcpsimp"><a name="p1319mcpsimp"></a><a name="p1319mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1320mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1322mcpsimp"><a name="p1322mcpsimp"></a><a name="p1322mcpsimp"></a>tmds mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1324mcpsimp"><a name="p1324mcpsimp"></a><a name="p1324mcpsimp"></a>当前硬件TMDS工作模式。</p>
<p id="p1325mcpsimp"><a name="p1325mcpsimp"></a><a name="p1325mcpsimp"></a>取值：{NONE, DVI, HDMI1.4, HDMI2.0, AUTO, UNKNOWN}</p>
</td>
</tr>
<tr id="row1326mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1328mcpsimp"><a name="p1328mcpsimp"></a><a name="p1328mcpsimp"></a>avmute</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1330mcpsimp"><a name="p1330mcpsimp"></a><a name="p1330mcpsimp"></a>当前是否使能音视频Mute（显示端）。</p>
<p id="p1331mcpsimp"><a name="p1331mcpsimp"></a><a name="p1331mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1332mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1334mcpsimp"><a name="p1334mcpsimp"></a><a name="p1334mcpsimp"></a>emi</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1336mcpsimp"><a name="p1336mcpsimp"></a><a name="p1336mcpsimp"></a>当前硬件展频状态。</p>
<p id="p1337mcpsimp"><a name="p1337mcpsimp"></a><a name="p1337mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1338mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1340mcpsimp"><a name="p1340mcpsimp"></a><a name="p1340mcpsimp"></a>src_scramble</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1342mcpsimp"><a name="p1342mcpsimp"></a><a name="p1342mcpsimp"></a>源端加扰状态。</p>
<p id="p1343mcpsimp"><a name="p1343mcpsimp"></a><a name="p1343mcpsimp"></a>注意：目前只有SS528V100有此信息。</p>
</td>
</tr>
<tr id="row1344mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1346mcpsimp"><a name="p1346mcpsimp"></a><a name="p1346mcpsimp"></a>sink_scramble</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1348mcpsimp"><a name="p1348mcpsimp"></a><a name="p1348mcpsimp"></a>sink端加扰状态。</p>
<p id="p1349mcpsimp"><a name="p1349mcpsimp"></a><a name="p1349mcpsimp"></a>注意：目前只有SS528V100有此信息。</p>
</td>
</tr>
<tr id="row1350mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1352mcpsimp"><a name="p1352mcpsimp"></a><a name="p1352mcpsimp"></a>tmds_bit_clk_ratio</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1354mcpsimp"><a name="p1354mcpsimp"></a><a name="p1354mcpsimp"></a>源端tmds时钟速率。</p>
<p id="p1355mcpsimp"><a name="p1355mcpsimp"></a><a name="p1355mcpsimp"></a>注意：目前只有SS528V100有此信息。</p>
</td>
</tr>
<tr id="row1356mcpsimp"><td class="cellrowborder" rowspan="10" valign="top" width="14.14%" headers="mcps1.1.4.1.1 "><p id="p1358mcpsimp"><a name="p1358mcpsimp"></a><a name="p1358mcpsimp"></a>detect timming</p>
</td>
<td class="cellrowborder" valign="top" width="29.26%" headers="mcps1.1.4.1.1 "><p id="p1360mcpsimp"><a name="p1360mcpsimp"></a><a name="p1360mcpsimp"></a>sync sw enable</p>
</td>
<td class="cellrowborder" valign="top" width="56.599999999999994%" headers="mcps1.1.4.1.2 "><p id="p1362mcpsimp"><a name="p1362mcpsimp"></a><a name="p1362mcpsimp"></a>是否使用软件极性配置。</p>
<p id="p1363mcpsimp"><a name="p1363mcpsimp"></a><a name="p1363mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1364mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1366mcpsimp"><a name="p1366mcpsimp"></a><a name="p1366mcpsimp"></a>hsync polarity</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1368mcpsimp"><a name="p1368mcpsimp"></a><a name="p1368mcpsimp"></a>Hsync极性逻辑检测值。</p>
<p id="p1369mcpsimp"><a name="p1369mcpsimp"></a><a name="p1369mcpsimp"></a>取值：{P, N}</p>
</td>
</tr>
<tr id="row1370mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1372mcpsimp"><a name="p1372mcpsimp"></a><a name="p1372mcpsimp"></a>progressive</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1374mcpsimp"><a name="p1374mcpsimp"></a><a name="p1374mcpsimp"></a>当前是否逐行输出。</p>
<p id="p1375mcpsimp"><a name="p1375mcpsimp"></a><a name="p1375mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1376mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1378mcpsimp"><a name="p1378mcpsimp"></a><a name="p1378mcpsimp"></a>vsync polarity</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1380mcpsimp"><a name="p1380mcpsimp"></a><a name="p1380mcpsimp"></a>Vsync极性逻辑检测值。</p>
<p id="p1381mcpsimp"><a name="p1381mcpsimp"></a><a name="p1381mcpsimp"></a>取值：{P, N}</p>
</td>
</tr>
<tr id="row1382mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1384mcpsimp"><a name="p1384mcpsimp"></a><a name="p1384mcpsimp"></a>hsync total</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1386mcpsimp"><a name="p1386mcpsimp"></a><a name="p1386mcpsimp"></a>行总像素点数逻辑检测值。</p>
</td>
</tr>
<tr id="row1387mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1389mcpsimp"><a name="p1389mcpsimp"></a><a name="p1389mcpsimp"></a>hactive cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1391mcpsimp"><a name="p1391mcpsimp"></a><a name="p1391mcpsimp"></a>行有效像素点逻辑检测值。</p>
</td>
</tr>
<tr id="row1392mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1394mcpsimp"><a name="p1394mcpsimp"></a><a name="p1394mcpsimp"></a>vsync total</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1396mcpsimp"><a name="p1396mcpsimp"></a><a name="p1396mcpsimp"></a>场总行数逻辑检测值。</p>
</td>
</tr>
<tr id="row1397mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1399mcpsimp"><a name="p1399mcpsimp"></a><a name="p1399mcpsimp"></a>vactive cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1401mcpsimp"><a name="p1401mcpsimp"></a><a name="p1401mcpsimp"></a>场有效行数逻辑检测值。</p>
</td>
</tr>
<tr id="row1402mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1404mcpsimp"><a name="p1404mcpsimp"></a><a name="p1404mcpsimp"></a>emi enable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1406mcpsimp"><a name="p1406mcpsimp"></a><a name="p1406mcpsimp"></a>展频使能标志。</p>
<p id="p1407mcpsimp"><a name="p1407mcpsimp"></a><a name="p1407mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1408mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1410mcpsimp"><a name="p1410mcpsimp"></a><a name="p1410mcpsimp"></a>emi debug enable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1412mcpsimp"><a name="p1412mcpsimp"></a><a name="p1412mcpsimp"></a>展频调试模式使能标志。</p>
<p id="p1413mcpsimp"><a name="p1413mcpsimp"></a><a name="p1413mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1414mcpsimp"><td class="cellrowborder" rowspan="10" valign="top" width="14.14%" headers="mcps1.1.4.1.1 "><p id="p1416mcpsimp"><a name="p1416mcpsimp"></a><a name="p1416mcpsimp"></a>task id event pool status</p>
</td>
<td class="cellrowborder" valign="top" width="29.26%" headers="mcps1.1.4.1.1 "><p id="p1418mcpsimp"><a name="p1418mcpsimp"></a><a name="p1418mcpsimp"></a>err total</p>
</td>
<td class="cellrowborder" valign="top" width="56.599999999999994%" headers="mcps1.1.4.1.2 "><p id="p1420mcpsimp"><a name="p1420mcpsimp"></a><a name="p1420mcpsimp"></a>失败事件总次数。</p>
</td>
</tr>
<tr id="row1421mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1423mcpsimp"><a name="p1423mcpsimp"></a><a name="p1423mcpsimp"></a>hpd</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1425mcpsimp"><a name="p1425mcpsimp"></a><a name="p1425mcpsimp"></a>热插拔事件-插入次数。</p>
</td>
</tr>
<tr id="row1426mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1428mcpsimp"><a name="p1428mcpsimp"></a><a name="p1428mcpsimp"></a>unhpd</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1430mcpsimp"><a name="p1430mcpsimp"></a><a name="p1430mcpsimp"></a>热插拔事件-拔出次数。</p>
</td>
</tr>
<tr id="row1431mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1433mcpsimp"><a name="p1433mcpsimp"></a><a name="p1433mcpsimp"></a>edid fail</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1435mcpsimp"><a name="p1435mcpsimp"></a><a name="p1435mcpsimp"></a>EDID读取失败次数。</p>
</td>
</tr>
<tr id="row1446mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1448mcpsimp"><a name="p1448mcpsimp"></a><a name="p1448mcpsimp"></a>rsen con</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1450mcpsimp"><a name="p1450mcpsimp"></a><a name="p1450mcpsimp"></a>Rsen连接次数。</p>
</td>
</tr>
<tr id="row1451mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1453mcpsimp"><a name="p1453mcpsimp"></a><a name="p1453mcpsimp"></a>rsen dis</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1455mcpsimp"><a name="p1455mcpsimp"></a><a name="p1455mcpsimp"></a>Rsen断开次数。</p>
</td>
</tr>
<tr id="row1461mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1463mcpsimp"><a name="p1463mcpsimp"></a><a name="p1463mcpsimp"></a>wkflg</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1465mcpsimp"><a name="p1465mcpsimp"></a><a name="p1465mcpsimp"></a>唤醒标志。</p>
</td>
</tr>
<tr id="row1466mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1468mcpsimp"><a name="p1468mcpsimp"></a><a name="p1468mcpsimp"></a>rdable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1470mcpsimp"><a name="p1470mcpsimp"></a><a name="p1470mcpsimp"></a>事件池中可读事件数。</p>
</td>
</tr>
<tr id="row1471mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1473mcpsimp"><a name="p1473mcpsimp"></a><a name="p1473mcpsimp"></a>rdptr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1475mcpsimp"><a name="p1475mcpsimp"></a><a name="p1475mcpsimp"></a>事件池中事件读指针。</p>
</td>
</tr>
<tr id="row1476mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1478mcpsimp"><a name="p1478mcpsimp"></a><a name="p1478mcpsimp"></a>wrptr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1480mcpsimp"><a name="p1480mcpsimp"></a><a name="p1480mcpsimp"></a>事件池中事件写指针。</p>
</td>
</tr>
<tr id="row1481mcpsimp"><td class="cellrowborder" rowspan="3" valign="top" width="14.14%" headers="mcps1.1.4.1.1 "><p id="p1483mcpsimp"><a name="p1483mcpsimp"></a><a name="p1483mcpsimp"></a>spec info</p>
</td>
<td class="cellrowborder" valign="top" width="29.26%" headers="mcps1.1.4.1.1 "><p id="p1485mcpsimp"><a name="p1485mcpsimp"></a><a name="p1485mcpsimp"></a>user_seting</p>
</td>
<td class="cellrowborder" valign="top" width="56.599999999999994%" headers="mcps1.1.4.1.2 "><p id="p1487mcpsimp"><a name="p1487mcpsimp"></a><a name="p1487mcpsimp"></a>用户设置的指标参数。</p>
</td>
</tr>
<tr id="row1488mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1490mcpsimp"><a name="p1490mcpsimp"></a><a name="p1490mcpsimp"></a>default_seting</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1492mcpsimp"><a name="p1492mcpsimp"></a><a name="p1492mcpsimp"></a>驱动默认的指标参数。</p>
</td>
</tr>
<tr id="row1493mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1495mcpsimp"><a name="p1495mcpsimp"></a><a name="p1495mcpsimp"></a>cur_seting</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1497mcpsimp"><a name="p1497mcpsimp"></a><a name="p1497mcpsimp"></a>驱动实际设置的指标参数。</p>
</td>
</tr>
</tbody>
</table>

注：

-   N/A/NONE/UNKNOWN为未知/无效；reserved为保留/未知；ERROR为错误。
-   spec info的stage栏中first、second、third、fourth分别表示四个频段。

>![](public_sys-resources/icon-notice.gif) **须知：** 
>注意avmute与下面的audio mute、video mute的区别：
>-   avmute是由TX发送给RX的一个标准的协议包，RX在接收到该数据包后会自动mute自身的音视频输出。proc里的avmute状态表示的是TX是否有发送avmute包。
>-   audio mute、video mute是TX实现的一个功能，主要目的是用于调试的场景，其状态反应的是TX音视频数据的输出情况。

【HDMI音频调试信息】

```
# cat /proc/umap/hdmi0_ao 
 
[HDMI] Version: [V1.0.0.0 B010 Release], Build Time[Sep  6 2019, 10:49:13]
 
HDMI version:   2.0.0.20190620.0
------------ audio attr ------------------------- audio info ----------
sound intf          : I2S                  |audio info enable   : YES                  
code type           : STREAM               |code type           : STREAM               
channel cnt         : 2_CH                 |channel cnt         : 2_CH                 
sample freq         : 48000                |sample freq         : STR_HEADER           
sample depth        : 16                   |sample depth        : 16                   
down sample         : NO                   |sample size         : STR_HEADER           
---------------- audio path ---------------|down mix inhibit    : NO                   
audio enable        : YES                  |level shift value   : 0                    
audio mute          : NO                   |lfe playback        : UNKNOWN              
sound intf          : I2S                  |channel/speakeralloc: 0x00 (0)
channel cnt         : 2_CH                 |audio info raw data : 
sample freq         : 48000                | 84 01 0a 70  01 00 00 00 
sample depth        : 16                   | 00 00 00 00  00 00 
down sample         : NO                   |
ref cts             : 74250                |
reg cts             : 152062               |
ref n               : 6144                 |
reg n               : 6144                 | 
```

【调试信息分析】

记录当前HDMI音频工作状态。

【参数说明】

<a name="table1532mcpsimp"></a>
<table><thead align="left"><tr id="row1538mcpsimp"><th class="cellrowborder" colspan="2" valign="top" id="mcps1.1.4.1.1"><p id="p1540mcpsimp"><a name="p1540mcpsimp"></a><a name="p1540mcpsimp"></a>Parameter</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.1.4.1.2"><p id="p1542mcpsimp"><a name="p1542mcpsimp"></a><a name="p1542mcpsimp"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="row1544mcpsimp"><td class="cellrowborder" rowspan="6" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p1546mcpsimp"><a name="p1546mcpsimp"></a><a name="p1546mcpsimp"></a>audio attr</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p1548mcpsimp"><a name="p1548mcpsimp"></a><a name="p1548mcpsimp"></a>sound intf</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.4.1.2 "><p id="p1550mcpsimp"><a name="p1550mcpsimp"></a><a name="p1550mcpsimp"></a>用户配置的HDMI音频接口类型。</p>
<p id="p1551mcpsimp"><a name="p1551mcpsimp"></a><a name="p1551mcpsimp"></a>取值：{I2S, SPDIF, HBRA, UNKNOWN}</p>
</td>
</tr>
<tr id="row1552mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1554mcpsimp"><a name="p1554mcpsimp"></a><a name="p1554mcpsimp"></a>code type</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1556mcpsimp"><a name="p1556mcpsimp"></a><a name="p1556mcpsimp"></a>音频编码类型。</p>
<p id="p1557mcpsimp"><a name="p1557mcpsimp"></a><a name="p1557mcpsimp"></a>取值：{STREAM, L-PCM, AC3, MPEG1, MP3, MPEG2, AAC_LC, DTS, ATRAC, OneBitAudio, EAC3, DTS-HD, MAT, DST, WMA_PRO, Reserved, UNKNOWN}</p>
<p id="p1558mcpsimp"><a name="p1558mcpsimp"></a><a name="p1558mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1559mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1561mcpsimp"><a name="p1561mcpsimp"></a><a name="p1561mcpsimp"></a>channel cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1563mcpsimp"><a name="p1563mcpsimp"></a><a name="p1563mcpsimp"></a>音频通道数。</p>
<p id="p1564mcpsimp"><a name="p1564mcpsimp"></a><a name="p1564mcpsimp"></a>取值：{STR_HEADER, 2_CH, 3_CH, 4_CH, 5_CH, 6_CH, 7_CH, 8_CH}</p>
<p id="p1565mcpsimp"><a name="p1565mcpsimp"></a><a name="p1565mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1566mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1568mcpsimp"><a name="p1568mcpsimp"></a><a name="p1568mcpsimp"></a>sample freq</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1570mcpsimp"><a name="p1570mcpsimp"></a><a name="p1570mcpsimp"></a>音频采样频率。</p>
<p id="p1571mcpsimp"><a name="p1571mcpsimp"></a><a name="p1571mcpsimp"></a>注：SSxx仅支持32000Hz、44100Hz、48000Hz采样率</p>
</td>
</tr>
<tr id="row1572mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1574mcpsimp"><a name="p1574mcpsimp"></a><a name="p1574mcpsimp"></a>sample depth</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1576mcpsimp"><a name="p1576mcpsimp"></a><a name="p1576mcpsimp"></a>采样深度（位宽）。</p>
</td>
</tr>
<tr id="row1577mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1579mcpsimp"><a name="p1579mcpsimp"></a><a name="p1579mcpsimp"></a>down sample</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1581mcpsimp"><a name="p1581mcpsimp"></a><a name="p1581mcpsimp"></a>是否降采样。</p>
<p id="p1582mcpsimp"><a name="p1582mcpsimp"></a><a name="p1582mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1583mcpsimp"><td class="cellrowborder" rowspan="11" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p1585mcpsimp"><a name="p1585mcpsimp"></a><a name="p1585mcpsimp"></a>audio info</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p1587mcpsimp"><a name="p1587mcpsimp"></a><a name="p1587mcpsimp"></a>audio info enable</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.4.1.2 "><p id="p1589mcpsimp"><a name="p1589mcpsimp"></a><a name="p1589mcpsimp"></a>音频信息帧是否使能。</p>
<p id="p1590mcpsimp"><a name="p1590mcpsimp"></a><a name="p1590mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1591mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1593mcpsimp"><a name="p1593mcpsimp"></a><a name="p1593mcpsimp"></a>code type</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1595mcpsimp"><a name="p1595mcpsimp"></a><a name="p1595mcpsimp"></a>音频编码类型。</p>
<p id="p1596mcpsimp"><a name="p1596mcpsimp"></a><a name="p1596mcpsimp"></a>取值：{ STREAM, L-PCM, AC3, MPEG1, MP3, MPEG2, AAC_LC, DTS, ATRAC, OneBitAudio, EAC3, DTS-HD, MAT, DST, WMA_PRO, Reserved, UNKNOWN}</p>
<p id="p1597mcpsimp"><a name="p1597mcpsimp"></a><a name="p1597mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1598mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1600mcpsimp"><a name="p1600mcpsimp"></a><a name="p1600mcpsimp"></a>channel cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1602mcpsimp"><a name="p1602mcpsimp"></a><a name="p1602mcpsimp"></a>音频通道数。</p>
<p id="p1603mcpsimp"><a name="p1603mcpsimp"></a><a name="p1603mcpsimp"></a>取值：{STR_HEADER, 2_CH, 3_CH, 4_CH, 5_CH, 6_CH, 7_CH, 8_CH}</p>
<p id="p1604mcpsimp"><a name="p1604mcpsimp"></a><a name="p1604mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1605mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1607mcpsimp"><a name="p1607mcpsimp"></a><a name="p1607mcpsimp"></a>sample freq</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1609mcpsimp"><a name="p1609mcpsimp"></a><a name="p1609mcpsimp"></a>音频采样频率。</p>
<p id="p1610mcpsimp"><a name="p1610mcpsimp"></a><a name="p1610mcpsimp"></a>取值：{ STR_HEADER, 32KHz, 44.1KHz, 48KHz , 88.2KHz , 96KHz, 176.4KHz, 192KHz }</p>
<p id="p1611mcpsimp"><a name="p1611mcpsimp"></a><a name="p1611mcpsimp"></a>注：SSxx目前仅支持32KHz, 44.1KHz, 48KHz采样率。</p>
</td>
</tr>
<tr id="row1612mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1614mcpsimp"><a name="p1614mcpsimp"></a><a name="p1614mcpsimp"></a>sample depth</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1616mcpsimp"><a name="p1616mcpsimp"></a><a name="p1616mcpsimp"></a>采样深度。</p>
</td>
</tr>
<tr id="row1617mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1619mcpsimp"><a name="p1619mcpsimp"></a><a name="p1619mcpsimp"></a>sample size</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1621mcpsimp"><a name="p1621mcpsimp"></a><a name="p1621mcpsimp"></a>采样大小（位宽）。</p>
<p id="p1622mcpsimp"><a name="p1622mcpsimp"></a><a name="p1622mcpsimp"></a>取值：{STR_HEADER, 16bit, 20bit, 24bit}</p>
</td>
</tr>
<tr id="row1623mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1625mcpsimp"><a name="p1625mcpsimp"></a><a name="p1625mcpsimp"></a>down mix inhibit</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1627mcpsimp"><a name="p1627mcpsimp"></a><a name="p1627mcpsimp"></a>Down-mix Inhibit 标志位。</p>
<p id="p1628mcpsimp"><a name="p1628mcpsimp"></a><a name="p1628mcpsimp"></a>取值：{YES, NO}</p>
<p id="p1629mcpsimp"><a name="p1629mcpsimp"></a><a name="p1629mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1630mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1632mcpsimp"><a name="p1632mcpsimp"></a><a name="p1632mcpsimp"></a>level shift value</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1634mcpsimp"><a name="p1634mcpsimp"></a><a name="p1634mcpsimp"></a>Level Shift Value。</p>
<p id="p1635mcpsimp"><a name="p1635mcpsimp"></a><a name="p1635mcpsimp"></a>取值：{0~15 dB}</p>
<p id="p1636mcpsimp"><a name="p1636mcpsimp"></a><a name="p1636mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1637mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1639mcpsimp"><a name="p1639mcpsimp"></a><a name="p1639mcpsimp"></a>lfe playback</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1641mcpsimp"><a name="p1641mcpsimp"></a><a name="p1641mcpsimp"></a>LFE playback level information，LFE播放等级信息。</p>
<p id="p1642mcpsimp"><a name="p1642mcpsimp"></a><a name="p1642mcpsimp"></a>取值：{UNKNOWN, 0dB, +10dB, Reserved}</p>
<p id="p1643mcpsimp"><a name="p1643mcpsimp"></a><a name="p1643mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1644mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1646mcpsimp"><a name="p1646mcpsimp"></a><a name="p1646mcpsimp"></a>channel/speakeralloc</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1648mcpsimp"><a name="p1648mcpsimp"></a><a name="p1648mcpsimp"></a>Channel/Speaker分配。</p>
<p id="p1649mcpsimp"><a name="p1649mcpsimp"></a><a name="p1649mcpsimp"></a>取值：{0x00~0xff}</p>
<p id="p1650mcpsimp"><a name="p1650mcpsimp"></a><a name="p1650mcpsimp"></a>注：以十六进制/十进制显示，详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1651mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1653mcpsimp"><a name="p1653mcpsimp"></a><a name="p1653mcpsimp"></a>audio info raw data</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1655mcpsimp"><a name="p1655mcpsimp"></a><a name="p1655mcpsimp"></a>AudioInfoframe原始数据</p>
</td>
</tr>
<tr id="row1656mcpsimp"><td class="cellrowborder" rowspan="11" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p1658mcpsimp"><a name="p1658mcpsimp"></a><a name="p1658mcpsimp"></a>audio path</p>
</td>
<td class="cellrowborder" valign="top" width="24%" headers="mcps1.1.4.1.1 "><p id="p1660mcpsimp"><a name="p1660mcpsimp"></a><a name="p1660mcpsimp"></a>audio enable</p>
</td>
<td class="cellrowborder" valign="top" width="61%" headers="mcps1.1.4.1.2 "><p id="p1662mcpsimp"><a name="p1662mcpsimp"></a><a name="p1662mcpsimp"></a>HDMI硬件音频是否使能。</p>
<p id="p1663mcpsimp"><a name="p1663mcpsimp"></a><a name="p1663mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1664mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1666mcpsimp"><a name="p1666mcpsimp"></a><a name="p1666mcpsimp"></a>audio mute</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1668mcpsimp"><a name="p1668mcpsimp"></a><a name="p1668mcpsimp"></a>音频Mute是否使能。</p>
<p id="p1669mcpsimp"><a name="p1669mcpsimp"></a><a name="p1669mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1670mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1672mcpsimp"><a name="p1672mcpsimp"></a><a name="p1672mcpsimp"></a>sound intf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1674mcpsimp"><a name="p1674mcpsimp"></a><a name="p1674mcpsimp"></a>音频接口类型。</p>
<p id="p1675mcpsimp"><a name="p1675mcpsimp"></a><a name="p1675mcpsimp"></a>取值：{I2S, SPDIF, HBRA, UNKNOWN}</p>
<p id="p1676mcpsimp"><a name="p1676mcpsimp"></a><a name="p1676mcpsimp"></a>注：SSxx目前只支持I2S。</p>
</td>
</tr>
<tr id="row1677mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1679mcpsimp"><a name="p1679mcpsimp"></a><a name="p1679mcpsimp"></a>channel cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1681mcpsimp"><a name="p1681mcpsimp"></a><a name="p1681mcpsimp"></a>当前音频输出通道数。</p>
<p id="p1682mcpsimp"><a name="p1682mcpsimp"></a><a name="p1682mcpsimp"></a>取值：{STR_HEADER, 2_CH, 3_CH, 4_CH, 5_CH, 6_CH, 7_CH, 8_CH}</p>
<p id="p1683mcpsimp"><a name="p1683mcpsimp"></a><a name="p1683mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1684mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1686mcpsimp"><a name="p1686mcpsimp"></a><a name="p1686mcpsimp"></a>sample freq</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1688mcpsimp"><a name="p1688mcpsimp"></a><a name="p1688mcpsimp"></a>当前音频采样频率。</p>
<p id="p1689mcpsimp"><a name="p1689mcpsimp"></a><a name="p1689mcpsimp"></a>注：SSxx仅支持32000Hz、44100Hz、48000Hz采样率。</p>
</td>
</tr>
<tr id="row1690mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1692mcpsimp"><a name="p1692mcpsimp"></a><a name="p1692mcpsimp"></a>sample depth</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1694mcpsimp"><a name="p1694mcpsimp"></a><a name="p1694mcpsimp"></a>当前音频采样深度。</p>
</td>
</tr>
<tr id="row1695mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1697mcpsimp"><a name="p1697mcpsimp"></a><a name="p1697mcpsimp"></a>down sample</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1699mcpsimp"><a name="p1699mcpsimp"></a><a name="p1699mcpsimp"></a>当前音频是否降采样。</p>
<p id="p1700mcpsimp"><a name="p1700mcpsimp"></a><a name="p1700mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1701mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1703mcpsimp"><a name="p1703mcpsimp"></a><a name="p1703mcpsimp"></a>ref cts</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1705mcpsimp"><a name="p1705mcpsimp"></a><a name="p1705mcpsimp"></a>协议规定理论CTS值。</p>
</td>
</tr>
<tr id="row1706mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1708mcpsimp"><a name="p1708mcpsimp"></a><a name="p1708mcpsimp"></a>reg cts</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1710mcpsimp"><a name="p1710mcpsimp"></a><a name="p1710mcpsimp"></a>实际输出的CTS值</p>
<p id="p1711mcpsimp"><a name="p1711mcpsimp"></a><a name="p1711mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1712mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1714mcpsimp"><a name="p1714mcpsimp"></a><a name="p1714mcpsimp"></a>ref n</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1716mcpsimp"><a name="p1716mcpsimp"></a><a name="p1716mcpsimp"></a>协议规定理论N值。</p>
</td>
</tr>
<tr id="row1717mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1719mcpsimp"><a name="p1719mcpsimp"></a><a name="p1719mcpsimp"></a>reg n</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1721mcpsimp"><a name="p1721mcpsimp"></a><a name="p1721mcpsimp"></a>实际输出N值</p>
<p id="p1722mcpsimp"><a name="p1722mcpsimp"></a><a name="p1722mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
</tbody>
</table>

注：N/A/NONE/UNKNOWN为未知/无效；reserved为保留/未知；ERROR为错误。

【HDMI视频调试信息】

```
# cat /proc/umap/hdmi0_vo 
 
[HDMI] Version: [V1.0.0.0 B010 Release], Build Time[Sep  6 2019, 10:49:13]
 
HDMI version:   2.0.0.20190620.0
--------------------- video attr -------------- avi info ----------------
video timing        : 1280*720p60 16:9     |avi infoframe enable: YES                  
disp fmt            : 720P@60              |current format      : 1280*720p60 16:9(vic= 4)
pixel clk           : 74250                |vsif format         : (HDMI vic= 0)
in bit depth        : 10 bit               |bar data present    : NONE                 
in color space      : YCBCR444             |color space         : YCBCR444             
colorimetry         : ITU-R BT.709         |colorimetry         : ITU-R BT.709         
pic aspect ratio    : 16:9                 |pic aspect ratio    : 16:9                 
act aspect ratio    : PICTURE              |act aspect ratio    : PICTURE              
pixel repeat        : 1                    |pixel repeat        : no repetition        
ycc quantization    : LIMITED              |ycc quantization    : LIMITED              
rgb quantization    : DEFAULT              |rgb quantization    : DEFAULT              
ext colorimetry     : XV_YCC601            |ext colorimetry     : XV_YCC601            
stereo mode         : NONE                 |it content valid    : NO                   
vsync pol           : 0                    |hsync pol           : 0                    
------------ video path -------------------|it content type     : GRAPHICS             
video mute          : NO                   |pic scaling         : UNKNOWN              
out bit depth       : 08 bit               |act fmt present     : YES                  
out color space     : YCBCR444             |scan info           : NONE                 
ycbcr420_422        : NO                   |avi info raw data   :
ycbcr422_444        : NO                   | 82 02 0d 73  50 a8 00 04 
ycbcr444_422        : NO                   | 00 00 00 00  00 00 00 00 
ycbcr422_420        : NO                   | 00 
rgb2ycbcr           : NO                   |vsif info raw data  :
ycbcr2rgb           : NO                   | 81 01 07 68  03 0c 00 00 
dither              : 10_8                 | 00 00 00 
deep color mode     : 24 bit(OFF)          | 
```

【调试信息分析】

记录当前HDMI视频工作状态。

【参数说明】

<a name="table1758mcpsimp"></a>
<table><thead align="left"><tr id="row1764mcpsimp"><th class="cellrowborder" colspan="2" valign="top" id="mcps1.1.4.1.1"><p id="p1766mcpsimp"><a name="p1766mcpsimp"></a><a name="p1766mcpsimp"></a>Parameter</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.1.4.1.2"><p id="p1768mcpsimp"><a name="p1768mcpsimp"></a><a name="p1768mcpsimp"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="row1770mcpsimp"><td class="cellrowborder" rowspan="14" valign="top" width="14.99%" headers="mcps1.1.4.1.1 "><p id="p1772mcpsimp"><a name="p1772mcpsimp"></a><a name="p1772mcpsimp"></a>video attr</p>
</td>
<td class="cellrowborder" valign="top" width="23.990000000000002%" headers="mcps1.1.4.1.1 "><p id="p1774mcpsimp"><a name="p1774mcpsimp"></a><a name="p1774mcpsimp"></a>video timing</p>
</td>
<td class="cellrowborder" valign="top" width="61.019999999999996%" headers="mcps1.1.4.1.2 "><p id="p1776mcpsimp"><a name="p1776mcpsimp"></a><a name="p1776mcpsimp"></a>当前视频时序。</p>
<p id="p1777mcpsimp"><a name="p1777mcpsimp"></a><a name="p1777mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》、《VESA Display Monitor Timing Standard》。</p>
</td>
</tr>
<tr id="row1778mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1780mcpsimp"><a name="p1780mcpsimp"></a><a name="p1780mcpsimp"></a>disp fmt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1782mcpsimp"><a name="p1782mcpsimp"></a><a name="p1782mcpsimp"></a>当前视频制式。</p>
<p id="p1783mcpsimp"><a name="p1783mcpsimp"></a><a name="p1783mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》、《VESA Display Monitor Timing Standard》。</p>
</td>
</tr>
<tr id="row1784mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1786mcpsimp"><a name="p1786mcpsimp"></a><a name="p1786mcpsimp"></a>pixel clk</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1788mcpsimp"><a name="p1788mcpsimp"></a><a name="p1788mcpsimp"></a>像素时钟。</p>
<p id="p1789mcpsimp"><a name="p1789mcpsimp"></a><a name="p1789mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》、《VESA Display Monitor Timing Standard》。</p>
</td>
</tr>
<tr id="row1790mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1792mcpsimp"><a name="p1792mcpsimp"></a><a name="p1792mcpsimp"></a>in bit depth</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1794mcpsimp"><a name="p1794mcpsimp"></a><a name="p1794mcpsimp"></a>VO输出给HDMI视频色深。</p>
<p id="p1795mcpsimp"><a name="p1795mcpsimp"></a><a name="p1795mcpsimp"></a>取值：{8bit, 10bit, 12bit, 16bit, UNKNOWN}</p>
<p id="p1796mcpsimp"><a name="p1796mcpsimp"></a><a name="p1796mcpsimp"></a>注： UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1797mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1799mcpsimp"><a name="p1799mcpsimp"></a><a name="p1799mcpsimp"></a>in color space</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1801mcpsimp"><a name="p1801mcpsimp"></a><a name="p1801mcpsimp"></a>VO输出给HDMI的颜色空间。</p>
<p id="p1802mcpsimp"><a name="p1802mcpsimp"></a><a name="p1802mcpsimp"></a>取值：{RGB, YCbCr422, YCbCr444, YCbCr420, UNKNOWN}</p>
<p id="p1803mcpsimp"><a name="p1803mcpsimp"></a><a name="p1803mcpsimp"></a>注： UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1804mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1806mcpsimp"><a name="p1806mcpsimp"></a><a name="p1806mcpsimp"></a>colorimetry</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1808mcpsimp"><a name="p1808mcpsimp"></a><a name="p1808mcpsimp"></a>VO输出给HDMI的色域。</p>
<p id="p1809mcpsimp"><a name="p1809mcpsimp"></a><a name="p1809mcpsimp"></a>取值：{No Data, SMPTE 170M, ITU-R BT.709, Extended }</p>
</td>
</tr>
<tr id="row1810mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1812mcpsimp"><a name="p1812mcpsimp"></a><a name="p1812mcpsimp"></a>pic aspect ratio</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1814mcpsimp"><a name="p1814mcpsimp"></a><a name="p1814mcpsimp"></a>输入视频的宽高比。</p>
<p id="p1815mcpsimp"><a name="p1815mcpsimp"></a><a name="p1815mcpsimp"></a>取值：{ NONE, 4:3, 16:9, FUTURE, UNKNOWN}</p>
<p id="p1816mcpsimp"><a name="p1816mcpsimp"></a><a name="p1816mcpsimp"></a>注： UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1817mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1819mcpsimp"><a name="p1819mcpsimp"></a><a name="p1819mcpsimp"></a>act aspect ratio</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1821mcpsimp"><a name="p1821mcpsimp"></a><a name="p1821mcpsimp"></a>输入视频有效图像的宽高比。</p>
<p id="p1822mcpsimp"><a name="p1822mcpsimp"></a><a name="p1822mcpsimp"></a>取值：{ Reserved, 16:9_TOP, 14:9_TOP, 16:9_CENTER, PICTURE, 4:3, 16:9, 14:9, 4:3_SP_14_9, 16:9_SP_14_9, 16:9_SP_4_3, UNKNOWN}</p>
<p id="p1823mcpsimp"><a name="p1823mcpsimp"></a><a name="p1823mcpsimp"></a>注： UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1824mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1826mcpsimp"><a name="p1826mcpsimp"></a><a name="p1826mcpsimp"></a>pixel repeat</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1828mcpsimp"><a name="p1828mcpsimp"></a><a name="p1828mcpsimp"></a>像素Repeat次数。</p>
</td>
</tr>
<tr id="row1829mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1831mcpsimp"><a name="p1831mcpsimp"></a><a name="p1831mcpsimp"></a>ycc quantization</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1833mcpsimp"><a name="p1833mcpsimp"></a><a name="p1833mcpsimp"></a>YCC量化范围。</p>
<p id="p1834mcpsimp"><a name="p1834mcpsimp"></a><a name="p1834mcpsimp"></a>取值：{LIMITED, FULL, UNKNOWN}</p>
<p id="p1835mcpsimp"><a name="p1835mcpsimp"></a><a name="p1835mcpsimp"></a>注： UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1836mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1838mcpsimp"><a name="p1838mcpsimp"></a><a name="p1838mcpsimp"></a>rgb quantization</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1840mcpsimp"><a name="p1840mcpsimp"></a><a name="p1840mcpsimp"></a>RGB量化范围。</p>
<p id="p1841mcpsimp"><a name="p1841mcpsimp"></a><a name="p1841mcpsimp"></a>取值：{DEFAULT, LIMITED, FULL, UNKNOWN}</p>
<p id="p1842mcpsimp"><a name="p1842mcpsimp"></a><a name="p1842mcpsimp"></a>注：UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1843mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1845mcpsimp"><a name="p1845mcpsimp"></a><a name="p1845mcpsimp"></a>ext colorimetry</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1847mcpsimp"><a name="p1847mcpsimp"></a><a name="p1847mcpsimp"></a>输入图像扩展色域。</p>
<p id="p1848mcpsimp"><a name="p1848mcpsimp"></a><a name="p1848mcpsimp"></a>取值：{XV_YCC601, XV_YCC709, S_YCC601, ADOBE_YCC601, ADOBE_RGB, BT2020_cYCC, BT2020_RGB/YCC, UNKNOWN}</p>
</td>
</tr>
<tr id="row1849mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1851mcpsimp"><a name="p1851mcpsimp"></a><a name="p1851mcpsimp"></a>stereo mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1853mcpsimp"><a name="p1853mcpsimp"></a><a name="p1853mcpsimp"></a>StereoMode</p>
<p id="p1854mcpsimp"><a name="p1854mcpsimp"></a><a name="p1854mcpsimp"></a>取值：{FRAME_PACK, FIELD_ALTER, LINE_ALTERN, SBS_FULL, L_DEPTH, L_DEPTH_GGD, TAndB, Reserved, SByS_HALF, NONE}</p>
<p id="p1855mcpsimp"><a name="p1855mcpsimp"></a><a name="p1855mcpsimp"></a>注：NONE为未知。</p>
</td>
</tr>
<tr id="row1856mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1858mcpsimp"><a name="p1858mcpsimp"></a><a name="p1858mcpsimp"></a>vsync pol</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1860mcpsimp"><a name="p1860mcpsimp"></a><a name="p1860mcpsimp"></a>HDMI是否使能垂直反向。</p>
<p id="p1861mcpsimp"><a name="p1861mcpsimp"></a><a name="p1861mcpsimp"></a>取值：{0, 1}</p>
</td>
</tr>
<tr id="row1862mcpsimp"><td class="cellrowborder" rowspan="11" valign="top" width="14.99%" headers="mcps1.1.4.1.1 "><p id="p1864mcpsimp"><a name="p1864mcpsimp"></a><a name="p1864mcpsimp"></a>video path</p>
</td>
<td class="cellrowborder" valign="top" width="23.990000000000002%" headers="mcps1.1.4.1.1 "><p id="p1866mcpsimp"><a name="p1866mcpsimp"></a><a name="p1866mcpsimp"></a>video mute</p>
</td>
<td class="cellrowborder" valign="top" width="61.019999999999996%" headers="mcps1.1.4.1.2 "><p id="p1868mcpsimp"><a name="p1868mcpsimp"></a><a name="p1868mcpsimp"></a>HDMI硬件是否使能视频Mute。</p>
<p id="p1869mcpsimp"><a name="p1869mcpsimp"></a><a name="p1869mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1870mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1872mcpsimp"><a name="p1872mcpsimp"></a><a name="p1872mcpsimp"></a>out bit depth</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1874mcpsimp"><a name="p1874mcpsimp"></a><a name="p1874mcpsimp"></a>输出视频色深。</p>
<p id="p1875mcpsimp"><a name="p1875mcpsimp"></a><a name="p1875mcpsimp"></a>取值：{8bit, 10bit, 12bit, 16bit, UNKNOWN}</p>
</td>
</tr>
<tr id="row1876mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1878mcpsimp"><a name="p1878mcpsimp"></a><a name="p1878mcpsimp"></a>out color space</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1880mcpsimp"><a name="p1880mcpsimp"></a><a name="p1880mcpsimp"></a>输出图像颜色空间</p>
<p id="p1881mcpsimp"><a name="p1881mcpsimp"></a><a name="p1881mcpsimp"></a>取值：{RGB, YCbCr422, YCbCr444, YCbCr420, UNKNOWN}</p>
</td>
</tr>
<tr id="row1882mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1884mcpsimp"><a name="p1884mcpsimp"></a><a name="p1884mcpsimp"></a>ycbcr420_422</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1886mcpsimp"><a name="p1886mcpsimp"></a><a name="p1886mcpsimp"></a>颜色空间是否有进行YCbCr420到YCbCr422转换</p>
<p id="p1887mcpsimp"><a name="p1887mcpsimp"></a><a name="p1887mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1888mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1890mcpsimp"><a name="p1890mcpsimp"></a><a name="p1890mcpsimp"></a>ycbcr422_444</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1892mcpsimp"><a name="p1892mcpsimp"></a><a name="p1892mcpsimp"></a>颜色空间是否有进行YCbCr422到YCbCr444转换。</p>
<p id="p1893mcpsimp"><a name="p1893mcpsimp"></a><a name="p1893mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1894mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1896mcpsimp"><a name="p1896mcpsimp"></a><a name="p1896mcpsimp"></a>ycbcr444_422</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1898mcpsimp"><a name="p1898mcpsimp"></a><a name="p1898mcpsimp"></a>颜色空间是否有进行YCbCr444到YCbCr422转换。</p>
<p id="p1899mcpsimp"><a name="p1899mcpsimp"></a><a name="p1899mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1900mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1902mcpsimp"><a name="p1902mcpsimp"></a><a name="p1902mcpsimp"></a>ycbcr422_420</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1904mcpsimp"><a name="p1904mcpsimp"></a><a name="p1904mcpsimp"></a>颜色空间是否有进行YCbCr422到YCbCr420转换。</p>
<p id="p1905mcpsimp"><a name="p1905mcpsimp"></a><a name="p1905mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1906mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1908mcpsimp"><a name="p1908mcpsimp"></a><a name="p1908mcpsimp"></a>rgb2ycbcr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1910mcpsimp"><a name="p1910mcpsimp"></a><a name="p1910mcpsimp"></a>颜色空间是否有进行RGB到YCbCr的转换。</p>
<p id="p1911mcpsimp"><a name="p1911mcpsimp"></a><a name="p1911mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1912mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1914mcpsimp"><a name="p1914mcpsimp"></a><a name="p1914mcpsimp"></a>ycbcr2rgb</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1916mcpsimp"><a name="p1916mcpsimp"></a><a name="p1916mcpsimp"></a>颜色空间是否有进行YCbCr到RGB的转换。</p>
<p id="p1917mcpsimp"><a name="p1917mcpsimp"></a><a name="p1917mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1918mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1920mcpsimp"><a name="p1920mcpsimp"></a><a name="p1920mcpsimp"></a>dither</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1922mcpsimp"><a name="p1922mcpsimp"></a><a name="p1922mcpsimp"></a>Dither工作模式。</p>
<p id="p1923mcpsimp"><a name="p1923mcpsimp"></a><a name="p1923mcpsimp"></a>取值：{12_10, 12_8, 10_8, disable}</p>
</td>
</tr>
<tr id="row1924mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1926mcpsimp"><a name="p1926mcpsimp"></a><a name="p1926mcpsimp"></a>deep color mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1928mcpsimp"><a name="p1928mcpsimp"></a><a name="p1928mcpsimp"></a>逻辑工作的DeepColor模式。</p>
<p id="p1929mcpsimp"><a name="p1929mcpsimp"></a><a name="p1929mcpsimp"></a>取值：{24 bit, 30 bit, 36 bit, 48 bit, 24 bit(OFF), UNKNOWN}</p>
</td>
</tr>
<tr id="row1930mcpsimp"><td class="cellrowborder" rowspan="20" valign="top" width="14.99%" headers="mcps1.1.4.1.1 "><p id="p1932mcpsimp"><a name="p1932mcpsimp"></a><a name="p1932mcpsimp"></a>avi info</p>
</td>
<td class="cellrowborder" valign="top" width="23.990000000000002%" headers="mcps1.1.4.1.1 "><p id="p1934mcpsimp"><a name="p1934mcpsimp"></a><a name="p1934mcpsimp"></a>avi infoframe enable</p>
</td>
<td class="cellrowborder" valign="top" width="61.019999999999996%" headers="mcps1.1.4.1.2 "><p id="p1936mcpsimp"><a name="p1936mcpsimp"></a><a name="p1936mcpsimp"></a>AVI信息帧是否使能。</p>
<p id="p1937mcpsimp"><a name="p1937mcpsimp"></a><a name="p1937mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row1938mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1940mcpsimp"><a name="p1940mcpsimp"></a><a name="p1940mcpsimp"></a>current format</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1942mcpsimp"><a name="p1942mcpsimp"></a><a name="p1942mcpsimp"></a>当前视频制式/VIC码。</p>
<p id="p1943mcpsimp"><a name="p1943mcpsimp"></a><a name="p1943mcpsimp"></a>注：详见《 EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1944mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1946mcpsimp"><a name="p1946mcpsimp"></a><a name="p1946mcpsimp"></a>vsif format</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1948mcpsimp"><a name="p1948mcpsimp"></a><a name="p1948mcpsimp"></a>4k非3D制式/VIC码。</p>
<p id="p1949mcpsimp"><a name="p1949mcpsimp"></a><a name="p1949mcpsimp"></a>注：详见《HDMI Specification 2.0》</p>
</td>
</tr>
<tr id="row1950mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1952mcpsimp"><a name="p1952mcpsimp"></a><a name="p1952mcpsimp"></a>bar data present</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1954mcpsimp"><a name="p1954mcpsimp"></a><a name="p1954mcpsimp"></a>Bar信息。</p>
<p id="p1955mcpsimp"><a name="p1955mcpsimp"></a><a name="p1955mcpsimp"></a>取值：{NONE, HnVp, HpVn, HpVp}</p>
<p id="p1956mcpsimp"><a name="p1956mcpsimp"></a><a name="p1956mcpsimp"></a>注:：H， horizontal 水平；V， vertical垂直；p， positive 正；n， negative 负；详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row1957mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1959mcpsimp"><a name="p1959mcpsimp"></a><a name="p1959mcpsimp"></a>color space</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1961mcpsimp"><a name="p1961mcpsimp"></a><a name="p1961mcpsimp"></a>视频颜色空间。</p>
<p id="p1962mcpsimp"><a name="p1962mcpsimp"></a><a name="p1962mcpsimp"></a>取值：{RGB, YCbCr422, YCbCr444, YCbCr420, UNKNOWN}</p>
<p id="p1963mcpsimp"><a name="p1963mcpsimp"></a><a name="p1963mcpsimp"></a>注：UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1964mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1966mcpsimp"><a name="p1966mcpsimp"></a><a name="p1966mcpsimp"></a>colorimetry</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1968mcpsimp"><a name="p1968mcpsimp"></a><a name="p1968mcpsimp"></a>色域。</p>
<p id="p1969mcpsimp"><a name="p1969mcpsimp"></a><a name="p1969mcpsimp"></a>取值：{ No Data, SMPTE 170M, ITU-R BT.709, Extended }</p>
</td>
</tr>
<tr id="row1970mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1972mcpsimp"><a name="p1972mcpsimp"></a><a name="p1972mcpsimp"></a>pic aspect ratio</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1974mcpsimp"><a name="p1974mcpsimp"></a><a name="p1974mcpsimp"></a>视频的宽高比。</p>
<p id="p1975mcpsimp"><a name="p1975mcpsimp"></a><a name="p1975mcpsimp"></a>取值：{NONE, 4:3, 16:9, FUTURE, UNKNOWN}</p>
<p id="p1976mcpsimp"><a name="p1976mcpsimp"></a><a name="p1976mcpsimp"></a>注：UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1977mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1979mcpsimp"><a name="p1979mcpsimp"></a><a name="p1979mcpsimp"></a>act aspect ratio</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1981mcpsimp"><a name="p1981mcpsimp"></a><a name="p1981mcpsimp"></a>视频有效图像的宽高比。</p>
<p id="p1982mcpsimp"><a name="p1982mcpsimp"></a><a name="p1982mcpsimp"></a>取值：{ Reserved, 16:9_TOP, 14:9_TOP, 16:9_CENTER, PICTURE, 4:3, 16:9, 14:9, 4:3_SP_14_9, 16:9_SP_14_9, 16:9_SP_4_3, UNKNOWN}</p>
<p id="p1983mcpsimp"><a name="p1983mcpsimp"></a><a name="p1983mcpsimp"></a>注：UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1984mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1986mcpsimp"><a name="p1986mcpsimp"></a><a name="p1986mcpsimp"></a>pixel repeat</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1988mcpsimp"><a name="p1988mcpsimp"></a><a name="p1988mcpsimp"></a>像素Repeat次数。</p>
</td>
</tr>
<tr id="row1989mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1991mcpsimp"><a name="p1991mcpsimp"></a><a name="p1991mcpsimp"></a>ycc quantization</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1993mcpsimp"><a name="p1993mcpsimp"></a><a name="p1993mcpsimp"></a>YCC量化范围。</p>
<p id="p1994mcpsimp"><a name="p1994mcpsimp"></a><a name="p1994mcpsimp"></a>取值：{LIMITED, FULL, UNKNOWN}</p>
<p id="p1995mcpsimp"><a name="p1995mcpsimp"></a><a name="p1995mcpsimp"></a>注：UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row1996mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1998mcpsimp"><a name="p1998mcpsimp"></a><a name="p1998mcpsimp"></a>rgb quantization</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2000mcpsimp"><a name="p2000mcpsimp"></a><a name="p2000mcpsimp"></a>RGB量化范围。</p>
<p id="p2001mcpsimp"><a name="p2001mcpsimp"></a><a name="p2001mcpsimp"></a>取值：{DEFAULT, LIMITED, FULL, UNKNOWN}</p>
<p id="p2002mcpsimp"><a name="p2002mcpsimp"></a><a name="p2002mcpsimp"></a>注：UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row2003mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2005mcpsimp"><a name="p2005mcpsimp"></a><a name="p2005mcpsimp"></a>ext colorimetry</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2007mcpsimp"><a name="p2007mcpsimp"></a><a name="p2007mcpsimp"></a>图像扩展色域。</p>
<p id="p2008mcpsimp"><a name="p2008mcpsimp"></a><a name="p2008mcpsimp"></a>取值：{XV_YCC601, XV_YCC709, S_YCC601, ADOBE_YCC601, ADOBE_RGB, BT2020_YCC, BT2020_RGB/YCC, UNKNOWN}</p>
<p id="p2009mcpsimp"><a name="p2009mcpsimp"></a><a name="p2009mcpsimp"></a>注： UNKNOWN为未知。</p>
</td>
</tr>
<tr id="row2010mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2012mcpsimp"><a name="p2012mcpsimp"></a><a name="p2012mcpsimp"></a>it content valid</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2014mcpsimp"><a name="p2014mcpsimp"></a><a name="p2014mcpsimp"></a>IT Content是否有效。</p>
<p id="p2015mcpsimp"><a name="p2015mcpsimp"></a><a name="p2015mcpsimp"></a>取值：{YES, NO}</p>
<p id="p2016mcpsimp"><a name="p2016mcpsimp"></a><a name="p2016mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2017mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2019mcpsimp"><a name="p2019mcpsimp"></a><a name="p2019mcpsimp"></a>hsync pol</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2021mcpsimp"><a name="p2021mcpsimp"></a><a name="p2021mcpsimp"></a>是否使能水平反向。</p>
<p id="p2022mcpsimp"><a name="p2022mcpsimp"></a><a name="p2022mcpsimp"></a>取值：{0, 1}</p>
</td>
</tr>
<tr id="row2023mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2025mcpsimp"><a name="p2025mcpsimp"></a><a name="p2025mcpsimp"></a>it content type</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2027mcpsimp"><a name="p2027mcpsimp"></a><a name="p2027mcpsimp"></a>IT Content类型。</p>
<p id="p2028mcpsimp"><a name="p2028mcpsimp"></a><a name="p2028mcpsimp"></a>取值：{GRAPHICS, PHOTO, CINEMA, GAME}</p>
</td>
</tr>
<tr id="row2029mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2031mcpsimp"><a name="p2031mcpsimp"></a><a name="p2031mcpsimp"></a>pic scaling</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2033mcpsimp"><a name="p2033mcpsimp"></a><a name="p2033mcpsimp"></a>图像均衡。</p>
<p id="p2034mcpsimp"><a name="p2034mcpsimp"></a><a name="p2034mcpsimp"></a>取值：{UNKNOWN, HpVn, HnVp, HpVp}</p>
<p id="p2035mcpsimp"><a name="p2035mcpsimp"></a><a name="p2035mcpsimp"></a>注:：H, horizontal 水平；V, vertical垂直；p, positive 正；n, negative 负；详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2036mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2038mcpsimp"><a name="p2038mcpsimp"></a><a name="p2038mcpsimp"></a>act fmt present</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2040mcpsimp"><a name="p2040mcpsimp"></a><a name="p2040mcpsimp"></a>有效信息是否显示。</p>
<p id="p2041mcpsimp"><a name="p2041mcpsimp"></a><a name="p2041mcpsimp"></a>取值：{YES, NO}</p>
<p id="p2042mcpsimp"><a name="p2042mcpsimp"></a><a name="p2042mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2043mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2045mcpsimp"><a name="p2045mcpsimp"></a><a name="p2045mcpsimp"></a>scan info</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2047mcpsimp"><a name="p2047mcpsimp"></a><a name="p2047mcpsimp"></a>Scan标志位</p>
<p id="p2048mcpsimp"><a name="p2048mcpsimp"></a><a name="p2048mcpsimp"></a>取值：{NONE, OVER_SCAN, UNDERS_SCAN, Reserved}</p>
</td>
</tr>
<tr id="row2049mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2051mcpsimp"><a name="p2051mcpsimp"></a><a name="p2051mcpsimp"></a>avi info raw data</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2053mcpsimp"><a name="p2053mcpsimp"></a><a name="p2053mcpsimp"></a>AVI 信息帧原始数据。</p>
</td>
</tr>
<tr id="row2054mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2056mcpsimp"><a name="p2056mcpsimp"></a><a name="p2056mcpsimp"></a>vsif info raw data</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2058mcpsimp"><a name="p2058mcpsimp"></a><a name="p2058mcpsimp"></a>VSIF信息帧原始数据。</p>
</td>
</tr>
</tbody>
</table>

注：N/A/NONE/UNKNOWN为未知/无效；reserved为保留/未知；ERROR为错误。

【EDID调试信息】

```
# cat /proc/umap/hdmi0_sink
 
[HDMI] Version: [V1.0.0.0 B010 Release], Build Time[Sep  6 2019, 10:49:13]
 
HDMI version:   2.0.0.20190620.0
 
------------------------------ edid raw data ----------------------------
/* 00H: */ 0x00,0xff,0xff,0xff,  0xff,0xff,0xff,0x00,  0x10,0xac,0x16,0xf0,  0x4c,0x45,0x43,0x41,  
/* 0fH: */ 0x1c,0x15,0x01,0x03,  0x80,0x34,0x20,0x78,  0xea,0x1e,0xc5,0xae,  0x4f,0x34,0xb1,0x26,  
/* 1fH: */ 0x0e,0x50,0x54,0xa5,  0x4b,0x00,0x81,0x80,  0xa9,0x40,0xd1,0x00,  0x71,0x4f,0x01,0x01,  
/* 2fH: */ 0x01,0x01,0x01,0x01,  0x01,0x01,0x28,0x3c,  0x80,0xa0,0x70,0xb0,  0x23,0x40,0x30,0x20,  
/* 3fH: */ 0x36,0x00,0x06,0x44,  0x21,0x00,0x00,0x1a,  0x00,0x00,0x00,0xff,  0x00,0x38,0x32,0x57,  
/* 4fH: */ 0x58,0x44,0x31,0x37,  0x34,0x41,0x43,0x45,  0x4c,0x0a,0x00,0x00,  0x00,0xfc,0x00,0x44,  
/* 5fH: */ 0x45,0x4c,0x4c,0x20,  0x55,0x32,0x34,0x31,  0x30,0x0a,0x20,0x20,  0x00,0x00,0x00,0xfd,  
/* 6fH: */ 0x00,0x38,0x4c,0x1e,  0x51,0x11,0x00,0x0a,  0x20,0x20,0x20,0x20,  0x20,0x20,0x01,0x1c,  
/* 7fH: */ 0x02,0x03,0x29,0xf1,  0x50,0x90,0x05,0x04,  0x03,0x02,0x07,0x16,  0x01,0x1f,0x12,0x13,  
/* 8fH: */ 0x14,0x20,0x15,0x11,  0x06,0x23,0x09,0x07,  0x07,0x67,0x03,0x0c,  0x00,0x10,0x00,0x38,  
/* 9fH: */ 0x2d,0x83,0x01,0x00,  0x00,0xe3,0x05,0x03,  0x01,0x02,0x3a,0x80,  0x18,0x71,0x38,0x2d,  
/* afH: */ 0x40,0x58,0x2c,0x45,  0x00,0x06,0x44,0x21,  0x00,0x00,0x1e,0x01,  0x1d,0x80,0x18,0x71,  
/* bfH: */ 0x1c,0x16,0x20,0x58,  0x2c,0x25,0x00,0x06,  0x44,0x21,0x00,0x00,  0x9e,0x01,0x1d,0x00,  
/* cfH: */ 0x72,0x51,0xd0,0x1e,  0x20,0x6e,0x28,0x55,  0x00,0x06,0x44,0x21,  0x00,0x00,0x1e,0x8c,  
/* dfH: */ 0x0a,0xd0,0x8a,0x20,  0xe0,0x2d,0x10,0x10,  0x3e,0x96,0x00,0x06,  0x44,0x21,0x00,0x00,  
/* efH: */ 0x18,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x3e,
-------------------------------- sw status ------------------------------
cap from sink       : YES                  raw update err cnt  : 0                    
cap is valid        : YES                  parse error type    : 0                    
raw is valid        : YES                  parse warn type     : 0x00000000
raw get err cnt     : 0                    raw length          : 256                  
-------------------------------- basic cap ------------------------------
hdmi1.4 support     : YES                 1st block version   : 1.3
hdmi2.0 support     : NO                  manufacturer name   : DEL                 
max tmds clock(MHz) : 225                 product code        : 61462               
serial number       : 1094927692          week of manufacture : 28                  
max disp width      : 52                  maxdisp height      : 32                  
scdc support        : NO                  year of manufacture : 2011                
dvi dual support    : NO                  cec addr is valid   : YES                 
ai support          : NO                  cec addr            : 01.00.00.00
ext block cnt       : 1                   speaker support     : FL_FR           
rgb quan selectable : NO                  ycc quan selectable : NO                  
-------------------------------- video cap ------------------------------
native format       : 1080P_60 16:9(vic 16)
color space         : rgb444 ycbcr444 ycbcr422
deep color          : rgb_30bit rgb_36bit ycbcr444_samergb
ycbcr420 deep color :
ycbcr420[also]      :
ycbcr420[olny]      :
colorimetry         : xvycc601 xvycc709
-------------------------------- format cap -----------------------------
1080P_60 16:9       1080i_60 16:9       720P_60 16:9        480P_60 16:9        
480P_60 4:3         NTSC 16:9           PAL 16:9            640X480_60 4:3      
1080P_50 16:9       576P_50 16:9        720P_50 16:9        1080i_50 16:9       
1080P_24 16:9       PAL 4:3             576P_50 4:3         NTSC 4:3            
1280x1024_60        1600x1200_60        1920x1200_60        1152x864_75         
V800X600_60         V800X600_56         V640X480_75         V640X480_72         
V640X480_67         V640X480_60         V720X400_88         V720X400_70         
 
---------------------------------- audio cap ----------------------------
NO.0:
code type           : L-PCM               max channel num     : 2                   
max bit rate(KHz)   : N/A                 bit depth           : 16 20 24
sample rate(Hz)     : 32000 44100 48000
------------------------------------ detail timing --------------------------------------
[NO.]:hact|vact|p/i |pclk|aspw|asph|hfb |hpw |hbb |vfb |vpw |vbb |img_w|img_h|ihs |ivs |idv 
[  0]:1920|1200|P   |154m|0   |0   |48  |32  |112 |3   |6   |32  |518  |324  |YES |NO  |NO   
[  1]:1920|1080|P   |148m|0   |0   |88  |44  |192 |4   |5   |41  |518  |324  |YES |YES |NO   
[  2]:1920|540 |I   |74 m|0   |0   |88  |44  |192 |2   |5   |20  |518  |324  |YES |YES |NO   
[  3]:1280|720 |P   |74 m|0   |0   |110 |40  |260 |5   |5   |25  |518  |324  |YES |YES |NO   
[  4]:720 |480 |P   |27 m|0   |0   |16  |62  |122 |9   |6   |36  |518  |324  |NO  |NO  |NO 
```

【调试信息分析】

记录当前显示设备的EDID。

【参数说明】

<a name="table2128mcpsimp"></a>
<table><thead align="left"><tr id="row2134mcpsimp"><th class="cellrowborder" colspan="2" valign="top" id="mcps1.1.4.1.1"><p id="p2136mcpsimp"><a name="p2136mcpsimp"></a><a name="p2136mcpsimp"></a>Parameter</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.1.4.1.2"><p id="p2138mcpsimp"><a name="p2138mcpsimp"></a><a name="p2138mcpsimp"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="row2140mcpsimp"><td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p2142mcpsimp"><a name="p2142mcpsimp"></a><a name="p2142mcpsimp"></a>edid raw data</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.4.1.1 "><p id="p2144mcpsimp"><a name="p2144mcpsimp"></a><a name="p2144mcpsimp"></a>-</p>
</td>
<td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.4.1.2 "><p id="p2146mcpsimp"><a name="p2146mcpsimp"></a><a name="p2146mcpsimp"></a>EDID原始数据，长度256Bytes</p>
</td>
</tr>
<tr id="row2147mcpsimp"><td class="cellrowborder" rowspan="8" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p2149mcpsimp"><a name="p2149mcpsimp"></a><a name="p2149mcpsimp"></a>sw status</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.4.1.1 "><p id="p2151mcpsimp"><a name="p2151mcpsimp"></a><a name="p2151mcpsimp"></a>cap from sink</p>
</td>
<td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.4.1.2 "><p id="p2153mcpsimp"><a name="p2153mcpsimp"></a><a name="p2153mcpsimp"></a>能力集是否来自显示设备(SINK)。</p>
<p id="p2154mcpsimp"><a name="p2154mcpsimp"></a><a name="p2154mcpsimp"></a>取值：{YES, NO}</p>
<p id="p2155mcpsimp"><a name="p2155mcpsimp"></a><a name="p2155mcpsimp"></a>注：能力集可能来自用于测试的数据，此时指示NO。</p>
</td>
</tr>
<tr id="row2156mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2158mcpsimp"><a name="p2158mcpsimp"></a><a name="p2158mcpsimp"></a>raw update err cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2160mcpsimp"><a name="p2160mcpsimp"></a><a name="p2160mcpsimp"></a>原始数据更新错误次数。</p>
</td>
</tr>
<tr id="row2161mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2163mcpsimp"><a name="p2163mcpsimp"></a><a name="p2163mcpsimp"></a>cap is valid</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2165mcpsimp"><a name="p2165mcpsimp"></a><a name="p2165mcpsimp"></a>能力集是否有效。</p>
<p id="p2166mcpsimp"><a name="p2166mcpsimp"></a><a name="p2166mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2167mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2169mcpsimp"><a name="p2169mcpsimp"></a><a name="p2169mcpsimp"></a>parse error type</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2171mcpsimp"><a name="p2171mcpsimp"></a><a name="p2171mcpsimp"></a>EDID解析错误标志。</p>
<p id="p2172mcpsimp"><a name="p2172mcpsimp"></a><a name="p2172mcpsimp"></a>取值：</p>
<p id="p2173mcpsimp"><a name="p2173mcpsimp"></a><a name="p2173mcpsimp"></a>0：无错误；</p>
<p id="p2174mcpsimp"><a name="p2174mcpsimp"></a><a name="p2174mcpsimp"></a>1：校验错误；</p>
<p id="p2175mcpsimp"><a name="p2175mcpsimp"></a><a name="p2175mcpsimp"></a>2：数据头错误；</p>
<p id="p2176mcpsimp"><a name="p2176mcpsimp"></a><a name="p2176mcpsimp"></a>3：基础数据块为非1.3版本；</p>
<p id="p2177mcpsimp"><a name="p2177mcpsimp"></a><a name="p2177mcpsimp"></a>4：扩展数据块TAG值未知；</p>
<p id="p2178mcpsimp"><a name="p2178mcpsimp"></a><a name="p2178mcpsimp"></a>5：CEA值无效；</p>
<p id="p2179mcpsimp"><a name="p2179mcpsimp"></a><a name="p2179mcpsimp"></a>6及以上，保留值。</p>
</td>
</tr>
<tr id="row2180mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2182mcpsimp"><a name="p2182mcpsimp"></a><a name="p2182mcpsimp"></a>raw is valid</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2184mcpsimp"><a name="p2184mcpsimp"></a><a name="p2184mcpsimp"></a>原始数据是否有效。</p>
<p id="p2185mcpsimp"><a name="p2185mcpsimp"></a><a name="p2185mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2186mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2188mcpsimp"><a name="p2188mcpsimp"></a><a name="p2188mcpsimp"></a>parse warn type</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2190mcpsimp"><a name="p2190mcpsimp"></a><a name="p2190mcpsimp"></a>EDID解析警告标志。</p>
<p id="p2191mcpsimp"><a name="p2191mcpsimp"></a><a name="p2191mcpsimp"></a>取值：{0x00000000~0xffffffff}</p>
<p id="p2192mcpsimp"><a name="p2192mcpsimp"></a><a name="p2192mcpsimp"></a>取值中每bit表示一类警告,见下</p>
<p id="p2193mcpsimp"><a name="p2193mcpsimp"></a><a name="p2193mcpsimp"></a>Bit0：无警告；</p>
<p id="p2194mcpsimp"><a name="p2194mcpsimp"></a><a name="p2194mcpsimp"></a>Bit1：VENDOR块无效；</p>
<p id="p2195mcpsimp"><a name="p2195mcpsimp"></a><a name="p2195mcpsimp"></a>Bit2：DTD块数量溢出；</p>
<p id="p2196mcpsimp"><a name="p2196mcpsimp"></a><a name="p2196mcpsimp"></a>Bit3：DTD块非法；</p>
<p id="p2197mcpsimp"><a name="p2197mcpsimp"></a><a name="p2197mcpsimp"></a>Bit4：无扩展块；</p>
<p id="p2198mcpsimp"><a name="p2198mcpsimp"></a><a name="p2198mcpsimp"></a>Bit5：扩展块数量大于4；</p>
<p id="p2199mcpsimp"><a name="p2199mcpsimp"></a><a name="p2199mcpsimp"></a>Bit6：AUDIO块数量溢出；</p>
<p id="p2200mcpsimp"><a name="p2200mcpsimp"></a><a name="p2200mcpsimp"></a>Bit7：VIC个数溢出；</p>
<p id="p2201mcpsimp"><a name="p2201mcpsimp"></a><a name="p2201mcpsimp"></a>Bit8：VIC非法；</p>
<p id="p2202mcpsimp"><a name="p2202mcpsimp"></a><a name="p2202mcpsimp"></a>Bit9：VSDB非法；</p>
<p id="p2203mcpsimp"><a name="p2203mcpsimp"></a><a name="p2203mcpsimp"></a>Bit10：HFVSDB非法；</p>
<p id="p2204mcpsimp"><a name="p2204mcpsimp"></a><a name="p2204mcpsimp"></a>Bit11：SPEAKER DB非法；</p>
<p id="p2205mcpsimp"><a name="p2205mcpsimp"></a><a name="p2205mcpsimp"></a>Bit12：YCBCR420 VIC数量溢出；</p>
<p id="p2206mcpsimp"><a name="p2206mcpsimp"></a><a name="p2206mcpsimp"></a>Bit13：数据块长度非法。</p>
<p id="p2207mcpsimp"><a name="p2207mcpsimp"></a><a name="p2207mcpsimp"></a>Bit14~Bit31：保留。</p>
</td>
</tr>
<tr id="row2208mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2210mcpsimp"><a name="p2210mcpsimp"></a><a name="p2210mcpsimp"></a>raw get err cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2212mcpsimp"><a name="p2212mcpsimp"></a><a name="p2212mcpsimp"></a>原始数据获取失败次数。</p>
</td>
</tr>
<tr id="row2213mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2215mcpsimp"><a name="p2215mcpsimp"></a><a name="p2215mcpsimp"></a>raw length</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2217mcpsimp"><a name="p2217mcpsimp"></a><a name="p2217mcpsimp"></a>原始数据长度。</p>
</td>
</tr>
<tr id="row2218mcpsimp"><td class="cellrowborder" rowspan="20" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p2220mcpsimp"><a name="p2220mcpsimp"></a><a name="p2220mcpsimp"></a>basic cap</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.4.1.1 "><p id="p2222mcpsimp"><a name="p2222mcpsimp"></a><a name="p2222mcpsimp"></a>HDMI1.4 support</p>
</td>
<td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.4.1.2 "><p id="p2224mcpsimp"><a name="p2224mcpsimp"></a><a name="p2224mcpsimp"></a>Sink端是否支持HDMI1.4。</p>
<p id="p2225mcpsimp"><a name="p2225mcpsimp"></a><a name="p2225mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2226mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2228mcpsimp"><a name="p2228mcpsimp"></a><a name="p2228mcpsimp"></a>1st block version</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2230mcpsimp"><a name="p2230mcpsimp"></a><a name="p2230mcpsimp"></a>EDID第一块版本号。</p>
</td>
</tr>
<tr id="row2231mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2233mcpsimp"><a name="p2233mcpsimp"></a><a name="p2233mcpsimp"></a>HDMI2.0 support</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2235mcpsimp"><a name="p2235mcpsimp"></a><a name="p2235mcpsimp"></a>是否支持HDMI2.0</p>
<p id="p2236mcpsimp"><a name="p2236mcpsimp"></a><a name="p2236mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2237mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2239mcpsimp"><a name="p2239mcpsimp"></a><a name="p2239mcpsimp"></a>manufacturer name</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2241mcpsimp"><a name="p2241mcpsimp"></a><a name="p2241mcpsimp"></a>Sink端厂商名。</p>
</td>
</tr>
<tr id="row2242mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2244mcpsimp"><a name="p2244mcpsimp"></a><a name="p2244mcpsimp"></a>max tmds clock(MHz)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2246mcpsimp"><a name="p2246mcpsimp"></a><a name="p2246mcpsimp"></a>最大TMDS时钟。</p>
<p id="p2247mcpsimp"><a name="p2247mcpsimp"></a><a name="p2247mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2248mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2250mcpsimp"><a name="p2250mcpsimp"></a><a name="p2250mcpsimp"></a>product code</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2252mcpsimp"><a name="p2252mcpsimp"></a><a name="p2252mcpsimp"></a>Sink端产品号。</p>
</td>
</tr>
<tr id="row2253mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2255mcpsimp"><a name="p2255mcpsimp"></a><a name="p2255mcpsimp"></a>serial number</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2257mcpsimp"><a name="p2257mcpsimp"></a><a name="p2257mcpsimp"></a>Sink产品序列号。</p>
</td>
</tr>
<tr id="row2258mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2260mcpsimp"><a name="p2260mcpsimp"></a><a name="p2260mcpsimp"></a>week of manufacture</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2262mcpsimp"><a name="p2262mcpsimp"></a><a name="p2262mcpsimp"></a>Sink端产品制造周。</p>
<p id="p2263mcpsimp"><a name="p2263mcpsimp"></a><a name="p2263mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2264mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2266mcpsimp"><a name="p2266mcpsimp"></a><a name="p2266mcpsimp"></a>max disp width</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2268mcpsimp"><a name="p2268mcpsimp"></a><a name="p2268mcpsimp"></a>Sink最大显示的水平宽度（单位：厘米）。</p>
</td>
</tr>
<tr id="row2269mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2271mcpsimp"><a name="p2271mcpsimp"></a><a name="p2271mcpsimp"></a>max disp height</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2273mcpsimp"><a name="p2273mcpsimp"></a><a name="p2273mcpsimp"></a>Sink最大显示的垂直高度（单位：厘米）。</p>
</td>
</tr>
<tr id="row2274mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2276mcpsimp"><a name="p2276mcpsimp"></a><a name="p2276mcpsimp"></a>SCDC support</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2278mcpsimp"><a name="p2278mcpsimp"></a><a name="p2278mcpsimp"></a>Sink端是否支持SCDC。</p>
<p id="p2279mcpsimp"><a name="p2279mcpsimp"></a><a name="p2279mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2280mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2282mcpsimp"><a name="p2282mcpsimp"></a><a name="p2282mcpsimp"></a>year of manufacture</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2284mcpsimp"><a name="p2284mcpsimp"></a><a name="p2284mcpsimp"></a>Sink端产品制造年份。</p>
</td>
</tr>
<tr id="row2285mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2287mcpsimp"><a name="p2287mcpsimp"></a><a name="p2287mcpsimp"></a>dvi dual support</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2289mcpsimp"><a name="p2289mcpsimp"></a><a name="p2289mcpsimp"></a>Sink端是否支持DVI Dual。</p>
</td>
</tr>
<tr id="row2290mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2292mcpsimp"><a name="p2292mcpsimp"></a><a name="p2292mcpsimp"></a>cec addr is valid</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2294mcpsimp"><a name="p2294mcpsimp"></a><a name="p2294mcpsimp"></a>Sink端CEC地址是否有效。</p>
<p id="p2295mcpsimp"><a name="p2295mcpsimp"></a><a name="p2295mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2296mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2298mcpsimp"><a name="p2298mcpsimp"></a><a name="p2298mcpsimp"></a>AI support</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2300mcpsimp"><a name="p2300mcpsimp"></a><a name="p2300mcpsimp"></a>Sink端是否支持AI。</p>
<p id="p2301mcpsimp"><a name="p2301mcpsimp"></a><a name="p2301mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2302mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2304mcpsimp"><a name="p2304mcpsimp"></a><a name="p2304mcpsimp"></a>cec addr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2306mcpsimp"><a name="p2306mcpsimp"></a><a name="p2306mcpsimp"></a>CEC物理地址。</p>
</td>
</tr>
<tr id="row2307mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2309mcpsimp"><a name="p2309mcpsimp"></a><a name="p2309mcpsimp"></a>ext block cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2311mcpsimp"><a name="p2311mcpsimp"></a><a name="p2311mcpsimp"></a>EDID扩展块数量。</p>
</td>
</tr>
<tr id="row2312mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2314mcpsimp"><a name="p2314mcpsimp"></a><a name="p2314mcpsimp"></a>speaker support</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2316mcpsimp"><a name="p2316mcpsimp"></a><a name="p2316mcpsimp"></a>Sink端支持的扬声器。</p>
<p id="p2317mcpsimp"><a name="p2317mcpsimp"></a><a name="p2317mcpsimp"></a>取值：{ FL_FR, LFE, FC, RL_RR, RC, FLC_FRC, RLC_RRC, FLW_FRW, FLH_FRH, TC, FCH, UNKNOWN}</p>
<p id="p2318mcpsimp"><a name="p2318mcpsimp"></a><a name="p2318mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2319mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2321mcpsimp"><a name="p2321mcpsimp"></a><a name="p2321mcpsimp"></a>rgb quan selectable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2323mcpsimp"><a name="p2323mcpsimp"></a><a name="p2323mcpsimp"></a>Sink端是否支持RGB量化范围可选。</p>
<p id="p2324mcpsimp"><a name="p2324mcpsimp"></a><a name="p2324mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2325mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2327mcpsimp"><a name="p2327mcpsimp"></a><a name="p2327mcpsimp"></a>ycc quan selectable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2329mcpsimp"><a name="p2329mcpsimp"></a><a name="p2329mcpsimp"></a>Sink端是否支持YCbCr量化范围可选。</p>
<p id="p2330mcpsimp"><a name="p2330mcpsimp"></a><a name="p2330mcpsimp"></a>取值：{YES, NO}</p>
</td>
</tr>
<tr id="row2331mcpsimp"><td class="cellrowborder" rowspan="7" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p2333mcpsimp"><a name="p2333mcpsimp"></a><a name="p2333mcpsimp"></a>video cap</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.4.1.1 "><p id="p2335mcpsimp"><a name="p2335mcpsimp"></a><a name="p2335mcpsimp"></a>native format</p>
</td>
<td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.4.1.2 "><p id="p2337mcpsimp"><a name="p2337mcpsimp"></a><a name="p2337mcpsimp"></a>显示设备的最佳制式/VIC码。</p>
<p id="p2338mcpsimp"><a name="p2338mcpsimp"></a><a name="p2338mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2339mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2341mcpsimp"><a name="p2341mcpsimp"></a><a name="p2341mcpsimp"></a>color space</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2343mcpsimp"><a name="p2343mcpsimp"></a><a name="p2343mcpsimp"></a>显示设备支持的颜色空间。</p>
<p id="p2344mcpsimp"><a name="p2344mcpsimp"></a><a name="p2344mcpsimp"></a>取值：{ RGB444, YCbCr444, YCbCr422, YCbCr420}</p>
<p id="p2345mcpsimp"><a name="p2345mcpsimp"></a><a name="p2345mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2346mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2348mcpsimp"><a name="p2348mcpsimp"></a><a name="p2348mcpsimp"></a>deep color</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2350mcpsimp"><a name="p2350mcpsimp"></a><a name="p2350mcpsimp"></a>支持的深色位数。</p>
<p id="p2351mcpsimp"><a name="p2351mcpsimp"></a><a name="p2351mcpsimp"></a>取值：{RGB_30Bit, RGB_36Bit, RGB_48Bit, YCbCr444_SameRGB}</p>
<p id="p2352mcpsimp"><a name="p2352mcpsimp"></a><a name="p2352mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2353mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2355mcpsimp"><a name="p2355mcpsimp"></a><a name="p2355mcpsimp"></a>ycbcr420 deep color</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2357mcpsimp"><a name="p2357mcpsimp"></a><a name="p2357mcpsimp"></a>支持YCbCr420的深色位数。</p>
<p id="p2358mcpsimp"><a name="p2358mcpsimp"></a><a name="p2358mcpsimp"></a>取值：{ 30Bit, 36Bit, 48Bit}</p>
<p id="p2359mcpsimp"><a name="p2359mcpsimp"></a><a name="p2359mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2360mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2362mcpsimp"><a name="p2362mcpsimp"></a><a name="p2362mcpsimp"></a>ycbcr420[also]</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2364mcpsimp"><a name="p2364mcpsimp"></a><a name="p2364mcpsimp"></a>除RGB、YCbCr444、YCbCr422外，同时支持YCbCr420的VIC码。</p>
<p id="p2365mcpsimp"><a name="p2365mcpsimp"></a><a name="p2365mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2366mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2368mcpsimp"><a name="p2368mcpsimp"></a><a name="p2368mcpsimp"></a>ycbcr420[olny]</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2370mcpsimp"><a name="p2370mcpsimp"></a><a name="p2370mcpsimp"></a>只支持YCbCr420 的VIC码。</p>
<p id="p2371mcpsimp"><a name="p2371mcpsimp"></a><a name="p2371mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2372mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2374mcpsimp"><a name="p2374mcpsimp"></a><a name="p2374mcpsimp"></a>colorimetry</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2376mcpsimp"><a name="p2376mcpsimp"></a><a name="p2376mcpsimp"></a>Colorimetry色域。</p>
<p id="p2377mcpsimp"><a name="p2377mcpsimp"></a><a name="p2377mcpsimp"></a>取值：{ xvYCC601, xvYCC709, sYCC601 ,AdobleYCC601, AdobleRGB, BT2020cYCC, BT2020YCC, BT2020RGB}</p>
<p id="p2378mcpsimp"><a name="p2378mcpsimp"></a><a name="p2378mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2379mcpsimp"><td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p2381mcpsimp"><a name="p2381mcpsimp"></a><a name="p2381mcpsimp"></a>format cap</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.4.1.1 "><p id="p2383mcpsimp"><a name="p2383mcpsimp"></a><a name="p2383mcpsimp"></a>-</p>
</td>
<td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.4.1.2 "><p id="p2385mcpsimp"><a name="p2385mcpsimp"></a><a name="p2385mcpsimp"></a>Sink端支持哪些制式。</p>
<p id="p2386mcpsimp"><a name="p2386mcpsimp"></a><a name="p2386mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》、《VESA Display Monitor Timing Standard》</p>
</td>
</tr>
<tr id="row2387mcpsimp"><td class="cellrowborder" rowspan="6" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p2389mcpsimp"><a name="p2389mcpsimp"></a><a name="p2389mcpsimp"></a>audio cap</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.4.1.1 "><p id="p2391mcpsimp"><a name="p2391mcpsimp"></a><a name="p2391mcpsimp"></a>NO.x:</p>
</td>
<td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.4.1.2 "><p id="p2393mcpsimp"><a name="p2393mcpsimp"></a><a name="p2393mcpsimp"></a>编号x</p>
</td>
</tr>
<tr id="row2394mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2396mcpsimp"><a name="p2396mcpsimp"></a><a name="p2396mcpsimp"></a>code type</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2398mcpsimp"><a name="p2398mcpsimp"></a><a name="p2398mcpsimp"></a>编码类型。</p>
<p id="p2399mcpsimp"><a name="p2399mcpsimp"></a><a name="p2399mcpsimp"></a>取值：{STREAM, L-PCM, AC3, MPEG1, MP3, MPEG2, AAC_LC, DTS, ATRAC, OneBitAudio, EAC3, DTS-HD, MAT, DST, WMA_PRO, Reserved, UNKNOWN}</p>
<p id="p2400mcpsimp"><a name="p2400mcpsimp"></a><a name="p2400mcpsimp"></a>注：详见《EIA-CEA-861-D（F）》。</p>
</td>
</tr>
<tr id="row2401mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2403mcpsimp"><a name="p2403mcpsimp"></a><a name="p2403mcpsimp"></a>max channel num</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2405mcpsimp"><a name="p2405mcpsimp"></a><a name="p2405mcpsimp"></a>最大通道数。</p>
</td>
</tr>
<tr id="row2406mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2408mcpsimp"><a name="p2408mcpsimp"></a><a name="p2408mcpsimp"></a>max bit rate(KHz)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2410mcpsimp"><a name="p2410mcpsimp"></a><a name="p2410mcpsimp"></a>最大bit速率。</p>
</td>
</tr>
<tr id="row2411mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2413mcpsimp"><a name="p2413mcpsimp"></a><a name="p2413mcpsimp"></a>bit depth</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2415mcpsimp"><a name="p2415mcpsimp"></a><a name="p2415mcpsimp"></a>支持的Bit深度。</p>
<p id="p2416mcpsimp"><a name="p2416mcpsimp"></a><a name="p2416mcpsimp"></a>取值：{0, 8, 16, 18, 20, 24, 32, N/A}</p>
</td>
</tr>
<tr id="row2417mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2419mcpsimp"><a name="p2419mcpsimp"></a><a name="p2419mcpsimp"></a>sample rate(Hz)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2421mcpsimp"><a name="p2421mcpsimp"></a><a name="p2421mcpsimp"></a>采样速率。</p>
<p id="p2422mcpsimp"><a name="p2422mcpsimp"></a><a name="p2422mcpsimp"></a>取值：{0, 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 88200, 96000, 176400, 192000, 768000}</p>
</td>
</tr>
<tr id="row2423mcpsimp"><td class="cellrowborder" rowspan="18" valign="top" width="15%" headers="mcps1.1.4.1.1 "><p id="p2425mcpsimp"><a name="p2425mcpsimp"></a><a name="p2425mcpsimp"></a>detail timing</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.4.1.1 "><p id="p2427mcpsimp"><a name="p2427mcpsimp"></a><a name="p2427mcpsimp"></a>NO.</p>
</td>
<td class="cellrowborder" valign="top" width="60%" headers="mcps1.1.4.1.2 "><p id="p2429mcpsimp"><a name="p2429mcpsimp"></a><a name="p2429mcpsimp"></a>编号</p>
</td>
</tr>
<tr id="row2430mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2432mcpsimp"><a name="p2432mcpsimp"></a><a name="p2432mcpsimp"></a>hact</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2434mcpsimp"><a name="p2434mcpsimp"></a><a name="p2434mcpsimp"></a>水平有效像素</p>
</td>
</tr>
<tr id="row2435mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2437mcpsimp"><a name="p2437mcpsimp"></a><a name="p2437mcpsimp"></a>vact</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2439mcpsimp"><a name="p2439mcpsimp"></a><a name="p2439mcpsimp"></a>垂直有效像素</p>
</td>
</tr>
<tr id="row2440mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2442mcpsimp"><a name="p2442mcpsimp"></a><a name="p2442mcpsimp"></a>P/I</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2444mcpsimp"><a name="p2444mcpsimp"></a><a name="p2444mcpsimp"></a>逐/隔行</p>
</td>
</tr>
<tr id="row2445mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2447mcpsimp"><a name="p2447mcpsimp"></a><a name="p2447mcpsimp"></a>pclk</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2449mcpsimp"><a name="p2449mcpsimp"></a><a name="p2449mcpsimp"></a>像素时钟</p>
</td>
</tr>
<tr id="row2450mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2452mcpsimp"><a name="p2452mcpsimp"></a><a name="p2452mcpsimp"></a>aspw</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2454mcpsimp"><a name="p2454mcpsimp"></a><a name="p2454mcpsimp"></a>宽高比-宽</p>
</td>
</tr>
<tr id="row2455mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2457mcpsimp"><a name="p2457mcpsimp"></a><a name="p2457mcpsimp"></a>asph</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2459mcpsimp"><a name="p2459mcpsimp"></a><a name="p2459mcpsimp"></a>宽高比-高</p>
</td>
</tr>
<tr id="row2460mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2462mcpsimp"><a name="p2462mcpsimp"></a><a name="p2462mcpsimp"></a>hfb</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2464mcpsimp"><a name="p2464mcpsimp"></a><a name="p2464mcpsimp"></a>水平消隐前肩</p>
</td>
</tr>
<tr id="row2465mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2467mcpsimp"><a name="p2467mcpsimp"></a><a name="p2467mcpsimp"></a>hpw</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2469mcpsimp"><a name="p2469mcpsimp"></a><a name="p2469mcpsimp"></a>水平脉冲宽度</p>
</td>
</tr>
<tr id="row2470mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2472mcpsimp"><a name="p2472mcpsimp"></a><a name="p2472mcpsimp"></a>hbb</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2474mcpsimp"><a name="p2474mcpsimp"></a><a name="p2474mcpsimp"></a>水平消隐后肩</p>
</td>
</tr>
<tr id="row2475mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2477mcpsimp"><a name="p2477mcpsimp"></a><a name="p2477mcpsimp"></a>vfb</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2479mcpsimp"><a name="p2479mcpsimp"></a><a name="p2479mcpsimp"></a>垂直消隐前肩</p>
</td>
</tr>
<tr id="row2480mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2482mcpsimp"><a name="p2482mcpsimp"></a><a name="p2482mcpsimp"></a>vpw</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2484mcpsimp"><a name="p2484mcpsimp"></a><a name="p2484mcpsimp"></a>垂直同步信号的宽度</p>
</td>
</tr>
<tr id="row2485mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2487mcpsimp"><a name="p2487mcpsimp"></a><a name="p2487mcpsimp"></a>vbb</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2489mcpsimp"><a name="p2489mcpsimp"></a><a name="p2489mcpsimp"></a>垂直消隐后肩</p>
</td>
</tr>
<tr id="row2490mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2492mcpsimp"><a name="p2492mcpsimp"></a><a name="p2492mcpsimp"></a>img_w</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2494mcpsimp"><a name="p2494mcpsimp"></a><a name="p2494mcpsimp"></a>图像宽</p>
</td>
</tr>
<tr id="row2495mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2497mcpsimp"><a name="p2497mcpsimp"></a><a name="p2497mcpsimp"></a>img_h</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2499mcpsimp"><a name="p2499mcpsimp"></a><a name="p2499mcpsimp"></a>图像高</p>
</td>
</tr>
<tr id="row2500mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2502mcpsimp"><a name="p2502mcpsimp"></a><a name="p2502mcpsimp"></a>ihs</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2504mcpsimp"><a name="p2504mcpsimp"></a><a name="p2504mcpsimp"></a>水平有效信号的极性，配置0为高有效，配置1为低有效</p>
</td>
</tr>
<tr id="row2505mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2507mcpsimp"><a name="p2507mcpsimp"></a><a name="p2507mcpsimp"></a>ivs</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2509mcpsimp"><a name="p2509mcpsimp"></a><a name="p2509mcpsimp"></a>垂直有效信号的极性，配置0为高有效，配置1为低有效</p>
</td>
</tr>
<tr id="row2510mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2512mcpsimp"><a name="p2512mcpsimp"></a><a name="p2512mcpsimp"></a>idv</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2514mcpsimp"><a name="p2514mcpsimp"></a><a name="p2514mcpsimp"></a>数据有效信号的极性。配置0为高有效，配置1为低有效</p>
</td>
</tr>
</tbody>
</table>

注：N/A/NONE/UNKNOWN为未知/无效；reserved为保留/未知；ERROR为错误。

# Debug调试<a name="ZH-CN_TOPIC_0000002441654017"></a>

当出现问题时，可通过Debug调试辅助完成一些实验，帮助快速定位问题原因。

>![](public_sys-resources/icon-notice.gif) **须知：** 
>-   Debug命令属于高级调试接口，无特殊原因请不要使用。
>-   正常情况下请勿使用，否则可能导致显示异常或无显示。
>-   SS626V100在使用Debug调试时，写入路径/proc/umap/argv中的argv取值为\[hdmi0, hdmi1\]，两个取值代表着对不同HDMI口进行调试。

【HDMI Debug调试命令】

-   help \(h\)

    命令：echo help \> /proc/umap/hdmi0

    功能：查看当前支持的debug命令、命令编号以及简化命令。

    参数说明：无

    说明：每条命令有对应的简化命令以及编号，使用时可选择使用命令全称、简化命令以及命令对应的编号。

-   thread \(t\)

    命令：echo thread argv1 \> /proc/umap/hdmi0

    功能：内核线程启动与停止。

    参数说明：argv1取值\[0, 1\]，0表示停止运行，1表示启动运行。

-   oe \(o\)

    命令：echo oe argv1 \> /proc/umap/hdmi0

    功能：HDMI OE打开与关闭。

    参数说明：argv1取值\[0, 1\]，0表示关闭输出，1表示打开输出。

-   event \(e\)

    命令：echo event argv1 \> /proc/umap/hdmi0

    功能：软件模拟产生事件。

    参数说明如[表1](#_Ref511133664)。

**表 1**  event命令参数说明

<a name="_Ref511133664"></a>
<table><thead align="left"><tr id="row272mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.2.3.1.1"><p id="p274mcpsimp"><a name="p274mcpsimp"></a><a name="p274mcpsimp"></a>argv1</p>
</th>
<th class="cellrowborder" valign="top" width="78%" id="mcps1.2.3.1.2"><p id="p276mcpsimp"><a name="p276mcpsimp"></a><a name="p276mcpsimp"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row278mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p280mcpsimp"><a name="p280mcpsimp"></a><a name="p280mcpsimp"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p282mcpsimp"><a name="p282mcpsimp"></a><a name="p282mcpsimp"></a>HPD，产生HotPlug事件</p>
</td>
</tr>
<tr id="row283mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p285mcpsimp"><a name="p285mcpsimp"></a><a name="p285mcpsimp"></a>17</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p287mcpsimp"><a name="p287mcpsimp"></a><a name="p287mcpsimp"></a>HOTUNPLUG，产生UN-HotPlug事件</p>
</td>
</tr>
<tr id="row288mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p290mcpsimp"><a name="p290mcpsimp"></a><a name="p290mcpsimp"></a>18</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p292mcpsimp"><a name="p292mcpsimp"></a><a name="p292mcpsimp"></a>EDID_FAIL，产生EDID失败事件</p>
</td>
</tr>
<tr id="row293mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p295mcpsimp"><a name="p295mcpsimp"></a><a name="p295mcpsimp"></a>21</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p297mcpsimp"><a name="p297mcpsimp"></a><a name="p297mcpsimp"></a>RSEN_CONNECT，产生connect事件</p>
</td>
</tr>
<tr id="row298mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p300mcpsimp"><a name="p300mcpsimp"></a><a name="p300mcpsimp"></a>22</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p302mcpsimp"><a name="p302mcpsimp"></a><a name="p302mcpsimp"></a>RSEN_DISCONNECT，产生disconnect事件</p>
</td>
</tr>
<tr id="row303mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p305mcpsimp"><a name="p305mcpsimp"></a><a name="p305mcpsimp"></a>25</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p307mcpsimp"><a name="p307mcpsimp"></a><a name="p307mcpsimp"></a>SCRAMBLE_FAIL，产生加扰失败注销事件</p>
</td>
</tr>
<tr id="row308mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p310mcpsimp"><a name="p310mcpsimp"></a><a name="p310mcpsimp"></a>26</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p312mcpsimp"><a name="p312mcpsimp"></a><a name="p312mcpsimp"></a>SCRAMBLE_SUCCESS，产生加扰成功注销事件</p>
</td>
</tr>
<tr id="row313mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p315mcpsimp"><a name="p315mcpsimp"></a><a name="p315mcpsimp"></a>27</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p317mcpsimp"><a name="p317mcpsimp"></a><a name="p317mcpsimp"></a>ZERO_DRM_TIMEOUT，产生全0信息帧发送超时事件</p>
</td>
</tr>
<tr id="row318mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.2.3.1.1 "><p id="p320mcpsimp"><a name="p320mcpsimp"></a><a name="p320mcpsimp"></a>28</p>
</td>
<td class="cellrowborder" valign="top" width="78%" headers="mcps1.2.3.1.2 "><p id="p322mcpsimp"><a name="p322mcpsimp"></a><a name="p322mcpsimp"></a>HDR_SWITCH_TIMEOUT，产生HDR模式切换事件</p>
</td>
</tr>
</tbody>
</table>

-   edid \(ed\)

    命令：echo edid argv1 argv2 \> /proc/umap/hdmi0

    功能：EDID调试。

    参数说明如[表2](#_Ref511133677)所示。

**表 2**  edid命令参数说明

<a name="_Ref511133677"></a>
<table><thead align="left"><tr id="row335mcpsimp"><th class="cellrowborder" valign="top" width="11%" id="mcps1.2.4.1.1"><p id="p337mcpsimp"><a name="p337mcpsimp"></a><a name="p337mcpsimp"></a>argv1</p>
</th>
<th class="cellrowborder" valign="top" width="23%" id="mcps1.2.4.1.2"><p id="p339mcpsimp"><a name="p339mcpsimp"></a><a name="p339mcpsimp"></a>argv2</p>
</th>
<th class="cellrowborder" valign="top" width="66%" id="mcps1.2.4.1.3"><p id="p341mcpsimp"><a name="p341mcpsimp"></a><a name="p341mcpsimp"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row342mcpsimp"><td class="cellrowborder" valign="top" width="11%" headers="mcps1.2.4.1.1 "><p id="p344mcpsimp"><a name="p344mcpsimp"></a><a name="p344mcpsimp"></a>0 (s)</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.2.4.1.2 "><p id="p346mcpsimp"><a name="p346mcpsimp"></a><a name="p346mcpsimp"></a>任意值</p>
</td>
<td class="cellrowborder" valign="top" width="66%" headers="mcps1.2.4.1.3 "><p id="p348mcpsimp"><a name="p348mcpsimp"></a><a name="p348mcpsimp"></a>从sink端读取EDID</p>
</td>
</tr>
<tr id="row349mcpsimp"><td class="cellrowborder" valign="top" width="11%" headers="mcps1.2.4.1.1 "><p id="p351mcpsimp"><a name="p351mcpsimp"></a><a name="p351mcpsimp"></a>1 (f)</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.2.4.1.2 "><p id="p353mcpsimp"><a name="p353mcpsimp"></a><a name="p353mcpsimp"></a>EDID文件路径</p>
</td>
<td class="cellrowborder" valign="top" width="66%" headers="mcps1.2.4.1.3 "><p id="p355mcpsimp"><a name="p355mcpsimp"></a><a name="p355mcpsimp"></a>EDID原始数据文件路径，软件加载EDID数据</p>
</td>
</tr>
</tbody>
</table>

-   avmute \(a\)

    命令：echo avmute argv1 \> /proc/umap/hdmi0

    功能：AVMUTE开关。

    参数说明：argv1取值\[0, 1\]，0表示发clear AVMUTE包，1表示发AVMUTE包。

-   cbar \(c\)

    命令：echo cbar argv1 \> /proc/umap/hdmi0

    功能：colorbar开关。

    参数说明：argv1取值\[0, 1\]，0表示关闭colorbar，1表示打开colorbar。

-   scdc \(sc\)

    命令：echo scdc argv1 argv2 argv3 \> /proc/umap/hdmi0

    功能：SCDC调试。

    >![](public_sys-resources/icon-notice.gif) **须知：** 
    >SS625V100、SS524V100、SS522V101均不支持SCDC。

参数说明：

参数说明如[表3](#_Ref18491063)所示。

**表 3**  scdc命令参数说明

<a name="_Ref18491063"></a>
<table><thead align="left"><tr id="row379mcpsimp"><th class="cellrowborder" valign="top" width="9%" id="mcps1.2.5.1.1"><p id="p381mcpsimp"><a name="p381mcpsimp"></a><a name="p381mcpsimp"></a>argv1</p>
</th>
<th class="cellrowborder" valign="top" width="14.000000000000002%" id="mcps1.2.5.1.2"><p id="p383mcpsimp"><a name="p383mcpsimp"></a><a name="p383mcpsimp"></a>argv2</p>
</th>
<th class="cellrowborder" valign="top" width="13%" id="mcps1.2.5.1.3"><p id="p385mcpsimp"><a name="p385mcpsimp"></a><a name="p385mcpsimp"></a>argv3</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.2.5.1.4"><p id="p387mcpsimp"><a name="p387mcpsimp"></a><a name="p387mcpsimp"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row388mcpsimp"><td class="cellrowborder" valign="top" width="9%" headers="mcps1.2.5.1.1 "><p id="p390mcpsimp"><a name="p390mcpsimp"></a><a name="p390mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.2.5.1.2 "><p id="p392mcpsimp"><a name="p392mcpsimp"></a><a name="p392mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.2.5.1.3 "><p id="p394mcpsimp"><a name="p394mcpsimp"></a><a name="p394mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.2.5.1.4 "><p id="p396mcpsimp"><a name="p396mcpsimp"></a><a name="p396mcpsimp"></a>关闭SCDC</p>
</td>
</tr>
<tr id="row397mcpsimp"><td class="cellrowborder" valign="top" width="9%" headers="mcps1.2.5.1.1 "><p id="p399mcpsimp"><a name="p399mcpsimp"></a><a name="p399mcpsimp"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.2.5.1.2 "><p id="p401mcpsimp"><a name="p401mcpsimp"></a><a name="p401mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.2.5.1.3 "><p id="p403mcpsimp"><a name="p403mcpsimp"></a><a name="p403mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.2.5.1.4 "><p id="p405mcpsimp"><a name="p405mcpsimp"></a><a name="p405mcpsimp"></a>打开SCDC</p>
</td>
</tr>
<tr id="row406mcpsimp"><td class="cellrowborder" valign="top" width="9%" headers="mcps1.2.5.1.1 "><p id="p408mcpsimp"><a name="p408mcpsimp"></a><a name="p408mcpsimp"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.2.5.1.2 "><p id="p410mcpsimp"><a name="p410mcpsimp"></a><a name="p410mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.2.5.1.3 "><p id="p412mcpsimp"><a name="p412mcpsimp"></a><a name="p412mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.2.5.1.4 "><p id="p414mcpsimp"><a name="p414mcpsimp"></a><a name="p414mcpsimp"></a>获取SCDC状态</p>
</td>
</tr>
</tbody>
</table>

-   ddc \(dd\)

    命令：echo ddc argv1 \> /proc/umap/hdmi0

    功能：设置DDC速率。

    参数说明：argv1取值\[1, 300\]，表示DDC速率为1-300KHz。

    注意事项：此速率并非精确值，实际速率可能与此值有偏差。

-   hdmimode \(m\)

    命令：echo hdmimode argv1 \> /proc/umap/hdmi0

    功能：设置HDMI输出模式。

    参数说明：argv1取值\[0, 2\]，0表示DVI模式，1表示HDMI1.4模式，2表示HDMI2.0模式。

-   outclrspace \(oc\)

    命令：echo outclrspace argv1 \> /proc/umap/hdmi0

    功能：设置输出的Color Space。

    参数说明：argv1取值\[0, 3\]，0表示RGB，1表示YCbCr422，2表示YCbCr444，3表示YCbCr420。

-   deepclr \(d\)

    命令：echo deepclr argv1 \> /proc/umap/hdmi0

    功能：设置输出的Deep Color。

    参数说明：argv1取值\[0, 2\]，0表示8bit，1表示10bit，2表示12bit。

-   dither \(di\)

    命令：echo dither argv1 \> /proc/umap/hdmi0

    功能：设置dither开关。

    参数说明如[表4](#_Ref511133753)。

**表 4**  dither命令参数说明

<a name="_Ref511133753"></a>
<table><thead align="left"><tr id="row443mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.3.1.1"><p id="p445mcpsimp"><a name="p445mcpsimp"></a><a name="p445mcpsimp"></a>argv1</p>
</th>
<th class="cellrowborder" valign="top" width="80%" id="mcps1.2.3.1.2"><p id="p447mcpsimp"><a name="p447mcpsimp"></a><a name="p447mcpsimp"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row448mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.1 "><p id="p450mcpsimp"><a name="p450mcpsimp"></a><a name="p450mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="80%" headers="mcps1.2.3.1.2 "><p id="p452mcpsimp"><a name="p452mcpsimp"></a><a name="p452mcpsimp"></a>关闭dither</p>
</td>
</tr>
<tr id="row453mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.1 "><p id="p455mcpsimp"><a name="p455mcpsimp"></a><a name="p455mcpsimp"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="80%" headers="mcps1.2.3.1.2 "><p id="p457mcpsimp"><a name="p457mcpsimp"></a><a name="p457mcpsimp"></a>打开dither，10bit-&gt;8bit模式</p>
</td>
</tr>
<tr id="row458mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.1 "><p id="p460mcpsimp"><a name="p460mcpsimp"></a><a name="p460mcpsimp"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="80%" headers="mcps1.2.3.1.2 "><p id="p462mcpsimp"><a name="p462mcpsimp"></a><a name="p462mcpsimp"></a>打开dither，12bit-&gt;8bit模式</p>
</td>
</tr>
<tr id="row463mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.1 "><p id="p465mcpsimp"><a name="p465mcpsimp"></a><a name="p465mcpsimp"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="80%" headers="mcps1.2.3.1.2 "><p id="p467mcpsimp"><a name="p467mcpsimp"></a><a name="p467mcpsimp"></a>打开dither，12bit-&gt;10bit模式</p>
</td>
</tr>
</tbody>
</table>

-   aut \(au\)

    命令：echo aut argv1 \> /proc/umap/hdmi0

    功能：认证模式开关。

    参数说明：argv1取值\[0, 1\]，0表示关闭认证模式，1表示打开认证模式。

-   cmd

    命令：echo cmd argv1 argv2\(optional\) \> /proc/umap/hdmi0

    功能：其他调试命令。

    参数说明如[表5](#_Ref511133818)。

**表 5**  cmd命令参数说明

<a name="_Ref511133818"></a>
<table><thead align="left"><tr id="row484mcpsimp"><th class="cellrowborder" valign="top" width="19.8%" id="mcps1.2.4.1.1"><p id="p486mcpsimp"><a name="p486mcpsimp"></a><a name="p486mcpsimp"></a>argv 1</p>
</th>
<th class="cellrowborder" valign="top" width="19.8%" id="mcps1.2.4.1.2"><p id="p488mcpsimp"><a name="p488mcpsimp"></a><a name="p488mcpsimp"></a>argv 2</p>
</th>
<th class="cellrowborder" valign="top" width="60.4%" id="mcps1.2.4.1.3"><p id="p490mcpsimp"><a name="p490mcpsimp"></a><a name="p490mcpsimp"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row491mcpsimp"><td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.1 "><p id="p493mcpsimp"><a name="p493mcpsimp"></a><a name="p493mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.2 "><p id="p495mcpsimp"><a name="p495mcpsimp"></a><a name="p495mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="60.4%" headers="mcps1.2.4.1.3 "><p id="p497mcpsimp"><a name="p497mcpsimp"></a><a name="p497mcpsimp"></a>关闭HDMI输出</p>
</td>
</tr>
<tr id="row498mcpsimp"><td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.1 "><p id="p500mcpsimp"><a name="p500mcpsimp"></a><a name="p500mcpsimp"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.2 "><p id="p502mcpsimp"><a name="p502mcpsimp"></a><a name="p502mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="60.4%" headers="mcps1.2.4.1.3 "><p id="p504mcpsimp"><a name="p504mcpsimp"></a><a name="p504mcpsimp"></a>开启HDMI输出</p>
</td>
</tr>
<tr id="row505mcpsimp"><td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.1 "><p id="p507mcpsimp"><a name="p507mcpsimp"></a><a name="p507mcpsimp"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.2 "><p id="p509mcpsimp"><a name="p509mcpsimp"></a><a name="p509mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="60.4%" headers="mcps1.2.4.1.3 "><p id="p511mcpsimp"><a name="p511mcpsimp"></a><a name="p511mcpsimp"></a>发送黑帧</p>
</td>
</tr>
<tr id="row512mcpsimp"><td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.1 "><p id="p514mcpsimp"><a name="p514mcpsimp"></a><a name="p514mcpsimp"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.2 "><p id="p516mcpsimp"><a name="p516mcpsimp"></a><a name="p516mcpsimp"></a>x</p>
</td>
<td class="cellrowborder" valign="top" width="60.4%" headers="mcps1.2.4.1.3 "><p id="p518mcpsimp"><a name="p518mcpsimp"></a><a name="p518mcpsimp"></a>关闭黑帧</p>
</td>
</tr>
<tr id="row519mcpsimp"><td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.1 "><p id="p521mcpsimp"><a name="p521mcpsimp"></a><a name="p521mcpsimp"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.2 "><p id="p523mcpsimp"><a name="p523mcpsimp"></a><a name="p523mcpsimp"></a>≥ 0</p>
</td>
<td class="cellrowborder" valign="top" width="60.4%" headers="mcps1.2.4.1.3 "><p id="p525mcpsimp"><a name="p525mcpsimp"></a><a name="p525mcpsimp"></a>0：选择模拟指标参数0</p>
<p id="p526mcpsimp"><a name="p526mcpsimp"></a><a name="p526mcpsimp"></a>1：选择模拟指标参数1</p>
</td>
</tr>
</tbody>
</table>

注：x表示无效参数。

-   compat\(cmp\)

    命令：echo cmp argv1 argv2 \> /proc/umap/hdmi0

    功能：兼容性调试命令。

    参数说明如[表6](#_Ref40891957)。

**表 6**  compat命令参数说明

<a name="_Ref40891957"></a>
<table><thead align="left"><tr id="row540mcpsimp"><th class="cellrowborder" valign="top" width="19.8%" id="mcps1.2.4.1.1"><p id="p542mcpsimp"><a name="p542mcpsimp"></a><a name="p542mcpsimp"></a>argv 1</p>
</th>
<th class="cellrowborder" valign="top" width="19.8%" id="mcps1.2.4.1.2"><p id="p544mcpsimp"><a name="p544mcpsimp"></a><a name="p544mcpsimp"></a>argv 2</p>
</th>
<th class="cellrowborder" valign="top" width="60.4%" id="mcps1.2.4.1.3"><p id="p546mcpsimp"><a name="p546mcpsimp"></a><a name="p546mcpsimp"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row548mcpsimp"><td class="cellrowborder" rowspan="2" valign="top" width="19.8%" headers="mcps1.2.4.1.1 "><p id="p550mcpsimp"><a name="p550mcpsimp"></a><a name="p550mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.2 "><p id="p552mcpsimp"><a name="p552mcpsimp"></a><a name="p552mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="60.4%" headers="mcps1.2.4.1.3 "><p id="p554mcpsimp"><a name="p554mcpsimp"></a><a name="p554mcpsimp"></a>关闭不发avmute包的调试功能。</p>
</td>
</tr>
<tr id="row555mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p557mcpsimp"><a name="p557mcpsimp"></a><a name="p557mcpsimp"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p559mcpsimp"><a name="p559mcpsimp"></a><a name="p559mcpsimp"></a>打开不发avmute包的调试功能。</p>
</td>
</tr>
<tr id="row560mcpsimp"><td class="cellrowborder" rowspan="2" valign="top" width="19.8%" headers="mcps1.2.4.1.1 "><p id="p562mcpsimp"><a name="p562mcpsimp"></a><a name="p562mcpsimp"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="19.8%" headers="mcps1.2.4.1.2 "><p id="p564mcpsimp"><a name="p564mcpsimp"></a><a name="p564mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="60.4%" headers="mcps1.2.4.1.3 "><p id="p566mcpsimp"><a name="p566mcpsimp"></a><a name="p566mcpsimp"></a>关闭使用gen5包来发送avi信息帧的调试功能。</p>
</td>
</tr>
<tr id="row567mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p569mcpsimp"><a name="p569mcpsimp"></a><a name="p569mcpsimp"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p571mcpsimp"><a name="p571mcpsimp"></a><a name="p571mcpsimp"></a>打开使用gen5包来发送avi信息帧的调试功能。</p>
</td>
</tr>
</tbody>
</table>

-   mutedelay\(md\)

    命令：echo mutedelay argv1  \> /proc/umap/hdmi0

    功能：设置mute delay的时长。

    参数说明：argv1取值\[0, 100000\]，单位ms。

-   fmtdelay\(fd\)

    命令：echo fmtdelay argv1  \> /proc/umap/hdmi0

    功能：设置fmt delay的时长。

    参数说明：argv1取值\[0, 100000\]，单位ms。

# 缩略语<a name="ZH-CN_TOPIC_0000002441654045"></a>

<a name="table5569mcpsimp"></a>
<table><thead align="left"><tr id="row5574mcpsimp"><th class="cellrowborder" valign="top" width="32%" id="mcps1.1.3.1.1"><p id="p5576mcpsimp"><a name="p5576mcpsimp"></a><a name="p5576mcpsimp"></a>缩略语</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.3.1.2"><p id="p5578mcpsimp"><a name="p5578mcpsimp"></a><a name="p5578mcpsimp"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row5580mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5582mcpsimp"><a name="p5582mcpsimp"></a><a name="p5582mcpsimp"></a>HDMI</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5584mcpsimp"><a name="p5584mcpsimp"></a><a name="p5584mcpsimp"></a>High-Definition Multimedia Interface</p>
</td>
</tr>
<tr id="row5585mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5587mcpsimp"><a name="p5587mcpsimp"></a><a name="p5587mcpsimp"></a>TMDS</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5589mcpsimp"><a name="p5589mcpsimp"></a><a name="p5589mcpsimp"></a>Transition Minimized Differential Signaling</p>
</td>
</tr>
<tr id="row5590mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5592mcpsimp"><a name="p5592mcpsimp"></a><a name="p5592mcpsimp"></a>CTS (Audio)</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5594mcpsimp"><a name="p5594mcpsimp"></a><a name="p5594mcpsimp"></a>Cycle Time Stamp</p>
</td>
</tr>
<tr id="row5595mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5597mcpsimp"><a name="p5597mcpsimp"></a><a name="p5597mcpsimp"></a>EDID</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5599mcpsimp"><a name="p5599mcpsimp"></a><a name="p5599mcpsimp"></a>Extended Display Identification Data</p>
</td>
</tr>
<tr id="row5600mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5602mcpsimp"><a name="p5602mcpsimp"></a><a name="p5602mcpsimp"></a>OSD</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5604mcpsimp"><a name="p5604mcpsimp"></a><a name="p5604mcpsimp"></a>On Screen Display</p>
</td>
</tr>
<tr id="row5605mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5607mcpsimp"><a name="p5607mcpsimp"></a><a name="p5607mcpsimp"></a>SDR</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5609mcpsimp"><a name="p5609mcpsimp"></a><a name="p5609mcpsimp"></a>Standard Dynamic Range</p>
</td>
</tr>
<tr id="row5610mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5612mcpsimp"><a name="p5612mcpsimp"></a><a name="p5612mcpsimp"></a>HDR</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5614mcpsimp"><a name="p5614mcpsimp"></a><a name="p5614mcpsimp"></a>High Dynamic Range</p>
</td>
</tr>
<tr id="row5615mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5617mcpsimp"><a name="p5617mcpsimp"></a><a name="p5617mcpsimp"></a>DRM</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5619mcpsimp"><a name="p5619mcpsimp"></a><a name="p5619mcpsimp"></a>Dynamic Range and Mastering</p>
</td>
</tr>
<tr id="row5620mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5622mcpsimp"><a name="p5622mcpsimp"></a><a name="p5622mcpsimp"></a>HLG</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5624mcpsimp"><a name="p5624mcpsimp"></a><a name="p5624mcpsimp"></a>Hybrid Log Gamma</p>
</td>
</tr>
<tr id="row5625mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5627mcpsimp"><a name="p5627mcpsimp"></a><a name="p5627mcpsimp"></a>DDC</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5629mcpsimp"><a name="p5629mcpsimp"></a><a name="p5629mcpsimp"></a>Display Data Channel</p>
</td>
</tr>
<tr id="row5630mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5632mcpsimp"><a name="p5632mcpsimp"></a><a name="p5632mcpsimp"></a>CTS</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5634mcpsimp"><a name="p5634mcpsimp"></a><a name="p5634mcpsimp"></a>Compliance Test Specification</p>
</td>
</tr>
<tr id="row5635mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5637mcpsimp"><a name="p5637mcpsimp"></a><a name="p5637mcpsimp"></a>AVI</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5639mcpsimp"><a name="p5639mcpsimp"></a><a name="p5639mcpsimp"></a>Auxiliary Video Information</p>
</td>
</tr>
<tr id="row5640mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5642mcpsimp"><a name="p5642mcpsimp"></a><a name="p5642mcpsimp"></a>DSC</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5644mcpsimp"><a name="p5644mcpsimp"></a><a name="p5644mcpsimp"></a>Display Stream Compression</p>
</td>
</tr>
<tr id="row5645mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5647mcpsimp"><a name="p5647mcpsimp"></a><a name="p5647mcpsimp"></a>OE</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5649mcpsimp"><a name="p5649mcpsimp"></a><a name="p5649mcpsimp"></a>Output enable</p>
</td>
</tr>
<tr id="row5650mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5652mcpsimp"><a name="p5652mcpsimp"></a><a name="p5652mcpsimp"></a>Gbps</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5654mcpsimp"><a name="p5654mcpsimp"></a><a name="p5654mcpsimp"></a>Giga Bit Per Second</p>
</td>
</tr>
<tr id="row5655mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5657mcpsimp"><a name="p5657mcpsimp"></a><a name="p5657mcpsimp"></a>PCB</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5659mcpsimp"><a name="p5659mcpsimp"></a><a name="p5659mcpsimp"></a>Printed Circuit Board</p>
</td>
</tr>
<tr id="row5661mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p5663mcpsimp"><a name="p5663mcpsimp"></a><a name="p5663mcpsimp"></a>EOTF</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p5665mcpsimp"><a name="p5665mcpsimp"></a><a name="p5665mcpsimp"></a>Electro-Optical Transfer Function</p>
</td>
</tr>
</tbody>
</table>

