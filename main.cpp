#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <fstream>
#include "input.h"
#include "scaner.h"
#include "welcome.h"
#include "paser.h"
#include "run.h"
#include <pwd.h>
#include <signal.h>

using namespace std;

const bool DEBUG = 0;

string getdir()
{
    string dir = get_current_dir_name();
    int index = dir.rfind("/");
    return dir.substr(index , dir.size());
}



int main(void)
{
    Colour CO;

    const char* historyfile = "/tmp/.4149his";

    struct passwd *my_info;
    my_info = getpwuid(getuid());

    string username = my_info->pw_name;
    bool open = 1;
    string com;
    list<string> comhis;
    vector <string> veccom;
    vector<vector<string> > vecvecstr_com;
    bool success;

    int status = 0;

    string startdir = my_info->pw_dir;
    chdir(startdir.c_str());
    welcome();

    ifstream fin(historyfile);
    while(getline(fin, com)){
        comhis.push_back(com);
    }
    fin.close();



    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);



    while(open) {
        //get com

        cout << CO.MAGENTA <<
            "[" << username << ":" << getdir() <<"]" <<
            CO.ENDC;
        if(status == 0)
            cout << CO.GREEN << " $ " << CO.ENDC;
        else
            cout << CO.RED << " $ " << CO.ENDC;

        com = getcom(comhis);

        if(com == "")
            continue;
        comhis.push_back(com);

        //analyze the com
        success = 1;
        veccom = scaner(com, success);

        if(success)
        {
            if(DEBUG)
                cout << "scan success!" << endl;
            vecvecstr_com = paser(veccom, success);
            if(DEBUG)
                if(success)
                    cout << "paser success!" << endl;
                else
                    cout << "paser error" << endl;
        }
        else
        {
            if(DEBUG)
                cout << "scan error." << endl;
        }

        if(DEBUG)
        {

            for(vector<string>::iterator it = veccom.begin(); it != veccom.end(); ++it)
                cout << *it << " " << (*it).size() << " ";
            cout << endl;
        }

        //run
        if(success)
        {
            status = run(vecvecstr_com, startdir.c_str(), username, comhis, historyfile, open);

        }
        veccom.clear();
        vecvecstr_com.clear();

    }
    return 0;
}
