## Noch im Aufbau, nur von mir getestet!

# E3DC to HomeMatic mit GUI

Hier beschreibe ich, wie du dein S10 Hauskraftwerk von E3DC an eine HomeMatic Hausautomation von eQ-3 anbinden kannst.

Als Schnittstelle zwischen S10 und HomeMatic dient ein Raspberry Pi. Die Applikation, kann auf dem Raspberry PI vielseitig eingesetzt werden.

__Daten per RSCP vom E3DC-S10 Speicher abfragen__   
__Daten vom E3DC zur HomeMatic Hausautomation senden__   
__Daten vom S10 auf dem Display darstellen__   
__Daten von der HomeMatic auf dem Display darstellen__

Die verschiedenen Möglichkeiten können in unterschiedlichen Konstellationen zusammen genutzt werden.
Es kann entweder ein Raspberry Pi __ohne Display__ eingesetzt werden, um die Daten vom S10 zu holen und/oder zur HomeMatic senden.
Oder es wird ein Raspberry Pi mit __7“ Display__ eingesetzt um diverse Daten darzustellen.  

<img src="https://s20.postimg.org/fidc9dsm5/E3_DC_Display.jpg" alt="E3DC-Display">  
Hier ein Foto mit der Software auf dem 7-Zoll Display.

Diese Kombination bietet eine gute Möglichkeit um das S10 einfach und schnell zu überwachen.

Ich nutze diese Applikation auf einem Raspberry Pi 3. Mit dem 7“ Raspberry Touchdisplay. Für das Material und die Installation ist unten ein Kapitel __Material__. Meine HomeMatic hat die Firmware 2.19.9 (zuvor auch 2.17.15 getestet) installiert. Mein S10-E hat Softwareversion 2016-02.

In der Applikation werden die Daten vom S10 mit einer RSCP-Applikation geholt. Diese Applikation bietet E3DC in Downloadbereich an. Ich habe ein paar Änderungen vorgenommen. Da ich mich erst seit kurzen mit der Programmierung beschäftige, sind die Änderungen eventuell noch etwas unprofessionell, aber bislang erfüllen sie ihren Zweck.

Die abgefragten Werte vom S10 wie z.B. Leistungswerte oder Batteriefüllstand werden per CURL-Befehl an die XML-API Schnittstelle der HomeMatic gesendet.

Wenn ein Display genutzt wird, kommen C Programme zum Einsatz, um die diversen Grafiken zu erstellen.

## Vorbereitung am S10
Hauptmenü > Einstellungen
Am S10 muss ein RSCP-Passwort vergeben werden. Im „Hauptmenü“ unter „Einstellungen“ kann ein RSCP-Passwort gesetzt werden. Das gesetzte Passwort wird später in der Parametern der Software benötigt.
Im selben Menüfenster ist weiter oben auch die IP-Adresse des S10 zu finden.

## Raspberry Pi
Ich erkläre hier nicht die Erstinstallation eines Raspberry Pi beschrieben. Diese Anleitung setzt einen lokalen Zugriff oder SSH-Zugriff auf den Raspberry voraus.

### Netzwerk
Es ist Wichtig darauf zu achten, dass der Raspberry Pi im selben Netzwerk wie das S10 installiert ist. Der IP-Adressbereich muss gleich sein, zum Beispiel so: 192.168.178.xxx

Auch darf die Verbindung zwischen S10 und Raspberry Pi nicht über eine VPN-Verbindung hergestellt werden.

### Applikation auf den Raspberry kopieren
Jetzt wird der lokale oder der SSH-Zugang zum Raspberry benötigt. Ich mache dies per SSH mit dem „Terminal“ für MAC.
Jetzt folgt der Download vom Github:
```shell
git clone git://github.com/nischram/E3dcGui.git ~/E3dcGui
```

### Einstellungen vornehmen

