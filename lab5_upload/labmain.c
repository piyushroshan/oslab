
#include <stdlib.h>	
#include <unistd.h>   // for execlp 
#include <sys/syscall.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

void error(char *message, ...)
{
	va_list args;

	va_start(args, message);
	fprintf(stderr, "error: ");
	vfprintf(stderr, message, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}





void  parse(char *line, char **argv)
{
     while (*line != '\0') {      
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';    
          *argv++ = line;         
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             
     }
     *argv = '\0';               
}


     
void  execute(char **argv)
{
     pid_t  pid;
     int    status;
     
     if ((pid = fork()) < 0) {     
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {         
          char temp[100];
          strcpy(temp,"./");
          strcat(temp,argv[0]); 
          argv[0]=temp;
          if (execvp(*argv, argv) < 0) {     
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  
          while (wait(&status) != pid)       
               ;
     }
}

     
int  main(void)
{
     char  line[1024];             
     char  *argv[64];              
     
     while (1) {                   
          printf("$");     
          gets(line);              /*   read in the command line     */
          printf("\n");
          parse(line, argv);       /*   parse the line               */
          if (strcmp(argv[0], "exit") == 0)  
               exit(0);            
          execute(argv);          
     }
     return 0;
}

