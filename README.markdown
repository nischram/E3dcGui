# E3DC to Homematic mit GUI
[Stand: V1.89 03.05.2021](https://github.com/nischram/E3dcGui#changelog)

Hier beschreibe ich, wie du dein S10 Hauskraftwerk von E3DC an eine Homematic Hausautomation anbinden kannst.

Als Schnittstelle zwischen S10 und Homematic dient ein Raspberry Pi. Die Applikation, kann auf dem Raspberry Pi vielseitig eingesetzt werden.

__Daten per RSCP vom E3DC-S10 Speicher abfragen__  
__Daten vom E3DC zur Homematic Hausautomation senden__  
__Daten vom S10 auf dem Display darstellen__  
__Daten von der Homematic auf dem Display darstellen__  
__Daten der E3DC Wallbox anzeigen oder steuern__  
__Werte aus der Homematic lesen und an die Wallbox senden__  
__Kleine Hausautomation mit GPIO's__  
__Wetterinformationen von "OpenWeatherMap"__  
__Daten einer Grünbeck SC18 anzeigen__  
__Daten einer Grünbeck SC18 zur Homematic senden__  
__Dein eigener Abfuhrkalender__  
__LED Stausanzeige__   

Die verschiedenen Möglichkeiten können in unterschiedlichen Konstellationen zusammen genutzt werden.  
Es kann entweder ein Raspberry Pi __ohne Display__ eingesetzt werden.  
Oder es wird ein Raspberry Pi mit __7“ Display__ eingesetzt um diverse Daten darzustellen.   

<img src="https://s20.postimg.cc/kxtyqs93x/E3_DC_Display.jpg" alt="E3DC-Display">  
Hier ein Foto mit der Software auf dem 7-Zoll Display.  

Diese Kombination bietet eine sehr gute Möglichkeit, um das S10 einfach und schnell mit einem eigenem Display zu überwachen.

Ich nutze diese Applikation auf einem Raspberry Pi, mit dem 7“ Raspberry Touchdisplay. Auf das __[Material](https://github.com/nischram/E3dcGui#material)__ und die Installation gehe ich in dieser Anleitung ein.

## Funktion
In der Applikation werden die Daten vom S10 mit der RSCP-Schnittstelle vom S10 abgefragt. Eine Beispiel-Applikation zum RSCP bietet E3DC in Downloadbereich an.  
Die abgefragten Werte vom S10 wie z.B. Leistungswerte oder Batteriefüllstand, werden auf Wunsch per CURL-Befehl an die XML-API Schnittstelle der Homematic gesendet.  
Die Meisten Funktionen meiner Software sind für das 7" Display geschrieben.

## Vorbereitung
### am S10  
Am S10 muss im „Hauptmenü“ unter „Einstellungen“ > "Personalisieren" ein RSCP-Passwort gesetzt werden. Das gesetzte Passwort wird später in den Parametern der Software benötigt.
Im selben Menü „Einstellungen“ > "Netzwerk" ist die IP-Adresse des S10 zu finden.

### Raspberry Pi  
Die Erstinstallation eines Raspberry Pi erkläre ich hier nicht im Detail, nur mit einer Schritt für Schritt Zusammenfassung von diversen Links.  
[Schritt für Schritt](https://github.com/nischram/E3dcGui/blob/master/STEPBYSTEP.markdown)  
Diese Anleitung setzt einen lokalen Zugriff oder SSH-Zugriff auf den Raspberry voraus.

### Netzwerk   
Es ist Wichtig darauf zu achten, dass der Raspberry Pi im selben Netzwerk wie das S10 installiert ist. Der IP-Adressbereich muss gleich sein, zum Beispiel so: 192.168.178.xxx  
Im Gastzugang eines Routers wird die Verbindung fehlschlagen, auch darf die Verbindung zwischen S10 und Raspberry Pi nicht über eine VPN-Verbindung hergestellt werden.

## Installation der E3dcGui
### Applikation auf den Raspberry kopieren   
Jetzt wird der Lokale oder der SSH-Zugang zum Raspberry benötigt. Ich mache dies per SSH mit dem „Terminal“ für MAC. Möglich ist dies unter Windows mit "PuTTy".
Jetzt folgt der Download vom Github: (Es ist __kein__ Github Account nötig.)

```shell
git clone git://github.com/nischram/E3dcGui.git ~/E3dcGui
```

### Einstellungen vornehmen   
In der Datei „parameter.h“ im Ordner E3dcGui kann nun alles eingestellt werden, was du zum nutzen der Software benötigst. Entweder öffnet man die Datei mit einem externen Editor. Hier können diverse Editoren zum Einsatz kommen, ich nutze auf meinem Mac „Atom“ und lade die Datei mit „Cyberduck“ runter. Unter Windows ist das runterladen und bearbeiten z.B. mit WinSCP möglich. Alternativ kann man die Datei direkt auf dem Raspberry bearbeiten, dies beschreibe ich jetzt.  
Alle Befehle müssen mit Groß- und Kleinschreibung korrekt übernommen werden.
Zuerst mit folgendem Befehl in den Ordner E3dcGui wechseln:

```shell
pi@raspberrypi:~ $  cd E3dcGui
```
Öffnen der Datei „parameter.h“ zum bearbeiten mit:

```shell
pi@raspberrypi:~/E3dcGui $  nano parameter.h
```
Es wird jetzt die Datei im Bearbeitungsprogramm „nano“ geöffnet und du kannst die Bearbeitung vornehmen.

Nach dem Bearbeiten müssen die Änderungen gespeichert werden. Die Änderungen in der „parameter.h“ speicherst du mit „STRG“ + „O“ und beendet wird der Editor mit „STRG“ + „X“.

### Inhalt der „parameter.h“   
Da ich in der Datei alles beschrieben habe, gehe ich jetzt nur auf das wichtigste Einstellungen ein.  
__Grundfunktionen__:  

```shell
#define GUI                         1
#define E3DC_S10                    1
#define Homematic_E3DC              1
#define Homematic_GUI               0
#define wetterGui                   1
#define Gruenbeck                   0
#define Abfuhrkalender              1
#define useAktor                    0
#define useDHT                      0
#define E3DC_LED                    0
```
Hier wird die Nutzung der Applikation definiert, also ob du das Display nutzen willst oder nicht und ob du eine Homematic anbinden willst oder nicht. Wenn du eine Funktion nutzen willst trag bitte eine „1“ ein sonst eine „0“.

### Zusätliche Software
Vor dem ersten kompilieren der Applikation, musse auf dem Raspberry __WiringPi__ installiert werden. Eine Anleitung habe ich im Wiki unter [WiringPI einrichten](https://github.com/nischram/E3dcGui/wiki/WiringPI).  
Auch muss zuvor __libcurl4__ installiert werden, wird für die Wetterdaten benötigt (Anleitung siehe unter "2. Wetteranzeige").  

### Speicherort RAMDisk einrichten  
Damit die SD-Karte des Raspberry Pi nicht übermäßig beansprucht wird, nutzt ich ein RAMDisk im Arbeitsspeicher. Die Einrichtung ist unten im Kapitel __[RAMDisk](https://github.com/nischram/E3dcGui/wiki/RAMDisk)__ beschrieben.

### Vorbereitung der Homematic CCU /CCU2   
Wenn du in den Einstellungen die Nutzung der Homematic aktiviert hast `#define Homematic_E3DC     1`, muss die Homematic nun vorbereitet werden.
Diesen Teil der Anleitung habe ich separat in dem Ordner Homematic erstellt.  
[Homematic README.markdown](https://github.com/nischram/E3dcGui/tree/master/Homematic)  
Wenn das Display nur für das E3DC S10 genutzt wird `#define Homematic_E3DC     0`, ist die Anleitung nicht nötig. Sollte die Nutzung aktiviert sein und die HM nicht vorbereitet dann würde die Applikation für jeden Sendebefehl mehrere Sekunden benötigen und nicht ordnungsgemäss laufen. Ausser alle ISE_ID's in der Datei "parameterHM.h" werden auf 0 gesetzt (default).

### Applikation Kompilieren   
Das „Makefile“ ist komplett vorbereitet, das Kompilieren läuft von alleine durch.

```shell
pi@raspberrypi ~/E3dcGui $  make
```
Weitere Informationen zum "make" findest du hier: __[Makefile](https://github.com/nischram/E3dcGui#makefile)__

### Applikation starten
Jetzt kann die Applikation für Tests von Hand gestartet werden.

```shell
pi@raspberrypi ~/E3dcGui $  ./start
```
Je nach Einstellungen startet nun nur die RSCP-Applikation oder mehrere Programme starten und das Display sollte die gewünschten Daten anzeigen.

### Systemvariable prüfen
Wenn genutzt, kannst du nun in der Homematic WebUi im Bereich „Status und Bedienung“ die Systemvariable überprüfen. ([Anleitung Homematic](https://github.com/nischram/E3dcGui/tree/master/Homematic))

### Applikation stoppen
Nun kann die Applikation gestoppt werden, hierfür habe ich ein Stop-Programm erstellt. Dies wird mit folgendem Befehl ausgeführt: `./stop`  
Die Eingabe ist schlecht zu erkennen, weil die eigentlichen Ausgaben durchlaufen.

### Autostart der Applikation (durch Crontab-Job)
Damit das Programm nun dauerhaft genutzt werden können, muss die Applikation auf dem Raspberry in den Autostart gebracht werden. Dies geschieht bei einem Raspberry Pi in dem ein Crontab-Job erstellt wird.

Die Crontab ruft man auf mit:

```shell
pi@raspberrypi ~/E3dcGui $  crontab -e
```
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
Der Raspberry Pi startet neu und die Applikation wird im Hintergrund ohne Bildschirmausgaben ausgeführt. Nach ca. 1 Minute werden die Werte der Systemvariablen der Homematic wieder aktualisiert. Oder das Display zeigt die Applikation

## Display bedienen und einrichten
Die Darstellung ist in bis zu 10 Bildschirmen unterteilt. Diese werden über die Symbole in der oberen Ziele abgerufen. Sollten Funktionen der Software nicht aktiviert sein, bleiben die Symbole ausgeblendet.

### 1. Setup
<img src="https://s20.postimg.cc/c6bv32kpp/Setup_V1-52.jpg" alt="Setup">  
Hier kann die Software oder der PI neu gestartet werden.  
Auf der rechten Seite wird die Display-Helligkeit eingestellt. Auch kannst du hier den PIR-Sensor aktivieren oder deaktivieren werden.  
Unterhalb werden Informationen zum Raspberry Pi ausgegeben.   

### 2. Wetteranzeige
<img src="https://s20.postimg.cc/5ewglai59/Wetter.jpg" alt="Wetteranzeige">     
Hier werden Standort bezogene Wetterdaten eingeblendet. Die Daten basieren auf einen Service, welcher die aktuellen Wetterdaten von OpenWeatherMap holt.
Du musst dir zuvor einen kostenlosen Account bei [OpenWeatherMap](https://openweathermap.org/guide#how) einrichten. Dort bekommst du einen API-Key den du hier eintragen musst:  
`  #define WEATHER_KEY                      "12345678910111213141516171819202"`   
Die OpenWeatherMap Anzeige ist auf die "OneCall-API" angepasst, leider ist in dieser API die Standortsuche nur per Koordinaten möglich, so musst du für dein Standort die Koordinaten ermittel und diese unter WEATER_LON und WEATHER_LAN eintragen. Der Name der Stadt und vom Land sind nur für die Anzeige im Display.
Da eventuell nicht alle einen Account einrichten wollen, kann man die Wetter-Seite ausblenden. Dies erfolgt mit:   
`  #define wetterGui                   0`  
Die Wetteranzeigen kann auch ohne S10 oder Homematic genutzt werden.  

__Wichtig__: für die Abfragen muss auf dem Raspberry noch eine CURL-Library installiert werden. Es muss folgender Befehl durchgeführt werden:  

```shell
sudo apt-get install libcurl4-openssl-dev
```

### 3. Aktuelle Werte des S10
`  #define E3DC_S10                    1 `  
<img src="https://s20.postimg.cc/7m0rhl63h/Aktuelle_Werte.jpg" alt="Aktuelle_Werte">  
Wenn die Aktuellen Werte des S10 im Display angezeigt werden, wird im Sekundenrhythmus aktualisiert. Sonst kann der Intervall definiert werden `#define SleepTime   1`
Wenn eine externe Quelle (Additional) oder die Wallbox aktiviert sind, wird auch für diese Daten je ein Symbol angezeigt.
Mit dem Symbol "History Today" können die HistoryValues für den Aktuellen Tag eingeblendet werden, ein weiteres Tippen jetzt auf "History Yesterday" blendet die Energiewerte für den Vortag ein und danach kann mit dem Tippen auf "History Off" die Einblendung abgeschaltet werden. Unter dem Symbol, mit dem man in die nächste Ansicht wechseln kann, wird der Zeitstempel der Daten angezeigt. In der parameter.h kannst du definieren ob du die Abfrage der HistoryValues aktivieren willst (historyAktiv) und in welchem Rhythmus die Daten abgefragt werden sollen (historyDelay).

### 4. Langzeit Werte des S10
`  #define E3DC_S10                    1`  
<img src="https://s20.postimg.cc/43orl757h/Langzeit_Werte.jpg" alt="Langzeit_Werte">  
Diese Werte werden von der RSCP-Applikation mit ein 15 Minuten Mittelwert gespeichert.
Die verschiedenen Kurven lassen sich durch einen Tipp auf das Symbol in der Legende ein oder ausblenden. Leider reagiert das Display mit der Software nicht empfindlich genug, somit muss eventuell häufiger gedrückt werden um eine Kurve auszublenden. Für Additional gibt es eine Kurve, für die Wallbox habe ich nichts eingebaut.
Damit die verschiedenen Größen der PV-Anlagen auch dargestellt werden können, muss die Maximalleistung in der „parameter.h" mit PowerMax definiert werden. Für Große Anlagen ist diese Grafik nicht geeignet. Die Langzeitwerte sind für 24 Stunden und werden durchlaufend dargestellt. Der 0:00 Uhr Punkt verschiebt sich und wird durch eine Linie gekennzeichnet.

### 5. Wallbox
`  #define Wallbox                    1`  
![Wallbox](https://user-images.githubusercontent.com/19279623/112896384-9dd42e80-90de-11eb-9140-25904abe8fb7.jpg)  
Hier werden Inofrmationen zu deiner E3DC-Wallbox angezeigt. Einige Parameter kann man per Schalter steuern. Gesteuert werden kann:
- Ladestrom > maximaler Ladestrom je Phase mit dem das Auto geladen wird.
- Sonnenmodus > wechseln zwischen Sonnenmode oder Mischbetrieb.
- Batterie vor Auto > hiermit wird zuvor die Batterie geladen dann erst das Auto. *
- Batterie zu Auto > hiermit wird freigegeben ob die Batterie entladen werde darf. *  
  *sind abhängig voneinander  

In der Ansicht ist zu erkennen ob das Auto, angeschlossen und verriegelt ist. Bei der Ladung ist farblich zu erkennen, ob die Ladung aus dem Netz, der Sonne oder gemischt kommt (Schwellwert je bei 200W).  
Ich habe die Funktion an meiner E3DC Walbox mit CAN-Bus getestet, die E3DC Easy Connect Wallbox mit Netzwerk hat Thomas (Issue #55) getestet.

### 6. Monitor
`  #define E3DC_S10                    1`  
<img src="https://s20.postimg.cc/d55f3bcnx/Monitor_Neu.jpg" alt="Monitor">  
Hier werden links die einzelnen Tracker des Wechselrichters dargestellt. Rechts ist für neue Ideen noch Platz, gerne darfst du einen Issue erstellen wenn du wünsche hast!

### 7. SmartHome
<img src="https://s20.postimg.cc/ukme71x0d/Smart_Home.jpg" alt="SmartHome">  
`  #define useDHT                      1`    
Du kannst am Raspberry den Standard Temperatur/Luftfeuchtigkeits-Sensor DHT11 oder DHT22 anschließen. Ich habe in der parameter.h für fünf Sensoren die Einstellungen vorbereitet. Diese Fünf werden dann auf der linken Seite angezeigt. Der rote oder grüne Punkt zeigt den Status und die erfolgreiche Kommunikation zum Sensor. _Achtung:_ Sensor DHT22 ist nicht möglich!     
`  #define useAktor                    1`  
Rechts ist der Status von Schaltaktoren zu sehen. Diese Aktoren können in der parameter.h definiert und den entsprechenden GPIO's zugeordnet werden. Mit den GPIO's ist es dann möglich zum Beispiel eine Relaisplatine anzusteuern. Mit der Platine kannst du dann ein Schütz in deiner Installation aktivieren und z.B. dein Heizstab ansteuern. Diese Funktion ist für alle die keine Homematic angebunden haben, aber trotzdem ein Gerät bei Überschuss aktivieren möchten. Getestet habe ich die Funktion mit einem "2 Kanal 5V Relais Modul für Arduino". Zur Auswahl der Aktoren stehen ein Überschussaktor, ein Aktor für Solarleistung, einer für den Batterie-SOC und ein Zeitaktor. Der Überschuss und der Solar-Aktor schalten ein wenn die Bedingung mindestens 2 Minuten überschritten wird, wenn die Leistung unter 90% vom Sollwert sinkt beginnt die Zeit neu. Abgeschaltet wird wenn die Bedingung 30 Sekunden unterschritten wird. Der Batterieaktor schaltet sofort sobald der Wert überschritten oder unterschritten wird. Auch hier darfst du weitere Ideen, Anregungen oder Fehler gerne als Issue erstellen. Die mindest Einschatzeit und die mindest Auschaltzeit, kann in Minuten definiert werden, dies ist z.B. für die Ansteuerung einer Spülmaschiene wichtig. Die Vergebene Priorität wird unter der Statuslampe angezeigt. Der Status ist hellrot wenn die Priorität erreicht ist sonst dunkelrot. Die Priorität muss in der parameter.h deklariert werden. Es kann von 1-5 gesetzt werden, bei "0" ist keine Priorität vergeben. Weiter ist es möglich den Aktoren ein Zeitfenster zu zuweisen. Nur in diesem Fenster schaltet der Aktor ein und zum Ende des Zeitfensters aus, ein gestarteter Aktor mit einer mindest Einschaltzeit läuft noch so lange, bis zum ablauf der Mindestzeit.

### 8. Homematic
`  #define Homematic_GUI               1`  
<img src="https://s20.postimg.cc/z0fw5rehp/Homematic.jpg" alt="Homematic">  
Da es für die Homematic kein ideales Display gibt, habe ich diese Software genutzt um mir wichtige Daten der Homematic darzustellen. Die Nutzung für euch mit dieser Funktion ist nur mit Aufwand möglich. Es müssen nicht nur die ISE_ID der Geräte oder Variablen in der "parameterHM.h" definiert werden, sondern muss auch im Sourcecode einiges geändert werden.  
Es werden verschiedene Etagen, Räume und Geräte angezeigt. Die Werte werden erst abgefragt, und danach die Grafik erstellt (Abfragezeit am roten Punkt unten rechts zu erkennen).
Die Etagen haben eine rote oder grüne Umrandung für den Verschluss aller Fenster und Türen.  
Die Räume haben unterschiedliche Fenstereinteilungen es gibt zum Teil Räume mit zwei Fenster und Jalousien (im Foto an OG Wohnz. zu sehen).  
Die Geräte und Variablen in rechten Bereich sind sogar bedienbar. Also ein Tipp auf's Symbol sendet einen einen Wert zur Homematic wie z.B. der Brunnen kann per Display eingeschaltet werden (programmiert in der "screenSaveHM.c").  
Für HM CPU, Sonnenaufgang, Sonnenuntergang, Vollmond usw. habe ich in der Homematic Systemvariablen angelegt die ich abfrage.  
Ich biete den Teil der Software hier gerne an, aber da die auf meine Homematic und meine Gegebenheiten eingerichtet ist, könnt ihr diesen Teil ggf. nicht nutzen. Ich empfehle erstmal diesen Teil nicht zu aktivieren, also `#define Homematic_GUI          0` eintragen.  
Das senden der Daten mit der RSCP-Applikation, ist hiervon nicht betroffen `#define Homematic_E3DC       1`.  
Damit du am Sourcecode eigene Änderungen vornehmen kannst, aber gleichzeitig Änderungen von mir in anderen Programmteilen übernehmen kannst habe ich die HM-Darstellung und die Touchfunktion in extra Dateien mit ausgelagert. Zusätzlich habe ich einige Kommentarzeilen und Infos als Bearbeitungshilfe eingefügt. Auch die Parameterdatei habe ich getrennt. Wenn du also die HM-Darstellung für deine Zwecke anpasst dann bitte die Dateien `HMGui.h`, `screenSaveHM.c` und `parameterHM.h` __nicht__ aktualisieren.

### 9. Grünbeck softliQ SC18
`  #define Gruenbeck               1`  
<img src="https://s20.postimg.cc/4d5nbw3kt/Gr_nbeck.jpg˘" alt="Gruenbeck">  
Du kannst mit dem Display Informationen einer Wasserenthärtungsanlage von Grünbeck holen und anzeigen lasen. Ich habe die Grünbeck softliQ SC18 eingebunden und lasse z.B. die Anlagenkapazität, den Verbrauch, die Restkapazität anzeigen, etc. Den Verbrauch summiere ich zum Monatsverbrauch, Jahresverbrauch und Gesamtverbrauch. Die Verbrauchsdaten werden jeden Tag in einer CSV-Datei gespeichert.  
__Wichtig__ ist für diese Funktion die Installation von XMLLINT mit folgndem Befehl:
`sudo apt-get install libxml2-utils`  
Weiter ist es möglich die Daten zur Homematic zu senden. `#define GruenbeckHM 1` Es werden in der Homematic entsprechende Systemvariablen benötigt.  
Es sollten alle Greäte der Serie softliQ von Grünbeck auslesbar sein. Es könnten Probleme bei anderen Geräten entstehen da diese zum Teil 2 Austauscher haben. Es müssten ggf. Anpassungen an der `External/Gruenbeck.h` und an `External/gruenSave.c` der vorgenommen werden.  
Es ist auch möglich eine Grünbeck __ohne Display__ am Raspberry mit einer Homematic zu verbinden. Hierfür müssen an der `parameter.h` und der `External/Gruenbeck.h` die entsprechenden Einstellungen vorgenommen werden. Das `make` muss ausgeführt werden, dann kann die Datei `External/gruenSave` z.B. per crontab in entsprechenden Abständen ausgeführt werden. Mit entsprechenden Systemvariablen und Scripten kann auch in der Homematic die Monats und Jahresberechnung vorgenommen werden. Auf wunsch kann ich per PN die Scripte zusenden.

### 10. Abfuhrkalender
`  #define Abfuhrkalender               1`  
<img src="https://s20.postimg.cc/brr85ul3x/Muell.jpg˘" alt="Entsorgung">  
Hier kannst du dir deinen eigenen Entsorgungskalender einpflegen. Es werden dann die Aktuelle und die nächste Kalenderwoche angezeigt. Unter der Grafik ist eine Legende eingeblendet, die bei Bedarf auch deaktiviert werden kann. Für die Pflege musst du in dem Ordner "Data" die Datei "Entsorgung_2017.txt" anpassen. Wichtig ist, dass du das Format, die Aufteilung und Zeilenreihenfolge nicht veränderst. Für die Bearbeitung empfehle ich OpenOffice oder Excel, die Bearbeitung ist in einer Tabelle am besten. Es ist möglich bis zu zwei Tonnen an einem Tag einzupflegen. Es bestehen die Möglichkeit für Biomüll, Papier, Gelber Sack, Restmüll, Glas, Metall, Schadstoffmobil und Feiertag.
Für das nächste Jahr musst du eine neue Datei mit der Jahreszahl anlegen, dann wird diese zum Jahreswechsel automatisch verwendet.

### Bildschirmschoner
Damit das Display nicht permanent hell ist, habe ich eine Bildschirmschoner-Funktion eingefügt. Die Zeit für den Bildschirmschoner ist in der „parameter.h“ auf Zeile 80 definiert:  
`#define ScreenSaverTime             180`  
Um den Bildschirmschoner zu beenden reicht ein Tipp auf das Display, dann wird es wieder hell. Für diesen Tipp ist nicht das ganze Display vorgesehen, da die anderen Funktionen versehentlich ausgelöst werden könnten. Der Bereich ist hier zu sehen:  
<img src="https://s20.postimg.cc/9vovscf19/Bildschirmschoner.jpg" alt="Bildschirmschoner">  
Im Homematic Menü ist der Bereich noch kleiner. Hier zu sehen:  
<img src="https://s20.postimg.cc/6dcvvye59/Bildschirmschoner_HM.jpg" alt="Bildschirmschoner_HM">  
Das Display kann auch sofort Dunkel geschaltet werden, ein Tipp in den oben gezeigten Bereichen reicht hierfür.  

### Bewegungsmelder
Es besteht die Möglichkeit den Bildschirmschoner mit Hilfe eines Bewegungssensors zu deaktivieren. Hier geht es zur Anleitung im Wiki: [Bewegungsmelder](https://github.com/nischram/E3dcGui/wiki/Bewegungsmelder)   

### Wichtig Pi-Funktionen deaktivieren
Der Raspberry Pi hat einen eignen Bildschirmschoner und eine Desktopanwendung, diese müssen deaktiviert werden. Hierfür siehe unter [Pi-Funktionen deaktiviern](https://github.com/nischram/E3dcGui/wiki/Pi-Funktionen-deaktivieren) im Wiki.

## Werte in der Homematic nutzen
Das Nutzen der Werte in der Homematic, ist in der [Anleitung für Homematic](https://github.com/nischram/E3dcGui/tree/master/Homematic) in dem Ordner Homematic zu finden.
Die Wallbox-Werte werden an die Homematic gesendet, es können Paremeter abgefragt werden und zur Wallbox gesendet werden.

## LED Statusanzeige
Mit RGB-LED's kannst du eine Statusanzeige der E3DC S10 Werte erstellen.  
Hierfür habe ich die Bibliothek von J.Garff verwendet und angepasst. Du kannst eine oder bis zu 12 LED's anschließen und einrichten. Es gibt folgende möglichkeiten für die LED's:   
- Batteriefüllstand mit 1x oder 4x LED  
- Solarproduktion  
- Einspeisung / Bezug  
- Hausverbrauch  
- Batterieladung / Endladung  
- Fehler-Status  
- DC-Leistung je PV-Tracker 2x LED  
- Leistung externe Quellen  

Als LED kannst du alle verwenden die einen WS2811 / WS2812 Controller integriert haben. Das Display ist für die Statusanzeige nicht erforderlich. Als Beispiel können folgende LED's oder Streifen verwendet werden:  
[https://www.amazon.de/PL9823-F8-bgl-WS2812-integrierter-Controller/dp/B00L9I078W](https://www.amazon.de/PL9823-F8-bgl-WS2812-integrierter-Controller/dp/B00L9I078W)   
[https://www.amazon.de/gp/product/B00PTEP14W](https://www.amazon.de/gp/product/B00PTEP14W)  
Für die Installation musst du in der parameter.h 'E3DC_LED' auf 1 setzen. Im E3dcGui Verzeichniss muss die Bibliothek von J.Garff kopiert werden danach musst du die LED's in der External/LedMain.c konfigurieren und zu letzt alles Kompilieren. Folgende Befehle sind nötig:

```shell
pi@raspberrypi:~ $ cd E3dcGui
```

```shell
pi@raspberrypi:~/E3dcGui $ git clone https://github.com/jgarff/rpi_ws281x.git
```

```shell
pi@raspberrypi:~/E3dcGui $ cd External
```

```shell
pi@raspberrypi:~/E3dcGui/External $ sudo nano LedMain.c
```

```shell
pi@raspberrypi:~/E3dcGui/External $ make
```
Informationen zum Anschluss der LED findest du z.B. hier:  
[https://learn.adafruit.com/neopixels-on-raspberry-pi/wiring](https://learn.adafruit.com/neopixels-on-raspberry-pi/wiring)  
Für die Idee der LED-Statusanzeige danke ich HCM_Stefan aus dem Homematic-Forum.

## WatchDog

Es kann Teilweise zu abstürzen und Problemen in der Applikation oder der RSCP-Schnittstelle kommen.

Für diese Probleme habe ich einen einfachen WatchDog geschrieben. Damit der WatchDog den Betrieb der Applikation überwachen kann, lasse ich mit einem kleinen Teil in der RscpMain, eine Datei im RAMDisk erstellen. In der Datei ist die Unixtime des S10, diese wiederum liest der WatchDog ein und vergleicht diese mit einer definierten Differenz mit der aktuellen Zeit.  

Ein paar Einstellmöglichkeiten für den WatchDog hast du bestimmt schon in der „parameter.h“ gesehen. Wenn die Applikation auch Daten von der Homematic holt können auch diese Daten überwacht werden. Da ich die Homematic nicht so häufig abfrage, sollten die Einstellungen zum WatchDog nicht großartig geändert werden.  
Wenn der Watchdog zuschlägt, erstellt er eine Datei "Watchdog.csv" im E3dcGui/Data Ordner. Somit ist eine Kontrolle der Aktivität möglich. Es wird je Aktivität eine Zeile erstellt, du kannst erkennen was der WatchDog neu gestartet hat.  
Der WatchDog startet den Raspberry Pi auch neu, wenn die Applikation über längere Zeit keine aktuellen Daten (E3DC oder Homematic) liefert. Hierdurch ergibt sich noch ein Problem, sollte die Netzwerkverbindung zum System oder die Geräte gestört sein, würde der Raspberry Pi mehrfach mit einem Reboot neu gestartet. Um dies zu stoppen musst du folgendes in der Kommandozeile eingeben:

```shell
pi@raspberrypi:~ $ killall watchdog
```

## Makefile
Das Makefile ist so aufgebaut, dass jedes Programmteil auch einzeln Kompiliert werden kann. Folgende Befehle sind möglich:   

```shell
make       
make watchdog   
make screenSave   
make screenSaveHM   
make GuiMain   
make start   
make stop   
make RscpMain   
make Rscp/RscpWb   
make S10history/S10history   
make Frame/touchtest
```
Diese Möglichkeit erspart zum Teil einiges an Zeit, wenn du eigene Änderungen testen möchtest.

## Aktuelle Uhrzeit aus dem Internet holen
Wenn der Watchdog den Pi neu startet, bleibt die Uhrzeit des Pi nicht Aktuell. Hier können schon mal ein paar Minuten Abweichung entstehen.
Ich habe eine Lösung für diese Problem, auf dieser Seite gefunden:
[http://logicals.at/de/forum/raspberry-pi/48-aktuelle-uhrzeit-aus-dem-internet-holen](http://logicals.at/de/forum/raspberry-pi/48-aktuelle-uhrzeit-aus-dem-internet-holen)

## E-Mail senden
Damit der Watchdog oder andere Programmteile eine eMail senden können, kann eine E-Mail Option installiert werden.
Im Wiki ist die Anleitung: [E-Mail Tool](https://github.com/nischram/E3dcGui/wiki/EMail-Tool)

## Material
Ich nutze die Software auf einem Komplettpaket von Conrad. Das Set besteht aus dem Raspberry Pi 3, SD-Karte (Noobs vorinstalliert), 7-Zoll Raspberry Touchdisplay, Standgehäuse und Netzteil.  
Hier die Artikelnummer von Conrad: [1437995-62](https://www.conrad.de/de/raspberry-pi-3-model-b-starter-set-1-gb-noobs-inkl-betriebssystem-noobs-inkl-gehaeuse-inkl-netzteil-inkl-software-1437995.html) oder bei Amazon: [Raspberry 7 Zoll Touch Display](http://www.amazon.de/dp/B014WKCFR4/)  
Die Grundinstallation ist einfach, da es im Internet schon viele Anleitungen hierzu gibt, gehe ich im Moment hierauf nicht näher ein.

### W-Lan einrichten
Einige Raspberry haben W-Lan on Board, hier eine anleitung zur Einrichtung: [W-LAN einrichten](https://github.com/nischram/E3dcGui/wiki/W-LAN-einrichten)

## Quellen

### RSCP
Downloadbereich im E3DC Kundenportal [https://s10.e3dc.com](https://s10.e3dc.com) > "Technische Anleitungen zu Zusatzoptionen der S10 Hauskraftwerke" > "RSCP-Beispielapplikation (Anleitung für Entwickler)"
[Link](https://s10.e3dc.com/s10/module/download/get.php?id=280) (Benutzerdaten erforderlich)

### Vergleichbare Projekte
* HistoryValues vom S10 abfragen von RalfJL: [Link](https://github.com/RalfJL/S10history)
* Sperrzeiten setzen von rella: [Link](https://github.com/rellla/E3DC-Rscp)

### Forum für diskussionen
Homematic-Forum: [Anbindung an Stromspeicher E3DC](https://homematic-forum.de/forum/viewtopic.php?f=19&t=30095)

### Grafiken
Bildschirmfotos aus dem E3DC Portal

## Changelog
### Wichtige Ergänzungen
V1.87 29.03.2021 Wallbox anzeigen o. steuern  
V1.81 08.09.2020 WetterGui auf OpenWeatherMap.org umgestellt  
V1.68 10.12.2017 Grünbeck softliQ SC18 eingebunden  
V1.61 11.10.2017 LED-Statusanzeige integriert  
V1.52 23.09.2017 Hausautomation integriert  
V1.49 05.09.2017 Abfuhrkalender eingebaut  
V1.47 03.09.2017 WetterGui eingebaut  

### Version Abfragen
Mit folgendem Befehl kann man direkt die Version ohne Display abfragen:  
`grep "Stand: " README.markdown |cut -d " " -f 2`

### Versionen
V1.89 03.05.2021 Wallbox Daten an HM senden und auslesen
- Wallbox-Werte an die HM senden
- Wallbox-Parameter aus HM lesen und an WB senden

V1.88 03.04.2021 Anleitung überarbeitet  

V1.87 29.03.2021 Wallbox anzeigen o. steuern / Fehlerbehebung
- Wallbox-Menü zur Anzeige und Steuerung
- Fehlerkorrektur im Setup-Menü
- kleine Fehlerkorrekturen
- Changelog archiviert  

[Changelog Archiv](https://github.com/nischram/E3dcGui/tree/master/Changelog_Archiv)