In der Datei „parameter.h“ im Ordner E3dcGui kann nun alles eingestellt werden, was du zum nutzen der Software benötigst. Entweder öffnet man die Datei mit einem externen Editor. Hier können diverse Editoren zum Einsatz kommen, ich nutze auf meinem Mac „Atom“ und lade die Datei runter mit „Cyberduck“, unter Windows ist das runterladen und bearbeiten mit z.B. WinSCP möglich. Alternativ kann man die Datei direkt auf dem Raspberry bearbeiten, dies beschreibe ich jetzt.
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

Da ich in der Datei eigentlich alles Beschrieben habe, gehe ich jetzt nur auf das wichtigste ein.
Dies sind die Einstellungen:
```shell
6    #define GUI                         1
10   #define E3DC_S10                    1
19   #define Homematic_E3DC              1
108  #define Homematic_GUI               0
```
Hier wird die Nutzung der Applikation definiert, also ob du das Display nutzen willst oder nicht und ob die eine HomeMatic anbinden willst oder nicht. Wenn du eine Funktion nutzen willst trag bitte eine „1“ ein sonst eine „0“.

### Änderungen speichern

Die Änderungen in der „parameter.h“ speicherst du mit „STRG“ und „O“ und beendet wird der Editor mit „STRG“ und „X“.

### Applikation Kompilieren

Das „Makefile“ ist komplett vorbereitet du brauchst nur noch „make“ in der Kommandozeile eingeben, dann läuft das Kompilieren von alleine durch.

### Wichtig
Damit die SD-Karte des Raspberry Pi nicht übermäßig beansprucht wird, nutzt ich ein RAMDisk im Arbeitsspeicher. Die Einrichtung ist unten im Kapitel __RAMDisk__ beschrieben.

## Vorbereitung der HomeMatic CCU /CCU2

Wenn du in den Einstellungen die Nutzung der HomeMatic aktiviert hast, muss die HomeMatic nun vorbereitet werden. Sollte die Nutzung aktiviert sein und die HM nicht vorbereitet dann würde die Applikation für jeden Sendebefehl mehrere Sekunden benötigen und nicht ordnungsgemäss laufen.

### Systemvariablen einrichten
In der WebUI der HomeMatic Zentrale müssen diverse Systemvariablen eingerichtet werden.
Benötigt werden folgende Systemvariablen:

<img src="http://s20.postimg.org/6enhgxc9p/TAB_001.jpg" alt="TAB_001">

<img src="http://s20.postimg.org/jcd18h9ul/HM_IMG_001.png" alt="HM_IMG_001">

Weil die HomeMatic in den Programmen keine negativen Werte auswerten kann, werden die Variablen „NET_IN“ und „NET_OUT“ benötigt. Weiter ist es zum Beispiel bei einem Anzeige-Display sinnvoller, mit einem einzelnen Parameter zu arbeiten. Hierfür wird die Variable „GRID“ benötigt.
Die Variable für die externe Quelle „ADD“ wird natürlich nur benötigt wenn ein Leistungsmesser für die externe Quelle eingesetzt ist.
Auch die Variable für die Wallbox „WB_ALL“ wird natürlich nur bei verwendeter Wallbox benötigt.
Die Variable „Time“ nutzte ich, um die Aktualität der letzten Übertragung zu prüfen.
Die Namen der Systemvariablen müssen nicht eindeutig sein, es können auch eigene Namen vergeben werden.
Die Systemvariablen werden vom Raspberry Pi über die eindeutige Variable „ise_id“ angesteuert oder geändert. Auch nach einer Namensänderung der Systemvariablen bleiben die „ise_id“ immer identisch.
Es ist wichtig, dass alle Variable mit dem Variablentyp „Zahl“ erstellt werden. Alle Variablen benötigen die Standardvorgabe mit einem Maximalwert von „65000“. Nur die Variable „Time“ benötigt einen Maximalwert von „2147483647“

