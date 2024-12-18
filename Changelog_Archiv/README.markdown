# Changelog Archiv

V2.20 21.02.2023 [Issue #79](https://github.com/nischram/E3dcGui/issues/79) Probleme mit Aktor behoben.
- Issue #79 Anpassung in Aktor.h vorgenommen

V2.19 05.12.2022 Fehler in funktion.h behoben
- Funktionsaufruf aus der "actionCheckHM" bei GUI=0 war fehlgeschlagen

V2.18 09.11.2022 Fehler in Rscp/RWData.h behoben
- In  Rscp/RWData.h fehlte ein "return 1"  

V2.17 16.08.2022 git clone geängert von git:// auf https://
- git-Server geändert auf https://  

V2.16 16.08.2022 Fehlerkorrektur "screnSave.c" erkannt im Homematic-Forum + [Issue #75](https://github.com/nischram/E3dcGui/issues/75)
- Variable falsch gesetzt  
- Kleine Anpassungen an der GuiMain.c   
- Issue #75 Anpassung für Schalter Batterie im Mischmode   

V2.15 12.06.2022 [Issue #77](https://github.com/nischram/E3dcGui/issues/77) Lade- und Entladesperrzeiten in Anziege eingebaut
- Issue #77 Menü Sperrzeiten eingabaut aktivieren und deaktivieren der Sperrzeiten möglich    
- Issue #75 Batterienutzung im Mischbetrieb kann eingestellt werden     

V2.14 07.06.2022 [Issue #77](https://github.com/nischram/E3dcGui/issues/77) RscpSet ergänzt um die Lade- und Entladesperrzeiten
- Issue #77 "RscpSet -idle ..." für die Sperrzeiten  
- Einige Anpassungen an der RscpSet vorgenommen  

V2.13 06.06.2022 [Issue #75](https://github.com/nischram/E3dcGui/issues/75) Fehlerkorrektor
- Fehler in den 10% Stufen behoben  

V2.12 06.06.2022 RscpSet geändert die Parameter der Wallbox werden in der EMS und WB jetzt getrennt geändert
- Änderung an der RscpSet  

V2.11 05.06.2022 [Issue #75](https://github.com/nischram/E3dcGui/issues/75) Falschen Issue angegeben
- Beim Commit zuvor falschen Issue angegebne richtig ist Issue #75  

V2.10 05.06.2022 [Issue #76](https://github.com/nischram/E3dcGui/issues/76) Ladegrenze "Batterie zu Auto" einstellbar
- Issue #76 über Tasten ist in 10% Stufen die Ladegrenze "Batterie zu Auto" jetzt einstellbar      
- Changelog archiviert  

V2.09 17.02.2022 [Issue #71](https://github.com/nischram/E3dcGui/issues/71) + [Issue #72](https://github.com/nischram/E3dcGui/issues/72) + [Issue #73](https://github.com/nischram/E3dcGui/issues/73) Fehlerbehebung und Diverses
- Issue #71 Anderer cast für Variable um Fehler zu vermeiden       
- Issue #72 Refresh für die Displayanzeige war falsch in der IF-Schleife eingefügt      
- Issue #73 Fehlende Datei hochgeladen und überflüssige Dateien gelöscht         
- Text in Anzeige für "PVI-Down" und "LM-Down" ersetzt duch Grafiken für "PV Off" und "LM Off"   

V2.08 07.02.2022 [Issue #70](https://github.com/nischram/E3dcGui/issues/70) RSCP Abfrage für PM benötigt unterschiedliche Indexnummern
- Issue #70 Variable in der parameter.h aufgenommen um den Index unterschiedlich zu setzen   

V2.07 07.02.2022 Fehlende Parameter mit "./copyPara" prüfen
- Idee ist in Issue #70 entstanden   

V2.06 06.02.2022 [Issue #70](https://github.com/nischram/E3dcGui/issues/70)
- Issue #70 Fehlerkorrektur der Anzeige und Ergänzung eines Offset für DHT Temperatursensor  
- Tippfehler in der README behoben      

V2.05 02.01.2022 Fehler screenSaveHM und Fehlerbehebung Watchdog
  - Fehlermeldung aus dem HM-Forum, es fehlte ein include in der screenSaveHM.c   
  - Watchdog für Release löst teilweise mit leerer Release-Version aus.  

V2.04 03.10.2021 [Issue #58](https://github.com/nischram/E3dcGui/issues/58), [Issue #66](https://github.com/nischram/E3dcGui/issues/66), [Issue #67](https://github.com/nischram/E3dcGui/issues/67),  Diverse Verbesserungen
- Issue #58 Mit der "Rscp/actionCheckHM.c" kann nun auch die Notstromreserve von der HM aus geändert werden.  
- Issue #67 Auf dem Display so wie mit der "Rscp/actionCheckHM.c" können sie BAtterie-Limits des S10 geändert werden.  
- Issue #66 Auf dem Display werden einige Sonderinformationen angezeigt.  
- "Rscp/wbCheckHM.c" umgebaut nach "Rscp/actionCheckHM.c".  

V2.03 29.07.2021 [Issue #62](https://github.com/nischram/E3dcGui/issues/62) Fehler in der S10History seit Update V2.00  
- Issue #62 Datei "S10History/RscpReader.cpp" Änderung in V2.00 nicht hochgeladen.

V2.02 10.07.2021 Fehlerbehebung RscpMain
- #include Reihenfolge in der RscpMain.cpp war falsch

V2.01 12.06.2021 Fehlerbehebung + Wallbox Leistungswerte
- Fehlerbehebung [Issue #61](https://github.com/nischram/E3dcGui/issues/61)
- Fehler Setup Touch passt nicht zur Anzeigen  
- Wallbox Leistungswerte im Screen Langzeit aufgenommen    

V2.00 06.06.2021 [Issue #52](https://github.com/nischram/E3dcGui/issues/52) Updatefähigkeit verbessert  
- Issue #52 parameter.h im .gitignore aktiviert und parameter.h.temp eingebunden
- Schriftgröße fest auf 8x16 umgestellt  
- Bei der Anzeige für Strom wurde die erste 0 nach dem Komma nicht angezeigt  
- In der Ansicht Aktor / DHT die Schrift für Größe 8x16 angepasst  
- Nur Programme die laufen werden beim ./stop beendet

V1.92 30.05.2021 [Issue #60](https://github.com/nischram/E3dcGui/issues/60) Tracker 1 zeigt Strom und Spannung falsch  
- [Issue #60](https://github.com/nischram/E3dcGui/issues/60) Fehler behoben  

V1.92 28.05.2021 in Version V1.91 das Programm "Rscp/wbCheckHM" vergessen  
- Programm angepasst und die Funktion senden von HM zu WB erweitert um Stop und Phasen  

V1.91 28.05.2021 [Issue #58](https://github.com/nischram/E3dcGui/issues/58) Notstrom-Reserve des Speichers  
- Für [Issue #58](https://github.com/nischram/E3dcGui/issues/58) kann nun die Reserve aktiviert oder eingestellt werden  
- Für [Issue #59](https://github.com/nischram/E3dcGui/issues/59) Stop-Button im Setup-Menü  
- Für [Issue #55](https://github.com/nischram/E3dcGui/issues/55) zur Wallbox kann jetzt die Anzahl der Phasen vorgewählt werden  
- Für [Issue #56](https://github.com/nischram/E3dcGui/issues/56) kann jetzt eine Ladung gestoppt werden  

V1.90 23.05.2021 [Issue #58](https://github.com/nischram/E3dcGui/issues/58) Schriftgröße der Leistungswerte
- Für Issue #58 Schriftgöße angepasset  

V1.89 03.05.2021 Wallbox Daten an HM senden und auslesen
- Wallbox-Werte an die HM senden
- Wallbox-Parameter aus HM lesen und an WB senden

V1.88 03.04.2021 Anleitung überarbeitet  

V1.87 29.03.2021 Wallbox anzeigen o. steuern / Fehlerbehebung
- Wallbox-Menü zur Anzeige und Steuerung
- Fehlerkorrektur im Setup-Menü
- kleine Fehlerkorrekturen
- Changelog archiviert  

V1.86 13.02.2021 Fehlerkorrektur für neuere Rasbain Versionen [Issue #48](https://github.com/nischram/E3dcGui/issues/48)
- Issue #48
- Anpassungen an der README
- Anpassungen an der STEPBYSTEP
- Anpassungen GuiMain.c Zeitstempel

V1.85 30.01.2021 Fehlerkorrektur bei der Wetteranzeige [Issue #47](https://github.com/nischram/E3dcGui/issues/47)
- Issue #47

V1.84 28.01.2021 OpenWeatherMap OneCall Anpassung [Issue #47](https://github.com/nischram/E3dcGui/issues/47)
- Issue #47

V1.83 04.12.2020 [Issue #42](https://github.com/nischram/E3dcGui/issues/42)
- Fehlerbehebung für Issue #42
- printsendHM auf float geändert für Issue #42

V1.82 03.11.2020 Anpassungen für WiringPi [Issue #38](https://github.com/nischram/E3dcGui/issues/38)
- Issue #38
- WiringPi Server angepasst

V1.81 08.09.2020 Wetteranzeige erneuert [Issue #36](https://github.com/nischram/E3dcGui/issues/36)
- Issue #36 Wetter-API umgestellt auf OpenWeatherMap.org    
- Changelog archiviert  

V1.80 24.08.2020 Anpassung für Schriftgröße [Issue #35](https://github.com/nischram/E3dcGui/issues/35)
- Issue #35 Mit einem Schalter in der parameter.h kann die Größe eingesgellt werden  

V1.79 01.09.2019 Speicherzugriffsfehler behoben [Issue #33](https://github.com/nischram/E3dcGui/issues/33)
- Issue #33 Durch fehlende "return" in "int" Funktionen gibt es bei der RscpMain einen Speicherzugriffsfehler  

V1.78 27.01.2019 Wetteranzeige umgabaut auf OpenWeatherMap
- für die Wetteranzeige ist nun ein Api-Key von OpenWeatherMap (Account) erforderlich
- [Issue #32](https://github.com/nischram/E3dcGui/issues/32)

V1.77 03.12.2018 Fehler nach Update für DHT22 behoben
- screenSave.c Zeile 207 useDHT11 geändert in useDHT

V1.76 14.10.2018 SmartHome angepasst für DHT11 und DHT22 Sensoren
- dht11.c ausgetauscht gegen dht.h damit auch die DHT22 Sensoren möglich sind.

V1.75 19.08.2018 WetterGui wieder aktiviert [Issue #32](https://github.com/nischram/E3dcGui/issues/32)
- Issue #32 Der Wetterdienst über weather.tuxnet24.de ist als https wieder aktiv.

V1.74 05.08.2018 Zeitstempel zur HM senden
- Das Script in der HM arbeitet nicht mehr, jetzt kann der Zeitstempel gesendet werden.

V1.73 25.06.2018 [Issue #30](https://github.com/nischram/E3dcGui/issues/30) + Korrekturen
- Beispieländerung für Issue #30
- Rückmeldung im "CURL-Befehl" abgestellt

V1.72 24.04.2018 Bilder wurden nicht mehr angezeigt
- Postimage ist unter postimg.org nicht mehr erreichbar jetz postimg.cc

V1.71 11.04.2018 Verbesserung IP Anzeige
- Bei der Verwendung von W-LAN wird für ETH ein falscher Wert angezeigt

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
