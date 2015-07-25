#include "input.h"

const bool DEBUG = 0;

string getcom(list<string> comhis)
{
    char ch;
    string a;
    fflush(stdin);
    int nowposition = 0;
    int maxsize = 0;

    vector<char> vecchar;

    Colour CO;

    list<string>::iterator it = comhis.end();
    while(ch = getch())
    {
        if(ch == '\033')
        {
            getch();
            switch(getch()) { // the real value
                case 'A':
                    //code for arrow up
                    //printf("up\n");
                    if(it != comhis.begin())
                    {
                        it--;
                        for(int i = 0; i < vecchar.size(); i++)
                            cout << " ";
                        for(int i = 0; i < vecchar.size(); i++)
                            cout << "\b";
                        for(int i = 0; i < a.size(); i++)
                            cout << "\b \b";
                        a = *it;
                        cout << a;
                        maxsize = a.size();
                        vecchar.clear();
                        nowposition = 0;
                    }
                    break;
                case 'B':
                    // code for arrow down
                    //printf("down\n");
                    if(it != comhis.end())
                    {
                        it++;
                        if(it == comhis.end())
                            it--;
                        for(int i = 0; i < vecchar.size(); i++)
                            cout << " ";
                        for(int i = 0; i < vecchar.size(); i++)
                            cout << "\b";
                        for(int i = 0; i < a.size(); i++)
                            cout << "\b \b";
                        a = *it;
                        cout << a;
                        maxsize = a.size();
                        vecchar.clear();
                        nowposition = 0;
                    }
                    break;
                case 'C':
                    // code for arrow right
                    //printf("right\n");
                    if(nowposition != 0)
                    {
                        cout << vecchar.back();
                        a += vecchar.back();
                        vecchar.pop_back();
                        nowposition--;
                    }
                    break;
                case 'D':
                    // code for arrow left
                    //printf("left\n");
                    if(nowposition != maxsize)
                    {
                        cout << "\b";
                        nowposition++;
                        vecchar.push_back(a[a.size()-1]);
                        a = a.substr(0, a.size()-1);

                        if(DEBUG)
                        {
                            cout << endl << CO.TAB(1);
                            for(vector<char>:: iterator iter = vecchar.end()-1; iter != vecchar.begin()-1; iter-- )
                            {
                                cout << CO.ARNING << *iter << CO.ENDC;
                            }
                            cout << endl;
                        }
                    }
                    break;
            }
        }
        else if(ch == '\n'){
            cout << endl;
            for(vector<char>:: iterator iter = vecchar.end()-1; iter != vecchar.begin()-1; iter--)
            {
                a += *iter;
            }
            return a;
        }
        else if(ch == 127){
            if(a.size() != 0){
                cout << "\b";
                if(vecchar.size() != 0)
                {
                    for(vector<char>:: iterator iter = vecchar.end()-1; iter != vecchar.begin()-1; iter-- )
                    {
                        cout << *iter;
                    }
                    cout << " ";
                    for(int i = 0; i < vecchar.size(); i++)
                        cout << "\b";
                }
                else {
                    cout << " ";
                }
                cout << "\b";
                a = a.substr(0, a.size()-1);
                maxsize--;
            }
        }
        else if(ch == 9){
            //nothing
        }
        else{
            maxsize++;

            cout << ch;
            if(vecchar.size() != 0)
            {
                for(vector<char>:: iterator iter = vecchar.end()-1; iter != vecchar.begin()-1; iter-- )
                {
                    cout << *iter;
                }
                for(int i = 0; i < vecchar.size(); i++)
                    cout << "\b";
            }
            a = a + ch;
        }
        fflush(stdin);
    }
}

/*
   int main(void)
   {
   list<string> comhistory;
   string com;
   while(1)
   {
   com = getcom(comhistory);
   cout << "com is " + com << endl;
   comhistory.push_back(com);

   cout << endl;
   for(list<string>::iterator it = comhistory.begin(); it != comhistory.end(); ++it)
   cout << *it << " ";

   cout << endl;
   }
   return 0;
   }
   */

int getch(void)
{
    int c=0;
    struct termios org_opts, new_opts;
    int res=0;
    //-----  store old settings -----------
    res=tcgetattr(STDIN_FILENO, &org_opts);
    assert(res==0);
    //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c=getchar();
    //------  restore old settings ---------
    res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);assert(res==0);
    return c;
}

