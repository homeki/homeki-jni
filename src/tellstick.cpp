#include <string>
#include <stdio.h>
#include <sstream>

#include <jni.h>
#include <telldus-core.h>

#include <com_homeki_core_device_tellstick_TellStickNative.h>
#include "tellstickeventqueue.h"

#define SWITCH 1
#define DIMMER 2

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
		return;
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
		return;
	}

	evq->addEvent(event.str());
}

int addDevice(int type, int house, int unit) {
	jint id = tdAddDevice();

	if (id < 0)
		return id;

	char houseStr[10];
	char unitStr[10];

	sprintf(houseStr, "%d", house);
	sprintf(unitStr, "%d", unit);

	if (type == SWITCH)
		tdSetModel(id, "selflearning-switch");
	else if (type == DIMMER)
		tdSetModel(id, "selflearning-dimmer");

	tdSetProtocol(id, "arctech");
	tdSetDeviceParameter(id, "house", houseStr);
	tdSetDeviceParameter(id, "unit", unitStr);

	return id;
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

JNIEXPORT jint JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_addSwitch(JNIEnv* env, jclass cl, jint house, jint unit) {
	return addDevice(SWITCH, house, unit);
}

JNIEXPORT jint JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_addDimmer(JNIEnv* env, jclass cl, jint house, jint unit) {
	return addDevice(DIMMER, house, unit);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_learn(JNIEnv* env, jclass cl, jint id) {
	tdLearn(id);
}

JNIEXPORT jboolean JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_removeDevice(JNIEnv* env, jclass cl, jint id) {
	return tdRemoveDevice(id);
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

