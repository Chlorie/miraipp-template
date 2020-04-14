# Mirai++ API Library

set(SOURCE_FILES
    "mirai/core/session.cpp" "mirai/core/common.cpp"
    "mirai/core/events.cpp" "mirai/core/types.cpp"
    "mirai/core/message/message.cpp" "mirai/core/message/segment.cpp" 
     "mirai/core/message/common.cpp" 
    "mirai/core/message/received_message.cpp"
    "mirai/core/websockets/client.cpp" "mirai/core/websockets/connection.cpp"
    "mirai/utils/thread.cpp" "mirai/utils/encoding.cpp" "mirai/utils/request.cpp" 
) 

add_library(miraipp ${SOURCE_FILES})
target_include_directories(miraipp INTERFACE ./)

target_compile_features(miraipp PUBLIC cxx_std_17) # Enforce C++17 or newer
set_target_properties(miraipp PROPERTIES CXX_EXTENSIONS off)
target_compile_definitions(miraipp PUBLIC ASIO_STANDALONE)

# /Zc:__cplusplus is needed for MSVC to produce correct value for the __cplusplus macro
# If this option is not on, __cplusplus will always expands to 199711L
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(miraipp PUBLIC "/Zc:__cplusplus")
endif()

find_package(cpr CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(unofficial-iconv CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(miraipp PUBLIC 
    cpr 
    websocketpp::websocketpp 
    OpenSSL::SSL OpenSSL::Crypto
    asio asio::asio 
    unofficial::iconv::libiconv unofficial::iconv::libcharset 
    nlohmann_json nlohmann_json::nlohmann_json)
