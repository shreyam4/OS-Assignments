#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<pwd.h>
int cd(char **args)
{
	if(args[1] == NULL)
	{
		fprintf(stderr, "Expected argument to \"cd\"\n");
	}
	else
	{
		if(chdir(args[1]) != 0)
		{
			perror("");
		}
	}
	return 1;
}

int pwd(char **args)
{
	char buf[1024];
	if(getcwd(buf, 1024) != NULL)
		fprintf(stdout, "Current working dir: %s\n", buf);
	else
		perror("");
	return 1;
}
int echo(char **args)
{
	char buf[1024];
	//      scanf("%s", buf);
	printf("%s\n", args[1]);
}
int Exit(char **args)
{
	return 0;
}

