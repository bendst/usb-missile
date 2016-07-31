help:
	@echo "Folgende Optionen sind unterstützt:"
	@echo "all          Kompiliere den Treiber und das Frontend     "
	@echo "clean        Entferne alles bis auf die .ko Datei        "
	@echo "clobber      Entferne ALLES bis auf die Source Dateien   "
	@echo "install      Kompiliere und installiere das Modul        "
	@echo "remove       Entferne das geladene Modul                 "
	@echo "start        Starte das Frontend"

all:
	$(MAKE) -C driver all
	$(MAKE) -C frontend all

install: all
	@echo "Udev Regel wird kopiert ..."
	@sudo cp 10-missile.rules /etc/udev/rules.d/
	@echo "Bitte schließe das Geräte an und drücke danach ENTER ..."
	@read input
	@sudo insmod driver/usb-missile.ko

remove:
	@echo "Entferne udev Regel ..."
	@sudo rm -f /etc/udev/rules.d/10-missile.rules
	@echo "Entferne Modul ..."
	@sudo rmmod usb_misssile	

start:
	@echo "Starte Frontend ..."
	@./frontend/./main

clobber:
	$(MAKE) -C driver clobber
	$(MAKE) -C frontend  clean

clean:
	$(MAKE) -C driver clean
	$(MAKE) -C frontend clean
