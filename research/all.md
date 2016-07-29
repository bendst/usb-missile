# Notizen zur Entwicklung

## Für das Frontend:
Zum generischen Auffinden der Devices folgenden Pfad verwenden:

/sys/bus/usb/drivers/usb_missile/1-3:1.0/

1-3 ist abhängig davon, an welchem Anschluss das Gerät eingestöpselt wurde. Der Link insgesamt ist allerdings von der Gestalt *:1.0.

# Was wissen wir über das Gerät?

Hardware-Device-IDs  
Vendor: 0x2123  
Product: 0x1010

Befehle für Bewegungen, Feuern und LED in Hex-Format (Anwendung in: frontends.md -> nmilford):  
Stop 0x02, 0x00  
Down 0x02, 0x01  
Up 0x02, 0x02  
Left  0x02, 0x04  
Right 0x02, 0x08  
Kobminationen aus Bewegung über Verundung des letzten Hexwertes. Z.B. Down-Left 0x02, 0x05  
Fire 0x02, 0x10  
LED off 0x03, 0x00  
LED on 0x03, 0x01

Enthält Informationen über den Aufbau von usb_control_msg
https://github.com/walac/pyusb/blob/9094c9b1ec2ac761dddce3c7d050fc4cd02e063d/usb/core.py#L997

# Udev
[Ausführliche Einführung in udev  und dessen Konfiguration von  opensuse](https://doc.opensuse.org/documentation/html/openSUSE_121/opensuse-reference/cha.udev.html)

[PDF von Free Electrons](http://free-electrons.com/doc/udev.pdf)
