# 前言<a name="ZH-CN_TOPIC_0000002441853489"></a>

**概述<a name="section122282764510"></a>**

本文档为使用IVS进行_识别_分析方案开发的程序员而写，目的是供您在开发过程中查阅IVS支持的各种参考信息，包括API、头文件、错误码等。

**产品版本<a name="section12318719459"></a>**

与本文档相对应的产品版本如下。

<a name="table1324247124515"></a>
<table><thead align="left"><tr id="row8283167154518"><th class="cellrowborder" valign="top" width="31.759999999999998%" id="mcps1.1.3.1.1"><p id="p1728315720455"><a name="p1728315720455"></a><a name="p1728315720455"></a>产品名称</p>
</th>
<th class="cellrowborder" valign="top" width="68.24%" id="mcps1.1.3.1.2"><p id="p92831478451"><a name="p92831478451"></a><a name="p92831478451"></a>产品版本</p>
</th>
</tr>
</thead>
<tbody><tr id="row628310712459"><td class="cellrowborder" valign="top" width="31.759999999999998%" headers="mcps1.1.3.1.1 "><p id="p628411716459"><a name="p628411716459"></a><a name="p628411716459"></a>SS928</p>
</td>
<td class="cellrowborder" valign="top" width="68.24%" headers="mcps1.1.3.1.2 "><p id="p152841577456"><a name="p152841577456"></a><a name="p152841577456"></a>V100</p>
</td>
</tr>
<tr id="row09316491432"><td class="cellrowborder" valign="top" width="31.759999999999998%" headers="mcps1.1.3.1.1 "><p id="p8622349102117"><a name="p8622349102117"></a><a name="p8622349102117"></a>SS927</p>
</td>
<td class="cellrowborder" valign="top" width="68.24%" headers="mcps1.1.3.1.2 "><p id="p9185184311112"><a name="p9185184311112"></a><a name="p9185184311112"></a>V100</p>
</td>
</tr>
</tbody>
</table>

**读者对象<a name="section1924037154519"></a>**

本文档（本指南）主要适用于以下工程师：

-   技术支持工程师
-   软件开发工程师

**符号约定<a name="section133020216410"></a>**

在本文中可能出现下列标志，它们所代表的含义如下。

<a name="table2622507016410"></a>
<table><thead align="left"><tr id="row1530720816410"><th class="cellrowborder" valign="top" width="20.580000000000002%" id="mcps1.1.3.1.1"><p id="p6450074116410"><a name="p6450074116410"></a><a name="p6450074116410"></a><strong id="b2136615816410"><a name="b2136615816410"></a><a name="b2136615816410"></a>符号</strong></p>
</th>
<th class="cellrowborder" valign="top" width="79.42%" id="mcps1.1.3.1.2"><p id="p5435366816410"><a name="p5435366816410"></a><a name="p5435366816410"></a><strong id="b5941558116410"><a name="b5941558116410"></a><a name="b5941558116410"></a>说明</strong></p>
</th>
</tr>
</thead>
<tbody><tr id="row1372280416410"><td class="cellrowborder" valign="top" width="20.580000000000002%" headers="mcps1.1.3.1.1 "><p id="p3734547016410"><a name="p3734547016410"></a><a name="p3734547016410"></a><a name="image2670064316410"></a><a name="image2670064316410"></a><span><img class="" id="image2670064316410" height="25.270000000000003" width="67.83" src="figures/zh-cn_image_0000002441733537.png"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79.42%" headers="mcps1.1.3.1.2 "><p id="p1757432116410"><a name="p1757432116410"></a><a name="p1757432116410"></a>表示如不避免则将会导致死亡或严重伤害的具有高等级风险的危害。</p>
</td>
</tr>
<tr id="row466863216410"><td class="cellrowborder" valign="top" width="20.580000000000002%" headers="mcps1.1.3.1.1 "><p id="p1432579516410"><a name="p1432579516410"></a><a name="p1432579516410"></a><a name="image4895582316410"></a><a name="image4895582316410"></a><span><img class="" id="image4895582316410" height="25.270000000000003" width="67.83" src="figures/zh-cn_image_0000002441733653.png"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79.42%" headers="mcps1.1.3.1.2 "><p id="p959197916410"><a name="p959197916410"></a><a name="p959197916410"></a>表示如不避免则可能导致死亡或严重伤害的具有中等级风险的危害。</p>
</td>
</tr>
<tr id="row123863216410"><td class="cellrowborder" valign="top" width="20.580000000000002%" headers="mcps1.1.3.1.1 "><p id="p1232579516410"><a name="p1232579516410"></a><a name="p1232579516410"></a><a name="image1235582316410"></a><a name="image1235582316410"></a><span><img class="" id="image1235582316410" height="25.270000000000003" width="67.83" src="figures/zh-cn_image_0000002408294304.png"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79.42%" headers="mcps1.1.3.1.2 "><p id="p123197916410"><a name="p123197916410"></a><a name="p123197916410"></a>表示如不避免则可能导致轻微或中度伤害的具有低等级风险的危害。</p>
</td>
</tr>
<tr id="row5786682116410"><td class="cellrowborder" valign="top" width="20.580000000000002%" headers="mcps1.1.3.1.1 "><p id="p2204984716410"><a name="p2204984716410"></a><a name="p2204984716410"></a><a name="image4504446716410"></a><a name="image4504446716410"></a><span><img class="" id="image4504446716410" height="25.270000000000003" width="67.83" src="figures/zh-cn_image_0000002441853733.png"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79.42%" headers="mcps1.1.3.1.2 "><p id="p4388861916410"><a name="p4388861916410"></a><a name="p4388861916410"></a>用于传递设备或环境安全警示信息。如不避免则可能会导致设备损坏、数据丢失、设备性能降低或其它不可预知的结果。</p>
<p id="p1238861916410"><a name="p1238861916410"></a><a name="p1238861916410"></a>“须知”不涉及人身伤害。</p>
</td>
</tr>
<tr id="row2856923116410"><td class="cellrowborder" valign="top" width="20.580000000000002%" headers="mcps1.1.3.1.1 "><p id="p5555360116410"><a name="p5555360116410"></a><a name="p5555360116410"></a><a name="image799324016410"></a><a name="image799324016410"></a><span><img class="" id="image799324016410" height="25.270000000000003" width="67.83" src="figures/zh-cn_image_0000002408134440.png"></span></p>
</td>
<td class="cellrowborder" valign="top" width="79.42%" headers="mcps1.1.3.1.2 "><p id="p4612588116410"><a name="p4612588116410"></a><a name="p4612588116410"></a>对正文中重点信息的补充说明。</p>
<p id="p1232588116410"><a name="p1232588116410"></a><a name="p1232588116410"></a>“说明”不是安全警示信息，不涉及人身、设备及环境伤害信息。</p>
</td>
</tr>
</tbody>
</table>

**修改记录<a name="section2467512116410"></a>**

<a name="table1557726816410"></a>
<table><thead align="left"><tr id="row2942532716410"><th class="cellrowborder" valign="top" width="20.72%" id="mcps1.1.4.1.1"><p id="p3778275416410"><a name="p3778275416410"></a><a name="p3778275416410"></a><strong id="b5687322716410"><a name="b5687322716410"></a><a name="b5687322716410"></a>文档版本</strong></p>
</th>
<th class="cellrowborder" valign="top" width="26.119999999999997%" id="mcps1.1.4.1.2"><p id="p5627845516410"><a name="p5627845516410"></a><a name="p5627845516410"></a><strong id="b5800814916410"><a name="b5800814916410"></a><a name="b5800814916410"></a>发布日期</strong></p>
</th>
<th class="cellrowborder" valign="top" width="53.16%" id="mcps1.1.4.1.3"><p id="p2382284816410"><a name="p2382284816410"></a><a name="p2382284816410"></a><strong id="b3316380216410"><a name="b3316380216410"></a><a name="b3316380216410"></a>修改说明</strong></p>
</th>
</tr>
</thead>
<tbody><tr id="row5947359616410"><td class="cellrowborder" valign="top" width="20.72%" headers="mcps1.1.4.1.1 "><p id="p2149706016410"><a name="p2149706016410"></a><a name="p2149706016410"></a>00B01</p>
</td>
<td class="cellrowborder" valign="top" width="26.119999999999997%" headers="mcps1.1.4.1.2 "><p id="p648803616410"><a name="p648803616410"></a><a name="p648803616410"></a>2025-09-15</p>
</td>
<td class="cellrowborder" valign="top" width="53.16%" headers="mcps1.1.4.1.3 "><p id="p1946537916410"><a name="p1946537916410"></a><a name="p1946537916410"></a>第1次临时版本发布</p>
</td>
</tr>
</tbody>
</table>

# 概述<a name="ZH-CN_TOPIC_0000002408294044"></a>


## 概述<a name="ZH-CN_TOPIC_0000002408134140"></a>

IVS（Intelligent Video Surveillance）比IVE（Intelligent Video Engine，识别加速引擎）更高层次的识别视频采集应用API。用户基于IVS可以快速开发出相关识别应用。当前IVS支持的识别应用有：MD（Motion Detection，移动侦测）。

# MD<a name="ZH-CN_TOPIC_0000002408134164"></a>






## 功能描述<a name="ZH-CN_TOPIC_0000002441733317"></a>


### 移动侦测<a name="ZH-CN_TOPIC_0000002408134128"></a>

移动侦测通过检测视频的亮度变化，侦测视频的运动状态，得出视频侦测分析结果。


#### 基本概念<a name="ZH-CN_TOPIC_0000002408294084"></a>

