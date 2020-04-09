cmake_minimum_required(VERSION 3.10)

set(SOURCE_FILES
    "main.cpp"
    "mirai/core/session.cpp"   
    "mirai/core/message.cpp" "mirai/core/events.cpp" "mirai/core/types.cpp"
    "mirai/utils/thread.cpp" "mirai/utils/encoding.cpp" "mirai/utils/request.cpp")

add_executable(main ${SOURCE_FILES})

target_compile_features(main PUBLIC cxx_std_17) # Enforce C++17
set_target_properties(main PROPERTIES CXX_EXTENSIONS off)

# /Zc:__cplusplus is needed for MSVC to produce correct value for the __cplusplus macro
# If this option is not on, __cplusplus will always expands to 199711L
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(main PUBLIC "/Zc:__cplusplus")
endif()

find_package(cpr CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(unofficial-iconv CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(main PRIVATE 
    cpr 
    websocketpp::websocketpp 
    OpenSSL::SSL OpenSSL::Crypto
    asio asio::asio 
    unofficial::iconv::libiconv unofficial::iconv::libcharset 
    nlohmann_json nlohmann_json::nlohmann_json)
