#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv){
	int fd;
	int status;

	/*判断参数*/
	if(argc != 3){
		printf("%s\n", "参数错误");
		return -1;
	}
	/*打开文件*/
	fd = open(argv[1], O_RDWR);
	if (fd == -1){
		printf("can not open file %s\n", argv[1]);
		return -1;
	}
	
	/* 3. 写文件或读文件 */
	if (0 == strcmp(argv[2], "on")){
		status = 1;
		write(fd, &status, 1);
	}else if(0 == strcmp(argv[2], "off")){
		status = 0;
		write(fd, &status, 1);
	}else{
		printf("%s\n", "请输入正确的命令");
	}
	
	close(fd);
	return 0;
}
