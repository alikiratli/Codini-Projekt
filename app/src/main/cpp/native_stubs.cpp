#include <jni.h>
#include <android/log.h>

#define LOG_TAG "codini_native"

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_codini_MainActivity_nativeInit(JNIEnv* env, jobject obj) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "nativeInit called");
}

JNIEXPORT void JNICALL
Java_com_example_codini_MainActivity_nativeOnSurfaceCreated(JNIEnv* env, jobject obj) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "nativeOnSurfaceCreated called");
}

JNIEXPORT void JNICALL
Java_com_example_codini_MainActivity_nativeOnSurfaceChanged(JNIEnv* env, jobject obj, jint width, jint height) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "nativeOnSurfaceChanged: %d x %d", width, height);
}

JNIEXPORT void JNICALL
Java_com_example_codini_MainActivity_nativeRender(JNIEnv* env, jobject obj) {
    // No rendering in stub
}

JNIEXPORT void JNICALL
Java_com_example_codini_MainActivity_nativeOnTouch(JNIEnv* env, jobject obj, jfloat x, jfloat y) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "nativeOnTouch: %f, %f", x, y);
}

JNIEXPORT void JNICALL
Java_com_example_codini_MainActivity_executeCommand(JNIEnv* env, jobject obj, jstring command) {
    // Command execution stub
}

}
