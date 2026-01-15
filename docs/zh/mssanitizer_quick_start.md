# **MindStudio Sanitizer快速入门<a id="ZH-CN_TOPIC_0000002526770247"></a>**

**简介<a id="section15151104724113"></a>**

msSanitizer工具应用于算子开发的整个周期，帮助开发者确保算子的质量和稳定性。通过在早期阶段发现并修复异常，msSanitizer显著减少了产品上线后的潜在风险和后期维护成本。

> [!NOTE] 说明
> 
> -   启动工具后，将会在当前目录下自动生成工具运行日志文件mssanitizer\__\{TIMESTAMP\}_\_\{_PID_\}.log，当用户程序运行完成后，界面将会打印异常报告。
>-   $\{git\_clone\_path\}为sample仓的路径。

**环境准备<a id="section11958811134212"></a>**

-   准备Atlas A2 训练系列产品/Atlas A2 推理系列产品的服务器，并安装对应的驱动和固件，具体安装过程请参见《[CANN 软件安装指南](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/83RC1/softwareinst/instg/instg_0000.html)》中的“安装NPU驱动和固件”章节。
-   安装配套版本的CANN Toolkit开发套件包和ops算子包并配置CANN环境变量，具体请参见《[CANN 软件安装指南](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/83RC1/softwareinst/instg/instg_0000.html)》。
-   若要使用MindStudio Insight进行查看时，需要单独安装MindStudio Insight软件包，具体下载链接请参见《[MindStudio Insight工具用户指南](https://www.hiascend.com/document/detail/zh/mindstudio/82RC1/GUI_baseddevelopmenttool/msascendinsightug/Insight_userguide_0002.html)》的“安装与卸载”章节。

>[!NOTE] 说明
>
> 在安装昇腾AI处理器的服务器执行`npu-smi info`命令进行查询，获取**Chip Name**信息。实际配置值为AscendChip Name，例如**Chip Name**取值为xxxyy，实际配置值为Ascendxxxyy。当Ascendxxxyy为代码样例路径时，需要配置Ascendxxxyy。

**操作步骤<a id="section156364416425"></a>**

1.  在$\{git\_clone\_path\}/samples/operator/ascendc/0\_introduction/1\_add\_frameworklaunch目录下执行以下命令，生成自定义算子工程，进行host侧和kernel侧的算子实现。

    ```sh
    bash install.sh -v Ascendxxxyy    # xxxyy为用户实际使用的具体芯片类型
    ```

2.  在$\{git\_clone\_path\}/samples/operator/ascendc/0\_introduction/1\_add\_frameworklaunch/CustomOp目录下执行以下命令，重新编译部署算子。

    ```sh
    bash build.sh
    ./build_out/custom_opp_<target_os>_<target_architecture>.run   # 当前目录下run包的名称
    ```

3.  切换到$\{git\_clone\_path\}/samples/operator/ascendc/0\_introduction/1\_add\_frameworklaunch/AclNNInvocation目录，拉起算子API运行脚本，进行内存检测。
    1.  启用内存检测：
        -   可显式指定内存检测，默认会开启非法读写、多核踩踏、非对齐访问和非法释放的检测功能：

            ```sh
            mssanitizer --tool=memcheck bash run.sh
            ```

        -   执行如下命令，可手动启用内存泄漏的检测功能：

            ```sh
            mssanitizer --tool=memcheck --leak-check=yes bash run.sh
            ```

    2.  定位内存异常，具体请参见《[MindStudio Sanitizer工具用户指南](./mssanitizer_user_guide.md)》中的“异常检测功能介绍>使用示例>内存检测”章节。

4.  进行竞争检测。
    1.  执行如下命令，启用竞争检测。

        ```sh
        mssanitizer --tool=racecheck bash run.sh
        ```

    2.  定位内存竞争，具体请参见《[MindStudio Sanitizer工具用户指南](./mssanitizer_user_guide.md)》中的“异常检测功能介绍>使用示例>竞争检测”章节。

        当前目录下会自动生成工具运行日志文件mssanitizer\__\{TIMESTAMP\}_\_\{_PID_\}.log，当用户程序运行完成后，界面将会打印异常报告。

5.  进行未初始化检测。
    1.  执行如下命令，可手动启用未初始化的检测。

        ```sh
        mssanitizer --tool=initcheck bash run.sh 
        ```

    2.  定位内存异常，具体请参见《[MindStudio Sanitizer工具用户指南](./mssanitizer_user_guide.md)》中的“异常检测功能介绍>使用示例>未初始化检测”章节。

