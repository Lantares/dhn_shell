#include "run.h"

const bool DEBUG = 0;

vector<JOB> vecJ;
string jobname;
int iB;

int needSig;
int nowcpid;

void sigtstp_handler(int sig)
{
    if(needSig == 1)
    {
        if(DEBUG)
            cout << "!!" << endl;
        kill(nowcpid, SIGTSTP);
        needSig = 0;
        nowcpid = 0;
    }
    cout << "^Z";
    fflush(stdout);

}

void sigint_handler(int sig)
{
    if(needSig == 1)
    {
        if(DEBUG)
            cout << "!!" << endl;
        kill(nowcpid, SIGINT);
        needSig = 0;
        nowcpid = 0;
    }
    cout << "^C";

    fflush(stdout);
}

int starOfCom(vector<string> com)
{
    int num = 0;
    for(vector<string>::iterator it = com.begin();
            it != com.end(); ++it)
        if((*it).find('*') != string::npos)
            num++;
    return num;
}
char * stoc(string s1)
{
    char *data;
    int len = s1.length();
    data = (char *)malloc((len+ 1)*sizeof(char));
    s1.copy(data,len,0);
    data[len] = '\0';
    return data;
}



int run(vector<vector<string> > com, const char* dir, string username, list<string> comhis, const char* hisfile, bool & open)
{
    needSig = 0;
    nowcpid = 0;

    Colour CO;
    int status;
    if(com[0][0] == "cd"){
        status = 0;
        if(com[0].size() ==1){
            chdir(dir);
        }
        else{
            if(com[0][1] == "~")
            {
                chdir(dir);
            }
            else if(chdir(com[0][1].c_str()) == -1){
                cout << "0x4149 shell : cd: "<< com[0][1] << ": No such file or directory" << endl;
                status = 1;
            }
        }

    }
    else if(com[0][0] == "history"){
        int index = 1;
        for(list<string>::iterator it = comhis.begin(); it != comhis.end(); ++it, index++)
            cout << index << " " << *it << endl;
        status = 0;
    }
    else if(com[0][0] == "exit") {
        cout << "good bye, "<< username << "." << endl;
        cout << CO.HEADER << CO.LINE << CO.ENDC << endl;
        ofstream fout(hisfile);
        for(list<string>::iterator it = comhis.begin(); it != comhis.end(); ++it)
            fout << *it << endl;
        fout.close();

        open = 0;
    }
    else if(com[0][0] == "status"){
        if(status == 0)
            cout << CO.KGREEN << "success! " << CO.ENDC;
        else
            cout << CO.FAIL << "failed.  " << CO.ENDC;
        cout << "return number: " << status << endl;
        status = 0;
    }
    else if(com[0][0] == "jobs"){
        for(int i = 0; i < vecJ.size(); i++)
        {
            if(waitpid(-1,NULL,WNOHANG | WUNTRACED)!= 0)
            {
                if(vecJ[i].stat != 2)
                {
                    vecJ.erase(vecJ.begin()+i);
                    i--;
                }
                else {
                    vecJ[i].stat = 1;
                }
            }
        }
        for(int i = 0; i < vecJ.size(); i++)
        {
            cout << "[" << i+1 << "]" << " ";
            if(vecJ[i].stat == 1)
                cout << "stopped" << "\t\t";
            else if(vecJ[i].stat == 2)
                cout << "running" << "\t\t";

            cout << vecJ[i].pid << "\t";
            cout << vecJ[i].com << endl;
        }
        status = 0;
    }
    else if(com[0][0] == "bg"){

        if(com[0].size() != 2)
            cout << CO.FAIL << "plz input job num" << CO.ENDC << endl;

        status = -1;

        int JN = atoi(com[0][1].c_str());
        if(DEBUG)
            cout << JN << endl;

        if(JN > vecJ.size())
            cout << CO.FAIL << "no exist job." << CO.ENDC << endl;
        else {
            int JPid = vecJ[JN-1].pid;
            vecJ[JN-1].stat = 2;

            kill(JPid, SIGCONT);

        }
    }
    else if(com[0][0] == "fg" || com[0][0] == "continue"){

        if(com[0].size() != 2)
            cout << CO.FAIL << "plz input job num" << CO.ENDC << endl;

        status = -1;

        int JN = atoi(com[0][1].c_str());

        if(DEBUG)
            cout << JN << endl;

        if(JN > vecJ.size())
            cout << CO.FAIL << "no exist job." << CO.ENDC << endl;
        else{
            int JPid = vecJ[JN-1].pid;
            string JName = vecJ[JN-1].com;

            vecJ.erase(vecJ.begin()+JN-1);

            kill(JPid, SIGCONT);

            needSig = 1;
            nowcpid = JPid;

            waitpid(JPid, &status, WUNTRACED);
            if(WIFSTOPPED(status))
            {
                if(DEBUG)
                    cout << "stop!" << endl;

                JOB j;
                j.pid = JPid;
                j.stat = 1;
                j.com = JName;
                vecJ.push_back(j);
            }
        }

    }
    else{
        status = runpipe(com);
    }

    return status;
}

