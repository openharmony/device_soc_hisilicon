# 前言<a name="ZH-CN_TOPIC_0000002408102218"></a>

**概述<a name="section140mcpsimp"></a>**

本文为使用HNR的软件开发工程师和图像质量开发工程师而写，目的是为您在开发过程中提供使用指导和帮助。

>![](public_sys-resources/icon-note.gif) **说明：** 
>本文以SS928V100描述为例，未有特殊说明，SS927V100与SS928V100内容一致。

**产品版本<a name="section143mcpsimp"></a>**

与本文档相对应的产品版本如下。

<a name="table146mcpsimp"></a>
<table><thead align="left"><tr id="row151mcpsimp"><th class="cellrowborder" valign="top" width="32%" id="mcps1.1.3.1.1"><p id="p153mcpsimp"><a name="p153mcpsimp"></a><a name="p153mcpsimp"></a>产品名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.3.1.2"><p id="p155mcpsimp"><a name="p155mcpsimp"></a><a name="p155mcpsimp"></a>产品版本</p>
</th>
</tr>
</thead>
<tbody><tr id="row157mcpsimp"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p159mcpsimp"><a name="p159mcpsimp"></a><a name="p159mcpsimp"></a>SS928</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p161mcpsimp"><a name="p161mcpsimp"></a><a name="p161mcpsimp"></a>V100</p>
</td>
</tr>
<tr id="row79393281419"><td class="cellrowborder" valign="top" width="32%" headers="mcps1.1.3.1.1 "><p id="p761953118419"><a name="p761953118419"></a><a name="p761953118419"></a>SS927</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.3.1.2 "><p id="p1361913118420"><a name="p1361913118420"></a><a name="p1361913118420"></a>V100</p>
</td>
</tr>
</tbody>
</table>

**修订记录<a name="section162mcpsimp"></a>**

修订记录累积了每次文档更新的说明。最新版本的文档包含以前所有文档版本的更新内容。

<a name="table1557726816410"></a>
<table><thead align="left"><tr id="row2942532716410"><th class="cellrowborder" valign="top" width="20.72%" id="mcps1.1.4.1.1"><p id="p3778275416410"><a name="p3778275416410"></a><a name="p3778275416410"></a>文档版本</p>
</th>
<th class="cellrowborder" valign="top" width="21.27%" id="mcps1.1.4.1.2"><p id="p5627845516410"><a name="p5627845516410"></a><a name="p5627845516410"></a>发布日期</p>
</th>
<th class="cellrowborder" valign="top" width="58.01%" id="mcps1.1.4.1.3"><p id="p2382284816410"><a name="p2382284816410"></a><a name="p2382284816410"></a>修改说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row16527181025018"><td class="cellrowborder" valign="top" width="20.72%" headers="mcps1.1.4.1.1 "><p id="p393381565011"><a name="p393381565011"></a><a name="p393381565011"></a>00B02</p>
</td>
<td class="cellrowborder" valign="top" width="21.27%" headers="mcps1.1.4.1.2 "><p id="p179331115155016"><a name="p179331115155016"></a><a name="p179331115155016"></a>2025-12-25</p>
</td>
<td class="cellrowborder" valign="top" width="58.01%" headers="mcps1.1.4.1.3 "><p id="p1893320150503"><a name="p1893320150503"></a><a name="p1893320150503"></a>第2次临时版本发布。</p>
<p id="p11117153545011"><a name="p11117153545011"></a><a name="p11117153545011"></a>ot_hnr_ref_mode【定义】及【成员】涉及修改</p>
</td>
</tr>
<tr id="row1665351220483"><td class="cellrowborder" valign="top" width="20.72%" headers="mcps1.1.4.1.1 "><p id="p10443201017431"><a name="p10443201017431"></a><a name="p10443201017431"></a>00B01</p>
</td>
<td class="cellrowborder" valign="top" width="21.27%" headers="mcps1.1.4.1.2 "><p id="p6851917114310"><a name="p6851917114310"></a><a name="p6851917114310"></a>2025-09-15</p>
</td>
<td class="cellrowborder" valign="top" width="58.01%" headers="mcps1.1.4.1.3 "><p id="p1946537916410"><a name="p1946537916410"></a><a name="p1946537916410"></a>第1次临时版本发布。</p>
</td>
</tr>
</tbody>
</table>

# 概述<a name="ZH-CN_TOPIC_0000002408262122"></a>

