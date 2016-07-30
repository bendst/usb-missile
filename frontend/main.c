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
	
	printf("Willkommen beim Steuerprogramm zum USB-Raketen-Werfer!\n\n");
	
	// Überprüfe, ob Treiber geladen ist
	struct stat s;
	int retval = stat(PATH_TO_DRIVER, &s);
	if (retval < 0) {
		printf("Error in file %s in line %d: Can't access %s.\nMaybe driver module isn't loaded.\n", __FILE__, __LINE__, PATH_TO_DRIVER);
		return retval;
	}
	
	// Ermittle Pfad zu Sys-Devices
	char *actionPath = NULL;
	retval = getActionPath(&actionPath);
	if (retval < 0) return retval;
	
	// Sys-Devices öffnen
	int fdUpLeft, fdUp, fdUpRight, fdLeft, fdStop, fdRight, fdDownLeft, fdDown, fdDownRight, fdLedOn, fdLedOff, fdFire;
	retval = openAction(&fdUpLeft, actionPath, "action_UpLeft");
	if (retval < 0) goto error_1;
	retval = openAction(&fdUp, actionPath, "action_Up");
	if (retval < 0) goto error_2;
	retval = openAction(&fdUpRight, actionPath, "action_UpRight");
	if (retval < 0) goto error_3;
	retval = openAction(&fdLeft, actionPath, "action_Left");
	if (retval < 0) goto error_4;
	retval = openAction(&fdStop, actionPath, "action_Stop");
	if (retval < 0) goto error_5;
	retval = openAction(&fdRight, actionPath, "action_Right");
	if (retval < 0) goto error_6;
	retval = openAction(&fdDownLeft, actionPath, "action_DownLeft");
	if (retval < 0) goto error_7;
	retval = openAction(&fdDown, actionPath, "action_Down");
	if (retval < 0) goto error_8;
	retval = openAction(&fdDownRight, actionPath, "action_DownRight");
	if (retval < 0) goto error_9;
	retval = openAction(&fdLedOn, actionPath, "action_LedOn");
	if (retval < 0) goto error_10;
	retval = openAction(&fdLedOff, actionPath, "action_LedOff");
	if (retval < 0) goto error_11;
	retval = openAction(&fdFire, actionPath, "action_Fire");
	if (retval < 0) goto error_12;
	
	// Lese LED-Status ein 
	char led;
	retval = read(fdLedOn, &led, 1);
	// TODO - Fehlerbehandlung
	while (led != '0' && led != '1') {
		retval = read(fdLedOn, &led, 1);
		// TODO - Fehlerbehandlung
	}
	
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
	
	char pressedKey;
	do {
		pressedKey = getchar();
		switch (pressedKey) {
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
	} while (pressedKey != KEY_EXIT);
	
	retval = system("stty cooked echo");
	// TODO - Fehlerbehandlung

	// Sys-Devices schließen
	retval = close(fdFire);
error_12:
	retval = close(fdLedOff);
error_11:
	retval = close(fdLedOn);
error_10:
	retval = close(fdDownRight);
error_9:
	retval = close(fdDown);
error_8:
	retval = close(fdDownLeft);
error_7:
	retval = close(fdRight);
error_6:
	retval = close(fdStop);
error_5:
	retval = close(fdLeft);
error_4:
	retval = close(fdUpRight);
error_3:
	retval = close(fdUp);
error_2:
	retval = close(fdUpLeft);
error_1:
	
	/* TODO - Fehlerbehandlung
	if (retval == -1) {	
	} */
	
	free(actionPath);
	
	return retval;
}


int getActionPath(char **actionPath) {
	int i;
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	int retval = 0;	
	
	udev = udev_new();
	if (udev == NULL) {
		printf("Error in file %s in line %d: Can't create udev.\n", __FILE__, __LINE__);
		retval = -1;
		return retval;
	}
	// Suche das passende Gerät
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "usb");
	udev_enumerate_add_match_sysattr(enumerate, "idVendor", ID_VENDOR);
	udev_enumerate_add_match_sysattr(enumerate, "idProduct", ID_PRODUCT);
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);
	
	if (devices == NULL) {
		printf("Error in file %s in line %d: Can't find matching device.\n", __FILE__, __LINE__);
		retval = -1;
		goto error;
	}
	
	i = 0;
	// Makro, expandiert zur Foreach-Schleife
	// Überprüft, ob nur ein Raketenwerfer angeschlossen wurde
	// Ermittelt den Pfad zu den sysfs-Dateien
	udev_list_entry_foreach(dev_list_entry, devices) {
		if (i > 0) {
			printf("Error in file %s in line %d: Found more than one usb-missile-launcher. Please unplug surplus devices.\n", __FILE__, __LINE__);
			free(*actionPath);
			retval = -1;
			goto error;
		}
		
		int pathLen = 7; // initialer Wert von Länge der "Füllzeichen" (s. unten)
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
			printf("Error in file %s in line %d: Out of memory.\n", __FILE__, __LINE__);
			udev_device_unref(dev);
			retval = -1;
			goto error;
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
	
error:
	udev_enumerate_unref(enumerate);
	udev_unref(udev);

	return retval;
}


int openAction(int *fd, char *actionPath, const char *actionName) {
	char *buf;
	int pathLen = strlen(actionPath);
	int nameLen = strlen(actionName);
	buf = calloc (pathLen + nameLen + 1, sizeof(char));
	if (buf == NULL) {
		printf("Error in file %s in lin %d: Out of memory.\n", __FILE__, __LINE__);
		return -1;
	}
	strncpy(buf, actionPath, pathLen);
	strncat(buf, actionName, nameLen);
	*fd = open(buf, O_RDWR);
	if (*fd == -1) {
		printf("Error in file %s in line %d: Can't open file \"%s\". %s.\n", __FILE__, __LINE__, actionName, strerror(errno));
		free(buf);
		return -1;
	}
	free(buf);
	return 0;
}