int runpipe(vector<vector<string> > com)
{
    bool hasInFile = false;
    string inFile = "";
    bool hasOutFile = false;
    bool hasAppFile = false;
    string outFile = "";
    iB = 0;

    jobname = "";
    for(int i = 0; i < com.size(); i++)
    {
        for(int j = 0; j < com[i].size(); j++)
        {
            jobname += com[i][j];
            jobname += " ";
        }
    }

    if(DEBUG)
        cout << jobname << endl;

    for (vector<vector<string> >::iterator it = com.begin(); it != com.end();)
    {
        if ((*it)[0] == "|"){
            (*it).erase((*it).begin());
            it++;
        }
        else if((*it)[0] == "<")
        {
            hasInFile = true;
            inFile = (*it).back();
            it = com.erase(it);
        }
        else if((*it)[0] == ">")
        {
            hasOutFile = true;
            hasAppFile = false;
            outFile = (*it).back();
            it = com.erase(it);
        }
        else if((*it)[0] == ">>")
        {
            hasOutFile = false;
            hasAppFile = true;
            outFile = (*it).back();
            it = com.erase(it);
        }
        else if((*it)[0] == "&")
        {
            iB = 1;
            it = com.erase(it);
        }
        else {
            it++;
        }
    }

    int ifd, ofd;

    if(hasInFile == true)
    {
        if(DEBUG)
            cout << "In: " << inFile << endl;
        ifd = open(stoc(inFile), O_RDONLY);
    }
    if(hasAppFile == true)
    {
        if(DEBUG)
            cout << "App: " << outFile << endl;
        ofd = open(stoc(outFile), O_APPEND|O_CREAT|O_WRONLY, 0666);
    }
    if(hasOutFile == true)
    {
        if(DEBUG)
            cout << "Out: " << outFile << endl;
        remove(stoc(outFile));
        ofd = open(stoc(outFile), O_RDWR|O_CREAT, 0666);
    }


    if(DEBUG)
        cout << "com size: " << com.size() << endl;
    int stat;
    if (com.size() == 1)
    {

        if(hasInFile && !hasOutFile && !hasAppFile)
            stat = run(com[0], PIPE_NONE, -1, -1, NULL, F_IN, ifd, 0);
        else if(!hasInFile && hasOutFile && !hasAppFile)
            stat = run(com[0], PIPE_NONE, -1, -1, NULL, F_OUT, 0, ofd);
        else if(hasInFile && hasOutFile && !hasAppFile)
            stat = run(com[0], PIPE_NONE, -1, -1, NULL, F_IN_AND_OUT, ifd, ofd);
        else if(!hasInFile && !hasOutFile && hasAppFile)
            stat = run(com[0], PIPE_NONE, -1, -1, NULL, F_APP, 0, ofd);
        else if(hasInFile && !hasOutFile && hasAppFile)
            stat = run(com[0], PIPE_NONE, -1, -1, NULL, F_IN_AND_APP, ifd, ofd);
        else
            stat = run(com[0], PIPE_NONE, -1, -1, NULL, F_NONE, 0, 0);

    }
    else if (com.size() >= 2)
    {
        int ** pifd = (int**)malloc((com.size()-1) * sizeof(int*));

        for(int i = 0; i < com.size() - 1; i++)
        {
            pifd[i] = (int*)malloc(2 * sizeof(int));

            if(pipe(pifd[i]) < 0)
            {
                cout << i << " pipe error" << endl;
                return -1;
            }
        }
        if(DEBUG)
            cout << "com.size() : " <<com.size() << endl;

        if(hasInFile)
            run(com[0], PIPE_IN, 0, com.size() - 1, pifd, F_IN, ifd, 0);
        else
            run(com[0], PIPE_IN, 0, com.size() - 1, pifd, F_NONE, 0, 0);


        for(int i = 1; i < com.size() - 1; i++)
        {
            run(com[i], PIPE_IAO, i-1, com.size() - 1, pifd, F_NONE, 0, 0);
        }

        if(hasAppFile)
            run(com[com.size()-1], PIPE_OUT, com.size()- 2, com.size() - 1, pifd, F_APP, 0, ofd);
        else if(hasOutFile)
            run(com[com.size()-1], PIPE_OUT, com.size()- 2, com.size() - 1, pifd, F_OUT, 0, ofd);
        else
            run(com[com.size()-1], PIPE_OUT, com.size()- 2, com.size() - 1, pifd, F_NONE, 0, 0);


        for(int i = 0; i < com.size() - 1; i++)
        {
            close(pifd[i][0]);
            close(pifd[i][1]);
        }

        do
        {
            if(DEBUG)
                cout << "wait" << endl;
        }
        while(wait(&stat) != -1);

        if(hasInFile)
            close(ifd);
        if(hasOutFile || hasAppFile)
            close(ofd);

        for(int i = 0; i < com.size() - 1; i++)
            free(pifd[i]);

        free(pifd);

    }


    return stat;

}

