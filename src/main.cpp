#include <iostream>
#include <string.h>
#include <vector>
#include "base.h"
#include "base.cpp"
#include <unistd.h>
using namespace std;

Base* make_command(char*); 
Base* build_tree(vector<char>, unsigned int &, char* &);
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

    string altcmd = "("; //want the string to always be enclosed by ()
    altcmd += cmd;
    altcmd += ')';
    char* str1;
    char* str2;

    str1 = new char[altcmd.size()];   //sets up two identical c-strings
    strcpy(str1, altcmd.c_str());
    str1 = strtok(str1, "#");

    str2 = new char[altcmd.size()];
    strcpy(str2, altcmd.c_str());
    str2 = strtok(str2, "#");

    char keys[] = "();&|\"";  
    vector<char> conns; // holds the delimiters in order in which they were found
    char* del;
    int parenthcounter = 0;

    del = strpbrk (str1, keys); //finds first delimiter              
    while (del != NULL) // checks to make sure don't go out of bounds
    {
        if ( (*del == '&') || (*del == '|') ) {
            del = strpbrk(del+1, keys);
        }
        if ( (*del == '(') || (*del == ')') ) {
            ++parenthcounter; //counts # of parenthesis 
        }
        conns.push_back(*del); //pushes last delimiter onto vector
        del = strpbrk (del+1, keys); //finds next delimiter
    }

    if ( parenthcounter % 2 != 0) { //if not paired up => results in an error
        cout << "Error: Incorrect number of parenthesis." << endl;
        return;
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
     
    unsigned int con_index = 0;
    Base* user = build_tree(conns, con_index, str2); // call function for first time
    user->execute();

    return;
}


Base* build_tree(vector<char> connects, unsigned int &con_index, char* &cmdstr) {

    Base* prev = NULL;
    Base* curr = NULL;
    Base* child = NULL;
    char* token = NULL;
    char d;
    char* del = new char[2];
    // use strtok_r because the inner while loop uses 
    // strtok also, so it messes up outer while loop's 
    // strtok pointers, thus we use strtok_r on outer

    if ( (connects.size() > 0) && (con_index < connects.size()) ) {
        d = connects.at(con_index);
        del[0] = d;
        del[1] = '\0';
    }
    
    else {
        d = ')';
        del[0] = d;
        del[1] = '\0';
    }
    //cout << "d = " << d << endl;
    // takes in the first delimiter in command line
    // used to create tokens of commands + flags


    do {
        if ( d == '(' ) { // calls recursive function again to build child
            
            ++con_index;
            char c = cmdstr[0];
            if (c == '(') {
                cmdstr = cmdstr + 1;
            }
            else {
                strtok_r(cmdstr, del, &cmdstr);
            }
            //cout << "cmdstr = " << cmdstr << endl;
             
            child = build_tree(connects, con_index, cmdstr);
            //gets returned a base pointer when ')' is reached
            
            //if there was already a connector at this recursion level
            // then we set child to rhs of prev
            if (prev != NULL) { 
                prev->set_rhs(child);
            }

            ++con_index;
            //cout << "con_index = " << con_index << endl;

            if ( con_index < connects.size() ) {
                d = connects.at(con_index);
                del[0] = d;
                del[1] = '\0';

                //these behave differently because of strtok_r
                c = cmdstr[0];
                if ( (c == ';') || (c == ')') ) {
                    cmdstr = cmdstr + 1;
                }
                else {
                    token = strtok_r(cmdstr, del, &cmdstr);
                }


                if (d == ')') {
                    if (prev == NULL) {
                        return child;
                    }
                    else {
                        return prev;
                    }
                }
            }
            else {
                prev = child;
                return prev;
            }
        }
        else {
            // if delimiter == " => it will find the con_index of the delimiter
            // after the closing "
            // it will concatenate the two tokens and make a command
            if (d == '\"') {
                
                do {
                    ++con_index;
                } while (connects.at(con_index) != '\"');
                ++con_index;

                char* tokentemp = NULL;
                token = strtok_r(cmdstr, del, &cmdstr);
                tokentemp = strtok_r(cmdstr, del, &cmdstr);
                strcat (token, tokentemp);
                child = make_command(token);

                d = connects.at(con_index);
                del[0] = d;
                del[1] = '\0';
                
                char f = cmdstr[0];
                if ( (f == ';') || (f == ')') ) { //this may break
                    cmdstr = cmdstr + 1;
                }
                else {
                    token = strtok_r(cmdstr, del, &cmdstr);
                }

            }
            else {    
                token = strtok_r(cmdstr, del, &cmdstr);
           
                //make command object
                child = make_command(token);
            }
        }

        string deltemp(del);
        if (deltemp == ";") {
            curr = new Semicolon();
            //cout << "made semi" << endl;
        }
        else if (deltemp == "&") {
            curr = new Ampersand();
            //cout << "made amper" << endl;
            cmdstr = cmdstr + 1;
            //cout << "cmdstr = " << cmdstr << endl;
        }
        else if (deltemp == "|") {
            curr = new Verticalbars();
            //cout << "made verticalbars" << endl;
            cmdstr = cmdstr + 1;
            //cout << "cmdstr = " << cmdstr << endl;
        }
        else if (deltemp == ")") {
            //hit ) => returning from recursion
            if (prev == NULL) {
                return child;
            }
            else {
                prev->set_rhs(child);
                return prev;
            }
        }
        else {
            cout << "something went wrong: choosing connector" << endl;
        }


        // places command and connector objects in correct place
        if (prev == NULL) {
            curr->set_lhs(child);
        }
        else { //don't want a command object to call lhs or rhs
            if (prev->check_type()) {
                //prev was a connector
                prev->set_rhs(child);
                curr->set_lhs(prev);
            }
            else {
                //prev was a command
                curr->set_lhs(prev);
            }
        }
        prev = curr;
                

        con_index = con_index + 1;
        //cout << "con_index = " << con_index << endl;
        if (con_index < connects.size()) {
            d = connects.at(con_index);
            del[0] = d;
            del[1] = '\0';
        }
        else {
            d = ')';
            del[0] = d;
            del[1] = '\0';
        }
            

        
    } while (con_index < connects.size());
    return prev;
}