Zusätzlich sind noch Variablen wie z.B. Autarky oder Eigenstrom möglich.

### XML-API einrichten
Damit der Raspberry Pi die Werte des S10 zur HomeMatic senden kann, benötigt die HomeMatic die Zusatz-Software „XML-API“.
Alle technischen Informationen entnehmen Sie bitte der Anleitung des Anbieters.
Den Download und eine Installationsanleitung finden Sie unter dem folgenden Link:
[http://www.homematic-inside.de/software/xml-api]( http://www.homematic-inside.de/software/xml-api)
Ich gehe hier nicht näher auf die Installation der XML-API ein.

### „ise_id“ auslesen
Damit über die „XML-API“ die Werte in die HomeMatic übertragen werden können, muss die „ise_id“ der zuvor erstellten Systemvariablen abgefragt werden. Die Abfrage kann z. B. über den Webbrowser mit diesem Befehl erfolgen:
`http://IP-HomeMatic/config/xmlapi/sysvarlist.cgi `
„IP-HomeMatic“ steht für die IP-Adresse der HomeMatic. Ich setzen voraus, dass dir die tatsächliche IP-Adresse bekannt ist. I.d.R. steht sie in der Adresszeile des Webbrowsers, wenn man mit der WebUi auf der HomeMatic eingeloggt ist.
In der Browserausgabe auf den zuvor eingegebenen Befehl sind alle konfigurierten Systemvariablen alphabetisch sortiert zu finden. Die „ise-id“ zu den zuvor konfigurierten Variablen werden in der „parameter.h“ benötigt


### Applikation starten
Jetzt kann die Applikation für Tests gestartet werden.
```shell
pi@raspberrypi ~/E3dcGui $  ./start
```
Je nach Einstellungen startet nun nur die RSCP-Applikation oder mehrere Programme starten und das Display sollte die gewünschten Daten anzeigen.

### Systemvariable prüfen
In der HomeMatic WebUi kannst du nun im Bereich „Status und Bedienung“ die Systemvariable einsehen. Hier sollten nun Zahlen mit einem aktuellen Zeitstempel stehen. Diese Werte aktualisieren sich im Intervallrhythmus.

<img src="http://s20.postimg.org/wj2h805jx/HM_IMG_002.png" alt="HM_IMG_002">

### Autostart der Applikation (durch Crontab-Job)
Damit das Programm und die Variablen nun dauerhaft genutzt werden können, muss die Applikation auf dem Raspberry in den Autostart gebracht werden. Dies geschieht bei einem Raspberry Pi in dem ein Crontab-Job erstellt wird.
Zuvor muss die Applikation gestoppt werden, hierfür kann man auch während die Applikation läuft das Stop-Programm ausführen. Eingabe: `./stop`

Die Crontab ruft man auf mit:
```shell
pi@raspberrypi ~/e3dc-rscp $  crontab -e
```
- Crontab für die Bearbeitung öffnen

In der geöffneten Crontab wird eine neue Zeile mit diesem Inhalt eingefügt:
```shell
@reboot /home/pi/E3dcGui/E3dcGui
```
Die Änderung wird mit „STRG“ + „O“ gespeichert und die Crontab beendet mit
„STRG“ + „X“.
Weitere Informationen zur Crontab entnehmen Sie bitte aus Quellen wie z. B. dieser:
[https://raspberry.tips/raspberrypi-einsteiger/cronjob-auf-dem-raspberry-pi-einrichten](https://raspberry.tips/raspberrypi-einsteiger/cronjob-auf-dem-raspberry-pi-einrichten)

### Raspberry Pi neu starten
Damit die Applikation gestartet wird, kann nun der Raspberry neu gestartet werden mit:
```shell
pi@raspberrypi ~/e3dc-rscp $  sudo reboot
```
- Reboot Befehl mit Administrator-Rechten
Der Raspberry Pi startet neu und die Applikation wird im Hintergrund ohne Bildschirmausgaben ausgeführt. Nach ca. 1 Minute werden die Werte der Systemvariablen der HomeMatic wieder aktualisiert. Oder das Display zeigt die Applikation

## Display bedienen
Die Darstellung ist in 5 Bildschirmen unterteilt.
#### 1. Setup
<img src="https://s20.postimg.org/nwxkb7j19/Setup.jpg" alt="Setup">  
Hier kann die Software oder der PI neu gestartet werden.
#### 2. Aktuelle Werte des S10
`  #define E3DC_S10                    1 `   
<img src="https://s20.postimg.org/avbvlcun1/Aktuelle_Werte.jpg" alt="Aktuelle_Werte">  
Die Aktuellen Werte des S10 werden im Sekundenrhytmus aktualisiert (`SleepTime   1`).
Wenn eine externe Quelle (Additional) oder die Wallbox aktiviert sind, werden auch für diese Daten ein Symbol angezeigt.
#### 3. Langzeit Werte des S10
`  #define E3DC_S10                    1`    
<img src="https://s20.postimg.org/xgbuievx9/Langzeit_Werte.jpg" alt="Langzeit_Werte">  
Diese Werte werden von der RSCP-Applikation mit ein 15 Minuten Mittelwert gespeichert.
Die verschiedenen Kurven lassen sich durch einen Tipp auf das Symbol in der Legende ein oder ausblenden. Leider reagiert das Display mit der Software nicht empfindlich genug, somit muss eventuell häufiger gedrückt werden um eine Kurve auszublenden. Für Additional gibt es eine Kurve, für die Wallbox habe ich nichts eingebaut.
Damit die verschiedenen PV-Anlagengrößen auch dargestellt werden können, muss die Maximalleistung in der "parameter.h" mit PowerMax definiert werden. Für Große Anlagen ist diese Grafik nicht geeignet. Die Langzeitwerte sind für 24 Stunden und werden durchlaufend dargestellt. Der 0:00 Uhr Punkt verschiebt sich und wird durch eine Linie gekennzeichnet.
#### 4. Monitor
<img src="https://s20.postimg.org/f752o3qxp/Monitor.jpg" alt="Monitor">  
Hier werden Informationen zum Raspberry Pi ausgegeben.
#### 5. HomeMatic
`  #define Homematic_GUI               1`   
<img src="https://s20.postimg.org/h0m810x7h/Homematic.jpg" alt="HomeMatic">  
Da es für die HomeMatic kein ideales Display gibt, habe ich diese Software genutzt um mir wichtige Daten der HomeMatic darzustellen. Die Nutzung für euch mit dieser Funktion ist nur mit aufwand möglich. Es müssen nicht nur die ISE_ID der Geräte oder Variablen definiert werden, sondern muss ggf. auch im Sourcecode einiges geändert werden.
Es werden verschiedene Etagen, Räume und Geräte angezeigt. Die Werte werden erst abgefragt, und danach die Grafik erstellt (am roten Punkt unten rechts zu erkennen).
Die Etagen haben eine rote oder grüne Umrandung für den Verschluss aller Fenster und Türen.
Die Räume haben unterschiedliche Fenstereinteilungen es gibt zum Teil Räume mit zwei Fenster und Jalousien (im Foto an OG Wohnz. zu sehen).
Die Geräte und Variablen in rechten Bereich sind sogar bedienbar. Also ein Tipp auf's Symbol sendet einen einen Wert zur HomeMatic ( z.B. der Brunnen kann per Display eingeschaltet werden).
Für HM CPU, Sonnenaufgang, Sonnenuntergang, Vollmond usw. habe ich in der HomeMatic Systemvariablen angelegt die ich abfrage.
Ich biete den Teil der Software hier gerne an, aber da die auf meine HomeMatic und meine Gegebenheiten eingerichtet ist, könnt ihr diesen Teil ggf. nicht nutzen. Ich empfehle erstmal diesen Teil nicht zu aktivieren, also `#define Homematic_GUI          0` eintragen.
Das senden der Daten mit der RSCP-Applikation, ist hiervon nicht betroffen (`#define Homematic_E3DC       1`).

### Bildschirmschoner
Damit des Display nicht permanent hell ist, habe ich eine Bildschirmschoner-Funktion eingefügt. Die Zeit für den Bildschirmschoner ist in der „parameter.h“ auf Zeile 54 definiert: `#define ScreenSaverTime             180`
Um den Bildschirmschoner zu beenden reicht ein Tipp auf das Display (zum Teil wegen der Empfindlichkeit auch zwei Tipp's), dann wird es wieder hell. Für diesen Tipp ist nicht das ganze Display vorgesehen, da die anderen Funktionen versehentlich ausgelöst werden könnten. Der  Bereich ist hier zu sehen:   
<img src="https://s20.postimg.org/f1apu9lp9/Bildschirmschoner.jpg" alt="Bildschirmschoner">   
Im HomeMatic Menü ist der Bereich noch kleiner. Hier zu sehen:   
<img src="https://s20.postimg.org/lg9qqxsf1/Bildschirmschoner_HM.jpg" alt="Bildschirmschoner_HM">   

## Werte in der HomeMatic nutzen
Nun können diverse Programme, Funktionen und Anzeigen mit den Werten in der HomeMatic realisiert werden. In dieser Anleitung gehen wir nur auf Möglichkeiten ein, die mit vorhandenen Mitteln getestet wurden und im täglichen Praxiseinsatz sind. Die folgenden Vorschläge sind als Hilfestellung gedacht und können jederzeit abgeändert werden.

###	HM Display nutzen
Im Ordner „HM-Scripte“ befindet sich ein Script, mit dem das Display „HM-Dis-WM55“ angesteuert werden kann. Auf das Anlernen des Displays wird hier nicht näher eingegangen. Damit das Display genutzt werden kann, muss im Bereich „Programme und Verknüpfungen“ > „Programme“ ein neues Programm angelegt werden.

<img src="http://s20.postimg.org/8w1ijw07x/HM_IMG_003.png" alt="HM_IMG_003">

Als „Bedingung: „Wenn...“ wird als Gerät das Display mit „Tastendruck kurz“ eingebunden. Die Bezeichnung des Display ist im obigen Bild „EG Display“ mit „:1“ wird der Tastendruck unten ausgewählt.
Als „Aktivität: Dann...“ wird als „Script“ der Inhalt der im Ordner "HM-Scripte" befindlichen Datei „Display-S10.hm“ eingefügt. Es wird nicht die Datei benötigt. Die Auslösung muss für ein Display „sofort“ erfolgen.

__Wichtig:__
Wenn die Systemvariablen mit einem anderen Namen angelegt wurden, muss im Script die Definition der Variable angepasst werden. Im Folgenden ein Ausschnitt aus dem Script-Editor „Atom“ für Mac. In grüner Schriftfarbe werden die Namen der Systemvariablen dargestellt, die ggf. angepasst werden müssen:
```shell
var S10EPP = dom.GetObject("S10_EMS_POWER_PV").State();
var S10EPB = dom.GetObject("S10_EMS_POWER_BAT").State();
var S10EPG = dom.GetObject("S10_EMS_POWER_GRID").State();
var S10EPH = dom.GetObject("S10_EMS_POWER_HOME").State();
var S10SOC = dom.GetObject("S10_BAT_SOC").State();
time RSCPTime = dom.GetObject("S10_RSCP_Time").ToTime();
```
Hier ein Bild vom Display mit der Anzeige der S10-Werte. Die Werte werden je nach Bedingung in unterschiedlichen Farben dargestellt.

<img src="http://s20.postimg.org/pvyuh3tul/IMG_001.png" alt="IMG_001">

Durch einen kurzen Tastendruck an der Unterkante des Displays löst man das Programm aus, und die Werte des S10 werden angezeigt.

Fehleranfälligkeit in diesem Programm:
Derzeit besteht ein bei der HomeMatic-Software ein Problem mit der maximalen Anzahl von Variablen in Scripten. Hier im Forum diskutiert:
[http://homematic-forum.de/forum/viewtopic.php?f=26&t=27907&start=100](http://homematic-forum.de/forum/viewtopic.php?f=26&t=27907&start=100)
In dem oben verwendetem Scrip ist diese Variable z. B. „S10EPP“. Aus eigener Erfahrung kann es bei der HomeMatic zu Problemen führen, wenn in diversen Scripten die maximale Anzahl von ca. 200 Variablen überschritten wird. In dem angegebenen Forum werden Lösungsansätze besprochen. In eigenen Scripten werden seit Bekanntwerden dieses Problem nur noch allgemeine Variablen verwendet (siehe „Display-S10-ohneVariable.hm“).

### Spülmaschinen per Aktor steuern
Im Folgenden wird beschrieben, wie man per Aktor eine Spülmaschine leistungsabhängig einschaltet.

__Spülmaschine prüfen__

Bevor diese Funktion umgesetzt wird, ist zu klären, ob die Spülmaschine diese Möglichkeit unterstützt.
Man muss nach dem Starten der Spülmaschine den Stecker der Spülmaschine ziehen und kurz drauf wieder einstecken. Wenn die Spülmaschine anschließend das Programm an der gleichen Stelle weiter führt, kann die Steuerung der Spülmaschine per HomeMatic genutzt werden.

__Aktor einbinden__

Für diese Anwendung wird der Zwischensteck-Schaltaktor mit Leistungsmessung „HM-ES-PMSw1-Pl“ benötigt.
Dieser Aktor muss in der Zuleitung der Spülmaschine gesteckt und bei der HomeMatic angemeldet sein.
Der Anschluss und das Anlernen des Aktors wird hier nicht näher beschrieben.

__Programm anlegen__

Im Bereich „Programme und Verknüpfungen“ > „Programme“ wird ein neues Programm erstellt.
Dieses Programm überprüft die momentane Leistung der Spülmaschine. Im Standby benötigen viele Maschinen nur wenig Leistung (Watt). Die von mir verwendete Spülmaschine zieht beim Programmieren maximal 3 – 6 Watt. Wenn die Spülmaschine gestartet wird, erhöht sich die Leistungsaufnahme. Diesen Umstand nutze ich, um den Aktor auszuschalten. Nach dem Ausschalten wartet das Programm auf die Einschaltbedingung und schaltet den Aktor zu. Weiter wird die Bedienung des Aktor gesperrt, damit die Spülmaschine für die Programmlaufzeit nicht abgeschaltet wird. Die Sperre wird nach Ablauf der Zeit aufgehoben.
Im folgenden Bild ist das gesamte Programm zu sehen, hier mit einer Startbedingung auf den Überschuss mit 2500 Watt. Somit startet die Spülmaschine erst bei einer Überschussleistung von 2500 Watt, was oft erst nach vollständig geladenen Batterien eintritt.

<img src="http://s20.postimg.org/p8bk9mejh/HM_IMG_004.png" alt="HM_IMG_004">

Im nächsten Bild ist zu sehen, wie die Spülmaschine bei einer PV-Produktion von 2500 Watt mit definierten Bedingungen startet.
Meine PV-Anlage ist kleiner und ich erreiche somit selten Überschuss. Somit schlte ich die Spülmaschine schon bei einer PV Produktion von 2500 Watt ein. Wie zuvor beschrieben, kann ein HomeMatic-Programm keine negativen Werte auswerten. Somit kommt hier der Wert „NET-IN“ zum Einsatz.

<img src="http://s20.postimg.org/cms93v3fh/HM_IMG_005.png" alt="HM_IMG_005">

In beiden Beispielen ist eine zusätzliche Zeitbedingung (hier 15:00 Uhr) enthalten, damit die Spülmaschine am Starttag oder am Folgetag nach dem Start auch durchläuft.

Weiter habe ich über einen Taster (z. B. „HM-PB-6-WM55“) ein Sonderstart ohne ein Berücksichtigung der Leistungsbedingung eingabaut.

### Heizstab mit Programm ansteuern
Im Folgenden beschreibe ich ein Programm zum Ansteuern eines Heizstab. (Nur zur Präsentation bei mir nicht getestet)
Es können diverse Aktoren für diese Anwendung genutzt werden. Daher geben ich keine genauere Bezeichnung für einen Aktor an.
Es wird wieder ein Programm erstellt. Dieses Programm (Abbildung unten) wertet die Überschussleistung „NET-IN“ aus. Im Beispiel wird bei 510 Watt eingeschaltet und bei einem Bezug „NET-OUT“ von 10 Watt wieder ausgeschaltet.

<img src="http://s20.postimg.org/5kubhnztp/HM_IMG_006.png" alt="HM_IMG_006">

### Werte des S10 auf der Startseite der WebUI anzeigen
Im Beriech „Einstellungen“ > „Benutzerverwaltung“ > „Bearbeiten“ kann man über „Systemvariable hinzufügen“ entsprechend Systemvariablen zur Startseite hinzufügen.

<img src="http://s20.postimg.org/5yvnh9jx9/HM_IMG_007.png" alt="HM_IMG_007">

### Diverse Anwendungen für S10-Werte

__Zeitstempel umrechnen__

Ein Programm, um den Zeitstempel „S10_RSCP_Time“ von UnixTime auf Uhrzeit und Datum für eine neue Systemvariable umzurechnen, sieht wie folgt aus:

<img src="http://s20.postimg.org/l8viogffh/HM_IMG_008.png" alt="HM_IMG_008">

Der Inhalt des Scripts sieht wie folgendermaßen aus:
```shell
var v105 = dom.GetObject("S10_Zeitstempel");
time v106 = dom.GetObject("S10_RSCP_Time").ToTime();
string v107 = v106.ToTime();
string v108 = v107.Format("%d.%m.%Y %H:%M:%S");
v105.State(v108);
```

__Tagesmaximalwert der PV-Leistung als Variable anlegen__

Speichern der Maximalwerte von PV-Leistung in eine Systemvariable. Diese Variable muss als Variablentyp „Zahl“ angelegt werden.
Folgendes Programm ist nötig:

<img src="http://s20.postimg.org/4mijt4bvh/HM_IMG_009.png" alt="HM_IMG_009">

Das Script sieht so aus:
```shell
var source = dom.GetObject("$src$");
var v101 = dom.GetObject("S10_PV-MAX");
if (source)
{
if (source.Value() > v101.Variable())
{
v101.Variable(source.Value());
}
}
else
{
var v102 = dom.GetObject("S10_EMS_POWER_PV");
v101.Variable(v102.Value());
}
```
Damit der Wert „S10-PV-MAX“ täglich auf „0“ gesetzt wird, kommt ein weiteres Programm zum Einsatz. Dieses wird täglich ausgelöst, z. B. um 0:00 Uhr. Im Script muss folgendes stehen:
```shell
var v001= dom.GetObject("S10_PV-MAX");
v001.Variable("0");
```

__Minimal- und Maximal-Werte SOC__

Ich Speicher mir die Minimal- und Maximal-Werte des SOC (State Of Charge) auch.
Passend dazu ist das Skript „S10-min-maxSOC.hm“ (_Datei Fehlt noch_) ist im Ordner „HM-Scripte“ zu finden. Das Skript „DATA-PV.hm“ gehört dazu und ersetzt hierbei das Script aus der täglichen Auslösung von PV_Max. Es trägt die Werte in eine zusätzliche Systemvariable für „Yesterday“ ein und speichert alle Werte in einer Datei. Zum Speichern der Datei wird die „CUxD“ verwendet. In dieser Anleitung gehen ich aber auf diese Funktion nicht weiter ein.

## WatchDog

Teilweise bleibt die RSCP-Applikation hängen und die Automatische re-connection in der Applikation funktioniert leider nicht immer. So wird ein Neustart der RSCP-Applikation nötig.

Für dieses Problem habe ich einen einfachen WatchDog geschrieben. Damit der WatchDog den Betrieb der Applikation überwachen kann, lasse ich mit einem kleinen Teil in der RscpMain, eine Datei im RAMDisk erstellen. In der Datei ist die Unixtime des S10, diese wiederum liest der WatchDog ein und vergleicht diese mit einer definierten Differenz mit der aktuellen Zeit.  

Ein paar Einstellmöglichkeiten für den WatchDog hast du bestimmt schon in der „parameter.h“ gesehen. Wenn die Applikation auch Daten von der HomeMatic holt werden auch diese Daten überwacht. Da ich die HomeMatic nicht so häufig abfrage, sollten die Einstellungen zum WatchDog nicht großartig geändert werden.
Wenn der Watchdog zuschlägt, erstellt er eine Datei "Watchdog.csv" im E3dcGui Ordner. Somit ist eine Kontrolle der Aktivität möglich. Es wird je Aktivität eine Zeile erstellt, du kannst erkennen was der WatchDog neu gestartet hat.
Der WatchDog startet den Raspberry Pi auch neu, wenn die Applikation über längere Zeit keine aktuellen Daten (E3DC oder HomeMatic) liefert. Hierdurch ergibt sich noch ein Problem, sollte die Netzwerkverbindung zum System oder die Geräte gestört sein, würde der Raspberry Pi mehrfach mit einem Reboot neu gestartet. Um dies zu stoppen musst du folgendes in der Kommandozeile eingeben:
```shell
pi@raspberrypi:~ $ pkill watchdogHM
```

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

## Material
Ich nutze die Software auf einem Komplettpaket von Conrad. Das Set besteht aus dem Raspberry Pi 3, SD-Karte (Noobs vorinstalliert), 7-Zoll Raspberry Touchdisplay und Standgehäuse.
Hier die Artikelnummer von Conrad: [1437995-62](https://www.conrad.de/de/raspberry-pi-3-model-b-starter-set-1-gb-noobs-inkl-betriebssystem-noobs-inkl-gehaeuse-inkl-netzteil-inkl-software-1437995.html)
Die Grundinstallation ist einfach, da es im Internet schon viele Anleitungen hierzu gibt, gehe ich nicht näher drauf ein.

### Desktop deaktivieren
Es sollte die Desktopanwendung „startx“ deaktiviert werden, dies kannst du im Raspberry Konfigurator machen:
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
Bitte nicht vergessen, dass nur der Pi3 W-Lan on Board, bei einem älteren Pi musst du entweder per Kabel oder per W-Lan USB-Stick die Netzwerkverbindung herstellen.

## RSCP to Loxone

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

ToDo: Ich werde die Loxone noch komplett aufnehmen.

## Quelle

#### RSCP
Downloadbereich E3DC Kundenportal [https://s10.e3dc.com](https://s10.e3dc.com)
"Zusätzliche Optionen" > " RSCP-Beispielapplikation_2016-04-25.pdf"
[Link](https://s10.e3dc.com/s10/module/download/get.php?dl=3408) (Benutzerdaten erforderlich)
#### Grafiken
Bilschirmfotos aus dem E3DC Portal (Ich hoffe E3DC hat nichts dagegen!?)

## Changelog

14.08.2016 Repository neu erstellt
