## Touch Test

Wird beim "make" von E3dcGui mit kompiliert.
Aufgerufen wird es mit folgenden Befehlen:
```shell
cd Frame
./touchtest
```
Alle vier Zahlen in den Ecken der Reihe nach anklicken. Wenn der Touch funktioniert, werden die Ecken grün.
Wenn der Touch verstellt ist oder gar nicht funktioniert, sind im Eingabefenster die Tauchkoordinaten als Ausgabe zu sehen.
Eine korrekte Ausgab sollte etwa so aussehen:
```shell
Input device name: "FT5406 memory based driver"
Supported events:
  Event type 0 (Sync)
  Event type 1 (Key)
    Event code 330 (Touch)
  Event type 3 (Absolute)
    Event code 0 (X)
     Value    797
     Min        0
     Max      800
    Event code 1 (Y)
     Value     25
     Min        0
     Max      480
    Event code 47 (?)
     Value      0
     Min        0
     Max        9
    Event code 53 (Position X)
     Value      0
     Min        0
     Max      800
    Event code 54 (Position Y)
     Value      0
     Min        0
     Max      480
    Event code 57 (Tracking ID)
     Value      0
     Min        0
     Max    65535
Original 800x480, 16bpp
Framebuffer /dev/fb0 resolution;
800x480, 16 bpp

X Scale Factor = 1.000000
Y Scale Factor = 1.000000
scaledX = 1 / scaledY = 5
scaledX = 2 / scaledY = 7
scaledX = 3 / scaledY = 7
scaledX = 6 / scaledY = 10
Links Oben OK
scaledX = 5 / scaledY = 10
scaledX = 5 / scaledY = 10
scaledX = 773 / scaledY = 16
Rechts Oben OK
scaledX = 774 / scaledY = 16
scaledX = 772 / scaledY = 17
scaledX = 778 / scaledY = 18
scaledX = 799 / scaledY = 478
Rechts Unten OK
scaledX = 799 / scaledY = 478
scaledX = 799 / scaledY = 478
scaledX = 792 / scaledY = 478
scaledX = 793 / scaledY = 478
scaledX = 793 / scaledY = 478
Links Unten OK
scaledX = 9 / scaledY = 471
scaledX = 10 / scaledY = 470
scaledX = 8 / scaledY = 468
scaledX = 8 / scaledY = 468
scaledX = 385 / scaledY = 338
Display OK
```

09.11.2016 In die Reposity E3dcGui eingebunden
