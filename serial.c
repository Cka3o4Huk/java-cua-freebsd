/*
 * serial.c
 *
 *  Created on: Oct 10, 2016
 *      Author: mizhka
 */

#include <dirent.h>
#include <string.h>

#include "serial.h"

#define DEV		"/dev"
#define	PREFIX		"cua"

#define FILEFILTER(name)						\
	if((strcmp(name, ".") == 0)   ||				\
	    (strcmp(name, "..") == 0) ||				\
	    (strncmp(PREFIX, name, strlen(PREFIX)) != 0) ||		\
	    (strlen(name) > 5 && 					\
		(strcmp(name + strlen(name) - 5, ".init") == 0 || 	\
		 strcmp(name + strlen(name) - 5, ".lock") == 0 )))	\
		continue;

JNIEXPORT jobjectArray JNICALL
Java_ru_unclebear_test_serial_NativeMain_getSerials
  (JNIEnv *env, jclass cls)
{
	int		 len, i;
	char		 tmp[MAXNAMLEN + 6];
	struct dirent	*dp;
	DIR		*dirp;
	jclass		 stringClass;
	jobjectArray	 ret;
	jstring		 val;

	len = 0;

	stringClass = (*env)->FindClass(env,"java/lang/String");
	if(stringClass == NULL) {
		return (NULL);
	}

	/* Count files */
	dirp = opendir(DEV);
	if(dirp == NULL)
		return (NULL);

	while((dp = readdir(dirp)) != NULL) {
		FILEFILTER(dp->d_name);
		len++;
	}

	closedir(dirp);

	/* Create array */
	ret = (*env)->NewObjectArray(env, len, stringClass, NULL);
	(*env)->DeleteLocalRef(env, stringClass);

	/* Fill array */
	dirp = opendir(DEV);
	if(dirp == NULL)
		return(NULL);

	i = 0;
	while((dp = readdir(dirp)) != NULL) {
		FILEFILTER(dp->d_name);
		len--;
		if(len < 0)
			break;
		strcpy(tmp, DEV "/");
		strcat(tmp, dp->d_name);
		val = (*env)->NewStringUTF(env, tmp);
		(*env)->SetObjectArrayElement(env, ret, i++, val);
		(*env)->DeleteLocalRef(env, val);
	}

	closedir(dirp);
	return ret;
}
