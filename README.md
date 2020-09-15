# Mirai++

![](https://github.com/Chlorie/miraipp-template/workflows/Build%20Status/badge.svg)

[English Version](README-en.md)

## 公告
目前开发者正在咕咕，随时有可能会弃坑。本项目可能会在一段时间后完全终止，因为作者正在咕咕基于 JNI 的框架。

## 总览
基于 Mirai 的 QQ bot C++ CMake 项目模板。本项目基于 Mirai 的 [HTTP API](https://github.com/mamoe/mirai-api-http)，而非 mirai-native。

## 需求
本项目使用了一些 C++17 的特性，所以若需要编译该项目你需要一个支持 C++17 的编译器。

建议使用包管理器来管理包依赖。本人目前使用 [vcpkg](https://github.com/microsoft/vcpkg) 做依赖管理。
目前本项目内用到了如下的这些库：
- [cpr](https://github.com/whoshuu/cpr): C++ HTTP请求库。
- [WebSocket++](https://github.com/zaphoyd/websocketpp): C++ websocket 客户端/服务端库。
- [Asio](https://think-async.com/Asio/): 跨平台 C++ 网络和底层 I/O 编程库。注意这里用的是单独的版本不是 boost 内的版本。
- [libiconv](https://www.gnu.org/software/libiconv/): C 语言文本编码库。
- [JSON for Modern C++](https://github.com/nlohmann/json): 很易用的 C++ json 库。

## 文档
若需要一个如何起步的简短教程，请看[这里](https://github.com/Chlorie/miraipp-template/wiki/Examples)。

本项目采用 Doxygen 注释格式写明 API 函数的使用方法，你可以用 Doxygen 来生成 pdf/html 格式的文档，或者如果你的 IDE 比较好的话，
在自动补全提示里面应该就会显示对应函数的文档。因为我尽量在维持代码全都是 ASCII 字符（防止出现编码问题，因为是 C++ 所以都懂……），
所以目前这些文档都是英文的（不过这些英文不会太难就是了，写代码英语是基础不是吗）。
