#include "paser.h"

const int DEBUG = 0;

bool iscom(string s){
    if(s.find("*") && s != ">" && s != ">>" && s != "&" && s != "<")
        return 1;
    else
        return 0;
}

vector<vector <string> > paser(vector<string> com, bool & is_success)
{
    vector<vector<string> > vecvecstr_ans;
    vector<string> vecstr_buf;

    is_success = 1;

    enum Stat {q0, q1, q2, q3, q4, q5, q6};
    Stat stat = q0;

    for(vector<string>::iterator it = com.begin(); it != com.end(); ++it){

        if(!is_success)
            break;

        switch(stat){
            case q0:
                if(DEBUG)
                    cout << "paser: q" << stat << endl;
                if(!iscom(*it)){
                    cout << "error in " << *it << endl;
                    is_success = 0;
                }
                else {
                    vecstr_buf.push_back(*it);
                    stat = q1;
                }
                break;
            case q1:
                if(DEBUG)
                    cout << "paser: q" << stat << endl;
                if(*it == ">" || *it == "<" || *it == ">>"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q2;
                }
                else if(*it == "|"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q4;
                }
                else if(*it == "&"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q6;
                }
                else{
                    vecstr_buf.push_back(*it);
                    stat = q3;
                }
                break;
            case q2:
                if(DEBUG)
                    cout << "paser: q" << stat << endl;
                vecstr_buf.push_back(*it);
                stat = q5;
                break;
            case q3:
                if(DEBUG)
                    cout << "paser: q" << stat << endl;
                if(*it == ">" || *it == "<" || *it == ">>"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q2;
                }
                else if(*it == "|"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q4;
                }
                else if(*it == "&"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q6;
                }
                else{
                    vecstr_buf.push_back(*it);
                }
                break;
            case q4:
                if(DEBUG)
                    cout << "paser: q" << stat << endl;
                if(!iscom(*it)){
                    cout << "error in " << *it << endl;
                    is_success = 0;
                }
                else {
                    vecstr_buf.push_back(*it);
                    stat = q1;
                }
                break;
            case q5:
                if(DEBUG)
                    cout << "paser: q" << stat << endl;
                if(*it == "|"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q4;
                }
                else if(*it == "&"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q6;
                }
                else if(*it == ">" || *it == "<" || *it == ">>"){
                    vecvecstr_ans.push_back(vecstr_buf);
                    vecstr_buf.clear();
                    vecstr_buf.push_back(*it);
                    stat = q2;
                }
                else{
                    cout << "error in " << *it << endl;
                    is_success = 0;
                }
            case q6:
                break;
        }
    }
    vecvecstr_ans.push_back(vecstr_buf);
    return vecvecstr_ans;
}
/*
int main(void)
{
    bool is_success = 1;
    string com;
    int num;
    vector<string> vecstr;
    vector<vector<string> > vecvecstr_ans;
    cout << "paser test: input num" << endl;
    cin >> num;

    for(int i = 0; i < num; i++){
        cin >> com;
        vecstr.push_back(com);
    }

    for(vector<string>::iterator it = vecstr.begin(); it != vecstr.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;

    vecvecstr_ans = paser(vecstr,is_success);

    for(vector<vector<string> >::iterator it1 = vecvecstr_ans.begin();
            it1 != vecvecstr_ans.end(); ++it1)
    {
        for(vector<string>::iterator it2 = (*it1).begin(); it2 != (*it1).end(); ++it2)
            cout << *it2 << " ";

        cout << endl;
    }
    return 0;
}
*/
