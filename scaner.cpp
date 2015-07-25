#include "scaner.h"

const bool DEBUG = 0;
//set to 1 to open the degub mode.


bool is_letter(char c)
{
    if((c <= '9' && c >= '0') || (c <= 'z' && c >= 'a') ||
            (c <= 'Z' && c >= 'A') || c == '-' || c == '*' ||
            c == '.' || c == '_' || c == '/' || c == '~' ||
            c == '\\' )
        return 1;
    return 0;
}

vector<string> scaner(string com, bool & success)
{
    vector<string> answer;
    enum Stat {q0, q1, q2, q3, q4, q5, q6, q7};
    Stat stat = q0;
    string bufstr = "";
    success = 1;
    for(int i = 0; i < com.size(); i++)
    {
	if(!success)
	    break;

        switch (stat) {
            case q0:
                if(DEBUG)
                    cout << "q" << stat << endl;
                if(is_letter(com[i])){
                    bufstr += com[i];
                }
                else if(com[i] == '&'){
                    stat = q3;
                    if(bufstr != "")
                        answer.push_back(bufstr);
                    bufstr = "";
                    answer.push_back("&");
                }
                else if(com[i] == ' '){
                    stat = q1;
                    if(bufstr != "")
                        answer.push_back(bufstr);
                    bufstr = "";
                }
                else if(com[i] == '|' || com[i] == '<'){
                    stat = q2;
                    if(bufstr != "")
                        answer.push_back(bufstr);
                    bufstr = com[i];
                    answer.push_back(bufstr);
                    bufstr = "";
                }
                else if(com[i] == '>'){
                    stat = q4;
                    if(bufstr != "")
                        answer.push_back(bufstr);
                    bufstr = "";
                }
                else{
                    cout << "error in " << i << " "<< com[i] << endl;
                    success = 0;
                }
                break;
            case q1:
                if(DEBUG)
                    cout << "q" << stat << endl;
                if(is_letter(com[i])){
                    stat = q0;
                    bufstr += com[i];
                }
                else if(com[i] == '&'){
                    stat = q3;
                    answer.push_back("&");
                }
                else if(com[i] == ' '){
                    //do notthing here.
                }
                else if(com[i] == '|' || com[i] == '<'){
                    stat = q2;
                    bufstr = com[i];
                    answer.push_back(bufstr);
                    bufstr = "";
                }
                else if(com[i] == '>'){
                    stat = q4;
                }
                else if(com[i] == '\''){
                    stat = q6;
                }
                else{
                    cout << "error in " << i << " "<< com[i] << endl;
                    success = 0;
                    //return 1;
                }
                break;
            case q2:
                if(DEBUG)
                    cout << "q" << stat << endl;
                if(is_letter(com[i])){
                    stat = q0;
                    bufstr += com[i];
                }
                else if(com[i] == ' '){
                    //do notthing here
                }
                else{
                    cout << "error in " << i << " "<< com[i] << endl;
                    success = 0;
                    //return 1;
                }
                break;
            case q3:
                if(com[i] == ' '){
                    //do notthing here
                }
                else{
                    cout << "error in " << i << " "<< com[i] << endl;
                    success = 0;
                }
                break;
            case q4:
                if(DEBUG)
                    cout << "q" << stat << endl;
                if(is_letter(com[i])){
                    answer.push_back(">");
                    stat = q0;
                    bufstr += com[i];
                }
                else if(com[i] == ' '){
                    answer.push_back(">");
                    stat = q2;
                }
                else if(com[i] == '>'){
                    stat = q5;
                    answer.push_back(">>");
                }
                else{
                    cout << "error in " << i << " "<< com[i] << endl;
                    success = 0;
                    //return 1;
                }
                break;
            case q5:
                if(DEBUG)
                    cout << "q" << stat << endl;
                if(is_letter(com[i])){
                    stat = q0;
                    bufstr += com[i];
                }
                else if(com[i] == ' '){
                    //do notthing here
                }
                else{
                    cout << "error in " << i << " "<< com[i] << endl;
                    success = 0;
                    //return 1;
                }
                break;
            case q6:
                if(DEBUG)
                    cout << "q" << stat << endl;
                if(com[i] == '\'')
                {
                    stat = q7;
                    if(bufstr != "")
                        answer.push_back(bufstr);
                    bufstr = "";
                }
                else{
                    bufstr += com[i];
                }
                break;
            case q7:
                if(DEBUG)
                    cout << "q" << stat << endl;
                if(com[i] == ' ')
                {
                    stat = q1;
                }
                else{
                    cout << "error in " << i << " "<< com[i] << endl;
                    success = 0;
                }
                break;

        }
    }
    if (bufstr != "")
        answer.push_back(bufstr);
    if(stat == q4)
        answer.push_back(">");
    return answer;
}

/*
int main(void)
{
    string com;
    vector <string> answer;
    bool suc;
    while(1){

        suc = 1;

        getline(cin, com);
        answer = paser(com, suc);

        cout << answer.size() << " " << suc << endl;
        for(vector<string>::iterator it = answer.begin(); it != answer.end(); ++it)
            cout << *it << " ";
        cout << endl;
        answer.clear();
    }
    return 0;
}
*/
