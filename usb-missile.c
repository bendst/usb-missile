#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>


#define DRIVER_AUTHOR "Florian Schwarz, Ben Stuart"
#define DRIVER_DESC "USB Missile Launcher"

#define VENDOR_ID 0x2123
#define PRODUCT_ID 0x1010

// usb_control_msg-Parameter
#define REQUEST 0x09
#define REQUEST_TYPE 0x21
#define MSG_VALUE 0 // aus pylibusb und frontend-launcher
#define MSG_INDEX 0 // "
#define LED_PREFIX 0x03
#define MV_PREFIX 0x02
#define BUF_SIZE 8


static struct usb_device_id id_table[] = {
	{USB_DEVICE(VENDOR_ID, PRODUCT_ID)},
	{},
};
MODULE_DEVICE_TABLE(usb, id_table);

enum Direction {
	Stop,
	Down,
	Up,
	Left = 0x04,
	Right = 0x08,
	UpLeft = Left | Up,
	DownLeft = Left | Down,
	UpRight = Right | Up,
	DownRight = Right | Down,
};

enum Led {
	LedOff,
	LedOn
};

enum CMDType {
	Led,
	Movement,
	Fire
};

struct usb_missile {
	struct usb_device *udev;
	enum Direction direction;
	enum Led led;
};


static void execute_order(struct usb_missile *missile, enum CMDType cmdType) {
	int result = 0;
	unsigned char *buf = NULL;

	buf = kzalloc(BUF_SIZE, GFP_KERNEL);
	if (buf == NULL) {
		dev_err(&missile->udev->dev, "Out of memory");
		return;
	}

	switch (cmdType) {
	case Movement:
		buf[0] = MV_PREFIX;
		buf[1] = missile->direction;
		break;
	case Led:
		buf[0] = LED_PREFIX;
		buf[1] = missile->led;
		break;
	case Fire:
		buf[0] = MV_PREFIX;
		buf[1] = 0x10;
	default:
		break;

	}

	// TODO richtiges timeout noch herausfinden
	result = usb_control_msg(missile->udev, usb_sndctrlpipe(missile->udev, 0), 
				REQUEST, REQUEST_TYPE, MSG_VALUE, MSG_INDEX, 
				buf, BUF_SIZE, 2 * HZ);
	if (result < 0) {
		dev_err(&missile->udev->dev, "could not send data, direction: %d, led: %d\n", missile->direction, missile->led);
	}

	kfree(buf);
}

// Das Verhalten der Geräte-Funktions-Dateien, wenn von ihnen gelesen bzw. auf sie geschrieben wird
#define function(name, cmdType) \
static ssize_t show_ ## name(struct device *dev, struct device_attribute *attr, char *buf) { \
	struct usb_missile *missile = usb_get_intfdata(to_usb_interface(dev));	\
	printk("%s\n", #name);							\
	if (cmdType == Movement) {						\
		return sprintf(buf, "Last movement: %d\n", missile->direction);	\
	} else if (cmdType == Led) {						\
		return sprintf(buf, "LED status: %d\n", missile->led);		\
	} else {								\
		return sprintf(buf, "Nothing to read.\n");			\
	}									\
}										\
static ssize_t store_ ## name(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) { \
	struct usb_missile *missile = usb_get_intfdata(to_usb_interface(dev));	\
	if (cmdType == Movement) {						\
		missile->direction = name;					\
	} else if (cmdType == Led) {						\
		missile->led = name;						\
	}									\
	execute_order(missile, cmdType);					\
	return count;								\
}										\
static DEVICE_ATTR(name, S_IRUSR | S_IWUSR, show_ ## name, store_ ## name);

#define movement(name) function(name, Movement)
#define action(name) function(name, Fire)
#define led(name) function(name, Led)

movement(Up);
movement(Down);
movement(Left);
movement(Right);
movement(UpLeft);
movement(UpRight);
movement(DownRight);
movement(DownLeft);
movement(Stop);

action(Fire);

led(LedOn);
led(LedOff);

// Makros zum Erstellen der Geräte-Funktionen als Dateien in sys
// TODO mögliche Fehler abfangen
#define create(name) device_create_file(&interface->dev, &dev_attr_ ## name)
#define create_all  create(Stop); create(Down); create(Up); create(Left); create(Right); create(UpLeft); create(UpRight); create(DownLeft); create(DownRight); create(Fire); create(LedOn); create(LedOff)
#define remove(name) device_remove_file(&interface->dev, &dev_attr_ ## name)
#define remove_all remove(Stop); remove(Down); remove(Up); remove(Left); remove(Right); remove(UpLeft); remove(UpRight);  remove(DownLeft); remove(DownRight); remove(Fire); remove(LedOn); remove(LedOff)

static int missile_probe(struct usb_interface *interface,
						 const struct usb_device_id *id) {
	struct usb_device *udev = interface_to_usbdev(interface);
	struct usb_missile *dev = NULL;

	dev = kzalloc(sizeof(struct usb_missile), GFP_KERNEL);
	if (dev == NULL) {
		dev_err(&interface->dev, "Out of memory\n");
		return -ENOMEM;
	}

	dev->udev = usb_get_dev(udev);
	usb_set_intfdata(interface, dev);
	// Alle benötigen Devices erstellen
	create_all;

	dev_info(&interface->dev, "Connected\n");
	return 0;
}

static void missile_disconnect(struct usb_interface *interface) {
	struct usb_missile *dev;

	dev = usb_get_intfdata(interface);
	usb_set_intfdata(interface, NULL);

	// Alle Devices wieder löschen
	remove_all;

	usb_put_dev(dev->udev);
	kfree(dev);
	dev_info(&interface->dev, "Disconnect\n");
}

static struct usb_driver missile_driver = {
	.name = "usb_missile",
	.probe = missile_probe,
	.disconnect = missile_disconnect,
	.id_table = id_table,
};

module_usb_driver(missile_driver);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
