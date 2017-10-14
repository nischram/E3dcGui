# E3DC to HomeMatic mit GUI
[Stand: V1.62 14.10.2017](https://github.com/nischram/E3dcGui#changelog)

Hier beschreibe ich, wie du dein S10 Hauskraftwerk von E3DC an eine HomeMatic Hausautomation von eQ-3 anbinden kannst.

Als Schnittstelle zwischen S10 und HomeMatic dient ein Raspberry Pi. Die Applikation, kann auf dem Raspberry PI vielseitig eingesetzt werden.

__Daten per RSCP vom E3DC-S10 Speicher abfragen__   
__Daten vom E3DC zur HomeMatic Hausautomation senden__   
__Daten vom S10 auf dem Display darstellen__   
__Daten von der HomeMatic auf dem Display darstellen__   
__Kleine Hausautomation mit GPIO's__   
__Wetterinformationen von "Yahoo Weather"__   
__Dein eigener Abfuhrkalender__   
__LED Stausanzeige__   

Die verschiedenen Möglichkeiten können in unterschiedlichen Konstellationen zusammen genutzt werden.   
Es kann entweder ein Raspberry Pi __ohne Display__ eingesetzt werden, der die Daten vom S10 abfragt und/oder zur HomeMatic senden.  
Oder es wird ein Raspberry Pi mit __7“ Display__ eingesetzt um diverse Daten darzustellen.  

<img src="https://s20.postimg.org/kxtyqs93x/E3_DC_Display.jpg" alt="E3DC-Display">  
Hier ein Foto mit der Software auf dem 7-Zoll Display. (noch ohne Wettersymbol)

Diese Kombination bietet eine sehr gute Möglichkeit um das S10 einfach und schnell mit einem eigenem Display zu überwachen.

Ich nutze diese Applikation auf einem Raspberry Pi 3. Mit dem 7“ Raspberry Touchdisplay. Für das Material und die Installation ist weiteres unten ein Kapitel __[Material](https://github.com/nischram/E3dcGui#material)__. Meine HomeMatic hat die Firmware 2.19.9 (zuvor auch 2.17.15 getestet) installiert. Mein S10-E hat Softwareversion 2017-02.

In der Applikation werden die Daten vom S10 mit einer RSCP-Applikation geholt. Diese Applikation bietet E3DC in Downloadbereich an. Ich habe ein paar Änderungen vorgenommen.

Die abgefragten Werte vom S10 wie z.B. Leistungswerte oder Batteriefüllstand werden auf Wunsch per CURL-Befehl an die XML-API Schnittstelle der HomeMatic gesendet.

Wenn ein 7" Display genutzt wird, kommen C Programme zum Einsatz, um die diversen Grafiken zu erstellen.

## Vorbereitung am S10    
Hauptmenü > Einstellungen
Am S10 muss im „Hauptmenü“ unter „Einstellungen“ > "Personalisieren" ein RSCP-Passwort gesetzt werden. Das gesetzte Passwort wird später in den Parametern der Software benötigt.
Im selben Menü „Einstellungen“ > "Netzwerk" ist die IP-Adresse des S10 zu finden.

## Raspberry Pi   
Die Erstinstallation eines Raspberry Pi erkläre ich hier nicht im Detail, nur mit einer Schritt für Schritt Zusammenfassung von diversen Links.    
[Schritt für Schritt](https://github.com/nischram/E3dcGui/blob/master/STEPBYSTEP.markdown)   
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
60   #define Homematic_GUI               0
```
Hier wird die Nutzung der Applikation definiert, also ob du das Display nutzen willst oder nicht und ob die eine HomeMatic anbinden willst oder nicht. Wenn du eine Funktion nutzen willst trag bitte eine „1“ ein sonst eine „0“.

### Änderungen speichern   
Die Änderungen in der „parameter.h“ speicherst du mit „STRG“ und „O“ und beendet wird der Editor mit „STRG“ und „X“.

### Applikation Kompilieren   
Das „Makefile“ ist komplett vorbereitet du brauchst nur noch „make“ in der Kommandozeile eingeben, dann läuft das Kompilieren von alleine durch.
```shell
pi@raspberrypi ~/E3dcGui $  make
```

### Wichtig   
Damit die SD-Karte des Raspberry Pi nicht übermäßig beansprucht wird, nutzt ich ein RAMDisk im Arbeitsspeicher. Die Einrichtung ist unten im Kapitel __[RAMDisk](https://github.com/nischram/E3dcGui#ramdisk)__ beschrieben.

### Vorbereitung der HomeMatic CCU /CCU2   
Wenn du in den Einstellungen die Nutzung der HomeMatic aktiviert hast (`#define Homematic_E3DC     1`), muss die HomeMatic nun vorbereitet werden.
Diesen Teil der Anleitung habe ich separat in dem Ordner Homematic erstellt.   
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
pi@raspberrypi ~/E3dcGui $  crontab -e
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
<img src="https://s20.postimg.org/c6bv32kpp/Setup_V1-52.jpg" alt="Setup">  
Hier kann die Software oder der PI neu gestartet werden.  
Auf der rechten Seite wird die Display-Helligkeit eingestellt. Auch kannst du hier den PIR-Sensor aktivieren oder deaktivieren werden.  
Unterhalb werden Informationen zum Raspberry Pi ausgegeben.   

#### 2. Wetteranzeige
<img src="https://s20.postimg.org/5ewglai59/Wetter.jpg" alt="Wetteranzeige">  
Hier werden Standort bezogene Wetterdaten eingeblendet. Die Daten basieren auf einen kostenlosen Service, welcher die aktuellen Wetterdaten von Yahoo! holt.   
[http://weather.tuxnet24.de](http://weather.tuxnet24.de)   
In der "parameter.h" muss die Standort-ID eingetragen werden, diese erhält man auf der Yahoo Seite ein dem die Location gesucht wird und man und in der Adresszeile aus dem Link die letzten Zahlen kopiert.   
`113 #define weatherID                   638242`   
Beispeil zur Adresszeile: "https://www.yahoo.com/news/weather/germany/berlin/berlin-638242"   
Die Wetteranzeigen könnte auch ohne S10 oder HomeMatic genutzt werden.

#### 3. Aktuelle Werte des S10
`  #define E3DC_S10                    1 `   
<img src="https://s20.postimg.org/7m0rhl63h/Aktuelle_Werte.jpg" alt="Aktuelle_Werte">  
Wenn die Aktuellen Werte des S10 im Display angezeigt werden, wird im Sekundenrhythmus aktualisiert. Sonst kann der Intervall definiert werden `#define SleepTime   1`
Wenn eine externe Quelle (Additional) oder die Wallbox aktiviert sind, wird auch für diese Daten je ein Symbol angezeigt.
Mit dem Symbol "History Today" können die HistoryValues für den Aktuellen Tag eingeblendet werden, ein weiteres Tippen jetzt auf "History Yesterday" blendet die Energiewerte für den Vortag ein und danach kann mit dem Tippen auf "History Off" die Einblendung abgeschaltet werden. Unter dem Symbol, mit dem man in die nächste Ansicht wechseln kann, wird der Zeitstempel der Daten angezeigt. In der parameter.h kannst du definieren ob du die Abfrage der HistoryValues aktivieren willst (historyAktiv) und in welchem Rhythmus die Daten abgefragt werden sollen (historyDelay).

#### 4. Langzeit Werte des S10
`  #define E3DC_S10                    1`    
<img src="https://s20.postimg.org/43orl757h/Langzeit_Werte.jpg" alt="Langzeit_Werte">  
Diese Werte werden von der RSCP-Applikation mit ein 15 Minuten Mittelwert gespeichert.
Die verschiedenen Kurven lassen sich durch einen Tipp auf das Symbol in der Legende ein oder ausblenden. Leider reagiert das Display mit der Software nicht empfindlich genug, somit muss eventuell häufiger gedrückt werden um eine Kurve auszublenden. Für Additional gibt es eine Kurve, für die Wallbox habe ich nichts eingebaut.
Damit die verschiedenen Größen der PV-Anlagen auch dargestellt werden können, muss die Maximalleistung in der „parameter.h" mit PowerMax definiert werden. Für Große Anlagen ist diese Grafik nicht geeignet. Die Langzeitwerte sind für 24 Stunden und werden durchlaufend dargestellt. Der 0:00 Uhr Punkt verschiebt sich und wird durch eine Linie gekennzeichnet.

#### 5. Monitor
`  #define E3DC_S10                    1`    
<img src="https://s20.postimg.org/d55f3bcnx/Monitor_Neu.jpg" alt="Monitor">  
Hier werden links die einzelnen Tracker des Wechselrichters dargestellt. Rechts ist für neue Ideen noch Platz, gerne darfst du einen Issue erstellen wenn du wünsche hast!

#### 6. SmartHome
<img src="https://s20.postimg.org/ukme71x0d/Smart_Home.jpg" alt="SmartHome">  
Du kannst am Raspberry den Standard Temperatur/Luftfeuchtigkeits-Sensor DHT11 anschließen. Ich habe in der parameter.h für fünf Sensoren die Einstellungen vorbereitet. Diese Fünf werden dann auf der linken Seite angezeigt. Der rote oder grüne Punkt zeigt den Status und die erfolgreiche Kommunikation zum Sensor. _Achtung:_ Sensor DHT22 ist nicht möglich!     
`  #define E3DC_S10                    1` Rechts ist der Status von Schaltaktoren zu sehen. Diese Aktoren können in der parameter.h definiert und den entsprechenden GPIO's zugeordnet werden. Mit den GPIO's ist es dann möglich zum Beispiel eine Relaisplatine anzusteuern. Mit der Platine kannst du dann ein Schütz in deiner Installation aktivieren und z.B. dein Heizstab ansteuern. Diese Funktion ist für alle die keine HomeMatic angebunden haben, aber trotzdem ein Gerät bei Überschuss aktivieren möchten. Getestet habe ich die Funktion mit einem "2 Kanal 5V Relais Modul für Arduino". Zur Auswahl der Aktoren stehen ein Überschussaktor, ein Aktor für Solarleistung, einer für den Batterie-SOC und ein Zeitaktor. Der Überschuss und der Solar-Aktor schalten ein wenn die Bedingung mindestens 2 Minuten überschritten wird, wenn die Leistung unter 90% vom Sollwert sinkt beginnt die Zeit neu. Abgeschaltet wird wenn die Bedingung 30 Sekunden unterschritten wird. Der Batterieaktor schaltet sofort sobald der Wert überschritten oder unterschritten wird. Auch hier darfst du weitere Ideen, Anregungen oder Fehler gerne als Issue erstellen. Die mindest Einschatzeit und die mindest Auschaltzeit, kann in Minuten definiert werden, dies ist z.B. für die Ansteuerung einer Spülmaschiene wichtig. Die Vergebene Priorität wird unter der Statuslampe angezeigt. Der Status ist hellrot wenn die Priorität erreicht ist sonst dunkelrot. Die Priorität muss in der parameter.h deklariert werden. Es kann von 1-5 gesetzt werden, bei "0" ist keine Priorität vergeben. Weiter ist es möglich den Aktoren ein Zeitfenster zu zuweisen. Nur in diesem Fenster schaltet der Aktor ein und zum Ende des Zeitfensters aus, ein gestarteter Aktor mit einer mindest Einschaltzeit läuft noch so lange, bis zum ablauf der Mindestzeit.

#### 7. HomeMatic
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
Damit du am Sourcecode eigene Änderungen vornehmen kannst, aber gleichzeitig Änderungen von mir in anderen Programmteilen übernehmen kannst habe ich die HM-Darstellung und die Touchfunktion in extra Dateien mit ausgelagert. Zusätzlich habe ich einige Kommentarzeilen und Infos als Bearbeitungshilfe eingefügt. Auch die Parameterdatei habe ich getrennt. Wenn du also die HM-Darstellung für deine Zwecke anpasst dann bitte die Dateien `HMGui.h`, `screenSaveHM.c` und `parameterHM.h` __nicht__ aktualisieren.

#### 8. Abfuhrkalender
`  #define Abfuhrkalender               1`   
<img src="https://s20.postimg.org/brr85ul3x/Muell.jpg˘" alt="Entsorgung">  
Hier kannst du dir deinen eigenen Entsorgungskalender einpflegen. Es werden dann die Aktuelle und die nächste Kalenderwoche angezeigt. Unter der Grafik ist eine Legende eingeblendet, die bei Bedarf auch deaktiviert werden kann. Für die Pflege musst du in dem Ordner "Data" die Datei "Entsorgung_2017.txt" anpassen. Wichtig ist, dass du das Format, die Aufteilung und Zeilenreihenfolge nicht veränderst. Für die Bearbeitung empfehle ich OpenOffice oder Excel, die Bearbeitung ist in einer Tabelle am besten. Es ist möglich bis zu zwei Tonnen an einem Tag einzupflegen. Es bestehen die Möglichkeit für Biomüll, Papier, Gelber Sack, Restmüll, Glas, Metall, Schadstoffmobil und Feiertag.
Für das nächste Jahr musst du eine neue Datei mit der Jahreszahl anlegen, dann wird diese zum Jahreswechsel automatisch verwendet.

### Bildschirmschoner
Damit das Display nicht permanent hell ist, habe ich eine Bildschirmschoner-Funktion eingefügt. Die Zeit für den Bildschirmschoner ist in der „parameter.h“ auf Zeile 73 definiert: `#define ScreenSaverTime             180`   
Um den Bildschirmschoner zu beenden reicht ein Tipp auf das Display, dann wird es wieder hell. Für diesen Tipp ist nicht das ganze Display vorgesehen, da die anderen Funktionen versehentlich ausgelöst werden könnten. Der Bereich ist hier zu sehen:   
<img src="https://s20.postimg.org/9vovscf19/Bildschirmschoner.jpg" alt="Bildschirmschoner">   
Im HomeMatic Menü ist der Bereich noch kleiner. Hier zu sehen:   
<img src="https://s20.postimg.org/6dcvvye59/Bildschirmschoner_HM.jpg" alt="Bildschirmschoner_HM">   
Das Display kann auch sofort Dunkel geschaltet werden, ein Tipp in den oben gezeigten Bereichen reicht hierfür.  

Es besteht die Möglichkeit den Bildschirmschoner mit Hilfe eines Bewegungssensors zu deaktivieren. Hierfür muss zum einen WiringPi installiert werden ([siehe unten](https://github.com/nischram/E3dcGui#wiringpi_einrichten)), der Pin des GPIO am Raspberry Pi muss ggf. angepasst werden (parameter.h "#define PirPin   4") und der Bewegungssensor (PIR) muss installiert werden.   
Hier ein Anschlussbild als Beispiel:   
<img src="https://s20.postimg.org/etiw6x7q5/pir-364x500.png" alt="fritzing PIRtoPI">   
Pinbelegung:   
VCC an Pin 2 (5V)   
OUT an Pin 16 (GPIO 23/GPIO_GEN 4)   
GND an Pin 6 (Ground)  

Ich habe den PIR-Sensor "HC-SR501" bei mir im gebrauch.   

Eine zufällige Möglichkeit besteht darin, einen Schaltaktor zu nutzen um den Bildschirmschoner auszuschalten. Einfach bei PIR und bei einem Aktor die selbe GPIO-Nr vergeben und schon wird das Display beim erreichen der Bedingung aktiviert. Wenn die Bedingung endet schaltet das Display über die Bildschirmschonerzeit auch wieder aus.   

#### Wichtig Pi-Bildschirmschoner deaktivieren
Der Raspberry Pi hat einen eignen Bildschirmschoner, dieser muss deaktiviert werden. Mit folgendem Befehl muss die Datei _rc.local_ geöffnet werden:
```shell
pi@raspberrypi:~ $ sudo nano /etc/rc.local
```
Vor dem "exit 0" musst du folgende Zeilen einfügen:
```shell
# turn off console blanking and cursor blinking
TERM=linux setterm -blank 0 -powerdown 0 -cursor off > /dev/tty1
```
Die Änderung wird mit „STRG“ + „O“ gespeichert und die Datei beendet mit „STRG“ + „X“.  
Der Teil "-cursor off" verhindert das Blinken des Coursers mitten im Bild.

## Werte in der HomeMatic nutzen
Das Nutzen der Werte in der HomeMatic, ist in der [Anleitung für HomeMatic](https://github.com/nischram/E3dcGui/tree/master/Homematic) in dem Ordner Homematic zu finden.

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

Version V1.61 ist die erste Version der Statusanzeige und könnte noch Fehler enthalten. Wenn euch welche auffallen, euch weitere Ideen oder Verbesserungen zu der LED-Statusanzeige einfallen, erstellt doch bitte einen Issue hier im Github.

## WatchDog

Teilweise bleibt die RSCP-Applikation hängen und die Automatische re-connection in der RSCP-Applikation funktioniert leider nicht immer. So wird ein Neustart der RSCP-Applikation nötig.

Für dieses Problem habe ich einen einfachen WatchDog geschrieben. Damit der WatchDog den Betrieb der Applikation überwachen kann, lasse ich mit einem kleinen Teil in der RscpMain, eine Datei im RAMDisk erstellen. In der Datei ist die Unixtime des S10, diese wiederum liest der WatchDog ein und vergleicht diese mit einer definierten Differenz mit der aktuellen Zeit.  

Ein paar Einstellmöglichkeiten für den WatchDog hast du bestimmt schon in der „parameter.h“ gesehen. Wenn die Applikation auch Daten von der HomeMatic holt werden auch diese Daten überwacht. Da ich die HomeMatic nicht so häufig abfrage, sollten die Einstellungen zum WatchDog nicht großartig geändert werden.   
Wenn der Watchdog zuschlägt, erstellt er eine Datei "Watchdog.csv" im E3dcGui Ordner. Somit ist eine Kontrolle der Aktivität möglich. Es wird je Aktivität eine Zeile erstellt, du kannst erkennen was der WatchDog neu gestartet hat.   
Der WatchDog startet den Raspberry Pi auch neu, wenn die Applikation über längere Zeit keine aktuellen Daten (E3DC oder HomeMatic) liefert. Hierdurch ergibt sich noch ein Problem, sollte die Netzwerkverbindung zum System oder die Geräte gestört sein, würde der Raspberry Pi mehrfach mit einem Reboot neu gestartet. Um dies zu stoppen musst du folgendes in der Kommandozeile eingeben:
```shell
pi@raspberrypi:~ $ killall watchdog
```
## Aktuelle Uhrzeit aus dem Internet holen
Wenn der Watchdog den Pi neu startet, bleibt die Uhrzeit des Pi nicht Aktuell. Hier können schon mal ein paar Minuten Abweichung entstehen.
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

## WiringPI einrichten

Für diverse Anwendungen der GPIO des Raspberry Pi muss die Software WiringPi installiert werden.
Es ist folgendes einzugeben.   
Zum home Verzeichnis wechslen:  
```
pi@raspberrypi ~/E3dcGui $ cd
```   
Nun brauchen wir git:   
```
pi@raspberry:~$ sudo apt-get install git git-core
```   
Jetzt kann WiringPi heruntergeladen   
```
pi@raspberry:~$ git clone git://git.drogon.net/wiringPi
pi@raspberry:~$ cd wiringPi
```
und installiert werden:   
```
pi@raspberrypi ~/wiringPi $ ./build
```

## eMail senden Installieren und aktivieren
Damit der Watchdog oder andere Programmteile eine eMail senden können, muss eine eMail Option installiert werden. Ich habe mich hier für "SendEmail" entschieden. Die Version 1.56 habe ich in meinem Github integriert, somit muss die Software nicht Heruntergeladen werden, was das installieren wesentlich einfacher macht. Folgende Befehle sind der Reihe nach auszuführen:
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
108  #define FromEmailAdress             "max.mustermann@web.de"
109  #define smtpServer                  "smtp.web.de"
110  #define smtpPort                    "587"
111  #define smtpTLS                     "yes"
112   #define smtpBenutzer                "max.mustermann@web.de"
113   #define smtpPass                    "1234abc"
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
95  #define WDtoEmailAdress             "max.mustermann@web.de"
```
Mit den Parametern
```
93   #define WDsendEmailReboot           1
94   #define WDsendEmailKill             0
```
kann noch definiert werden ob für Kill und/oder Reboot die eMail gesendet werden soll. Beide Parameter auf "0", dann wird keine eMail gesendet und die Software muss __nicht__ installiert werden.

__Wichtig:__ Bitte bei neueren Raspberry Versionen zur Fehlerbehebung [Issue#11](https://github.com/nischram/E3dcGui/issues/11) beachten!

## Material
Ich nutze die Software auf einem Komplettpaket von Conrad. Das Set besteht aus dem Raspberry Pi 3, SD-Karte (Noobs vorinstalliert), 7-Zoll Raspberry Touchdisplay, Standgehäuse und Netzteil.  
Hier die Artikelnummer von Conrad: [1437995-62](https://www.conrad.de/de/raspberry-pi-3-model-b-starter-set-1-gb-noobs-inkl-betriebssystem-noobs-inkl-gehaeuse-inkl-netzteil-inkl-software-1437995.html)   
Die Grundinstallation ist einfach, da es im Internet schon viele Anleitungen hierzu gibt, gehe ich im Moment hierauf nicht näher ein.

### Desktop deaktivieren
Es muss die Desktopanwendung „startx“ deaktiviert werden, dies kannst du im Raspberry Konfigurator machen:
```
pi@raspberry:~$ sudo raspi-config
```
Dort unter „Boot Options“ > "B1 Desktop / CLI" > „B2 Console Autologin Text console, automatically logged in as 'pi' user“ auswählen.

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

## RSCP to Loxone
Es besteht eine Möglichkeit meine Software auch für Loxone statt für Homematic zu nutzen. Dies ist in einer separaten Anleitung beschrieben.
[Loxone README](https://github.com/nischram/E3dcGui/blob/master/Loxone)

## Quelle

#### RSCP
Downloadbereich E3DC Kundenportal [https://s10.e3dc.com](https://s10.e3dc.com)
"Zusätzliche Optionen" > " RSCP-Beispielapplikation_2016-04-25.pdf"
[Link](https://s10.e3dc.com/s10/module/download/get.php?dl=3408) (Benutzerdaten erforderlich)
#### Grafiken
Bildschirmfotos aus dem E3DC Portal (Ich hoffe E3DC hat nichts dagegen!?)

## Changelog
#### Wichtige Ergänzungen
V1.61 11.10.2017 LED-Statusanzeige integriert   
V1.52 23.09.2017 Hausautomation integriert   
V1.49 05.09.2017 Abfuhrkalender eingebaut  
V1.47 03.09.2017 WetterGui eingebaut  

#### Versionen
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

[Changelog Archiv](https://github.com/nischram/E3dcGui/tree/master/Changelog_Archiv)
