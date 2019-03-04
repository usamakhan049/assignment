// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <jni.h>

extern "C"
{
    JNIEXPORT void JNICALL Java_com_eegeo_options_OptionsViewJniMethods_CloseButtonSelected(
        JNIEnv* jenv, jobject obj,
        jlong nativeObjectPtr);

    JNIEXPORT void JNICALL Java_com_eegeo_options_OptionsViewJniMethods_StreamOverWifiToggled(
        JNIEnv* jenv, jobject obj,
        jlong nativeObjectPtr);

    JNIEXPORT void JNICALL Java_com_eegeo_options_OptionsViewJniMethods_CachingEnabledToggled(
        JNIEnv* jenv, jobject obj,
        jlong nativeObjectPtr);

    JNIEXPORT void JNICALL Java_com_eegeo_options_OptionsViewJniMethods_ClearCacheSelected(
        JNIEnv* jenv, jobject obj,
        jlong nativeObjectPtr);

    JNIEXPORT void JNICALL Java_com_eegeo_options_OptionsViewJniMethods_ClearCacheTriggered(
        JNIEnv* jenv, jobject obj,
        jlong nativeObjectPtr);

    JNIEXPORT void JNICALL Java_com_eegeo_options_OptionsViewJniMethods_PlayTutorialAgainSelected(
        JNIEnv* jenv, jobject obj,
        jlong nativeObjectPtr);
}
