//============================================================================
// Name        : S10history.cpp
// Author      : Ralf Lehmann
// Copyright   : Ralf Lehmann 02.2017
// Version     : 1.0
// Description : Reads historical data from a s10 solar power station (E3DC)
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//============================================================================

#include <iostream>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

//using namespace rlog;
using namespace std;

#define REPORT_YEAR 1
#define REPORT_MONTH 2
#define REPORT_DAY 4
#define today 1
#define yesterday 2


char *progname;

int usage(const char *errstr) {
	cerr << errstr << endl;
	cerr << "This program reads historical data from a E3DC/S10 and prints it" << endl;
	cerr << "[-y year] [-m month] -d day prints data of the given day; details every 15 minutes" << endl;
	cerr << "[-y year] -m month          prints data of the given month; details once per day" << endl;
	cerr << "-y year                     print the sum of the given year, no details" << endl;
	cerr << "Options:" << endl;
	cerr << "--version      version string" << endl;
	cerr << "--help         this message" << endl;
	cerr << "--brief        brief report; sum only" << endl;
	cerr << "--year num     year > 2014; current year if not present" << endl;
	cerr << "--month -+num  month; current month if not present" << endl;
	cerr << "--day +-num    day; current day if not present" << endl;
	cerr << "--today	  		report today and save to use in E3dcGui/GuiMain" << endl;
	cerr << "--yesterday	  report yesterday and save to use in E3dcGui/GuiMain" << endl;

	return 1;
}

int main(int argc, char *argv[]) {
	progname = argv[0];    // save program name

	time_t rawtime;        // used for time calculation
	time(&rawtime);        // get current time to complete time arguments
	//struct tm *l = localtime(&rawtime);
	struct tm *l = gmtime(&rawtime);

	if (l->tm_isdst >= 0) {
		l->tm_isdst = -1;	// DST is not used by S10; do not interpret it
	}
	//use UTC to report
	char text[16];
	snprintf (text, (size_t)16, "TZ=UTC");
	putenv(text);

	// report type
	int report_type = 0; // 1=year; 2=month, 4=day; 0=current day; 5=last
	bool brief = false;	 // brief means only sum container to report
	int writedata = 0;			 // if use 'D' or 'Y' to write data
	// option struct
	const struct option longopts[] = {
		{ "version", no_argument, 0, 'v' },
		{ "year", required_argument, 0, 'y' },
		{ "month", required_argument, 0, 'm' },
		{ "day", required_argument, 0, 'd' },
		{ "help", no_argument, 0, 'h' },
		{ "brief", no_argument, 0, 'b' },
		{ "yesterday", no_argument, 0, 'Y' },
		{ "today", no_argument, 0, 'T' },
	};

	// process arguments
	int index;
	int iarg = 0;
	long y, m, d = 0;

	// turn off getopt error message
	// opterr=1;
	while (iarg != -1) {
		iarg = getopt_long(argc, argv, "vhYTy:m:d:d:b", longopts, &index);
		switch (iarg) {
		case 'h':
			return usage("");
			break;
		case 'v':
			printf("version 1.0");
			return 0;
			break;
		case 'b':
			brief = true;
			break;
		case 'y':
			y = atol(optarg);
			if (y < 2012 || y > 2032) {
				return usage("ERROR: Invalid year");
			}
			l->tm_year = y - 1900;
			if (report_type & REPORT_YEAR) {
				return usage("ERROR: only one year please");
			}
			report_type |= REPORT_YEAR;
			break;

		case 'm':
			m = atol(optarg);
			if (m < 1 || m > 12) {
				return usage("ERROR: invalid month");
			}
			l->tm_mon = m - 1;
			if (report_type & REPORT_MONTH) {
				return usage("ERROR: only one month please");
			}
			report_type |= REPORT_MONTH;
			break;

		case 'd':
			d = atoi(optarg);
			if (d < 0) {
				l->tm_mday += d;
			} else {
				l->tm_mday = d;
			}
			rawtime = mktime(l);
			if (report_type & REPORT_DAY) {
				return usage("ERROR: only one day please");
			}
			report_type |= REPORT_DAY;
			break;

		case 'T':
			l->tm_mday;
			rawtime = mktime(l);
			report_type |= REPORT_DAY;
			writedata = today;
			brief = true;
			break;

		case 'Y':
			l->tm_mday += -1;
			rawtime = mktime(l);
			report_type |= REPORT_DAY;
			writedata = yesterday;
			brief = true;
			break;
		}
	}

	// check time
	l->tm_sec = l->tm_min = l->tm_hour = 0;
	rawtime = mktime(l);
	time_t now;
	time(&now);
	if (rawtime >= now) {
		return usage("ERROR: report date is in the future");
	}

	extern int RscpReader_Day(struct tm *l, bool brief, int writedata);
	extern int RscpReader_Month(struct tm *l, bool brief, int writedata);
	extern int RscpReader_Year(struct tm *l, bool brief, int writedata);
	int (*report_func)(struct tm *, bool brief, int writedata) = RscpReader_Day;

	// check report span
	if (report_type == 0) {
		report_type |= REPORT_DAY;
	}
	switch (report_type) {
	case 1:
		l->tm_mday = 1;
		l->tm_mon = 0;
		printf("Reporting one year ");
		report_func = &RscpReader_Year;
		break;
	case 2:
	case 3:
		l->tm_mday = 1;
		printf("Reporting one month ");
		report_func = &RscpReader_Month;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		printf("Reporting one day ");
		report_func = &RscpReader_Day;
		break;

	}
		printf("Report starts: %s", asctime(l));
		return (*report_func)(l, brief, writedata);
	return 0;
}
