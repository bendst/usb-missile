#include <linux/kconfig.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>


#define DRIVER_AUTHOR "Florian Schwarz, Ben Stuart"
#define DRIVER_DESC "USB Missile Launcher"

#define VENDOR_ID 0x2123
#define PRODUCT_ID 0x1010

#define LED_PREFIX 0x03
#define MV_PREFIX 0x02

// https://www.kernel.org/doc/htmldocs/usb/API-usb-control-msg.html
#define REQUEST_TYPE 0x21   // ungetest
#define REQUEST 0x09        // ungetest

static struct usb_device_id id_table[] = {
    {USB_DEVICE(VENDOR_ID, PRODUCT_ID)},
    {},
};

MODULE_DEVICE_TABLE(usb, id_table);

// Ein enum fängt bei 0 an.
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
    Off,
    On
};

struct usb_missile {
    struct usb_device *udev;
    enum Direction direction;
    enum Led led;
    unsigned char fire;
};



static ssize_t show_dummy(struct device *dev, struct device_attribute *attr, char *buf) {
    dev_dbg(dev, "%d\n", 42);
    return sprintf(buf, "\n");
}


static ssize_t store_dummy(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    return count;
}

static DEVICE_ATTR(dummy, S_IWUSR | S_IRUSR, show_dummy, store_dummy);

static void execute_order(struct usb_missile *missile) {
    // TODO Alles
    unsigned char order = Stop;

    switch (missile->direction) {
    case Down:
        break;
    case Up:
        break;
    case Left:
        break;
    case Right:
        break;
    case UpLeft:
        break;
    case DownLeft:
        break;
    case UpRight:
        break;
    case DownRight:
        break;
    default:
        order = Stop;
        break;
    }

    switch (missile->led) {
    case Off:
        break;
    case On:
        break;
    default:
        break;
    }

}

static int missile_probe(struct usb_interface *interface,
                         const struct usb_device_id *id) {
    printk("halaskdlasd");
    struct usb_device *udev = interface_to_usbdev(interface);
    struct usb_missile *dev = NULL;
    int result = -ENOMEM; 
    
    dev = kmalloc(sizeof(struct usb_missile), GFP_KERNEL);
    if (dev == NULL) {
        dev_err(&interface->dev, "OOM\n");
        goto error;
    }
    memset(dev, 0, sizeof(struct usb_missile));
    dev->udev = usb_get_dev(udev);
    usb_set_intfdata(interface, dev);

    device_create_file(&interface->dev, &dev_attr_dummy);
    // TODO devices erstellen


    dev_info(&interface->dev, "Connected\n");
    return 0;

error:
    // Ist das wirklich notwendig?
    dev_info(&interface->dev, "Error\n");
    kfree(dev);
    return result;
}

static void missile_disconnect(struct usb_interface *interface) {
    struct usb_missile *dev;

    dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);

    device_remove_file(&interface->dev, &dev_attr_dummy);
    // TODO erstelte Devices in Probe wieder löschen


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

