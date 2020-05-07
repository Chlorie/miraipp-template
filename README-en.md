# Mirai++

![](https://github.com/Chlorie/miraipp-template/workflows/Build%20Status/badge.svg)

[中文版](README.md)

## Overview
Mirai QQ bot C++ CMake template project. Based on the [HTTP API](https://github.com/mamoe/mirai-api-http) instead of mirai-native.

## Requirements
This project utilizes some C++17 features, so a working C++17 compliant compiler is needed to compile this project.

Use of package managers is encouraged. I'm currently using [vcpkg](https://github.com/microsoft/vcpkg) for package management.
Currently the following packages are used in this project:
- [cpr](https://github.com/whoshuu/cpr): C++ requests library.
- [WebSocket++](https://github.com/zaphoyd/websocketpp): C++ websocket client/server library.
- [Asio](https://think-async.com/Asio/): Cross-platform C++ library for network and low-level I/O programming. This is the standalone version, not the boost version.
- [libiconv](https://www.gnu.org/software/libiconv/): C text encoding library.
- [JSON for Modern C++](https://github.com/nlohmann/json): Easy integration of json in C++.

## Documentation
This project uses Doxygen syntax for documenting the API functions. You can use Doxygen to generate pdf/html documentation,
or if you have a decent IDE proper documentation will show up in the auto-complete pop-ups.
