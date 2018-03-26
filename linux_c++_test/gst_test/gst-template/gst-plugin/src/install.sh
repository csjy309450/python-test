#!/bin/bash

cd .libs
PLUGIN_NAME="myfilter"
LIB_NAME='lib'${PLUGIN_NAME}'.so'
LA_NAME='libs/lib'${PLUGIN_NAME}'.la'
RESULT=`find $(LIB_NAME)`

echo ${PLUGIN_NAME}
echo ${LIB_NAME}
echo ${LA_NAME}
echo ${RESULT}

if [ ${RESULT}=${LIB_NAME} ]; then 
	echo "OK";
else
	echo "NO"
fi
