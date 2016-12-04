# Changelog Archiv

Stand V1.22 04.12.2016
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
