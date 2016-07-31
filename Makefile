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
	@echo "Lade Modul ..."
	@sudo insmod driver/usb-missile.ko
	@echo "Bitte schließe das Geräte an ... Viel Spaß"
	@echo "Zum Starten \"make start\" eingeben"

remove:
	@echo "Entferne udev Regel ..."
	@sudo rm -f /etc/udev/rules.d/10-missile.rules
	@echo "Entlade Modul ..."
	@sudo rmmod usb_missile	

start:
	@echo "Starte Frontend ..."
	@./frontend/./main

clobber:
	$(MAKE) -C driver clobber
	$(MAKE) -C frontend  clean

clean:
	$(MAKE) -C driver clean
	$(MAKE) -C frontend clean
