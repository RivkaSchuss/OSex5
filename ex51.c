//Rivka Schuss 340903129

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <signal.h>

#define ERROR "Error in system call.\n"
#define TETRIS "./draw.out"
#define TRUE 1
#define FALSE 0
#define STD_ERROR 2

/**
 * writes an error
 */
void error() {
    write(STD_ERROR, ERROR, sizeof(ERROR));
    exit(-1);
}

/**
 * gets the char entered
 * @return the char entered
 */
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

/**
 * returns true if the key entered was a game key
 * @param c the key entered
 * @return true if the char was a key, false if not
 */
int gameKey(char c) {
    if (c == 'a' || c == 'd' || c == 's' || c == 'w' || c == 'q') {
        return TRUE;
    } else {
        return FALSE;
    }
}


/**
 * the program main
 * @return
 */
int main() {
    int fileDes[2], pid;
    pipe(fileDes);

    pid = fork();
    if (pid  < 0) {
        error();
    }
    if (pid == 0) {
        //in child
        dup2(fileDes[0], 0);
        execlp(TETRIS, TETRIS, NULL);
        error();
    }

    //in father
    char c;
    while (TRUE) {
        c = getch();
        if (!gameKey(c)) {
            continue;
        }
        if (write(fileDes[1], &c, 1) < 0) {
            error();
        }
        kill(pid, SIGUSR2);
        if (c == 'q') {
            break;
        }
    }
    return 0;
}



