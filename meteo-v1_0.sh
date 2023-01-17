#!/bin/bash

# Explain how to use the command
usage() {
	#echo "usage: $0 -t<1-3> -p<1-3> -w -h -m [-F|-G|-S|-A|-O|-Q] [--tab|--val|--arb] [-d <min> <max>(YYYY-MM-DD)]"
	#echo "                  |At least one option set|                    | default to --tab|"

	echo "usage: $0 -f <file> [Filter options] [[Zone options] [Sorting options] [Date options]]"
	echo
	echo "[Filter options] : -t<1-3> -p<1-3> -w -h -m"
	echo "At least one filter option should be set"
	echo
	echo "[Zone options] : -F -G -S -A -O -Q || -g <longitude_min> <longitude_max> -a <latitude_min> <latitude_max>"
	echo "At most one zone option should be set (-g and -a can be set together, integer expected)"
	echo
	echo "[Sorting options] : --tab --val --arb"
	echo "At most one sorting option should be set, default to --tab"
	echo
	echo "[Date options] : -d <min> <max>"
	echo "The date format is YYYY-MM-DD, and min should be inferior to max"
	exit 0
}

error_usage() {
	usage
	exit 1
}


# Check the passed arguments
getopt -a -n "meteoSH" -o \?f:whmFGSAOQd:g::a:: --long t1,t2,t3,p1,p2,p3,tab,val,arb,help -- "$@" > /dev/null 2>&1

file=""
filters=()
zone=""
sorting=""
date_min=""
date_max=""
long_min=""
long_max=""
lat_min=""
lat_max=""

# Parse options
while [[ "$1" != "" ]] ; do
	case $1 in
		# File to read and filter
		-f)
			if [[ ! -f $2 ]] ; then echo "File $2 does not exist"; exit 1; fi
			echo "File set      : $2"
			file="$2"
			shift
		;;
		# Filters options
		-t1|-t2|-t3|-p1|-p2|-p3|-w|-h|-m)
			echo "Filter set    : $1"
			filters+=("$1")
		;;
		# Zone options
		-F|-G|-S|-A|-O|-Q)
			if [[ -z $zone && -z $long_min && -z $lat_min ]] ; then zone="$1"
			else echo "Zone or long/lat already set"; exit 1; fi
			echo "Zone selected : $1"
		;;
		# Sorting options
		--tab|--val|--arb)
			if [[ -z $sorting ]] ; then sorting="$1"; else echo "Sorting method already set"; exit 1; fi
			echo "Sorting mode  : $1"
		;;
		# Date options
		-d)
			if [[ -z $date_min ]] ; then date_min="$2"; else echo "Date min already set"; exit 1; fi
			if [[ -z $date_max ]] ; then date_max="$3"; else echo "Date max already set"; exit 1; fi
			if [[ $(date -d $date_min +%s) -ge $(date -d $date_max +%s) ]] ; then
				echo "Date min should be inferior to date max"; exit 1; 
			fi
			echo "Date min      : $2"
			echo "Date max      : $3"
			shift 2
		;;
		-g)
			if [[ -z $long_min && -z $zone && $2 =~ ^-?[0-9]+$ && $3 =~ ^-?[0-9]+$ ]] ; then 
				long_min="$2"
				long_max="$3"
			else echo "Zone or longitude already set or in wrong format (integer expected)"; exit 1; fi
			if [[ $2 -gt $3 ]] ; then
				echo "Longitude min should be inferior to longitude max ($2 > $3)"; exit 1; 
			fi
			echo "Longitude min : $2"
			echo "Longitude max : $3"
			shift 2
		;;
		-a)
			if [[ -z $lat_min && -z $zone && $2 =~ ^-?[0-9]+$ && $3 =~ ^-?[0-9]+$ ]] ; then 
				lat_min="$2"
				lat_max="$3"
			else echo "Zone or latitude already set or in wrong format (integer expected)"; exit 1; fi
			if [[ $2 -gt $3 ]] ; then
				echo "Latitude min should be inferior to latitude max ($2 > $3)"; exit 1; 
			fi
			echo "Latitude min  : $2"
			echo "Latitude max  : $3"
			shift 2
		;;
		# Help option
		-\?|--help)
			usage
		;;
		# Invalid option
		*)
			echo
			echo
			echo -e "\033[31mInvalid option : $1\033[0m" >&2
			error_usage
		;;
	esac
	shift
done

# Check if all mandatory options are set
if [[ -z $file ]] ; then echo "File should be set (use -f <filename>)"; exit 1; fi
if [[ -z $filters ]] ; then echo "At least one filter should be set (type --help for usage)"; exit 1; fi

if [[ -z $sorting ]] ; then echo "Sorting method not specified, default to --avl"; sorting="--avl"; fi


echo "Filters       : ${#filters[@]}  ( ${filters[@]} )"
echo

tmp_out_file="tmp_file1.csv"
out_file="output.csv"
result_file="result.csv"