-   **[概述](#ZH-CN_TOPIC_0000002441661549)**  

-   **[基本概念](#ZH-CN_TOPIC_0000002408102234)**  

-   **[性能](#ZH-CN_TOPIC_0000002441701401)**  

## 概述<a name="ZH-CN_TOPIC_0000002441661549"></a>

HNR（hypersensitive noise reduction）是一种新型的去噪算法，它能使成像设备在更低照度时噪声去除更干净，细节保留更多，从而提高成像设备极低照度的感光能力。本文主要介绍HNR的接口说明和使用注意事项。

HNR参考帧普通模式性能支持3840 x 2160@30fps，无参考帧模式支持3840 x 2160@40fps，HNR支持的最高帧率为100fps。

## 基本概念<a name="ZH-CN_TOPIC_0000002408102234"></a>

-   sfs \(spatial filter strength\)，即空域去噪强度。
-   tfs \(temporal filter strength\)，即时域去噪强度。

## 性能<a name="ZH-CN_TOPIC_0000002441701401"></a>

线性OT\_HNR\_REF\_MODE\_NORM模式性能数据如下。

<a name="table11488022326"></a>
<table><thead align="left"><tr id="row353517225215"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p195350221520"><a name="p195350221520"></a><a name="p195350221520"></a>分辨率</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p135351522226"><a name="p135351522226"></a><a name="p135351522226"></a>帧率</p>
</th>
</tr>
</thead>
<tbody><tr id="row853513223211"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p1753511221526"><a name="p1753511221526"></a><a name="p1753511221526"></a>3840x2160</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p7535132215214"><a name="p7535132215214"></a><a name="p7535132215214"></a>单路30fps</p>
</td>
</tr>
<tr id="row1053512226218"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p13535132211219"><a name="p13535132211219"></a><a name="p13535132211219"></a>1920x1080</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p16535132216210"><a name="p16535132216210"></a><a name="p16535132216210"></a>单路96fps</p>
</td>
</tr>
<tr id="row165353221023"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p175358221229"><a name="p175358221229"></a><a name="p175358221229"></a>1920x1080</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p195351922525"><a name="p195351922525"></a><a name="p195351922525"></a>四路25fps</p>
</td>
</tr>
<tr id="row1853514221218"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p5535182219215"><a name="p5535182219215"></a><a name="p5535182219215"></a>2688x1520</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p7535162214216"><a name="p7535162214216"></a><a name="p7535162214216"></a>单路57fps</p>
</td>
</tr>
<tr id="row1653610221822"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p5536152215212"><a name="p5536152215212"></a><a name="p5536152215212"></a>2688x1520</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p1753612218214"><a name="p1753612218214"></a><a name="p1753612218214"></a>四路14fps</p>
</td>
</tr>
</tbody>
</table>

# API参考<a name="ZH-CN_TOPIC_0000002408262110"></a>

该功能模块为用户提供以下API：

-   [ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)：初始化HNR模块。
-   [ss\_mpi\_hnr\_exit](#ZH-CN_TOPIC_0000002408262170)：去初始化HNR模块。
-   [ss\_mpi\_hnr\_load\_cfg](#ZH-CN_TOPIC_0000002441701369)：加载HNR的配置文件。
-   [ss\_mpi\_hnr\_unload\_cfg](#ZH-CN_TOPIC_0000002441701385)：卸载HNR的配置文件。
-   [ss\_mpi\_hnr\_set\_alg\_cfg](#ZH-CN_TOPIC_0000002441661513)：设置HNR算法相关配置参数。
-   [ss\_mpi\_hnr\_get\_alg\_cfg](#ZH-CN_TOPIC_0000002441661465)：获取HNR算法相关配置参数。
-   [ss\_mpi\_hnr\_enable](#ZH-CN_TOPIC_0000002408102158)：使能HNR数据流开关。
-   [ss\_mpi\_hnr\_disable](#ZH-CN_TOPIC_0000002408102186)：关闭HNR数据流开关。
-   [ss\_mpi\_hnr\_set\_attr](#ZH-CN_TOPIC_0000002441661457)：设置HNR的属性。
-   [ss\_mpi\_hnr\_get\_attr](#ZH-CN_TOPIC_0000002408262070)：获取HNR的属性。
-   [ss\_mpi\_hnr\_set\_input\_depth](#ZH-CN_TOPIC_0000002441661493)：设置HNR输入队列的缓存深度。
-   [ss\_mpi\_hnr\_set\_thread\_attr](#ZH-CN_TOPIC_0000002441661481)：设置HNR线程属性。
-   [ss\_mpi\_hnr\_get\_thread\_attr](#ZH-CN_TOPIC_0000002408262054)：获取HNR线程属性。
-   [ss\_mpi\_hnr\_attach\_out\_vb\_pool](#ZH-CN_TOPIC_0000002408262098)：HNR输出绑定到某个视频缓存VB池。
-   [ss\_mpi\_hnr\_detach\_out\_vb\_pool](#ZH-CN_TOPIC_0000002441701317)：HNR输出与视频缓存VB池解除绑定。

## ss\_mpi\_hnr\_init<a name="ZH-CN_TOPIC_0000002441661525"></a>

【描述】

初始化HNR模块。

【语法】

```
td_s32 ss_mpi_hnr_init(td_void);
```

【参数】

无

【返回值】

<a name="table194mcpsimp"></a>
<table><thead align="left"><tr id="row199mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p201mcpsimp"><a name="p201mcpsimp"></a><a name="p201mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p203mcpsimp"><a name="p203mcpsimp"></a><a name="p203mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row205mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p207mcpsimp"><a name="p207mcpsimp"></a><a name="p207mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p209mcpsimp"><a name="p209mcpsimp"></a><a name="p209mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row210mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p212mcpsimp"><a name="p212mcpsimp"></a><a name="p212mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p214mcpsimp"><a name="p214mcpsimp"></a><a name="p214mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   不支持多进程调用。
-   调用此接口前需要先初始化VI、ISP模块。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_exit<a name="ZH-CN_TOPIC_0000002408262170"></a>

【描述】

去初始化HNR模块。

【语法】

```
td_void ss_mpi_hnr_exit(td_void);
```

【参数】

无

【返回值】

<a name="table234mcpsimp"></a>
<table><thead align="left"><tr id="row239mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p241mcpsimp"><a name="p241mcpsimp"></a><a name="p241mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p243mcpsimp"><a name="p243mcpsimp"></a><a name="p243mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row245mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p247mcpsimp"><a name="p247mcpsimp"></a><a name="p247mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p249mcpsimp"><a name="p249mcpsimp"></a><a name="p249mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row250mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p252mcpsimp"><a name="p252mcpsimp"></a><a name="p252mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p254mcpsimp"><a name="p254mcpsimp"></a><a name="p254mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   不支持多进程调用。
-   调用此接口去初始化HNR模块前，需要关闭HNR数据流开关。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_load\_cfg<a name="ZH-CN_TOPIC_0000002441701369"></a>

【描述】

加载HNR的配置文件。

【语法】

```
td_s32 ss_mpi_hnr_load_cfg(const ot_hnr_cfg *cfg, td_s32 *cfg_id);
```

【参数】

<a name="table276mcpsimp"></a>
<table><thead align="left"><tr id="row282mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.4.1.1"><p id="p284mcpsimp"><a name="p284mcpsimp"></a><a name="p284mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.4.1.2"><p id="p286mcpsimp"><a name="p286mcpsimp"></a><a name="p286mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p288mcpsimp"><a name="p288mcpsimp"></a><a name="p288mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row290mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p292mcpsimp"><a name="p292mcpsimp"></a><a name="p292mcpsimp"></a>cfg</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.4.1.2 "><p xml:lang="sv-SE" id="p294mcpsimp"><a name="p294mcpsimp"></a><a name="p294mcpsimp"></a>HNR配置文件指针<span xml:lang="en-US" id="ph295mcpsimp"><a name="ph295mcpsimp"></a><a name="ph295mcpsimp"></a>。</span></p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p297mcpsimp"><a name="p297mcpsimp"></a><a name="p297mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row298mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p300mcpsimp"><a name="p300mcpsimp"></a><a name="p300mcpsimp"></a>cfg_id</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.4.1.2 "><p xml:lang="sv-SE" id="p302mcpsimp"><a name="p302mcpsimp"></a><a name="p302mcpsimp"></a>返回的配置文件ID指针<span xml:lang="en-US" id="ph303mcpsimp"><a name="ph303mcpsimp"></a><a name="ph303mcpsimp"></a>。</span></p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p305mcpsimp"><a name="p305mcpsimp"></a><a name="p305mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table307mcpsimp"></a>
<table><thead align="left"><tr id="row312mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p314mcpsimp"><a name="p314mcpsimp"></a><a name="p314mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p316mcpsimp"><a name="p316mcpsimp"></a><a name="p316mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row318mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p320mcpsimp"><a name="p320mcpsimp"></a><a name="p320mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p322mcpsimp"><a name="p322mcpsimp"></a><a name="p322mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row323mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p325mcpsimp"><a name="p325mcpsimp"></a><a name="p325mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p327mcpsimp"><a name="p327mcpsimp"></a><a name="p327mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块。
-   调用该接口前HNR数据流需要保持关闭状态。
-   相同类型的配置文件，如果图像宽高相等时，不能重复加载。
-   支持多个配置文件的加载，或者同一配置文件、不同分辨率多次加载。
-   不支持多进程调用。
-   WDR模式不支持加载抓拍模型。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_unload\_cfg<a name="ZH-CN_TOPIC_0000002441701385"></a>

【描述】

卸载HNR的配置文件。

【语法】

```
td_s32 ss_mpi_hnr_unload_cfg(td_s32 cfg_id);
```

【参数】

<a name="table351mcpsimp"></a>
<table><thead align="left"><tr id="row357mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.4.1.1"><p id="p359mcpsimp"><a name="p359mcpsimp"></a><a name="p359mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.4.1.2"><p id="p361mcpsimp"><a name="p361mcpsimp"></a><a name="p361mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p363mcpsimp"><a name="p363mcpsimp"></a><a name="p363mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row365mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p id="p367mcpsimp"><a name="p367mcpsimp"></a><a name="p367mcpsimp"></a>cfg_id</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.4.1.2 "><p xml:lang="sv-SE" id="p369mcpsimp"><a name="p369mcpsimp"></a><a name="p369mcpsimp"></a>配置文件对应的ID。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p372mcpsimp"><a name="p372mcpsimp"></a><a name="p372mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table374mcpsimp"></a>
<table><thead align="left"><tr id="row379mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p381mcpsimp"><a name="p381mcpsimp"></a><a name="p381mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p383mcpsimp"><a name="p383mcpsimp"></a><a name="p383mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row385mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p387mcpsimp"><a name="p387mcpsimp"></a><a name="p387mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p389mcpsimp"><a name="p389mcpsimp"></a><a name="p389mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row390mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p392mcpsimp"><a name="p392mcpsimp"></a><a name="p392mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p394mcpsimp"><a name="p394mcpsimp"></a><a name="p394mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   应用程序退出前，需调用此接口释放已加载配置文件的内存资源，或者调用[ss\_mpi\_hnr\_exit](#ZH-CN_TOPIC_0000002408262170)接口释放HNR的所有资源。
-   不支持多进程调用。
-   调用此接口前需要关闭HNR数据流开关。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_set\_alg\_cfg<a name="ZH-CN_TOPIC_0000002441661513"></a>

【描述】

设置HNR算法相关配置参数。

【语法】

```
td_s32 ss_mpi_hnr_set_alg_cfg(ot_vi_pipe vi_pipe, const ot_hnr_alg_cfg *cfg)
```

【参数】

<a name="table418mcpsimp"></a>
<table><thead align="left"><tr id="row424mcpsimp"><th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.1"><p id="p426mcpsimp"><a name="p426mcpsimp"></a><a name="p426mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.4.1.2"><p id="p428mcpsimp"><a name="p428mcpsimp"></a><a name="p428mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p430mcpsimp"><a name="p430mcpsimp"></a><a name="p430mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row432mcpsimp"><td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p434mcpsimp"><a name="p434mcpsimp"></a><a name="p434mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.4.1.2 "><p id="p436mcpsimp"><a name="p436mcpsimp"></a><a name="p436mcpsimp"></a>VI模块的PIPE号，</p>
<p id="p437mcpsimp"><a name="p437mcpsimp"></a><a name="p437mcpsimp"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)。详见《MPP 媒体处理软件V5.0 开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p440mcpsimp"><a name="p440mcpsimp"></a><a name="p440mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row441mcpsimp"><td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p443mcpsimp"><a name="p443mcpsimp"></a><a name="p443mcpsimp"></a>cfg</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.4.1.2 "><p id="p445mcpsimp"><a name="p445mcpsimp"></a><a name="p445mcpsimp"></a>HNR算法参数配置结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p447mcpsimp"><a name="p447mcpsimp"></a><a name="p447mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table449mcpsimp"></a>
<table><thead align="left"><tr id="row454mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p456mcpsimp"><a name="p456mcpsimp"></a><a name="p456mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p458mcpsimp"><a name="p458mcpsimp"></a><a name="p458mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row460mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p462mcpsimp"><a name="p462mcpsimp"></a><a name="p462mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p464mcpsimp"><a name="p464mcpsimp"></a><a name="p464mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row465mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p467mcpsimp"><a name="p467mcpsimp"></a><a name="p467mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p469mcpsimp"><a name="p469mcpsimp"></a><a name="p469mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块。
-   调用该接口前需要已创建pipe，并且已初始化ISP。
-   调用该接口时HNR数据流需要保持关闭状态。
-   如果未调用此接口，默认为有参考帧模式。
-   如果是拍照场景，必须调用此接口设置为无参考帧模式。
-   HNR参考帧普通模式支持的最大分辨率为4096×4096；HNR无参考帧模式（OT\_HNR\_REF\_MODE\_NONE和OT\_HNR\_REF\_MODE\_NONE\_ADVANCED）支持的最大分辨率为8192×8192。
-   当前版本HNR 的无参考帧模式（OT\_HNR\_REF\_MODE\_NONE和OT\_HNR\_REF\_MODE\_NONE\_ADVANCED）不支持BGGR和GRBG的Bayer图像的数据格式。
-   WDR模式不支持HNR无参考帧模式（OT\_HNR\_REF\_MODE\_NONE和OT\_HNR\_REF\_MODE\_NONE\_ADVANCED）。
-   OT\_HNR\_REF\_MODE\_NONE\_ADVANCED模式仅支持在VI视频模式为OT\_VI\_VIDEO\_MODE\_ADVANCED时使用。
-   OT\_HNR\_REF\_MODE\_NONE\_ADVANCED模式仅支持送帧方式，推荐使用RunOnce方式。
-   OT\_HNR\_REF\_MODE\_NONE\_ADVANCED模式的输入帧仅支持非压缩格式。
-   如果在非主进程中调用此接口，必须先调用ss\_mpi\_isp\_mem\_share或ss\_mpi\_isp\_mem\_share\_all接口，共享ISP相关MMZ buffer。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_get\_alg\_cfg<a name="ZH-CN_TOPIC_0000002441661465"></a>

【描述】

获取HNR算法相关配置参数。

【语法】

```
td_s32 ss_mpi_hnr_get_alg_cfg(ot_vi_pipe vi_pipe, ot_hnr_alg_cfg *cfg);
```

【参数】

<a name="table494mcpsimp"></a>
<table><thead align="left"><tr id="row500mcpsimp"><th class="cellrowborder" valign="top" width="20%" id="mcps1.1.4.1.1"><p id="p502mcpsimp"><a name="p502mcpsimp"></a><a name="p502mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="64%" id="mcps1.1.4.1.2"><p id="p504mcpsimp"><a name="p504mcpsimp"></a><a name="p504mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p506mcpsimp"><a name="p506mcpsimp"></a><a name="p506mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row508mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p510mcpsimp"><a name="p510mcpsimp"></a><a name="p510mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.4.1.2 "><p id="p512mcpsimp"><a name="p512mcpsimp"></a><a name="p512mcpsimp"></a>VI模块的PIPE号。</p>
<p id="p513mcpsimp"><a name="p513mcpsimp"></a><a name="p513mcpsimp"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)。详见《MPP 媒体处理软件V5.0 开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p516mcpsimp"><a name="p516mcpsimp"></a><a name="p516mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row517mcpsimp"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p519mcpsimp"><a name="p519mcpsimp"></a><a name="p519mcpsimp"></a>cfg</p>
</td>
<td class="cellrowborder" valign="top" width="64%" headers="mcps1.1.4.1.2 "><p id="p521mcpsimp"><a name="p521mcpsimp"></a><a name="p521mcpsimp"></a>HNR算法参数配置结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p523mcpsimp"><a name="p523mcpsimp"></a><a name="p523mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table525mcpsimp"></a>
<table><thead align="left"><tr id="row530mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p532mcpsimp"><a name="p532mcpsimp"></a><a name="p532mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p534mcpsimp"><a name="p534mcpsimp"></a><a name="p534mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row536mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p538mcpsimp"><a name="p538mcpsimp"></a><a name="p538mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p540mcpsimp"><a name="p540mcpsimp"></a><a name="p540mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row541mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p543mcpsimp"><a name="p543mcpsimp"></a><a name="p543mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p545mcpsimp"><a name="p545mcpsimp"></a><a name="p545mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块。
-   调用该接口前需要已创建pipe，并且已初始化ISP。
-   如果在非主进程中调用此接口，必须先调用ss\_mpi\_isp\_mem\_share或ss\_mpi\_isp\_mem\_share\_all接口，共享ISP相关MMZ buffer。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_enable<a name="ZH-CN_TOPIC_0000002408102158"></a>

【描述】

使能HNR数据流开关。

【语法】

```
td_s32 ss_mpi_hnr_enable(ot_vi_pipe vi_pipe);
```

【参数】

<a name="table563mcpsimp"></a>
<table><thead align="left"><tr id="row569mcpsimp"><th class="cellrowborder" valign="top" width="27%" id="mcps1.1.4.1.1"><p id="p571mcpsimp"><a name="p571mcpsimp"></a><a name="p571mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="56.99999999999999%" id="mcps1.1.4.1.2"><p id="p573mcpsimp"><a name="p573mcpsimp"></a><a name="p573mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p575mcpsimp"><a name="p575mcpsimp"></a><a name="p575mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row577mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p579mcpsimp"><a name="p579mcpsimp"></a><a name="p579mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.4.1.2 "><p id="p581mcpsimp"><a name="p581mcpsimp"></a><a name="p581mcpsimp"></a>VI模块的PIPE号。</p>
<p id="p582mcpsimp"><a name="p582mcpsimp"></a><a name="p582mcpsimp"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)，详见《MPP 媒体处理软件V5.0 开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p585mcpsimp"><a name="p585mcpsimp"></a><a name="p585mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table587mcpsimp"></a>
<table><thead align="left"><tr id="row592mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p594mcpsimp"><a name="p594mcpsimp"></a><a name="p594mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p596mcpsimp"><a name="p596mcpsimp"></a><a name="p596mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row598mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p600mcpsimp"><a name="p600mcpsimp"></a><a name="p600mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p602mcpsimp"><a name="p602mcpsimp"></a><a name="p602mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row603mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p605mcpsimp"><a name="p605mcpsimp"></a><a name="p605mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p607mcpsimp"><a name="p607mcpsimp"></a><a name="p607mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   主进程调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块，并调用[ss\_mpi\_hnr\_load\_cfg](#ZH-CN_TOPIC_0000002441701369)接口加载HNR配置文件。
-   其他非主进程（例如PQTools）支持使用该接口打开HNR数据流，无需初始化HNR；但需要主进程初始化HNR模块，否则无法实际打开数据流。
-   如果在非主进程中调用此接口，必须先调用ss\_mpi\_isp\_mem\_share或ss\_mpi\_isp\_mem\_share\_all接口，共享ISP相关MMZ buffer。
-   enable hnr前有如下限制条件：
    -   对应的pipe必须已经创建。
    -   需要用户调用函数去分配VB池，VB大小计算使用函数ot\_hnr\_get\_pic\_buf\_size，详见《MPP 媒体处理软件V5.0 开发参考》“系统控制”章节，且分配的VB池需要attach对应的PIPE上。

-   如果更改了输入Bayer图像的数据格式，需要[ss\_mpi\_hnr\_disable](#ZH-CN_TOPIC_0000002408102186)关闭之后，在重新调用本接口使能HNR数据流才会生效。
-   HNR使能之后，有参考帧模式延迟4帧生效，无参考帧模式延迟2帧生效。
-   HNR仅支持12bit的Bayer图像数据处理。如果sensor输入非12bit的Bayer图像，需设置VI PIPE输出的图像数据格式为Bayer 12bit。
-   HNR的线性模式使用有如下限制：

    如果另一路使用WDR模式，则VI视频模式需要设置为OT\_VI\_VIDEO\_MODE\_NORM模式，否则会影响WDR路的BNR效果；

-   HNR的WDR模式使用有如下限制：
    -   只能在VI离线时生效；
    -   VI视频模式必须设置为OT\_VI\_VIDEO\_MODE\_NORM模式；
    -   HNR建议绑定长帧所在的pipe，不能是虚拟pipe；
    -   pipe通路不能开压缩；
    -   当前版本HNR 的WDR模式不支持BGGR和GRBG的Bayer图像的数据格式；
    -   不支持normal\_blend模式；
    -   超性能场景可能会出现WDR长短帧丢帧不同步，进而导致WDR融合效果异常。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_disable<a name="ZH-CN_TOPIC_0000002408102186"></a>

【描述】

关闭HNR数据流开关。

【语法】

```
td_s32 ss_mpi_hnr_disable(ot_vi_pipe vi_pipe);
```

【参数】

<a name="table651mcpsimp"></a>
<table><thead align="left"><tr id="row657mcpsimp"><th class="cellrowborder" valign="top" width="27%" id="mcps1.1.4.1.1"><p id="p659mcpsimp"><a name="p659mcpsimp"></a><a name="p659mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="56.99999999999999%" id="mcps1.1.4.1.2"><p id="p661mcpsimp"><a name="p661mcpsimp"></a><a name="p661mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p663mcpsimp"><a name="p663mcpsimp"></a><a name="p663mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row665mcpsimp"><td class="cellrowborder" valign="top" width="27%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p667mcpsimp"><a name="p667mcpsimp"></a><a name="p667mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="56.99999999999999%" headers="mcps1.1.4.1.2 "><p id="p669mcpsimp"><a name="p669mcpsimp"></a><a name="p669mcpsimp"></a>VI模块的PIPE号。</p>
<p id="p670mcpsimp"><a name="p670mcpsimp"></a><a name="p670mcpsimp"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)。详见《MPP 媒体处理软件V5.0 开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p673mcpsimp"><a name="p673mcpsimp"></a><a name="p673mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table675mcpsimp"></a>
<table><thead align="left"><tr id="row680mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p682mcpsimp"><a name="p682mcpsimp"></a><a name="p682mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p684mcpsimp"><a name="p684mcpsimp"></a><a name="p684mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row686mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p688mcpsimp"><a name="p688mcpsimp"></a><a name="p688mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p690mcpsimp"><a name="p690mcpsimp"></a><a name="p690mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row691mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p693mcpsimp"><a name="p693mcpsimp"></a><a name="p693mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p695mcpsimp"><a name="p695mcpsimp"></a><a name="p695mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   主进程调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块，并调用[ss\_mpi\_hnr\_load\_cfg](#ZH-CN_TOPIC_0000002441701369)接口加载HNR配置文件。
-   其他非主进程（例如PQTools）支持使用该接口关闭HNR数据流，无需初始化HNR。
-   如果在非主进程中调用此接口，必须先调用ss\_mpi\_isp\_mem\_share或ss\_mpi\_isp\_mem\_share\_all接口，共享ISP相关MMZ buffer。
-   在VI模块销毁对应的pipe之前，需要先调用该接口关闭HNR功能，否则可能出现VB泄漏。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_set\_attr<a name="ZH-CN_TOPIC_0000002441661457"></a>

【描述】

设置HNR的属性。

【语法】

```
td_s32 ss_mpi_hnr_set_attr(ot_vi_pipe vi_pipe, const ot_hnr_attr *attr);
```

【参数】

<a name="table719mcpsimp"></a>
<table><thead align="left"><tr id="row725mcpsimp"><th class="cellrowborder" valign="top" width="21%" id="mcps1.1.4.1.1"><p id="p727mcpsimp"><a name="p727mcpsimp"></a><a name="p727mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p729mcpsimp"><a name="p729mcpsimp"></a><a name="p729mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p731mcpsimp"><a name="p731mcpsimp"></a><a name="p731mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row733mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p735mcpsimp"><a name="p735mcpsimp"></a><a name="p735mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p737mcpsimp"><a name="p737mcpsimp"></a><a name="p737mcpsimp"></a>VI模块的PIPE号，</p>
<p id="p738mcpsimp"><a name="p738mcpsimp"></a><a name="p738mcpsimp"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)。详见《MPP 媒体处理软件V5.0 开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p741mcpsimp"><a name="p741mcpsimp"></a><a name="p741mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row742mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p744mcpsimp"><a name="p744mcpsimp"></a><a name="p744mcpsimp"></a>attr</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p746mcpsimp"><a name="p746mcpsimp"></a><a name="p746mcpsimp"></a>HNR属性结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p748mcpsimp"><a name="p748mcpsimp"></a><a name="p748mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table750mcpsimp"></a>
<table><thead align="left"><tr id="row755mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p757mcpsimp"><a name="p757mcpsimp"></a><a name="p757mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p759mcpsimp"><a name="p759mcpsimp"></a><a name="p759mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row761mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p763mcpsimp"><a name="p763mcpsimp"></a><a name="p763mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p765mcpsimp"><a name="p765mcpsimp"></a><a name="p765mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row766mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p768mcpsimp"><a name="p768mcpsimp"></a><a name="p768mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p770mcpsimp"><a name="p770mcpsimp"></a><a name="p770mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块，并调用[ss\_mpi\_hnr\_load\_cfg](#ZH-CN_TOPIC_0000002441701369)接口加载HNR配置文件。
-   使能HNR数据流开关之后才能调用此接口成功设置HNR属性。
-   VI视频模式为OT\_VI\_VIDEO\_MODE\_NORM时，当attr-\>enable为TD\_FALSE时，bnr\_bypass必须为TD\_FALSE。
-   VI视频模式为OT\_VI\_VIDEO\_MODE\_ADVANCED时，bnr\_bypass不起作用。
-   VI视频模式为OT\_VI\_VIDEO\_MODE\_NORM时，并且bnr\_bypass为TD\_FALSE时，才能开启normal\_blend。
-   VI视频模式为OT\_VI\_VIDEO\_MODE\_NORM时，normal\_blend开启，不支持获取HNR处理后的raw，不支持ISP RunOnce方式使用HNR，因为BE灌的raw没有经过HNR处理。
-   VI视频模式为OT\_VI\_VIDEO\_MODE\_NORM时，normal\_blend关闭，支持获取HNR处理后的raw, 支持ISP RunOnce方式使用HNR。
-   如果在非主进程中调用此接口，必须先调用ss\_mpi\_isp\_mem\_share或ss\_mpi\_isp\_mem\_share\_all接口，共享ISP相关MMZ buffer。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_get\_attr<a name="ZH-CN_TOPIC_0000002408262070"></a>

【描述】

获取HNR的属性。

【语法】

```
td_s32 ss_mpi_hnr_get_attr(ot_vi_pipe vi_pipe, ot_hnr_attr *attr);
```

【参数】

<a name="table796mcpsimp"></a>
<table><thead align="left"><tr id="row802mcpsimp"><th class="cellrowborder" valign="top" width="14.000000000000002%" id="mcps1.1.4.1.1"><p id="p804mcpsimp"><a name="p804mcpsimp"></a><a name="p804mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.4.1.2"><p id="p806mcpsimp"><a name="p806mcpsimp"></a><a name="p806mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p808mcpsimp"><a name="p808mcpsimp"></a><a name="p808mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row810mcpsimp"><td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p812mcpsimp"><a name="p812mcpsimp"></a><a name="p812mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.4.1.2 "><p id="p814mcpsimp"><a name="p814mcpsimp"></a><a name="p814mcpsimp"></a>VI模块的PIPE号，</p>
<p id="p815mcpsimp"><a name="p815mcpsimp"></a><a name="p815mcpsimp"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)。详见《MPP 媒体处理软件V5.0 开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p818mcpsimp"><a name="p818mcpsimp"></a><a name="p818mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row819mcpsimp"><td class="cellrowborder" valign="top" width="14.000000000000002%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p821mcpsimp"><a name="p821mcpsimp"></a><a name="p821mcpsimp"></a>attr</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.4.1.2 "><p id="p823mcpsimp"><a name="p823mcpsimp"></a><a name="p823mcpsimp"></a>HNR属性结构体指针。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p825mcpsimp"><a name="p825mcpsimp"></a><a name="p825mcpsimp"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table827mcpsimp"></a>
<table><thead align="left"><tr id="row832mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p834mcpsimp"><a name="p834mcpsimp"></a><a name="p834mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p836mcpsimp"><a name="p836mcpsimp"></a><a name="p836mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row838mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p840mcpsimp"><a name="p840mcpsimp"></a><a name="p840mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p842mcpsimp"><a name="p842mcpsimp"></a><a name="p842mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row843mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p845mcpsimp"><a name="p845mcpsimp"></a><a name="p845mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p847mcpsimp"><a name="p847mcpsimp"></a><a name="p847mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   使能HNR数据流开关之后才能调用此接口成功获取HNR属性。
-   如果在非主进程中调用此接口，必须先调用ss\_mpi\_isp\_mem\_share或ss\_mpi\_isp\_mem\_share\_all接口，共享ISP相关MMZ buffer。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_set\_input\_depth<a name="ZH-CN_TOPIC_0000002441661493"></a>

【描述】

设置HNR输入队列的缓存深度。

【语法】

```
td_s32 ss_mpi_hnr_set_input_depth(ot_vi_pipe vi_pipe, td_u32 depth);
```

【参数】

<a name="table865mcpsimp"></a>
<table><thead align="left"><tr id="row871mcpsimp"><th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.1"><p id="p873mcpsimp"><a name="p873mcpsimp"></a><a name="p873mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="68%" id="mcps1.1.4.1.2"><p id="p875mcpsimp"><a name="p875mcpsimp"></a><a name="p875mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p877mcpsimp"><a name="p877mcpsimp"></a><a name="p877mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row879mcpsimp"><td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p881mcpsimp"><a name="p881mcpsimp"></a><a name="p881mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.4.1.2 "><p id="p883mcpsimp"><a name="p883mcpsimp"></a><a name="p883mcpsimp"></a>VI模块的PIPE号。</p>
<p id="p884mcpsimp"><a name="p884mcpsimp"></a><a name="p884mcpsimp"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)。详见《MPP媒体处理软件V5.0开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p887mcpsimp"><a name="p887mcpsimp"></a><a name="p887mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row888mcpsimp"><td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p890mcpsimp"><a name="p890mcpsimp"></a><a name="p890mcpsimp"></a>depth</p>
</td>
<td class="cellrowborder" valign="top" width="68%" headers="mcps1.1.4.1.2 "><p id="p892mcpsimp"><a name="p892mcpsimp"></a><a name="p892mcpsimp"></a>输入缓存深度，取值范围：[1, 20]</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p894mcpsimp"><a name="p894mcpsimp"></a><a name="p894mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table896mcpsimp"></a>
<table><thead align="left"><tr id="row901mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p903mcpsimp"><a name="p903mcpsimp"></a><a name="p903mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p905mcpsimp"><a name="p905mcpsimp"></a><a name="p905mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row907mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p909mcpsimp"><a name="p909mcpsimp"></a><a name="p909mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p911mcpsimp"><a name="p911mcpsimp"></a><a name="p911mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row912mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p914mcpsimp"><a name="p914mcpsimp"></a><a name="p914mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p916mcpsimp"><a name="p916mcpsimp"></a><a name="p916mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块，并且对应的pipe必须已经创建。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_set\_thread\_attr<a name="ZH-CN_TOPIC_0000002441661481"></a>

【描述】

设置HNR线程属性。

【语法】

```
td_s32 ss_mpi_hnr_set_thread_attr(const ot_hnr_thread_attr *thread_attr);
```

【参数】

<a name="table719mcpsimp"></a>
<table><thead align="left"><tr id="row725mcpsimp"><th class="cellrowborder" valign="top" width="21%" id="mcps1.1.4.1.1"><p id="p727mcpsimp"><a name="p727mcpsimp"></a><a name="p727mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p729mcpsimp"><a name="p729mcpsimp"></a><a name="p729mcpsimp"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p731mcpsimp"><a name="p731mcpsimp"></a><a name="p731mcpsimp"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row742mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p744mcpsimp"><a name="p744mcpsimp"></a><a name="p744mcpsimp"></a>thread_attr</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p746mcpsimp"><a name="p746mcpsimp"></a><a name="p746mcpsimp"></a>HNR线程属性。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p748mcpsimp"><a name="p748mcpsimp"></a><a name="p748mcpsimp"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table750mcpsimp"></a>
<table><thead align="left"><tr id="row755mcpsimp"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p757mcpsimp"><a name="p757mcpsimp"></a><a name="p757mcpsimp"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p759mcpsimp"><a name="p759mcpsimp"></a><a name="p759mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row761mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p763mcpsimp"><a name="p763mcpsimp"></a><a name="p763mcpsimp"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p765mcpsimp"><a name="p765mcpsimp"></a><a name="p765mcpsimp"></a>成功。</p>
</td>
</tr>
<tr id="row766mcpsimp"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p768mcpsimp"><a name="p768mcpsimp"></a><a name="p768mcpsimp"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p770mcpsimp"><a name="p770mcpsimp"></a><a name="p770mcpsimp"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

调用该接口支持在调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块前或开启HNR通路后动态修改。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_get\_thread\_attr<a name="ZH-CN_TOPIC_0000002408262054"></a>

【描述】

获取HNR线程属性。

【语法】

```
td_s32 ss_mpi_hnr_get_thread_attr(const ot_hnr_thread_attr *thread_attr);
```

【参数】

<a name="table1578315475611"></a>
<table><thead align="left"><tr id="row8783114718618"><th class="cellrowborder" valign="top" width="21%" id="mcps1.1.4.1.1"><p id="p7784144713611"><a name="p7784144713611"></a><a name="p7784144713611"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p3784184719613"><a name="p3784184719613"></a><a name="p3784184719613"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p8784947961"><a name="p8784947961"></a><a name="p8784947961"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row13784647564"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p177843471469"><a name="p177843471469"></a><a name="p177843471469"></a>thread_attr</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p57849476611"><a name="p57849476611"></a><a name="p57849476611"></a>HNR线程属性。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p278415471565"><a name="p278415471565"></a><a name="p278415471565"></a>输出</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table97841347266"></a>
<table><thead align="left"><tr id="row4784647763"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p378419471264"><a name="p378419471264"></a><a name="p378419471264"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p4784047561"><a name="p4784047561"></a><a name="p4784047561"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row177842474614"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p27841647861"><a name="p27841647861"></a><a name="p27841647861"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p1578410473614"><a name="p1578410473614"></a><a name="p1578410473614"></a>成功。</p>
</td>
</tr>
<tr id="row57845471368"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p207845470614"><a name="p207845470614"></a><a name="p207845470614"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p77841847464"><a name="p77841847464"></a><a name="p77841847464"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

无

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_attach\_out\_vb\_pool<a name="ZH-CN_TOPIC_0000002408262098"></a>

【描述】

将HNR输出绑定到某个视频缓存VB池。

【语法】

```
td_s32 ss_mpi_hnr_attach_out_vb_pool(ot_vi_pipe vi_pipe, ot_vb_pool vb_pool);
```

【参数】

<a name="table1010413506202"></a>
<table><thead align="left"><tr id="row3104850192012"><th class="cellrowborder" valign="top" width="21%" id="mcps1.1.4.1.1"><p id="p810445072017"><a name="p810445072017"></a><a name="p810445072017"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p191041250122018"><a name="p191041250122018"></a><a name="p191041250122018"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p7104115082014"><a name="p7104115082014"></a><a name="p7104115082014"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row1610435013205"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p881mcpsimp"><a name="p881mcpsimp"></a><a name="p881mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p883mcpsimp"><a name="p883mcpsimp"></a><a name="p883mcpsimp"></a>VI模块的PIPE号。</p>
<p id="p884mcpsimp"><a name="p884mcpsimp"></a><a name="p884mcpsimp"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)。详见《MPP媒体处理软件V5.0开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p887mcpsimp"><a name="p887mcpsimp"></a><a name="p887mcpsimp"></a>输入</p>
</td>
</tr>
<tr id="row11168919162613"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.4.1.1 "><p id="p5168619152615"><a name="p5168619152615"></a><a name="p5168619152615"></a>vb_pool</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p116861915261"><a name="p116861915261"></a><a name="p116861915261"></a>视频缓存VB池信息。具体请参见“系统控制”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p81681019122617"><a name="p81681019122617"></a><a name="p81681019122617"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table1810575010201"></a>
<table><thead align="left"><tr id="row13105205052018"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p1510595052016"><a name="p1510595052016"></a><a name="p1510595052016"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p710535022013"><a name="p710535022013"></a><a name="p710535022013"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row141051550132017"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p19105205022019"><a name="p19105205022019"></a><a name="p19105205022019"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p910525022014"><a name="p910525022014"></a><a name="p910525022014"></a>成功。</p>
</td>
</tr>
<tr id="row910575013204"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p171056503201"><a name="p171056503201"></a><a name="p171056503201"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p1105350112018"><a name="p1105350112018"></a><a name="p1105350112018"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块。
-   调用该接口前HNR数据流需要保持关闭状态。
-   只推荐在VI stagger输入场景且VI视频模式为OT\_VI\_VIDEO\_MODE\_NORM时使用；其他场景使用会造成vb利用率下降。

【举例】

无

【相关主题】

无

## ss\_mpi\_hnr\_detach\_out\_vb\_pool<a name="ZH-CN_TOPIC_0000002441701317"></a>

【描述】

将HNR输出与某个视频缓存VB池解绑定。

【语法】

```
td_s32 ss_mpi_hnr_detach_out_vb_pool(ot_vi_pipe vi_pipe);
```

【参数】

<a name="table14616149162819"></a>
<table><thead align="left"><tr id="row86161449132817"><th class="cellrowborder" valign="top" width="21%" id="mcps1.1.4.1.1"><p id="p761615496286"><a name="p761615496286"></a><a name="p761615496286"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.4.1.2"><p id="p56162492284"><a name="p56162492284"></a><a name="p56162492284"></a>描述</p>
</th>
<th class="cellrowborder" valign="top" width="16%" id="mcps1.1.4.1.3"><p id="p20617164911286"><a name="p20617164911286"></a><a name="p20617164911286"></a>输入/输出</p>
</th>
</tr>
</thead>
<tbody><tr id="row2617174918282"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.1.4.1.1 "><p xml:lang="sv-SE" id="p16176495283"><a name="p16176495283"></a><a name="p16176495283"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.4.1.2 "><p id="p186175498281"><a name="p186175498281"></a><a name="p186175498281"></a>VI模块的PIPE号。</p>
<p id="p12617849142812"><a name="p12617849142812"></a><a name="p12617849142812"></a>取值范围：[0, OT_VI_MAX_PIPE_NUM)。详见《MPP媒体处理软件V5.0开发参考》“视频输入”章节。</p>
</td>
<td class="cellrowborder" valign="top" width="16%" headers="mcps1.1.4.1.3 "><p id="p9617149122813"><a name="p9617149122813"></a><a name="p9617149122813"></a>输入</p>
</td>
</tr>
</tbody>
</table>

【返回值】

<a name="table461744915286"></a>
<table><thead align="left"><tr id="row13617154902818"><th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.1"><p id="p16617449152814"><a name="p16617449152814"></a><a name="p16617449152814"></a>返回值</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.1.3.1.2"><p id="p361784992819"><a name="p361784992819"></a><a name="p361784992819"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1661774912811"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p11617104912813"><a name="p11617104912813"></a><a name="p11617104912813"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p3617144902818"><a name="p3617144902818"></a><a name="p3617144902818"></a>成功。</p>
</td>
</tr>
<tr id="row361724918281"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.1 "><p id="p8617154915280"><a name="p8617154915280"></a><a name="p8617154915280"></a>非0</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.1.3.1.2 "><p id="p196171249172810"><a name="p196171249172810"></a><a name="p196171249172810"></a>失败，请参见<a href="#ZH-CN_TOPIC_0000002441701421">错误码</a>。</p>
</td>
</tr>
</tbody>
</table>

【需求】

-   头文件：ss\_mpi\_hnr.h
-   库文件：libss\_hnr.a

【注意】

-   调用该接口前必须先调用[ss\_mpi\_hnr\_init](#ZH-CN_TOPIC_0000002441661525)初始化HNR模块。
-   调用该接口前HNR数据流需要保持关闭状态。

【举例】

无

【相关主题】

无

# 数据类型<a name="ZH-CN_TOPIC_0000002441701341"></a>

HNR模块相关数据类型定义如下：

-   [OT\_HNR\_MAX\_CFG\_NUM](#ZH-CN_TOPIC_0000002408262082)：定义HNR配置文件最大支持加载数。
-   [ot\_hnr\_mem\_info](#ZH-CN_TOPIC_0000002441701333)：定义HNR配置文件内存信息。
-   [ot\_hnr\_cfg](#ZH-CN_TOPIC_0000002408102170)：定义HNR配置文件信息。
-   [ot\_hnr\_param](#ZH-CN_TOPIC_0000002441701305)：定义HNR图像效果参数。
-   [ot\_hnr\_manual\_attr](#ZH-CN_TOPIC_0000002408102146)：定义HNR手动参数。
-   [ot\_hnr\_attr](#ZH-CN_TOPIC_0000002408102266)：定义HNR属性。
-   [ot\_hnr\_ref\_mode](#ZH-CN_TOPIC_0000002408102250)：定义HNR参考帧模式。
-   [ot\_hnr\_alg\_cfg](#ZH-CN_TOPIC_0000002441701357)：定义HNR算法配置参数。
-   [ot\_hnr\_thread\_attr](#ZH-CN_TOPIC_0000002441661533)：定义HNR线程配置参数。

## OT\_HNR\_MAX\_CFG\_NUM<a name="ZH-CN_TOPIC_0000002408262082"></a>

【说明】

定义HNR配置文件最大支持加载数。

【定义】

```
#define OT_HNR_MAX_CFG_NUM  32
```

【注意事项】

无

【相关数据类型及接口】

无

## ot\_hnr\_mem\_info<a name="ZH-CN_TOPIC_0000002441701333"></a>

【说明】

HNR配置文件内存信息。

【定义】

```
typedef struct {
    td_phys_addr_t phys_addr;
    td_void *virt_addr;
    td_u32  size;
} ot_hnr_mem_info;
```

【成员】

<a name="table976mcpsimp"></a>
<table><thead align="left"><tr id="row981mcpsimp"><th class="cellrowborder" valign="top" width="37%" id="mcps1.1.3.1.1"><p id="p983mcpsimp"><a name="p983mcpsimp"></a><a name="p983mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.3.1.2"><p id="p985mcpsimp"><a name="p985mcpsimp"></a><a name="p985mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row987mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p989mcpsimp"><a name="p989mcpsimp"></a><a name="p989mcpsimp"></a>phys_addr</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p xml:lang="de-DE" id="p991mcpsimp"><a name="p991mcpsimp"></a><a name="p991mcpsimp"></a>HNR配置文件的内存物理地址。</p>
<p id="p992mcpsimp"><a name="p992mcpsimp"></a><a name="p992mcpsimp"></a>td_phys_addr_t详细请参考《MPP 媒体处理软件 V5.0 开发参考》“系统控制”章节。</p>
</td>
</tr>
<tr id="row994mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p996mcpsimp"><a name="p996mcpsimp"></a><a name="p996mcpsimp"></a>virt_addr</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p xml:lang="de-DE" id="p998mcpsimp"><a name="p998mcpsimp"></a><a name="p998mcpsimp"></a>HNR配置文件的内存虚拟地址。</p>
</td>
</tr>
<tr id="row999mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1001mcpsimp"><a name="p1001mcpsimp"></a><a name="p1001mcpsimp"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p xml:lang="de-DE" id="p1003mcpsimp"><a name="p1003mcpsimp"></a><a name="p1003mcpsimp"></a>HNR配置文件的内存大小。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

[ot\_hnr\_cfg](#ZH-CN_TOPIC_0000002408102170)

## ot\_hnr\_cfg<a name="ZH-CN_TOPIC_0000002408102170"></a>

【说明】

HNR配置文件信息。

【定义】

```
typedef struct {
    ot_hnr_mem_info mem_info;
    ot_size image_size;
    td_bool is_wdr_mode;
} ot_hnr_cfg;
```

【成员】

<a name="table1020mcpsimp"></a>
<table><thead align="left"><tr id="row1025mcpsimp"><th class="cellrowborder" valign="top" width="30%" id="mcps1.1.3.1.1"><p id="p1027mcpsimp"><a name="p1027mcpsimp"></a><a name="p1027mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="70%" id="mcps1.1.3.1.2"><p id="p1029mcpsimp"><a name="p1029mcpsimp"></a><a name="p1029mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1031mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1033mcpsimp"><a name="p1033mcpsimp"></a><a name="p1033mcpsimp"></a>mem_info</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p id="p1035mcpsimp"><a name="p1035mcpsimp"></a><a name="p1035mcpsimp"></a>HNR配置文件内存信息<span xml:lang="de-DE" id="ph1036mcpsimp"><a name="ph1036mcpsimp"></a><a name="ph1036mcpsimp"></a>。</span></p>
</td>
</tr>
<tr id="row1037mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1039mcpsimp"><a name="p1039mcpsimp"></a><a name="p1039mcpsimp"></a>image_size</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p xml:lang="de-DE" id="p1041mcpsimp"><a name="p1041mcpsimp"></a><a name="p1041mcpsimp"></a>配置文件所支持的图像分辨率。</p>
</td>
</tr>
<tr id="row1042mcpsimp"><td class="cellrowborder" valign="top" width="30%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1044mcpsimp"><a name="p1044mcpsimp"></a><a name="p1044mcpsimp"></a>is_wdr_mode</p>
</td>
<td class="cellrowborder" valign="top" width="70%" headers="mcps1.1.3.1.2 "><p xml:lang="de-DE" id="p1046mcpsimp"><a name="p1046mcpsimp"></a><a name="p1046mcpsimp"></a>配置文件是否支持WDR模式，否则支持线性模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

-   用户需要保证内存中所存储的配置文件信息的完整性和正确性，否则可能导致运行异常。
-   配置文件加载时需指定所支持的图像分辨率。
-   支持的图像分辨率宽高都必须是4的整数倍。
-   HNR参考帧模式支持的图像分辨率宽高与VI PIPE离线图像大小范围一致。详细请参考《MPP 媒体处理软件 V5.0 开发参考》“视频输入”章节。

【相关数据类型及接口】

无

## ot\_hnr\_param<a name="ZH-CN_TOPIC_0000002441701305"></a>

【说明】

定义HNR图像效果参数。

【定义】

```
typedef struct {
    td_u32 sfs;
    td_u32 tfs;
} ot_hnr_param;
```

【成员】

<a name="table1064mcpsimp"></a>
<table><thead align="left"><tr id="row1069mcpsimp"><th class="cellrowborder" valign="top" width="37%" id="mcps1.1.3.1.1"><p id="p1071mcpsimp"><a name="p1071mcpsimp"></a><a name="p1071mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.3.1.2"><p id="p1073mcpsimp"><a name="p1073mcpsimp"></a><a name="p1073mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1075mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1077mcpsimp"><a name="p1077mcpsimp"></a><a name="p1077mcpsimp"></a>sfs</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p xml:lang="de-DE" id="p1079mcpsimp"><a name="p1079mcpsimp"></a><a name="p1079mcpsimp"></a>整体画面的空域去噪强度。数值越大，去噪越强。</p>
<p xml:lang="sv-SE" id="p1080mcpsimp"><a name="p1080mcpsimp"></a><a name="p1080mcpsimp"></a>取值范围: [0, 31]</p>
</td>
</tr>
<tr id="row1081mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1083mcpsimp"><a name="p1083mcpsimp"></a><a name="p1083mcpsimp"></a>tfs</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p xml:lang="de-DE" id="p1085mcpsimp"><a name="p1085mcpsimp"></a><a name="p1085mcpsimp"></a>时域去噪强度。数值越大，时域去噪越强。此版本暂不支持此参数的效果调节。</p>
<p xml:lang="sv-SE" id="p1086mcpsimp"><a name="p1086mcpsimp"></a><a name="p1086mcpsimp"></a>取值范围: [0, 31]</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

当前版本，tfs参数暂未生效。参数调节请参考《HNR调优指南》。

【相关数据类型及接口】

[ot\_hnr\_manual\_attr](#ZH-CN_TOPIC_0000002408102146)

## ot\_hnr\_manual\_attr<a name="ZH-CN_TOPIC_0000002408102146"></a>

【说明】

定义HNR手动参数。

【定义】

```
typedef struct {
    ot_hnr_param param;
} ot_hnr_manual_attr;
```

【成员】

<a name="table1102mcpsimp"></a>
<table><thead align="left"><tr id="row1107mcpsimp"><th class="cellrowborder" valign="top" width="37%" id="mcps1.1.3.1.1"><p id="p1109mcpsimp"><a name="p1109mcpsimp"></a><a name="p1109mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.3.1.2"><p id="p1111mcpsimp"><a name="p1111mcpsimp"></a><a name="p1111mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1113mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1115mcpsimp"><a name="p1115mcpsimp"></a><a name="p1115mcpsimp"></a>param</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p1117mcpsimp"><a name="p1117mcpsimp"></a><a name="p1117mcpsimp"></a>HNR图像效果参数<span xml:lang="de-DE" id="ph1118mcpsimp"><a name="ph1118mcpsimp"></a><a name="ph1118mcpsimp"></a>。</span></p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

[ot\_hnr\_attr](#ZH-CN_TOPIC_0000002408102266)

## ot\_hnr\_attr<a name="ZH-CN_TOPIC_0000002408102266"></a>

【说明】

定义HNR属性。

【定义】

```
typedef struct {
    td_bool enable;
    td_bool bnr_bypass;
    td_bool normal_blend;
    ot_op_mode op_type;
    ot_hnr_manual_attr manual_attr;
    ot_hnr_auto_attr auto_attr;
} ot_hnr_attr;
```

【成员】

<a name="table1138mcpsimp"></a>
<table><thead align="left"><tr id="row1143mcpsimp"><th class="cellrowborder" valign="top" width="37%" id="mcps1.1.3.1.1"><p id="p1145mcpsimp"><a name="p1145mcpsimp"></a><a name="p1145mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="63%" id="mcps1.1.3.1.2"><p id="p1147mcpsimp"><a name="p1147mcpsimp"></a><a name="p1147mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1149mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1151mcpsimp"><a name="p1151mcpsimp"></a><a name="p1151mcpsimp"></a>enable</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p1153mcpsimp"><a name="p1153mcpsimp"></a><a name="p1153mcpsimp"></a>图像效果使能开关。</p>
</td>
</tr>
<tr id="row1154mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1156mcpsimp"><a name="p1156mcpsimp"></a><a name="p1156mcpsimp"></a>bnr_bypass</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p1158mcpsimp"><a name="p1158mcpsimp"></a><a name="p1158mcpsimp"></a>BNR Bypass开关。详细请参考《HNR 调试指南》HNR与BNR切换。</p>
</td>
</tr>
<tr id="row1796684721119"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p id="p1496617478119"><a name="p1496617478119"></a><a name="p1496617478119"></a>normal_blend</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p189661447151113"><a name="p189661447151113"></a><a name="p189661447151113"></a>normal模式blend开关。</p>
</td>
</tr>
<tr id="row1159mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1161mcpsimp"><a name="p1161mcpsimp"></a><a name="p1161mcpsimp"></a>op_type</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p1163mcpsimp"><a name="p1163mcpsimp"></a><a name="p1163mcpsimp"></a>HNR选择模式。详细请参考《MPP 媒体处理软件 V5.0 开发参考》“系统控制”章节。</p>
</td>
</tr>
<tr id="row1164mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1166mcpsimp"><a name="p1166mcpsimp"></a><a name="p1166mcpsimp"></a>manual_attr</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p1168mcpsimp"><a name="p1168mcpsimp"></a><a name="p1168mcpsimp"></a>HNR手动模式参数。</p>
</td>
</tr>
<tr id="row1169mcpsimp"><td class="cellrowborder" valign="top" width="37%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1171mcpsimp"><a name="p1171mcpsimp"></a><a name="p1171mcpsimp"></a>auto_attr</p>
</td>
<td class="cellrowborder" valign="top" width="63%" headers="mcps1.1.3.1.2 "><p id="p1173mcpsimp"><a name="p1173mcpsimp"></a><a name="p1173mcpsimp"></a>HNR自动模式的标准参数。</p>
<p id="p1174mcpsimp"><a name="p1174mcpsimp"></a><a name="p1174mcpsimp"></a>ot_hnr_auto_attr参数暂不支持。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

本版本暂只支持手动模式参数，不支持自动模式参数配置。

【相关数据类型及接口】

无

## ot\_hnr\_ref\_mode<a name="ZH-CN_TOPIC_0000002408102250"></a>

【说明】

定义HNR参考帧模式。

【定义】

```
typedef enum {
    OT_HNR_REF_MODE_NORM = 0,
    OT_HNR_REF_MODE_NONE,
    OT_HNR_REF_MODE_NONE_ADVANCED,
    OT_HNR_REF_MODE_NORM_FACE,
    OT_HNR_REF_MODE_BUTT
} ot_hnr_ref_mode;
```

【成员】

<a name="table1190mcpsimp"></a>
<table><thead align="left"><tr id="row1195mcpsimp"><th class="cellrowborder" valign="top" width="45%" id="mcps1.1.3.1.1"><p id="p1197mcpsimp"><a name="p1197mcpsimp"></a><a name="p1197mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="55.00000000000001%" id="mcps1.1.3.1.2"><p id="p1199mcpsimp"><a name="p1199mcpsimp"></a><a name="p1199mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1201mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1203mcpsimp"><a name="p1203mcpsimp"></a><a name="p1203mcpsimp"></a>OT_HNR_REF_MODE_NORM</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p1205mcpsimp"><a name="p1205mcpsimp"></a><a name="p1205mcpsimp"></a>HNR参考帧普通模式，图像处理需要参考帧。默认为此模式。</p>
</td>
</tr>
<tr id="row1206mcpsimp"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1208mcpsimp"><a name="p1208mcpsimp"></a><a name="p1208mcpsimp"></a>OT_HNR_REF_MODE_NONE</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p1210mcpsimp"><a name="p1210mcpsimp"></a><a name="p1210mcpsimp"></a>HNR无参考帧模式，图像处理无需参考帧。</p>
</td>
</tr>
<tr id="row051475832617"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p15514195882619"><a name="p15514195882619"></a><a name="p15514195882619"></a>OT_HNR_REF_MODE_NONE_ADVANCED</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p9514175819262"><a name="p9514175819262"></a><a name="p9514175819262"></a>HNR无参考帧的高级模式，提升图像暗处细节，图像处理无需参考帧。</p>
</td>
</tr>
<tr id="row177621635151412"><td class="cellrowborder" valign="top" width="45%" headers="mcps1.1.3.1.1 "><p id="p77622035101419"><a name="p77622035101419"></a><a name="p77622035101419"></a>OT_HNR_REF_MODE_NORM_FACE</p>
</td>
<td class="cellrowborder" valign="top" width="55.00000000000001%" headers="mcps1.1.3.1.2 "><p id="p117621135131413"><a name="p117621135131413"></a><a name="p117621135131413"></a>HNR参考帧人脸模式，提升运动人脸细节，图像处理需要参考帧。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

如果是拍照场景，需选用HNR无参考帧模式，或HNR无参考帧的高级模式。

【相关数据类型及接口】

[ot\_hnr\_alg\_cfg](#ZH-CN_TOPIC_0000002441701357)

## ot\_hnr\_alg\_cfg<a name="ZH-CN_TOPIC_0000002441701357"></a>

【说明】

定义HNR算法配置参数。

【定义】

```
typedef struct {
    ot_hnr_ref_mode ref_mode;
} ot_hnr_alg_cfg;
```

【成员】

<a name="table1225mcpsimp"></a>
<table><thead align="left"><tr id="row1230mcpsimp"><th class="cellrowborder" valign="top" width="49%" id="mcps1.1.3.1.1"><p id="p1232mcpsimp"><a name="p1232mcpsimp"></a><a name="p1232mcpsimp"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="51%" id="mcps1.1.3.1.2"><p id="p1234mcpsimp"><a name="p1234mcpsimp"></a><a name="p1234mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1236mcpsimp"><td class="cellrowborder" valign="top" width="49%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p1238mcpsimp"><a name="p1238mcpsimp"></a><a name="p1238mcpsimp"></a>ref_mode</p>
</td>
<td class="cellrowborder" valign="top" width="51%" headers="mcps1.1.3.1.2 "><p id="p1240mcpsimp"><a name="p1240mcpsimp"></a><a name="p1240mcpsimp"></a>HNR参考帧模式。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

## ot\_hnr\_thread\_attr<a name="ZH-CN_TOPIC_0000002441661533"></a>

【说明】

定义HNR线程属性。

【定义】

```
typedef struct {
    td_u32 cpu_id;
} ot_hnr_thread_attr;
```

【成员】

<a name="table439015344443"></a>
<table><thead align="left"><tr id="row16390173444411"><th class="cellrowborder" valign="top" width="49%" id="mcps1.1.3.1.1"><p id="p7390734124410"><a name="p7390734124410"></a><a name="p7390734124410"></a>成员名称</p>
</th>
<th class="cellrowborder" valign="top" width="51%" id="mcps1.1.3.1.2"><p id="p3390734174417"><a name="p3390734174417"></a><a name="p3390734174417"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row4390173418443"><td class="cellrowborder" valign="top" width="49%" headers="mcps1.1.3.1.1 "><p xml:lang="sv-SE" id="p93905348447"><a name="p93905348447"></a><a name="p93905348447"></a>cpu_id</p>
</td>
<td class="cellrowborder" valign="top" width="51%" headers="mcps1.1.3.1.2 "><p id="p63901434134419"><a name="p63901434134419"></a><a name="p63901434134419"></a>绑定的CPU ID，取值范围为[0, 3]。</p>
</td>
</tr>
</tbody>
</table>

【注意事项】

无

【相关数据类型及接口】

无

# Proc调试信息<a name="ZH-CN_TOPIC_0000002408102202"></a>

-   **[概述](#ZH-CN_TOPIC_0000002408262154)**  

-   **[Proc信息说明](#ZH-CN_TOPIC_0000002441661569)**  

## 概述<a name="ZH-CN_TOPIC_0000002408262154"></a>

调试信息采用了Linux下的proc文件系统，可实时反映当前系统的运行状态，所记录的信息可供问题定位及分析时使用。

【文件目录】

/proc/umap

【信息查看方法】

-   在控制台上可以使用cat命令查看信息，cat /proc/umap/isp或cat /proc/umap/pqp；也可以使用其他常用的文件操作命令，例如 cp /proc/umap/isp ./或cp /proc/umap/pqp ./，将文件拷贝到当前目录。
-   在应用程序中可以将上述文件当作普通只读文件进行读操作，例如fopen、fread等。

>![](public_sys-resources/icon-note.gif) **说明：** 
>参数在描述时有以下两种情况需要注意：
>-   取值为\{0, 1\}的参数，如未列出具体取值和含义的对应关系，则参数为1时表示肯定，为0时表示否定。
>-   取值为\{aaa, bbb, ccc\}的参数，未列出具体取值和含义的对应关系，但可直接根据取值aaa、bbb或ccc判断参数含义。

## Proc信息说明<a name="ZH-CN_TOPIC_0000002441661569"></a>

-   **[HNR调试信息说明](#ZH-CN_TOPIC_0000002408262134)**  

-   **[PQP调试信息说明](#ZH-CN_TOPIC_0000002441701433)**  

### HNR调试信息说明<a name="ZH-CN_TOPIC_0000002408262134"></a>

【调试信息】

```
# cat /proc/umap/isp 
 
[ISP] Version: [MPP_Vx.x.x.x B0xx Release], Build Time[mm dd yyyy, hh:mm:ss]
----------------------------------------hnr info------------------------------------------------------------------------
hnr_en   attr_en       sfs   set_bnr_bypass bnr_bypass_status  long_frame_mode   normal_blend     normal_blend_status
1         1        31                1                 0                N                   0                       0
 
    cfg_id     width     height     is_wdr   ref_mode  version_id
        0      3840      2160        N   norm     2021082801
 
   vi_pipe       iso    handle  busy_node  free_node  ref_mode  input_depth  ref_cnt work_time   slp_cnt  slp_time
        0      3521        1         2         1      norm         8               1     12226        26     12231 
 
```

【注意事项】

当HNR数据流首次使能之后，才能查看到HNR相关的proc信息。

【调试信息分析】

记录当前HNR工作状态资源信息，主要包括HNR用户设置信息，加载的配置文件信息，任务状态信息。

【参数说明】

<a name="table1278mcpsimp"></a>
<table><thead align="left"><tr id="row1284mcpsimp"><th class="cellrowborder" colspan="2" valign="top" id="mcps1.1.4.1.1"><p id="p1286mcpsimp"><a name="p1286mcpsimp"></a><a name="p1286mcpsimp"></a>参数</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.1.4.1.2"><p id="p1288mcpsimp"><a name="p1288mcpsimp"></a><a name="p1288mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1290mcpsimp"><td class="cellrowborder" rowspan="8" valign="top" width="21.21212121212121%" headers="mcps1.1.4.1.1 "><p id="p1292mcpsimp"><a name="p1292mcpsimp"></a><a name="p1292mcpsimp"></a>HNR用户设置信息</p>
<p id="p203181514193710"><a name="p203181514193710"></a><a name="p203181514193710"></a></p>
</td>
<td class="cellrowborder" valign="top" width="32.32323232323232%" headers="mcps1.1.4.1.1 "><p id="p1294mcpsimp"><a name="p1294mcpsimp"></a><a name="p1294mcpsimp"></a>hnr_en</p>
</td>
<td class="cellrowborder" valign="top" width="46.464646464646464%" headers="mcps1.1.4.1.2 "><p id="p1296mcpsimp"><a name="p1296mcpsimp"></a><a name="p1296mcpsimp"></a>HNR数据流开关。</p>
</td>
</tr>
<tr id="row1297mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1299mcpsimp"><a name="p1299mcpsimp"></a><a name="p1299mcpsimp"></a>attr_en</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1301mcpsimp"><a name="p1301mcpsimp"></a><a name="p1301mcpsimp"></a>HNR效果开关。</p>
</td>
</tr>
<tr id="row1302mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1304mcpsimp"><a name="p1304mcpsimp"></a><a name="p1304mcpsimp"></a>sfs</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p xml:lang="de-DE" id="p1306mcpsimp"><a name="p1306mcpsimp"></a><a name="p1306mcpsimp"></a>画面的空域去噪强度<span xml:lang="en-US" id="ph1307mcpsimp"><a name="ph1307mcpsimp"></a><a name="ph1307mcpsimp"></a>。</span></p>
</td>
</tr>
<tr id="row1308mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1310mcpsimp"><a name="p1310mcpsimp"></a><a name="p1310mcpsimp"></a>set_bnr_bypass</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p xml:lang="de-DE" id="p1312mcpsimp"><a name="p1312mcpsimp"></a><a name="p1312mcpsimp"></a>BNR Bypass设置开关。</p>
</td>
</tr>
<tr id="row1313mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1315mcpsimp"><a name="p1315mcpsimp"></a><a name="p1315mcpsimp"></a>bnr_bypass_status</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p xml:lang="de-DE" id="p1317mcpsimp"><a name="p1317mcpsimp"></a><a name="p1317mcpsimp"></a>BNR Bypass生效状态。</p>
</td>
</tr>
<tr id="row1318mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1320mcpsimp"><a name="p1320mcpsimp"></a><a name="p1320mcpsimp"></a>long_frame_mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1322mcpsimp"><a name="p1322mcpsimp"></a><a name="p1322mcpsimp"></a>是否为长帧模式。</p>
<p id="p1323mcpsimp"><a name="p1323mcpsimp"></a><a name="p1323mcpsimp"></a>Y：是长帧模式；</p>
<p id="p1324mcpsimp"><a name="p1324mcpsimp"></a><a name="p1324mcpsimp"></a>N：不是长帧模式。</p>
</td>
</tr>
<tr id="row1174272813719"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p117421828163717"><a name="p117421828163717"></a><a name="p117421828163717"></a>normal_blend</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p87424285371"><a name="p87424285371"></a><a name="p87424285371"></a>normal_blend设置开关。</p>
</td>
</tr>
<tr id="row5318131493719"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p19318181415371"><a name="p19318181415371"></a><a name="p19318181415371"></a>normal_blend_status</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1831851419375"><a name="p1831851419375"></a><a name="p1831851419375"></a>normal_blend生效状态。</p>
</td>
</tr>
<tr id="row1325mcpsimp"><td class="cellrowborder" rowspan="6" valign="top" width="21.21212121212121%" headers="mcps1.1.4.1.1 "><p id="p1327mcpsimp"><a name="p1327mcpsimp"></a><a name="p1327mcpsimp"></a>HNR配置文件信息</p>
</td>
<td class="cellrowborder" valign="top" width="32.32323232323232%" headers="mcps1.1.4.1.1 "><p id="p1329mcpsimp"><a name="p1329mcpsimp"></a><a name="p1329mcpsimp"></a>cfg_id</p>
</td>
<td class="cellrowborder" valign="top" width="46.464646464646464%" headers="mcps1.1.4.1.2 "><p xml:lang="sv-SE" id="p1331mcpsimp"><a name="p1331mcpsimp"></a><a name="p1331mcpsimp"></a>配置文件ID<span xml:lang="en-US" id="ph1332mcpsimp"><a name="ph1332mcpsimp"></a><a name="ph1332mcpsimp"></a>。</span></p>
</td>
</tr>
<tr id="row1333mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1335mcpsimp"><a name="p1335mcpsimp"></a><a name="p1335mcpsimp"></a>width</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1337mcpsimp"><a name="p1337mcpsimp"></a><a name="p1337mcpsimp"></a>配置文件对应图像的宽度信息。</p>
</td>
</tr>
<tr id="row1338mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1340mcpsimp"><a name="p1340mcpsimp"></a><a name="p1340mcpsimp"></a>height</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1342mcpsimp"><a name="p1342mcpsimp"></a><a name="p1342mcpsimp"></a>配置文件对应图像的高度信息。</p>
</td>
</tr>
<tr id="row1343mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1345mcpsimp"><a name="p1345mcpsimp"></a><a name="p1345mcpsimp"></a>is_wdr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1347mcpsimp"><a name="p1347mcpsimp"></a><a name="p1347mcpsimp"></a>配置文件是否为WDR模式。</p>
<p id="p1348mcpsimp"><a name="p1348mcpsimp"></a><a name="p1348mcpsimp"></a>Y：WDR模式；</p>
<p id="p1349mcpsimp"><a name="p1349mcpsimp"></a><a name="p1349mcpsimp"></a>N：线性模式。</p>
</td>
</tr>
<tr id="row1350mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1352mcpsimp"><a name="p1352mcpsimp"></a><a name="p1352mcpsimp"></a>ref_mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1354mcpsimp"><a name="p1354mcpsimp"></a><a name="p1354mcpsimp"></a>配置文件的参考帧模式。</p>
<p id="p1355mcpsimp"><a name="p1355mcpsimp"></a><a name="p1355mcpsimp"></a>norm：参考帧普通模式，图像处理需要参考帧；</p>
<p id="p1356mcpsimp"><a name="p1356mcpsimp"></a><a name="p1356mcpsimp"></a>none：无参考帧模式，图像处理无需参考帧；</p>
<p id="p17409131362115"><a name="p17409131362115"></a><a name="p17409131362115"></a>advanced：无参考帧高级模式。</p>
</td>
</tr>
<tr id="row1357mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1359mcpsimp"><a name="p1359mcpsimp"></a><a name="p1359mcpsimp"></a>version_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1361mcpsimp"><a name="p1361mcpsimp"></a><a name="p1361mcpsimp"></a>配置文件版本ID。</p>
</td>
</tr>
<tr id="row1362mcpsimp"><td class="cellrowborder" rowspan="11" valign="top" width="21.21212121212121%" headers="mcps1.1.4.1.1 "><p id="p1364mcpsimp"><a name="p1364mcpsimp"></a><a name="p1364mcpsimp"></a>HNR任务状态信息</p>
<p id="p1090419201610"><a name="p1090419201610"></a><a name="p1090419201610"></a></p>
<p id="p682122314166"><a name="p682122314166"></a><a name="p682122314166"></a></p>
<p id="p13717102819160"><a name="p13717102819160"></a><a name="p13717102819160"></a></p>
</td>
<td class="cellrowborder" valign="top" width="32.32323232323232%" headers="mcps1.1.4.1.1 "><p id="p1366mcpsimp"><a name="p1366mcpsimp"></a><a name="p1366mcpsimp"></a>vi_pipe</p>
</td>
<td class="cellrowborder" valign="top" width="46.464646464646464%" headers="mcps1.1.4.1.2 "><p id="p1368mcpsimp"><a name="p1368mcpsimp"></a><a name="p1368mcpsimp"></a>HNR使能通路对应VI的PIPE号。</p>
</td>
</tr>
<tr id="row1369mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1371mcpsimp"><a name="p1371mcpsimp"></a><a name="p1371mcpsimp"></a>iso</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1373mcpsimp"><a name="p1373mcpsimp"></a><a name="p1373mcpsimp"></a>HNR计算输入的iso信息。</p>
<p id="p128741112132515"><a name="p128741112132515"></a><a name="p128741112132515"></a>iso值来自帧信息supplement的isp_frame_info中的iso。</p>
<p id="p1374mcpsimp"><a name="p1374mcpsimp"></a><a name="p1374mcpsimp"></a>如果是WDR非长帧模式，此iso值等于isp的iso值除以isp增益。</p>
</td>
</tr>
<tr id="row1375mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1377mcpsimp"><a name="p1377mcpsimp"></a><a name="p1377mcpsimp"></a>handle</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1379mcpsimp"><a name="p1379mcpsimp"></a><a name="p1379mcpsimp"></a>HNR当前任务handle号。</p>
</td>
</tr>
<tr id="row1380mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1382mcpsimp"><a name="p1382mcpsimp"></a><a name="p1382mcpsimp"></a>busy_node</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1384mcpsimp"><a name="p1384mcpsimp"></a><a name="p1384mcpsimp"></a>HNR任务队列的工作节点数量。</p>
</td>
</tr>
<tr id="row1385mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1387mcpsimp"><a name="p1387mcpsimp"></a><a name="p1387mcpsimp"></a>free_node</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1389mcpsimp"><a name="p1389mcpsimp"></a><a name="p1389mcpsimp"></a>HNR任务队列的空闲节点数量。</p>
</td>
</tr>
<tr id="row1390mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1392mcpsimp"><a name="p1392mcpsimp"></a><a name="p1392mcpsimp"></a>ref_mode</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1394mcpsimp"><a name="p1394mcpsimp"></a><a name="p1394mcpsimp"></a><span xml:lang="sv-SE" id="ph1395mcpsimp"><a name="ph1395mcpsimp"></a><a name="ph1395mcpsimp"></a>HNR</span>参考帧模式。</p>
<p id="p1121935572210"><a name="p1121935572210"></a><a name="p1121935572210"></a>norm：参考帧普通模式，图像处理需要参考帧；</p>
<p id="p1321935518225"><a name="p1321935518225"></a><a name="p1321935518225"></a>none：无参考帧模式，图像处理无需参考帧；</p>
<p id="p7219855162219"><a name="p7219855162219"></a><a name="p7219855162219"></a>advanced：无参考帧高级模式。</p>
</td>
</tr>
<tr id="row1398mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1400mcpsimp"><a name="p1400mcpsimp"></a><a name="p1400mcpsimp"></a>input_depth</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1402mcpsimp"><a name="p1402mcpsimp"></a><a name="p1402mcpsimp"></a>HNR输入队列的缓存深度。</p>
</td>
</tr>
<tr id="row168991915161"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p149091917163"><a name="p149091917163"></a><a name="p149091917163"></a>ref_cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1190819181614"><a name="p1190819181614"></a><a name="p1190819181614"></a>当前已提交任务个数。</p>
</td>
</tr>
<tr id="row1282114237166"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p182192312167"><a name="p182192312167"></a><a name="p182192312167"></a>work_time</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1682119239162"><a name="p1682119239162"></a><a name="p1682119239162"></a>10s内提交任务最大耗时，单位us。</p>
</td>
</tr>
<tr id="row83822541710"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p2038275171714"><a name="p2038275171714"></a><a name="p2038275171714"></a>slp_cnt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p938212513172"><a name="p938212513172"></a><a name="p938212513172"></a>submit线程10s内进入睡眠次数。</p>
</td>
</tr>
<tr id="row127161928171615"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p671752871615"><a name="p671752871615"></a><a name="p671752871615"></a>slp_time</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p15717162816166"><a name="p15717162816166"></a><a name="p15717162816166"></a>submit线程10s睡眠一次的平均耗时，单位us。</p>
</td>
</tr>
</tbody>
</table>

### PQP调试信息说明<a name="ZH-CN_TOPIC_0000002441701433"></a>

【调试信息】

```
# cat /proc/umap/pqp 
 
[PQP] Version: [Vx.x.x.x B0xx Release], Build Time[mm  dd yyyy, hh:mm:ss]
----------------------------------------pqp module param------------------------------
   dev_id       high_profile
        0             N

-------------------------------- pqp task queue info----------------------------------
   dev_id     wait_queue_id   work_queue_id   wait_head_idx    wait_tail_idx
        0                0               -1               0              0
   work_head_idx   work_tail_idx
                0             0
 
-------------------------------------- pqp task info -----------------------------------
   dev_id   handle   task_send    task_finish  last_finish_id  start_task_id  handle_wrap
        0        0           0            0            0             0            0
   finish_wrap
            0
 
-------------------------------------- pqp perf info------------------------------------
    dev_id  irq_num_per_sec  last_irq_time  max_irq_time  last_task_time
         0                0            0              0             0
 
--------------------------------------- pqp err info-------------------------------------
 dev_id  query_timeout_num  hw_timeout_num  hw_err_num
      0                   0                 0            0 
```

【调试信息分析】

记录当前PQP工作状态资源信息，主要包括PQP队列状态信息，任务状态信息，性能信息和错误信息。

【参数说明】

<a name="table1426mcpsimp"></a>
<table><thead align="left"><tr id="row1432mcpsimp"><th class="cellrowborder" colspan="2" valign="top" id="mcps1.1.4.1.1"><p id="p1434mcpsimp"><a name="p1434mcpsimp"></a><a name="p1434mcpsimp"></a>参数</p>
</th>
<th class="cellrowborder" valign="top" id="mcps1.1.4.1.2"><p id="p1436mcpsimp"><a name="p1436mcpsimp"></a><a name="p1436mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row10291152111527"><td class="cellrowborder" rowspan="2" valign="top" width="21.21212121212121%" headers="mcps1.1.4.1.1 "><p id="p14628150105311"><a name="p14628150105311"></a><a name="p14628150105311"></a>pqp module param</p>
<p id="p1310942045417"><a name="p1310942045417"></a><a name="p1310942045417"></a>PQP模块参数信息</p>
</td>
<td class="cellrowborder" valign="top" width="32.32323232323232%" headers="mcps1.1.4.1.1 "><p id="p1429102175212"><a name="p1429102175212"></a><a name="p1429102175212"></a>dev_id</p>
</td>
<td class="cellrowborder" valign="top" width="46.464646464646464%" headers="mcps1.1.4.1.2 "><p id="p1429172105216"><a name="p1429172105216"></a><a name="p1429172105216"></a>Dev ID。</p>
</td>
</tr>
<tr id="row1868161410523"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p568914165216"><a name="p568914165216"></a><a name="p568914165216"></a>high_profile</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1868914175214"><a name="p1868914175214"></a><a name="p1868914175214"></a>高性能的开关：仅SS928V100支持。</p>
<p id="p6443521105512"><a name="p6443521105512"></a><a name="p6443521105512"></a>Y：使能；</p>
<p id="p0324227205519"><a name="p0324227205519"></a><a name="p0324227205519"></a>N：不使能。</p>
</td>
</tr>
<tr id="row1438mcpsimp"><td class="cellrowborder" rowspan="7" valign="top" width="21.21212121212121%" headers="mcps1.1.4.1.1 "><p id="p1440mcpsimp"><a name="p1440mcpsimp"></a><a name="p1440mcpsimp"></a>pqp task queue info</p>
<p id="p1441mcpsimp"><a name="p1441mcpsimp"></a><a name="p1441mcpsimp"></a>PQP 模块任务队列信息</p>
</td>
<td class="cellrowborder" valign="top" width="32.32323232323232%" headers="mcps1.1.4.1.1 "><p id="p1443mcpsimp"><a name="p1443mcpsimp"></a><a name="p1443mcpsimp"></a>dev_id</p>
</td>
<td class="cellrowborder" valign="top" width="46.464646464646464%" headers="mcps1.1.4.1.2 "><p id="p1445mcpsimp"><a name="p1445mcpsimp"></a><a name="p1445mcpsimp"></a>Dev ID。</p>
</td>
</tr>
<tr id="row1446mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1448mcpsimp"><a name="p1448mcpsimp"></a><a name="p1448mcpsimp"></a>wait_queue_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1450mcpsimp"><a name="p1450mcpsimp"></a><a name="p1450mcpsimp"></a>等待队列ID（0或1）。</p>
</td>
</tr>
<tr id="row1451mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1453mcpsimp"><a name="p1453mcpsimp"></a><a name="p1453mcpsimp"></a>work_queue_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1455mcpsimp"><a name="p1455mcpsimp"></a><a name="p1455mcpsimp"></a>工作队列ID（0，1或-1），-1表示硬件空闲。</p>
</td>
</tr>
<tr id="row1456mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1458mcpsimp"><a name="p1458mcpsimp"></a><a name="p1458mcpsimp"></a>wait_head_idx</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1460mcpsimp"><a name="p1460mcpsimp"></a><a name="p1460mcpsimp"></a>等待队列的首个有效任务索引。</p>
</td>
</tr>
<tr id="row1461mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1463mcpsimp"><a name="p1463mcpsimp"></a><a name="p1463mcpsimp"></a>wait_tail_idx</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1465mcpsimp"><a name="p1465mcpsimp"></a><a name="p1465mcpsimp"></a>等待队列的末尾有效任务索引。</p>
</td>
</tr>
<tr id="row1466mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1468mcpsimp"><a name="p1468mcpsimp"></a><a name="p1468mcpsimp"></a>work_head_idx</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1470mcpsimp"><a name="p1470mcpsimp"></a><a name="p1470mcpsimp"></a>工作队列的首个有效任务索引。</p>
</td>
</tr>
<tr id="row1471mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1473mcpsimp"><a name="p1473mcpsimp"></a><a name="p1473mcpsimp"></a>work_tail_idx</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1475mcpsimp"><a name="p1475mcpsimp"></a><a name="p1475mcpsimp"></a>工作队列的末尾有效任务索引。</p>
</td>
</tr>
<tr id="row1476mcpsimp"><td class="cellrowborder" rowspan="8" valign="top" width="21.21212121212121%" headers="mcps1.1.4.1.1 "><p id="p1478mcpsimp"><a name="p1478mcpsimp"></a><a name="p1478mcpsimp"></a>pqp task info</p>
<p id="p1479mcpsimp"><a name="p1479mcpsimp"></a><a name="p1479mcpsimp"></a>PQP 任务相关信息</p>
</td>
<td class="cellrowborder" valign="top" width="32.32323232323232%" headers="mcps1.1.4.1.1 "><p id="p1481mcpsimp"><a name="p1481mcpsimp"></a><a name="p1481mcpsimp"></a>dev_id</p>
</td>
<td class="cellrowborder" valign="top" width="46.464646464646464%" headers="mcps1.1.4.1.2 "><p id="p1483mcpsimp"><a name="p1483mcpsimp"></a><a name="p1483mcpsimp"></a>Dev ID。</p>
</td>
</tr>
<tr id="row1484mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1486mcpsimp"><a name="p1486mcpsimp"></a><a name="p1486mcpsimp"></a>handle</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1488mcpsimp"><a name="p1488mcpsimp"></a><a name="p1488mcpsimp"></a>当前分配的任务handle号。</p>
</td>
</tr>
<tr id="row1489mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1491mcpsimp"><a name="p1491mcpsimp"></a><a name="p1491mcpsimp"></a>task_send</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1493mcpsimp"><a name="p1493mcpsimp"></a><a name="p1493mcpsimp"></a>当前已下发任务的个数。</p>
</td>
</tr>
<tr id="row1494mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1496mcpsimp"><a name="p1496mcpsimp"></a><a name="p1496mcpsimp"></a>task_finish</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1498mcpsimp"><a name="p1498mcpsimp"></a><a name="p1498mcpsimp"></a>当前已完成任务的个数。</p>
</td>
</tr>
<tr id="row1499mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1501mcpsimp"><a name="p1501mcpsimp"></a><a name="p1501mcpsimp"></a>last_finish_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1503mcpsimp"><a name="p1503mcpsimp"></a><a name="p1503mcpsimp"></a>上一次完成任务的ID。</p>
</td>
</tr>
<tr id="row1504mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1506mcpsimp"><a name="p1506mcpsimp"></a><a name="p1506mcpsimp"></a>start_task_id</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1508mcpsimp"><a name="p1508mcpsimp"></a><a name="p1508mcpsimp"></a>最近一次启动硬件执行的任务ID。</p>
</td>
</tr>
<tr id="row1509mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1511mcpsimp"><a name="p1511mcpsimp"></a><a name="p1511mcpsimp"></a>handle_wrap</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1513mcpsimp"><a name="p1513mcpsimp"></a><a name="p1513mcpsimp"></a>handle号分配发生回写的次数。</p>
</td>
</tr>
<tr id="row1514mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1516mcpsimp"><a name="p1516mcpsimp"></a><a name="p1516mcpsimp"></a>finish_wrap</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1518mcpsimp"><a name="p1518mcpsimp"></a><a name="p1518mcpsimp"></a>完成任务数发生回写的次数。</p>
</td>
</tr>
<tr id="row1519mcpsimp"><td class="cellrowborder" rowspan="5" valign="top" width="21.21212121212121%" headers="mcps1.1.4.1.1 "><p id="p1521mcpsimp"><a name="p1521mcpsimp"></a><a name="p1521mcpsimp"></a>pqp perf info</p>
<p id="p1522mcpsimp"><a name="p1522mcpsimp"></a><a name="p1522mcpsimp"></a>PQP性能相关信息</p>
</td>
<td class="cellrowborder" valign="top" width="32.32323232323232%" headers="mcps1.1.4.1.1 "><p id="p1524mcpsimp"><a name="p1524mcpsimp"></a><a name="p1524mcpsimp"></a>dev_id</p>
</td>
<td class="cellrowborder" valign="top" width="46.464646464646464%" headers="mcps1.1.4.1.2 "><p id="p1526mcpsimp"><a name="p1526mcpsimp"></a><a name="p1526mcpsimp"></a>Dev ID。</p>
</td>
</tr>
<tr id="row1527mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1529mcpsimp"><a name="p1529mcpsimp"></a><a name="p1529mcpsimp"></a>irq_num_per_sec</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1531mcpsimp"><a name="p1531mcpsimp"></a><a name="p1531mcpsimp"></a>最近一次的1秒内中断执行次数。</p>
</td>
</tr>
<tr id="row1532mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1534mcpsimp"><a name="p1534mcpsimp"></a><a name="p1534mcpsimp"></a>last_irq_time</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1536mcpsimp"><a name="p1536mcpsimp"></a><a name="p1536mcpsimp"></a>最近一次执行中断的执行耗时。</p>
<p id="p1537mcpsimp"><a name="p1537mcpsimp"></a><a name="p1537mcpsimp"></a>单位：us。</p>
</td>
</tr>
<tr id="row1538mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1540mcpsimp"><a name="p1540mcpsimp"></a><a name="p1540mcpsimp"></a>max_irq_time</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1542mcpsimp"><a name="p1542mcpsimp"></a><a name="p1542mcpsimp"></a>执行一次中断的最大耗时。</p>
<p id="p1543mcpsimp"><a name="p1543mcpsimp"></a><a name="p1543mcpsimp"></a>单位：us。</p>
</td>
</tr>
<tr id="row1544mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1546mcpsimp"><a name="p1546mcpsimp"></a><a name="p1546mcpsimp"></a>last_task_time</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1548mcpsimp"><a name="p1548mcpsimp"></a><a name="p1548mcpsimp"></a>最近一个任务执行耗时。</p>
<p id="p1549mcpsimp"><a name="p1549mcpsimp"></a><a name="p1549mcpsimp"></a>单位：us。</p>
</td>
</tr>
<tr id="row1550mcpsimp"><td class="cellrowborder" rowspan="4" valign="top" width="21.21212121212121%" headers="mcps1.1.4.1.1 "><p id="p1552mcpsimp"><a name="p1552mcpsimp"></a><a name="p1552mcpsimp"></a>pqp err info</p>
<p id="p1553mcpsimp"><a name="p1553mcpsimp"></a><a name="p1553mcpsimp"></a>PQP错误信息</p>
</td>
<td class="cellrowborder" valign="top" width="32.32323232323232%" headers="mcps1.1.4.1.1 "><p id="p1555mcpsimp"><a name="p1555mcpsimp"></a><a name="p1555mcpsimp"></a>dev_id</p>
</td>
<td class="cellrowborder" valign="top" width="46.464646464646464%" headers="mcps1.1.4.1.2 "><p id="p1557mcpsimp"><a name="p1557mcpsimp"></a><a name="p1557mcpsimp"></a>Dev ID。</p>
</td>
</tr>
<tr id="row1558mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1560mcpsimp"><a name="p1560mcpsimp"></a><a name="p1560mcpsimp"></a>query_timeout_num</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1562mcpsimp"><a name="p1562mcpsimp"></a><a name="p1562mcpsimp"></a>任务查询超时次数。</p>
</td>
</tr>
<tr id="row1563mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1565mcpsimp"><a name="p1565mcpsimp"></a><a name="p1565mcpsimp"></a>hw_timeout_num</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1567mcpsimp"><a name="p1567mcpsimp"></a><a name="p1567mcpsimp"></a>硬件执行超时次数。</p>
</td>
</tr>
<tr id="row1568mcpsimp"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1570mcpsimp"><a name="p1570mcpsimp"></a><a name="p1570mcpsimp"></a>hw_err_num</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="p1572mcpsimp"><a name="p1572mcpsimp"></a><a name="p1572mcpsimp"></a>硬件执行报错次数。</p>
</td>
</tr>
</tbody>
</table>

# 错误码<a name="ZH-CN_TOPIC_0000002441701421"></a>

HNR错误码如下所示。

**表 1**  HNR API错误码

<a name="_Ref248290030"></a>
<table><thead align="left"><tr id="row1613mcpsimp"><th class="cellrowborder" valign="top" width="21%" id="mcps1.2.4.1.1"><p id="p1615mcpsimp"><a name="p1615mcpsimp"></a><a name="p1615mcpsimp"></a>错误代码</p>
</th>
<th class="cellrowborder" valign="top" width="52%" id="mcps1.2.4.1.2"><p id="p1617mcpsimp"><a name="p1617mcpsimp"></a><a name="p1617mcpsimp"></a>宏定义</p>
</th>
<th class="cellrowborder" valign="top" width="27%" id="mcps1.2.4.1.3"><p id="p1619mcpsimp"><a name="p1619mcpsimp"></a><a name="p1619mcpsimp"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1621mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.2.4.1.1 "><p id="p1623mcpsimp"><a name="p1623mcpsimp"></a><a name="p1623mcpsimp"></a>0xa0528002</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.4.1.2 "><p xml:lang="sv-SE" id="p1625mcpsimp"><a name="p1625mcpsimp"></a><a name="p1625mcpsimp"></a>OT_ERR_HNR_INVALID_PIPE_ID</p>
</td>
<td class="cellrowborder" valign="top" width="27%" headers="mcps1.2.4.1.3 "><p id="p1627mcpsimp"><a name="p1627mcpsimp"></a><a name="p1627mcpsimp"></a>PIPE号无效</p>
</td>
</tr>
<tr id="row1628mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.2.4.1.1 "><p id="p1630mcpsimp"><a name="p1630mcpsimp"></a><a name="p1630mcpsimp"></a>0xa0528007</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.4.1.2 "><p xml:lang="sv-SE" id="p1632mcpsimp"><a name="p1632mcpsimp"></a><a name="p1632mcpsimp"></a>OT_ERR_HNR_ILLEGAL_PARAM</p>
</td>
<td class="cellrowborder" valign="top" width="27%" headers="mcps1.2.4.1.3 "><p id="p1634mcpsimp"><a name="p1634mcpsimp"></a><a name="p1634mcpsimp"></a>参数无效</p>
</td>
</tr>
<tr id="row1635mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.2.4.1.1 "><p id="p1637mcpsimp"><a name="p1637mcpsimp"></a><a name="p1637mcpsimp"></a>0xa052800a</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.4.1.2 "><p xml:lang="sv-SE" id="p1639mcpsimp"><a name="p1639mcpsimp"></a><a name="p1639mcpsimp"></a>OT_ERR_HNR_NULL_PTR</p>
</td>
<td class="cellrowborder" valign="top" width="27%" headers="mcps1.2.4.1.3 "><p id="p1641mcpsimp"><a name="p1641mcpsimp"></a><a name="p1641mcpsimp"></a>输入参数空指针错误</p>
</td>
</tr>
<tr id="row1642mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.2.4.1.1 "><p id="p1644mcpsimp"><a name="p1644mcpsimp"></a><a name="p1644mcpsimp"></a>0xa052800c</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.4.1.2 "><p xml:lang="sv-SE" id="p1646mcpsimp"><a name="p1646mcpsimp"></a><a name="p1646mcpsimp"></a>OT_ERR_HNR_NOT_SUPPORT</p>
</td>
<td class="cellrowborder" valign="top" width="27%" headers="mcps1.2.4.1.3 "><p id="p1648mcpsimp"><a name="p1648mcpsimp"></a><a name="p1648mcpsimp"></a>操作不支持</p>
</td>
</tr>
<tr id="row1649mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.2.4.1.1 "><p id="p1651mcpsimp"><a name="p1651mcpsimp"></a><a name="p1651mcpsimp"></a>0xa052800d</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.4.1.2 "><p xml:lang="sv-SE" id="p1653mcpsimp"><a name="p1653mcpsimp"></a><a name="p1653mcpsimp"></a>OT_ERR_HNR_NOT_PERM</p>
</td>
<td class="cellrowborder" valign="top" width="27%" headers="mcps1.2.4.1.3 "><p id="p1655mcpsimp"><a name="p1655mcpsimp"></a><a name="p1655mcpsimp"></a>操作不允许</p>
</td>
</tr>
<tr id="row1656mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.2.4.1.1 "><p id="p1658mcpsimp"><a name="p1658mcpsimp"></a><a name="p1658mcpsimp"></a>0xa0528014</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.4.1.2 "><p xml:lang="sv-SE" id="p1660mcpsimp"><a name="p1660mcpsimp"></a><a name="p1660mcpsimp"></a>OT_ERR_HNR_NO_MEM</p>
</td>
<td class="cellrowborder" valign="top" width="27%" headers="mcps1.2.4.1.3 "><p id="p1662mcpsimp"><a name="p1662mcpsimp"></a><a name="p1662mcpsimp"></a>数据缓存溢出</p>
</td>
</tr>
<tr id="row1663mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.2.4.1.1 "><p id="p1665mcpsimp"><a name="p1665mcpsimp"></a><a name="p1665mcpsimp"></a>0xa0528018</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.4.1.2 "><p xml:lang="sv-SE" id="p1667mcpsimp"><a name="p1667mcpsimp"></a><a name="p1667mcpsimp"></a>OT_ERR_HNR_NOT_READY</p>
</td>
<td class="cellrowborder" valign="top" width="27%" headers="mcps1.2.4.1.3 "><p id="p1669mcpsimp"><a name="p1669mcpsimp"></a><a name="p1669mcpsimp"></a>未初始化</p>
</td>
</tr>
<tr id="row1670mcpsimp"><td class="cellrowborder" valign="top" width="21%" headers="mcps1.2.4.1.1 "><p id="p1672mcpsimp"><a name="p1672mcpsimp"></a><a name="p1672mcpsimp"></a>0xa0528050</p>
</td>
<td class="cellrowborder" valign="top" width="52%" headers="mcps1.2.4.1.2 "><p xml:lang="sv-SE" id="p1674mcpsimp"><a name="p1674mcpsimp"></a><a name="p1674mcpsimp"></a>OT_ERR_HNR_BIN_NOT_MATACH</p>
</td>
<td class="cellrowborder" valign="top" width="27%" headers="mcps1.2.4.1.3 "><p id="p1676mcpsimp"><a name="p1676mcpsimp"></a><a name="p1676mcpsimp"></a><span xml:lang="sv-SE" id="ph1677mcpsimp"><a name="ph1677mcpsimp"></a><a name="ph1677mcpsimp"></a>Bin</span>配置文件不匹配</p>
</td>
</tr>
</tbody>
</table>

