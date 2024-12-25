#版本号
cmake_minimum_required(VERSION 3.22.1)

#cpp工程名
project("native_kt_demo")

add_library(
        #SO lib库名字
        native_kt_demo
        # shared :动态库， STATIC:静态库
        SHARED
        # 需要编译的源文件
        native-lib.cpp)

#需要引入其他库，在系统环境变量中查找
#提供环境变量为： D:\android_software\android_sdk\android_sdk\ndk-bundle\toolchains\llvm\prebuilt\windows-x86_64\sysroot\usr\lib\aarch64-linux-android\22
#系统环境目录里面 查找到 liblog.so  赋值给变量   log-lib
find_library(
        #赋值给系统变量 log-lib
        log-lib
        #系统环境目录里面查找到liblog.so
        log)

#取上面 log-lib 变量对应的 liblog.so 库添加到 nativex这个工程里面
target_link_libraries(
        native_kt_demo
        ${log-lib})