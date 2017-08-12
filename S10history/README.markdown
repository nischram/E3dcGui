# S10history

Dieses Programme habe ich mit freundlicher Erlaubniss von [RalfJL](https://github.com/RalfJL/S10history) übernommen.

Damit dieses Programm direkt mit meiner Software zusammenarbeitet, habe ich ein paar anpassungen vorgenommen:
* User, PW, AES, IP und Port werden aus der parameter.h eingelesen.
* -T und -Y für today und yesterday eingefügt, damit die Werte gespeichert werden um auf der E3dcGui anzuzeigen.
* Alle Abfragen dauerhaft mit dem UTC schalter versehen, ohne Schalter sind alle Werte falsch.
* rLog komplett entfert.
* examples/getYearperX.sh angepasst.
* Anleitung angepasst.

Noch nicht getestet:
* Ich habe das Perl Programm S10toMysql.pl noch nicht getestet. Es könnten Probleme auftreten, bezüglich der Änderungen User, PW, AES, IP und Port

Hier jetzt die Anleitung von RalfJL und an dieser Stelle einen herzlichen Dank für das Programm!

# From RalfJL
This program reads historical data from a S10 solar power station
manufactured by [E3DC](http://www.e3dc.com) and prints it
to standard output.

Reports include:
* Energy that was feed to the battery
* Energy taken from battery
* Energy produced by solar panels
* Energy feed to the power grid
* Energy taken from the power grid
* Energy consumed by your house

## Features
* Read the sum of a Day, Month, Year
* For Day you can also list an average power (not energy) per 15 minutes
* For Month you can also list a daily energy report
* CSV output for better parsing
* PERL program to feed the output to a Mysql database
* Scripts that show some usage

## Usage
Reading one paticular day (2017-02-17):<br>
`S10history -y 2017 -m 2 -d 17`

Read only the sum of that day:<br>
`S10history -y 2017 -m 2 -d 17 -b`

Read the sum of yesterday:<br>
`S10history -d -1 -b`

Read the Month and all days of that month:<br>
`S10history -y 2017 -m 2`

Read the sum of one year:<br>
`S10history -y 2016`

Put all days of one year into a Mysql database (please fill the scripts with your values):<br>
`examples/getYearperDay.sh 2016 > Year2016perDay.txt` # reads all days to file<br>
`./S10toMysql.pl -dbname=myDBName -user=mySQLUser -password=PWofSQLuser Year2016perDay.txt`<br>
use the file to fill the database

## History
07.2017 added switch so that UTC can be used instead of local TZ
	S10 uses UTC only for time; did not really help with accuracy
02.2017 Initial version

## Credits
All credits go to E3DC for providing the RSCP protocol and demo that this program is based on
