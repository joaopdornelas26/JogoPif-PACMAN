#include "keyboard.h"
#include <termios.h>
#include <unistd.h>
#include <stdio.h> 

static struct termios initialSettings, newSettings;

void keyboardInit() {
    tcgetattr(0, &initialSettings);
    newSettings = initialSettings;
    newSettings.c_lflag &= ~ICANON;
    newSettings.c_lflag &= ~ECHO;
    newSettings.c_cc[VMIN] = 1;
    newSettings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &newSettings);
}

void keyboardDestroy() {
    tcsetattr(0, TCSANOW, &initialSettings);
}

int keyhit() {
    struct termios tempSettings;
    tcgetattr(0, &tempSettings);
    tempSettings.c_lflag &= ~ICANON;
    tempSettings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &tempSettings);
    int ch = getchar();
    tcsetattr(0, TCSANOW, &initialSettings);
    return ch != EOF;
}

int readch() {
    return getchar();
}
