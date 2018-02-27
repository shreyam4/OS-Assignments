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

int cd(char **args);
int pwd(char **args);
int echo(char **args);
int Exit(char **args);
int launch(char **args, int towait);
int execute(char **args, int towait)
{
	int i;
	if(args[0] == NULL)
	{
		return 1;
	}

	if(strcmp(args[0], "cd") == 0)
	{
		return (cd)(args);
	}
	else if(strcmp(args[0], "pwd") == 0)
	{
		return (pwd)(args);
	}
	else if(strcmp(args[0], "echo") == 0)
	{
		return(echo)(args);
	}
	else if(strcmp(args[0], "Exit") == 0)
	{
		return(Exit)(args);
	}
	return launch(args, towait);
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


void loop(void)
{
	char *line = NULL;
	ssize_t bufsize = 0;
	char **args;
	char **args1;
	char **args2;
	int status = 1;
	char buf1[1024];
	char init_pwd[1024];
	char pwd[1024];
	char buf2[1024];
	char r[2] = "~";
	int val,i,j,towait;
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
		getline(&line, &bufsize, stdin);
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
		while(args1[i] != NULL)
		{
			args2 = split_line(args1[i], DELIM);
			status = execute(args2, towait);
			i++;
			free(args2);
		}

		free(line);
		free(args1);
		free(args);
		sleep(0.2);
	}	
	while(status);
}
int main(int argc, char **argv)
{
	loop();
	return 0;
}

