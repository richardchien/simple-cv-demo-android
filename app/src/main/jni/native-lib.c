#include <jni.h>

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapGrayingR(JNIEnv *env, jobject instance,
                                                                    jintArray colorArray_,
                                                                    jint width, jint height) {
    jint *colorArray = (*env)->GetIntArrayElements(env, colorArray_, NULL);

    // TODO

    (*env)->ReleaseIntArrayElements(env, colorArray_, colorArray, 0);
}