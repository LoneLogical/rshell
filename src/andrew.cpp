#include <iostream>
#include <string.h>
#include <vector>
#include "base.h"
#include "base.cpp"
#include <unistd.h>
using namespace std;

void command_line(string);

int main ()
{
    char* a;
    int b;
    string cmd;    
    while(1) {
        a = getlogin();
        cout << a << "@";
        b = gethostname(a, 100);
        cout << a;
        b++;
        cout << "$ ";

        getline(cin, cmd);

        command_line(cmd);
    }
    return 0;
}

void command_line(string cmd) {

    char* str1;
    char* str2;

    str1 = new char[cmd.size()];
    strcpy(str1, cmd.c_str());
    str1 = strtok(str1, "#");

    str2 = new char[cmd.size()];
    strcpy(str2, cmd.c_str());
    str2 = strtok(str2, "#");

    char keys[] = ";&|"; 
    vector<char> conns; // holds the delimiters in order in which they were found
    char* del;

    del = strpbrk (str1, keys); //finds first delimiter              
    while (del != NULL) // checks to make sure don't go out of bounds
    {
        if (*del != ';') {
            del = strpbrk(del+1, keys);
        }  
        conns.push_back(*del); //pushes last delimiter onto vector
        del = strpbrk (del+1,keys); //finds next delimiter
    }
    //ouput tester
    /*
    cout << "conns size: " << conns.size() << endl;
    for (unsigned int i = 0; i < conns.size(); ++i)
    {
        cout << conns.at(i) << ' '; //prints delimiters found
    }
    cout << endl;
    */ 
    Connector* prev = NULL;
    Connector* curr = NULL;
    Command* command = NULL;
    Base* user = NULL;

    char* token = NULL;
    char d;
    char* trash;
    del = new char[2];
    char* saved;  // used with strtok_r because the inner while loop uses 
    // strtok also, so it messes up outer while loop's 
    // strtok pointers, thus we use strtok_r on outer
    vector<char*> args;
    char* small;
    char** cmdarr;
    char* tempzero;

    if (conns.size() == 0) {
        small = strtok(str2, " ");
        while (small != NULL)
        {
            //cout << '_' << small << '_'; 
            args.push_back(small);
            small = strtok(NULL, " ");
        }
        //cout << endl;

        cmdarr = new char*[args.size()+ 1];

        for (unsigned int a = 0; a < args.size(); ++a) {
            string temp(args.at(a)); // turn it into string so i can use size()
            cmdarr[a] = new char[temp.size()]; //allocate memory for each char arr[]
            strcpy(cmdarr[a], args.at(a)); //copy c_str from vector into array
        }

        cmdarr[args.size()] = NULL;
        string check_exit(cmdarr[0]);
        if (check_exit == "exit") {
            command = new Exit(cmdarr);
        }
        else {
            command  = new Command(cmdarr);
        }
        user = command;
        user->execute();
        return;   
    }

    else {
        unsigned int j = 0;
        d = conns.at(j);
        del[0] = d;
        del[1] = '\0';
        // takes in the first delimiter in command line
        // used to create tokens of commands + flags

        token = strtok_r(str2, del, &saved);
        while ( (token != NULL) && (j <= conns.size()) ) {
            //cout << "Token: " << token << endl;
            small = strtok(token, " ");
            while (small != NULL)
            {
                //cout << '_' << small << '_'; 
                args.push_back(small);
                small = strtok(NULL, " ");
            }
            //cout << endl;
            // for loop to convert vector<char*> into char* array[]
            // then create a command object by passing in array

            cmdarr = new char*[args.size() + 1];

            for (unsigned int a = 0; a < args.size(); ++a) {
                string temp(args.at(a)); // turn it into string so i can use size()
                cmdarr[a] = new char[temp.size()]; //allocate memory for each char arr[]
                strcpy(cmdarr[a], args.at(a)); //copy c_str from vector into array
            }

            cmdarr[args.size()] = NULL; //puts null into last element of array


            string chk_exit(cmdarr[0]);

            // if statement to test if array[0] == exit
            if(chk_exit == "exit") {
                command = new Exit(cmdarr);
            }
            else {
                command = new Command(cmdarr);
            }

            // if else statements to make a connector!

            if (j == conns.size()) { //changed from token == NULL
                prev->set_rhs(command);
                user = prev;
                j = j + 1;
            }
            else {
                string deltemp(del);
                if (deltemp == ";") {
                    curr = new Semicolon();
                    //cout << "made semi" << endl;
                }
                else if (deltemp == "&") {
                    curr = new Ampersand();
                    //cout << "made amper" << endl;
                    saved = saved + 1;
                }
                else if (deltemp == "|") {
                    curr = new Verticalbars();
                    //cout << "made verticalbars" << endl;
                    saved = saved + 1;
                }
                else {
                    cout << "something went wrong: choosing connector" << endl;
                }

                // places command and connector objects in correct place
                if (prev == NULL) {
                    curr->set_lhs(command);
                }
                else {
                    prev->set_rhs(command);
                    curr->set_lhs(prev);
                }
                prev = curr;
                

                args.resize(0);
                j = j + 1;
                if (j < conns.size()) {

                    d = conns.at(j);
                    del[0] = d;
                    del[1] = '\0';
                    //deltemp = del;
                    //cout << "del" << deltemp << endl;
                    token = strtok_r(saved, del, &saved);
                }
                else {
                    token = saved;
                }
            }
            

        }
        //cout << "out here" << endl;
        user = prev;
        user->execute();
    }
    return; 
}   




