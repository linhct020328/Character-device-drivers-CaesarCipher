#include <linux/init.h>/* thu vien nay dinh nghia cac macro nhu module_init va module_exit */
#include <linux/module.h>/* thu vien nay dinh nghia cac macro nhu module_init va module_exit */
#include <linux/kernel.h>
#include <linux/device.h>/* thu vien nay chua cac ham phuc vu viec tao device file */
#include <linux/slab.h>/* thu vien nay chua cac ham kmalloc va kfree */
#include <linux/string.h>//thu vien sd cac ham của string: strlen
#include <linux/ctype.h>/*thu vien sd cac ham kiem tra, anh xa cac ky tu: isalpha(),isupper(),islower()*/
#include <linux/cdev.h>/* thu vien nay chua cac ham lam viec voi cdev */
#include <linux/uaccess.h>/*thu vien dang ky entry point read & write*//* thu vien chua cac ham trao doi du lieu giua user va kernel */
#include <linux/fs.h>/* thu vien nay dinh nghia cac ham cap phat/giai phong device number */

#define MEM_SIZE 1024

char *kernel_buffer;
char XauRo[MEM_SIZE], XauMa[MEM_SIZE], XauGiaiMa[MEM_SIZE];//biến để lưu giữ chuỗi ký tự:
int i,len, key, choice;
char x;

struct vchar_drv
{
	dev_t dev_num;/*Một biến kiểu dev_t để lưu giữ device number mà device được cấp phát,  Chứa device number; */
	struct class *dev_class;
	struct device *dev;
	struct cdev *vcdev; /* cdev là cấu trúc biểu diễn char devices; */
}driver_module;

//my_open:khi tien trinh tren user buffer goi system call open-> dc goi de ktra thiet bi da san sang chua, khoi tao thiet bi neu can, luu lai munor number...
static int my_open(struct inode *inode, struct file *filp);

//khi tien trinh tren user buffer goi system call close-> dc goi de tat tbi, lam thie bi ngung hoat dong....
static int my_release(struct inode *inode, struct file *filp);

//doc du lieu tu buffer cua char device vao kernel buffer -> sao chep dlieu kernel buffer vao user buffer cua tien trinh
static ssize_t my_read(struct file *filp, char __user *user_buf,size_t len, loff_t *off);

//sao chep du lieu tu buffer cua user buffer vao kernel buffer -> ghi dlieu kernel buffer vao buffer cua char device
static ssize_t my_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off);


/*Mã hóa cũng có thể được biểu diễn thông qua số học mô đun, bằng cách gán các ký tự bằng các con số, theo tuần tự, A → 0, B → 1,..., Z → 25. 
Mã hóa một chữ cái x bằng phép dịch chuyển n vị trí có thể mô tả bằng biểu thức toán học sau:
En(x)=(x+n)\mod {26}.
Giải mã được mô tả tương tự,
Dn(x)=(x-n)\mod {26}
*/
void MaHoa(char XauRo[MEM_SIZE],char XauMa[MEM_SIZE], int key);
void GiaiMa(char XauMa[MEM_SIZE],char XauGiaiMa[MEM_SIZE], int key);

static struct file_operations fops = {
	.owner  = THIS_MODULE,
	.read   = my_read,
	.write  = my_write,
	.open   = my_open,
	.release= my_release,
};

int my_open(struct inode *inode, struct file *filp)
{
	pr_info("Driver: open\n");
	return 0;
}

int my_release(struct inode *inode, struct file *filp)
{
	pr_info("Driver: close\n");
	return 0;
}


//sử dụng hàm copy_to_user để sao chép dữ liệu từ kernel buffer vào user buffe
ssize_t my_read(struct file *filp, char __user *user_buf, size_t len, loff_t *off)
{
	if(choice == 2) {
		copy_to_user(user_buf, XauMa, MEM_SIZE);
	}
	if(choice == 3 )
	{
		copy_to_user(user_buf, XauGiaiMa, MEM_SIZE);
	}
	return MEM_SIZE;
}

ssize_t my_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off)
{	
	copy_from_user(kernel_buffer, user_buf, len);
	choice = *kernel_buffer;
	if(choice == 1)
	{
		i = 0;
		kernel_buffer++;
		while(*kernel_buffer != '\0') {
			XauRo[i++] = *kernel_buffer;
			kernel_buffer++;
		}
		XauRo[i] = '\0';
	}
	
	if(choice == 2)//encrypt
	{
		kernel_buffer++;
		key = *kernel_buffer;
		key = key - 22;
		MaHoa(XauRo,XauMa,key);
	}
	if(choice == 3)//decrypt
	{
		kernel_buffer++;
		key = *kernel_buffer;
		key = key - 22;
		GiaiMa(XauMa,XauGiaiMa,key);
	}
	return len;
}

