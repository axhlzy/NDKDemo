#include<jni.h>
#include<android/log.h>
#include "../../../../../SDK/ndk-bundle/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/jni.h"
#include "../../../../../SDK/ndk-bundle/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/android/log.h"

JavaVM *_vm;

//动态注册的第一个方法
void dynamicJNIMethod1(JNIEnv *env, jobject jobj, jint ji) {
    __android_log_print(ANDROID_LOG_ERROR, "JNI", "第一个动态注册方法传递的参数:%d", ji);
}

//动态注册的第二个方法
jstring dynamicJNIMethod2(JNIEnv *env, jobject jobj) {
    jstring returnStr = env->NewStringUTF("第二个动态注册方法返回的字符串");
    return returnStr;
}


//需要动态注册的方法组
static JNINativeMethod gMethods[] = {
        {"dynamicJavaMethod1", "(I)V",                 (void *) dynamicJNIMethod1},

        {"dynamicJavaMethod2", "()Ljava/lang/String;", (void *) dynamicJNIMethod2}
};
//需要动态注册的类全名
static const char *mClassName = "com/lzy/ndk/JniClient";

//此函数通过调用RegisterNatives方法来注册我们的函数

static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *getMethods,
                                 int methodsNum) {
    jclass clazz;
    //找到声明native方法的类
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }

    jint result = 0;
    result = env->RegisterNatives(clazz, gMethods, methodsNum);
    //注册函数 参数：java类 所要注册的函数数组 注册函数的个数
    if (result < 0) {

        __android_log_print(ANDROID_LOG_ERROR, "JNI", "RegisterNatives:%d", result);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static int registerNatives(JNIEnv *env) {
    const char *className = "com/example/ancpp/MainActivity";
    return registerNativeMethods(env, className, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
}

//动态注册时都会执行到这个方法中
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;//定义JNI Env
    jint result = -1;
    /*JavaVM::GetEnv 原型为 jint (*GetEnv)(JavaVM*, void**, jint);
     */
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
    /*开始注册
     * 传入参数是JNI env
     * 以registerNatives(env)为例说明
     */

    if (!registerNatives(env)) {
        return -1;
    }
    _vm = vm;

    result = JNI_VERSION_1_6;
    return result;
}
