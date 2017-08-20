# Changelog Archiv

Stand V1.37 20.08.2017

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
