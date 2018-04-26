## E3DC to HomeMatic mit GUI
`  #define Homematic_E3DC              1` Für das Verwenden der Daten des S10 in der HomeMatic  
`  #define Homematic_GUI               1` Für das Darstellen von Geräten und Variablen der HomeMatic  

Anleitung zur Anbindung deines S10 Hauskraftwerk von E3DC an deine HomeMatic Hausautomation von eQ-3.

### Softwareversion
Meine HomeMatic hat die Firmware 2.27.8 installiert.

### Schnittstelle
Die abgefragten Werte vom S10 wie z.B. Leistungswerte oder Batteriefüllstand werden per CURL-Befehl an die XML-API Schnittstelle der HomeMatic gesendet (Zusatz-Software)

## Vorbereitung der HomeMatic CCU /CCU2

Wenn du in den Einstellungen die Nutzung der HomeMatic aktiviert hast, muss die HomeMatic nun vorbereitet werden. Sollte die Nutzung aktiviert sein und die HM nicht vorbereitet dann würde die Applikation für jeden Sendebefehl mehrere Sekunden benötigen und nicht ordnungsgemäss laufen.

### Systemvariablen einrichten
In der WebUI der HomeMatic Zentrale müssen diverse Systemvariablen eingerichtet werden.
Benötigt werden folgende Systemvariablen:

<table>
  <thead>
    <tr>
      <th>Name</th>
      <th>Bezeichnung</th>
      <th>Variablentyp</th>
      <th>Maximalwert</th>
      <th>Einheit</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>S10_EMS_POWER_PV</td>
      <td>S10_PV-Produktion</td>
      <td>Zahl</td>
      <td>65000</td>
      <td>W</td>
    </tr>
    <tr>
      <td>S10_EMS_POWER_BAT</td>
      <td>S10_Batterieleistung</td>
      <td>Zahl</td>
      <td>65000</td>
      <td>W</td>
    </tr>
    <tr>
      <td>S10_EMS_POWER_HOME</td>
      <td>S10_Hausverbrauch</td>
      <td>Zahl</td>
      <td>65000</td>
      <td>W</td>
    </tr>
    <tr>
      <td>S10_EMS_POWER_GRID</td>
      <td>S10_Netzleistung</td>
      <td>Zahl</td>
      <td>65000</td>
      <td>W</td>
    </tr>
    <tr>
      <td>S10_EMS_POWER_NET_IN</td>
      <td>S10_Einspeisung</td>
      <td>Zahl</td>
      <td>65000</td>
      <td>W</td>
    </tr>
    <tr>
      <td>S10_EMS_POWER_NET_OUT</td>
      <td>S10_Bezug</td>
      <td>Zahl</td>
      <td>65000</td>
      <td>W</td>
    </tr>
    <tr>
      <td>S10_EMS_POWER_ADD</td>
      <td>S10_ext.Quelle (falls vorhanden)</td>
      <td>Zahl</td>
      <td>65000</td>
      <td>W</td>
    </tr>
    <tr>
      <td>S10_EMS_POWER_WB_ALL</td>
      <td>Wallbox (falls vorhanden)</td>
      <td>Zahl</td>
      <td>65000</td>
      <td>W</td>
    </tr>
    <tr>
    <td>S10_BAT_SOC</td>
    <td>S10_Batterie SOC</td>
    <td>Zahl</td>
    <td>65000</td>
    <td>%</td>
    </tr>
    <tr>
    <td>S10_RSCP_Time</td>
    <td>Zeitstempel (UnixTime)</td>
    <td>Zahl</td>
    <td>2147483647</td>
    <td> </td>
    </tr>
    <tr>
    <td>S10_Zeitstempel</td>
    <td>S10_Zeitstempel (UnixTime)</td>
    <td>Zeichenkette</td>
    <td> </td>
    <td> </td>
    </tr>
  </tbody>
</table>

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

## Werte in der HomeMatic nutzen
Nun können diverse Programme, Funktionen und Anzeigen mit den Werten in der HomeMatic realisiert werden. In dieser Anleitung gehen wir nur auf Möglichkeiten ein, die mit vorhandenen Mitteln getestet wurden und im täglichen Praxiseinsatz sind. Die folgenden Vorschläge sind als Hilfestellung gedacht und können jederzeit abgeändert werden.

###	HM Display nutzen
Im Ordner „HM-Scripte“ befindet sich ein Script, mit dem das Display „HM-Dis-WM55“ angesteuert werden kann. Auf das Anlernen des Displays wird hier nicht näher eingegangen. Damit das Display genutzt werden kann, muss im Bereich „Programme und Verknüpfungen“ > „Programme“ ein neues Programm angelegt werden.

<img src="https://s20.postimg.cc/8w1ijw07x/HM_IMG_003.png" alt="HM_IMG_003">

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

<img src="https://s20.postimg.cc/pvyuh3tul/IMG_001.png" alt="IMG_001">

Durch einen kurzen Tastendruck an der Unterkante des Displays löst man das Programm aus, und die Werte des S10 werden angezeigt.

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
Im folgenden Bild ist das gesamte Programm zu sehen, hier mit einer Startbedingung auf den Überschuss mit 2500 Watt. Somit startet die Spülmaschine erst bei einer Überschussleistung von 2500 Watt, was oft erst nach vollständig geladenen Batterien eintritt. Wie zuvor beschrieben, kann ein HomeMatic-Programm keine negativen Werte auswerten. Somit kommt hier der Wert „NET-IN“ zum Einsatz.

