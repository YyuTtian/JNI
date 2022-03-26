# 最小支持版本
cmake_minimum_required(VERSION 3.10.2)
# 项目名字
project("jni_test")

# 定义cpp文件路径
file(GLOB file1 *.cpp *.c)
file(GLOB file2 impl/*.cpp impl/*.c)

# 导入头文件
include_directories("inc")

# 设置库文件路径  设置CMAKE_CXX_FLAGS环境变量(库的路径)
# CMAKE_ANDROID_ARCH_ABI 自动选择abi架构
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")

add_library(jni_test # so库名字
        SHARED # 动态库
        ${file1} # 源文件路径
        ${file2} # 源文件路径
        )

find_library(
        log-lib # 给liblog.so取个名字叫log-lib
        # 去这个路径找liblog.so
        # D:\as\sdk\ndk\21.4.7075529\platforms\android-23\arch-arm64\usr\lib
        log
)

target_link_libraries(
        jni_test # 输出的so的名字
        ${log-lib} # 把log-lib库链接到jni_test的so里面
        fmod # 把fmod库链接到jni_test的so里面
        fmodL # 把fmodL库链接到jni_test的so里面
)