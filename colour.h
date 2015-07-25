
#ifndef _COLOUR_H_
#define _COLOUR_H_

#include <iostream>

using namespace std;

class Colour {
    public:
    string HEADER;
    string OKBLUE;
    string KGREEN;
    string ARNING;
    string FAIL;
    string ENDC;
    string BOLD;
    string UNDERLINE;
    string LINE;

    string BLACK;
    string RED;
    string GREEN;
    string YELLOW;
    string BLUE;
    string MAGENTA;
    string CYAN;
    string WHITE;

    Colour()
    {
        HEADER = "\033[95m";
        OKBLUE = "\033[94m";
        KGREEN = "\033[92m";
        ARNING = "\033[93m";
        FAIL = "\033[91m";
        ENDC = "\033[0m";
        BOLD = "\033[1m";
        UNDERLINE = "\033[4m";
        LINE = " ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ";

        BLACK = "\033[30;1m";
        RED = "\033[31;1m";
        GREEN = "\033[32;1m";
        YELLOW = "\033[33;1m";
        BLUE = "\033[34;1m";
        MAGENTA = "\033[35;1m";
        CYAN = "\033[36;1m";
        WHITE = "\033[37;1m";
    }

    string TAB(int num)
    {
        string TAB = "";
        for(int i = 0; i < num; i++)
            TAB += "> ";
        return TAB;
    }

};
//Colour CO;

#endif

