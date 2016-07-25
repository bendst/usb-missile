# usb-missile

## udev Regeln
Zum Verwenden des Treibers sind folgende Schritte notwenig:

1. Kopiere "10-missile.rules" nach "/etc/udev/rules.d/"
2. Führe "udevadm trigger" im Terminal aus
3. Schließe den USB-Raketenwerfer an
4. Führe "sudo insmod usb-missile.ko" im Terminal aus 

## Bugs/Probleme: 
[ ] Nach Aus- und Einstöpseln des Raketenwerfer muss das Treiber-Modul neu geladen werden:

1. "sudo rmmod usb-missile"
2. "sudo insmod usb-missile.ko"

[ ] Die Zugriffsrechte auf das Gerät erfordern Root-Rechte. Lässt sich eventuell über udev lösen.

([ ] Abschuss lässt sich unterbrechen, was beim nächsten Abschuss zu merkwürdigem Verhalten führen kann. Eventuell Verantwortlichkeit beim Frontend belassen...)
