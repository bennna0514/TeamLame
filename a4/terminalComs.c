/*
 * Filename: TerminalComs.c (adapted)
 * Author: James K. Peckol
 * Contact: jkp@uw.washington.edu
 */

// http://man7.org/linux/man-pages/man2/open.2.html

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // for exit
#include "TCB.h"
#define MAX 1000

FILE *fp = NULL; // declare file here

int terminalComs(char *output) {
    // 1. Declare storage and data to be used
    char buffer[MAX];
    static int fd0; // for earth terminal
    static int firstTime = 0;
	
    // 2. open file for reading and writing
	//    open terminal ports for writing
	//    NOTE: first time prevents continually opening file and
	//          terminal every function call
    if (firstTime == 0) {
        fp = fopen("file.txt", "w+");
        if(!fp) {
            exit(1);
        }		 
        fd0 = open("/dev/pts/2", O_WRONLY);
	firstTime++;
	}

    // 3. error handling: check if file and terminal ports exist 
    if (fp && fd0) {
	dprintf(fd0, "\033[2J");
	dprintf(fd0, "\033[1;1H");

	// 3.1 Write data to the file
        fseek(fp, 0, SEEK_SET);	 		
        fwrite(output, strlen(output) + 1, 1, fp);

	//3.2 Read and print the data from the file
        fseek(fp, 0, SEEK_SET);	 
        fread(buffer, 1, MAX, fp);
	
        // 3.3 Transmit buffer to terminal
        dprintf(fd0, "%s\n", buffer);
		return (0);
	} else {
		fprintf(stderr, "ERROR, fp, fd0 and/or fd1 not opened correctly\n");
		exit(1);
	}
}
