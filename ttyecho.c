/*
This is a very handy utility to send commands to another tty.
I have not written this code, I just found it here:
http://www.humbug.in/2010/utility-to-send-commands-or-data-to-other-terminals-ttypts/

I was looking for some way to emulate the workflow of emacs in
vim (sending region or buffer to the REPL...) and this piece of code, a
tiling window manager and two xterm windows almost do the trick.

-- compile, setuid, install --
% cc -o ttyecho ttyecho.c
# chown root:wheel ttyecho
# chmod 4550 ttyecho # very dangerous setuid practice...
# mv ttyecho /usr/local/sbin

-- usage --
get the name of the terminal you want to send command to with `tty`
% ttyecho -n /dev/pts/1 <cmd>
send a command to tty1 from vim:
:command! -nargs=* A :silent !ttyecho -n /dev/pts/1 "<args>"
:A <cmd>
*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

void print_help(char *prog_name) {
        printf("Usage: %s [-n] DEVNAME COMMAND\n", prog_name);
        printf("Usage: '-n' is an optional argument if you want to push a new line at the end of the text\n");
        printf("Usage: Will require 'sudo' to run if the executable is not setuid root\n");
        exit(1);
}

int main (int argc, char *argv[]) {
    char *cmd, *nl = "\n";
    int i, fd;
    int devno, commandno, newline;
    int mem_len;
    devno = 1; commandno = 2; newline = 0;
    if (argc < 3) {
        print_help(argv[0]);
    }
    if (argc > 3 && argv[1][0] == '-' && argv[1][1] == 'n') {
        devno = 2; commandno = 3; newline=1;
    } else if (argc > 3 && argv[1][0] == '-' && argv[1][1] != 'n') {
        printf("Invalid Option\n");
        print_help(argv[0]);
    }
    fd = open(argv[devno],O_RDWR);
    if(fd == -1) {
        perror("open DEVICE");
        exit(1);
    }
    mem_len = 0;
    for ( i = commandno; i < argc; i++ ) {
        mem_len += strlen(argv[i]) + 2;
        if ( i > commandno ) {
            cmd = (char *)realloc((void *)cmd, mem_len);
        } else { //i == commandno
            cmd = (char *)malloc(mem_len);
        }

        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }
  if (newline == 0)
        usleep(225000);
    for (i = 0; cmd[i]; i++)
        ioctl (fd, TIOCSTI, cmd+i);
    if (newline == 1)
        ioctl (fd, TIOCSTI, nl);
    close(fd);
    free((void *)cmd);
    exit (0);
}

