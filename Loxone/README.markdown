# E3DC to Loxone
Stand: V1.1 04.12.2016

Hier beschreibe ich, wie du dein S10 Hauskraftwerk von E3DC an eine Loxone Hausautomation anbinden kannst.

__Diese Anleitung muss noch angepasst werden und die Funktionen muss ich erneut testen. Stand basiert etwa auf V1.1__

## RSCP to Loxone
__Achtung__   
Aktuell werden die ID aus der "paramter.h" nur als Integer verarbeitet. Für Loxone wird wie unten beschrieben ein char benötigt. Ich muss den Fehler im Quellcode noch suchen.
Somit kann Loxone im Moment nicht genutzt werden.

Durch ein Forum bin ich auf die Möglichkeit aufmerksam gemacht worden, dass mit wenigen Änderungen auch eine Loxone Zentrale mit Daten versorgt werden kann.

Da ich keine Loxone besitze gehe ich nicht näher auf das einrichten der Variablen ein.

Im SourceCode der RscpMain.cpp sind ein paar Änderungen nötig.
Einstellung in der „parameter.h“ definierst du einfach die Einstellungen für die Loxone. In der RscpMain.cpp muss du nun die CURL-Adresse zur Loxone anpassen.
Die Zeile 64 sieht für HomeMatic so aus:
```
snprintf(batch, sizeof(batch), "curl \"http://%s/config/xmlapi/statechange.cgi?ise_id=%s&new_value=%i\" > /dev/null 2>&1",HM_IP , id, value);
```
Für Loxone müsste es etwa so aussehen:
```
snprintf(batch, sizeof(batch), "curl \"http://user:passwort@%s/​dev/sps/​io/%s/%i" > /dev/null 2>&1",HM_IP , id, value);
```
Für HM_IP hast du zuvor in der "parameter.h" einfach deine Loxone IP eingetragen. Den "user" und "passwort" für Loxone musst du in der Ziele anpassen.
Natürlich könnte man Abfrage der "parameter.h" für die "RscpMain.cpp" auch umschreiben und den "user" und das "passwort" abfragen. Dann müsste die Zeile oben etwa so aussehen:
```
snprintf(batch, sizeof(batch), "curl \"http://%s:%s@%s/​dev/sps/​io/%s/%i" > /dev/null 2>&1", loxuser, lospassword, HM_IP , id, value);
```
Und in der "parameter.h" müssten dann folgende Zeilen eingefügt werden:
```
#define loxuser         LoxoneUserName
#define lospassword     LoxonePasword
```
Nach den Anpassungen muss die Applikation neu kompiliert (make) werden. Folgendes eintippen:
```
pi@raspberrypi ~/E3dcGui $  cd make
```
Jetzt sollte mit der "E3dcGui" deine Loxone Zentrale erreicht werden. Da ich keine Loxone sondern die HomeMatic besitze, habe ich es natürlich nicht getestet, bin also auf Rückmeldungen bei Problemen angewiesen.

## Changelog

V1.1 04.12.2016 Readme für Loxone erstellt
- Ordner neu erstellt
