#include <string>
#include <stdio.h>
#include <sstream>

#include <jni.h>
#include <telldus-core.h>

#include <com_homeki_core_device_tellstick_TellStickNative.h>
#include "tellstickeventqueue.h"

static int deviceCallbackId = 0;
static int sensorCallbackId = 0;
static TellstickEventQueue* evq;

void deviceEvent(int deviceId, int method, const char* data, int callbackId, void* context) {
	std::stringstream event;
	event << "device" << " " << deviceId << " ";

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

void sensorEvent(const char* protocol, const char* model, int id, int dataType, const char* data, int timestamp, int callbackId, void* context) {
	std::stringstream event;
	event << "sensor";

	switch (dataType) {	
	case TELLSTICK_TEMPERATURE:
		event << " " << id;
		event << " " << data; 
		event << " " << timestamp; 
		break;
	default:
		break;
	}

	evq->addEvent(event.str());
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_open(JNIEnv* env, jclass jobj) {
	evq = new TellstickEventQueue();
	tdInit();
	deviceCallbackId = tdRegisterDeviceEvent(deviceEvent, NULL);
	sensorCallbackId = tdRegisterSensorEvent(sensorEvent, NULL);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_close(JNIEnv* env, jclass jobj) {
	tdUnregisterCallback(deviceCallbackId);
	tdUnregisterCallback(sensorCallbackId);
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

JNIEXPORT jstring JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_getEvent(JNIEnv* env, jclass jc) {
	std::string event = evq->getEvent();
	return env->NewStringUTF(event.c_str());
}

