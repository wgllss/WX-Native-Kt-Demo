#include <jni.h>
#include <android/log.h>

#define LOG_TAG "Wgllss"
#define LOGD(fmt,args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGI(fmt,args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGW(fmt,args...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, fmt, ##args)
#define LOGE(fmt,args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#define LOGF(fmt,args...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, fmt, ##args)