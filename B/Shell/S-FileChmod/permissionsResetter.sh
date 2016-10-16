#!/bin/bash

if [ $# != 2 ]; then
	echo "Usage: $0 <-s|-u> <permissionfile>"
	exit
fi

if [ $1 != '-s' -a $1 != '-r' ]; then
	echo "Usage: $0 <-s|-u> <permissionfile>"
	exit
fi

if [ ! -e $2 ]; then
	echo "Permissionfile not found."
	exit
fi

if [ $1 == '-s' ]; then
	echo -e "Old\t\tCurrent\t\tFile"

	cat $2 | while read line; do

		arr=(${line// / })

#for item in ${arr[@]}

		newattr=$(ls -l ${arr[1]}  | awk '{print $1}')

		if [ "$newattr" != "${arr[0]}" ]; then
			echo -e "${arr[0]}\t$newattr\t${arr[1]}"
		fi
	done
fi

lso() { 
#ls -alG "$@" | awk '{k=0;for(i=0;i<=8;i++)k+=((substr($1,i+2,1)~/[rwx]/)*2^(8-i));if(k)printf(" %0o ",k);print}'; 
	echo $1 | awk '{for(i=0;i<=8;i++)k+=((substr($1,i+2,1)~/[rwx]/)*2^(8-i)); printf("%0o",k);}'; 
}

if [ $1 == '-r' ]; then
	cat $2 | while read line; do

		arr=(${line// / })

		newattr=$(ls -l ${arr[1]}  | awk '{print $1}')

		attroct=$(lso ${arr[0]})

		if [ "$newattr" != "${arr[0]}" ]; then
			echo -e "${arr[1]} permissions reset from $newattr to ${arr[0]}"
			chmod $attroct ${arr[1]}
		fi
	done
fi
