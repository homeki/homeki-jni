#include <string>
#include <stdio.h>

#include <jni.h>
#include <telldus-core.h>

#include <com_homeki_core_device_tellstick_TellStickNative.h>

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_open(JNIEnv* env, jclass jobj) {
	tdInit();
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_close(JNIEnv* env, jclass jobj) {
	tdClose();
}

JNIEXPORT jintArray JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_getDeviceIds(JNIEnv* env, jclass jobj) {
	int count = tdGetNumberOfDevices();
	jint ids[count];

	for (int i = 0; i < count; i++)
		ids[i] = tdGetDeviceId(i);

	jintArray result = env->NewIntArray(count);
	env->SetIntArrayRegion(result, 0, count, ids);

	return result;
}

JNIEXPORT jstring JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_getDeviceType(JNIEnv* env, jclass cl, jint id) {
	int features = tdMethods((int)id, TELLSTICK_DIM | TELLSTICK_TURNON);
	std::string type;

	if (features & TELLSTICK_DIM)
		type = "dimmer";
	else if (features & TELLSTICK_TURNON)
		type = "switch";

	return env->NewStringUTF(type.c_str());
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_turnOn(JNIEnv* env, jclass cl, jint id) {
	tdTurnOn((int)id);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_turnOff(JNIEnv* env, jclass cl, jint id) {
	tdTurnOff((int)id);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_dim(JNIEnv* env, jclass cl, jint id, jint level) {
	tdDim((int)id, (unsigned char)level);
}
