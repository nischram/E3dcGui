# E3DC to HomeMatic mit GUI
[Stand: V1.25 11.12.2016](https://github.com/nischram/E3dcGui#changelog)

Hier beschreibe ich, wie du dein S10 Hauskraftwerk von E3DC an eine HomeMatic Hausautomation von eQ-3 anbinden kannst.

Als Schnittstelle zwischen S10 und HomeMatic dient ein Raspberry Pi. Die Applikation, kann auf dem Raspberry PI vielseitig eingesetzt werden.

__Daten per RSCP vom E3DC-S10 Speicher abfragen__   
__Daten vom E3DC zur HomeMatic Hausautomation senden__   
__Daten vom S10 auf dem Display darstellen__   
__Daten von der HomeMatic auf dem Display darstellen__

Die verschiedenen Möglichkeiten können in unterschiedlichen Konstellationen zusammen genutzt werden.   
Es kann entweder ein Raspberry Pi __ohne Display__ eingesetzt werden, der die Daten vom S10 abfragt und/oder zur HomeMatic senden.  
Oder es wird ein Raspberry Pi mit __7“ Display__ eingesetzt um diverse Daten darzustellen.  

<img src="https://s20.postimg.org/kxtyqs93x/E3_DC_Display.jpg" alt="E3DC-Display">  
Hier ein Foto mit der Software auf dem 7-Zoll Display.

Diese Kombination bietet eine sehr gute Möglichkeit um das S10 einfach und schnell mit einem eigenem Display zu überwachen.

Ich nutze diese Applikation auf einem Raspberry Pi 3. Mit dem 7“ Raspberry Touchdisplay. Für das Material und die Installation ist weiteres unten ein Kapitel __[Material](https://github.com/nischram/E3dcGui#material)__. Meine HomeMatic hat die Firmware 2.19.9 (zuvor auch 2.17.15 getestet) installiert. Mein S10-E hat Softwareversion 2016-04.

In der Applikation werden die Daten vom S10 mit einer RSCP-Applikation geholt. Diese Applikation bietet E3DC in Downloadbereich an. Ich habe ein paar Änderungen vorgenommen. Da ich mich erst seit kurzen mit der Programmierung beschäftige, sind die Änderungen eventuell noch etwas unprofessionell, aber bislang erfüllen sie ihren Zweck.

Die abgefragten Werte vom S10 wie z.B. Leistungswerte oder Batteriefüllstand werden auf wunsch per CURL-Befehl an die XML-API Schnittstelle der HomeMatic gesendet.

Wenn ein 7" Display genutzt wird, kommen C Programme zum Einsatz, um die diversen Grafiken zu erstellen.

## Vorbereitung am S10    
Hauptmenü > Einstellungen
Am S10 muss im „Hauptmenü“ unter „Einstellungen“ > "Personalisieren" ein RSCP-Passwort gesetzt werden. Das gesetzte Passwort wird später in den Parametern der Software benötigt.
Im selben Menü „Einstellungen“ > "Netzwerk" ist die IP-Adresse des S10 zu finden.

## Raspberry Pi   
Ich erkläre die Erstinstallation eines Raspberry Pi hier nicht, dies Informationen werden im Internet beschrieben.   
Diese Anleitung setzt einen lokalen Zugriff oder SSH-Zugriff auf den Raspberry voraus.

### Netzwerk   
Es ist Wichtig darauf zu achten, dass der Raspberry Pi im selben Netzwerk wie das S10 installiert ist. Der IP-Adressbereich muss gleich sein, zum Beispiel so: 192.168.178.xxx   
Auch darf die Verbindung zwischen S10 und Raspberry Pi nicht über eine VPN-Verbindung hergestellt werden.

### Applikation auf den Raspberry kopieren   
Jetzt wird der Lokale oder der SSH-Zugang zum Raspberry benötigt. Ich mache dies per SSH mit dem „Terminal“ für MAC. Möglich ist dies unter Windows mit "Putty".
Jetzt folgt der Download vom Github: (Es ist __kein__ Github Account nötig.)
```shell
git clone git://github.com/nischram/E3dcGui.git ~/E3dcGui
```

### Einstellungen vornehmen   
In der Datei „parameter.h“ im Ordner E3dcGui kann nun alles eingestellt werden, was du zum nutzen der Software benötigst. Entweder öffnet man die Datei mit einem externen Editor. Hier können diverse Editoren zum Einsatz kommen, ich nutze auf meinem Mac „Atom“ und lade die Datei mit „Cyberduck“ runter. Unter Windows ist das runterladen und bearbeiten z.B. mit WinSCP möglich. Alternativ kann man die Datei direkt auf dem Raspberry bearbeiten, dies beschreibe ich jetzt.
Zuerst mit folgendem Befehl in den Ordner E3dcGui wechseln: (auf Groß und Kleinschreibung achten)
```shell
pi@raspberrypi:~ $  cd E3dcGui
```
Öffnen der Datei „parameter.h“ zum bearbeiten mit:
```shell
pi@raspberrypi:~/E3dcGui $  nano parameter.h
```
Es wird jetzt die Datei im Bearbeitungsprogramm „nano“ geöffnet.

### Inhalt der „parameter.h“   
Da ich in der Datei alles beschrieben habe, gehe ich jetzt nur auf das wichtigste Einstellungen ein.   
Die wichtigsten Einstellungen sind:
```shell
6    #define GUI                         1
12   #define E3DC_S10                    1
21   #define Homematic_E3DC              1
51   #define Homematic_GUI               0
```
Hier wird die Nutzung der Applikation definiert, also ob du das Display nutzen willst oder nicht und ob die eine HomeMatic anbinden willst oder nicht. Wenn du eine Funktion nutzen willst trag bitte eine „1“ ein sonst eine „0“.

### Änderungen speichern   
Die Änderungen in der „parameter.h“ speicherst du mit „STRG“ und „O“ und beendet wird der Editor mit „STRG“ und „X“.

### Applikation Kompilieren   
Das „Makefile“ ist komplett vorbereitet du brauchst nur noch „make“ in der Kommandozeile eingeben, dann läuft das Kompilieren von alleine durch.

### Wichtig   
Damit die SD-Karte des Raspberry Pi nicht übermäßig beansprucht wird, nutzt ich ein RAMDisk im Arbeitsspeicher. Die Einrichtung ist unten im Kapitel __[RAMDisk](https://github.com/nischram/E3dcGui#ramdisk)__ beschrieben.

### Vorbereitung der HomeMatic CCU /CCU2   
Wenn du in den Einstellungen die Nutzung der HomeMatic aktiviert hast (`#define Homematic_E3DC     1`), muss die HomeMatic nun vorbereitet werden.
Disen Teil der Anleitung habe ich separat in dem Ordner Homematic erstellt.   
[HomeMatic README.markdown](https://github.com/nischram/E3dcGui/tree/master/Homematic)   
Wenn das Display nur für das E3DC S10 genutzt wird (`#define Homematic_E3DC     0`), ist die Anleitung nicht nötig. Sollte die Nutzung aktiviert sein und die HM nicht vorbereitet dann würde die Applikation für jeden Sendebefehl mehrere Sekunden benötigen und nicht ordnungsgemäss laufen. Ausser alle ISE_ID's in der Datei "parameterHM.h" werden auf 0 gesetzt (default).

### Applikation starten
Jetzt kann die Applikation für Tests gestartet werden.
```shell
pi@raspberrypi ~/E3dcGui $  ./start
```
Je nach Einstellungen startet nun nur die RSCP-Applikation oder mehrere Programme starten und das Display sollte die gewünschten Daten anzeigen.

#### Systemvariable prüfen
Wenn genutzt: Kannst du nun in der HomeMatic WebUi im Bereich „Status und Bedienung“ die Systemvariable überprüfen. ([Anleitung Homematic](https://github.com/nischram/E3dcGui/tree/master/Homematic))

### Applikation stoppen
Nun kann die Applikation gestoppt werden, hierfür habe ich ein Stop-Programm erstellt. Dies wird mit folgendem Befehl ausgeführt: `./stop` (Eingabe ist schlecht zu erkennen)

### Autostart der Applikation (durch Crontab-Job)
Damit das Programm nun dauerhaft genutzt werden können, muss die Applikation auf dem Raspberry in den Autostart gebracht werden. Dies geschieht bei einem Raspberry Pi in dem ein Crontab-Job erstellt wird.

Die Crontab ruft man auf mit:
```shell
pi@raspberrypi ~/e3dc-rscp $  crontab -e
```
- Crontab für die Bearbeitung öffnen

In der geöffneten Crontab wird eine neue Zeile mit diesem Inhalt eingefügt:
```shell
@reboot /home/pi/E3dcGui/start
```
Die Änderung wird mit „STRG“ + „O“ gespeichert und die Crontab beendet mit
„STRG“ + „X“.
Weitere Informationen zur Crontab entnehmen Sie bitte aus Quellen wie z. B. dieser:
[https://raspberry.tips/raspberrypi-einsteiger/cronjob-auf-dem-raspberry-pi-einrichten](https://raspberry.tips/raspberrypi-einsteiger/cronjob-auf-dem-raspberry-pi-einrichten)

### Raspberry Pi neu starten
Damit die Applikation gestartet wird, kann nun der Raspberry neu gestartet werden mit:
```shell
pi@raspberrypi ~/E3dcGui $  sudo reboot
```
- Reboot Befehl mit Administrator-Rechten
Der Raspberry Pi startet neu und die Applikation wird im Hintergrund ohne Bildschirmausgaben ausgeführt. Nach ca. 1 Minute werden die Werte der Systemvariablen der HomeMatic wieder aktualisiert. Oder das Display zeigt die Applikation

## Display bedienen
Die Darstellung ist in 5 Bildschirmen unterteilt. Diese werden über die Symbole in der oberen Ziele abgerufen. Sollten Funktionen der Software nicht aktiviert sein, bleiben die Symbole ausgeblendet.

#### 1. Setup
<img src="https://s20.postimg.org/fazpxtg4d/Setup_Neu.jpg" alt="Setup">  
Hier kann die Software oder der PI neu gestartet werden.
Auf der rechten Seite werden Informationen zum Raspberry Pi ausgegeben
#### 2. Aktuelle Werte des S10
`  #define E3DC_S10                    1 `   
<img src="https://s20.postimg.org/7m0rhl63h/Aktuelle_Werte.jpg" alt="Aktuelle_Werte">  
Wenn die Aktuellen Werte des S10 im Display angezeigt werden, wird im Sekundenrhytmus aktualisiert. Sonst kann der Intervall definiert werden `#define SleepTime   1`
Wenn eine externe Quelle (Additional) oder die Wallbox aktiviert sind, wid auch für diese Daten je ein Symbol angezeigt.
#### 3. Langzeit Werte des S10
`  #define E3DC_S10                    1`    
<img src="https://s20.postimg.org/43orl757h/Langzeit_Werte.jpg" alt="Langzeit_Werte">  
Diese Werte werden von der RSCP-Applikation mit ein 15 Minuten Mittelwert gespeichert.
Die verschiedenen Kurven lassen sich durch einen Tipp auf das Symbol in der Legende ein oder ausblenden. Leider reagiert das Display mit der Software nicht empfindlich genug, somit muss eventuell häufiger gedrückt werden um eine Kurve auszublenden. Für Additional gibt es eine Kurve, für die Wallbox habe ich nichts eingebaut.
Damit die verschiedenen PV-Anlagengrößen auch dargestellt werden können, muss die Maximalleistung in der "parameter.h" mit PowerMax definiert werden. Für Große Anlagen ist diese Grafik nicht geeignet. Die Langzeitwerte sind für 24 Stunden und werden durchlaufend dargestellt. Der 0:00 Uhr Punkt verschiebt sich und wird durch eine Linie gekennzeichnet.
#### 4. Monitor
`  #define E3DC_S10                    1`    
<img src="https://s20.postimg.org/d55f3bcnx/Monitor_Neu.jpg" alt="Monitor">  
Hier werden die einzelnen Tracker des Wechselrichters dargestellt.
#### 5. HomeMatic
`  #define Homematic_GUI               1`   
<img src="https://s20.postimg.org/z0fw5rehp/Homematic.jpg" alt="HomeMatic">  
Da es für die HomeMatic kein ideales Display gibt, habe ich diese Software genutzt um mir wichtige Daten der HomeMatic darzustellen. Die Nutzung für euch mit dieser Funktion ist nur mit aufwand möglich. Es müssen nicht nur die ISE_ID der Geräte oder Variablen in der "parameterHM.h" definiert werden, sondern muss auch im Sourcecode einiges geändert werden.   
Es werden verschiedene Etagen, Räume und Geräte angezeigt. Die Werte werden erst abgefragt, und danach die Grafik erstellt (Abfragezeit am roten Punkt unten rechts zu erkennen).
Die Etagen haben eine rote oder grüne Umrandung für den Verschluss aller Fenster und Türen.   
Die Räume haben unterschiedliche Fenstereinteilungen es gibt zum Teil Räume mit zwei Fenster und Jalousien (im Foto an OG Wohnz. zu sehen).   
Die Geräte und Variablen in rechten Bereich sind sogar bedienbar. Also ein Tipp auf's Symbol sendet einen einen Wert zur HomeMatic wie z.B. der Brunnen kann per Display eingeschaltet werden (programmiert in der "screenSaveHM.c").   
Für HM CPU, Sonnenaufgang, Sonnenuntergang, Vollmond usw. habe ich in der HomeMatic Systemvariablen angelegt die ich abfrage.   
Ich biete den Teil der Software hier gerne an, aber da die auf meine HomeMatic und meine Gegebenheiten eingerichtet ist, könnt ihr diesen Teil ggf. nicht nutzen. Ich empfehle erstmal diesen Teil nicht zu aktivieren, also `#define Homematic_GUI          0` eintragen.   
Das senden der Daten mit der RSCP-Applikation, ist hiervon nicht betroffen (`#define Homematic_E3DC       1`).   
Damit du am Sorcecode eigene Änderungen vornehmen kannst, aber gleichzeitig Änderungen von mir in anderen Programmteilen übernehmen kannst habe ich die HM-Darstellung und die Touchfunktion in extra Dateien mit ausgelagert. Zusätzlich habe ich einige Komentarzeilen und Infos als Bearbeitungshilfe eingefügt. Auch die Parameterdatei habe ich getrennt. Wenn du also die HM-Darstellung für deine Zwecke anpasst dann bitte die Datein `HMGui.h`, `screenSaveHM.c` und `parameterHM.h` __nicht__ aktualisieren.

### Bildschirmschoner
Damit des Display nicht permanent hell ist, habe ich eine Bildschirmschoner-Funktion eingefügt. Die Zeit für den Bildschirmschoner ist in der „parameter.h“ auf Zeile 70 definiert: `#define ScreenSaverTime             180`   
Um den Bildschirmschoner zu beenden reicht ein Tipp auf das Display (zum Teil wegen der Empfindlichkeit auch zwei Tipp's), dann wird es wieder hell. Für diesen Tipp ist nicht das ganze Display vorgesehen, da die anderen Funktionen versehentlich ausgelöst werden könnten. Der Bereich ist hier zu sehen:   
<img src="https://s20.postimg.org/9vovscf19/Bildschirmschoner.jpg" alt="Bildschirmschoner">   
Im HomeMatic Menü ist der Bereich noch kleiner. Hier zu sehen:   
<img src="https://s20.postimg.org/6dcvvye59/Bildschirmschoner_HM.jpg" alt="Bildschirmschoner_HM">   
Das Display kann auch sofort Dunkel geschaltet werden, ein Tipp in den oben gezeigten Bereichen reicht hierfür.  

## Werte in der HomeMatic nutzen
Das Nutzen der Werte in der HomeMatic, ist wieder in der Anleitung für HomeMatic in dem Ordner Homematic zu finden.

## WatchDog

Teilweise bleibt die RSCP-Applikation hängen und die Automatische re-connection in der RSCP-Applikation funktioniert leider nicht immer. So wird ein Neustart der RSCP-Applikation nötig.

Für dieses Problem habe ich einen einfachen WatchDog geschrieben. Damit der WatchDog den Betrieb der Applikation überwachen kann, lasse ich mit einem kleinen Teil in der RscpMain, eine Datei im RAMDisk erstellen. In der Datei ist die Unixtime des S10, diese wiederum liest der WatchDog ein und vergleicht diese mit einer definierten Differenz mit der aktuellen Zeit.  

Ein paar Einstellmöglichkeiten für den WatchDog hast du bestimmt schon in der „parameter.h“ gesehen. Wenn die Applikation auch Daten von der HomeMatic holt werden auch diese Daten überwacht. Da ich die HomeMatic nicht so häufig abfrage, sollten die Einstellungen zum WatchDog nicht großartig geändert werden.   
Wenn der Watchdog zuschlägt, erstellt er eine Datei "Watchdog.csv" im E3dcGui Ordner. Somit ist eine Kontrolle der Aktivität möglich. Es wird je Aktivität eine Zeile erstellt, du kannst erkennen was der WatchDog neu gestartet hat.   
Der WatchDog startet den Raspberry Pi auch neu, wenn die Applikation über längere Zeit keine aktuellen Daten (E3DC oder HomeMatic) liefert. Hierdurch ergibt sich noch ein Problem, sollte die Netzwerkverbindung zum System oder die Geräte gestört sein, würde der Raspberry Pi mehrfach mit einem Reboot neu gestartet. Um dies zu stoppen musst du folgendes in der Kommandozeile eingeben:
```shell
pi@raspberrypi:~ $ pkill watchdogHM
```
## Aktuelle Uhrzeit aus dem Internet holen
Wenn der Watchdog den Pi neu startet, bleibt die Uhrzeiteit des Pi nicht Aktuell. Hier können schon mal ein paar Minuten Abweichung entstehen.
Ich habe eine Lösung für diese Problem, auf dieser Seite gefunden:
[http://logicals.at/de/forum/raspberry-pi/48-aktuelle-uhrzeit-aus-dem-internet-holen](http://logicals.at/de/forum/raspberry-pi/48-aktuelle-uhrzeit-aus-dem-internet-holen)

## RAMDisk

RAMDisk am Raspberry erstellen:
Zuerst wird ein Mountpoint für die RAM-Disk erzeugt:
```
sudo mkdir /mnt/RAMDisk
```
Für die RAM-Disk muß die Filesystem Table angepasst werden:
```
sudo nano /etc/fstab
```
Einfügen dieser Zeile am Ende der Datei:
```
tmpfs /mnt/RAMDisk tmpfs nodev,nosuid,size=4M 0 0
```
Die Größe wird über den Parameter "4M" auf 4 MB festgelegt. Jetzt montiert man alle Filesysteme über:
```
sudo mount -a
```
Der Erfolg lässt sich mit Diskfree überprüfen:
```
df
```
Es sollte dann ein Eintrag mit der RAM-Disk zu finden sein:
```
Filesystem 1K-blocks Used Available Use% Mounted on
rootfs 15071704 2734624 11674436 19% /
/dev/root 15071704 2734624 11674436 19% /
devtmpfs 218620 0 218620 0% /dev
tmpfs 44580 236 44344 1% /run
tmpfs 5120 0 5120 0% /run/lock
tmpfs 89140 0 89140 0% /run/shm
/dev/mmcblk0p1 57288 19712 37576 35% /boot
tmpfs 4096 0 4096 0% /mnt/RAMDisk
```

Diesen Teil zum RAMDisk habe ich von hier Kopiert:
[http://www.kriwanek.de/raspberry-pi/486-ram-disk-auf-raspberry-pi-einrichten.html](http://www.kriwanek.de/raspberry-pi/486-ram-disk-auf-raspberry-pi-einrichten.html)

## eMail senden Installieren und aktivieren
Damit der Wachtdog oder andere Programmteile eine eMail senden können, muss eine eMail Option installiert werden. Ich habe mich hier für "SendEmail" entschieden. Die Version 1.56 habe ich im Github integriert, somit muss die Software nicht Heruntergeladen werden, was das installieren wesentlich einfacher macht. Folgendes Befehle sind der Reihe nach auszuführen:
```
pi@raspberry:~$ cd E3dcGui
pi@raspberrypi ~/E3dcGui $ sudo cp -a sendEmail-v1.56/sendEmail /usr/local/bin
pi@raspberrypi ~/E3dcGui $ sudo chmod +x /usr/local/bin/sendEmail
pi@raspberrypi ~/E3dcGui $ sudo apt-get update
pi@raspberrypi ~/E3dcGui $ sudo apt-get install libcrypt-ssleay-perl
pi@raspberrypi ~/E3dcGui $ sudo apt-get install libnet-ssleay-perl
pi@raspberrypi ~/E3dcGui $ sudo apt-get install libssl-dev
pi@raspberrypi ~/E3dcGui $ sudo apt-get install libio-socket-ssl-perl
```
Jetzt müssen die eMail Einstellung in den "parameter.h" definiert werden.
```
// sendEmail Parameter
96   #define FromEmailAdress             "max.mustermann@web.de"
97   #define smtpServer                  "smtp.web.de"
98   #define smtpPort                    "587"
99   #define smtpTLS                     "yes"
100   #define smtpBenutzer                "max.mustermann@web.de"
101   #define smtpPass                    "1234abc"
```
Dies ist für Web.de (von mir getestet) und so muss es für gmail.com aussehen.
```
// sendEmail Parameter
#define FromEmailAdress             "max.mustermann@gmail.com"
#define smtpServer                  "smtp.gmail.com"
#define smtpPort                    "587"
#define smtpTLS                     "yes"
#define smtpBenutzer                "max.mustermann@gmail.com"
#define smtpPass                    "1234abc"
```
Für den Watchdog ist in der "parameter.h" noch die eMail-Adresse einzustellen in der die Nachrichten gesendet werden sollen:
```
92   #define WDtoEmailAdress             "max.mustermann@web.de"
```
Mit den Parametern
```
90   #define WDsendEmailReboot           1
91   #define WDsendEmailKill             0
```
kann noch definiert werden ob für Kill und/oder Reboot die eMail gesendet werden soll. Beide Parameter auf "0", dann wird keine eMail gesendet und die Software muss __nicht__ installiert werden.

## Material
Ich nutze die Software auf einem Komplettpaket von Conrad. Das Set besteht aus dem Raspberry Pi 3, SD-Karte (Noobs vorinstalliert), 7-Zoll Raspberry Touchdisplay, Standgehäuse und Netzteil.  
Hier die Artikelnummer von Conrad: [1437995-62](https://www.conrad.de/de/raspberry-pi-3-model-b-starter-set-1-gb-noobs-inkl-betriebssystem-noobs-inkl-gehaeuse-inkl-netzteil-inkl-software-1437995.html)   
Die Grundinstallation ist einfach, da es im Internet schon viele Anleitungen hierzu gibt, gehe ich im Moment hierauf nicht näher ein.

### Desktop deaktivieren
Es muss die Desktopanwendung „startx“ deaktiviert werden, dies kannst du im Raspberry Konfigurator machen:
```
pi@raspberry:~$ sudo raspi-config
```
Dort unter „Boot Options“ > „B2 Console Autologin Text console, automatically logged in as 'pi' user“ auswählen.

### W-Lan einrichten
Zuvor kannst du dein W-Lan scannen, um zu sehen ob der Pi empfang hat:
```
sudo iwlist wlan0 scan | egrep "(ESSID|IEEE)"
```
Dann wird mit folgendem Befehl ein Editor mit einer Datei geöffnet, in der du eine W-Lan Zugangsdaten eintragen kannst:
```
sudo nano /etc/wpa_supplicant/wpa_supplicant.conf
```
Die Einstellung erfolgt in dieser Form:
```
network={
    ssid="W-LAN_SSID"
    psk="ganzgeheimespasswort"
}
```
Bitte nicht vergessen, dass nur der Pi3 W-Lan on Board hat, bei einem älteren Pi musst du entweder per Kabel oder per W-Lan USB-Stick die Netzwerkverbindung herstellen.

## Umschaltung Sommer-/ Winterzeit
Auf der GUI im Menü Setup gibt es ein Umschaltbutton für die Umschaltung zwischen Sommerzeit und Winterzeit. Damit die Einstellung wirksam wird startet die Software nach dem betätigen des Button neu.

Wenn diese Software ohne Display verwendet wird, muss die Einstellung der Sommer oder Winterzeit von Hand vorgenommen werden, dafür muss in der Datei "Timezone.txt" im Ordner "Data" entweder "Wintertime" oder "Summertime" eingetragen werden.

## RSCP to Loxone
Es besteht eine Möglichkeit meine Software auch für Loxone statt für Homematic zu nutzen. Dies ist in einer separaten Anleitung beschrieben.
[Loxone README.markdown](https://github.com/nischram/E3dcGui/blob/master/Loxone)

## ToDo
01 weiteres in der Aneitung zum Aufbau
02 Dokumentation im Sourcecode
03 Loxone Unterstützung aufnehmen

## Quelle

#### RSCP
Downloadbereich E3DC Kundenportal [https://s10.e3dc.com](https://s10.e3dc.com)
"Zusätzliche Optionen" > " RSCP-Beispielapplikation_2016-04-25.pdf"
[Link](https://s10.e3dc.com/s10/module/download/get.php?dl=3408) (Benutzerdaten erforderlich)
#### Grafiken
Bilschirmfotos aus dem E3DC Portal (Ich hoffe E3DC hat nichts dagegen!?)

## Changelog
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
- Links eingefügt
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

[Changelog Archiv](https://github.com/nischram/E3dcGui/tree/master/Changelog_Archiv)
