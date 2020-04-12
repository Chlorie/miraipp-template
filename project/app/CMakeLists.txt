# User code, which contains the main() function 
# and builds to an executable

add_executable(app main.cpp)
target_link_libraries(app PRIVATE miraipp)