-   MD 算法

    MD 算法包含帧差法（MD\_ALG\_MODE\_REF）和背景法（MD\_ALG\_MODE\_BG）两种。

    -   帧差法（MD\_ALG\_MODE\_REF）

        直接以用户指定的图像为参考帧，得出视频侦测分析结果的算法，称为帧差法。

    -   背景法（MD\_ALG\_MODE\_BG）

        在MD 处理的过程中，将产生当前视频的背景图像。然后以背景图像为参考帧，得出视频侦测分析结果的算法，称为背景法。

-   背景更新权重

    当MD 算法选择为背景法时，每次MD 处理都会产生静止部分图像，这部分图像和背景会作一次像素值叠加，新背景 = （静止部分图像的叠加权重x×静止部分图像 + 动态部分图像的叠加权重y× 旧背景）\>\> 16。

>![](public_sys-resources/icon-notice.gif) **须知：** 
>如果是使用64位操作系统，需要使用到的MMZ地址必须是在一个4GB空间内，否则会出现异常。

## API 参考<a name="ZH-CN_TOPIC_0000002441733361"></a>

MD API提供了初始化、退出、获取句柄、释放句柄、获取背景和侦测处理基本接口。

该功能模块提供以下API：

-   [ss\_ivs\_md\_init](ss_ivs_md_init.md)：初始化。
-   [ss\_ivs\_md\_exit](ss_ivs_md_exit.md)：退出。
-   [ss\_ivs\_md\_create\_chn](ss_ivs_md_create_chn.md)：创建MD通道。
-   [ss\_ivs\_md\_destroy\_chn](ss_ivs_md_destroy_chn.md)：销毁MD通道。
-   [ss\_ivs\_md\_set\_chn\_attr](ss_ivs_md_set_chn_attr.md)：设置MD通道属性。
-   [ss\_ivs\_md\_get\_chn\_attr](ss_ivs_md_get_chn_attr.md)：获取MD通道属性。
-   [ss\_ivs\_md\_get\_bg](ss_ivs_md_get_bg.md)：获取背景。
-   [ss\_ivs\_md\_proc](ss_ivs_md_proc.md)：侦测处理。









### ss\_ivs\_md\_init<a name="ZH-CN_TOPIC_0000002441733309"></a>

【描述】

移动侦测初始化。

【语法】

```
td_s32 ss_ivs_md_init(td_void)；
```

【参数】

无。

【返回值】

<a name="table288mcpsimp"></a>
<table><thead align="left"><tr id="row293mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p295mcpsimp"><a name="p295mcpsimp"></a><a name="p295mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p297mcpsimp"><a name="p297mcpsimp"></a><a name="p297mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row299mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p301mcpsimp"><a name="p301mcpsimp"></a><a name="p301mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p303mcpsimp"><a name="p303mcpsimp"></a><a name="p303mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row304mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p306mcpsimp"><a name="p306mcpsimp"></a><a name="p306mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p308mcpsimp"><a name="p308mcpsimp"></a><a name="p308mcpsimp"></a>失败，参见<a href="错误码.md">错误码</a><span xml:lang="fr-FR" id="ph311mcpsimp"><a name="ph311mcpsimp"></a><a name="ph311mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ot\_common\_svp.h、ot\_common\_md.h、ss\_ivs\_md.h
-   库文件：libss\_md.a

【注意】

-   调用MD其他接口前必须先调用此接口进行初始化，而且只需调用一次即可，否则返回错误。
-   该接口必须和[ss\_ivs\_md\_exit](ss_ivs_md_exit.md)配套使用。

【举例】

无。

【相关主题】

