cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

#编译器优化等级
#-O0:无优化(默认,适合调试)
#-O1:基本优化(平衡速度和代码大小)
#-O2:中等优化(提升性能,可能增加代码体积)
#-O3:最高优化(可能影响代码稳定性)
#-Os:优化代码大小(适用于资源受限设备)
#-Ofast：激进优化(可能牺牲精度)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -flto")

# 设置项目名字,宏的作用
set(CMAKE_PROJECT_NAME myPro)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#真正设置项目名字必须得在以上操作下
project(${CMAKE_PROJECT_NAME})

#让cmake支持c和汇编
enable_language(C ASM)

#可执行文件(.elf)
add_executable(${CMAKE_PROJECT_NAME})

#添加子文件:cmake目录里面的cmakeLists.txt
add_subdirectory(cmake)

#添加头文件路径和全局宏
target_link_libraries(${CMAKE_PROJECT_NAME}
        header_file_path_and_global_definitions
)
