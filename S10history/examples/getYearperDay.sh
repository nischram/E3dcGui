#! /bin/bash
#
# get the sum of all days of one year from S10 solar power station
# you can redirect that output to a file and use S10toMysql.pl to
# read all those values to a mysql database
#
# Copyright Ralf Lehmann
# 02.2017 - initial version
# 09.2017 - Issue #20 (Nischram)
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
PROG=../S10history

#alias errecho='>&2 echo'
errecho(){
        >&2 echo $*
}

usage()
{
>&2 cat << EOT
usage $0 year
year > 2011
EOT
exit 1
}

if [ $# -ne 1 ] || [  "$1" -le "2011" ]; then
	errecho "ERROR: Invalid year as argument"
	usage
fi

if [ ! -e $PROG ]; then
	errecho "ERROR: can't find S10reader program \"$PROG\""
	exit 1
fi

# check if program is ok, user and name is correct, by calling the whole year
if ! $PROG -y $1; then
	errecho "ERROR: program $PROG returns errors"
	exit 1
fi

i=1;
while [ $i -le 366 ]; do
	#echo "$i";
	j=4;
	while ! $PROG -y $1 -m 1 -b -d $i ; do
		errecho "WARNING: retrying connection"
		sleep 10;
		let j=$j-1
		if [ $j -le 0 ]; then
			errecho "ERROR: to many connection failiures"
			exit 1
		fi
	done
	let i=$i+1
done
