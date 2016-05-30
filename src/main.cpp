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
        cout << "but does it get here twice?" << endl;
        a = getlogin();
        cout << a << "@";
        b = gethostname(a, 100);
        cout << a;
        b++;
        cout << "$ ";

        getline(cin, cmd);

        command_line(cmd);
        cout << "it returned here" << endl;
    }
    return 0;
}

void command_line(string cmd) {

    string altcmd = "("; //want the string to always be inclosed by ()
    altcmd += cmd;
    altcmd += ')';
    char* str1;
    char* str2;

    str1 = new char[altcmd.size()];
    strcpy(str1, altcmd.c_str());
    str1 = strtok(str1, "#");

    str2 = new char[altcmd.size()];
    strcpy(str2, altcmd.c_str());
    str2 = strtok(str2, "#");

    char keys[] = "();&|"; //don't forget to insert quotations here _\"_ 
    //char keyquotes[] = "\"";
    vector<char> conns; // holds the delimiters in order in which they were found
    char* del;

    del = strpbrk (str1, keys); //finds first delimiter              
    while (del != NULL) // checks to make sure don't go out of bounds
    {
        if ( (*del == '&') || (*del == '|') ) {
            del = strpbrk(del+1, keys);
        }
        /*
        else if (*del == '"') {
            conns.push_back(*del);
            del = strpbrk(del+1, keyquotes);
        }
        */
        conns.push_back(*del); //pushes last delimiter onto vector
        del = strpbrk (del+1, keys); //finds next delimiter
    }
    //ouput tester
    
    cout << "conns size: " << conns.size() << endl;
    for (unsigned int i = 0; i < conns.size(); ++i)
    {
        cout << conns.at(i) << ' '; //prints delimiters found
    }
    cout << endl;
     
    unsigned int con_index = 0;
    Base* user = build_tree(conns, con_index, str2); // call function for first time
    cout << "finished building" << endl;
    user->execute();
    cout << "finished executing" << endl;

    return;
}

    // NEED SOME CHECK FOR EVEN AMOUNT OF PARANTHESIS

