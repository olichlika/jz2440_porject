#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define OLED_CLEAR 0

/* 
* 格式为：i2c_oled_test string
*/

void print_usage(char *file)
{
	printf(" %s string \n\r",file);
}

int main(int argc,char **argv) {
	char str1[20];
	int fd; 
	unsigned char buf[64] = {0};
	int i;

	fd = open("/dev/oled",O_RDWR);

	
	if(fd < 0){
		printf(" can't open /dev/oled \n\r");
		return -1;
	}

// /*
// 	while(argv[1][i] != '\0'){
// 		buf[i] = argv[1][i];
// 		i++;
// 	}
// */

	//write(fd, argv[1], 64);
	while(1) {
		scanf("%s",str1);
		//printf("%s\n",str1);
		ioctl(fd, OLED_CLEAR);
		write(fd, str1, 64);
	}

	return 0;

}
