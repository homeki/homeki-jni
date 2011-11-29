#include <stdio.h>
#include <jni.h>
#include "java/com_homeki_core_device_tellstick_TellStickNative.h"

JNIEXPORT void JNICALL 
Java_com_homeki_core_device_tellstick_TellStickNative_print(JNIEnv *env, jobject obj) {
     printf("Hello World from C!\n");
     return;
}

