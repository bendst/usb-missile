# usb-missile

## Treiber
### Hinweise zum Einrichten
Zum Verwenden des Treibers sind folgende Schritte notwenig:

1. Kopiere "10-missile.rules" nach "/etc/udev/rules.d/"
2. Schließe den USB-Raketenwerfer an
3. Führe "sudo insmod usb-missile.ko" im Terminal aus 

## Frontent
### Hinweis zum Kompilieren
Das Paket "libudev-dev" muss installiert sein.

## Bugs/Probleme: 
[ ] Nach Aus- und Einstöpseln des Raketenwerfer muss das Treiber-Modul neu geladen werden:

1. "sudo rmmod usb-missile"
2. "sudo insmod usb-missile.ko"

[ ] Die Zugriffsrechte auf das Gerät erfordern Root-Rechte. Lässt sich eventuell über udev lösen.
    Neuere Version des Linux Kernel lassen keine höhere Berechtigung wie Usr  zu. 
    Sollte eine allgemeinere Berechtigung eingestellt werden, schlägt die Kompilation fehl.

([ ] Abschuss lässt sich unterbrechen, was beim nächsten Abschuss zu merkwürdigem Verhalten führen kann. Eventuell Verantwortlichkeit beim Frontend belassen...)
