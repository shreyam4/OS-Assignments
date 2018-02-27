#include<unistd.h>
#include<sys/stat.h>
//#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
int main(int argc, char *argv[])
{
	int count=0,i=1;
	char buffer;
	char arr[1000] = "output_";
	strcat(arr, argv[1]);
//	printf("%s\n", arr);
	int val = mkdir("Assignment", 0711);
	int fd1 = open(argv[1], O_RDONLY);
	chdir("Assignment");
	int fd2 = open(arr, O_CREAT|O_WRONLY|O_TRUNC, 0600);
	while(read(fd1, &buffer, 1)>0)
	{
		count++;
	}
	while(++i <= count)
	{
		lseek(fd1, -i, SEEK_END);
		read(fd1, &buffer, 1);
		write(fd2, &buffer, 1);
	}
	close(fd1);
	close(fd2);
}

