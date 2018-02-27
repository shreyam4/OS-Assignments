#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<pwd.h>
#define BUFSIZE_RL 1024
#define BUFSIZE_TOK 1024
#define DELIM " \t\r\n\a"
#define DELIM2 ";"
#define DELIM1 "&"

int launch(char **args, int towait)
{
	int status;
	pid_t pid,pid1;
	pid = fork();
	if (pid == 0)
	{
		if(towait == 0)
		{
			fprintf(stdout, "ProcessId: %d\n", getpid());
			pid1 = fork();
			if(pid1 == 0)
			{
				if(execvp(args[0], args) == -1)
				{
					perror("");
				}
				exit(1);
			}
			else
			{
				waitpid(pid1, &status, WUNTRACED);
				fprintf(stdout ,"%s exited normally\n", args[0]);
				exit(1);

			}	
		}
		else
		{
			if(execvp(args[0], args) == -1)
			{	
				perror("");
			}
			exit(1);
		}
	}
	else if(pid < 0)
	{
		perror("");
	}
	else
	{
		if(towait == 1)
		{
			waitpid(pid, &status, WUNTRACED);
		}
	}
	return 1;
}


