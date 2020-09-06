/* 
 * cyg-profile.c - CygProfiler runtime functions.
 *
 * Michal Ludvig <michal@logix.cz>
 * http://www.logix.cz/michal/devel
 *
 * cyg-profile.c
 * - Compile your program with -finstrument-functions and link 
 *   together with this code.
 * - Logging is enabled as soon as your program calls
 *   cygprofile_enable() and disabled with cygprofile_disable().
 * - Before logging was enabled you can change the name 
 *   of a logfile by calling cygprofile_setfilename().
 */

/* Hint: -finstrument-functions, no_instrument_function */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "cyg-profile.h"

#define FN_SIZE 100
#define FN_DEFAULT "/cyglog.txt"

/* Private variables.  */
static int level=0;
FILE* logfile = NULL;
static int cyg_profile_enabled=0;
static char cyg_profile_filename[FN_SIZE+1];

struct perf
{
	unsigned int function;
	unsigned int hit_count;
//	unsigned int entry_time;
	unsigned int ticks;
	unsigned int min_stack_entry_pos;
	unsigned int max_stack_entry_pos;
};
struct perf *perfdata = NULL;
int perf_position = 0;
extern int hblanks;
extern int blanks;
#define MAX_PERF 500

unsigned int depth_entry_time[100];
unsigned int depth_total_time[100];
#define MAX_LEVEL 100

#ifdef __cplusplus
extern "C" {
#endif

/* Static functions. */
static FILE* openlogfile (const char *filename)
	__attribute__ ((no_instrument_function));
static void closelogfile (void)
	__attribute__ ((no_instrument_function));

/* Note that these are linked internally by the compiler. 
 * Don't call them directly! */
void __cyg_profile_func_enter (void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));
void __cyg_profile_func_exit (void *this_fn, void *call_site)
	__attribute__ ((no_instrument_function));

#ifdef __cplusplus
};
#endif

void
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
	if (cyg_profile_enabled)
		if ((logfile) || openlogfile(cyg_profile_filename))
		{
			unsigned int address_above = ((volatile unsigned int)this_fn & 0xfffffffc) - 4;
//			printf("address above is %08x\n", address_above);
			unsigned int above_contents = *(volatile unsigned int *)address_above;
//			printf("above is %08x\n", above_contents);
			
			if (((above_contents >> 24) & 0xff) == 0xff)
			{
				*(volatile unsigned int *)address_above = (unsigned int)(perfdata + perf_position);
//				printf("above is now %08x\n", *(volatile unsigned int *)address_above);
				
				struct perf *ptr = (struct perf *)*(volatile unsigned int *)address_above;
				ptr->function = (unsigned int)this_fn & 0xfffffffe;
				ptr->hit_count++;
//				ptr->entry_time = hblanks;
				depth_entry_time[level] = hblanks;
				
				perf_position++;
				level++;
				
				if (perf_position == MAX_PERF)
				{
					printf("we\'re out of perf!\n");
					*(int *)0 = 0;
					while(1);
				}
				
				if (level == MAX_LEVEL)
				{
					printf("we\'re out of levels!\n");
					*(int *)0 = 0;
					while(1);
				}
			}
			else
			{
				volatile struct perf *ptr = (volatile struct perf *)*(volatile unsigned int *)address_above;
//				printf("hit %08x %d times\n", ptr->function, ptr->hit_count);
				ptr->hit_count++;
//				ptr->entry_time = hblanks;
				depth_entry_time[level] = hblanks;
				level++;
			}
//			while(1);
		}
}

