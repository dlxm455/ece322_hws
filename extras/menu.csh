#! /bin/csh

while (1)
	echo
	echo
	echo
	echo ==================== MENU =========================
	echo "a. List all files in the present working director"
	echo "b. Display today's date and time"
	echo "c. Get number of terminals for users"
	echo "d. Check pathname is file or directory"
	echo "e. Create backups"
	echo "f. Start a telnet session"
	echo "g. Start an ftp session"
	echo "h. Exit"
	echo ---------------------------------------------------
	set req=$<

	switch($req)
		case [aA]:
			echo
			ls .
			breaksw
		case [bB]:
			echo
			date
			breaksw
		case [cC]:
			echo
			echo "Please enter a list of login names:"
			set req_c=($<)
			./menu_c.csh $req_c
			breaksw
		case [dD]:
			echo
			echo "Please enter a list of pathnames:"
			set req_d=($<)
			while ($#req_d)
				if (-f $req_d[1]) then
					echo "$req_d[1] is a file"
				else if (-d $req_d[1]) then
					echo "$req_d[1] is a directory"
				else 
					echo "$req_d[1] is neither a file nor a directory"
				endif
				shift req_d
			end
			breaksw
		case [eE]:
			echo
			echo "Please enter a list of files to backup"
			set req_e=($<)
			set backpath='~/.backup'
			if (! -e $backpath) mkdir $backpath
			while($#req_e)
				if (-e $req_e[1]) then
					cp $req_e[1] $backpath/$req_e[1]:t
				else
					echo $req_e[1] does not exist
				endif	
			shift req_e
			end
			breaksw
		case [fF]:
			echo
			echo "Enter the hostname (and port number if needed) for telnet:"
			set req_f=($<)
			telnet $req_f
			breaksw
		case [gG]:
			echo
			echo "Enter the site name for ftp:"
			set req_g=$<
			ftp $req_g
			breaksw	
		case [hH]:
			exit 0
			breaksw
		default:
			echo "Error--no such option"
			breaksw
	endsw
end
