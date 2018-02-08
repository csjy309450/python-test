#! /bin/bash
declare -a arr
arr=($@)
length=${#arr[@]}
for((i=0;i<${length};i++));
do
	arr[i]="-name *."${arr[i]}
	if [ ${i} != 0 ];then
		arr[i]="-o "${arr[i]}
	fi
done
echo ${arr[@]}
