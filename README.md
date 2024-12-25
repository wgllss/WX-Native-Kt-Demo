> Android 越往深处研究，必然离不开NDK,和JNI相关知识

## 一、前言

Android开发中，最重要的一项技能便是NDK开发，它涉及到JNI,C,C++等相关知识\
我们常见的MMKV,音视频库FFmpeg等库的应用，都有相关这方面的知识，它是Android开发人员通往深水区的一张门票。

本文我们就简单介绍JNI,NDK的相关入门知识：
**1. JNI方法注册（静态注册，动态注册）**
**2. JNI的基础数据类型**
**3. JNI引用数据类型**
**4. JNI函数签名信息**
**5. JNIEnv的介绍**
**6. JNI编译之Cmake**

***

**7. 示例：获取JNI返回字符串(静态注册)**
**8. 示例：调用JNI,JNI调用Java层无返回值方法(静态注册)**
**9. 示例：调用JNI,JNI调用Java层无返回值方法(带参数)(静态注册)**
**10. 示例：调用JNI去调用java方法(静态注册)**
**11. 示例：调用JNI去调用java 变量值(静态注册)**
**12. 示例：去调用JNI去调用Java带callback方法，带参数(静态注册)**
**13. 示例：动态注册**

## 二、基础介绍

**1. JNI是什么？**   JNI(Java Native Interface),它是提供一种Java字节码调用C/C++的解决方案，JNI描述的是一种技术。

