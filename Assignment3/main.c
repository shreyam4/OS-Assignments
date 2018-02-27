#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<pwd.h>
#include<signal.h>
#include<syscall.h>
#include<sys/types.h>
#include<sys/file.h>
#include<fcntl.h>
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

void replace(char *str, char *orig, char *rep)
{
	char buffer[4096];
	char temp[4096];
	char *p;
	
	strcpy(temp, str);
	if(!(p = strstr(temp, orig)))
		return ;
	sprintf(buffer," %s%s", rep, p + strlen(orig));
	sprintf(str,"%s", buffer);
}

pid_t shellpid;
int std_out;
int std_in;
int cur;
char **redirect(char **args);
int killallbg();
int fg(char **args);
int kjob(char **args);
int cd(char **args);
int pwd(char **args);
int echo(char **args);
int quit(char **args);
int jobs(char **args);
int launch(char **args, int towait);
int piping(char **args, int towait);
int execute(char **args, int towait)
{
	char **args1;
	std_out = dup(1);
	std_in = dup(0);
	args1=redirect(args);
	int i;
	if(args1[0] == NULL)
	{
		return 1;
	}
	if(strcmp(args1[0], "cd") == 0)
	{
		return (cd)(args1);
	}
	else if(strcmp(args1[0], "pwd") == 0)
	{
		return (pwd)(args1);
	}
	else if(strcmp(args1[0], "echo") == 0)
	{
		return(echo)(args1);
	}
	else if(strcmp(args1[0], "jobs") == 0)
	{
		return (jobs)(args1);
	}
	else if(strcmp(args1[0], "quit") == 0)
	{
		return(quit)(args1);
	}
	else if(strcmp(args1[0], "kjob") == 0)
	{
		return(kjob)(args1);
	}
	else if(strcmp(args1[0], "killallbg") == 0)
	{
		return(killallbg)();
	}
	else if(strcmp(args1[0], "fg") == 0)
	{
		return(fg)(args1);
	}
	//std_out = dup(1);
	//dup2(std_out, 1);
	int test = launch(args1, towait);
	dup2(std_out,1);
	dup2(std_in, 0);
	return test;
	//printf("aaaa");
}

void sigchld()
{
	int status,i;
	pid_t pid=1;
	
	while(pid > 0)
	{
		pid = waitpid(-1, &status, WNOHANG);
		if(pid > 0)
		{
			for(i=0; proc[i].pid!=pid && i<cur; i++);
			if(WIFEXITED(status))
			printf("%s with pid %d exited normally\n",proc[i].name,pid);
			else if(WIFSIGNALED(status))
			printf("%s with pid %d killed by signal\n",proc[i].name,pid);
			proc[i].status = 0;
		}
	}
	return;
}
char **split_line(char *line, char *delim)
{
	int bufsize = BUFSIZE_TOK, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	token = strtok(line, delim);
	while(token != NULL)
	{
		tokens[position] = token;
		position++;	
		token = strtok(NULL, delim);
	}
	tokens[position] = NULL;
	return tokens;
}

void sigint()
{
	if(getpid() != shellpid)
		kill(getpid(),SIGKILL);
}
void loop(void)
{
	char *line = NULL;
	ssize_t bufsize = 0;
	char **args;
	char **args1;
	char **args2;
	char **args3;
	int status = 1;
	char buf1[1024];
	char init_pwd[1024];
	char pwd[1024];
	char buf2[1024];
	char r[2] = "~";
	int n,val,i,j,towait,co;
	struct passwd *pw;
	getcwd(init_pwd, 1024);
	do
	{
		i=0;
		j=0;
		val = getuid();
		pw = getpwuid (val);
		gethostname(buf2, 1024);
		if(getcwd(pwd, 1024) != NULL)
		{
			replace(pwd, init_pwd, r);
			printf("<%s@%s:%s> ",pw->pw_name,buf2,pwd);
		}
		line=NULL;
		if(getline(&line, &bufsize, stdin) < 0)
		{
			status = 0;
			break;
		}
		args = split_line(line, DELIM1);
		while(args[j] != NULL)
		{
			j++;
		}
		if(j>1)
			towait = 0;
		else
			towait = 1;
		args1 = split_line(args[0], DELIM2);
		i=0;
		for(co=0; args1[co] != NULL; co++)
//		while(args1[i] != NULL)
		{
			std_out = dup(1);
			std_in = dup(0);
			args3 = split_line(args1[co], "|");
			for(n=0; args3[n] != NULL; n++);
//			printf("n=%d\n",n);
			if(n > 1)
			{
				status = piping(args3, towait);
			}
		//while(args1[i] != NULL)
		//{
			else
			{
				args2 = split_line(args1[co], DELIM);
				status = execute(args2, towait);
				free(args2);
			}
			free(args3);
	//		i++;
		}
//		printf("co=%d\n",co);
		free(line);
		free(args1);
		free(args);
		sleep(0.2);
	}	
	while(status);
}
int main(int argc, char **argv)
{
	shellpid=getpid();
//	printf("%d %d\n",pid1, shellpid);
	signal(SIGCHLD, sigchld);
	signal(SIGINT,sigint);
	loop();
	return 0;
}

