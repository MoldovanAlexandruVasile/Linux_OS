arg=`cat Procese/Intermediar.txt`

if [ -d $arg ]; then

	aux=`find $arg`
	file $aux | grep -v "directory" | wc -l
	rm -R Procese/Intermediar.txt
	touch Procese/Intermediar.txt

fi
