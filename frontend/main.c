#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libudev.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>

#define PATH_TO_DRIVER "/sys/bus/usb/drivers/usb_missile"
#define ID_VENDOR "2123"
#define ID_PRODUCT "1010"

#define LED_DELIMITER '['
#define LED_MAX_READ_CHARS 64

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
int openAction(char *actionPath, const char *actionName);

int main(int argc, char *argv[])
{
	int result = EXIT_SUCCESS;
	
	printf("Willkommen zur Raketenwerfer-Steuerung!\n\n");
	
	// Überprüfe, ob Treiber geladen ist
	struct stat s;
	int retval = stat(PATH_TO_DRIVER, &s);
	if (retval < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "Couldn't find driver module");
		error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", PATH_TO_DRIVER);
	}
	
	// Ermittle Pfad zu Sys-Devices
	char *actionPath = NULL;
	retval = getActionPath(&actionPath);
	if (retval < 0) {
		error_at_line(EXIT_FAILURE, 0, __FILE__, __LINE__, "getActionPath()");
	}
	
	// Sys-Devices öffnen
	int fdUpLeft, fdUp, fdUpRight, fdLeft, fdStop, fdRight, fdDownLeft, fdDown, fdDownRight, fdLedOn, fdLedOff, fdFire;
	fdUpLeft = openAction(actionPath, "action_UpLeft");
	if (fdUpLeft < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_1;
	}
	fdUp = openAction(actionPath, "action_Up");
	if (fdUp < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_2;
	}
	fdUpRight = openAction(actionPath, "action_UpRight");
	if (fdUpRight < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_3;
	}
	fdLeft = openAction(actionPath, "action_Left");
	if (fdLeft < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_4;
	}
	fdStop = openAction(actionPath, "action_Stop");
	if (fdStop < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_5;
	}
	fdRight = openAction(actionPath, "action_Right");
	if (fdRight < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_6;
	}
	fdDownLeft = openAction(actionPath, "action_DownLeft");
	if (fdDownLeft < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_7;
	}
	fdDown = openAction(actionPath, "action_Down");
	if (fdDown < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_8;
	}
	fdDownRight = openAction(actionPath, "action_DownRight");
	if (fdDownRight < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_9;
	}
	fdLedOn = openAction(actionPath, "action_LedOn");
	if (fdLedOn < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_10;
	}
	fdLedOff = openAction(actionPath, "action_LedOff");
	if (fdLedOff < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_11;
	}
	fdFire = openAction(actionPath, "action_Fire");
	if (fdFire < 0) {
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed");
		result = EXIT_FAILURE;
		goto error_12;
	}
	
	// Lese LED-Status ein 
	char led;
	retval = read(fdLedOn, &led, 1);
	if (retval == 1) {
		for (int i=1; (led!=LED_DELIMITER) && (i<=LED_MAX_READ_CHARS); i++) {
			retval = read(fdLedOn, &led, 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"read() failed");
				goto fallback;
			}
		}
		if (led == LED_DELIMITER) {
			retval = read(fdLedOn, &led, 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"read() failed");
				goto fallback;
			}
			if (led != '0' && led != '1') {
				error_at_line(0, 0, __FILE__, __LINE__,"Read wrong Syntax in Led-Device");
				goto fallback;
			}
		} else {
			error_at_line(0, 0, __FILE__, __LINE__,"Read wrong Syntax in Led-Device");
			goto fallback;
		}
	} else {
	fallback:
		printf("Warning: Couldn't read LED-Status. Assuming LED is off.\n\n");
		led = '0';
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
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"system() failed");
		result = EXIT_FAILURE;
		goto error_13;
	}
	
	char pressedKey;
	do {
		pressedKey = getchar();
		switch (pressedKey) {
		case KEY_MV_UP_LEFT:
			retval = write(fdUpLeft, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_MV_UP:
			retval = write(fdUp, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_MV_UP_RIGHT:
			retval = write(fdUpRight, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_MV_LEFT:
			retval = write(fdLeft, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_MV_STOP:
			retval = write(fdStop, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_MV_RIGHT:
			retval = write(fdRight, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_MV_DOWN_LEFT:
			retval = write(fdDownLeft, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_MV_DOWN:
			retval = write(fdDown, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_MV_DOWN_RIGHT:
			retval = write(fdDownRight, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			break;
		case KEY_LED:
			if (led == '0') {
				retval = write(fdLedOn, "1", 1);
				if (retval < 0) {
					error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
					result = EXIT_FAILURE;
					goto error_14;
				}
				led = '1';
			} else {
				retval = write(fdLedOff, "1", 1);
				if (retval < 0) {
					error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
					result = EXIT_FAILURE;
					goto error_14;
				}
				led = '0';
			}
			break;
		case KEY_FIRE:
			retval = write(fdFire, "1", 1);
			if (retval < 0) {
				error_at_line(0, errno, __FILE__, __LINE__,"write() failed");
				result = EXIT_FAILURE;
				goto error_14;
			}
			sleep(3);
			break;
		case KEY_EXIT:
			printf("Auf Wiedersehen!\n");
			break;
		default:
			break;
		}
	} while (pressedKey != KEY_EXIT);
	
error_14:
	retval = system("stty cooked echo");
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"system() failed");
		result = EXIT_FAILURE;
	}

	// Sys-Devices schließen
error_13:
	retval = close(fdFire);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_12:
	retval = close(fdLedOff);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_11:
	retval = close(fdLedOn);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_10:
	retval = close(fdDownRight);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_9:
	retval = close(fdDown);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_8:
	retval = close(fdDownLeft);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_7:
	retval = close(fdRight);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_6:
	retval = close(fdStop);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_5:
	retval = close(fdLeft);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_4:
	retval = close(fdUpRight);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_3:
	retval = close(fdUp);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_2:
	retval = close(fdUpLeft);
	if (retval < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"close() failed");
		result = EXIT_FAILURE;
	}
error_1:
	
	free(actionPath);
	return result;
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
		error_at_line(0, 0, __FILE__, __LINE__,"udev_new() failed");
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
		error_at_line(0, 0, __FILE__, __LINE__,"Can't find matching device");
		retval = -1;
		goto error;
	}
	
	i = 0;
	// Makro, expandiert zur Foreach-Schleife
	// Überprüft, ob nur ein Raketenwerfer angeschlossen wurde
	// Ermittelt den Pfad zu den sysfs-Dateien
	udev_list_entry_foreach(dev_list_entry, devices) {
		if (i > 0) {
			error_at_line(0, 0, __FILE__, __LINE__,"Found more than one usb-missile-launcher. Please unplug surplus devices");
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
			error_at_line(0, errno, __FILE__, __LINE__," ");
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


int openAction(char *actionPath, const char *actionName) {
	char *buf;
	int pathLen = strlen(actionPath);
	int nameLen = strlen(actionName);
	buf = calloc(pathLen + nameLen + 1, sizeof(char));
	if (buf == NULL) {
		error_at_line(0, errno, __FILE__, __LINE__," ");
		return -1;
	}
	strncpy(buf, actionPath, pathLen);
	strncat(buf, actionName, nameLen);
	int fd = open(buf, O_RDWR);
	if (fd < 0) {
		error_at_line(0, errno, __FILE__, __LINE__,"Can't open file \"%s\"", actionName);
		free(buf);
		return -1;
	}
	free(buf);
	return fd;
}
