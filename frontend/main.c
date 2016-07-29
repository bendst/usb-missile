/*
	TODO - Fehlerbehandlungen einrichten bzw. umformulieren

*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libudev.h>

#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define PATH_TO_DRIVER "/sys/bus/usb/drivers/usb_missile"
#define ID_VENDOR "2123"
#define ID_PRODUCT "1010"

#define PATH_MAX_LENGTH 256

#define KEY_MV_UP_LEFT 'q'
#define KEY_MV_UP 'w'
#define KEY_MV_UP_RIGHT 'e'
#define KEY_MV_LEFT 'a'
#define KEY_MV_STOP 's'
#define KEY_MV_RIGHT 'd'
#define KEY_MV_DOWN_LEFT 'y'
#define KEY_MV_DOWN 'x'
#define KEY_MV_DOWN_RIGHT 'c'
#define KEY_LED 'r'
#define KEY_FIRE 'f'
#define KEY_EXIT 'v'

int getActionPath(char **actionPath);
int openAction(int *fd, char *actionPath, const char *actionName);

int main(int argc, char *argv[])
{
	
	char c;
	char *actionPath = NULL;
	char led;
	
	printf("Willkommen beim Steuerprogramm zum USB-Raketen-Werfer!\n\n");
	
	// Überprüft, ob der Treiber geladen ist
	struct stat s;
	int retval = stat(PATH_TO_DRIVER, &s);
	if (retval == -1) {
		printf("Fehler: Auf das Verzeichnis %s kann nicht zugegriffen werden.\nIst das Treibermodul geladen?\n", PATH_TO_DRIVER);
		return -1;
	}
	
	retval = getActionPath(&actionPath);
	if (actionPath == NULL)
		return -1;
	
	// Sys-Devices öffnen
	int fdUpLeft, fdUp, fdUpRight, fdLeft, fdStop, fdRight, fdDownLeft, fdDown, fdDownRight, fdLedOn, fdLedOff, fdFire;
	retval = openAction(&fdUpLeft, actionPath, "action_UpLeft");
	retval = openAction(&fdUp, actionPath, "action_Up");
	retval = openAction(&fdUpRight, actionPath, "action_UpRight");
	retval = openAction(&fdLeft, actionPath, "action_Left");
	retval = openAction(&fdStop, actionPath, "action_Stop");
	retval = openAction(&fdRight, actionPath, "action_Right");
	retval = openAction(&fdDownLeft, actionPath, "action_DownLeft");
	retval = openAction(&fdDown, actionPath, "action_Down");
	retval = openAction(&fdDownRight, actionPath, "action_DownRight");
	retval = openAction(&fdLedOn, actionPath, "action_LedOn");
	retval = openAction(&fdLedOff, actionPath, "action_LedOff");
	retval = openAction(&fdFire, actionPath, "action_Fire");
	/* TODO - Fehlerbehandlung für alle openActions;
	if (retval) {
		free(actionPath);
		return -1;
	}
	*/
	
	// Lese LED-Status ein 
	char buf;
	retval = read(fdLedOn, &buf, 1);
	// TODO - Fehlerbehandlung
	while (buf != '0' && buf != '1') {
		retval = read(fdLedOn, &buf, 1);
		// TODO - Fehlerbehandlung
	}
	led = buf;
	
	printf("Steuerung:\n");
	printf("%c - Drehung nach oben-links\n", KEY_MV_UP_LEFT);
	printf("%c - Drehung nach oben\n", KEY_MV_UP);
	printf("%c - Drehung nach oben-rechts\n", KEY_MV_UP_RIGHT);
	printf("%c - Drehung links\n", KEY_MV_LEFT);
	printf("%c - Drehung stoppen\n", KEY_MV_STOP);
	printf("%c - Drehung rechts\n", KEY_MV_RIGHT);
	printf("%c - Drehung nach unten-links\n", KEY_MV_DOWN_LEFT);
	printf("%c - Drehung nach unten\n", KEY_MV_DOWN);
	printf("%c - Drehung nach unten-rechts\n", KEY_MV_DOWN_RIGHT);
	printf("%c - LED an/aus\n", KEY_LED);
	printf("%c - Feuer\n", KEY_FIRE);
	printf("%c - Beenden\n\n", KEY_EXIT);
	
	printf("Viel Spaß!\n\n");
	
	retval = system("stty cbreak -echo");
	// TODO - Fehlerbehandlung
	
	do {
		c = getchar();
		switch (c) {
		case KEY_MV_UP_LEFT:
			retval = write(fdUpLeft, "1", 1);
			// TODO - Fehlerbehandlung
			break;
		case KEY_MV_UP:
			retval = write(fdUp, "1", 1);
			break;
		case KEY_MV_UP_RIGHT:
			retval = write(fdUpRight, "1", 1);
			break;
		case KEY_MV_LEFT:
			retval = write(fdLeft, "1", 1);
			break;
		case KEY_MV_STOP:
			retval = write(fdStop, "1", 1);
			break;
		case KEY_MV_RIGHT:
			retval = write(fdRight, "1", 1);
			break;
		case KEY_MV_DOWN_LEFT:
			retval = write(fdDownLeft, "1", 1);
			break;
		case KEY_MV_DOWN:
			retval = write(fdDown, "1", 1);
			break;
		case KEY_MV_DOWN_RIGHT:
			retval = write(fdDownRight, "1", 1);
			break;
		case KEY_LED:
			if (led == '0') {
				retval = write(fdLedOn, "1", 1);
				led = '1';
			} else {
				retval = write(fdLedOff, "1", 1);
				led = '0';
			}
			break;
		case KEY_FIRE:
			retval = write(fdFire, "1", 1);
			sleep(3);
			break;
		case KEY_EXIT:
			printf("Auf Wiedersehen!\n");
			break;
		default:
			break;
		}
	} while (c != KEY_EXIT);
	
	// Sys-Devices schließen
	retval = close(fdUpLeft);
	retval = close(fdUp);
	retval = close(fdUpRight);
	retval = close(fdLeft);
	retval = close(fdStop);
	retval = close(fdRight);
	retval = close(fdDownLeft);
	retval = close(fdDown);
	retval = close(fdDownRight);
	retval = close(fdLedOn);
	retval = close(fdLedOff);
	retval = close(fdFire);
	/* TODO - Fehlerbehandlung
	if (retval == -1) {	
	} */
	
	free(actionPath);
	retval = system("stty cooked echo");
	// TODO - Fehlerbehandlung
	
	return 0;
}


