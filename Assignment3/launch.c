#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<pwd.h>
#include<signal.h>
#define size 50
#define BUFSIZE_RL 1024
#define BUFSIZE_TOK 1024
#define DELIM " \t\r\n\a"
#define DELIM2 ";"
#define DELIM1 "&"

struct proc
{
	int status;
	char name[50];
	pid_t pid;

}proc[size];
int cur=0;
int launch(char **args, int towait)
{
	int status;
	pid_t pid;
//	int shellpid;
	int pid1;
	pid = fork();
	if (pid == 0)
	{
		pid1 = getpid();
		if(towait == 0)
		{
			fprintf(stdout, "ProcessId: %d\n", getpid());
				if(execvp(args[0], args) == -1)
				{
					perror("");
				}
				exit(1);
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
		else
		{
			strcpy(proc[cur].name, args[0]);
			proc[cur].pid = pid;
			proc[cur].status = 1;
			cur++;
		}
	}
	return 1;
}


