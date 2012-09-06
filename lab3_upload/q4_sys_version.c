#include <stdio.h>
#include <sys/utsname.h>
#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
int main ()
{
	const double megabyte = 1024 * 1024;
	struct sysinfo si;
	sysinfo (&si);
	struct utsname u;
	uname (&u);
printf ("%s release %s (version %s) on %s\n", u.sysname, u.release,
	 u.version, u.machine);
  printf ("total RAM     : %5.1f MB\n", si.totalram / megabyte);
  printf ("free RAM      : %5.1f MB\n", si.freeram / megabyte);
  printf ("process count : %d\n", si.procs);

  return 0;
}
