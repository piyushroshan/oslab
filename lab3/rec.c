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
/* "readdir" etc. are defined here. */
#include <dirent.h>
/* limits.h defines "PATH_MAX". */
#include <limits.h>

int sum;
struct stat fileStat;

static void
list_du (const char * dir_name)
{
    DIR * d;

    /* Open the directory specified by "dir_name". */

    d = opendir (dir_name);

    /* Check it was opened. */
    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
           dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {
        struct dirent * entry;
        const char * d_name;

        /* "Readdir" gets subsequent entries from "d". */
        entry = readdir (d);
        if (! entry) {
            /* There are no more entries in this directory, so break
               out of the while loop. */
            break;
        }
        d_name = entry->d_name;
        /* Print the name of the file and directory. */
        //printf ("%s/%s\n", dir_name, d_name);

        /* See if "entry" is a subdirectory of "d". */

        if (entry->d_type & DT_DIR) {

            /* Check that the directory is not "d" or d's parent. */

            if (strcmp (d_name, "..") != 0 &&
                strcmp (d_name, ".") != 0)
            {
                int path_length;
                char path[PATH_MAX];

                path_length = snprintf (path, PATH_MAX,
                    "%s/%s", dir_name, d_name);
                int file=0;

                if (path_length >= PATH_MAX)
                {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }

                if((file=open(&path,O_RDONLY)) < -1)
                {
                    printf("error returning\n");
                    exit(1);
                }

                if(fstat(file,&fileStat) < 0) 
                {   
                    printf("error");   
                }
                long int div = 1024;
                long int size = fileStat.st_size;
                long int sizenew=size/div;
                printf ("%ld\t%s\n",sizenew, path);
                close(file);
                /* Recursively call "list_dir" with the new path. */
                list_du (path);
            }
        }
    }
    /* After going through all the entries, close the directory. */
    if (closedir (d)) {
        fprintf (stderr, "Could not close '%s': %s\n",
           dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
}

int main (int argc, char **argv)
{
    list_du(argv[1]);
    return 0;
}