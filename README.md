# MindStudio Sanitizer

<br>

## 最新消息
* [2025.12.30]：MindStudio Sanitizer项目首次上线

## 简介
MindStudio Sanitizer（算子异常检测，msSanitizer）是一款基于昇腾AI处理器异常检测工具，面向单算子开发场景，提供内存检测、竞争检测、未初始化检测及同步检测四项核心功能。

## 功能介绍
msSanitizer 通过多个子功能模块提供不同类型的异常检测能力，当前已支持的功能如下：
| 功能 | 功能简介  |
|---------|--------|
| 内存检测 | 检测外部存储（Global Memory）和内部存储（Local Memory）的越界及未对齐等内存访问异常。 |
| 竞争检测 | 检测在并行计算环境中内存访问竞争的问题。 |
| 未初始化检测  | 检测由于使用未初始化的变量而导致的内存异常。 |
| 同步检测   |检测未配对的SetFlag指令（在Ascend C算子开发过程中，必须成对使用SetFlag和WaitFlag）。 |

## 快速入门
以简单的加法算子为例，快速体验本工具核心功能，详细操作步骤请参见[《msSanitizer 快速入门》](./docs/zh/quick_start/mssanitizer_quick_start.md)。

## 安装指南
介绍工具的环境依赖及安装方式，具体请参见[《msSanitizer 安装指南》](docs/zh/install_guide/mssanitizer_install_guide.md)。

## 使用指南
工具详细使用方法，具体请参见[《msSanitizer 使用指南》](docs/zh/user_guide/mssanitizer_user_guide.md)

## 典型案例
通过一些典型案例帮助用户理解并熟悉工具，具体请参见[《msSanitizer 典型案例》](docs/zh/best_practices/basic_cases.md)。

## API参考
包含sanitizer接口和mstx扩展接口两种类型，具体请参见[《msSanitizer 对外接口使用说明》](docs/zh/api_reference/mssanitizer_api_reference.md)。

## 限制与注意事项
1. msSanitizer工具不支持对多线程算子及使用了掩码的向量类计算指令进行检测。
2. check-device-heap或--check-cann-heap使能后，将不会对Kernel内进行检测。Device侧内存检测和CANN软件栈内存检测不能同时使能，若同时使能会提示“CANNOT enable both --check-cann-heap and --check-device-heap”。
3. 使用msSanitizer工具提供的API头文件重新编译的待检测程序只能用于Ascend CL系列接口的泄漏检测，无法用于Device接口的检测。

## FAQ
工具常见问题请参见[《msSanitizer FAQ》](docs/zh/support/faq.md)。

## 贡献指南
1. 贡献流程请参见[《msOT 贡献流程说明》](https://gitcode.com/Ascend/msot/blob/master/docs/zh/common/contribute_workflow.md)。   
2. 开发环境搭建及编译、测试方法等，请参见[《msSanitizer开发指南》](docs/zh/development_guide/develop_guide.md)。

## License
详细说明请参见[《msOT License声明》](https://gitcode.com/Ascend/msot/blob/master/docs/zh/common/license_notice.md)。  

## 免责声明
详细说明请参见[《msOT 免责声明》](https://gitcode.com/Ascend/msot/blob/master/docs/zh/common/disclaimer.md)。  

## 社区交流
欢迎大家为社区做贡献。如果有任何疑问或建议，请提交[Issues](https://gitcode.com/Ascend/mssanitizer/issues)，我们会尽快回复。感谢您的支持。

## 致谢
本工具由华为公司的下列部门联合贡献：   
<span style="font-size:8px;">●</span> 计算产品线    
<span style="font-size:8px;">●</span> 2012实验室    
感谢来自社区的每一个PR，欢迎贡献。
