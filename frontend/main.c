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


#define open_device(fdName, actionName, label) fdName = openAction(actionPath, actionName); \
	if (fdName < 0) { \
		error_at_line(0, 0, __FILE__, __LINE__, "openAction() failed"); \
		result = EXIT_FAILURE; \
		goto label; \
	}

#define write_device(fdName) retval = write(fdName, "1", 1); \
	if (retval < 0) { \
		error_at_line(0, errno, __FILE__, __LINE__, "write() failed"); \
		result = EXIT_FAILURE; \
		goto error_14; \
	}

#define close_device(fdName) retval = close(fdName); \
	if (retval < 0) { \
		error_at_line(0, errno, __FILE__, __LINE__, "close() failed"); \
		result = EXIT_FAILURE; \
	}


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
	if (retval < 0)
		error_at_line(EXIT_FAILURE, 0, __FILE__, __LINE__, "getActionPath()");

	// Sys-Devices öffnen
	int fdUpLeft, fdUp, fdUpRight, fdLeft, fdStop, fdRight, fdDownLeft, fdDown, fdDownRight, fdLedOn, fdLedOff, fdFire;

	open_device(fdUpLeft, "action_UpLeft", error_1);
	open_device(fdUp, "action_Up", error_2);
	open_device(fdUpRight, "action_UpRight", error_3);
	open_device(fdLeft, "action_Left", error_4);
	open_device(fdStop, "action_Stop", error_5);
	open_device(fdRight, "action_Right", error_6);
	open_device(fdDownLeft, "action_DownLeft", error_7);
	open_device(fdDown, "action_Down", error_8);
	open_device(fdDownRight, "action_DownRight", error_9);
	open_device(fdLedOn, "action_LedOn", error_10);
	open_device(fdLedOff, "action_LedOff", error_11);
	open_device(fdFire, "action_Fire", error_12);

	char led = '0';
	write_device(fdLedOff);
	write_device(fdStop);

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
		error_at_line(0, errno, __FILE__, __LINE__, "system() failed");
		result = EXIT_FAILURE;
		goto error_13;
	}

	char pressedKey;
	do {
		pressedKey = getchar();
		switch (pressedKey) {
		case KEY_MV_UP_LEFT:
			write_device(fdUpLeft);
			break;
		case KEY_MV_UP:
			write_device(fdUp);
			break;
		case KEY_MV_UP_RIGHT:
			write_device(fdUpRight);
			break;
		case KEY_MV_LEFT:
			write_device(fdLeft);
			break;
		case KEY_MV_STOP:
			write_device(fdStop);
			break;
		case KEY_MV_RIGHT:
			write_device(fdRight);
			break;
		case KEY_MV_DOWN_LEFT:
			write_device(fdDownLeft);
			break;
		case KEY_MV_DOWN:
			write_device(fdDown);
			break;
		case KEY_MV_DOWN_RIGHT:
			write_device(fdDownRight);
			break;
		case KEY_LED:
			if (led == '0') {
				write_device(fdLedOn);
				led = '1';
			} else {
				write_device(fdLedOff);
				led = '0';
			}
			break;
		case KEY_FIRE:
			write_device(fdFire);
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
		error_at_line(0, errno, __FILE__, __LINE__, "system() failed");
		result = EXIT_FAILURE;
	}

	// Sys-Devices schließen
error_13:
	close_device(fdFire);
error_12:
	close_device(fdLedOff);
error_11:
	close_device(fdLedOn);
error_10:
	close_device(fdDownRight);
error_9:
	close_device(fdDown);
error_8:
	close_device(fdDownLeft);
error_7:
	close_device(fdRight);
error_6:
	close_device(fdStop);
error_5:
	close_device(fdLeft);
error_4:
	close_device(fdUpRight);
error_3:
	close_device(fdUp);
error_2:
	close_device(fdUpLeft);
error_1:
	free(actionPath);

	return result;
}


int getActionPath(char **actionPath)
{
	int retval = 0;

	// Suche das passende Gerät
	struct udev *udev = udev_new();

	if (udev == NULL) {
		error_at_line(0, 0, __FILE__, __LINE__, "udev_new() failed");
		return -1;
	}
	struct udev_enumerate *enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "usb");
	udev_enumerate_add_match_sysattr(enumerate, "idVendor", ID_VENDOR);
	udev_enumerate_add_match_sysattr(enumerate, "idProduct", ID_PRODUCT);
	udev_enumerate_scan_devices(enumerate);
	struct udev_list_entry *device = udev_enumerate_get_list_entry(enumerate);
	if (device == NULL) {
		error_at_line(0, 0, __FILE__, __LINE__, "Can't find matching device");
		udev_enumerate_unref(enumerate);
		udev_unref(udev);
		return -1;
	}

	// Überprüfe, ob mehr als ein Raketenwerfer vorhanden
	if (udev_list_entry_get_next(device) != NULL) {
		error_at_line(0, 0, __FILE__, __LINE__, "Found more than one usb-missile-launcher. Please unplug surplus devices and restart this program");
		free(*actionPath);
		udev_enumerate_unref(enumerate);
		udev_unref(udev);
		return -1;
	}

	// Ermittle Pfad zu den sysfs-Dateien
	const char *path = udev_list_entry_get_name(device);
	struct udev_device *dev = udev_device_new_from_syspath(udev, path);
	const char *sysPath = udev_device_get_syspath(dev);
	const char *busNum = udev_device_get_sysattr_value(dev, "busnum");
	const char *devPath = udev_device_get_sysattr_value(dev, "devpath");
	// +7, wegen Füllzeichen (s. unten)
	int pathLen = strlen(sysPath) + strlen(busNum) + strlen(devPath) + 7;
	*actionPath = calloc(pathLen + 1, sizeof(char));
	if (*actionPath == NULL) {
		error_at_line(0, errno, __FILE__, __LINE__, "calloc() failed");
		udev_device_unref(dev);;
		udev_enumerate_unref(enumerate);
		udev_unref(udev);
		return -1;
	}
	snprintf(*actionPath, pathLen + 1, "%s/%s-%s:1.0/", sysPath, busNum, devPath);

	udev_device_unref(dev);
	udev_enumerate_unref(enumerate);
	udev_unref(udev);

	return retval;
}


int openAction(char *actionPath, const char *actionName)
{
	char *buf;
	int pathLen = strlen(actionPath);
	int nameLen = strlen(actionName);
	int filePathLen = pathLen + nameLen;

	buf = calloc(filePathLen + 1, sizeof(char));
	if (buf == NULL) {
		error_at_line(0, errno, __FILE__, __LINE__, "calloc() failed");
		return -1;
	}
	snprintf(buf, filePathLen + 1, "%s%s", actionPath, actionName);
	int fd = open(buf, O_RDWR);
	if (fd < 0)
		error_at_line(0, errno, __FILE__, __LINE__, "Can't open file \"%s\"", actionName);
	free(buf);
	return fd;
}
