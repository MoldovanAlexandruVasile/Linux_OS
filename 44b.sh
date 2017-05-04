#!/bin/sh

#44b) Write a shell script that continously reads user names from keyboard
#and for each of them outputs its full name, the number of processes
#and the curent running command.

#Until the name is not "exit" the program will run
while true; do

    #We read the name
    read name

    #Check if the name is equal to exit, if it is, the program will stop
    if [ $name = "exit" ]; then
	exit 0
    fi

    #We test if the user name does exist in the list
    #If it does, then we go into the if
    #If not, we go on else and prin "User does not exist"
    if [ `finger | grep -c "^$name\>"` -gt 0  ]; then

	#We print the full name of the user
	echo Name: `finger | grep "$name" | sed "s/\ \ \ */_/g" | cut -d"_" -f2`

	#Print out how many processes has that user
	aux=`ps -u $name | wc -l`
	#The first line tells what each column represents, and we decrement with
	#one because the number of the lines also representes the number
	#of the processes
	aux=`expr $aux - 1`
	echo Number of processes: $aux

	# "a" will print the number of lines of the command w -u $name
	#w -u $name will return on the first line some detalils about the user
	#On the second line it will be wath each column represents
	#On the las column we will have the current running command
	a=`w -u $name | wc -l`
	lines=`expr $a - 2`

	#In cmd we will have the current command
	cmd=`w -u $name | tail -n $lines | cur -d"s" -f4`
	#We test if there is a current running command
	#If it does, then we will print the command
	#Otherwise we will print "No running command !"
	if [ cmd != "" ]; then
	    echo Current running command: $cmd
	else echo No running command !
	fi
    else echo User does not exist !
    fi
done