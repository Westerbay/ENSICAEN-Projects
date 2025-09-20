#!/bin/bash

make 

if [ $? -eq 0 ]; then
	./bin/launcher
else
	echo "Compilation Failed !"
fi

