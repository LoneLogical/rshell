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
	while(1) {
		char* a = getlogin();
		cout << a << "@";
		int b = gethostname(a, 100);
		cout << a;
		b++;
		cout << "$ ";
		
		string cmd;
	    getline(cin, cmd);

		command_line(cmd);
	}
    return 0;
}

void command_line(string cmd) {
    char* str1;
    char* str2;
    str1 = new char[cmd.size() + 1];
    strcpy(str1, cmd.c_str());
    /*
    for (int index = 0; index < cmd.size(); ++index) {
        str1[index] = cmd.at(index);
    }
    str1[cmd.size()] = '\0';
    */
    str1 = strtok(str1, "#");
    
    str2 = new char[cmd.size()];
    strcpy(str2, cmd.c_str());
    /*
    for (int index = 0; index < cmd.size(); ++index) {
        str2[index] = cmd.at(index);
    }
    str2[cmd.size()] = '\0';
    */
    str2 = strtok(str2, "#");
    
    
    //char str1[] = "ls -a; echo hello && ls -l || exit";
    //char str2[] = "ls -a; echo hello && ls -l || exit";

    char keys[] = ";&|"; 
    vector<char> conns; // holds the delimiters in order in which they were found
    char* del;

    cout << "Connectors in " << str1 << ": ";
    
    del = strpbrk (str1, keys); //finds first delimiter              
    while (del != NULL) // checkes to make sure don't go out of bounds
    {
        conns.push_back(*del); //pushes last delimiter onto vector
        del = strpbrk (del+1,keys); //finds next delimiter
    }
                    
    for (unsigned int i = 0; i < conns.size(); ++i)
    {
        cout << conns.at(i) << ' '; //prints delimiters found
    }
    cout << endl;

    
    Connector* prev = NULL;
    Connector* curr = NULL;
    Command* command = NULL;
    Connector* user = NULL;

    unsigned int j = 0;
    char d = conns.at(j);
    del = new char[2];
    del[0] = d; 
    del[1] = '\0';
    // takes in the first delimiter in command line
    // used to create tokens of commands + flags
    string temp2(del);
    cout << "temp2: " << temp2 << endl;
    char* saved ; // used with strtok_r because the inner while loop uses 
                 // strtok also, so it messes up outer while loop's 
                 // strtok pointers, thus we use strtok_r on outer

    char* token = strtok_r(str2, del, &saved); // first command
    string tempt1(token);
    cout << "tempt1: " << tempt1 << endl;

    while ((token != NULL) && (j <= conns.size())) //makes sure we don't go out of bounds
    {
        cout << "made it through" << endl;
        cout << "Token #" << j << ": " << token << endl;
        //char *token2 = token;  // THIS MAY CAUSES PROBLEMS
        vector<char*> args;
        char* small = strtok(token, " ");
        while (small != NULL)
        {
            cout << '_' << small << '_' << ' '; 
            args.push_back(small);
            small = strtok(NULL, " ");
        }
        cout << endl;

        // for loop to convert vector<char*> into char* array[]
        // then create a command object by passing in array
        char** cmdarr;
        cmdarr = new char*[args.size()];

        for (int a = 0; a < args.size(); ++a) {
           string temp(args.at(a));
           cmdarr[a] = new char[temp.size()];
           strcpy(cmdarr[a], args.at(a));
        }
        cout << "this works1" << endl;
		
		string tmp(cmdarr[0]);
        if(tmp == "exit") {
			command = new Exit(cmdarr);
        }
        else {
			command = new Command(cmdarr);
		}
        // also...if statement to test if array[0] == exit
        // command = new Command(array);

        // if else statements to make a connector!
        //char semi[] = ";";
        //char amper[] = "&";
        //char verti[] = "|";
        
        if (j == conns.size()) {
            prev->set_rhs(command);
            user = prev;
            j = j + 1;
        }
        else {

            string temp1(del);
            cout << "temp1: " << temp1 << endl;
            if (temp1 == ";") {
                curr = new Semicolon();
                cout << "entered semi" << endl;
            }
            else if (temp1 == "&") {
                curr = new Ampersand();
                cout << "entered amper" << endl;
            }
            else if (temp1 == "|") {
                curr = new Verticalbars();
                cout << "entered verti" << endl;
            }
            else {
                cout << "still didn't enter" << endl;
            }
            cout << "this works2" << endl;
            // places command and connector objects in correct place
            if (prev == NULL) {
                curr->set_lhs(command);
            }
            else {
                prev->set_rhs(command);
                curr->set_lhs(prev);
            }
            cout << "this works3" << endl; 
            prev = curr;
        
            j = j + 1;
            if (j < conns.size()) {

                d = conns.at(j);
                del = new char[2];
                del[0] = d;
                del[1] = '\0';
            }
            token = strtok_r(saved, del, &saved);

            string tempt2(token);
            cout << "tempt2: " << tempt2 << endl;
    
        }
    }
    cout << "out here" << endl;
    user->execute();
}