<img src="https://s20.postimg.cc/p8bk9mejh/HM_IMG_004.png" alt="HM_IMG_004">

Im nächsten Bild ist zu sehen, wie die Spülmaschine bei einer PV-Produktion von 2500 Watt mit definierten Bedingungen startet.
Meine PV-Anlage ist kleiner und ich erreiche somit selten Überschuss. Somit schalte ich die Spülmaschine schon bei einer PV Produktion von 2500 Watt ein. Hierfür nehme ich den Wert "POWER_PV".

<img src="https://s20.postimg.cc/cms93v3fh/HM_IMG_005.png" alt="HM_IMG_005">

In beiden Beispielen ist eine zusätzliche Zeitbedingung (hier 15:00 Uhr) enthalten, damit die Spülmaschine am Starttag oder am Folgetag nach dem Start auch durchläuft.

Weiter habe ich über einen Taster (z. B. „HM-PB-6-WM55“) ein Sonderstart ohne ein Berücksichtigung der Leistungsbedingung eingabaut.

### Heizstab mit Programm ansteuern
Im Folgenden beschreibe ich ein Programm zum Ansteuern eines Heizstab. (Nur zur Präsentation bei mir nicht getestet)
Es können diverse Aktoren für diese Anwendung genutzt werden. Daher geben ich keine genauere Bezeichnung für einen Aktor an.
Es wird wieder ein Programm erstellt. Dieses Programm (Abbildung unten) wertet die Überschussleistung „NET-IN“ aus. Im Beispiel wird bei 510 Watt eingeschaltet und bei einem Bezug „NET-OUT“ von 10 Watt wieder ausgeschaltet.

<img src="https://s20.postimg.cc/5kubhnztp/HM_IMG_006.png" alt="HM_IMG_006">

### Werte des S10 auf der Startseite der WebUI anzeigen
Im Beriech „Einstellungen“ > „Benutzerverwaltung“ > „Bearbeiten“ kann man über „Systemvariable hinzufügen“ entsprechend Systemvariablen zur Startseite hinzufügen.

<img src="https://s20.postimg.cc/5yvnh9jx9/HM_IMG_007.png" alt="HM_IMG_007">

### Diverse Anwendungen für S10-Werte

__Zeitstempel umrechnen__

Hier ein Programm, um den Zeitstempel „S10_RSCP_Time“ von UnixTime auf Uhrzeit und Datum  für "S10_Zeitstempel" umzurechnen, sieht wie folgt aus:

<img src="https://s20.postimg.cc/l8viogffh/HM_IMG_008.png" alt="HM_IMG_008">

Der Inhalt des Scripts sieht wie folgendermaßen aus:
```shell
var S10Time = dom.GetObject("S10_Zeitstempel");
time S10Unix = dom.GetObject("S10_RSCP_Time").ToTime();
string V001 = S10Unix.ToTime();
string V002 = V001.Format("%d.%m.%Y %H:%M:%S");
S10Time.State(V002);
```

__Tagesmaximalwert der PV-Leistung als Variable anlegen__

Speichern der Maximalwerte von PV-Leistung in eine Systemvariable. Diese Variable muss als Variablentyp „Zahl“ angelegt werden.
Folgendes Programm ist nötig:

<img src="https://s20.postimg.cc/4mijt4bvh/HM_IMG_009.png" alt="HM_IMG_009">

Das Script sieht so aus:
```shell
var source = dom.GetObject("$src$");
var S10PVMAX = dom.GetObject("S10_PV-MAX");
if (source)
{
if (source.Value() > S10PVMAX.Variable())
{
S10PVMAX.Variable(source.Value());
}
}
else
{
var S10PowerPV = dom.GetObject("S10_EMS_POWER_PV");
S10PVMAX.Variable(S10PowerPV.Value());
}
```
Damit der Wert „S10-PV-MAX“ täglich auf „0“ gesetzt wird, kommt ein weiteres Programm zum Einsatz. Dieses wird täglich ausgelöst, z. B. um 0:00 Uhr. Im Script muss folgendes stehen:
```shell
var S10PVMAX= dom.GetObject("S10_PV-MAX");
S10PVMAX.Variable("0");
```

__Minimal- und Maximal-Werte SOC__

Ich Speicher mir die Minimal- und Maximal-Werte des SOC (State Of Charge) auch.
Passend dazu ist das Skript „S10-min-maxSOC.hm“ ist im Ordner „HM-Scripte“ zu finden. Das Skript „DATA-PV.hm“ gehört dazu und ersetzt hierbei das Script aus der täglichen Auslösung von PV_Max. Es trägt die Werte in eine zusätzliche Systemvariable für „Yesterday“ ein und speichert alle Werte in einer Datei. Zum Speichern der Datei wird die „CUxD“ verwendet. In dieser Anleitung gehen ich aber auf diese Funktion nicht weiter ein.

## Quelle

#### Scripte

Die Scripte für die HM habe ich mir zu Teil aus Informationen des Homematic-Forum zusammengebaut.

## Changelog

14.08.2016 Repository neu erstellt
