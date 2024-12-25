#include <jni.h>
#include <string>
#include "Log.h"


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

extern "C" JNIEXPORT void JNICALL
Java_com_wx_nativex_kt_demo_MainActivity_callJNI(JNIEnv *env, jobject thiz) {
    LOGE("-----静态注册 ， 无返回值方法 调用成功-----");
    jclass js = env->GetObjectClass(thiz);
    jmethodID jmethodId = env->GetMethodID(js, "toast", "(Ljava/lang/String;)V");
    env->CallVoidMethod(thiz, jmethodId, env->NewStringUTF("静态注册 无返回值方法 调用成功"));
}
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

extern "C"
JNIEXPORT void JNICALL
Java_com_wx_nativex_kt_demo_MainActivity_callNativeCallJavaMethod(JNIEnv *env, jobject thiz) {
    jclass js = env->GetObjectClass(thiz);
    jmethodID jmethodId = env->GetMethodID(js, "toast", "(Ljava/lang/String;)V");
    env->CallVoidMethod(thiz, jmethodId, env->NewStringUTF("jni 通过反射调用 java toast方法"));
}
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
extern "C"
JNIEXPORT void JNICALL
Java_com_wx_nativex_kt_demo_MainActivity_callNativeWithCallBack(JNIEnv *env, jobject thiz, jobject call_back) {
    LOGE("-----静态注册 ， callback 调用成功-----");
    jclass js = env->GetObjectClass(call_back);
    jmethodID jmethodId = env->GetMethodID(js, "nmd", "(Ljava/lang/String;)V");
    env->CallVoidMethod(call_back, jmethodId, env->NewStringUTF("我是Jni Native层callBack回调回来的数据值"));
}

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
//    手机app   手机开机了
    LOGE("-----JNI_OnLoad 方法调用了-----");
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jint result = RegisterNatives(env);
//    函数注册
    return JNI_VERSION_1_6;
}
