#include<unistd.h>
#include<sys/stat.h>
#include<sys/file.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
	int count=0,i=1,j=0;
	int flag=0;
	char buffer;
	char buffer1;
	char buffer2;
	struct stat fileStat;
	struct stat fileStat2;
	struct stat fs;
	char arr[1000]="output_";
	strcat(arr, argv[1]);
	int dir=stat("Assignment", &fs);
	if(!dir && S_ISDIR(fs.st_mode))
		system("echo 'Checking whether the directory has been created: Yes'");
	else
	{
		system("echo 'Checking whether the directory has been created: No'");
		return 1;
	}
	
	stat("Assignment", &fileStat2);
	system( (fileStat2.st_mode & S_IRUSR) ? "echo -n 'r'" : "echo -n '-'");
	system( (fileStat2.st_mode & S_IWUSR) ? "echo -n 'w'" : "echo -n '-'");
	system( (fileStat2.st_mode & S_IXUSR) ? "echo -n 'x'" : "echo -n '-'");
	system( (fileStat2.st_mode & S_IRGRP) ? "echo -n 'r'" : "echo -n '-'");
	system( (fileStat2.st_mode & S_IWGRP) ? "echo -n 'w'" : "echo -n '-'");
	system( (fileStat2.st_mode & S_IXGRP) ? "echo -n 'x'" : "echo -n '-'");
	system( (fileStat2.st_mode & S_IROTH) ? "echo -n 'r'" : "echo -n '-'");
	system( (fileStat2.st_mode & S_IWOTH) ? "echo -n 'w'" : "echo -n '-'");
	system( (fileStat2.st_mode & S_IXOTH) ? "echo 'x'" : "echo '-'");
	
//	system("echo '\n'");
	int fd1 = open(argv[1], O_RDONLY);

	chdir ("Assignment");
	int fd2 = open(arr, O_RDONLY);
	if(fd2 == -1)
	{
		system("echo 'Checking whether the file has been created: No'");
		return 1;
	}
	else
		system("echo 'Checking whether the file has been created: Yes'");

	stat(arr, &fileStat);
	while(read(fd1, &buffer, 1)>0)
	{
		count++;
	}
	while(++i <= count)
	{
		lseek(fd1, -i, SEEK_END);
		lseek(fd2, j, SEEK_SET);
		read(fd1, &buffer1, 1);
		read(fd2, &buffer2, 1);
		if(buffer1 != buffer2)
			flag=1;
		//i++;
		j++;
	}
	if(flag == 0)
		system("echo 'Checking whether file contents have been reversed: Yes'");
	else
		system("echo 'Checking whether file contents have been reversed: No'");
	system( (fileStat.st_mode & S_IRUSR) ? "echo -n 'r'" : "echo -n '-'");
	system( (fileStat.st_mode & S_IWUSR) ? "echo -n 'w'" : "echo -n '-'");
	system( (fileStat.st_mode & S_IXUSR) ? "echo -n 'x'" : "echo -n '-'");
	system( (fileStat.st_mode & S_IRGRP) ? "echo -n 'r'" : "echo -n '-'");
	system( (fileStat.st_mode & S_IWGRP) ? "echo -n 'w'" : "echo -n '-'");
	system( (fileStat.st_mode & S_IXGRP) ? "echo -n 'x'" : "echo -n '-'");
	system( (fileStat.st_mode & S_IROTH) ? "echo -n 'r'" : "echo -n '-'");
	system( (fileStat.st_mode & S_IWOTH) ? "echo -n 'w'" : "echo -n '-'");
	system( (fileStat.st_mode & S_IXOTH) ? "echo 'x'" : "echo '-'");

//	system("echo '\n'");
	close(fd1);
	close(fd2);
	return 0;
}

