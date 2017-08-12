#!/usr/bin/perl -w
# Author      : Ralf Lehmann
# Version     : 1.0
# Copyright   : Ralf Lehmann 02.2071
# Description : Reads historical data from s10history generated files and stores it in mysql
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


use strict;
use warnings;
use Getopt::Long;

use DBI;

# globals
my $debug;

my $usage;
my $dbname;
my $dbusername;
my $dbpassword;
my $dbh;

# table names and definitions
my $table_day         = "day";
my $table_month       = "month";
my $table_year        = "year";
my $table_default_col = "date date";

sub usage($) {
	my $e = shift;
	print <<"EOT";
$e
Usage : $0 [Options] -dbname=DB -user=DBuser -password=DBpassword [CSV-files]
Read in CSV files and put into database.
CSV-file format is from S10reader
-password password|ENV-variable     password can be in ENV-variable

Options:
-debug		add debug information
EOT
	exit 1;
}

#
# db functions
#
sub create_table($) {
	my $t = shift;
	$dbh->do("CREATE TABLE $t (date date PRIMARY KEY)")
	  || die("ERROR: can't create table $t");
}

sub check_tables() {
	foreach my $t ( $table_day, $table_month, $table_year ) {
		my $sth = $dbh->table_info( undef, undef, $t, 'TABLE' );
		$sth->execute
		  || die("ERROR: executing table_info for table $t");
		my @info   = $sth->fetchrow_array;
		my $exists = scalar @info;
		if ( !$exists ) {
			create_table($t);
		}
	}
}

sub check_db_cols($$) {
	my $line  = shift;
	my $table = shift;
	chomp($line);
	print STDERR "DEBUG: Checking db cols with \"$line\"\n" if $debug;

	# get cols
	$line =~ m/.*-head[:]*\s+(.*)/;
	my @line = split( /[;,]/, $1 );

	foreach my $i (@line) {
		print STDERR "DEBUG: checking table \"$table\" for column: \"$i\"\n"
		  if $debug;
		my $sth = $dbh->column_info( undef, undef, $table, $i );
		my @info = $sth->fetchall_arrayref;
		if ( defined $info[0][0] ) {
			print STDERR "DEBUG: col $i exists\n" if $debug;
		}
		else {
			my $type = "double";
			if ( $i eq "date" ) {
				$type = "date";
			}
			$dbh->do("ALTER TABLE $table ADD $i $type ")
			  || die "ERROR : unable to alter table $table;
                $DBI::errstr ";
			print STDERR "DEBUG: creating column $i \n" if $debug;
		}
	}
}

#
# read in function
#
sub read_file($) {
	my $in = shift;
	my @headline;
	my $col;
	my $table;
	while (<$in>) {
		my $line = $_;
		chomp($line);
		print STDERR "DEBUG: Reading : $line\n" if $debug;
		if ( !/CSV/ ) {    # only use lines that contain CSV data
			next;
		}
		if (/CSV-head/i) {

			# determine which table to use
			if (/month/i) {
				$table = $table_month;
			}
			if (/day/i) {
				$table = $table_day;
			}
			if (/year/i) {
				$table = $table_year;
			}
			if ( !defined $table ) {
				print STDERR
"ERROR: check_db_table can't determin table to check; Line: \"$_\"";
				exit 1;
			}
			check_db_cols( $_, $table );
			$line =~ m/.*-head[:]\s+(.*)/;
			@headline = split( /[;,]/, $1 );
			$col = join( ", ", @headline );
			next;
		}

		# data line
		if ( !defined $headline[0] ) {
			die "ERROR: CSV-head line missing";
		}
		if ( !( $line =~ m/.*-CSV[:]?\s+(.*)/ ) ) {
			warn "WARNING: illegal line \"$line\"";
			next;
		}
		my @line = split( /[;,]/, $1 );
		$line[0] = "FROM_UNIXTIME($line[0])";
		my $values = join( ", ", @line );

		# check headline and this line
		if ( $#headline != $#line ) {
			warn "WARNING: line does not match headline: \"$line\"";
			next;
		}
		my $insert = "INSERT into $table ($col) VALUES($values)";
		print STDERR "DEBUG: \"$insert\"\n" if $debug;
		$dbh->do($insert) or warn "WARNING: \"$insert\"; $DBI::errstr";
	}
}

#
# this is main
#

GetOptions(
	"dbname=s"   => \$dbname,        # name of the database
	"user=s"     => \$dbusername,    # login name
	"password=s" => \$dbpassword,
	"help"       => \$usage,
	"debug"      => \$debug,
) or die("ERROR in command line arguments");

usage("") if $usage;
usage("ERROR: dbname, user and password are mandatory")
  if ( !defined $dbname
	|| !defined $dbusername
	|| !defined $dbpassword );

# check if user password is in ENV variable
if ( defined $dbpassword ) {
	if ( defined $ENV{$dbpassword} ) {
		$dbpassword = $ENV{$dbpassword};
	}
}

$dbh = DBI->connect( "DBI:mysql:$dbname", $dbusername, $dbpassword )
  || die("Could not connect to database: $DBI::errstr");

# check if tables exist
check_tables();

# check for CSV files
if ( $#ARGV < 0 ) {
	print STDERR "DEBUG: Reading from STDIN\n" if $debug;

	# read from STDIN
	read_file(*STDIN);
}
else {
	while ( $#ARGV >= 0 ) {
		my $f = shift @ARGV;
		print STDERR "DEBUG: Parsing file: $f\n" if $debug;
		my $in;
		open( $in, "<", $f ) or die("ERROR: can't open file $f");
		read_file($in);
	}
}

# finish
$dbh->disconnect()
  || die("ERROR: disconnecting from database");