Base* make_command(char* token) { 
    vector<char*> args;
    char* small;
    char** cmdarr;
    Command* command;
    Base* child;

    if (token != NULL) {

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

        string chk_str(args.at(0));
        if ( (chk_str == "test") || (chk_str == "[") ) {
            if (chk_str == "test") {
                //makes array of same size because "test" will be removed
                cmdarr = new char*[args.size()];
                for (unsigned int i = 1; i < args.size(); ++i) {
                    string temp(args.at(i));
                    cmdarr[i - 1] = new char[temp.size()];
                    strcpy(cmdarr[i - 1], args.at(i));
                }
                cmdarr[args.size() - 1] = NULL;
            }
            else {
                //makes array of size - 1 because "[" and "]" are removed
                cmdarr = new char*[args.size() - 1];
                for (unsigned int i = 1; i < args.size() - 1; ++i) {
                    string temp(args.at(i));
                    cmdarr[i - 1] = new char[temp.size()];
                    strcpy(cmdarr[i - 1], args.at(i));
                }
                cmdarr[args.size() - 2] = NULL;
            }
            command = new Test(cmdarr);           
        }        
        else {
            cmdarr = new char*[args.size() + 1];
            for (unsigned int a = 0; a < args.size(); ++a) {
                string temp(args.at(a)); // turn it into string so i can use size()
                cmdarr[a] = new char[temp.size()]; //allocate memory for each char arr[]
                strcpy(cmdarr[a], args.at(a)); //copy c_str from vector into array
            }
            cmdarr[args.size()] = NULL; //puts null into last element of array

            if (chk_str == "exit") {
                command = new Exit(cmdarr);
            }
            else {
                command = new Command(cmdarr);
            }
        }
                    
        child = command;
        return child;
    }
    else {
        cout << "Error: Somehow make_command was passed a NULL pointer." << endl;
        return NULL;
    }
}






