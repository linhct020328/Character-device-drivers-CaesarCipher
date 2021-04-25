#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*định dạng mảng ký tự, sd strlen*/
#include <sys/types.h>/*thư viện : kiểu dữ liệu*/
#include <sys/stat.h>/*trong thư viện C POSIX cho ngôn ngữ lập trình C có cấu trúc tạo điều kiện nhận thông tin về các thuộc tính tệp.*/
#include <fcntl.h> /* xác định tùy chọn file control*/
#include <unistd.h>/*read()/write() là một lời gọi tới hệ thống nằm trong thư viện mà lập trình viên hệ thống nào cũng hay sử dụng,
				dùng để đọc dữ liệu từ file vật lý hoặc ghi dữ liệu ra file vật lý.*/

#define DEVICE_NODE "/dev/driver_module_device"

#define MEM_SIZE 1024 // số byte cần ghi

// _buf là con trỏ tới bộ đệm để chứa dữ liệu đọc được hoặc chứa dữ liệu cần ghi
char write_buf[MEM_SIZE];
char read_buf[MEM_SIZE];
int key, i ;

int main()
{
	int fd;
	char opt;
	printf("**************************************************\n");
	printf("*******user application to test char driver*******\n");
        
        //mo file thiet bi
        fd = open(DEVICE_NODE, O_RDWR);//fd là file descriptor (đặc tả file)
        
        if(fd < 0)
        {
        	printf("Cannot open device file ... \n");
        	return 0;
        }
        
        
        while(1)
        {
        	printf("\n*********Please Enter the Option*******\n");
        	printf("*        1. Nhap vao mot xau.		*\n");
        	printf("*        2. Encrypt Caesar and save.	*\n");
        	printf("*        3. Read and decrypt Caesar.	*\n"); 
        	printf("*        4. Exit.			*\n");
        	printf("***************************************\n");
        	
        	scanf("%c", &opt);
        	printf("\nYou Option: %c\n", opt);
        	
        	switch(opt)
        	{
        		case '1':
        			*write_buf = 1;
        			printf("\nNhap xau ro: ");
        			scanf("  %[^\t\n]s", write_buf + 1);
        			write(fd, write_buf, strlen(write_buf) + 1);
        			break;
        		case '2':
        			*write_buf = 2;
				printf("\nNhap key de ma hoa: ");
				scanf(" %s", write_buf + 1);
				write(fd, write_buf, strlen(write_buf) + 1);
				read(fd, read_buf, MEM_SIZE);
				printf("\nXau sau khi ma hoa ---->> %s\n",read_buf);
        			break;
        		case '3':
        			*write_buf = 3;
        			printf("\nNhap key de giai ma: ");
        			scanf("%s", write_buf + 1);
        			write(fd, write_buf, strlen(write_buf) + 1);
        			read(fd, read_buf, MEM_SIZE);
        			printf("\nXau sau khi giai ma ---->> %s\n", read_buf);
        			break;
        		case '4':
        			close(fd);
        			exit(1);
        			break;
        		default:
        			printf("Enter Valid option = %c\n",opt);
     				break;
        	}
        }
       
	return 0;
}
