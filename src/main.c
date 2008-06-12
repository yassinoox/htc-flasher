/* 
 * HTCFlasher
 *
 * Copyright (C) 2007-2008 Pau Oliva Fora - <pof@eslack.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3 as published
 * by the Free Software Foundation.
 *
 */

#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "main.h"

int fd;
char buf[BUFSIZE];
char rec[REC];
int DEBUG;
#ifdef WIN32
char *device = "\\\\.\\WCEUSBSH001";
#else
char *device = "/dev/ttyUSB0";
#endif

void help_show_message()
{
        fprintf(stderr, "Usage: HTCFlasher [options]\n\n");
        fprintf(stderr, " options:\n");
        fprintf(stderr, "    -X [fileanme]    extract NBH file contents\n");
        fprintf(stderr, "    -F [filename]    flash NBH file\n");
	fprintf(stderr, "    -D [device]      use another serial device (default: %s)\n", device);
        fprintf(stderr, "    -i               get device info\n");
	fprintf(stderr, "    -p               enter serial prompt (minicom/mtty like)\n");
        fprintf(stderr, "    -v               verbose mode\n");
        fprintf(stderr, "    -h               shows this help message\n\n");
}

/* main - start here */
int main(int argc, char **argv)
{
	int c;
	int info = 0;
	int flash = 0;
	int mtty = 0;
	int extract = 0;
	char *NBHfile;

	printf ("=== HTCflasher v3.0\n");
	printf ("=== Open source RUU for HTC devices\n");
	printf ("=== (c) 2007-2008 Pau Oliva Fora\n\n");
	
	if (argc < 2) {
		help_show_message();
		return 1;
	}

	while ((c = getopt(argc, argv, "F:D:X:ipvh")) != -1) {
		switch(c) {
		case 'X':
			NBHfile = optarg;
			printf ("[] Extract NBH file '%s'\n",NBHfile);
			extract = 1;
			break;
		case 'D':
			printf ("[] Using device '%s'\n",device);
			device = optarg;
			break;
		case 'F':
			NBHfile = optarg;
			printf ("[] Flash NBH file '%s'\n",NBHfile);
			flash = 1;
			break;
		case 'i':
			printf("[] Getting device info...\n");
			info = 1;
			break;
		case 'v':
			printf("[] Verbose mode enabled\n");
			DEBUG = 1;
			break;
		case 'p':
			printf("[] Serial prompt: type 'quit' to exit.\n");
			mtty = 1;
			break;
		case 'h':
			help_show_message();
			return 0;
		default:
			help_show_message();
			return 1;
		}
	}

	signal(SIGINT, (void (*)(int))control_c);

	if (extract) {
		extractNBH(NBHfile);
		return 0;
	}

	opendev();

	if (mtty) 
		prompt();

	if (info)
		getinfo();

	if (flash)
		flashNBH(NBHfile);

	close(fd);
	return 0;
}
