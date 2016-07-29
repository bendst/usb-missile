/*
	TODO - Fehlerbehandlungen formalisieren

*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libudev.h>

#include <string.h>

#define PATH_TO_DRIVER "/sys/bus/usb/drivers/usb_missile"
#define ID_VENDOR "2123"
#define ID_PRODUCT "1010"

#define KEY_MV_UP_LEFT 'q'
#define KEY_MV_UP 'w'
#define KEY_MV_UP_RIGHT 'e'
#define KEY_MV_LEFT 'a'
#define KEY_MV_RIGHT 'd'
#define KEY_MV_DOWN_LEFT 'y'
#define KEY_MV_DOWN 's'
#define KEY_MV_DOWN_RIGHT 'c'
#define KEY_LED 'r'
#define KEY_FIRE 'f'
#define KEY_EXIT 'v'

int main(int argc, char *argv[])
{
	
	char c;
	int i;
	char *actionPath;
	
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	
	printf("Willkommen beim Steuerprogramm zum USB-Raketen-Werfer!\n\n");
	
	/* Überprüft, ob der Treiber geladen ist */
	struct stat s;
	int retval = stat(PATH_TO_DRIVER, &s);
	if (retval == -1) {
		printf("Fehler: Auf das Verzeichnis %s kann nicht zugegriffen werden.\nIst das Treibermodul geladen?\n", PATH_TO_DRIVER);
		return -1;
	}
	
	/* Sucht den passenden Anschluss für den Pfad heraus */
	udev = udev_new();
	if (!udev) {
		printf("Fehler: udev kann nicht erstellt werden.\n");
		exit(1);
	}
	// Suche das passende Gerät
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "usb");
	udev_enumerate_add_match_sysattr(enumerate, "idVendor", ID_VENDOR);
	udev_enumerate_add_match_sysattr(enumerate, "idProduct", ID_PRODUCT);
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);
	
	if (!devices) {
		printf("Fehler: Es wurde kein passendes Gerät gefunden.\n");
		udev_enumerate_unref(enumerate);
		udev_unref(udev);
		exit(1);
	}
	
	// Makro, expandiert zur Foreach-Schleife
	// Überprüft, ob nur ein Raketenwerfer angeschlossen wurde
	// Ermittelt den Pfad zu den sysfs-Dateien
	i = 0;
	udev_list_entry_foreach(dev_list_entry, devices) {
		if (i > 0) {
			printf("Fehler: Es wurden mehr als ein USB-Raketenwerfer gefunden. Bitte entfernen Sie alle überschüssigen Raketenwerfer bis nur noch einer angeschlossen ist.\n");
			udev_enumerate_unref(enumerate);
			udev_unref(udev);
			free(actionPath);
			exit(1);
		}
		
		int pathLen = 7; // initialer Wert enthält Länge von ":1.0", 1 "-" und 2 "/"
		int sysPathLen, busNumLen, devPathLen;
		const char *path;
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);
		
		// Bestimme die Längen der einzelnen Teile
		const char *sysPath = udev_device_get_syspath(dev);
		const char *busNum = udev_device_get_sysattr_value(dev, "busnum");
		const char *devPath = udev_device_get_sysattr_value(dev, "devpath");
		sysPathLen = strlen(sysPath);
		busNumLen = strlen(busNum);
		devPathLen = strlen(devPath);
		pathLen += sysPathLen + busNumLen + devPathLen;
		
		printf("pathLen: %d\n", pathLen); // TODO - DEBUG
		actionPath = malloc(pathLen * sizeof(char) + 1);
		if (!actionPath) {
			printf("Fehler: Out of memory.\n");
			udev_device_unref(dev);
			udev_enumerate_unref(enumerate);
			udev_unref(udev);
			exit(1);
		}
		
		strncpy(actionPath, sysPath, sysPathLen);
		/*
		strncat(actionPath, "/", 1);
		strncat(actionPath, busNum, busNumLen);
		strncat(actionPath, "-", 1);
		strncat(actionPath, devPath, devPathLen);
		strncat(actionPath, ":1.0/", 5);
		*/
		printf("actionPath: %s\n", actionPath); // TODO - DEBUG
		
		// TODO - Hier Pfad abgreifen
		printf("Experimental: %s\n", udev_device_get_syspath(dev));
		
		printf("  VID/PID: %s %s\n",
			udev_device_get_sysattr_value(dev, "idVendor"),
			udev_device_get_sysattr_value(dev, "idProduct"));
		printf("  Specific folder name: %s-%s:1.0\n",
			udev_device_get_sysattr_value(dev, "busnum"),
			udev_device_get_sysattr_value(dev, "devpath"));
		udev_device_unref(dev);
		i++;	
	}
	
	udev_enumerate_unref(enumerate);
	udev_unref(udev);
	
	// TODO - Sys-Devices öffnen
	
	printf("Steuerung:\n");
	printf("%c - Drehung nach oben-links\n", KEY_MV_UP_LEFT);
	printf("%c - Drehung nach oben\n", KEY_MV_UP);
	printf("%c - Drehung nach oben-rechts\n", KEY_MV_UP_RIGHT);
	printf("%c - Drehung links\n", KEY_MV_LEFT);
	printf("%c - Drehung rechts\n", KEY_MV_RIGHT);
	printf("%c - Drehung nach unten-links\n", KEY_MV_DOWN_LEFT);
	printf("%c - Drehung nach unten\n", KEY_MV_DOWN);
	printf("%c - Drehung nach unten-rechts\n", KEY_MV_DOWN_RIGHT);
	printf("%c - LED an/aus\n", KEY_LED);
	printf("%c - Feuer\n", KEY_FIRE);
	printf("%c - Beenden\n\n", KEY_EXIT);
	
	printf("Viel Spaß!\n\n");
	
	system("stty cbreak -echo");
	
	do {
		// TODO - DEBUG-START
		c = 'v';
		//c = getchar();
		// TODO - DEBUG-END
		switch (c) {
		// TODO - restliche Bewegungsrichtungen
		// TODO - Schreibzugriffe auf Sysfs-Devices
		case KEY_MV_UP:
			
			break;
		case KEY_MV_DOWN:
			
			break;
		case KEY_MV_LEFT:
			
			break;
		case KEY_MV_RIGHT:
			
			break;
		case KEY_LED:
			
			break;
		case KEY_FIRE:
			
			break;
		case KEY_EXIT:
			printf("Auf Wiedersehen!\n");
			break;
		default:
			break;
		}
	} while (c != KEY_EXIT);
	
	free(actionPath);
	
	system("stty cooked echo");
	
	return 0;
}
