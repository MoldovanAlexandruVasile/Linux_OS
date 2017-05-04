#!/bin/sh

#44a) Write a shell script that reads a folder name from the keyboard
#and computes the average number of lines from each text file inside
#it and its sufolders

#We read the directory
echo The directory:
read folder

#We test if the directory exists, if not, we output that is an
#invalid directory
if [ ! -d $folder ]; then
    echo Invalid directory !
    exit 1
fi

#the number of text files
n=0

#The total number of lines from the directory
suma=0

#We got all the content from the directory
find $folder |
{
while read f; do
    
    #if f is an ASCII text file
    if [ `file $f | grep -c "ASCII"` -gt 0 ]; then
	#We increment the number of text files
	nr=`expr $nr + 1`

	#We count how many lines there are in the text file
	aux=`wc -l $f | cut -d" " -f1`

	#We add the number of lines
	suma=`expr $suma + $aux`
    fi
done

#If there was at least one text file
if [ $nr -ne 0 ]; then
    #We compute the average
    average=$(($suma/$nr))
    echo The average is: $average
fi
}