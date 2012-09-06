 #include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <limits.h>

struct stat fileStat;

long int list_du (char * dir_name)

{ long int sum=0;
    DIR * d;
    d = opendir (dir_name);

    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {

        struct dirent * entry;
        char * d_name;
        entry = readdir (d);

        if (! entry) {
            break;

        }

        d_name = entry->d_name;

        if (entry->d_type & DT_DIR) {

            if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0)

            {
                int path_length;
                char path[PATH_MAX];
                path_length = snprintf (path, PATH_MAX,"%s/%s", dir_name, d_name);
                int file=0;
                if (path_length >= PATH_MAX)
              {
                  fprintf (stderr, "Path length has got too long.\n");   
                  exit (EXIT_FAILURE);
                }
               if((file=open(&path,O_RDONLY)) < -1)   {
                   printf("error returning\n");
                   exit(1);
                   }

	       if(fstat(file,&fileStat) < 0) 

                {    
                    printf("error");   
                }  
  
                 sum=sum+list_du(path);
                long int div = 1024;
                long int size = fileStat.st_size;
                long int sizenew=(size)/div;
                sum+=sizenew;
                printf ("%ld\t%s\n",sum, path);
                close(file);
                //list_du (path);
            }
        }
        else {
 
          int path_length;
                char path[PATH_MAX];
                path_length = snprintf (path, PATH_MAX,"%s/%s", dir_name, d_name);
                int file=0;

            if((file=open(&path,O_RDONLY)) < -1)   {
                   printf("error returning\n");
                   exit(1);
                   }

               if(fstat(file,&fileStat) < 0)

                {
                    printf("error");
                }
              long int div = 1024;  
              long int size = fileStat.st_size;
                long int sizenew=(size)/div;
                sum+=sizenew;
  
}

    }

    if (closedir (d)) {

        fprintf (stderr, "Could not close '%s': %s\n",

                 dir_name, strerror (errno));

        exit (EXIT_FAILURE);

    }
return sum;
}

int main (int argc,char **argv)
{
    list_du(argv[1]);
    return 0;

}
