#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<pwd.h>
#include<dirent.h>
#include<fnmatch.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/file.h>
#include<signal.h>
#include<fcntl.h>
#define DELIM " \t\r\n\a"
#define size 50
int cur;
int execute(char **args, int towait);
char **split_line(char *line, char *delim);

struct proc
{
	int status;
	char name[50];
	pid_t pid;

}proc[size];

char **redirect(char **args)
{
	int i,n,fd_out,fd_in,fd_app;
	int flagg = 0;
	char to[100],in[100],app[100];
	for(n=0; args[n]!=NULL;n++);
	for(i=0; i<n; i++)
	{
		//flagg = 0
		if(strcmp(args[i],">")==0)
		{
			//			printf("yesss\n");
			if(i<n-1)
			{
				//flagg = 1;
				strcpy(to,args[i+1]);
				fd_out = creat(to, 0644);
				if(fd_out == -1)
				{
					perror("Failed to open file");
				}
				if(dup2(fd_out,1) != 1)
				{
					perror("dup2 fail");
					printf("hiii");
				}
				close(fd_out);
			}
			args[i] = NULL;
		}

		else if(strcmp(args[i], "<")==0)
		{
			if(i<n-1)
			{
				strcpy(in,args[i+1]);
				fd_in = open(in , O_RDONLY);
				if(fd_in == -1)
				{
					perror("Failed to open file");
				}
				if(dup2(fd_in,0) == -1)
				{
					printf("biiii");
					perror("dup2 fail");
				}
				close(fd_in);
			}
			args[i] = NULL;
		}
		else if(strcmp(args[i], ">>")==0)
		{
			if(i<n-1)
			{
				strcpy(app, args[i+1]);
				fd_app = open(app, O_RDONLY | O_WRONLY | O_APPEND | O_CREAT, 0644);
				if(fd_app == -1)
				{
					perror("Failed to open file");
				}
				if(dup2(fd_app,1) != 1)
					perror("dup2 fail");
				close(fd_app);
			}
			args[i] = NULL;
		}
	}
	return args;

}

int piping(char **args, int towait)
{
	int in,status,pid;
	int std_out = dup(1);
	int std_in = dup(0);
	//		char **args3;
	char **args4;
	int p[2],n,i;
	p[0]=0;
	//		args3 = split_line(args , "|");
	for(n=0; args[n] != NULL; n++);
	printf(" n = %d\n",n);
	for(i=0; i<n; i++)
	{
		pipe(p); 
		pid = fork();
		if(pid == -1)
		{
			perror("piping");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) 
		{
			dup2(in,0);
			if(i!=n-1)
				dup2(p[1],1);	
			close(p[0]); 
			args4 = split_line(args[i], DELIM);
			status = execute(args4, towait);
			free(args4);
			exit(0);
		} 
		else
		{
			wait(&pid);
			close(p[1]);
			in = p[0];
		}
	}
	dup2(std_out,1);
	dup2(std_in, 0);
	close(p[0]);
	close(p[1]);

	return status;					
}

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
int quit(char **args)
{
	return 0;
}
void processdir(const struct dirent *dir)
{
	puts(dir->d_name);
}

int jobs()
{
	int i,c=0;
	for(i=0; i<cur; i++)
	{
		if(proc[i].status==1)
		{
			c++;
			printf("[%d] %s [%d]\n",c,proc[i].name,proc[i].pid);
		}
	}
	return 1;
}
int kjob(char **args)
{
	if(args[2] == NULL)
	{
		printf("not enough arguments\n");
		return 1;
	}
	int i,num,sig,cnt=0;
	num = atoi(args[1]);
	sig = atoi(args[2]);
	if(!num)
	{
		printf("Error");
		return 1;
	}
	for(i=0; cnt!=num && i<cur; i++)
	{
		if(proc[i].status == 1)
		{
			cnt++;
		}
	}
	if(cnt != num)
	{
		printf("No job numbered %d\n", num);
		return 1;
	}
	kill(proc[i-1].pid,sig);
	return 1;
}
int fg(char **args)
{
	int stat;
	if(args[1] == NULL)
	{
		printf("not enough arguments\n");
		return 1;
	}
	int i,num,cnt=0;
	num = atoi(args[1]);
	if(!num)
	{
		printf("Error");
		return 1;
	}
	for(i=0; cnt!=num && i<cur; i++)
	{
		if(proc[i].status == 1)
		{
			cnt++;
		}
	}
	if(cnt != num)
	{
		printf("No job numbered %d\n", num);
		return 1;
	}
	waitpid(proc[i].pid, &stat, 0);
	return 1;

}
int killallbg()
{
	int i;
	for(i=0; i<cur; i++)
	{
		if(proc[i].status == 1)
		{
			proc[i].status = 0;
			kill(proc[i].pid,9);
		}
	}
	return 1;
}

