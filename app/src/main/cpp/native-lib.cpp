#include <jni.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
// 引入log头文件
#include  <android/log.h>
#include <unistd.h>
// log标签
#define  TAG    "=====LOG====="
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

JNIEnv *env1;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (JNI_OK != vm->GetEnv(reinterpret_cast<void **> (&env), JNI_VERSION_1_4)) {
        LOGD("JNI_OnLoad could not get JNI env");
        return JNI_ERR;
    }
    jclass clazz = env->FindClass("com/lzy/ndk/JniClient");

    static JNINativeMethod methods[] = {
//            {"getStr", "()Ljava/lang/String;", (void*)getString }
    };
    env1 = env;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof((methods)[0])) < 0) {
        LOGD("RegisterNatives error");
        return JNI_ERR;
    }

    return JNI_VERSION_1_4;
}

//编译生成后在.init_array段
extern "C"
__attribute__((constructor))
void _getString(void) {
    if (env1 != NULL) {
        jstring jstr = env1->NewStringUTF("This is Natvie String!");
    }
    int a = 10;
    static char *b = const_cast<char *>("123poi一二三");
    double c = 222;

}

extern "C"
__attribute__((constructor))
void init(void) {
    int a = 99;
    char haystack[20] = "RUNOOB";
    char needle[10] = "NOOB";
    char *ret = NULL;
    for (int i = 0; i < a; i += 3) {
        ret = strstr(haystack, needle);
    }
    if (strlen(ret) > 5) {
        LOGI("Init", ret);
    }
}

////编译生成后在.init段
extern "C"
void _init(void) {
    int a = 10;
}

int num = 54321;

extern "C"
__attribute__((constructor))
void check(void) {
    FILE *pfile = NULL;
    char buf[0x1000] = {0};
    // 执行命令
    char *strCatTcp = "cat /proc/net/tcp |grep :5D8A";
    char *strNetstat = "netstat |grep :23946";
    pfile = popen(strCatTcp, "r");
    int pid = getpid();
    if (NULL == pfile) {
        printf("打开命令失败!\n");
        return;
    }
    // 获取结果
    while (fgets(buf, sizeof(buf), pfile)) {
        // 执行到这里，判定为调试状态
        printf("执行cat /proc/net/tcp |grep :5D8A的结果:\n");
        printf("%s", buf);
        int ret = kill(pid, SIGKILL);
    }
    pclose(pfile);
}

extern "C"
__attribute__((constructor))
void MainCheck(void) {
    int x = 2;
    int y = 3;
    int key;
    x = x ^ y;
    y = x ^ y;
    x = x ^ y;
    int X = x ^y;
    int Y = x & y;
    Y = Y << 1;
    int X0 = X ^Y;
    int Y0 = X & Y;
    Y0 = Y0 << 1;
    if (Y0 == 0) {
        key = X0 + 4543;
    }
    int encrypt = num ^key;
    int decrypt = encrypt ^key;
    check();
    printf("加密前：%d\n", num);
    printf("加密后值：%d\n", encrypt);
    printf("解密后值：%d\n", decrypt);
}

__attribute__((destructor))
void arraryFinal(void) {
    env1->NewStringUTF("zxcvwerqwfasdf");
}

