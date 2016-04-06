#!/bin/sh

clean ()
{
	echo "clean .bak file"
	rm -f *.bak
	
	for i in `ls .`
	do
		if [ -d $i ]; then
			cd $i
			clean
			cd ..
		fi
	done
}

clean