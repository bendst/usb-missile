#include <linux/init.h>
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

static void execute_order(struct usb_missile *missile) {

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

    // devices erstellen
    return 0;
}

static void missile_disconnect(struct usb_interface *interface) {
    // erstelte Devices in Probe wieder löschen
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

