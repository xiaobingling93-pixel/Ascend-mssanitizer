# MindStudio Sanitizer

<br>

## 最新消息

* [2025.12.30]：MindStudio Sanitizer 项目首次上线

## 简介

MindStudio Sanitizer（算子异常检测，msSanitizer）是一款基于昇腾AI处理器异常检测工具，面向单算子开发场景，提供内存检测、竞争检测、未初始化检测及同步检测四项核心功能。

## 功能介绍

msSanitizer 通过多个子功能模块提供不同类型的异常检测能力，当前已支持的功能如下：

| 功能名称 | 功能描述  |
|---------|--------|
| **内存检测** | 检测 Global Memory 与 Local Memory 中的越界访问、未对齐访问等内存异常。 |
| **竞争检测** | 检测并行计算环境中因并发内存访问引发的数据竞争问题。 |
| **未初始化检测**  | 检测因使用未初始化变量导致的内存读取异常。 |
| **同步检测**   |检测 SetFlag/WaitFlag 指令未配对使用的问题（在 Ascend C 算子开发中，SetFlag 与 WaitFlag 必须成对出现）。 |

## 快速入门

以简单的加法算子为例，快速体验本工具的核心功能，详细操作步骤请参见 [《msSanitizer 快速入门》](./docs/zh/quick_start/mssanitizer_quick_start.md)。

## 安装指南

介绍工具的环境依赖与安装方法，具体请参见 [《msSanitizer 安装指南》](docs/zh/install_guide/mssanitizer_install_guide.md)。

## 使用指南

工具的详细使用方法，具体请参见 [《msSanitizer 使用指南》](docs/zh/user_guide/mssanitizer_user_guide.md)

## 典型案例

通过典型问题场景帮助用户理解并掌握工具使用，具体请参见 [《msSanitizer 典型案例》](docs/zh/best_practices/basic_cases.md)。

## 限制与注意事项

1. msSanitizer工具不支持对多线程算子及使用掩码的向量类计算指令的检测。
2. 启用 --check-device-heap 或 --check-cann-heap 后，将不再对 Kernel 内部进行检测。
3. Device 侧内存检测与 CANN 软件栈内存检测不可同时启用；若同时启用，将报错：“CANNOT enable both --check-cann-heap and --check-device-heap”。
4. 使用 msSanitizer 提供的 API 头文件重新编译的程序，仅适用于基于 Ascend CL 接口的内存泄漏检测，不支持 Device 接口检测。

## API参考

包含sanitizer接口和mstx扩展接口两种类型，具体请参见 [《msSanitizer 对外接口使用说明》](docs/zh/api_reference/mssanitizer_api_reference.md)。

## FAQ

常见问题详见 [《msSanitizer FAQ》](docs/zh/support/faq.md)。

## 贡献指南

贡献方法详见 [《贡献指南》](./docs/zh/contributing/contributing_guide.md)。

## License

许可条款详见 [《License声明》](docs/zh/legal/license_notice.md)。  

## 安全声明

安全声明详见 [《安全声明》](docs/zh/legal/security_statement.md)。  

## 免责声明

免责条款详见 [《免责声明》](docs/zh/legal/disclaimer.md)。  

## 社区交流

欢迎大家为社区做贡献。如果有任何疑问或建议，请提交[Issues](https://gitcode.com/Ascend/mssanitizer/issues)，我们会尽快回复。感谢您的支持。

## 致谢

本工具由华为公司的下列部门贡献：   

- 计算产品线     

感谢来自社区的每一个PR，欢迎贡献。
