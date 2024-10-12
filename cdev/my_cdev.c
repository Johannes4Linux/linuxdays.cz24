#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#define MYMAJOR 64

static int i;

static ssize_t my_read(struct file *file, char __user *buffer, size_t len, loff_t *off)
{
	char kbuf[16];
	int not_copied, to_copy = (sizeof(kbuf) < len) ? sizeof(kbuf) : len;

	/* Damit cat terminiert */
	if(*off >= to_copy)
		return 0;

	/* Wandeln wir i in einen String um */
	sprintf(kbuf, "0x%08x\n", i);


	/* Holen wir uns die Daten vom User- in den Kernelspace */
	not_copied = copy_to_user(buffer, kbuf, to_copy);

	/* Erhöhen wir die Offset, damit cat funktioniert */
	*off += to_copy - not_copied;

	/* Gib die Anzahl der gelesenen Bytes zurück */
	return to_copy - not_copied;
}

static ssize_t my_write(struct file *file, const char __user *buffer, size_t len, loff_t *off)
{
	char kbuf[16];
	long tmp;
	int not_copied, to_copy = (sizeof(kbuf) < len) ? sizeof(kbuf) : len;

	/* Holen wir uns die Daten vom User- in den Kernelspace */
	memset(kbuf, 0, sizeof(kbuf));
	not_copied = copy_from_user(kbuf, buffer, to_copy);


	/* Dann wandeln wir mal die Zahl um */
	if(kstrtol(kbuf, 0, &tmp) == 0) {
		i = (int) tmp;
	}

	/* Gib die Anzahl der geschriebenen Bytes zurück */
	return to_copy - not_copied;
}

static struct file_operations fops = {
	.read = my_read,
	.write = my_write,
};

static int __init my_init(void)
{
	return register_chrdev(MYMAJOR, "mydev", &fops);
}

static void __exit my_exit(void)
{
	unregister_chrdev(MYMAJOR, "mydev");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes Roith <johannes@gnu-linux.rocks>");
MODULE_DESCRIPTION("Ein Beispieltreiber zur Nutzung von Char-Devices und Gerätenummern");


