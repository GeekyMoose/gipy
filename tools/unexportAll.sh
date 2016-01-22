#!/bin/sh
#
# Unexport all exported GPIO


# Main function
main(){
	for pin in `ls /sys/class/gpio/ | grep -ie gpio[0-9] | cut -c 5-`;do
		echo $pin > /sys/class/gpio/unexport
	done
}


# Launch program 
main "$@"


