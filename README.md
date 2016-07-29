# Usb-Missile Anleitung

Vorrausetzung: Das Paket "libudev-dev" muss zum Kompilieren von "main.c" installiert sein.

Hilfeoptionen lassen sich mit
```
make help
```
anzeigen.


# Automatische Kompilation und Installation

Die (De)Installtion kann automatisiert werden mit 
```
make install
```
und
```
make remove
```


# Manuelle Kompilation und Installation

Führe zum Kompilieren 
```
make all
```
im Ordner usb-missile aus.


## udev Regeln

Zum Verwenden des Treibers sind folgende Schritte notwenig:

1. Kopiere "10-missile.rules" nach "/etc/udev/rules.d/"

    ```
    sudo cp 10-missile.rules /etc/udev/rules.d/
    ```
2. Schließe den USB-Raketenwerfer an.
3. Lade das Module.
   
   ```
    sudo insmod drivers/usb-missile.ko
    ```

Das Modul kann wieder entladen werden mit:
```
sudo rmmode usb_missile
```

## Bugs/Probleme: 
[ ] Nach Aus- und Einstöpseln des Raketenwerfer muss das Treiber-Modul neu geladen werden:

    1. sudo rmmod usb_missile
    2. sudo insmod drivers/usb-missile.ko

[ ] Die Zugriffsrechte auf das Gerät erfordern Root-Rechte. Lässt sich eventuell über udev lösen.
    Neuere Version des Linux Kernel lassen keine höhere Berechtigung wie Usr  zu. 
    Sollte eine allgemeinere Berechtigung eingestellt werden, schlägt die Kompilation fehl.

[ ] Memory Leak im Frontend. 2 fehlende frees ausgehend von "udev_enumerate_add_match_subsystem(enumerate, "usb");" und "udev_enumerate_scan_devices(enumerate);".