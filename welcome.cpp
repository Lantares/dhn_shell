#include "welcome.h"

void welcome()
{
    Colour CO;
    cout << endl;
    srand((unsigned)time(NULL));
    int lucknum = rand()%5000;
    string line;
    //cout << lucknum << endl;

    if(lucknum == 4149){
        ifstream in;
        in.open("badapple");
        if(in){
            while (getline (in, line)){
                cout << line << endl;
            }
        }
        in.close();
    }

    cout << CO.MAGENTA << CO.LINE << CO.ENDC << endl;
    cout << CO.BLUE;
    cout << CO.TAB(1) << "welcome to shell" << endl << endl;
    cout << CO.TAB(1) << "built-in command" << endl;
    cout << CO.ENDC;

    cout << CO.OKBLUE;
    cout << CO.TAB(1) << "Use `cd` to change dir." << endl;
    cout << CO.TAB(1) << "Input up and down or use command `history` to use history." << endl;
    cout << CO.TAB(1) << "Use command `exit` to exit the shell." << endl;
    cout << CO.TAB(1) << "Use `status` to check the return value of the last command." << endl;
    cout << CO.TAB(1) << "Use `jobs` to view the jobs list." << endl;
    cout << CO.TAB(1) << "Use `control+z` to suspend the process." << endl;
    cout << CO.TAB(1) << "Use `fg` to continue the jobs. Use `bg`" << endl;
    cout << CO.TAB(1) << "Use `bg` to run it in the background." << endl << endl;
    cout << CO.ENDC;

    cout << CO.BLUE;
    cout << CO.TAB(1) << "others" << endl;
    cout << CO.ENDC;

    cout << CO.OKBLUE;
    cout << CO.TAB(1) << "Use `com &` to run a job in background." << endl;
    cout << CO.TAB(1) << "You can check your job in the jobs list." << endl;

    cout << CO.TAB(1) << "have fun~" << endl;
    cout << CO.ENDC;
    cout << CO.MAGENTA << CO.LINE << CO.ENDC << endl;
    cout << endl;
}

/*
int main(void)
{
    //while(1){
    //  sleep(1);
    eggs();
    //}
    return 0;
}
*/
