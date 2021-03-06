//
// Created by Li Zuoming on 11/11/15.
//
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/document.h"

using namespace std;
using namespace rapidjson;

#define MAX_LINES 20000
#define MAX_SALARY 500
#define MAX_SALARY_TT 200
#define MAX_POS 3

#define CLEAR() printf("\033[2J")

int err_input()
{
    cerr<<"ERROR: ILLAGE INPUT. Please use DKSalaries.csv or .json as stdin."<<endl;
    cerr<<"ABORT"<<endl;
    return 1;
}

int draft(int a_sal, int* has, string data);

int main(int argc, char *argv[]) {

    int has[8] = {1,1,1,1,1,1,1,1};
    int max_salary = 0;
    if(argc>1) 
    {
        int val = atoi(argv[1]);
        if(val>0 && val<=MAX_SALARY) {
            max_salary = val;
        }
        for(int i=2; i<argc; i++) {
            has[i-2] = atoi(argv[i]);
        }
    }
    string data = "";
    string str;
    while (getline(cin, str))
    {
        data += str;
        data += "\n";
    }
    draft(max_salary, has, data);
}


int draft(int a_sal, int* has, string data)
{

    fprintf(stderr,"\033[2J\033[35m \
           ************************************************\n \
           *****   DRAFT KING NBA optimal draft 1.2   *****\n \
           *****        Author:     Zuoming Li        *****\n \
           *****  please use DKSalaries.csv as stdin  *****\n \
           ************************************************\033[0m");

    string ori_data[MAX_LINES][7];
	string line;
    string word;
    bool is_tt = false;
    int max_salary = a_sal;

    int player_need = 0;
    for(int i=0; i<8; i++)
        player_need += has[i];
    istringstream cin;
    cin.str(data);

    fprintf(stderr,"\n\n\033[34m\033[47mReading from stdin ...\033[0m\n");
    int i=0;
    while( getline(cin,line))
    {
        //if(line.at(0)!='"')
        //    return err_input();

        istringstream is;
        is.str(line);
        int j=0;
        if (is_tt == true || *line.begin() == '{' || *line.begin() == '[')
        {
            is_tt = true;
            Document d;
            const char * cline = line.c_str();
            d.Parse(cline);
            string pos_map[6] = {"","PG", "SG", "SF", "PF", "C"};
            ori_data[i][0]=pos_map[stoi(d["position"].GetString())];
            ori_data[i][1]=d["player"].GetString();
            ori_data[i][2]=to_string(d["salary"].GetInt());
            ori_data[i][4]=d["point"].GetString();
            ori_data[i][5]=d["team"].GetString();
            ori_data[i][6]=d["id"].GetString();
            cerr<<ori_data[i][0]<<" "<<ori_data[i][1]<<" "<<ori_data[i][2]<<" "<<ori_data[i][4]<<" "<<ori_data[i][5]<<endl;
        } 
        else 
        {
            while(getline(is,word,','))
            {   
                if (*word.begin() == '"')
                word.erase(word.begin());
                if (*(--word.end()) == '"')
                    word.erase(--word.end());
                if (*(word.end()-2) == '"')
                {
                    word.erase(--word.end());
                    word.erase(--word.end());
                }
                ori_data[i][j]=word;
                j++;
                //cerr<<word<<endl;
            }
            if(j!=6)
                return err_input();
        }
        i++;
        //cerr<<"rd.."<<i<<endl;
    }

    int player_num = i-1;
    if(max_salary==0)
        max_salary = (is_tt ? MAX_SALARY_TT : MAX_SALARY);
    int max_pos_num = 3;

    fprintf(stderr, "\033[32mDONE: %d players read.\033[0m\n\n", player_num);
    fprintf(stderr, "\033[32mMax salary set to %d.\033[0m\n", max_salary);
    //cerr<<"DONE: "<<player_num<<" players read."<<endl<<endl;
    fprintf(stderr,"\033[?25l\033[34m\033[47mComputing the optimal selection ...\033[0m\n\033[31m0%%\033[0m\n");

    auto dp = new double[player_num+1][MAX_SALARY+1][MAX_POS+1][MAX_POS+1][MAX_POS+1][MAX_POS+1][MAX_POS+1];
    memset(dp,0, sizeof(double)*(player_num+1)*(MAX_SALARY+1)*(MAX_POS+1)*(MAX_POS+1)*(MAX_POS+1)*(MAX_POS+1)*(MAX_POS+1));


    for(int j=0; j<=max_salary; j++)
    {
        for(int a=0; a<=MAX_POS; a++)
        {
            for(int b=0; b<=MAX_POS; b++)
            {
                for(int c=0; c<=MAX_POS; c++)
                {
                    for(int d=0; d<=MAX_POS; d++)
                    {
                        for(int e=0; e<=MAX_POS; e++)
                        {
                            if(a!=0 || b!=0 ||c!=0||d!=0||e!=0)
                                dp[0][j][a][b][c][d][e]=-1000000;
                        }
                    }
                }
            }
        }
    }


    int dis=1;
    for(int i=1; i<=player_num; i++)
    {
        if(100*i/player_num>=dis)
        {
            //CLEAR();
            fprintf(stderr,"\033[1A\033[K\033[31m%d%%\033[0m\n",dis);
            //cerr<<dis<<"%"<<endl;
            dis=100*i/player_num+1;
        }
        for(int j=0; j<=max_salary; j++)
        {
            for(int a=0; a<=MAX_POS; a++)
            {
                for(int b=0; b<=MAX_POS; b++)
                {
                    for(int c=0; c<=MAX_POS; c++)
                    {
                        for(int d=0; d<=MAX_POS; d++)
                        {
                            for(int e=0; e<=MAX_POS; e++)
                            {
                                int salary  = stoi(ori_data[i][2]);
                                if(!is_tt)  salary /= 100;
                                double fppg = stof(ori_data[i][4]);
                                string pos = ori_data[i][0];
                                double max = dp[i-1][j][a][b][c][d][e];

                                if(j-salary >= 0)
                                {
                                    double put=-10000;
                                    if(pos=="PG" && a>0)
                                        put = dp[i - 1][j - salary][a-1][b][c][d][e];
                                    else if(pos=="SG" && b>0)
                                        put = dp[i - 1][j - salary][a][b-1][c][d][e];
                                    else if(pos=="SF" && c>0)
                                        put = dp[i - 1][j - salary][a][b][c-1][d][e];
                                    else if(pos=="PF" && d>0)
                                        put = dp[i - 1][j - salary][a][b][c][d-1][e];
                                    else if(pos=="C" && e>0)
                                        put = dp[i - 1][j - salary][a][b][c][d][e-1];
                                    put+=fppg;
                                    if(put>max)
                                        max=put;
                                }
                                dp[i][j][a][b][c][d][e]=max;
                                //cerr<<i<<" "<<j<<" "<<a<<b<<c<<d<<e<<" : "<<max<<endl;
                            }
                        }
                    }
                }
            }

        }

    }

    //cerr<<"re:"<<dp[player_num][500][2][2][2][1][1]<<endl;

    map<double, int> res;

    for(int i=0; i<(has[5]==1?2:1); i++)
        for(int j=0; j<(has[6]==1?2:1); j++)
            for(int k=1; k<=(has[7]==1?5:1); k++)
            {
                int a = has[0] + (i==0 && has[5]==1) + (k==1 && has[7]==1);
                int b = has[1] + (i==1 && has[5]==1) + (k==2 && has[7]==1);
                int c = has[2] + (j==0 && has[6]==1) + (k==3 && has[7]==1);
                int d = has[3] + (j==1 && has[6]==1) + (k==4 && has[7]==1);
                int e = has[4] + (k==5 && has[7]==1);
                if(a<0||b<0||c<0||d<0||e<0)
                    continue;

                double subop = dp[player_num][max_salary][a][b][c][d][e];
                res[subop] = a*10000+b*1000+c*100+d*10+e;
                //cerr<<count<<" : "<<a<<b<<c<<d<<e<<" : "<<subop<<endl;
            }

    fprintf(stderr, "\033[32mDONE.\033[0m");
    fprintf(stderr,"\n\n\033[34m\033[47mOutputing results to stdout ...\033[0m\n\33[?25h");
    int count=0;
    for(auto it= res.rbegin(); it!=res.rend(); it++)
    {
        int type = (*it).second;
        double op = (*it).first;

        int e = type%10; type/=10;
        int d = type%10; type/=10;
        int c = type%10; type/=10;
        int b = type%10; type/=10;
        int a = type%10; type/=10;

        count++;

        map<int, int> ply;
        int key;
        int num=0;
        int j = max_salary;
        for (int i = player_num; i > 0; i--) {

            if (dp[i][j][a][b][c][d][e] != dp[i - 1][j][a][b][c][d][e])
            {
            	num++;
                int salary = stoi(ori_data[i][2]);
                if(!is_tt)  salary/=100;
                string pos = ori_data[i][0];

                j -= salary;
                if (pos == "PG")
                {
                	a--;
                	key=10+num;
                }
                else if (pos == "SG")
                {
                	b--;
                	key=20+num;
                }
                else if (pos == "SF")
                {
                	c--;
                	key=30+num;
                }
                else if (pos == "PF")
                {
                	d--;
                	key=40+num;
                }
                else if (pos == "C")
                {
                	e--;
                	key=50+num;
                }
                ply[key]=i;
                //cerr<<a<<b<<c<<d<<e<<endl;
            }

        }

        if(ply.size()==player_need)
        {
            cerr<<"-------------------------------------------------------------------------------"<<endl;
            cerr<<"Selection "<<count<<":"<<endl;
            cerr<<"TYPE: "<<a<<"PG "<<b<<"SG "<<c<<"SF "<<d<<"PF "<<e<<"C"<<endl;
            cerr<<"TOTAL FPPG: "<<op<<endl;
            cerr<<"TOTAL SALARY: "<<(is_tt? (max_salary-j) : (max_salary-j)*100)<<endl<<endl;
            for(auto it=ply.begin(); it!=ply.end(); it++)
            {
        	   int i= (*it).second;
               cout << ori_data[i][6] << ",";
        	   cerr << ori_data[i][0] << "\t" << ori_data[i][1] << "\t\t" << ori_data[i][5] << "\t"
                 << ori_data[i][2] << "\t" << ori_data[i][4] <<"\t" << ori_data[i][3]<< endl;
            }
            cout<<endl;
            cerr<<"-------------------------------------------------------------------------------"<<endl<<endl;
        }
        
        //cerr<<"PRESS ANY KEY TO COMTINUE"<<endl;
        //getchar();
        //system("PAUSE");
    }
    fprintf(stderr, "\033[32mDONE.\033[0m\n");

    return 0;
}
