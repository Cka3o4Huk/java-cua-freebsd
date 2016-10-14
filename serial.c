/*
 * Copyright (c) 2016, Michael Zhilin
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
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