int run(vector<string> com, int inorout, int num, int max, int **pifd, int fileInOut, int filein, int fileout)
{
    if(DEBUG){

        cout << "com size: " <<  com.size() << endl;
        cout << starOfCom(com) << endl;
    }


    if(com[0] == "ls" || com[0] == "grep")
    {
        com.push_back("--color=auto");
    }


    int status = 0;

    int star = starOfCom(com);

    //char * array[] = {stoc(com[0]), NULL};



    int issub = fork();
    if(issub == 0){

        //signal(SIGUSR1, sigusr1_handler);
        /*
        if(iB == 1)
        {
            close(1);
            close(0);
            setpgrp();
        }
        */
        setpgrp();

        if(inorout == PIPE_IN)
        {
            for(int i = 0; i < max; i++)
            {
                if(DEBUG)
                    cout << com[0] << " in!!! max: " << max << " i : " << i << endl;

                if(i != num)
                {
                    if(DEBUG)
                        cout << ">" << com[0] << " close " << i << endl;
                    close(pifd[i][0]);
                    close(pifd[i][1]);
                }
            }
            close(pifd[num][0]);
            dup2(pifd[num][1], 1);
            close(pifd[num][1]);
        }
        else if(inorout == PIPE_IAO)
        {
            for(int i = 0; i < max; i++)
            {
                if(DEBUG)
                    cout << com[0] << " in!!! max: " << max << " i : " << i << endl;
                if(i != num && i != num+1)
                {
                    if(DEBUG)
                        cout << com[0] << " close " << i << endl;
                    close(pifd[i][0]);
                    close(pifd[i][1]);
                }
            }
            close(pifd[num][1]);
            dup2(pifd[num][0], 0);
            close(pifd[num][0]);

            close(pifd[num+1][0]);
            dup2(pifd[num+1][1], 1);
            close(pifd[num+1][1]);
        }
        else if(inorout == PIPE_OUT)
        {
            for(int i = 0; i < max; i++)
            {
                if(DEBUG)
                    cout << com[0] << " in!!! max: " << max << " i : " << i << endl;
                if(i != num)
                {
                    if(DEBUG)
                        cout << ">" << com[0] << " close " << i << endl;
                    close(pifd[i][0]);
                    close(pifd[i][1]);
                }
            }
            close(pifd[num][1]);
            dup2(pifd[num][0], 0);
            close(pifd[num][0]);
        }


        if(fileInOut == F_IN)
            dup2(filein, 0);
        else if(fileInOut == F_OUT || fileInOut == F_APP)
            dup2(fileout, 1);
        else if(fileInOut == F_IN_AND_OUT || fileInOut == F_IN_AND_APP)
        {
            dup2(filein, 0);
            dup2(fileout, 1);
        }



        if(star == 0)
        {
            char ** arg = (char **)malloc((com.size()+ 1) * sizeof(char *));
            for(int i = 0; i < com.size(); i++){
                arg[i] = stoc(com[i]);
            }
            arg[com.size()] = NULL;

            execvp(com[0].c_str(), arg);

            cout << com[0] << ": command not found" << endl;
            for(int i = 0; i <= com.size(); i++)
            {
                free(arg[i]);
            }
            free(arg);
            exit(-1);
        }
        else {
            glob_t globbuf;

            globbuf.gl_offs = com.size() - star;

            int comnum = 0;
            bool isNumOne = 1;
            for(int i = 0; i < com.size(); i++){
                if(com[i].find('*') != string::npos){
                    if(isNumOne){
                        glob(com[i].c_str(), GLOB_DOOFFS, NULL, &globbuf);
                        isNumOne = 0;
                    }
                    else
                        glob(com[i].c_str(), GLOB_DOOFFS | GLOB_APPEND,
                                NULL, &globbuf);
                }
            }

            for(int i = 0; i < com.size(); i++){
                if(com[i].find('*') == string::npos){
                    globbuf.gl_pathv[comnum] = stoc(com[i]);
                    comnum ++;
                }
            }

            execvp(com[0].c_str(), &globbuf.gl_pathv[0]);
            cout << com[0] << ": command not found" << endl;
            for(int i = 0; i < globbuf.gl_offs; i++)
            {
                free(globbuf.gl_pathv[i]);
            }
            exit(-1);

        }
    }
    else {
        if(inorout == PIPE_NONE && iB != 1)
        {
            needSig = 1;
            nowcpid = issub;

            waitpid(issub, &status, WUNTRACED);
            if(WIFSTOPPED(status))
            {
                if(DEBUG)
                    cout << "stop!" << endl;

                JOB j;
                j.pid = issub;
                j.stat = 1;
                j.com = jobname;
                vecJ.push_back(j);

            }

        }

        if(iB == 1)
        {
            JOB j;
            j.pid = issub;
            j.stat = 2;
            j.com = jobname;
            vecJ.push_back(j);
        }
    }
    if(DEBUG)
        cout << "end " << com[0] << endl;

    return status;
}


