#include <stdio.h>
#include <jni.h>
#include <telldus-core.h>

#include <com_homeki_core_device_tellstick_TellStickNative.h>

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_open(JNIEnv* env, jobject jobj) {
	tdInit();
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_close(JNIEnv* env, jobject jobj) {
	tdClose();
}

JNIEXPORT jintArray JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_getDeviceIds(JNIEnv* env, jobject jobj) {
	int count = tdGetNumberOfDevices();
	jint ids[count];

	for (int i = 0; i < count; i++)
		ids[i] = tdGetDeviceId(i);

	jintArray result = env->NewIntArray(count);
	env->SetIntArrayRegion(result, 0, count, ids);

	return result;
}
