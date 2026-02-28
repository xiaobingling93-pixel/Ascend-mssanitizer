# 代码架构说明

## 目录结构
整体目录设计如下：
```
MindStudio-Sanitizer
|-- .gitsubmodule           # 管理依赖的submodule文件
|-- build.py                # 一键式构建脚本入口
|-- CMakeLists.txt          # cmake构建入口，只包含编译过程，不包含打包
|-- cmake
    |-- CMakeLists.txt      # 集成构建cmake入口，包含打包过程
|-- csrc
   |-- address_sanitizer    # 此处提供外部信息输入接口，在明确芯片类型后，能依据输入信息，调用统一信息展示接口
      |-- config
      |--
      |-- address_sanitizer.h # 提供外部接口
   |-- core
      |-- stub_def          # 桩空间，会针对不同的文件组合生成不同场景的桩
      |-- framework         # 此处需要针对信息
         |-- parse          # 依据不同工具要求，将信息加工后提供给不同工具
         |-- msg_def.h      # 统一信息展示接口
         |-- process_ctl.h  # 整体看流程代码固定而且每个流程不多，如果不涉及针对不同工具需要抽象的，建议一个。
   |-- main.cpp
|-- msopscommon             # 公共组件子仓
|-- test
   |-- address_sanitizer
   |-- framework
|-- thirdparty              # 三方依赖子仓存放目录
|-- docs                    # 项目文档介绍
└── README.md               # 整体仓代码说明
```