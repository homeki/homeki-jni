#include <string>
#include <stdio.h>
#include <sstream>

#include <jni.h>
#include <telldus-core.h>

#include <com_homeki_core_device_tellstick_TellStickNative.h>
#include "tellstickeventqueue.h"

static int callbackId = 0;
static TellstickEventQueue* evq;

void deviceEvent(int deviceId, int method, const char* data, int callbackId, void* context) {
	std::stringstream event;
	event << deviceId << " ";

	switch (method) {
	case TELLSTICK_TURNON:
		event << "true";
		break;
	case TELLSTICK_TURNOFF:
		event << "false";
		break;
	case TELLSTICK_DIM:
		event << data;
		break;
	default:
		event << "(unknown)";
		break;
	}

	evq->addEvent(event.str());
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_open(JNIEnv* env, jclass jobj) {
	evq = new TellstickEventQueue();
	tdInit();
	callbackId = tdRegisterDeviceEvent(deviceEvent, NULL);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_close(JNIEnv* env, jclass jobj) {
	tdUnregisterCallback(callbackId);
	tdClose();
	delete evq;
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

	if (features & TELLSTICK_DIM) {
		type = "dimmer";
	} else if (features & TELLSTICK_TURNON) {
		std::string name = tdGetName((int)id);

		if (name.size() >= 12 && name.substr(name.size() - 11, 11) == "-fakedimmer")
			type = "fakedimmer";
		else
			type = "switch";
	}

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

JNIEXPORT jstring JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_getEvent(JNIEnv* env, jclass jc) {
	evq->ensureSemaphores();

	std::string event = evq->getEvent();
	return env->NewStringUTF(event.c_str());
}

