#include "tellstickeventqueue.h"

#include <com_homeki_core_device_tellstick_TellStickNative.h>
#include <jni.h>
#include <telldus-core.h>

#include <iostream>
#include <string>
#include <sstream>

#define SWITCH 1
#define DIMMER 2

static int deviceCallbackId = 0;
static int sensorCallbackId = 0;
static TellstickEventQueue* evq;

jint throwTellStickException(JNIEnv* env, const char message[]) {
	const char className[] = "com/homeki/core/device/tellstick/TellStickNativeException";
	jclass exClass = env->FindClass(className);

	if (exClass == NULL) {
		std::cout << "Exception class not found, don't know what to do now. Panic! Abort!" << std::endl;
        return -1;
	}

	return env->ThrowNew(exClass, message);
}

bool checkIfFailed(JNIEnv* env, int result) {
	if (result < 0 && result != TELLSTICK_SUCCESS) {
		char* errorString = tdGetErrorString(result);
		throwTellStickException(env, errorString);
		tdReleaseString(errorString);
		return true;
	}
	return false;
}

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

	std::cout << "NATIVE: Sensor event received." << std::endl;

	switch (dataType) {	
	case TELLSTICK_TEMPERATURE:
		event << " " << id;
		event << " " << data; 
		event << " " << timestamp;
		std::cout << "NATIVE: Sensor event data:";
		std::cout << " " << id;
		std::cout << " " << data;
		std::cout << " " << timestamp;
		std::cout << std::endl;
		break;
	default:
		std::cout << "NATIVE: Returned without action, not a TELLSTICK_TEMPERATURE." << std::endl;
		return;
	}

	std::cout << "NATIVE: Before evq->addEvent()" << std::endl;
	evq->addEvent(event.str());
	std::cout << "NATIVE: After evq->addEvent()" << std::endl;
}

int addDevice(JNIEnv* env, int type, int house, int unit) {
	jint id = tdAddDevice();

	if (checkIfFailed(env, id))
		return -1;

	char houseStr[10];
	char unitStr[10];

	sprintf(houseStr, "%d", house);
	sprintf(unitStr, "%d", unit);

	if (type == SWITCH) {
		if (!tdSetModel(id, "selflearning-switch"))
			return throwTellStickException(env, "Failed to set model 'selflearning-switch'.");
	}
	else if (type == DIMMER) {
		if (!tdSetModel(id, "selflearning-dimmer"))
			return throwTellStickException(env, "Failed to set model 'selflearning-dimmer'.");
	}

	if (!tdSetProtocol(id, "arctech"))
		return throwTellStickException(env, "Failed to set protocol 'arctech'.");

	if (!tdSetDeviceParameter(id, "house", houseStr))
		return throwTellStickException(env, "Failed to set device parameter 'house'.");

	if (!tdSetDeviceParameter(id, "unit", unitStr))
		return throwTellStickException(env, "Failed to set device parameter 'unit	'.");

	return id;
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_open(JNIEnv* env, jclass jobj) {
	evq = new TellstickEventQueue();
	tdInit();
	deviceCallbackId = tdRegisterDeviceEvent(deviceEvent, NULL);
	sensorCallbackId = tdRegisterSensorEvent(sensorEvent, NULL);

	if (checkIfFailed(env, deviceCallbackId))
		return;
	if (checkIfFailed(env, sensorCallbackId))
		return;
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_close(JNIEnv* env, jclass jobj) {
	// we ignore negative return values here, not much to do if we fail to unregister,
	// Homeki is most probably shutting down anyway
	tdUnregisterCallback(deviceCallbackId);
	tdUnregisterCallback(sensorCallbackId);

	tdClose();

	delete evq;
}

JNIEXPORT jintArray JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_getDeviceIds(JNIEnv* env, jclass jobj) {
	int count = tdGetNumberOfDevices();

	if (checkIfFailed(env, count) || count == 0)
		return NULL;

	jint ids[count];

	for (int i = 0; i < count; i++) {
		ids[i] = tdGetDeviceId(i);

		if (checkIfFailed(env, ids[i]))
			return NULL;
	}

	jintArray result = env->NewIntArray(count);
	env->SetIntArrayRegion(result, 0, count, ids);

	return result;
}

JNIEXPORT jstring JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_getDeviceType(JNIEnv* env, jclass cl, jint id) {
	// dont use checkIfFailed() here as I'm not sure if the OR'ing of values can result in a
	// negative int even though the tdMethods was a success
	int features = tdMethods((int)id, TELLSTICK_DIM | TELLSTICK_TURNON);
	std::string type;

	if (features & TELLSTICK_DIM)
		type = "dimmer";
	else if (features & TELLSTICK_TURNON)
		type = "switch";

	return env->NewStringUTF(type.c_str());
}

JNIEXPORT jint JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_addSwitch(JNIEnv* env, jclass cl, jint house, jint unit) {
	return addDevice(env, SWITCH, house, unit);
}

JNIEXPORT jint JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_addDimmer(JNIEnv* env, jclass cl, jint house, jint unit) {
	return addDevice(env, DIMMER, house, unit);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_learn(JNIEnv* env, jclass cl, jint id) {
	int result = tdLearn(id);
	checkIfFailed(env, result);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_removeDevice(JNIEnv* env, jclass cl, jint id) {
	if (!tdRemoveDevice(id))
		throwTellStickException(env, "Failed to remove device from tellstick.conf.");
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_turnOn(JNIEnv* env, jclass cl, jint id) {
	int result = tdTurnOn((int)id);
	checkIfFailed(env, result);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_turnOff(JNIEnv* env, jclass cl, jint id) {
	int result = tdTurnOff((int)id);
	checkIfFailed(env, result);
}

JNIEXPORT void JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_dim(JNIEnv* env, jclass cl, jint id, jint level) {
	int result = tdDim((int)id, (unsigned char)level);
	checkIfFailed(env, result);
}

JNIEXPORT jstring JNICALL Java_com_homeki_core_device_tellstick_TellStickNative_getEvent(JNIEnv* env, jclass jc) {
	std::string event = evq->getEvent();
	return env->NewStringUTF(event.c_str());
}

