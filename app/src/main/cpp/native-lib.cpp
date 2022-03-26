#include <jni.h>
#include <string>
#include "fmod.hpp"

#include <android/log.h>
#include <pthread.h>

#define TAG "qglog" // 这个是自定义的LOG的TAG
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


extern "C" // 采用c的环境 因为c++兼容c 有些库是纯c的代码
JNIEXPORT // 标记该函数可以被外部调用
jstring // 返回值类型
JNICALL // jni函数标记 可以不写
Java_com_jni_Test_stringFromJNI // 函数名这么长是为了禁止函数重载  c不允许函数重载
        (JNIEnv *env/*jni环境*/, jobject thiz/*这个函数所属的对象实例*/) {
    return env->NewStringUTF("我是C++");
}

/**
    基本类型
    Java	Native	 Signature

    byte	jbyte	    B
    char	jchar	    C
    double	jdouble 	D
    float	jfloat	    F
    int	    jint	    I
    short	jshort	    S
    long	jlong	    J
    boolean	jboolean	Z
    void	void	    V

    引用类型
    Java	    Native	        Signature

    所有对象	    jobject	        L+类名+;
    Class	    jclass	        Ljava/lang/Class;
    String	    jstring	        Ljava/lang/String;
    Throwable	jthrowable	    Ljava/lang/Throwable;
    Object[]	jobjectArray	[L+类名+;
    byte[]	    jbyteArray	    [B
    char[]	    jcharArray	    [C
    double[]	jdoubleArray	[D
    float[]	    jfloatArray	    [F
    int[]	    jintArray	    [I
    short[]	    jshortArray	    [S
    long[]	    jlongArray	    [J


 */

JavaVM *jvm = nullptr;


// native 真正的函数
// void dynamicMethod01(JNIEnv *env, jobject thiz) { // OK的
void dynamicMethod01() { // 也OK  如果你用不到  JNIEnv jobject ，可以不用写
    LOGD("我是动态注册的函数 dynamicMethod01...");
}

int dynamicMethod02(JNIEnv *env, jobject thiz, jstring valueStr) { // 也OK
    const char *text = env->GetStringUTFChars(valueStr, nullptr);
    LOGD("我是动态注册的函数 dynamicMethod02... %s", text);
    env->ReleaseStringUTFChars(valueStr, text);
    return 200;
}

/*
     typedef struct {
        const char* name;       // 函数名
        const char* signature;  // 函数的签名
        void*       fnPtr;      // 函数指针
     } JNINativeMethod;
     */
static const JNINativeMethod jniNativeMethod[] = {
        {"dynamicRegister1", "()V",                   (void *) (dynamicMethod01)},
        {"dynamicRegister2", "(Ljava/lang/String;)I", (int *) (dynamicMethod02)},
};


// Java：像 Java的构造函数，如果你不写构造函数，默认就有构造函数，如果你写构造函数 覆写默认的构造函数
// JNI JNI_OnLoad函数，如果你不写JNI_OnLoad，默认就有JNI_OnLoad，如果你写JNI_OnLoad函数 覆写默认的JNI_OnLoad函数
extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *javaVm, void *) {
    // 保存全局jvm实例
    ::jvm = javaVm;

    JNIEnv *jniEnv = nullptr;
    int result = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);

    // result 等于0  就是成功
    if (result != JNI_OK) {
        return -1; // 会奔溃，故意奔溃
    }

    const char *testClassName = "com/jni/Test";
    jclass testClass = jniEnv->FindClass(testClassName);

    // jint RegisterNatives(Class, 我们的数组==jniNativeMethod， 注册的数量 = 2)
    jniEnv->RegisterNatives(testClass,
                            jniNativeMethod,
                            sizeof(jniNativeMethod) / sizeof(JNINativeMethod));

    return JNI_VERSION_1_6; //  // AS的JDK在JNI默认最高1.6      存Java的JDKJNI 1.8
}