/*
   int main(void)
   {
   vector<string> com1, com2, com3, com4;
   vector<vector<string> > com;

   com1.push_back("ls");
   com.push_back(com1);
   com2.push_back("|");
   com2.push_back("grep");
   com2.push_back("r");
   com.push_back(com2);
   com2.push_back("|");
   com3.push_back("grep");
   com3.push_back("c");
   com.push_back(com3);
   com2.push_back("|");
   com4.push_back("grep");
   com4.push_back("h");
   com.push_back(com4);

   runpipe(com);
   if(DEBUG)
   cout << "end p" << endl;

   string com;
   vector<string> vecstr;
   int num = 0;
   while(1){
   cout << "simple shell:" << endl;
   cout << "input arg num" << endl;
   cin >> num;
   for(int i = 0; i < num; i++){
   cin >> com ;
   vecstr.push_back(com);
   }
   if(DEBUG){
   for(vector<string>::iterator it = vecstr.begin(); it != vecstr.end(); ++it ){
   cout << *it << " ";
   }
   cout << endl;
   }
   num = 0;
   if(com == "exit")
   break;
   run(vecstr);

   vecstr.clear();
   }

   char * array[] = { "ls", "-a", NULL};
   execvp("ls", array);
   cout << "after. fail!" << endl;
   return 0;

   }*/
