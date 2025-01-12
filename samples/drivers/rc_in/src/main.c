#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <shell/shell.h>
#include <settings/settings.h>

#include "custom_drivers/rc.h"

#define RC_IN DT_NODELABEL(rc)

const struct device *receiver = DEVICE_DT_GET(RC_IN);
struct Command rc_in;

static volatile bool do_print = false;

static int cmd_save_settings(const struct shell *sh, size_t argc, char **argv) {
    settings_save();
    return 0;
}

static int cmd_toggle_print(const struct shell *sh, size_t argc, char **argv) {
    do_print = !do_print;
    return 0;
}

SHELL_CMD_REGISTER(settings_save, NULL, "Save all parameters", cmd_save_settings);
SHELL_CMD_REGISTER(toggle_printing, NULL, "Toggle rc value print", cmd_toggle_print);

void main() {

    int rc;
    

    // check for rc device
	if (!device_is_ready(receiver)) {
		printk("RC IN device not ready.\n");
		return;
	}

    // check and initialize seetings
    rc = settings_subsys_init();
	if (rc) {
		printk("settings subsys initialization: fail (err %d)\n", rc);
		return;
	}

    rc = settings_load();
    if (rc) {
        printk("Failed loading settings from none volatile storage. Failed with error %d\n", rc);
    }

    while (true)  {
        rc_update(receiver, &rc_in);
        if (do_print) {
            printk("Got roll: %d, pitch: %d, thrust: %d, yaw: %d\n", 
                (int)(rc_in.roll*100), (int)(rc_in.pitch*100), 
                (int)(rc_in.thrust*100), (int)(rc_in.yaw*100));

        }
        
        k_sleep(K_MSEC(500));
    }
}
