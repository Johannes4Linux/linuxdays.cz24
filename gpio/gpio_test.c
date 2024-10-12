#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>

#define IO_OFFSETS 512

#define IO_LED 11
#define IO_BUTTON 24

struct gpio_desc *led;
struct gpio_desc *button;

static int my_init(void)
{
	int status;

	led = gpio_to_desc(IO_OFFSETS + IO_LED);
	if (!led) {
		printk("Error getting IO_LED\n");
		return -ENODEV;
	}

	button = gpio_to_desc(IO_OFFSETS + IO_BUTTON);
	if (!button) {
		printk("Error getting IO_BUTTON\n");
		return-ENODEV;
	}

	status = gpiod_direction_output(led, 1);
	if (status != 0) {
		printk("Error setting IO_LED to output\n");
		return status;
	}

	status = gpiod_direction_input(button);
	if (status != 0) {
		printk("Error setting IO_BUTTON to input\n");
		return status;
	}

	printk("Button is %s\n", gpiod_get_value(button) ? "pressed" : "not pressed");
	return 0;
}

static void my_exit(void)
{
	gpiod_set_value(led, 0);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
