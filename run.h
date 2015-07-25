#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <glob.h>
#include <fcntl.h>
#include <sys/types.h>
#include "colour.h"
#include <cstdio>
#include <list>

using namespace std;

char * stoc(string s1);

struct JOB {
    int pid;
    int stat;
    string com;
};

int run(vector<string> com, int inorout, int num, int max, int ** p, int fileInOut, int fileid, int fileout);
int runpipe(vector<vector<string> > com);
int run(vector<vector<string> > com, const char* dir, string username, list<string> comhis, const char* hisfile, bool & open);

void sigint_handler(int sig);
void sigtstp_handler(int sig);

enum typeOfPipe {PIPE_NONE, PIPE_IN, PIPE_IAO, PIPE_OUT};

enum typeOfFIO {F_NONE, F_IN, F_OUT, F_IN_AND_OUT, F_APP, F_IN_AND_APP};