void MaHoa(char XauRo[MEM_SIZE],char XauMa[MEM_SIZE], int key)
{
	len = strlen(XauRo);// do dai xau
	for(i = 0; i < len; i++)
	{
		if(isalpha(XauRo[i]))//check chu
		{
			if(isupper(XauRo[i]))// la chu hoa
			{
				XauMa[i] = ((((XauRo[i] - 65) + key) % 26) + 65);
			}
			else if (islower(XauRo[i]))//la chu thuong
			{
				XauMa[i] = ((((XauRo[i] - 97) + key) % 26) + 97);
			}
		}
		else 
		{
			XauMa[i] = XauRo[i];
		}
	}
}
void GiaiMa(char XauMa[MEM_SIZE],char XauGiaiMa[MEM_SIZE], int key)
{
	len = strlen(XauMa);// do dai xau
	for(i = 0; i < len; i++)
	{
		if(isalpha(XauMa[i]))//check chu
		{
			if(isupper(XauMa[i]))// la chu hoa
			{
				XauGiaiMa[i] = (((((XauMa[i] - 65) + 26) - key) % 26) + 65);
			}
			else if (islower(XauRo[i]))//la chu thuong
			{
				XauGiaiMa[i] = (((((XauMa[i] - 97) + 26) - key) % 26) + 97);
			}
		}
		else 
		{
			XauGiaiMa[i] = XauMa[i];
		}
	}
}

//Ham khoi tao driver
static int driver_module_init(void)
{
	int ret = 0;
	
	/* cap phat dong device number. Va gan device number voi ten device truyen vao*/
	driver_module.dev_num = 0;
	
	pr_info("Bat dau driver_module\n");
	ret =  alloc_chrdev_region(&driver_module.dev_num,0,1,"driver_module");
	if(ret < 0) {
		printk("Can't allocate character driver\n");
		goto failed_register_devnum;
	}
	printk("Insert character driver successfully. major(%d), minor(%d)\n", MAJOR(driver_module.dev_num), MINOR(driver_module.dev_num));
	
	/* tao device file /dev/tenFileDevice */
	driver_module.dev_class = class_create(THIS_MODULE, "driver_module_class");
	if(IS_ERR(driver_module.dev_class)) {
		printk("Can't create class\n");
		goto failed_create_class;
	}
	
	driver_module.dev = device_create(driver_module.dev_class, NULL, driver_module.dev_num, NULL, "driver_module_device");
	if(IS_ERR(driver_module.dev)) {
		printk("Can't create device file\n");
		goto failed_create_device;
	}
	
	kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL);/*kmalloc:phân bổ bộ nhớ có kích thước nhỏ hơn kích thước page trong kernel, Bộ nhớ được phân bổ bởi kmalloc() nằm trong lowmem và nó tiếp giáp về mặt vật lý, GFP_KERNEL: phân bổ Ram bình thường*/
	
	driver_module.vcdev = cdev_alloc();/*cdev_alloc: cap phat bo nho cho cau chuc cdev*/
	cdev_init(driver_module.vcdev, &fops);/*KHoi tao cac truong cau truc cho cdev*/
	cdev_add(driver_module.vcdev, driver_module.dev_num,1);/*dang ky cau chuc cdev vs kernel, lke cau truc vs device file*/
	return 0;
	
failed_create_device:
	//huy ham tuong ung voi class_create
	class_destroy(driver_module.dev_class);
failed_create_class:
	//giai phong driver number
	unregister_chrdev_region(driver_module.dev_num, 1);
failed_register_devnum:
	return ret;
}

/* ham giai phong thiet bi */
static void driver_module_exit(void)
{
	//huy dky xu ly ngat
	
	//huy dky entry point vs kernel
	cdev_del(driver_module.vcdev);/* huy bo vung nho  cua cau truc cdev*/
	kfree(kernel_buffer);/* giai phong bo nho da cap phat */
	/* xoa bo device file */
	device_destroy(driver_module.dev_class, driver_module.dev_num);//huy ham tuong ung voi driver_create
	class_destroy(driver_module.dev_class);//huy ham tuong ung voi class_create
	
	unregister_chrdev_region(driver_module.dev_num, 1);//giai phong driver number
	pr_info("ket thu driver_module \n");
}



module_init(driver_module_init);
module_exit(driver_module_exit)

MODULE_LICENSE("GPL");/* giay phep su dung cua module */
