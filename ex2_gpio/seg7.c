#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>

#define IO_OFFSET 512
static uint8_t pin_nr[4] = {11, 9, 25, 8};
#define IO_BUTTON 24

static struct gpio_desc *outputs[4];
static struct gpio_desc *button;

static int __init my_init(void)
{
	int i, status;

	for (i=0; i<4; i++) {
		outputs[i] = gpio_to_desc(pin_nr[i] + IO_OFFSET);
		if (outputs[i] == NULL) {
			printk("Couldn't get GPIO %d\n", pin_nr[i]);
			return -ENODEV;
		}
		status = gpiod_direction_output(outputs[i], 0);
		if (status) {
			printk("Couldn't configure GPIO %d as output\n", pin_nr[i]);
			return status;
		}
	}
	gpiod_set_value(outputs[0], 1);

	button = gpio_to_desc(IO_OFFSET + IO_BUTTON);
	if (!button) {
		printk("Couldn't get GPIO %d\n", IO_BUTTON);
		return -ENODEV;
	}

	status = gpiod_direction_input(button);
	if (status) {
		printk("Couldn't configure GPIO %d as output\n", IO_BUTTON);
		return status;
	}

	printk("Button is %s\n", gpiod_get_value(button) ? "pressed" : "not pressed");

	return 0;
}

static void __exit my_exit(void)
{
	gpiod_set_value(outputs[0], 0);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes Roith");
MODULE_DESCRIPTION("A hello world kernel module");

module_init(my_init);
module_exit(my_exit);

