#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <string>

#include "SharedLibrary/SharedLibrary.h"

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env, jobject thiz )
{
	SharedLibrary sharedLibrary;
	std::string stringFromSharedLibrary(sharedLibrary.GetPlatformABI());
	jstring result = env->NewStringUTF(stringFromSharedLibrary.c_str());
	return result;
}
