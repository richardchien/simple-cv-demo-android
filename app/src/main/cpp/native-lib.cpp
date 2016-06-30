#include <jni.h>
#include <iostream>
#include "simplecv/scv.h"

void bitmapGraying(JNIEnv *env, jintArray colorArray_, jint width, jint height,
                   SCV_GRAYING_TYPE type);

void bitmapBinary(JNIEnv *env, jintArray colorArray_, jint width, jint height);

void bitmapEqualizationHist(JNIEnv *env, jintArray colorArray_, jint width, jint height);

void bitmapGaussianSmooth(JNIEnv *env, jintArray colorArray_, jint width, jint height);

void bitmapCanny(JNIEnv *env, jintArray colorArray_, jint width, jint height);

void bitmapInverse(JNIEnv *env, jintArray colorArray_, jint width, jint height) ;

using namespace std;

extern "C" {

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapGrayingR(JNIEnv *env, jobject instance,
                                                                    jintArray colorArray_,
                                                                    jint width, jint height) {
    bitmapGraying(env, colorArray_, width, height, SCV_GRAYING_R);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapGrayingG(JNIEnv *env, jobject instance,
                                                                    jintArray colorArray_,
                                                                    jint width, jint height) {
    bitmapGraying(env, colorArray_, width, height, SCV_GRAYING_G);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapGrayingB(JNIEnv *env, jobject instance,
                                                                    jintArray colorArray_,
                                                                    jint width, jint height) {
    bitmapGraying(env, colorArray_, width, height, SCV_GRAYING_B);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapGrayingMax(JNIEnv *env, jobject instance,
                                                                      jintArray colorArray_,
                                                                      jint width, jint height) {
    bitmapGraying(env, colorArray_, width, height, SCV_GRAYING_MAX);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapGrayingAVG(JNIEnv *env, jobject instance,
                                                                      jintArray colorArray_,
                                                                      jint width, jint height) {
    bitmapGraying(env, colorArray_, width, height, SCV_GRAYING_AVG);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapGrayingWAVG(JNIEnv *env,
                                                                       jobject instance,
                                                                       jintArray colorArray_,
                                                                       jint width, jint height) {
    bitmapGraying(env, colorArray_, width, height, SCV_GRAYING_W_AVG);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapBinary(JNIEnv *env,
                                                                  jobject instance,
                                                                  jintArray colorArray_,
                                                                  jint width, jint height) {
    bitmapBinary(env, colorArray_, width, height);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapInverse(JNIEnv *env,
                                                                   jobject instance,
                                                                   jintArray colorArray_,
                                                                   jint width, jint height) {
    bitmapInverse(env, colorArray_, width, height);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapEqualizationHist(JNIEnv *env,
                                                                            jobject instance,
                                                                            jintArray colorArray_,
                                                                            jint width,
                                                                            jint height) {
    bitmapEqualizationHist(env, colorArray_, width, height);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapGaussianSmooth(JNIEnv *env,
                                                                          jobject instance,
                                                                          jintArray colorArray_,
                                                                          jint width, jint height) {
    bitmapGaussianSmooth(env, colorArray_, width, height);
}

JNIEXPORT void JNICALL
Java_im_r_1c_android_simplecvdemo_MainActivity_nativeBitmapCanny(JNIEnv *env,
                                                                 jobject instance,
                                                                 jintArray colorArray_,
                                                                 jint width, jint height) {
    bitmapCanny(env, colorArray_, width, height);
}

}

int red(int color) {
    return (color >> 16) & 0xFF;
}

int greed(int color) {
    return (color >> 8) & 0xFF;
}

int blue(int color) {
    return color & 0xFF;
}

int rgb(int r, int g, int b) {
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

void copyColorArrayToScvImage(JNIEnv *env, int *colorArray, ScvImage *image) {
    for (int iy = 0; iy < image->height; iy++) {
        for (int ix = 0; ix < image->width; ix++) {
            int color = colorArray[iy * image->width + ix];
            scvSetPixel(image, ix, iy, scvPixel(red(color), greed(color), blue(color)));
        }
    }
}

void copyScvImageToColorArray(JNIEnv *env, ScvImage *image, int *colorArray) {
    for (int iy = 0; iy < image->height; iy++) {
        for (int ix = 0; ix < image->width; ix++) {
            ScvPixel pxl = scvGetPixel(image, ix, iy);
            int color = rgb(pxl.r, pxl.g, pxl.b);
            colorArray[iy * image->width + ix] = color;
        }
    }
}

void bitmapGraying(JNIEnv *env, jintArray colorArray_, jint width, jint height,
                   SCV_GRAYING_TYPE type) {
    ScvImage *image = scvCreateImage(scvSize(width, height));
    int *colorArray = env->GetIntArrayElements(colorArray_, false);
    copyColorArrayToScvImage(env, colorArray, image);
    scvGraying(image, image, type);
    copyScvImageToColorArray(env, image, colorArray);
    scvReleaseImage(image);
}

void bitmapBinary(JNIEnv *env, jintArray colorArray_, jint width, jint height) {
    ScvImage *image = scvCreateImage(scvSize(width, height));
    int *colorArray = env->GetIntArrayElements(colorArray_, false);
    copyColorArrayToScvImage(env, colorArray, image);
    scvThreshold(image, image, SCV_GRAYING_W_AVG);
    copyScvImageToColorArray(env, image, colorArray);
    scvReleaseImage(image);
}

void bitmapInverse(JNIEnv *env, jintArray colorArray_, jint width, jint height) {
    ScvImage *image = scvCreateImage(scvSize(width, height));
    int *colorArray = env->GetIntArrayElements(colorArray_, false);
    copyColorArrayToScvImage(env, colorArray, image);
    scvInverse(image, image);
    copyScvImageToColorArray(env, image, colorArray);
    scvReleaseImage(image);
}

void bitmapEqualizationHist(JNIEnv *env, jintArray colorArray_, jint width, jint height) {
    ScvImage *image = scvCreateImage(scvSize(width, height));
    int *colorArray = env->GetIntArrayElements(colorArray_, false);
    copyColorArrayToScvImage(env, colorArray, image);
    ScvHistogram *hist = scvCreateHist(SCV_GRAYING_W_AVG);
    scvCalcHist(image, hist);
    scvEqualizeHist(image, hist, image);
    copyScvImageToColorArray(env, image, colorArray);
    scvReleaseHist(hist);
    scvReleaseImage(image);
}

void bitmapGaussianSmooth(JNIEnv *env, jintArray colorArray_, jint width, jint height) {
    ScvImage *image = scvCreateImage(scvSize(width, height));
    int *colorArray = env->GetIntArrayElements(colorArray_, false);
    copyColorArrayToScvImage(env, colorArray, image);
    scvSmooth(image, image, SCV_SMOOTH_GAUSSIAN);
    copyScvImageToColorArray(env, image, colorArray);
    scvReleaseImage(image);
}

void bitmapCanny(JNIEnv *env, jintArray colorArray_, jint width, jint height) {
    ScvImage *image = scvCreateImage(scvSize(width, height));
    int *colorArray = env->GetIntArrayElements(colorArray_, false);
    copyColorArrayToScvImage(env, colorArray, image);
    scvCanny(image, image);
    copyScvImageToColorArray(env, image, colorArray);
    scvReleaseImage(image);
}