# Filter by zone
if [[ ! -z $zone ]] ; then
	echo "Zone filtering : $zone..."
	if [[ -f $out_file ]] ; then 
		in_file=$out_file; 
	else 
		in_file="meteo_filtered_data_v1.csv"
	fi
	# Keep the header
	head $in_file -n 1 > $tmp_out_file
	# Check coordinates to keep only the ones in the zone
	case $zone in
		-F)
			tail -n +2 $in_file | grep ";[4-5][0-9]\.[0-9]*,-*[0-9]\." >> $tmp_out_file
		;;
		-G)
			tail -n +2 $in_file | grep ";[0-9]\.[0-9]*,-5[0-9]" >> $tmp_out_file
		;;
		-S)
			tail -n +2 $in_file | grep ";[4-6][0-9]\.[0-9]*,-[4-6][0-9]" >> $tmp_out_file
		;;
		-A)
			tail -n +2 $in_file | grep ";[1-3][0-9]\.[0-9]*,-[6-9][0-9]" >> $tmp_out_file
		;;
		-O)
			tail -n +2 $in_file | grep ";-[1-6][0-9]\.[0-9]*,[4-9][0-9]" >> $tmp_out_file
		;;
		-Q)
			tail -n +2 $in_file | grep ";-[6-9][0-9]\.[0-9]*," >> $tmp_out_file
		;;
		*)
			echo "Invalid zone : -$zone" >&2
			error_usage
		;;
	esac

	mv $tmp_out_file $out_file
	echo "Zone filtering done"
	echo
fi

#Filter by coordinates
if [[ ! -z $long_min || ! -z $lat_min ]] ; then
	# If latitude is not set, set it to the whole range
	if [[ -z $lat_min ]] ; then
		lat_min=-100
		lat_max=100
	fi
	# If longitude is not set, set it to the whole range
	if [[ -z $long_min ]] ; then
		long_min=-190
		long_max=190
	fi

	echo "Coordinates filtering : $lat_min - $lat_max / $long_min - $long_max..."
	if [[ -f $out_file ]] ; then 
		in_file=$out_file
	else 
		in_file="meteo_filtered_data_v1.csv"
	fi
	# Keep the header
	head $in_file -n 1 > $tmp_out_file

	# Check coordinates to keep only the ones in the zone
	tail -n +2 $in_file | awk -F ';' \
	-v lat_min=$lat_min -v lat_max=$lat_max -v long_min=$long_min -v long_max=$long_max \
	'{ split($10, coords, ","); if (coords[1] >= lat_min && coords[1] <= lat_max && \
	coords[2] >= long_min && coords[2] <= long_max) print }' >> $tmp_out_file

	mv $tmp_out_file $out_file
	echo "Coordinates filtering done"
	echo
fi

# Filter by date
if [[ ! -z $date_min && ! -z $date_max ]] ; then
	echo "Date filtering : $date_min / $date_max..."
	if [[ -f $out_file ]] ; then 
		in_file=$out_file
	else 
		in_file="meteo_filtered_data_v1.csv"
	fi
	# Keep the header
	head $in_file -n 1 > $tmp_out_file
	# Check date to keep only the ones in the range
	tail -n +2 $in_file | awk -F ';' -v date_min=$date_min -v date_max=$date_max \
		'{ split($2, date, "T"); if (date[1] >= date_min && date[1] <= date_max) print }' >> $tmp_out_file
	
	mv $tmp_out_file $out_file
	echo "Date filtering done"
	echo
fi


# Write result into the final file
if [[ -f $out_file ]] ; then 
	mv $out_file $result_file
else
	result_file="meteo_filtered_data_v1.csv"
fi

# Test presence of C programm compiled
# if not, compile it with Makefile
c_name="sort"
if [[ ! -f $c_name ]] ; then
	echo "C programm not found, launching Makefile..."
	if [[ ! -f "Makefile" ]] ; then
		echo "Error : Makefile not found" >&2
		exit 1
	else
		output_make=$(make 2>&1)
		success_make=$?
		if [[ $success_make -ne 0 ]] ; then
			echo "Error with Makefile : $output_make" >&2
			exit 1
		fi
	fi
fi

# Launch C programm for each filter
nb_filter=1
for filter in "${filters[@]}"; do
	echo "Computing filter : $filter"

	base_filename="resultcut"
	filename="$base_filename$filter.csv"
	if [[ $filter =~ ^-t[1-3]$ ]] ; then
		filename="$base_filename-t1-t2-t3.csv"
	fi
	if [[ $filter =~ ^-p[1-3]$ ]] ; then
		filename="$base_filename-p1-p2-p3.csv"
	fi

	case $filter in
		-t1|-t2|-t3)
			if [[ ! -f $filename ]] ; then
				cut -d ";" -f 1,2,11,10 $result_file | grep ";;;$" -v > $filename
			fi
		;;
		-p1|-p2|-p3)
			if [[ ! -f $filename ]] ; then
				cut -d ";" -f 1,2,7,8 $result_file | grep ";;$" -v > $filename
			fi
		;;
		-w)
			cut -d ";" -f 1,2,4,5 $result_file | grep ";;$" -v > $filename
		;;
		-h)
			cut -d ";" -f 1,2,14,10 $result_file > $filename
		;;
		-m)
			cut -d ";" -f 1,2,6,10 $result_file | grep ";$" -v > $filename
		;;
	esac

	#output_c=$(./$c_name -f $filename -o result$nb_filter.csv $filter $sorting 2>&1)
	success=$?
	if [[ $success -ne 0 ]] ; then
		echo "Error while applying filter $filter in the C executable '$output_c'" >&2
		# Check error code to determine the error
		case $success in
			1)
				echo "Options error" >&2
			;;
			2)
				echo "Input file error" >&2
			;;
			3)
				echo "Output file error" >&2
			;;
			4)
				echo "Internal error" >&2
			;;
			126)
				echo "Permission problem (try 'chmod +x $c_name')" >&2
			;;
			127)
				echo "C executable not found (verify that '$c_name' is the correct name)" >&2
			;;
			*)
				echo "Unknown error : $output_c" >&2
			;;
		esac
		exit $success
	fi
	((nb_filter++))
done

echo
echo -e "\033[32mSorting done !\033[0m"
#rm result.csv $base_filename*  2> /dev/null

exit 0