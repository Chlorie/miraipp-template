cmake_minimum_required (VERSION 3.8)

add_executable(main "main.cpp" "mirai/session.cpp" "mirai/utils.cpp" "mirai/common.cpp" "mirai/message.cpp")
set_target_properties(main PROPERTIES
    CXX_STANDARD 17)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(main PRIVATE cpr)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(main PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
find_package(unofficial-iconv CONFIG REQUIRED)
target_link_libraries(main PRIVATE unofficial::iconv::libiconv unofficial::iconv::libcharset)