int getActionPath(char **actionPath) {
	int i;
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;	
	
	udev = udev_new();
	if (udev == NULL) {
		printf("Fehler: udev kann nicht erstellt werden.\n");
		return -1;
	}
	// Suche das passende Gerät
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "usb");
	udev_enumerate_add_match_sysattr(enumerate, "idVendor", ID_VENDOR);
	udev_enumerate_add_match_sysattr(enumerate, "idProduct", ID_PRODUCT);
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);
	
	if (devices == NULL) {
		printf("Fehler: Es wurde kein passendes Gerät gefunden.\n");
		udev_enumerate_unref(enumerate);
		udev_unref(udev);
		return -1;
	}
	
	i = 0;
	// Makro, expandiert zur Foreach-Schleife
	// Überprüft, ob nur ein Raketenwerfer angeschlossen wurde
	// Ermittelt den Pfad zu den sysfs-Dateien
	udev_list_entry_foreach(dev_list_entry, devices) {
		if (i > 0) {
			printf("Fehler: Es wurden mehr als ein USB-Raketenwerfer gefunden. Bitte entfernen Sie alle überschüssigen Raketenwerfer bis nur noch einer angeschlossen ist.\n");
			udev_enumerate_unref(enumerate);
			udev_unref(udev);
			free(*actionPath);
			return -1;
		}
		
		int pathLen = 7; // initialer Wert kommt von "Füllstrings" (s. unten)
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
		
		*actionPath = calloc(pathLen + 1, sizeof(char));
		if (*actionPath == NULL) {
			printf("Fehler: Out of memory.\n");
			udev_device_unref(dev);
			udev_enumerate_unref(enumerate);
			udev_unref(udev);
			return -1;
		}
		
		strncpy(*actionPath, sysPath, sysPathLen);
		strncat(*actionPath, "/", 1);
		strncat(*actionPath, busNum, busNumLen);
		strncat(*actionPath, "-", 1);
		strncat(*actionPath, devPath, devPathLen);
		strncat(*actionPath, ":1.0/", 5);
		
		udev_device_unref(dev);
		i++;	
	}
	
	udev_enumerate_unref(enumerate);
	udev_unref(udev);

	return 0;
}


int openAction(int *fd, char *actionPath, const char *actionName) {
	char *buf;
	int pathLen = strlen(actionPath);
	int nameLen = strlen(actionName);
	buf = calloc (pathLen + nameLen + 1, sizeof(char));
	if (buf == NULL) {
		printf("Error in file %s in lin %d: Out of Memory\n", __FILE__, __LINE__);
		return -1;
	}
	strncpy(buf, actionPath, pathLen);
	strncat(buf, actionName, nameLen);
	*fd = open(buf, O_RDWR);
	if (*fd == -1) {
		printf("ERROR in %s in line %d: Cannot open file \"%s\". %s.\n", __FILE__, __LINE__, actionName, strerror(errno));
		return -1;
	}
	free(buf);
	return 0;
}
