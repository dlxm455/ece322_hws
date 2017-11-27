#!/bin/csh

echo "Started $0"

##############################################
##############################################
# TODO: add a check for the correct number 
# of arguments and print usage is not there
##############################################
##############################################
if ($#argv != 3) then 
	echo "Usage: $0 <c_level> <# ele> <data_file>"
	echo " number of  argv: $#argv"
	exit 1
endif



##############################################
# arguments
# <c_level> <# ele> <data_file>
##############################################

# programs
set sprog=`pwd`/mySort
set mprog=`pwd`/myMerge

##############################################
# locations
# temploc: path to temporary storage, 
#          I changed the temploc so that you 
#          do not clobber other peoples files
# sfileh: head part of the temp files name
# sfileext: extension of the temp files name
# ofile: final output filename
##############################################
set tmploc=~/tmp
set sfileh=sort_
set sfileext=.txt322
set ofile=sout.txt322

# create the temploc if needed
if (! -e $tmploc) /bin/mkdir $tmploc

##############################################
# variables
# clevel: concurrent level, i.e. number of mySorts
# nele: total number of strings
# dfile: input data file to sort
##############################################
set clevel=$argv[1]
set nele=$argv[2]
set dfile=$argv[3]


##############################################
# numeach: set to how many each mySort does
##############################################
set numeach=0
@ numeach = $nele / $clevel

##############################################
# flist: emptty wordlist to store the temp filenames
# start: start line (initial value is 1)
# end: end line (initial value is $numeach
##############################################
set flist=()
set start=1
set end=$numeach

##############################################
# loop for clevel times and run mySort each time
##############################################
while ($clevel)

# set the temp filename and add to flist
	set sfile="$tmploc/$sfileh$clevel$sfileext"
	set flist = ($flist $sfile)

# create and run the command
	echo "/usr/bin/sed -n '$start,$end p' $dfile | $sprog $numeach $sfile"
	/usr/bin/sed -n "$start , $end p" $dfile | $sprog $numeach $sfile

# update start and end
	@ start = $start + $numeach
	@ end = $end + $numeach

# decrement clevel
	@ clevel--
end

##############################################
# merge
# ofname: output file path and name
##############################################
echo "merging  $flist"
set ofname=$tmploc/$ofile

# check if output file exists and is a file and delete
# create an empty version of the output file
if (-e $ofname && -f $ofname) /bin/rm $ofname
/usr/bin/touch $ofname

##############################################
##############################################
# TODO: write the code for the simple merge
##############################################
##############################################
#set numout=0
#while ($#flist) 
#	cat $ofname $flist[1] | $mprog $numout $numeach $ofname
#	@ numout = $numout + $numeach
#	shift flist
#end
 
##############################################
##############################################
# TODO: write the code for the proper merge
##############################################
##############################################
echo "merging with method2"
set ofile2=sout2.txt322
set ofname2=$tmploc/$ofile2
if (-e $ofname2 && -f $ofname2) /bin/rm $ofname2

while ($#flist > 1)
	echo "flist number: $#flist"
	set mfile="$flist[1]:r_$#flist$sfileext"
	set flist=($flist $mfile)
	set wc1list=`wc -l $flist[1]`
	set num1=$wc1list[1]
	set wc2list=`wc -l $flist[2]`
	set num2=$wc2list[1]
	echo "cat $flist[1] $flist[2] | $mprog $num1 $num2 $mfile"
	cat $flist[1] $flist[2] | $mprog $num1 $num2 $mfile
	shift flist
	shift flist
end

mv $flist[1] $ofname2

echo "Done $0"
exit 0