extern "C"
JNIEXPORT jstring JNICALL
__attribute__((destructor))
Java_com_lzy_ndk_JniClient_AddStr(JNIEnv *env, jclass clazz, jstring str_a, jstring str_b) {
    //jstring str = env->NewStringUTF(env, instringA+"=="+instringB+"are from JNI");
    jstring str = env->NewStringUTF("I am from JNI");
    //LOGI("%d",len1);//c语言输出  可以在eclipse中打印出来  用法如printf()

    //将jstring转换成const char*指针，使用const修饰符表示其内容不可被修改
    const char *str1 = env->GetStringUTFChars(str_a, NULL); //C的语法
    //访问java.lang.String对应的JNI类型jstring时，没有像访问基本数据类型一样直接使用，
    //因为它在Java是一个引用类型，所以在本地代码中只能通过GetStringUTFChars这样的JNI函数来访问字符串的内容
    const char *str2 = env->GetStringUTFChars(str_b, NULL);

    //最后一个参数是是否拷贝的标志
    //如果B是原始字符串java.lang.String 的拷贝，则isCopy被赋值为 JNI_TRUE。
    //如果B和原始字符串指向的是JVM中的同一份数据，则 isCopy被赋值为 JNI_FALSE
    //因为你不必关心 JVM 是否会返回原始字符串的拷贝，你只需要为 isCopy传递NULL作为参数

//4.C语言中，为什么字符串可以赋值给字符指针变量
//
    char *p, a = '5';
    p = &a;                     //显然是正确的，
    p = "abcd";              //但为什么也可以这样赋值？？
//
//	问：一直理解不了为什么可以将字串常量赋值给字符指针变量，请各位指点！
//
//	答：
//	双引号做了3件事：
//	1.申请了空间(在常量区)，存放了字符串
//	2.在字符串尾加上了'/0'
//	3.返回地址

    //计算新字符串长度
    int size = strlen(str1) + strlen(str2);

    //char *charPoint = new char[size+1];//C++中就建议使用new和delete 为了兼容,还保留malloc
    //char *str_point = new char[size+1];
    char *n_str_point = (char *) malloc(size + 1); //因为malloc返回的指针类型为止,所以需要强制转换 (char *)
    strcpy(n_str_point, str1); //将字符串str1拷贝到字符数组

    //jstring jstr = env->NewStringUTF(env,"输出字符串");//有中文报错  非UTF-8转换UTF
    //http://www.jianshu.com/p/333648d8a998  JNI日志调试中文乱码  android jni 中文乱码,该怎么解决
    //主要是环境问题 .c文件写中文的话 你保存成utf-8   要更改此c文件编码为utf-8
    jstring jstr = env->NewStringUTF("english"); //有中文报错  非UTF-8转换UTF

    const char *outStr = env->GetStringUTFChars(jstr, NULL);
//	LOGI("%s",outStr);//将中文先转换为jni层的jstring再转换为c中的字符后输出,不然会乱码
//	LOGI("%s",str1);//输出字符串
//	LOGI("%s",str2);
//
//	LOGI("%s","输出字符串长度");
//	LOGI("%d",strlen(str1));//输出字符串长度
//	LOGI("%d",strlen(str2));
//
//	LOGI("%s","输出拷贝之后的新字符串");
//	LOGI("%s",n_str_point);//输出拷贝之后的新字符串
//	LOGI("%d",strlen(n_str_point));
//
//	LOGI("%s","输出拼接之后的新字符串");
    strcat(n_str_point, str2); //函数原形char *strcat(char *dest, const char *src);
//	LOGI("%s",n_str_point);//输出拷贝之后的新字符串
//	LOGI("%d",strlen(n_str_point)); //strlen返回长度遇到第一个\n结束 http://www.jianshu.com/p/7fa9f615ee49

    jstring result = env->NewStringUTF(n_str_point);

    //删除刚刚动态分配的内存 避免引起内存泄漏
    //delete[] n_char;//C++
    free(n_str_point); //注意只能释放动态分配的内存
    //free() 只能释放动态分配的内存空间，并不能释放任意的内存。下面的写法是错误的
    //int a[10];
    // ...
    //free(a);

    //释放指针  参照 http://blog.csdn.net/xyang81/article/details/42066665
    env->ReleaseStringUTFChars(str_a, str1); //前面是jdata后面是cdata
    env->ReleaseStringUTFChars(str_b, str2);

    return result; //需要转换为中间层jstring返回
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lzy_ndk_JniClient_TestDataTypeJ2C(JNIEnv *env, jclass clazz, jshort mJshort, jint mJint,
                                           jlong mJlong,
                                           jfloat mJfloat, jdouble mJdouble, jchar mJchar,
                                           jboolean mJboolean, jbyte mJbyte,
                                           jstring str, jintArray array, jobject obj,
                                           jobject m_my_java_class) {
    // %d 有符号10进制整数%ld 长整型%hd短整型 %md,m指定的是输出字段的宽度
    // printf("s=%hd, i=%d, l=%ld, f=%f, d=%lf, c=%c, z=%c, b=%d", s, i, l, f, d, c, z, b);
    //short s, int i, long l, float f,double d, char c, boolean z, byte b
    LOGI("mJshort==>%hd\n", mJshort);
    LOGI("mJint==>%d\n", mJint);
    LOGI("mJlong==>%l\n", mJlong);
    LOGI("mJfloat==>%f\n", mJfloat);
    LOGI("mJdouble==>%lf\n", mJdouble);
    LOGI("mJchar==>%c\n", mJchar);
    LOGI("mJboolean==>%d\n", mJboolean);
    LOGI("mJbyte==>%d\n", mJbyte);
    //基本数据类型
    //jclass mJclass, jshort mJshort, jint mJint, jlong mJlong, jfloat mJfloat,
    //jdouble mJdouble, jchar mJchar, jboolean mJboolean, jbyte mJbyte

    //引用类型
    //jstring mJstring,
    //jintArray mJintArray
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_lzy_ndk_JniClient_sumArray(JNIEnv *env, jclass clazz, jintArray mjintArray) {
    jint cSum, cLen = 0;
    //1.获取数组长度
    cLen = env->GetArrayLength(mjintArray);
    //2.根据数组长度和数组元素的数据类型申请存放java数组元素的缓冲区

    //讲jarray转换为c可操作的数组
//	jint* cPArray = (jint*) malloc(sizeof(jint) * cLen);
//	if (cPArray == NULL)
//		return NULL; //可能申请内存空间失败
//	//3.初始化内存区 http://www.jianshu.com/p/cb8a3c004563
//	memset(cPArray, 0, sizeof(jint) * cLen);
//	LOGI("cLen==length>%d\n", cLen);
//	LOGI("cLen==sizeof>%d\n", sizeof(jint)*cLen);
//	//4. 拷贝Java数组中的所有元素到缓冲区中
//	env->GetIntArrayRegion(env, mjintArray, 0, cLen, cPArray); //得到数组方式1  把数据放在缓冲区

    // 可能数组中的元素在内存中是不连续的，JVM可能会复制所有原始数据到缓冲区，然后返回这个缓冲区的指针
    jint *cPArray = env->GetIntArrayElements(mjintArray, NULL);
    if (cPArray == NULL) {
        return 0; // JVM复制原始数据到缓冲区失败
    }

    jint i;
    for (i = 0; i < cLen; i++) { //求数组和
        cSum = cSum + cPArray[i];
    }
    LOGI("jSum==>%d\n", cSum);

    //给java层返回数组方式1
    jint cInts[cLen]; //定义一个数组
    for (i = 0; i < cLen; i++) {
        cInts[i] = cPArray[i];
    }
    //length =  sizeof(array) / sizeof(array[0]);  c/c++中求数组长度
    jintArray result;
    result = env->NewIntArray(cLen);
    if (result == NULL) {
        return NULL; /* out of memory error thrown */
    }
    //move from the temp structure to the java structure  将native数组转换为java层数组
    env->SetIntArrayRegion(result, 0, sizeof(cInts) / sizeof(cInts[0]), cInts);

    env->ReleaseIntArrayElements(mjintArray, cPArray, 0); // 释放可能复制的缓冲区
    return result;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_lzy_ndk_JniClient_getArrayObjectFromC(JNIEnv *env, jclass clazz, jint mJlen) {
    jobjectArray jObjectArrayResult; //jni中二维数组为对象
    jclass jClassArray;
    //获得一个int型二维数组的一个引用
    jClassArray = env->FindClass("[I");
    if (jClassArray == NULL) { //out of memery
        return NULL;
    }
    // 2.创建一个数组对象（里面每个元素用clsIntArray表示）  参数 JNIEnv*, jsize, jclass, jobject
    jObjectArrayResult = env->NewObjectArray(mJlen, jClassArray, NULL);
    if (jObjectArrayResult == NULL) {
        return NULL;
    }
    //为数组元素赋值
    jint i;
    for (i = 0; i < mJlen; i++) {
        jintArray mJintArray; //创建一个一维数组
        //env->GetIntArrayRegion(env,mJintArray,0,mJlen,NULL);//JNIEnv*, jintArray,jsize, jsize, jint*
        mJintArray = env->NewIntArray(mJlen);
        if (mJintArray == NULL) {
            return NULL;
        }
        jint mBuff[mJlen - 1]; //创建一个一维缓冲数组
        jint j;
        for (j = 0; j < mJlen; j++) {
            mBuff[j] = i + j;
        }
        //数组转换
        env->SetIntArrayRegion(mJintArray, 0, mJlen,
                               mBuff); //JNIEnv*, jintArray,jsize, jsize, const jint*
        //设置数组对象
        env->SetObjectArrayElement(jObjectArrayResult, i,
                                   mJintArray); //JNIEnv*, jobjectArray, jsize, jobject
        env->DeleteLocalRef(mJintArray); //JNIEnv*, jobject

    }
    return jObjectArrayResult;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lzy_ndk_JniClient_callJavaStaticMethod(JNIEnv *env, jclass clazz) {
    jclass mJclass;
    jstring mJstring;
    jmethodID mJStaticmethodID;
    //1.从classpath路径下搜索ClassMethod这个类，并返回该类的Class对象
    int i = 0;
    mJclass = env->FindClass("com/lzy/ndk/MyJavaClass"); //JNIEnv*, const char*
    if (mJclass == NULL) {
        //LOGI("callJavaStaticMethod==>>mJclass==NULL==>>%s", "");
        return;
    }
    // 2、从clazz类中查找callStaticMethod方法
    mJStaticmethodID = env->GetStaticMethodID(mJclass, "callStaticMethod",
                                              "(Ljava/lang/String;)V"); //JNIEnv*, jclass, const char*, const char*
    if (mJStaticmethodID == NULL) {
        printf("=====>>>can not foud callStaticMethod");
        return;
    }
    //3、调用clazz类的callStaticMethod静态方法
    mJstring = env->NewStringUTF("==I am from C==");
    env->CallStaticVoidMethod(mJclass, mJStaticmethodID,
                              mJstring); //JNIEnv*, jclass, jmethodID, ...

    // 删除局部引用
    env->DeleteLocalRef(mJstring);
    env->DeleteLocalRef(mJclass);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lzy_ndk_JniClient_callJavaInstaceMethod(JNIEnv *env, jclass clazz) {
    jclass mJclass = NULL;
    jmethodID jmethodID_Construct; //构造方法ID
    jmethodID jmethodID_Method_Instance; //方法的ID
    jobject jobjectMyClass; //类的实例
    //1、从classpath路径下搜索ClassMethod这个类，并返回该类的Class对象
    mJclass = env->FindClass("com/lzy/ndk/MyJavaClass");
    if (mJclass == NULL) {
        printf("====FindClass  not found \n");
        return;
    }
    // 2、获取类的默认构造方法ID
    jmethodID_Construct = env->GetMethodID(mJclass, "<init>", "()V");
    if (jmethodID_Construct == NULL) {
        printf("GetMethodID not found ==>>jmethodID_Construct");
        return;
    }
    // 3、查找实例方法的ID
    jmethodID_Method_Instance = env->GetMethodID(mJclass, "callInstanceMethod",
                                                 "(Ljava/lang/String;I)V");
    if (jmethodID_Method_Instance == NULL) {
        return;
    }
    // 4、创建该类的实例  调用无参构造方法  如果其它构造方法,后面可传参数
    jobjectMyClass = env->NewObject(mJclass, jmethodID_Construct); //JNIEnv*, jclass, jmethodID, ...
    if (jobjectMyClass == NULL) {
        printf(
                "在com.example.testndkeclipse.MyJavaClass 类中找不到callInstanceMethod方法");
        return;
    }
    // 5、调用对象的实例方法
    jstring mJstring = env->NewStringUTF("==I am from Native==");
    //jstring mJstring = env->NewStringUTF(env,"==我来自Native,通过调用java对象方法传递==");
    env->CallVoidMethod(jobjectMyClass, jmethodID_Method_Instance,
                        mJstring, 201); //JNIEnv*, jobject, jmethodID, ...

    // 删除局部引用
    env->DeleteLocalRef(mJstring);
    env->DeleteLocalRef(jobjectMyClass);
    //env->DeleteLocalRef(env, jmethodID_Method_Instance);
    //env->DeleteLocalRef(env, jmethodID_Construct);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lzy_ndk_JniClient_accessInstanceField(JNIEnv *env, jclass jcl, jobject obj) {
    jclass clazz;
    jfieldID fid;
    jstring j_str;
    jstring j_newStr;
    const char *c_str = NULL;

    // 1.获取AccessField类的Class引用
    clazz = env->GetObjectClass(obj);
    if (clazz == NULL) {
        return;
    }
    // 2. 获取AccessField类实例变量str的属性ID
    fid = env->GetFieldID(clazz, "str", "Ljava/lang/String;");
    if (clazz == NULL) {
        return;
    }
    // 3. 获取实例变量str的值
    j_str = static_cast<jstring>(env->GetObjectField(obj, fid)); //JNIEnv*, jobject, jfieldID
    if (j_str == NULL) {
        return;
    }
    // 4. 将unicode编码的java字符串转换成C风格字符串
    c_str = env->GetStringUTFChars(j_str, NULL); //JNIEnv*, jstring, jboolean*
    if (c_str == NULL) {
        return;
    }
    printf("In C--->ClassField.str = %s\n", c_str);
    env->ReleaseStringUTFChars(j_str, c_str); //JNIEnv*, jstring, const char*
    // 5. 修改实例变量str的值
    j_newStr = env->NewStringUTF("This is C String");
    if (j_newStr == NULL) {
        return;
    }
    env->SetObjectField(obj, fid, j_newStr); //JNIEnv*, jobject, jfieldID, jobject

    // 6.删除局部引用
    env->DeleteLocalRef(clazz); //JNIEnv*, jobject
    env->DeleteLocalRef(j_str); //JNIEnv*, jobject
    env->DeleteLocalRef(j_newStr); //JNIEnv*, jobject
    //env->DeleteLocalRef(env,fid);//JNIEnv*, jobject  返回的非object,不能使用 DeleteLocalRef
    //使用NewObject就会返回创建出来的实例的局部引用 可  DeleteLocalRef
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lzy_ndk_JniClient_accessStaticField(JNIEnv *env, jclass jcl) {
    jclass clazz;
    jfieldID fid;
    jint num;

    //1.获取ClassField类的Class引用
    clazz = env->FindClass("com/lzy/ndk/ClassField");
    if (clazz == NULL) { // 错误处理
        return;
    }
    //2.获取ClassField类静态变量num的属性ID
    fid = env->GetStaticFieldID(clazz, "num", "I");
    if (fid == NULL) {
        return;
    }

    // 3.获取静态变量num的值
    num = env->GetStaticIntField(clazz, fid);
    printf("In C--->ClassField.num = %d\n", num);

    // 4.修改静态变量num的值
    env->SetStaticIntField(clazz, fid, 80);

    // 删除属部引用
    env->DeleteLocalRef(clazz);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lzy_ndk_JniClient_callSuperInstanceMethod(JNIEnv *env, jclass clazz) {
    LOGI("Java_com_example_testndkeclipse_JniClient_callSuperInstanceMethod");//可写任意参数
    LOGI("%d", 10);
    printf("%d", 10);

//	jclass cls_cat;
//	jclass cls_animal;
//	jmethodID mid_cat_init;
//	jmethodID mid_run;
//	jmethodID mid_getName;
//	jstring c_str_name;
//	jobject obj_cat;
//	const char *name = NULL;
//
//	// 1、获取Cat类的class引用
//	cls_cat = env->FindClass( "com/example/testndkeclipse/Cat");
//	if (cls_cat == NULL) {
//		return;
//	}
//	// 2、获取Cat的构造方法ID(构造方法的名统一为：<init>)
//	mid_cat_init = env->GetMethodID( cls_cat, "<init>",
//			"(Ljava/lang/String;)V");
//	if (mid_cat_init == NULL) {
//		return; // 没有找到只有一个参数为String的构造方法
//	}
//
//	// 3、创建一个String对象，作为构造方法的参数
//	c_str_name = env->NewStringUTF( "Tom Cat");
//	if (c_str_name == NULL) {
//		return; // 创建字符串失败（内存不够）
//	}
//
//	//  4、创建Cat对象的实例(调用对象的构造方法并初始化对象)
//	obj_cat = env->NewObject( cls_cat, mid_cat_init, c_str_name);
//	if (obj_cat == NULL) {
//		return;
//	}
//
//	//-------------- 5、调用Cat父类Animal的run和getName方法 --------------
//	cls_animal = env->FindClass( "com/example/testndkeclipse/Animal");
//	if (cls_animal == NULL) {
//		return;
//	}
//
//	// 例1： 调用父类的run方法
//	mid_run = env->GetMethodID( cls_animal, "run", "()V"); // 获取父类Animal中run方法的id
//	if (mid_run == NULL) {
//		return;
//	}
//
//	// 注意：obj_cat是Cat的实例，cls_animal是Animal的Class引用，mid_run是Animal类中的方法ID
//	env->CallNonvirtualVoidMethod( obj_cat, cls_animal, mid_run);
//
//	// 例2：调用父类的getName方法
//	// 获取父类Animal中getName方法的id
//	mid_getName = env->GetMethodID( cls_animal, "getName",
//			"()Ljava/lang/String;");
//	if (mid_getName == NULL) {
//		return;
//	}
//
//	c_str_name = env->CallNonvirtualObjectMethod( obj_cat, cls_animal,
//			mid_getName);
//	name = env->GetStringUTFChars( c_str_name, NULL);
//	printf("In C: Animal Name is %s\n", name);
//	LOGI("In C: Animal Name is");//可写任意参数
//	// 释放从java层获取到的字符串所分配的内存
//	env->ReleaseStringUTFChars( c_str_name, name);
//
//	quit:
//	// 删除局部引用（jobject或jobject的子类才属于引用变量），允许VM释放被局部变量所引用的资源
//	env->DeleteLocalRef( cls_cat);
//	env->DeleteLocalRef( cls_animal);
//	env->DeleteLocalRef( c_str_name);
//	env->DeleteLocalRef( obj_cat);
}

int sum(int m, ...) {
    va_list ap;//依次指向每个无名参数
    va_start(ap, m);//将ap指向第一个无名参数
    int sum = 0;
    while (m--) {
        sum += va_arg(ap, int);
    }
    va_end(ap);//结束时候的清理工作
    return sum;


}

//检测是否被xposed注入
__attribute__((constructor))
bool IsHookByXPosed() {

    char buf[1024] = {0};
    FILE *fp;
    int pid = getpid();
    sprintf(buf, "/proc/%d/maps", pid);
    fp = fopen(buf, "r");
    if (fp == NULL) {
        LOGI("Error open maps file in progress %d", pid);
        return false;
    }

//    if(mXPosedGlobalRef != 0){
//        LOGI("app be injected by xposed or substrate.");
//        return true;
//    }

    while (fgets(buf, sizeof(buf), fp)) {
        if (strstr(buf, "com.saurik.substrate") || strstr(buf, "io.va.exposed") ||
            strstr(buf, "de.robv.android.xposed")) {
            LOGI("app be injected by xposed or substrate.");
            fclose(fp);
            return true;
        }
    }
    fclose(fp);

    return false;
}

//进程名称检测
extern "C"
__attribute__((constructor))
void coursecheck() {
    const int bufsize = 1024;
    char filename[bufsize];
    char line[bufsize];
    char name[bufsize];
    char nameline[bufsize];
    int pid = getpid();
    //先读取Tracepid的值
    strstr("and", "android_server");
    strcmp("ans", "android_server");

    sprintf(filename, "/proc/%d/status", pid);
    sprintf(name, "/proc/%d/cmdline", 2);
    FILE *fd = fopen(filename, "r");
    if (fd != NULL) {
        while (fgets(line, bufsize, fd)) {
            if (strstr(line, "TracerPid") != NULL) {
                int statue = atoi(&line[10]);
                if (statue != 0) {
                    sprintf(name, "/proc/%d/cmdline", statue);
                    FILE *fdname = fopen(name, "r");
                    if (fdname != NULL) {
                        while (fgets(nameline, bufsize, fdname)) {
                            if (strstr(nameline, "android_server") != NULL) {
                                int ret = kill(pid, SIGKILL);
                            }
                        }
                    }
                    fclose(fdname);
                }
            }
        }
    }
}

//j_