#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gpio/consumer.h>

#define IO_OFFSET 512
static uint8_t pin_nr[4] = {11, 9, 25, 8};
#define IO_BUTTON 24

#define MY_MAYOR 64

static struct gpio_desc *outputs[4];
static struct gpio_desc *button;

static ssize_t my_write(struct file *file, const char __user *buffer, size_t len, loff_t *off)
{
	char kbuf[16];
	int not_copied, to_copy = (len > 16) ? 16 : len;

	not_copied = copy_from_user(kbuf, buffer, to_copy);

	if (!not_copied) {
		gpiod_set_value(outputs[0], kbuf[0] & (1<<0));
		gpiod_set_value(outputs[1], kbuf[0] & (1<<1));
		gpiod_set_value(outputs[2], kbuf[0] & (1<<2));
		gpiod_set_value(outputs[3], kbuf[0] & (1<<3));
	}
	return to_copy - not_copied;
}

static ssize_t my_read(struct file *file, char __user *buffer, size_t len, loff_t *off)
{
	char kbuf[2];
	int not_copied, to_copy = (len > 2) ? 2 : len;

	memset(kbuf, 0, 2);
	kbuf[1] = '\n';
	kbuf[0] = gpiod_get_value(button) + '0';

	not_copied = copy_to_user(buffer, kbuf, 2);
	return to_copy - not_copied;
}

static struct file_operations fops = {
	.write = my_write,
	.read = my_read
};

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

	return register_chrdev(MY_MAYOR, "seg", &fops);
}

static void __exit my_exit(void)
{
	unregister_chrdev(MY_MAYOR, "seg");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes Roith");
MODULE_DESCRIPTION("A hello world kernel module");

module_init(my_init);
module_exit(my_exit);

