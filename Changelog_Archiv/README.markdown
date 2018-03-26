# Changelog Archiv
V1.70 26.03.2018 Anpassungen in der Anleitung
- Link zur Beispielapplikation von E3DC überarbeiter
- Anpassungen in der Anleitung
- Changelog archiviert

V1.69 17.12.2017 [Issue #26](https://github.com/nischram/E3dcGui/issues/26)
- Größe des RAMDisk erhöht

V1.68 10.12.2017 Grünbeck softliQ SC18 eingebunden
- Daten einer Grünbeck Wasseraufbereitung am Display anzeige und an die HM senden  

V1.67 02.12.2017 WetterGui abgeschaltet
- Der Wetterdienst von http://weather.tuxnet24.de wurde eingestellt
- Ich hatte die Falsche Datei der srceenSaveHM hochgeladen.

V1.66 30.11.2017 WetterGui abgeschaltet
- Der Wetterdienst von http://weather.tuxnet24.de wurde eingestellt

V1.65 06.11.2017 Fehler Abfuhrkalender
- Fehler in der Zeilenabfragen Maximalwert war KW44

V1.64 29.10.2017 Fehler LED-Statusanzeige
- Ohne kompilieren der "rpi_ws281x" fehlt die Datei "version.h"

V1.63 17.10.2017 Verbesserungen
- Makefile für einfaches kompilieren
- parameter.h geteilt für einfachere Anpassungen
- Watchdog Dateien werden im Verzeichnis Data gespeichert

V1.62 14.10.2017 Fehlerkorrektur Aktoren
- Fehlende Parameteränderung nachgetragen

V1.61 11.10.2017 LED-Statusanzeige integriert
- Bibliothek WS281x mit LED-Statusanzeige integriert

V1.60 03.10.2017 Verbesserung Bildschirmschoner
- Schaltaktor um den Bildschirmschoner abzuschalten

V1.59 03.10.2017 Verbesserungen: Schaltaktoren + DHT11-Sensor
- Zeitaktor und Zeitfenster bei den Aktoren hinzugefügt
- DHT11: Akktualisierung der Werte nur bei erfolgreicher Abfrage des Sensors da die Abfrage häufig fehlschlägt

V1.58 01.10.2017 [Issue #25](https://github.com/nischram/E3dcGui/issues/25) + Korrekturen
- In Funktion "readWrite900()" fehlt ein "datei.clear()"
- Gekürter Dateiname "yester.txt" wieder auf "yerterday.txt" geändert, der Fehler lag auch an dem fehlenden "datei.clear()"
- Datei erstellen wenn diese nicht existiert
- Datein für Langzeitwerte "xxx900.txt" werden jetzt erstellt wenn diese nicht existieren (gelöscht)
- Today oder Yesterday Zeit war unter Langzeitwerte und Monitor auch zu sehen
- Changelog archiviert

V1.57 28.09.2017 Fehlerkorrektur für V1.54
- Fehlerkorrektur für V1.54, das Ändern des doppelten öffnens führt dazu, dass die Datei nicht erstellt werden kann und nach einem Reboot ist das RAMDisk Verzeichnis erst einmal leer
- Tippfehler bei wrietHistory
- Dateiname yesterday.txt gekürtzt auf yester.txt

V1.56 27.09.2017 [Issue #23](https://github.com/nischram/E3dcGui/issues/23)
- Priorität für die Aktoren eingebaut
- Fehlerkorrektur für V1.54 in der External/dht11.h
- Fehlerkorrektur in der External/Aktor.h nach Änderung V1.54 wurde der Überschussaktor nicht mehr aktiv

V1.55 27.09.2017 [Issue #24](https://github.com/nischram/E3dcGui/issues/24)
- Fehlerkorrektur, Schreibweise falsch für andere Compiler

V1.54 27.09.2017 Systemstabilität & Mindestzeiten für Schaltaktoren
- Fehlerkorrektur zur Systemstabilität bei BitRead, BitWrite und BitChange (kein doppeltes öffnen für lesen und schreiben)
- Für die Schaltakoren noch mindeste Einschaltzeit und Ausschaltzeit integriert
- Für eine bessere Übersicht MuellGui.h und WetterGui.h verschoben nach External/

V1.53 24.09.2017 [Issue #22](https://github.com/nischram/E3dcGui/issues/22) & Fehlerkorrekturen
- Fehlerkorrektur Issue #22
- Innerhalb der ersten 24 Std. nach Erstinstallation stürzt GuiMain bei Langzeitgrafik ab, weil 15 Minuten-Dateien fehlen wie z.B. Solar900.txt
- Debug Funktion fest integriert mit Schalter in der parameter.h (#define debugUse 0)

V1.52 23.09.2017 WiringPi, PIR-Sensor, DHT11, Schaltaktor, Verbesserungen
- Button SmartHome hinzugefügt für Schaltaktoren und DHT11
- Aufteilung Button erste Zeile überarbeitet
- Anpassung Anleitung für WiringPi installation (Installation erforderlich!)
- Automatisches Beenden des Bildschirmschoners mit PRI-Sensor
- Schaltaktoren für Überschuss, Solar oder Batterie mit GPIO's
- Anbindung des Sensors DHT11
- Fehlende Parameter für Abfuhrkalender nachgetragen
- Fehler WetterGui behoben, Watchdog schlägt zu, zu große Wartezeit
- Fehler GUI behoben, negativer oder zu großer SOC/Autarkie/Eigenstrom führt zum Speicherzugriffsfehler
- Fehler behoben wenn für die Version die README.markdown nicht geöffnet werden kann kommt Speicherzugriffsfehler
- Rechtschreibfehler korrigiert

V1.51 08.09.2017 Anpassung Setup Menü
- Rücklesen der Displayhelligkeit
- Erneuter Umbau der Anzeige für die IP-Adresse
- Fehler bei der Benutzung von int Arrays behoben
- Changelog archiviert

V1.50 06.09.2017 [Issue #18](https://github.com/nischram/E3dcGui/issues/18) & [Issue #21](https://github.com/nischram/E3dcGui/issues/21)
- Fehlerkorrektur Issue #18
- Fehlerkorrektur Issue #21

V1.49 05.09.2017 Abfuhrkalender eingebaut
- 2 Kalenderwochen vom Abfuhrkalender als Grafiv erstellt
- Diverse Anpassungen für die Anzeige vom Abfuhrkalender
- Zeitanzeige in funktion.h ausgelagert

V1.48 03.09.2017 [Issue #20](https://github.com/nischram/E3dcGui/issues/20)
- Anpassung durch Tipp von RalfJL

V1.47 03.09.2017 WetterGui eingebaut
- Wetteranzeige erstellt
- Aufbau Hintergrundbild in funktion.h ausgelagert

V1.46 01.09.2017 [Issue #20](https://github.com/nischram/E3dcGui/issues/20)
- Änderungen laut Issue #20

V1.45 31.08.2017 IP-Adresse
- Abfrage IP-Adresse erneut überarbeitet
- korrektur Ausgabe "readHM"

V1.44 31.08.2017 Versionsanzeige
- Versionsanzeige überarbeitet

V1.43 31.08.2017 [Issue #19](https://github.com/nischram/E3dcGui/issues/19)
- Fehler im Script "S10_Zeitstempel" behoben

V1.42 27.08.2017 Diverses
- Anleitung überarbeitet um den Pi Bildschirmschoner zu deaktivieren
- Diverse korrekturen in der README
- "pkill" in "killall" geändert
- Schritt für Schritt Anleitung erstellt

V1.41 26.08.2017 "warning" wegen neuen Compiler behoben
- Neu Compiler meldet einige "warning"

V1.40 26.08.2017 [Issue #11](https://github.com/nischram/E3dcGui/issues/11)
- Anpassung für sendEmail vorgenommen Zeile 1906

V1.39 20.08.2017 Anleitung überarbeitet
- Abschalten vom PI Bildschirmschoner und Cousor in der README aufgenommen
- Tippfehler in der README
- Anpassung für den Pfad zu /dev/fb

V1.38 20.08.2017 [Issue #18](https://github.com/nischram/E3dcGui/issues/18)
- Sommer/Winterzeit Einstellungen ist nicht mehr nötig
- Button für Sommer/Winterzeit entfernt

V1.37 20.08.2017 Diverse Verbesserungen und Fehlerbehebung
- [Issue #17](https://github.com/nischram/E3dcGui/issues/17)
- Versionsanzeige im Setupmenü eingebaut
- Changelog archiviert

V1.36 15.08.2017 HistoryValues verbessert
- Definition für "historyAktiv" und "historyDelay" eingebaut

V1.35 13.08.2017 [Issue #13](https://github.com/nischram/E3dcGui/issues/13)
- Fehlerkorektur für Einstellung der Display-Helligkeit

V1.34 13.08.2017 [Issue #16](https://github.com/nischram/E3dcGui/issues/16)
- Fehlerkorektur screenSave.c

V1.33 13.08.2017 [Issue #13](https://github.com/nischram/E3dcGui/issues/13)
- Einstellung der Display-Helligkeit

V1.32 11.08.2017 S10history integriert
- S10history von [RalfJL](https://github.com/RalfJL/S10history) integriert
- [Anleitunf für S10History](https://github.com/nischram/E3dcGui/tree/master/S10history) angepasst
- Abfrage der HistoryValues für Tag und Vortag
- Anzeige der HistoryValues für Tag und Vortag in den Aktuellen Werten
- [Issue #12](https://github.com/nischram/E3dcGui/issues/12)
- Anpassungen im RSCP für S10history
- ASE.cpp für 64Bit vorbereitet [Issue #4](https://github.com/nischram/E3dcGui/issues/4)
- [Issue #15](https://github.com/nischram/E3dcGui/issues/15) WatchDog verbessert
- IP Abfrage für das Setup Menü angepasst
- Funktionen für Setup Menü ausgelagert !!! Achtung !!! In der screenSaveHM.c muss folgende Zeile im oberen Bereich eingefügt werden:
`  #include <sys/sysinfo.h>  `
- Zusammenfassen von Funktionen in der GuiMain.c

V1.31 31.07.2017 [Issue #14](https://github.com/nischram/E3dcGui/issues/14)
- Änderung an der HomeMatic README, HM Variable nicht beschrieben
- Änderung an der HomeMatic README, Limitation 200 Variablen entfernt

V1.30 14.05.2017 Tippfehler README
- Changelog archiviert

V1.29 14.05.2017 Issue #11
- Fehlermeldung bei sendEmail

V1.28 01.05.2017 Issue #10
- ADD Power wurde Negativ angezeigt, Invertierung eingebaut

V1.27 13.02.2017 Issue #9
- Wegen häufigen Reebot eine Wartezeit von 3600 Sekunden eingebaut

V1.26 11.12.2016 Fehlerkorrektur
- Grafiken für die einzelnen Tracker fehlen

V1.25 11.12.2016 Fehlerkorrektur für PN aus Homematicforum
- GuiMain.c und den Teil gekürzt der nur für meine zusätzliche Anwendung ist
- DrawImage.h hatte ich bei einen der letzten Aktualisierungen vergessen   

V1.24 05.12.2016 Verbesseung der Darstellung
- Verbesseung der Darstellung der Uptime vom Pi

V1.23 05.12.2016 Stromanzeige der einzelnen Tracker überarbeitet
- Speicherung zurückgeändert von char nach int
- int *100 um die Kommastellen nicht zu verlieren   

V1.22 04.12.2016 Readme überarbeitet
- Fehlerkorrektur
- Loxone Readme ausgelagert

V1.21 04.12.2016 Anzeige der einzelnen Tracker des Wechselrichters
- PI Daten aus Monitor-Seite verschoben in die Setup-Seite
- PI Daten um die IP-Adresse erweitert
- Azeige für die einzelnen Tracke des Wechselrichters auf der Monitor-Seite erstellt
- Speichern der Tracker-Ströme in char damit die Nachkommastellen angezeigt werden können   

V1.20 02.12.2016 Issue #6
- Containerabfrage für WR Tracker in RscpMain.cpp eingebaut incl. definition in der parameter.h

V1.19 01.12.2016 Issue #7
- Fehler im upload zu "V1.13 Systemverbesserungen" falscher Dateinamen für Solar900

V1.18 28.11.2016 Extrahieren der Touchfunktion für Homematic und HM-Anpassungen
- Grafikproblem beim beenden des Bildschirmschoners behoben
- ScreenSave.c getrennt damit ich in der screenSaveHM.c für die HM Änderungen vornehmen kann ohne anderen Usern Probleme zu machen
- Anordnung der Darstellung der Homematic-Symbole in der screenSaveHM.c (für Nico)
- weitern Button für Homematic-Touch zur Steuerung in der screenSaveHM.c (für Nico)

V1.17 27.11.2016 Issue #5
- Fehler bei den Langzeitwerten behoben
- wenn das Display nicht genutzt wird, kann die SleepTime definiert werden sonst wir mit 1 Sekunde aktualisiert  

V1.16 24.11.2016 Phasenabfrage des Leistungsmessers
- Abfrage der Aktiven Phasen
- Darstellung bei Ausfall von Phasen

V1.15 24.11.2016 Option im Makefile geändert   
V1.14 24.11.2016 Issue #3
- von RalfJL aufgezeigter Fehler behoben

V1.13 23.11.2016 Systemverbesserungen  
- Globalvariablen in der RscpMain.cpp verringert
- GuiMain.c für Änderungen der RscpMain.cpp angepasst
- zu viele Darstellungen verringert um Speicherfehler zu vermeiden
- Fehler bei der Übergabe aus HmGui.h der GuiTime
- parameter.h übersichtlicher gemacht   
- write, read und change Funktionen verbessert
- Fehler bei HMGui = 0 im watchdog.cpp behoben
- RSCP Abfragen verringern wenn Bildschirmschoner aktiv

V1.12 21.11.2016 Systemverbesserungen  
- DrawImage.h Bildschirmausgaben auf das nötige reduziert

V1.11 21.11.2016 Issue #1  
- allocierter Speicher wurde nicht freigegeben `cleanup();` mit `free` war deaktiviert  
- Anpassung doppelte inizialisierter Globalvariablen in framebuffer.c und DrawImage.h   
- Debug Möglichkeit in funktion.h integriert

V1.10 15.11.2016 Watchdog Ausgaben verbessert   
V1.9 15.11.2016 Systemverbesserungen:
- weniger Dateien im RAMDisk
- Watchdog stabilisiert
- Kontrollmöglichkeit für den WD eingefügt

V1.8 12.11.2016 eMail senden vom WD
V1.7 12.11.2016 Stabilitätsverbesserung Touchbedienung  
V1.6 10.11.2016 Fehlerkorrektur bei der Touchbedienung  
V1.5 10.11.2016 Zeit aktualisieren  
V1.4 10.11.2016 Courser blinken im Code integriert und aus Anleitung entfernt  
V1.3 09.11.2016 TouchTest integriert / Diverses  
V1.3 09.11.2016 Auslagern der HM_Gui  
V1.3 09.11.2016 Display temporär auf 16Bit setzen  
V1.2 07.11.2016 Courser Blinken abschalten  
V1.1 31.11.2016 Umschaltung Sommer- Winterzeit  
V1.0 14.08.2016 Repository neu erstellt  