void
__cyg_profile_func_exit (void *this_fn, void *call_site)
{
	if (cyg_profile_enabled)
		if ((logfile) || openlogfile(cyg_profile_filename))
		{
			unsigned int address_above = ((volatile unsigned int)this_fn & 0xfffffffc) - 4;
//			printf("address above is %08x\n", address_above);
			unsigned int above_contents = *(volatile unsigned int *)address_above;
//			printf("above is %08x\n", above_contents);
			
			if (((above_contents >> 24) & 0xff) == 0xff)
			{
				//err...
				printf("perf exit looks like an error\n");
				*(int *)0 = 0;
				while(1);
			}
			else
			{
				volatile struct perf *ptr = (volatile struct perf *)*(volatile unsigned int *)address_above;
//				printf("hit %08x %d times\n", ptr->function, ptr->hit_count);
//				ptr->ticks += (hblanks - ptr->entry_time);
				int total_spent = hblanks - depth_entry_time[level - 1];
				depth_total_time[level - 1] += total_spent;
				ptr->ticks += (total_spent - depth_total_time[level]);
				depth_entry_time[level] = 0;
				depth_total_time[level] = 0;
				level--;
				
				register unsigned int stack_ptr asm ("sp");
				if (stack_ptr > ptr->max_stack_entry_pos)
					ptr->max_stack_entry_pos = stack_ptr;
				if (stack_ptr < ptr->min_stack_entry_pos)
					ptr->min_stack_entry_pos = stack_ptr;
				
				if (level < 0)
				{
					printf("negative level\n");
					*(int *)0 = 0;
					while(1);
				}
			}
		}
}

void cygprofile_begin(void)
{
	if (!cyg_profile_filename[0])
		cygprofile_setfilename (FN_DEFAULT);
	if (!openlogfile (cyg_profile_filename))
	{
		//*(int *)0 = 0;
	}
		
	perfdata = (struct perf *)malloc(sizeof(struct perf) * MAX_PERF);
	
	if (perfdata == NULL)
	{
		printf("couldn\'t allocate perf data\n");
		*(int *)0 = 0;
		while(1);
	}
	
	memset(perfdata, 0, sizeof(struct perf) * MAX_PERF);
	memset(depth_entry_time, 0, sizeof(depth_entry_time));
	memset(depth_total_time, 0, sizeof(depth_total_time));
	printf("profiling has been enabled\n");
	
	int count;
	for (count = 0; count < MAX_PERF; count++)
		perfdata[count].min_stack_entry_pos = 0x0b000000 + 16 * 1024;
}

void
cygprofile_enable (void)
{	
	cyg_profile_enabled = 1;
}

void
cygprofile_disable (void)
{
	cyg_profile_enabled = 0;
}

int Sys_FileWrite (int handle, void *data, int count);

void
cygprofile_end(void)
{
	cyg_profile_enabled = 0;
	
	int count;
	for (count = 0; count < perf_position; count++)
	{
		char buf[120];
		memset(buf, 0, 120);
		
		snprintf(buf, 120, "%d %08x %d %d %d\n", perfdata[count].ticks, perfdata[count].function, perfdata[count].hit_count,
			perfdata[count].min_stack_entry_pos - 0x0b000000, perfdata[count].max_stack_entry_pos - 0x0b000000);
		fwrite(buf, strlen(buf), 1, logfile);
	}
	
	printf("perf pos is %d\n", perf_position);
	
	closelogfile();
}

int
cygprofile_isenabled (void)
{ return cyg_profile_enabled; }

int 
cygprofile_setfilename (const char *filename)
{
	char *ptr;

	if (cygprofile_isenabled ())
		return -1;

	if (strlen (filename) > FN_SIZE)
		return -2;

	ptr = strstr (filename, "%d");
	if (ptr)
	{
		size_t len;
		len = ptr - filename;
		snprintf (cyg_profile_filename, len+1, "%s", filename);
		snprintf (&cyg_profile_filename[len], FN_SIZE - len, 
			"%d", getpid ());
		len = strlen (cyg_profile_filename);
		snprintf (&cyg_profile_filename[len], FN_SIZE - len,
			"%s", ptr + 2);
	}
	else
		snprintf (cyg_profile_filename, FN_SIZE, "%s", filename);

	if (logfile)
		closelogfile ();

	return 0;
}

char *
cygprofile_getfilename (void)
{
	if (!cyg_profile_filename[0])
		cygprofile_setfilename (FN_DEFAULT);
	return cyg_profile_filename;
}

int Sys_FileOpenWrite (char *path);
void Sys_FileClose (int handle);

static FILE*
openlogfile (const char *filename)
{
	static int complained = 0;

	
	if (complained)
		return NULL;
	
	if (logfile)
		return logfile;

	FILE* file = fopen(filename, "w");
	if (file == NULL)
	{
		printf ("WARNING: Can't open logfile '%s': %s\n", 
			filename, strerror (errno));
		complained = 1;
		return NULL;
	}
	
//	setlinebuf (file);
	logfile = file;

	return file;
}

static void
closelogfile (void)
{
	if (logfile)
		fclose (logfile);
}
