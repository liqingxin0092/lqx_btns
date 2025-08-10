cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)

#全局宏
set(global_defines
		STM32F10X_MD
		USE_STDPERIPH_DRIVER
)

#头文件路径
set(defines_dir
    ${CMAKE_SOURCE_DIR}/core/inc
    ${CMAKE_SOURCE_DIR}/lib/inc
    ${CMAKE_SOURCE_DIR}/cmsis/Device/ST/STM32F1xx/Include
    ${CMAKE_SOURCE_DIR}/cmsis/Include
)

#应用的源文件
set(main_src
    ${CMAKE_SOURCE_DIR}/core/src/main.c
		${CMAKE_SOURCE_DIR}/core/src/delay.c
		${CMAKE_SOURCE_DIR}/core/src/lqx_btns.c
		${CMAKE_SOURCE_DIR}/core/src/lqx_btns_port.c
		${CMAKE_SOURCE_DIR}/Core/Src/system_stm32f10x.c
		${CMAKE_SOURCE_DIR}/core/src/uart.c
   		 ${CMAKE_SOURCE_DIR}/startup_stm32f103xb.s
)

#添加库的源文件,1:选择性的添加. 0:全部添加
if(1)
set(lib_src
		${CMAKE_SOURCE_DIR}/lib/src/misc.c
		${CMAKE_SOURCE_DIR}/lib/src/stm32f10x_gpio.c
		${CMAKE_SOURCE_DIR}/lib/src/stm32f10x_rcc.c
		${CMAKE_SOURCE_DIR}/lib/src/stm32f10x_usart.c
)
else()
file(GLOB_RECURSE lib_src "${CMAKE_SOURCE_DIR}/lib/src/*.c")
endif()

#项目的库文件
set(lib_sets
    lib
)

#一些全局属性,INTERFACE:无构建产物，仅传递属性。
#头文件库、工具链配置或纯依赖声明
#这里的信息就是头文件路径和全局宏
add_library(header_file_path_and_global_definitions INTERFACE)
target_include_directories(header_file_path_and_global_definitions INTERFACE ${defines_dir}) #所有头文件路径
target_compile_definitions(header_file_path_and_global_definitions INTERFACE ${global_defines}) #全局宏

#这是具体的库,lib:stm32标准库
add_library(lib OBJECT)
target_sources(lib PRIVATE ${lib_src})#库的源文件,lib中的源文件仅用于构建lib自身
target_link_libraries(lib PUBLIC header_file_path_and_global_definitions)   #lib也就是stm32标准库 依赖 头文件路径,全局宏

#给项目添加源文件(main.c等)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${main_src}) #应用的源文件

# 给项目链接一些库的集合
target_link_libraries(${CMAKE_PROJECT_NAME} ${lib_sets})

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate that STM32CubeMX code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()