[ss\_ivs\_md\_exit](#ss_ivs_md_exit)

### ss\_ivs\_md\_exit<a name="ZH-CN_TOPIC_0000002408134148"></a>

【描述】

移动侦测退出。

【语法】

```
td_s32 ss_ivs_md_exit(td_void);
```

【参数】

无。

【返回值】

<a name="table333mcpsimp"></a>
<table><thead align="left"><tr id="row338mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p340mcpsimp"><a name="p340mcpsimp"></a><a name="p340mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p342mcpsimp"><a name="p342mcpsimp"></a><a name="p342mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row344mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p346mcpsimp"><a name="p346mcpsimp"></a><a name="p346mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p348mcpsimp"><a name="p348mcpsimp"></a><a name="p348mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row349mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p351mcpsimp"><a name="p351mcpsimp"></a><a name="p351mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p353mcpsimp"><a name="p353mcpsimp"></a><a name="p353mcpsimp"></a>失败，参见<a href="错误码.md">错误码</a><span xml:lang="fr-FR" id="ph356mcpsimp"><a name="ph356mcpsimp"></a><a name="ph356mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ot\_common\_svp.h、ot\_common\_md.h、ss\_ivs\_md.h
-   库文件：libss\_md.a

【注意】

必须先调用[ss\_ivs\_md\_init](ss_ivs_md_init.md)初始化才能调用此接口退出，否则返回错误。

【举例】

无。

【相关主题】

[ss\_ivs\_md\_init](#ss_ivs_md_init)

### ss\_ivs\_md\_create\_chn<a name="ZH-CN_TOPIC_0000002441733333"></a>

【描述】

创建MD通道。

【语法】

```
td_s32 ss_ivs_md_create_chn(ot_md_chn md_chn, ot_md_attr *md_attr);
```

【参数】

<a name="table376mcpsimp"></a>
<table><thead align="left"><tr id="row382mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p384mcpsimp"><a name="p384mcpsimp"></a><a name="p384mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.4.1.2"><p id="p386mcpsimp"><a name="p386mcpsimp"></a><a name="p386mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="23%" id="mcps1.1.4.1.3"><p id="p388mcpsimp"><a name="p388mcpsimp"></a><a name="p388mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row390mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p392mcpsimp"><a name="p392mcpsimp"></a><a name="p392mcpsimp"></a>md_chn</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p394mcpsimp"><a name="p394mcpsimp"></a><a name="p394mcpsimp"></a>通道号，有效范围：[0, 63]</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.1.4.1.3 "><p id="p396mcpsimp"><a name="p396mcpsimp"></a><a name="p396mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row397mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p399mcpsimp"><a name="p399mcpsimp"></a><a name="p399mcpsimp"></a>md_attr</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p401mcpsimp"><a name="p401mcpsimp"></a><a name="p401mcpsimp"></a>通道信息指针。</p>
<p id="p402mcpsimp"><a name="p402mcpsimp"></a><a name="p402mcpsimp"></a>不能为空。</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.1.4.1.3 "><p id="p404mcpsimp"><a name="p404mcpsimp"></a><a name="p404mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table406mcpsimp"></a>
<table><thead align="left"><tr id="row411mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p413mcpsimp"><a name="p413mcpsimp"></a><a name="p413mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p415mcpsimp"><a name="p415mcpsimp"></a><a name="p415mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row417mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p419mcpsimp"><a name="p419mcpsimp"></a><a name="p419mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p421mcpsimp"><a name="p421mcpsimp"></a><a name="p421mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row422mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p424mcpsimp"><a name="p424mcpsimp"></a><a name="p424mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p426mcpsimp"><a name="p426mcpsimp"></a><a name="p426mcpsimp"></a>失败，参见<a href="错误码.md">错误码</a><span xml:lang="fr-FR" id="ph429mcpsimp"><a name="ph429mcpsimp"></a><a name="ph429mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ot\_common\_svp.h、ot\_common\_md.h、ss\_ivs\_md.h
-   库文件：libss\_md.a

【注意】

必须先调用[ss\_ivs\_md\_init](ss_ivs_md_init.md)初始化，否则返回错误。

【举例】

无。

【相关主题】

-   [ss\_ivs\_md\_destroy\_chn](#ss_ivs_md_destroy_chn)
-   [ss\_ivs\_md\_set\_chn\_attr](#ss_ivs_md_set_chn_attr)
-   [ss\_ivs\_md\_get\_chn\_attr](#ss_ivs_md_get_chn_attr)
-   [ss\_ivs\_md\_get\_bg](#ss_ivs_md_get_bg)
-   [ss\_ivs\_md\_proc](#ss_ivs_md_proc)

### ss\_ivs\_md\_destroy\_chn<a name="ZH-CN_TOPIC_0000002441853505"></a>

【描述】

销毁MD通道。

【语法】

```
td_s32 ss_ivs_md_destroy_chn(ot_md_chn md_chn);
```

【参数】

<a name="table457mcpsimp"></a>
<table><thead align="left"><tr id="row463mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p465mcpsimp"><a name="p465mcpsimp"></a><a name="p465mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.4.1.2"><p id="p467mcpsimp"><a name="p467mcpsimp"></a><a name="p467mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="23%" id="mcps1.1.4.1.3"><p id="p469mcpsimp"><a name="p469mcpsimp"></a><a name="p469mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row471mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p473mcpsimp"><a name="p473mcpsimp"></a><a name="p473mcpsimp"></a>md_chn</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p475mcpsimp"><a name="p475mcpsimp"></a><a name="p475mcpsimp"></a>通道号，有效范围：[0, 63]</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.1.4.1.3 "><p id="p477mcpsimp"><a name="p477mcpsimp"></a><a name="p477mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table479mcpsimp"></a>
<table><thead align="left"><tr id="row484mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p486mcpsimp"><a name="p486mcpsimp"></a><a name="p486mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p488mcpsimp"><a name="p488mcpsimp"></a><a name="p488mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row490mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p492mcpsimp"><a name="p492mcpsimp"></a><a name="p492mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p494mcpsimp"><a name="p494mcpsimp"></a><a name="p494mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row495mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p497mcpsimp"><a name="p497mcpsimp"></a><a name="p497mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p499mcpsimp"><a name="p499mcpsimp"></a><a name="p499mcpsimp"></a>失败，参见<a href="错误码.md">错误码</a><span xml:lang="fr-FR" id="ph502mcpsimp"><a name="ph502mcpsimp"></a><a name="ph502mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ot\_common\_svp.h、ot\_common\_md.h、ss\_ivs\_md.h
-   库文件：libss\_md.a

【注意】

-   必须先调用[ss\_ivs\_md\_init](ss_ivs_md_init.md)初始化，否则返回错误。
-   md\_chn必须为[ss\_ivs\_md\_create\_chn](ss_ivs_md_create_chn.md)已创建的通道号,否则返回错误。

【举例】

无。

【相关主题】

-   [ss\_ivs\_md\_create\_chn](#ss_ivs_md_create_chn)
-   [ss\_ivs\_md\_set\_chn\_attr](#ss_ivs_md_set_chn_attr)
-   [ss\_ivs\_md\_get\_chn\_attr](#ss_ivs_md_get_chn_attr)
-   [ss\_ivs\_md\_get\_bg](#ss_ivs_md_get_bg)
-   [ss\_ivs\_md\_proc](#ss_ivs_md_proc)

### ss\_ivs\_md\_set\_chn\_attr<a name="ZH-CN_TOPIC_0000002408294068"></a>

【描述】

设置MD通道属性。

【语法】

```
td_s32 ss_ivs_md_set_chn_attr(ot_md_chn md_chn, ot_md_attr *md_attr);
```

【参数】

<a name="table534mcpsimp"></a>
<table><thead align="left"><tr id="row540mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p542mcpsimp"><a name="p542mcpsimp"></a><a name="p542mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.4.1.2"><p id="p544mcpsimp"><a name="p544mcpsimp"></a><a name="p544mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="23%" id="mcps1.1.4.1.3"><p id="p546mcpsimp"><a name="p546mcpsimp"></a><a name="p546mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row548mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p550mcpsimp"><a name="p550mcpsimp"></a><a name="p550mcpsimp"></a>md_chn</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p552mcpsimp"><a name="p552mcpsimp"></a><a name="p552mcpsimp"></a>通道号，有效范围：[0, 63]</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.1.4.1.3 "><p id="p554mcpsimp"><a name="p554mcpsimp"></a><a name="p554mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row555mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p557mcpsimp"><a name="p557mcpsimp"></a><a name="p557mcpsimp"></a>md_attr</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p559mcpsimp"><a name="p559mcpsimp"></a><a name="p559mcpsimp"></a>通道信息指针。</p>
<p id="p560mcpsimp"><a name="p560mcpsimp"></a><a name="p560mcpsimp"></a>不能为空</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.1.4.1.3 "><p id="p562mcpsimp"><a name="p562mcpsimp"></a><a name="p562mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table564mcpsimp"></a>
<table><thead align="left"><tr id="row569mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p571mcpsimp"><a name="p571mcpsimp"></a><a name="p571mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p573mcpsimp"><a name="p573mcpsimp"></a><a name="p573mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row575mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p577mcpsimp"><a name="p577mcpsimp"></a><a name="p577mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p579mcpsimp"><a name="p579mcpsimp"></a><a name="p579mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row580mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p582mcpsimp"><a name="p582mcpsimp"></a><a name="p582mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p584mcpsimp"><a name="p584mcpsimp"></a><a name="p584mcpsimp"></a>失败，参见<a href="错误码.md">错误码</a><span xml:lang="fr-FR" id="ph587mcpsimp"><a name="ph587mcpsimp"></a><a name="ph587mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ot\_common\_svp.h、ot\_common\_md.h、ss\_ivs\_md.h
-   库文件：libss\_md.a

【注意】

-   必须先调用[ss\_ivs\_md\_init](ss_ivs_md_init.md)初始化，否则返回错误。
-   md\_chn必须为[ss\_ivs\_md\_create\_chn](ss_ivs_md_create_chn.md)已创建的通道号,否则返回错误。
-   通道静态属性\(alg\_mode、sad\_mode、width、height\)不能更改，必须与创建通道时相等，否则返回错误。

【举例】

无。

【相关主题】

-   [ss\_ivs\_md\_create\_chn](#ss_ivs_md_create_chn)
-   [ss\_ivs\_md\_destroy\_chn](#ss_ivs_md_destroy_chn)
-   [ss\_ivs\_md\_get\_chn\_attr](#ss_ivs_md_get_chn_attr)
-   [ss\_ivs\_md\_get\_bg](#ss_ivs_md_get_bg)
-   [ss\_ivs\_md\_proc](#ss_ivs_md_proc)

### ss\_ivs\_md\_get\_chn\_attr<a name="ZH-CN_TOPIC_0000002408294052"></a>

【描述】

获取MD通道属性。

【语法】

```
td_s32 ss_ivs_md_get_chn_attr(ot_md_chn md_chn, ot_md_attr *md_attr);
```

【参数】

<a name="table620mcpsimp"></a>
<table><thead align="left"><tr id="row626mcpsimp"><th class="cellrowborder" valign="top" width="22%" id="mcps1.1.4.1.1"><p id="p628mcpsimp"><a name="p628mcpsimp"></a><a name="p628mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.4.1.2"><p id="p630mcpsimp"><a name="p630mcpsimp"></a><a name="p630mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="23%" id="mcps1.1.4.1.3"><p id="p632mcpsimp"><a name="p632mcpsimp"></a><a name="p632mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row634mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p636mcpsimp"><a name="p636mcpsimp"></a><a name="p636mcpsimp"></a>md_chn</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p638mcpsimp"><a name="p638mcpsimp"></a><a name="p638mcpsimp"></a>通道号，有效范围：[0, 63]</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.1.4.1.3 "><p id="p640mcpsimp"><a name="p640mcpsimp"></a><a name="p640mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row641mcpsimp"><td class="cellrowborder" valign="top" width="22%" headers="mcps1.1.4.1.1 "><p id="p643mcpsimp"><a name="p643mcpsimp"></a><a name="p643mcpsimp"></a>md_attr</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.4.1.2 "><p id="p645mcpsimp"><a name="p645mcpsimp"></a><a name="p645mcpsimp"></a>通道信息指针</p>
<p id="p646mcpsimp"><a name="p646mcpsimp"></a><a name="p646mcpsimp"></a>不能为空</p>
</td>
<td class="cellrowborder" valign="top" width="23%" headers="mcps1.1.4.1.3 "><p id="p648mcpsimp"><a name="p648mcpsimp"></a><a name="p648mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table650mcpsimp"></a>
<table><thead align="left"><tr id="row655mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p657mcpsimp"><a name="p657mcpsimp"></a><a name="p657mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p659mcpsimp"><a name="p659mcpsimp"></a><a name="p659mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row661mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p663mcpsimp"><a name="p663mcpsimp"></a><a name="p663mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p665mcpsimp"><a name="p665mcpsimp"></a><a name="p665mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row666mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p668mcpsimp"><a name="p668mcpsimp"></a><a name="p668mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p670mcpsimp"><a name="p670mcpsimp"></a><a name="p670mcpsimp"></a>失败，参见<a href="错误码.md">错误码</a><span xml:lang="fr-FR" id="ph673mcpsimp"><a name="ph673mcpsimp"></a><a name="ph673mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ot\_common\_svp.h、ot\_common\_md.h、ss\_ivs\_md.h
-   库文件：libss\_md.a

【注意】

-   必须先调用[ss\_ivs\_md\_init](ss_ivs_md_init.md)初始化，否则返回错误。
-   md\_chn必须为[ss\_ivs\_md\_create\_chn](ss_ivs_md_create_chn.md)已创建的通道号,否则返回错误。

【举例】

无。

【相关主题】

-   [ss\_ivs\_md\_create\_chn](#ss_ivs_md_create_chn)
-   [ss\_ivs\_md\_destroy\_chn](#ss_ivs_md_destroy_chn)
-   [ss\_ivs\_md\_set\_chn\_attr](#ss_ivs_md_set_chn_attr)
-   [ss\_ivs\_md\_get\_bg](#ss_ivs_md_get_bg)
-   [ss\_ivs\_md\_proc](#ss_ivs_md_proc)

### ss\_ivs\_md\_get\_bg<a name="ZH-CN_TOPIC_0000002408134192"></a>

【描述】

获取移动侦测背景。

【语法】

```
td_s32 ss_ivs_md_get_bg(ot_md_chn md_chn, ot_svp_dst_img *bg);
```

【参数】

<a name="table704mcpsimp"></a>
<table><thead align="left"><tr id="row710mcpsimp"><th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.1"><p id="p712mcpsimp"><a name="p712mcpsimp"></a><a name="p712mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="67%" id="mcps1.1.4.1.2"><p id="p714mcpsimp"><a name="p714mcpsimp"></a><a name="p714mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="17%" id="mcps1.1.4.1.3"><p id="p716mcpsimp"><a name="p716mcpsimp"></a><a name="p716mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row718mcpsimp"><td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.1 "><p id="p720mcpsimp"><a name="p720mcpsimp"></a><a name="p720mcpsimp"></a>md_chn</p>
</td>
<td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.4.1.2 "><p id="p722mcpsimp"><a name="p722mcpsimp"></a><a name="p722mcpsimp"></a>通道号，有效范围：[0, 63]</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.4.1.3 "><p id="p724mcpsimp"><a name="p724mcpsimp"></a><a name="p724mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row725mcpsimp"><td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.1 "><p id="p727mcpsimp"><a name="p727mcpsimp"></a><a name="p727mcpsimp"></a>bg</p>
</td>
<td class="cellrowborder" valign="top" width="67%" headers="mcps1.1.4.1.2 "><p id="p729mcpsimp"><a name="p729mcpsimp"></a><a name="p729mcpsimp"></a>背景图像指针；不能为空。</p>
<p id="p730mcpsimp"><a name="p730mcpsimp"></a><a name="p730mcpsimp"></a>详细定义请参见《IVE API参考》的3.1章节</p>
</td>
<td class="cellrowborder" valign="top" width="17%" headers="mcps1.1.4.1.3 "><p id="p732mcpsimp"><a name="p732mcpsimp"></a><a name="p732mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

<a name="table733mcpsimp"></a>
<table><thead align="left"><tr id="row740mcpsimp"><th class="cellrowborder" valign="top" width="15%" id="mcps1.1.5.1.1"><p id="p742mcpsimp"><a name="p742mcpsimp"></a><a name="p742mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="41%" id="mcps1.1.5.1.2"><p id="p744mcpsimp"><a name="p744mcpsimp"></a><a name="p744mcpsimp"></a>支持图像类型</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.5.1.3"><p id="p746mcpsimp"><a name="p746mcpsimp"></a><a name="p746mcpsimp"></a>地址对齐</p>
</th>
<th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.5.1.4"><p id="p748mcpsimp"><a name="p748mcpsimp"></a><a name="p748mcpsimp"></a>分辨率</p>
</th>
</tr>
</thead>
<tbody><tr id="row750mcpsimp"><td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.5.1.1 "><p id="p752mcpsimp"><a name="p752mcpsimp"></a><a name="p752mcpsimp"></a>bg</p>
</td>
<td class="cellrowborder" valign="top" width="41%" headers="mcps1.1.5.1.2 "><p id="p754mcpsimp"><a name="p754mcpsimp"></a><a name="p754mcpsimp"></a>OT_SVP_IMG_TYPE_U8C1</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.5.1.3 "><p id="p756mcpsimp"><a name="p756mcpsimp"></a><a name="p756mcpsimp"></a>16 byte</p>
</td>
<td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.5.1.4 "><p id="p758mcpsimp"><a name="p758mcpsimp"></a><a name="p758mcpsimp"></a>64x64～1920x1080</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table760mcpsimp"></a>
<table><thead align="left"><tr id="row765mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p767mcpsimp"><a name="p767mcpsimp"></a><a name="p767mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p769mcpsimp"><a name="p769mcpsimp"></a><a name="p769mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row771mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p773mcpsimp"><a name="p773mcpsimp"></a><a name="p773mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p775mcpsimp"><a name="p775mcpsimp"></a><a name="p775mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row776mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p778mcpsimp"><a name="p778mcpsimp"></a><a name="p778mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p780mcpsimp"><a name="p780mcpsimp"></a><a name="p780mcpsimp"></a>失败，参见<a href="错误码.md">错误码</a><span xml:lang="fr-FR" id="ph783mcpsimp"><a name="ph783mcpsimp"></a><a name="ph783mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ot\_common\_svp.h、ot\_common\_md.h、ss\_ivs\_md.h
-   库文件：libss\_md.a

【注意】

-   必须先调用[ss\_ivs\_md\_init](ss_ivs_md_init.md)初始化，否则返回错误。
-   md\_chn必须为[ss\_ivs\_md\_create\_chn](ss_ivs_md_create_chn.md)已创建的通道号，否则返回错误。
-   只有背景法时，才能获取背景数据，否则返回错误。

【举例】

无。

【相关主题】

-   [ss\_ivs\_md\_create\_chn](#ss_ivs_md_create_chn)
-   [ss\_ivs\_md\_destroy\_chn](#ss_ivs_md_destroy_chn)
-   [ss\_ivs\_md\_set\_chn\_attr](#ss_ivs_md_set_chn_attr)
-   [ss\_ivs\_md\_proc](#ss_ivs_md_proc)

### ss\_ivs\_md\_proc<a name="ZH-CN_TOPIC_0000002441733297"></a>

【描述】

移动侦测处理。

【语法】

```
td_s32 ss_ivs_md_proc(ot_md_chn md_chn, ot_svp_src_img *cur, ot_svp_src_img *ref, ot_svp_dst_img *sad, ot_svp_dst_mem_info *blob)；
```

【参数】

<a name="table815mcpsimp"></a>
<table><thead align="left"><tr id="row821mcpsimp"><th class="cellrowborder" valign="top" width="14.000000000000002%" id="mcps1.1.4.1.1"><p id="p823mcpsimp"><a name="p823mcpsimp"></a><a name="p823mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.4.1.2"><p id="p825mcpsimp"><a name="p825mcpsimp"></a><a name="p825mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="15%" id="mcps1.1.4.1.3"><p id="p827mcpsimp"><a name="p827mcpsimp"></a><a name="p827mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row829mcpsimp"><td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.1.4.1.1 "><p id="p831mcpsimp"><a name="p831mcpsimp"></a><a name="p831mcpsimp"></a>md_chn</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.4.1.2 "><p id="p833mcpsimp"><a name="p833mcpsimp"></a><a name="p833mcpsimp"></a>通道号，有效范围：[0, 63]</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p835mcpsimp"><a name="p835mcpsimp"></a><a name="p835mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row836mcpsimp"><td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.1.4.1.1 "><p id="p838mcpsimp"><a name="p838mcpsimp"></a><a name="p838mcpsimp"></a>cur</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.4.1.2 "><p id="p840mcpsimp"><a name="p840mcpsimp"></a><a name="p840mcpsimp"></a>当前帧图像指针；不能为空。</p>
<p id="p841mcpsimp"><a name="p841mcpsimp"></a><a name="p841mcpsimp"></a>详细定义请参见《IVE API参考》的3.1章节。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p843mcpsimp"><a name="p843mcpsimp"></a><a name="p843mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row844mcpsimp"><td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.1.4.1.1 "><p id="p846mcpsimp"><a name="p846mcpsimp"></a><a name="p846mcpsimp"></a>ref</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.4.1.2 "><p id="p848mcpsimp"><a name="p848mcpsimp"></a><a name="p848mcpsimp"></a>参考帧图像指针；不能为空。</p>
<p id="p849mcpsimp"><a name="p849mcpsimp"></a><a name="p849mcpsimp"></a>详细定义请参见《IVE API参考》的3.1章节。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p851mcpsimp"><a name="p851mcpsimp"></a><a name="p851mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row852mcpsimp"><td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.1.4.1.1 "><p id="p854mcpsimp"><a name="p854mcpsimp"></a><a name="p854mcpsimp"></a>sad</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.4.1.2 "><p id="p856mcpsimp"><a name="p856mcpsimp"></a><a name="p856mcpsimp"></a>Sad指针。</p>
<p id="p857mcpsimp"><a name="p857mcpsimp"></a><a name="p857mcpsimp"></a>根据md_attr-&gt; sad_out_ctrl，若需要输出则不能为空。</p>
<p id="p858mcpsimp"><a name="p858mcpsimp"></a><a name="p858mcpsimp"></a>详细定义请参见《IVE API参考》的3.1章节。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p860mcpsimp"><a name="p860mcpsimp"></a><a name="p860mcpsimp"></a>输出</p>
</td>
</tr>
<tr id="row861mcpsimp"><td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.1.4.1.1 "><p id="p863mcpsimp"><a name="p863mcpsimp"></a><a name="p863mcpsimp"></a>blob</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.4.1.2 "><p id="p865mcpsimp"><a name="p865mcpsimp"></a><a name="p865mcpsimp"></a>区域信息指针。</p>
<p id="p866mcpsimp"><a name="p866mcpsimp"></a><a name="p866mcpsimp"></a>不能为空。</p>
<p id="p867mcpsimp"><a name="p867mcpsimp"></a><a name="p867mcpsimp"></a>详细定义请参见《SVPx.0 API参考》的1.4章节。</p>
</td>
<td class="cellrowborder" valign="top" width="15%" headers="mcps1.1.4.1.3 "><p id="p869mcpsimp"><a name="p869mcpsimp"></a><a name="p869mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

<a name="table870mcpsimp"></a>
<table><thead align="left"><tr id="row877mcpsimp"><th class="cellrowborder" valign="top" width="8.91089108910891%" id="mcps1.1.5.1.1"><p id="p879mcpsimp"><a name="p879mcpsimp"></a><a name="p879mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="43.56435643564357%" id="mcps1.1.5.1.2"><p id="p881mcpsimp"><a name="p881mcpsimp"></a><a name="p881mcpsimp"></a>支持图像类型</p>
</th>
<th class="cellrowborder" valign="top" width="10.891089108910892%" id="mcps1.1.5.1.3"><p id="p883mcpsimp"><a name="p883mcpsimp"></a><a name="p883mcpsimp"></a>地址对齐</p>
</th>
<th class="cellrowborder" valign="top" width="36.633663366336634%" id="mcps1.1.5.1.4"><p id="p885mcpsimp"><a name="p885mcpsimp"></a><a name="p885mcpsimp"></a>分辨率</p>
</th>
</tr>
</thead>
<tbody><tr id="row887mcpsimp"><td class="cellrowborder" valign="top" width="8.91089108910891%" headers="mcps1.1.5.1.1 "><p id="p889mcpsimp"><a name="p889mcpsimp"></a><a name="p889mcpsimp"></a>cur</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.1.5.1.2 "><p id="p891mcpsimp"><a name="p891mcpsimp"></a><a name="p891mcpsimp"></a>OT_SVP_IMG_TYPE_U8C1</p>
</td>
<td class="cellrowborder" valign="top" width="10.891089108910892%" headers="mcps1.1.5.1.3 "><p id="p893mcpsimp"><a name="p893mcpsimp"></a><a name="p893mcpsimp"></a>16 byte</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.1.5.1.4 "><p id="p895mcpsimp"><a name="p895mcpsimp"></a><a name="p895mcpsimp"></a>64x64～1920x1080</p>
</td>
</tr>
<tr id="row896mcpsimp"><td class="cellrowborder" valign="top" width="8.91089108910891%" headers="mcps1.1.5.1.1 "><p id="p898mcpsimp"><a name="p898mcpsimp"></a><a name="p898mcpsimp"></a>ref</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.1.5.1.2 "><p id="p900mcpsimp"><a name="p900mcpsimp"></a><a name="p900mcpsimp"></a>OT_SVP_IMG_TYPE_U8C1</p>
</td>
<td class="cellrowborder" valign="top" width="10.891089108910892%" headers="mcps1.1.5.1.3 "><p id="p902mcpsimp"><a name="p902mcpsimp"></a><a name="p902mcpsimp"></a>16 byte</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.1.5.1.4 "><p id="p904mcpsimp"><a name="p904mcpsimp"></a><a name="p904mcpsimp"></a>64x64～1920x1080</p>
</td>
</tr>
<tr id="row905mcpsimp"><td class="cellrowborder" valign="top" width="8.91089108910891%" headers="mcps1.1.5.1.1 "><p id="p907mcpsimp"><a name="p907mcpsimp"></a><a name="p907mcpsimp"></a>sad</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.1.5.1.2 "><p id="p909mcpsimp"><a name="p909mcpsimp"></a><a name="p909mcpsimp"></a>OT_SVP_IMG_TYPE_U8C1/ OT_SVP_IMG_TYPE_U16C1</p>
</td>
<td class="cellrowborder" valign="top" width="10.891089108910892%" headers="mcps1.1.5.1.3 "><p id="p911mcpsimp"><a name="p911mcpsimp"></a><a name="p911mcpsimp"></a>16byte</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.1.5.1.4 "><p id="p913mcpsimp"><a name="p913mcpsimp"></a><a name="p913mcpsimp"></a>根据md_attr-&gt;sad_mode，对应 4x4、8x8、16x16 分块模式，高、宽分别为cur的1/4、1/8、1/16。</p>
</td>
</tr>
<tr id="row914mcpsimp"><td class="cellrowborder" valign="top" width="8.91089108910891%" headers="mcps1.1.5.1.1 "><p id="p916mcpsimp"><a name="p916mcpsimp"></a><a name="p916mcpsimp"></a>blob</p>
</td>
<td class="cellrowborder" valign="top" width="43.56435643564357%" headers="mcps1.1.5.1.2 "><p id="p918mcpsimp"><a name="p918mcpsimp"></a><a name="p918mcpsimp"></a>─</p>
</td>
<td class="cellrowborder" valign="top" width="10.891089108910892%" headers="mcps1.1.5.1.3 "><p id="p920mcpsimp"><a name="p920mcpsimp"></a><a name="p920mcpsimp"></a>16 byte</p>
</td>
<td class="cellrowborder" valign="top" width="36.633663366336634%" headers="mcps1.1.5.1.4 "><p id="p922mcpsimp"><a name="p922mcpsimp"></a><a name="p922mcpsimp"></a>─</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table924mcpsimp"></a>
<table><thead align="left"><tr id="row929mcpsimp"><th class="cellrowborder" valign="top" width="28.999999999999996%" id="mcps1.1.3.1.1"><p id="p931mcpsimp"><a name="p931mcpsimp"></a><a name="p931mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="71%" id="mcps1.1.3.1.2"><p id="p933mcpsimp"><a name="p933mcpsimp"></a><a name="p933mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row935mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p937mcpsimp"><a name="p937mcpsimp"></a><a name="p937mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p939mcpsimp"><a name="p939mcpsimp"></a><a name="p939mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row940mcpsimp"><td class="cellrowborder" valign="top" width="28.999999999999996%" headers="mcps1.1.3.1.1 "><p id="p942mcpsimp"><a name="p942mcpsimp"></a><a name="p942mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="71%" headers="mcps1.1.3.1.2 "><p id="p944mcpsimp"><a name="p944mcpsimp"></a><a name="p944mcpsimp"></a>失败，参见<a href="错误码.md">错误码</a><span xml:lang="fr-FR" id="ph947mcpsimp"><a name="ph947mcpsimp"></a><a name="ph947mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ot\_common\_svp.h、ot\_common\_md.h、ss\_ivs\_md.h
-   库文件：libss\_md.a

【注意】

-   必须先调用[ss\_ivs\_md\_init](ss_ivs_md_init.md)初始化，否则返回错误。
-   md\_chn必须为[ss\_ivs\_md\_create\_chn](ss_ivs_md_create_chn.md)已创建的通道号，否则返回错误。
-   最多输出区域信息个数为254，区域信息请参见《IVE API参考》第3章节数据类型中的ot\_ive\_ccblob。ot\_ive\_ccblob的成员info中的cur\_area\_threshold是分块后的面积阈值信息。在这里输出的连通区域信息是连续储存。
-   在同一个线程中，完成初始化和通道创建之后，对同一通道只调用一次ss\_ivs\_md\_proc接口。

【举例】

无。

【相关主题】

-   [ss\_ivs\_md\_create\_chn](#ss_ivs_md_create_chn)
-   [ss\_ivs\_md\_destroy\_chn](#ss_ivs_md_destroy_chn)
-   [ss\_ivs\_md\_set\_chn\_attr](#ss_ivs_md_set_chn_attr)
-   [ss\_ivs\_md\_get\_bg](#ss_ivs_md_get_bg)

## MD数据类型<a name="ZH-CN_TOPIC_0000002441853461"></a>



### ot\_md\_alg\_mode<a name="ZH-CN_TOPIC_0000002441733345"></a>

【说明】

定义MD算法模式。

【定义】

```
typedef enum {
    OT_MD_ALG_MODE_BG	= 0x0,/*Base on background img*/
    OT_MD_ALG_MODE_REF	= 0x1,/*Base on reference img*/
    OT_MD_ALG_MODE_BUTT
}ot_md_alg_mode;
```

【成员】

<a name="table984mcpsimp"></a>
<table><thead align="left"><tr id="row989mcpsimp"><th class="cellrowborder" valign="top" width="72%" id="mcps1.1.3.1.1"><p id="p991mcpsimp"><a name="p991mcpsimp"></a><a name="p991mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="28.000000000000004%" id="mcps1.1.3.1.2"><p id="p993mcpsimp"><a name="p993mcpsimp"></a><a name="p993mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row995mcpsimp"><td class="cellrowborder" valign="top" width="72%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p997mcpsimp"><a name="p997mcpsimp"></a><a name="p997mcpsimp"></a>OT_MD_ALG_MODE_BG</p>
</td>
<td class="cellrowborder" valign="top" width="28.000000000000004%" headers="mcps1.1.3.1.2 "><p id="p999mcpsimp"><a name="p999mcpsimp"></a><a name="p999mcpsimp"></a>背景法。</p>
</td>
</tr>
<tr id="row1000mcpsimp"><td class="cellrowborder" valign="top" width="72%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p1002mcpsimp"><a name="p1002mcpsimp"></a><a name="p1002mcpsimp"></a>OT_MD_ALG_M<span xml:lang="en-US" id="ph1003mcpsimp"><a name="ph1003mcpsimp"></a><a name="ph1003mcpsimp"></a>ODE_REF</span></p>
</td>
<td class="cellrowborder" valign="top" width="28.000000000000004%" headers="mcps1.1.3.1.2 "><p id="p1005mcpsimp"><a name="p1005mcpsimp"></a><a name="p1005mcpsimp"></a>帧差法。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

### ot\_md\_attr<a name="ZH-CN_TOPIC_0000002408134180"></a>

【说明】

定义MD通道属性。

【定义】

```
typedef struct {
         ot_md_alg_mode                alg_mode;            /*Md algorithm mode*/
         ot_ive_sad_mode               sad_mode;            /*Sad mode*/
         ot_ive_sad_out_ctrl           sad_out_ctrl;        /*Sad output ctrl*/
         td_u32                        width;               /*Img width*/
         td_u32                        height;              /*Img height*/
         td_u16                        sad_threshold;       /*Sad thresh*/
         ot_ive_ccl_ctrl               ccl_ctrl;            /*Ccl ctrl*/
         ot_ive_add_ctrl               add_ctrl;            /*Add ctrl*/
}ot_md_attr
```

【成员】

<a name="table1026mcpsimp"></a>
<table><thead align="left"><tr id="row1031mcpsimp"><th class="cellrowborder" valign="top" width="19%" id="mcps1.1.3.1.1"><p id="p1033mcpsimp"><a name="p1033mcpsimp"></a><a name="p1033mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="81%" id="mcps1.1.3.1.2"><p id="p1035mcpsimp"><a name="p1035mcpsimp"></a><a name="p1035mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1037mcpsimp"><td class="cellrowborder" valign="top" width="19%" headers="mcps1.1.3.1.1 "><p id="p1039mcpsimp"><a name="p1039mcpsimp"></a><a name="p1039mcpsimp"></a>alg_mode</p>
</td>
<td class="cellrowborder" valign="top" width="81%" headers="mcps1.1.3.1.2 "><p id="p1041mcpsimp"><a name="p1041mcpsimp"></a><a name="p1041mcpsimp"></a>算法模式。</p>
</td>
</tr>
<tr id="row1042mcpsimp"><td class="cellrowborder" valign="top" width="19%" headers="mcps1.1.3.1.1 "><p id="p1044mcpsimp"><a name="p1044mcpsimp"></a><a name="p1044mcpsimp"></a>sad_mode</p>
</td>
<td class="cellrowborder" valign="top" width="81%" headers="mcps1.1.3.1.2 "><p id="p1046mcpsimp"><a name="p1046mcpsimp"></a><a name="p1046mcpsimp"></a>Sad模式，详细定义请参见《IVE API参考》的3.3章节。</p>
</td>
</tr>
<tr id="row1047mcpsimp"><td class="cellrowborder" valign="top" width="19%" headers="mcps1.1.3.1.1 "><p id="p1049mcpsimp"><a name="p1049mcpsimp"></a><a name="p1049mcpsimp"></a>sad_out_ctrl</p>
</td>
<td class="cellrowborder" valign="top" width="81%" headers="mcps1.1.3.1.2 "><p id="p1051mcpsimp"><a name="p1051mcpsimp"></a><a name="p1051mcpsimp"></a>Sad输出控制，详细定义请参见《IVE API参考》的3.3章节。</p>
<p id="p1052mcpsimp"><a name="p1052mcpsimp"></a><a name="p1052mcpsimp"></a>只支持OT_IVE_SAD_OUT_CTRL_16BIT_BOTH、OT_IVE_SAD_OUT_CTRL_8BIT_BOTH、OT_IVE_SAD_OUT_CTRL_THRESHOLD输出控制。</p>
</td>
</tr>
<tr id="row1053mcpsimp"><td class="cellrowborder" valign="top" width="19%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p1055mcpsimp"><a name="p1055mcpsimp"></a><a name="p1055mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" width="81%" headers="mcps1.1.3.1.2 "><p id="p1057mcpsimp"><a name="p1057mcpsimp"></a><a name="p1057mcpsimp"></a>图像宽，必须为宏块宽的偶数倍，范围：[64, 1920]</p>
</td>
</tr>
<tr id="row1058mcpsimp"><td class="cellrowborder" valign="top" width="19%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p1060mcpsimp"><a name="p1060mcpsimp"></a><a name="p1060mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" width="81%" headers="mcps1.1.3.1.2 "><p id="p1062mcpsimp"><a name="p1062mcpsimp"></a><a name="p1062mcpsimp"></a>图像高，必须为宏块高的偶数倍，范围：[64, 1080]</p>
</td>
</tr>
<tr id="row1063mcpsimp"><td class="cellrowborder" valign="top" width="19%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p1065mcpsimp"><a name="p1065mcpsimp"></a><a name="p1065mcpsimp"></a>sad_threshold</p>
</td>
<td class="cellrowborder" valign="top" width="81%" headers="mcps1.1.3.1.2 "><p id="p1067mcpsimp"><a name="p1067mcpsimp"></a><a name="p1067mcpsimp"></a>Sad阈值。</p>
<p id="p1068mcpsimp"><a name="p1068mcpsimp"></a><a name="p1068mcpsimp"></a>取值依赖sad_out_ctrl：</p>
<a name="ol1069mcpsimp"></a><a name="ol1069mcpsimp"></a><ol id="ol1069mcpsimp"><li>OT_IVE_SAD_OUT_CTRL_8BIT_BOTH，取值[0, 255]</li><li>OT_IVE_SAD_OUT_CTRL_16BIT_BOTH和OT_IVE_SAD_OUT_CTRL_THRESHOLD，取值[0, 65535]</li></ol>
</td>
</tr>
<tr id="row1072mcpsimp"><td class="cellrowborder" valign="top" width="19%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p1074mcpsimp"><a name="p1074mcpsimp"></a><a name="p1074mcpsimp"></a>ccl_ctrl</p>
</td>
<td class="cellrowborder" valign="top" width="81%" headers="mcps1.1.3.1.2 "><p id="p1076mcpsimp"><a name="p1076mcpsimp"></a><a name="p1076mcpsimp"></a><span xml:lang="fr-FR" id="ph1077mcpsimp"><a name="ph1077mcpsimp"></a><a name="ph1077mcpsimp"></a>Ccl</span>控制参数<span xml:lang="fr-FR" id="ph1078mcpsimp"><a name="ph1078mcpsimp"></a><a name="ph1078mcpsimp"></a>，</span>详细定义请参见《IVE API参考》的3.3章节。Ccl控制参数成员信息都是针对分块后的图。</p>
</td>
</tr>
<tr id="row1079mcpsimp"><td class="cellrowborder" valign="top" width="19%" headers="mcps1.1.3.1.1 "><p xml:lang="fr-FR" id="p1081mcpsimp"><a name="p1081mcpsimp"></a><a name="p1081mcpsimp"></a>add_ctrl</p>
</td>
<td class="cellrowborder" valign="top" width="81%" headers="mcps1.1.3.1.2 "><p id="p1083mcpsimp"><a name="p1083mcpsimp"></a><a name="p1083mcpsimp"></a><span xml:lang="fr-FR" id="ph1084mcpsimp"><a name="ph1084mcpsimp"></a><a name="ph1084mcpsimp"></a>Add</span>控制参数<span xml:lang="fr-FR" id="ph1085mcpsimp"><a name="ph1085mcpsimp"></a><a name="ph1085mcpsimp"></a>，</span>详细定义请参见《IVE API参考》的3.3章节。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无。

【相关数据类型及接口】

无。

## 错误码<a name="ZH-CN_TOPIC_0000002408294036"></a>

IVS的错误码与IVE的错误码大部分共用，IVS 错误码表所示中前面部分与《IVE API参考》中相同，其他特殊的列在表后面。

**表 1**  IVS 错误码

<a name="_Ref248310770"></a>
<table><thead align="left"><tr id="row1100mcpsimp"><th class="cellrowborder" valign="top" width="22.54%" id="mcps1.2.4.1.1"><p id="p1102mcpsimp"><a name="p1102mcpsimp"></a><a name="p1102mcpsimp"></a>错误代码</p>
</th>
<th class="cellrowborder" valign="top" width="41.03%" id="mcps1.2.4.1.2"><p id="p1104mcpsimp"><a name="p1104mcpsimp"></a><a name="p1104mcpsimp"></a>宏定义</p>
</th>
<th class="cellrowborder" valign="top" width="36.43%" id="mcps1.2.4.1.3"><p id="p1106mcpsimp"><a name="p1106mcpsimp"></a><a name="p1106mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1108mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1110mcpsimp"><a name="p1110mcpsimp"></a><a name="p1110mcpsimp"></a>0xa01d8001</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1112mcpsimp"><a name="p1112mcpsimp"></a><a name="p1112mcpsimp"></a>OT_ERR_IVE_INVALID_DEV_ID</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1114mcpsimp"><a name="p1114mcpsimp"></a><a name="p1114mcpsimp"></a>设备<span xml:lang="fr-FR" id="ph1115mcpsimp"><a name="ph1115mcpsimp"></a><a name="ph1115mcpsimp"></a>ID</span>超出合法范围</p>
</td>
</tr>
<tr id="row1116mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1118mcpsimp"><a name="p1118mcpsimp"></a><a name="p1118mcpsimp"></a>0xa01d8003</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1120mcpsimp"><a name="p1120mcpsimp"></a><a name="p1120mcpsimp"></a>OT_ERR_IVE_INVALID_CHN_ID</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p xml:lang="fr-FR" id="p1122mcpsimp"><a name="p1122mcpsimp"></a><a name="p1122mcpsimp"></a><span xml:lang="en-US" id="ph1123mcpsimp"><a name="ph1123mcpsimp"></a><a name="ph1123mcpsimp"></a>通道</span>组号错误或无效区域句柄</p>
</td>
</tr>
<tr id="row1124mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1126mcpsimp"><a name="p1126mcpsimp"></a><a name="p1126mcpsimp"></a>0xa01d8007</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p xml:lang="it-IT" id="p1128mcpsimp"><a name="p1128mcpsimp"></a><a name="p1128mcpsimp"></a>OT_ERR_IVE_ILLEGAL_PARAM</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1130mcpsimp"><a name="p1130mcpsimp"></a><a name="p1130mcpsimp"></a>参数超出合法范围</p>
</td>
</tr>
<tr id="row1131mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1133mcpsimp"><a name="p1133mcpsimp"></a><a name="p1133mcpsimp"></a>0xa01d8008</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p xml:lang="it-IT" id="p1135mcpsimp"><a name="p1135mcpsimp"></a><a name="p1135mcpsimp"></a>OT_ERR_IVE_EXIST</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1137mcpsimp"><a name="p1137mcpsimp"></a><a name="p1137mcpsimp"></a>重复创建已存在的设备、通道或资源</p>
</td>
</tr>
<tr id="row1138mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1140mcpsimp"><a name="p1140mcpsimp"></a><a name="p1140mcpsimp"></a>0xa01d8009</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1142mcpsimp"><a name="p1142mcpsimp"></a><a name="p1142mcpsimp"></a>OT_ERR_IVE_UNEXIST</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1144mcpsimp"><a name="p1144mcpsimp"></a><a name="p1144mcpsimp"></a>试图使用或者销毁不存在的设备、通道或者资源</p>
</td>
</tr>
<tr id="row1145mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1147mcpsimp"><a name="p1147mcpsimp"></a><a name="p1147mcpsimp"></a>0xa01d800a</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p xml:lang="de-DE" id="p1149mcpsimp"><a name="p1149mcpsimp"></a><a name="p1149mcpsimp"></a>OT_ERR_IVE_NULL_PTR</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1151mcpsimp"><a name="p1151mcpsimp"></a><a name="p1151mcpsimp"></a>函数参数中有空指针</p>
</td>
</tr>
<tr id="row1152mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1154mcpsimp"><a name="p1154mcpsimp"></a><a name="p1154mcpsimp"></a>0xa01d800b</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1156mcpsimp"><a name="p1156mcpsimp"></a><a name="p1156mcpsimp"></a>OT_ERR_IVE_NOT_CFG</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1158mcpsimp"><a name="p1158mcpsimp"></a><a name="p1158mcpsimp"></a>模块没有配置</p>
</td>
</tr>
<tr id="row1159mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1161mcpsimp"><a name="p1161mcpsimp"></a><a name="p1161mcpsimp"></a>0xa01d800c</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1163mcpsimp"><a name="p1163mcpsimp"></a><a name="p1163mcpsimp"></a>OT_ERR_IVE_NOT_SUPPORT</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1165mcpsimp"><a name="p1165mcpsimp"></a><a name="p1165mcpsimp"></a>不支持的参数或者功能</p>
</td>
</tr>
<tr id="row1166mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1168mcpsimp"><a name="p1168mcpsimp"></a><a name="p1168mcpsimp"></a>0xa01d800d</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1170mcpsimp"><a name="p1170mcpsimp"></a><a name="p1170mcpsimp"></a>OT_ERR_IVE_NOT_PERM</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1172mcpsimp"><a name="p1172mcpsimp"></a><a name="p1172mcpsimp"></a>该操作不允许<span xml:lang="fr-FR" id="ph1173mcpsimp"><a name="ph1173mcpsimp"></a><a name="ph1173mcpsimp"></a>，</span>如试图修改静态配置参数</p>
</td>
</tr>
<tr id="row1174mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1176mcpsimp"><a name="p1176mcpsimp"></a><a name="p1176mcpsimp"></a>0xa01d8014</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1178mcpsimp"><a name="p1178mcpsimp"></a><a name="p1178mcpsimp"></a>OT_ERR_IVE_NO_MEM</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1180mcpsimp"><a name="p1180mcpsimp"></a><a name="p1180mcpsimp"></a>分配内存失败，如系统内存不足</p>
</td>
</tr>
<tr id="row1181mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1183mcpsimp"><a name="p1183mcpsimp"></a><a name="p1183mcpsimp"></a>0xa01d8015</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1185mcpsimp"><a name="p1185mcpsimp"></a><a name="p1185mcpsimp"></a>OT_ERR_IVE_NO_BUF</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1187mcpsimp"><a name="p1187mcpsimp"></a><a name="p1187mcpsimp"></a>分配缓存失败，如申请的图像缓冲区太大</p>
</td>
</tr>
<tr id="row1188mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1190mcpsimp"><a name="p1190mcpsimp"></a><a name="p1190mcpsimp"></a>0xa01d8016</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p xml:lang="de-DE" id="p1192mcpsimp"><a name="p1192mcpsimp"></a><a name="p1192mcpsimp"></a>OT_ERR_IVE_BUF_EMPTY</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1194mcpsimp"><a name="p1194mcpsimp"></a><a name="p1194mcpsimp"></a>缓冲区中无图像</p>
</td>
</tr>
<tr id="row1195mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1197mcpsimp"><a name="p1197mcpsimp"></a><a name="p1197mcpsimp"></a>0xa01d8017</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1199mcpsimp"><a name="p1199mcpsimp"></a><a name="p1199mcpsimp"></a>OT_ERR_IVE_BUF_FULL</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1201mcpsimp"><a name="p1201mcpsimp"></a><a name="p1201mcpsimp"></a>缓冲区中图像满</p>
</td>
</tr>
<tr id="row1202mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1204mcpsimp"><a name="p1204mcpsimp"></a><a name="p1204mcpsimp"></a>0xa01d8018</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1206mcpsimp"><a name="p1206mcpsimp"></a><a name="p1206mcpsimp"></a>OT_ERR_IVE_NOT_READY</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1208mcpsimp"><a name="p1208mcpsimp"></a><a name="p1208mcpsimp"></a>系统没有初始化或没有加载相应模块</p>
</td>
</tr>
<tr id="row1209mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1211mcpsimp"><a name="p1211mcpsimp"></a><a name="p1211mcpsimp"></a>0xa01d8021</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1213mcpsimp"><a name="p1213mcpsimp"></a><a name="p1213mcpsimp"></a>OT_ERR_IVE_BAD_ADDR</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1215mcpsimp"><a name="p1215mcpsimp"></a><a name="p1215mcpsimp"></a>地址非法</p>
</td>
</tr>
<tr id="row1216mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1218mcpsimp"><a name="p1218mcpsimp"></a><a name="p1218mcpsimp"></a>0xa01d8022</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1220mcpsimp"><a name="p1220mcpsimp"></a><a name="p1220mcpsimp"></a>OT_ERR_IVE_BUSY</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1222mcpsimp"><a name="p1222mcpsimp"></a><a name="p1222mcpsimp"></a>系统忙</p>
</td>
</tr>
<tr id="row1223mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1225mcpsimp"><a name="p1225mcpsimp"></a><a name="p1225mcpsimp"></a>0xa01d8040</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1227mcpsimp"><a name="p1227mcpsimp"></a><a name="p1227mcpsimp"></a>OT_ERR_IVE_SYS_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1229mcpsimp"><a name="p1229mcpsimp"></a><a name="p1229mcpsimp"></a>IVE系统超时</p>
</td>
</tr>
<tr id="row1230mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1232mcpsimp"><a name="p1232mcpsimp"></a><a name="p1232mcpsimp"></a>0xa01d8041</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1234mcpsimp"><a name="p1234mcpsimp"></a><a name="p1234mcpsimp"></a>OT_ERR_IVE_QUERY_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1236mcpsimp"><a name="p1236mcpsimp"></a><a name="p1236mcpsimp"></a>Query查询超时</p>
</td>
</tr>
<tr id="row1237mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1239mcpsimp"><a name="p1239mcpsimp"></a><a name="p1239mcpsimp"></a>0xa01d8042</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1241mcpsimp"><a name="p1241mcpsimp"></a><a name="p1241mcpsimp"></a>OT_ERR_IVE_BUS_ERR</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1243mcpsimp"><a name="p1243mcpsimp"></a><a name="p1243mcpsimp"></a>总线错误</p>
</td>
</tr>
<tr id="row1244mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1246mcpsimp"><a name="p1246mcpsimp"></a><a name="p1246mcpsimp"></a>0xa01d8043</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1248mcpsimp"><a name="p1248mcpsimp"></a><a name="p1248mcpsimp"></a>OT_ERR_IVE_OPEN_FILE</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1250mcpsimp"><a name="p1250mcpsimp"></a><a name="p1250mcpsimp"></a>打开文件失败</p>
</td>
</tr>
<tr id="row1251mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1253mcpsimp"><a name="p1253mcpsimp"></a><a name="p1253mcpsimp"></a>0xa01d8044</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1255mcpsimp"><a name="p1255mcpsimp"></a><a name="p1255mcpsimp"></a>OT_ERR_IVE_READ_FILE</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1257mcpsimp"><a name="p1257mcpsimp"></a><a name="p1257mcpsimp"></a>读文件失败</p>
</td>
</tr>
<tr id="row1258mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1260mcpsimp"><a name="p1260mcpsimp"></a><a name="p1260mcpsimp"></a>0xa0308003</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1262mcpsimp"><a name="p1262mcpsimp"></a><a name="p1262mcpsimp"></a>OT_ERR_ODT_INVALID_CHN_ID</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p xml:lang="fr-FR" id="p1264mcpsimp"><a name="p1264mcpsimp"></a><a name="p1264mcpsimp"></a><span xml:lang="en-US" id="ph1265mcpsimp"><a name="ph1265mcpsimp"></a><a name="ph1265mcpsimp"></a>ODT通道</span>组号错误或无效区域句柄</p>
</td>
</tr>
<tr id="row1266mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1268mcpsimp"><a name="p1268mcpsimp"></a><a name="p1268mcpsimp"></a>0xa0308008</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1270mcpsimp"><a name="p1270mcpsimp"></a><a name="p1270mcpsimp"></a>OT_ERR_ODT_<span xml:lang="it-IT" id="ph1271mcpsimp"><a name="ph1271mcpsimp"></a><a name="ph1271mcpsimp"></a>EXIST</span></p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1273mcpsimp"><a name="p1273mcpsimp"></a><a name="p1273mcpsimp"></a>重复创建已存在的设备、通道或资源</p>
</td>
</tr>
<tr id="row1274mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1276mcpsimp"><a name="p1276mcpsimp"></a><a name="p1276mcpsimp"></a>0xa0308009</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1278mcpsimp"><a name="p1278mcpsimp"></a><a name="p1278mcpsimp"></a>OT_ERR_ODT_UNEXIST</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1280mcpsimp"><a name="p1280mcpsimp"></a><a name="p1280mcpsimp"></a>试图使用或者销毁不存在的设备、通道或者资源</p>
</td>
</tr>
<tr id="row1281mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1283mcpsimp"><a name="p1283mcpsimp"></a><a name="p1283mcpsimp"></a>0xa030800d</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1285mcpsimp"><a name="p1285mcpsimp"></a><a name="p1285mcpsimp"></a>OT_ERR_ODT_NOT_PERM</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1287mcpsimp"><a name="p1287mcpsimp"></a><a name="p1287mcpsimp"></a>该操作不允许<span xml:lang="fr-FR" id="ph1288mcpsimp"><a name="ph1288mcpsimp"></a><a name="ph1288mcpsimp"></a>，</span>如试图修改静态配置参数</p>
</td>
</tr>
<tr id="row1289mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1291mcpsimp"><a name="p1291mcpsimp"></a><a name="p1291mcpsimp"></a>0xa0308018</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1293mcpsimp"><a name="p1293mcpsimp"></a><a name="p1293mcpsimp"></a>OT_ERR_ODT_NOT_READY</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1295mcpsimp"><a name="p1295mcpsimp"></a><a name="p1295mcpsimp"></a>ODT没有初始化</p>
</td>
</tr>
<tr id="row1296mcpsimp"><td class="cellrowborder" valign="top" width="22.54%" headers="mcps1.2.4.1.1 "><p id="p1298mcpsimp"><a name="p1298mcpsimp"></a><a name="p1298mcpsimp"></a>0xa0308022</p>
</td>
<td class="cellrowborder" valign="top" width="41.03%" headers="mcps1.2.4.1.2 "><p id="p1300mcpsimp"><a name="p1300mcpsimp"></a><a name="p1300mcpsimp"></a>OT_ERR_ODT_BUSY</p>
</td>
<td class="cellrowborder" valign="top" width="36.43%" headers="mcps1.2.4.1.3 "><p id="p1302mcpsimp"><a name="p1302mcpsimp"></a><a name="p1302mcpsimp"></a>ODT系统忙</p>
</td>
</tr>
</tbody>
</table>

## Proc调试信息<a name="ZH-CN_TOPIC_0000002408294100"></a>



### 概述<a name="ZH-CN_TOPIC_0000002441853473"></a>

调试信息采用了Linux下的proc文件系统，可实时反映当前系统的运行状态，所记录的信息可供问题定位及分析时使用。

【文件目录】

/proc/umap

【信息查看方法】

-   在控制台上可以使用cat命令查看信息，cat /proc/umap/md 可以查看MD的proc信息；也可以使用其他常用的文件操作命令，例如 cp /proc/umap/md ./，将文件拷贝到当前目录。
-   在应用程序中可以将上述文件当作普通只读文件进行读操作，例如fopen、fread等。

>![](public_sys-resources/icon-note.gif) **说明：** 
>参数在描述时有以下2种情况需要注意：
>-   取值为\{0, 1\}的参数，如未列出具体取值和含义的对应关系，则参数为1时表示肯定，为0时表示否定。
>-   取值为\{aaa, bbb, ccc\}的参数，未列出具体取值和含义的对应关系，但可直接根据取值aaa、bbb或ccc判断参数含义。

### MD Proc信息说明<a name="ZH-CN_TOPIC_0000002441853441"></a>

【调试信息】

```
~ # cat /proc/umap/md
[MD] Version: [Vx.x.x.x B0xx Release], Build Time[Feb 20 2020, 16:42:49]

---------------------------md chn attr-----------------------------------
no.    w    h   alg   sad_mode  sad_out_ctrl   sad_thr    ccl_mode  ccl_init_thr   
0     720   576   0         0           0         200         1      16 
ccl_step    xwt      ywt        frm_rate  cost_tm_per_frm
4          32768     32768          0          2625
```

【调试信息分析】

记录MD的工作状态信息。

【参数说明】

<a name="table1330mcpsimp"></a>
<table><thead align="left"><tr id="row1336mcpsimp"><th class="cellrowborder" colspan="2" valign="top" id="mcps1.1.4.1.1"><p id="p1338mcpsimp"><a name="p1338mcpsimp"></a><a name="p1338mcpsimp"></a>参数</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.1.4.1.2"><p id="p1340mcpsimp"><a name="p1340mcpsimp"></a><a name="p1340mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1342mcpsimp"><td class="cellrowborder" rowspan="14" valign="top" width="21.21%" headers="mcps1.1.4.1.1 "><p id="p1344mcpsimp"><a name="p1344mcpsimp"></a><a name="p1344mcpsimp"></a>md chn attr</p>
<p id="p1345mcpsimp"><a name="p1345mcpsimp"></a><a name="p1345mcpsimp"></a>通道属性</p>
</td>
<td class="cellrowborder" valign="top" width="19.439999999999998%" headers="mcps1.1.4.1.1 "><p id="p1347mcpsimp"><a name="p1347mcpsimp"></a><a name="p1347mcpsimp"></a>no.</p>
</td>
<td class="cellrowborder" valign="top" width="59.35%" headers="mcps1.1.4.1.2 "><p id="p1349mcpsimp"><a name="p1349mcpsimp"></a><a name="p1349mcpsimp"></a>通道号。</p>
</td>
</tr>
<tr id="row1350mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1352mcpsimp"><a name="p1352mcpsimp"></a><a name="p1352mcpsimp"></a>w</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1354mcpsimp"><a name="p1354mcpsimp"></a><a name="p1354mcpsimp"></a>通道宽度（单位：像素）。</p>
</td>
</tr>
<tr id="row1355mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1357mcpsimp"><a name="p1357mcpsimp"></a><a name="p1357mcpsimp"></a>h</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1359mcpsimp"><a name="p1359mcpsimp"></a><a name="p1359mcpsimp"></a>通道高度（单位：像素）。</p>
</td>
</tr>
<tr id="row1360mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1362mcpsimp"><a name="p1362mcpsimp"></a><a name="p1362mcpsimp"></a>alg</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1364mcpsimp"><a name="p1364mcpsimp"></a><a name="p1364mcpsimp"></a>工作算法。</p>
<p id="p1365mcpsimp"><a name="p1365mcpsimp"></a><a name="p1365mcpsimp"></a>0：背景法；</p>
<p id="p1366mcpsimp"><a name="p1366mcpsimp"></a><a name="p1366mcpsimp"></a>1：帧差法。</p>
</td>
</tr>
<tr id="row1367mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1369mcpsimp"><a name="p1369mcpsimp"></a><a name="p1369mcpsimp"></a>sad_mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1371mcpsimp"><a name="p1371mcpsimp"></a><a name="p1371mcpsimp"></a>Sad模式。</p>
<p id="p1372mcpsimp"><a name="p1372mcpsimp"></a><a name="p1372mcpsimp"></a>0：4x4宏块；</p>
<p id="p1373mcpsimp"><a name="p1373mcpsimp"></a><a name="p1373mcpsimp"></a>1：8x8 宏块；</p>
<p id="p1374mcpsimp"><a name="p1374mcpsimp"></a><a name="p1374mcpsimp"></a>2：16x16 宏块。</p>
</td>
</tr>
<tr id="row1375mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1377mcpsimp"><a name="p1377mcpsimp"></a><a name="p1377mcpsimp"></a>sad_out_ctrl</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1379mcpsimp"><a name="p1379mcpsimp"></a><a name="p1379mcpsimp"></a>Sad输出控制。</p>
<p id="p1380mcpsimp"><a name="p1380mcpsimp"></a><a name="p1380mcpsimp"></a>0：OT_IVE_SAD_OUT_CTRL_16BIT_BOTH；</p>
<p id="p1381mcpsimp"><a name="p1381mcpsimp"></a><a name="p1381mcpsimp"></a>1：OT_IVE_SAD_OUT_CTRL_8BIT_BOTH；</p>
<p id="p1382mcpsimp"><a name="p1382mcpsimp"></a><a name="p1382mcpsimp"></a>4：OT_IVE_SAD_OUT_CTRL_THRESHOLD。</p>
</td>
</tr>
<tr id="row1383mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1385mcpsimp"><a name="p1385mcpsimp"></a><a name="p1385mcpsimp"></a>sad_thr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1387mcpsimp"><a name="p1387mcpsimp"></a><a name="p1387mcpsimp"></a>Sad阈值。</p>
</td>
</tr>
<tr id="row1388mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1390mcpsimp"><a name="p1390mcpsimp"></a><a name="p1390mcpsimp"></a>ccl_mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1392mcpsimp"><a name="p1392mcpsimp"></a><a name="p1392mcpsimp"></a>Ccl 模式。</p>
<p id="p1393mcpsimp"><a name="p1393mcpsimp"></a><a name="p1393mcpsimp"></a>0：4连通；</p>
<p id="p1394mcpsimp"><a name="p1394mcpsimp"></a><a name="p1394mcpsimp"></a>1：8连通。</p>
</td>
</tr>
<tr id="row1395mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1397mcpsimp"><a name="p1397mcpsimp"></a><a name="p1397mcpsimp"></a>ccl_init_thr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1399mcpsimp"><a name="p1399mcpsimp"></a><a name="p1399mcpsimp"></a>Ccl初始阈值。</p>
</td>
</tr>
<tr id="row1400mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1402mcpsimp"><a name="p1402mcpsimp"></a><a name="p1402mcpsimp"></a>ccl_step</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1404mcpsimp"><a name="p1404mcpsimp"></a><a name="p1404mcpsimp"></a>Ccl步长。</p>
</td>
</tr>
<tr id="row1405mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1407mcpsimp"><a name="p1407mcpsimp"></a><a name="p1407mcpsimp"></a>xwt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1409mcpsimp"><a name="p1409mcpsimp"></a><a name="p1409mcpsimp"></a>背景法更新X权重。</p>
</td>
</tr>
<tr id="row1410mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1412mcpsimp"><a name="p1412mcpsimp"></a><a name="p1412mcpsimp"></a>ywt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1414mcpsimp"><a name="p1414mcpsimp"></a><a name="p1414mcpsimp"></a>背景法更新Y权重。</p>
</td>
</tr>
<tr id="row1415mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1417mcpsimp"><a name="p1417mcpsimp"></a><a name="p1417mcpsimp"></a>frm_rate</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1419mcpsimp"><a name="p1419mcpsimp"></a><a name="p1419mcpsimp"></a>帧率。</p>
</td>
</tr>
<tr id="row1420mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1422mcpsimp"><a name="p1422mcpsimp"></a><a name="p1422mcpsimp"></a>cost_tm_per_frm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1424mcpsimp"><a name="p1424mcpsimp"></a><a name="p1424mcpsimp"></a>每帧耗时(单位us)。</p>
<p id="p1425mcpsimp"><a name="p1425mcpsimp"></a><a name="p1425mcpsimp"></a><strong id="b1426mcpsimp"><a name="b1426mcpsimp"></a><a name="b1426mcpsimp"></a>注意：帧率及每帧耗时，每10s统计一次。</strong></p>
</td>
</tr>
</tbody>
</table>