![a63332d245e450bd38b7b571c4988397\_webp.webp](https://p0-xtjj-private.juejin.cn/tos-cn-i-73owjymdk6/4067be600e2445489cb0066445c61ab4~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAgV2dsbHNz:q75.awebp?policy=eyJ2bSI6MywidWlkIjoiMzU2NjYxODM1MDgyNTczIn0%3D&rk3s=f64ab15b&x-orig-authkey=f32326d3454f2ac7e96d3d06cdbb035152127018&x-orig-expires=1735718507&x-orig-sign=RxfAY6lnYbgn7yck5YN1CnDTUKg%3D)\
**2. NDK是什么?** NDK(Native Development Kit)
Android NDK 是一组允许您将 C 或 C++（“原生代码”）嵌入到 Android 应用中的工具，NDK描述的是工具集。 能够在 Android 应用中使用原生代码对于想执行以下一项或多项操作的开发者特别有用：

*   在平台之间移植其应用。
*   重复使用现有库，或者提供其自己的库供重复使用。
*   在某些情况下提高性能，特别是像游戏这种计算密集型应用。

**3. JNI方法静态注册：**
JNI函数名格式（需将”.”改为”—”）：
Java\_ + 包名（com.example.auto.jnitest）+ 类名(MainActivity) + 函数名(stringFromJNI)

静态方法的缺点：

*   要求JNI函数的名字必须遵循JNI规范的命名格式；
*   名字冗长，容易出错；
*   初次调用会根据函数名去搜索JNI中对应的函数，会影响执行效率；
*   需要编译所有声明了native函数的Java类，每个所生成的class文件都要用javah工具生成一个头文件；

**4. JNI方法动态注册：**
Java与JNI通过JNINativeMethod的结构来建立函数映射表，它在jni.h头文件中定义，其结构内容如下：

    typedef struct {
        const char* name;
        const char* signature;
        void*       fnPtr;
    } JNINativeMethod;

创建映射表后，调用RegisterNatives函数将映射表注册给JVM;
当Java层通过System.loadLibrary加载JNI库时，会在库中查JNI\_OnLoad函数。可将JNI\_OnLoad视为JNI库的入口函数，需要在这里完成所有函数映射和动态注册工作，及其他一些初始化工作。

**5. JNI的基础数据类型对照表：**

| Java类型         | JNI类型    | 描述     |
| -------------- | -------- | ------ |
| boolean(布尔型)   | jboolean | 无符号8位  |
| byte(字节型)      | jbyte    | 有符号8位  |
| char(字符型)      | jchar    | 无符号16位 |
| short(短整型)     | jshort   | 有符号16位 |
| int(整型)        | jint     | 有符号32位 |
| long(长整型)      | jlong    | 有符号64位 |
| foat(浮点型)      | jfloat   | 32位    |
| double(双精度浮点型) | jdouble  | 64位    |

**6. JNI引用数据类型对照表:**

| Java引用类型            | JNI类型         | Java引用类型   | JNI类型        |
| ------------------- | ------------- | ---------- | ------------ |
| All objects         | jobject       | char\[ ]   | jcharArray   |
| java.lang.Class     | jclass        | short\[ ]  | jshortArray  |
| java.lang.String    | jstring       | int\[]     | jintArray    |
| java.lang.Throwable | jthrowable    | long\[ ]   | jlongArray   |
| Object\[ ]          | jobjectArray  | float\[]   | jfloatArray  |
| boolean\[ ]         | jbooleanArray | double\[ ] | jdoubleArray |
| byte\[ ]            | jbyteArray    |            |              |

**7. JNI函数签名信息**
由于Java支持函数重载，因此仅仅根据函数名是没法找到对应的JNI函数。为了解决这个问题，JNI将参数类型和返回值类型作为函数的签名信息。

JNI规范定义的函数签名信息格式：  **(参数1类型字符…)返回值类型字符**

函数签名例子:

![308c38d80edf5aac76fbfdd0f8d07548\_webp.webp](https://p0-xtjj-private.juejin.cn/tos-cn-i-73owjymdk6/20b6f6af608343cba7fe977a76cae47e~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAgV2dsbHNz:q75.awebp?policy=eyJ2bSI6MywidWlkIjoiMzU2NjYxODM1MDgyNTczIn0%3D&rk3s=f64ab15b&x-orig-authkey=f32326d3454f2ac7e96d3d06cdbb035152127018&x-orig-expires=1735718507&x-orig-sign=uleSGBs6pJsvn7n8%2FPDtnNG8Kuc%3D)

JNI常用的数据类型及对应字符对照表:

| Java类型    | 字符                                           |
| --------- | -------------------------------------------- |
| void      | V                                            |
| boolean   | Z (容易误写成B)                                   |
| int       | I                                            |
| long      | J (容易误写成L)                                   |
| double    | D                                            |
| float     | F                                            |
| byte      | B                                            |
| char      | C                                            |
| short     | S                                            |
| int\[ ]   | \[I (数组以"\["开始)                              |
| String    | Ljava/lang/String; (引用类型格式为”L包名类名;”，要记得加";") |
| Object\[] | \[Ljava/lang/object;                         |

**8. JNIEnv的介绍**

1.  JNIEnv概念 : JNIEnv是一个线程相关的结构体, 该结构体代表了 Java 在本线程的运行环境。通过JNIEnv可以调用到一系列JNI系统函数。
2.  JNIEnv线程相关性： 每个线程中都有一个 JNIEnv 指针。JNIEnv只在其所在线程有效, 它不能在线程之间进行传递。

> 注意：在C++创建的子线程中获取JNIEnv，要通过调用JavaVM的AttachCurrentThread函数获得。在子线程退出时，要调用JavaVM的DetachCurrentThread函数来释放对应的资源，否则会出错。

**9. JNI编译之Cmake**

> CMake 则是一个跨平台的编译工具，它并不会直接编译出对象，而是根据自定义的语言规则（CMakeLists.txt）生成 对应 makefile 或 project 文件，然后再调用底层的编译， 在Android Studio 2.2 之后支持Cmake编译。

*   add\_library 指令\
    语法：add\_library(libname \[SHARED | STATIC | MODULE] \[EXCLUDE\_FROM\_ALL] \[source])\
    将一组源文件 source 编译出一个库文件，并保存为 libname.so (lib 前缀是生成文件时 CMake自动添加上去的)。其中有三种库文件类型，不写的话，默认为 STATIC;

    *   SHARED: 表示动态库，可以在(Java)代码中使用 System.loadLibrary(name) 动态调用；
    *   STATIC: 表示静态库，集成到代码中会在编译时调用；
    *   MODULE: 只有在使用 dyId 的系统有效，如果不支持 dyId，则被当作 SHARED 对待；
    *   EXCLUDE\_FROM\_ALL: 表示这个库不被默认构建，除非其他组件依赖或手工构建;

<!---->

    #将compress.c 编译成 libcompress.so 的共享库
    add_library(compress SHARED compress.c)

*   target\_link\_libraries 指令 **语法：target\_link\_libraries(target library \<debug | optimized> library2…)**  这个指令可以用来为 target 添加需要的链接的共享库，同样也可以用于为自己编写的共享库添加共享库链接。如：

<!---->

    #指定 compress 工程需要用到 libjpeg 库和 log 库
    target_link_libraries(compress libjpeg ${log-lib})

*   find\_library 指令 **语法：find\_library(<VAR> name1 path1 path2 ...)**  VAR 变量表示找到的库全路径，包含库文件名 。例如：

<!---->

    find_library(libX  X11 /usr/lib)
    find_library(log-lib log)  #路径为空，应该是查找系统环境变量路径

示例工程Cmake截图如下：

![ca8c9dfcd9a9697885245bb1e54410a.png](https://p0-xtjj-private.juejin.cn/tos-cn-i-73owjymdk6/42019731a4434767bd9ac921c2c6c722~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAgV2dsbHNz:q75.awebp?policy=eyJ2bSI6MywidWlkIjoiMzU2NjYxODM1MDgyNTczIn0%3D&rk3s=f64ab15b&x-orig-authkey=f32326d3454f2ac7e96d3d06cdbb035152127018&x-orig-expires=1735718507&x-orig-sign=Li%2FU9RPc8ihOs7%2BO6SSye8yqb4E%3D)

## 三、示例工程代码

示例工程截图：

![fc733d67be28090d46db73b44785f42.png](https://p0-xtjj-private.juejin.cn/tos-cn-i-73owjymdk6/9dc5e2fd2ef1417c91e4461d7eceda70~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAgV2dsbHNz:q75.awebp?policy=eyJ2bSI6MywidWlkIjoiMzU2NjYxODM1MDgyNTczIn0%3D&rk3s=f64ab15b&x-orig-authkey=f32326d3454f2ac7e96d3d06cdbb035152127018&x-orig-expires=1735718507&x-orig-sign=jXGfagsLqJs83tonO0FuDehmFKI%3D)

示例MainActivity内需要加载SO:

    companion object {
        // Used to load the 'native_kt_demo' library on application startup.
        init {
            System.loadLibrary("native_kt_demo")
        }
    }

**1. 示例：获取JNI返回字符串(静态注册)**

Kotlin 代码

    external fun stringFromJNI(): String

JNI层下代码

    //extern "C" 避免编绎器按照C++的方式去编绎C函数
    extern "C"
    //JNIEXPORT ：用来表示该函数是否可导出（即：方法的可见性
    //1、宏 JNIEXPORT 代表的就是右侧的表达式： __attribute__ ((visibility ("default")))
    //2、或者也可以说： JNIEXPORT 是右侧表达式的别名；
    //3、宏可表达的内容很多，如：一个具体的数值、一个规则、一段逻辑代码等
    JNIEXPORT
    //jstring 代表方法返回类型为Java中的 String
    jstring
    //用来表示函数的调用规范（如：__stdcall）
    JNICALL
    Java_com_wx_nativex_kt_demo_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

**2. 示例：调用JNI,JNI调用Java层无返回值方法(静态注册)**\
Kotlin代码：

    external fun callJNI()

JNI层代码：

    extern "C" JNIEXPORT void JNICALL
    Java_com_wx_nativex_kt_demo_MainActivity_callJNI(JNIEnv *env, jobject thiz) {
        LOGE("-----静态注册 ， 无返回值方法 调用成功-----");
        jclass js = env->GetObjectClass(thiz);
        jmethodID jmethodId = env->GetMethodID(js, "toast", "(Ljava/lang/String;)V");
        env->CallVoidMethod(thiz, jmethodId, env->NewStringUTF("静态注册 无返回值方法 调用成功"));
    }

**3. 示例：调用JNI,JNI调用Java层无返回值方法(带参数)(静态注册)**\
Kotlin代码：

    external fun stringFromJNIwithParameter(str: String): String

JNI层代码：

    extern "C" JNIEXPORT jstring JNICALL
    Java_com_wx_nativex_kt_demo_MainActivity_stringFromJNIwithParameter(JNIEnv *env, jobject thiz, jstring str) {
        const char *data = env->GetStringUTFChars(str, NULL);
        LOGE("-----获取到Java 传来的数据:data %s-----", data);
        env->ReleaseStringChars(str, reinterpret_cast<const jchar *>(data));
        const char *src = "111---";
        const int size = sizeof(data) + sizeof(src);
        char datares[size] = "111---";
        return env->NewStringUTF(strcat(datares, data));
    }

**4. 示例：调用JNI去调用java方法(静态注册)**\
Kotlin代码：

    external fun callNativeCallJavaMethod()

JNI层代码：

    extern "C"
    JNIEXPORT void JNICALL
    Java_com_wx_nativex_kt_demo_MainActivity_callNativeCallJavaMethod(JNIEnv *env, jobject thiz) {
        jclass js = env->GetObjectClass(thiz);
        jmethodID jmethodId = env->GetMethodID(js, "toast", "(Ljava/lang/String;)V");
        env->CallVoidMethod(thiz, jmethodId, env->NewStringUTF("jni 通过反射调用 java toast方法"));
    }

**5. 示例：调用JNI去调用java 变量值(静态注册)**

Kotlin代码：

    external fun callNativeCallJavaField()

JNI层代码：

    extern "C"
    JNIEXPORT void JNICALL
    Java_com_wx_nativex_kt_demo_MainActivity_callNativeCallJavaField(JNIEnv *env, jobject thiz) {
        jclass js = env->GetObjectClass(thiz);
        jfieldID jfieldId = env->GetFieldID(js, "androidData", "Ljava/lang/String;");
        jstring newDataValue = env->NewStringUTF("四海一家");
    //    jclass js = env->GetObjectClass(thiz);
        jmethodID jmethodId = env->GetMethodID(js, "toast", "(Ljava/lang/String;)V");
        env->CallVoidMethod(thiz, jmethodId, newDataValue);
    //    env->SetObjectField(thiz, jfieldId, newDataValue);
    }

**6. 示例：去调用JNI去调用Java带callback方法，带参数(静态注册)**\
Kotlin代码：

    external fun callNativeWithCallBack(callBack: NativeCallBack)

JNI层代码：

    extern "C"
    JNIEXPORT void JNICALL
    Java_com_wx_nativex_kt_demo_MainActivity_callNativeWithCallBack(JNIEnv *env, jobject thiz, jobject call_back) {
        LOGE("-----静态注册 ， callback 调用成功-----");
        jclass js = env->GetObjectClass(call_back);
        jmethodID jmethodId = env->GetMethodID(js, "nmd", "(Ljava/lang/String;)V");
        env->CallVoidMethod(call_back, jmethodId, env->NewStringUTF("我是Jni Native层callBack回调回来的数据值"));
    }

**7. 示例：动态注册**
Kotlin代码：

    external fun dynamicRegisterCallBack(callBack: NativeCallBack)

JNI层代码：

```

void regist(JNIEnv *env, jobject thiz, jobject call_back) {
    LOGD("--动态注册调用成功-->");
    jclass js = env->GetObjectClass(call_back);
    jmethodID jmethodId = env->GetMethodID(js, "nmd", "(Ljava/lang/String;)V");
    env->CallVoidMethod(call_back, jmethodId, env->NewStringUTF("我是Jni Native层动态注册callBack回调回来的数据值"));
}

jint RegisterNatives(JNIEnv *env) {
    jclass activityClass = env->FindClass("com/wx/nativex/kt/demo/MainActivity");
    if (activityClass == NULL) {
        return JNI_ERR;
    }
    JNINativeMethod methods_MainActivity[] = {
            {
                    "dynamicRegisterCallBack",
                    "(Lcom/wx/nativex/kt/demo/NativeCallBack;)V",
                    (void *) regist
            }
    };

    return env->
            RegisterNatives(activityClass, methods_MainActivity,
                            sizeof(methods_MainActivity) / sizeof(methods_MainActivity[0]));
}


//JNI_OnLoad   java
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGE("-----JNI_OnLoad 方法调用了-----");
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jint result = RegisterNatives(env);
//    函数注册
    return JNI_VERSION_1_6;
}
```

## 总结

本文简单介绍了NDK编程中JNI的基础：并写了相关示例Demo代码

1.  JNI方法注册（静态注册，动态注册）
2.  JNI的基础数据类型
3.  JNI引用数据类型
4.  JNI函数签名信息
5.  JNIEnv的介绍
6.  JNI编译之Cmake
7.  示例：获取JNI返回字符串(静态注册)
8.  示例：调用JNI,JNI调用Java层无返回值方法(静态注册)
9.  示例：调用JNI,JNI调用Java层无返回值方法(带参数)(静态注册)
10. 示例：调用JNI去调用java方法(静态注册)
11. 示例：调用JNI去调用java 变量值(静态注册)
12. 示例：去调用JNI去调用Java带callback方法，带参数(静态注册)
13. 示例：动态注册

## 感谢阅读：

#### 欢迎用你发财的小手： 点点赞、收藏收藏，或者 关注关注

#### 这里你会学到不一样的东西

#### 项目地址

[Gitee地址](https://gitee.com/wgllss888/WX-Native-Kt-Demo)\
[Github地址](https://github.com/wgllss/WX-Native-Kt-Demo)
