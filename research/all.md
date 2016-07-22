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
