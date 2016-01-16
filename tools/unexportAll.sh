#!/bin/sh
#
# Display the status of one GPIO


# Display data for one pin
displayDataPin(){
	if [[ ! -e "/sys/class/gpio/gpio$1" ]];then
		echo "***** Pin $1 *****"
		echo "Not exported yet"
		echo "***** *****"
		return #Leave function
	fi

	echo "***** Pin $1 *****"
	echo Direction: `cat /sys/class/gpio/gpio$1/direction`
	echo Edge: `cat /sys/class/gpio/gpio$1/edge`
	echo Value: `cat /sys/class/gpio/gpio$1/value`
	echo "***** *****"
}

# Check whether parameter is valid
isValidParameter(){
	if [[ $# -lt 1 ]];then
		echo "One or more pin must be given"
		exit 1
	fi
}

# Launch program 
main(){
	isValidParameter "$@"
	for pin in "$@";do
		displayDataPin $pin
	done
}


main "$@"


