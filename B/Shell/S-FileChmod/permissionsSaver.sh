#!/bin/bash

name=savedPermissions

# get the to save file name
if [[ -e $name ]] ; then
	i=1
	while [[ -e $name$i ]] ; do
		let i++
	done
	name=$name$i
fi

touch $name

# read dirs and files, save the permissions

list_alldir(){  
	for file2 in `ls -a $1`  
		do  
			if [ x"$file2" != x"." -a x"$file2" != x".." ];then  
				if [ -d "$1/$file2" ];then  
					list_alldir "$1/$file2"  
				else
					ls -l "$1/$file2" | awk '{print $1 " " substr($9, 3)}' >> $name
				fi
			fi  
		done  
}

list_alldir .