extern "C"
JNIEXPORT void JNICALL
Java_com_jni_Test_changeTxt(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    jfieldID txt = env->GetStaticFieldID(clazz, "txt", "Ljava/lang/String;");
    jstring js = static_cast<jstring>(env->GetStaticObjectField(clazz, txt));
    char *oldValue = const_cast<char *>(env->GetStringUTFChars(js, NULL));
    LOGI("oldValue=%s", oldValue);

    jstring newValue = env->NewStringUTF("qglNewValue");
    env->SetStaticObjectField(clazz, txt, newValue);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_jni_Test_nativeCallJava(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    jmethodID addMethodId = env->GetMethodID(clazz, "add", "(II)I");
    jint result = env->CallIntMethod(thiz, addMethodId, 10, 20);
    LOGI("add result=%d", result);
}



extern "C"
JNIEXPORT void JNICALL
Java_com_jni_Test_nativeOpArrays(JNIEnv *env, jobject thiz, jintArray int_array, jobjectArray string_array) {
    jint intArrayLength = env->GetArrayLength(int_array);
    LOGI("intArrayLength=%d", intArrayLength);

    jint *intArray = env->GetIntArrayElements(int_array, 0);

    for (int i = 0; i < intArrayLength; i++) {
        *(intArray + i) += 100;
        LOGI("native intArray i=%d value=%d", i, *(intArray + i));
    }
    // 加上这句话才会修改Java层的数组
    // mode = 0 刷新Java数组 并释放C++数组
    // mode = JNI_COMMIT 刷新Java数组 不释放C++数组
    // mode = JNI_ABORT 释放C++数组
    env->ReleaseIntArrayElements(int_array, intArray, 0);

    jsize stringArrayLength = env->GetArrayLength(string_array);
    LOGI("stringArrayLength=%d", stringArrayLength);
    for (int i = 0; i < stringArrayLength; ++i) {
        jstring ele = static_cast<jstring>(env->GetObjectArrayElement(string_array, i));
        const char *str = env->GetStringUTFChars(ele, 0);
        LOGI("stringArray i=%d value=%s", i, str);
        env->ReleaseStringUTFChars(ele, str);
    }

}

// 这样写不是全局引用
// globalPersonClass = static_cast<jclass>(env->NewGlobalRef(personClass));这样才把它变成全局
jclass globalPersonClass = NULL;

extern "C"
JNIEXPORT void JNICALL
Java_com_jni_Test_reference(JNIEnv *env, jobject thiz) {
    if (globalPersonClass == NULL) {
        jclass personClass = env->FindClass("com/jni/Person");
        globalPersonClass = static_cast<jclass>(env->NewGlobalRef(personClass));
        env->DeleteLocalRef(personClass);
    }

    // 手动释放全局引用
    // env->DeleteGlobalRef(globalPersonClass);
    // globalPersonClass = NULL;
}

// 声明  在funcImpl中实现
extern void func();

extern "C"
JNIEXPORT jobject JNICALL
Java_com_jni_Test_nativeCreatePerson(JNIEnv *env, jobject thiz) {
    // 使用全局引用 创建Person对象
    jmethodID init = env->GetMethodID(globalPersonClass, "<init>", "(Ljava/lang/String;I)V");
    jstring name = env->NewStringUTF("邱广雷");
    jint age = 18;
    jobject personObj = env->NewObject(globalPersonClass, init, name, age);
    // 调用声明的函数
    func();
    return personObj;
}

extern const char *changeVoice(const char *path, int mode);

extern "C"
JNIEXPORT void JNICALL
Java_com_jni_Test_changeVoice(JNIEnv *env, jobject thiz, jstring path) {
    const char *_path = env->GetStringUTFChars(path, 0);
    const char *result = changeVoice(_path, 1);
    env->ReleaseStringUTFChars(path, _path);

    // 告知Java播放完毕
    jclass mainCls = env->GetObjectClass(thiz);
    jmethodID endMethod = env->GetMethodID(mainCls, "playerEnd", "(Ljava/lang/String;)V");
    jstring value = env->NewStringUTF(result);
    env->CallVoidMethod(thiz, endMethod, value);
}


class Params {
public:
    jobject obj;

};

void *threadTask(void *pVoid) {
    JNIEnv *jniEnv = nullptr; // 全新的JNIEnv  异步线程里面操作
    jint attachResult = jvm->AttachCurrentThread(&jniEnv, nullptr); // 附加当前异步线程后，会得到一个全新的 env，此env相当于是子线程专用env
    if (attachResult != JNI_OK) {
        return nullptr; // 附加失败，返回了
    }

    auto *params = static_cast<Params *>(pVoid);
    jclass mainClass = jniEnv->GetObjectClass(params->obj);
    jmethodID msgMethod = jniEnv->GetMethodID(mainClass, "msg", "(Ljava/lang/String;)V");
    jstring msg = jniEnv->NewStringUTF("来自Native的子线程");
    jniEnv->CallVoidMethod(params->obj, msgMethod, msg);

    jvm->DetachCurrentThread();
    return nullptr;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_jni_Test_nativeWorkNotifyUi(JNIEnv *env, jobject thiz) {

    auto *params = new Params;
    // jobject 不能跨线程 要提升为全局引用
    // 不用的时候 要记得释放
    params->obj = env->NewGlobalRef(thiz);

    pthread_t pid;
    pthread_create(&pid, nullptr, threadTask, params);
}