Base* build_tree(vector<char> connects, unsigned int &con_index, char* &cmdstr) {

    //Connector* child = NULL;
    Base* prev = NULL;
    Base* curr = NULL;
    Base* child = NULL;
    char* token = NULL;
    char d;
    char* del = new char[2];
    //char* saved;  // used with strtok_r because the inner while loop uses 
    // strtok also, so it messes up outer while loop's 
    // strtok pointers, thus we use strtok_r on outer

    cout << "Just entered build_tree: con_index = " << con_index << endl; 
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
    cout << "d = " << d << endl;
    // takes in the first delimiter in command line
    // used to create tokens of commands + flags


    do {
        if ( d == '(' ) {
            //need to know what kind of tree is returned
            
            ++con_index;
            char c = cmdstr[0];
            cout << "char c = _" << c << '_' << endl;
            if (c == '(') {
                cmdstr = cmdstr + 1;
            }
            else {
                strtok_r(cmdstr, del, &cmdstr);
            }
            cout << "cmdstr = " << cmdstr << endl;
            //cmdstr = cmdstr + 1;
             
            child = build_tree(connects, con_index, cmdstr);
            
            if (prev != NULL) {
                prev->set_rhs(child);
            } // here is where we might mess up

            ++con_index;
            cout << "con_index = " << con_index << endl;

            if ( con_index < connects.size() ) {
                d = connects.at(con_index);
                cout << "d = " << d << endl;
                del[0] = d;
                del[1] = '\0';

                if ( (d == ';') || (d == ')') ) {
                    c = cmdstr[0];
                    if ( (c == ';') || (c == ';') ) {
                        cmdstr = cmdstr + 1;
                    }
                    cout << "limbo 3" << endl;
                    char stringlimbo[] = "limbostr";
                    token = stringlimbo;
                }
                else {
                    token = strtok_r(cmdstr, del, &cmdstr);
                }
                cout << "tokenAlpha = " << token << endl;
                cout << "cmdstr = " << cmdstr << endl;


                if (d == ')') {
                    if (prev == NULL) {
                        cout << "immediately return child" << endl;
                        return child;
                    }
                    else {
                        cout << "does it return from here???" << endl;
                        //prev = child;
                        return prev;
                    }
                }
                cout << "limbo 2" << endl;
            }
            else {
                prev = child;
                cout << "limbo 1" << endl;
                return prev;
            }
        }
        else {
            token = strtok_r(cmdstr, del, &cmdstr);
            cout << "tokenBravo = " << token << endl;
            cout << "cmdstr = " << cmdstr << endl;
           
            //make command object
            child = make_command(token);
            cout << "command made" << endl;
        }

        string deltemp(del);
        if (deltemp == ";") {
            curr = new Semicolon();
            cout << "made semi" << endl;
        }
        else if (deltemp == "&") {
            curr = new Ampersand();
            cout << "made amper" << endl;
            cmdstr = cmdstr + 1;
            cout << "cmdstr = " << cmdstr << endl;
        }
        else if (deltemp == "|") {
            curr = new Verticalbars();
            cout << "made verticalbars" << endl;
            cmdstr = cmdstr + 1;
            cout << "cmdstr = " << cmdstr << endl;
        }
        else if (deltemp == ")") {
            cout << "hit ) => returning from recursion" << endl;
            if (prev == NULL) {
                cout << "returned from here1" << endl;
                return child;
            }
            else {
                cout << "returned from here2" << endl;
                prev->set_rhs(child);
                return prev;
            }
        }
        else {
            cout << "something went wrong: choosing connector" << endl;
        }


        // places command and connector objects in correct place
        if (prev == NULL) {
            cout << "set lhs 1" << endl;
            curr->set_lhs(child);
        }
        else {
            if (prev->check_type()) {
                cout << "prev was a connector" << endl;
                prev->set_rhs(child);
                curr->set_lhs(prev);
            }
            else {
                cout << "prev was a command" << endl;
                curr->set_lhs(prev);
            }
        }
        prev = curr;
                

        con_index = con_index + 1;
        cout << "con_index = " << con_index << endl;
        if (con_index < connects.size()) {
            d = connects.at(con_index);
            cout << "d = " << d << endl;
            del[0] = d;
            del[1] = '\0';
        }
        else {
            d = ')';
            cout << "setting fake d = )" << endl;
            del[0] = d;
            del[1] = '\0';
        }
            

        
    } while (con_index < connects.size());
    cout << "broke the while loop" << endl;
    return prev;
}

//does it matter if i return command* or base* ???
Base* make_command(char* token) { 
    vector<char*> args;
    char* small;
    char** cmdarr;
    Command* command;
    Base* child;

    if (token != NULL) {

        cout << "Token: " << token << endl;
        small = strtok(token, " ");
        while (small != NULL)
        {
            cout << '_' << small << '_'; 
            args.push_back(small);
            small = strtok(NULL, " ");
        }
        cout << endl;
        // for loop to convert vector<char*> into char* array[]
        // then create a command object by passing in array

        string chk_str(args.at(0));
        if ( (chk_str == "test") || (chk_str == "[") ) {
            if (chk_str == "test") {
                cmdarr = new char*[args.size()];
                for (unsigned int i = 1; i < args.size(); ++i) {
                    string temp(args.at(i));
                    cmdarr[i - 1] = new char[temp.size()];
                    strcpy(cmdarr[i - 1], args.at(i));
                }
                cmdarr[args.size() - 1] = NULL;
            }
            else {
                cmdarr = new char*[args.size() - 1];
                for (unsigned int i = 1; i < args.size() - 1; ++i) {
                    string temp(args.at(i));
                    cmdarr[i - 1] = new char[temp.size()];
                    strcpy(cmdarr[i - 1], args.at(i));
                }
                cmdarr[args.size() - 2] = NULL;
            }
            cout << "this is where a test command is made" << endl;
            //command = new Test(cmdarr);           
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






