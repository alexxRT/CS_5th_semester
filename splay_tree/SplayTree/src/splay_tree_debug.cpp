#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "splay_tree_debug.h"

const int CMD_SIZE = 200;

void call_display(const char* file_png, const char* file_gv) {
    char* command = (char*)calloc(CMD_SIZE, sizeof(char));

    //starting building commands
    //create png:
    size_t offset = 0;
    strncpy(command + offset, "dot ", strlen("dot "));
    offset += strlen("dot ");

    strncpy(command + offset, file_gv, strlen(file_gv));
    offset += strlen(file_gv);

    strncpy(command + offset, " -Tpng -o ", strlen(" -Tpng -o "));
    offset += strlen(" -Tpng -o ");

    strncpy(command + offset, file_png, strlen(file_png));
    offset += strlen(file_png);

    std::cout << "Test " << command << "\n";
    system(command);

    bzero(command, offset);
    offset = 0;

    //rm .gv system dump file
    strncpy(command + offset, "rm ", strlen("rm "));
    offset += strlen("rm ");

    strncpy(command + offset, file_gv, strlen(file_gv));
    offset += strlen(file_gv);

    std::cout << "Test " << command << "\n";
    system(command);

    bzero(command, offset);
    offset = 0;

    //display the image on a screen
    strncpy(command + offset, "open ", strlen("open "));
    offset += strlen("open ");

    strncpy(command + offset, file_png, strlen(file_png));
    offset += strlen(file_png);

    std::cout << "Test " << command << "\n";
    system(command);

    free(command);
}
