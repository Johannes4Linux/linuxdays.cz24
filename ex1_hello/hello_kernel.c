#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	printk("hello - Hello, Kernel!\n");
	return 0;
}

static void __exit my_exit(void)
{
	printk("hello - Goodbye, kernel!\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes Roith");
MODULE_DESCRIPTION("A hello world kernel module");

module_init(my_init);
module_exit(my_exit);

