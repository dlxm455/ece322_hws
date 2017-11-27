#! /bin/csh 

while ($#argv)
	set ttynum=`who | grep $argv[1] | wc -l`
	echo $argv[1]\: $ttynum terminals
	shift argv
end
