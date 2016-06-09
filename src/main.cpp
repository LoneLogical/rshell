#include <iostream>
#include <string.h>
#include <vector>
#include "base.h"
#include "base.cpp"
#include <unistd.h>
using namespace std;

void change_del(char* &, char);
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

    char keys[] = "><();&|\"";  
    vector<char> conns; // holds the delimiters in order in which they were found
    char* del;
    int paircounter = 0;
    del = strpbrk (str1, keys); //finds first delimiter              
    while (del != NULL) // checks to make sure don't go out of bounds
    {
        char d = *del; 
        char nextdel = *(del + 1);

        if (d == '&') {
            if (nextdel == '&')  { //only adds d if hits "&&"
                conns.push_back(d);
                del = strpbrk(del+2, keys);
            }
            else {
                del = strpbrk(del+1, keys);
            }
        }
        else if (d == '|') {
            if (nextdel == '|') { //only add d if hits "||"
                d = '!'; //filling in for double |
                conns.push_back(d);
                del = strpbrk(del+2, keys);
            }
            else {
                conns.push_back(d);
                del = strpbrk(del+1, keys);
            }
        }
        else if (d == '>') {
            if (nextdel == '>') {
                d = '}'; //filling in for >>
                conns.push_back(d);
                del = strpbrk(del+2, keys);
            }
            else {
                conns.push_back(d);
                del = strpbrk(del+1, keys);
            }
        }
        else {
            conns.push_back(d);
            if ( (d == '(') || (d == ')') || (d == '\"') ) {
                ++paircounter; //counts # of parenthesis 
            }
            del = strpbrk(del+1, keys); // find nexts delimiter
        }
    }
    if ( paircounter % 2 != 0) { //if not paired up => results in an error
        cout << "Error: Incorrect number of parenthesis or quotations." << endl;
        return;
    }

         
    unsigned int con_index = 0;
    Base* user = build_tree(conns, con_index, str2); // call function for first time
    user->execute(0, 1);

    return;
}


Base* build_tree(vector<char> connects, unsigned int &con_index, char* &cmdstr) {

    Base* prev = NULL;
    Base* helper = NULL;
    bool helperflag = false;
    Base* curr = NULL;
    Base* child = NULL;
    char* token = NULL;
    char d;
    char* del = new char[3];
    // use strtok_r because the inner while loop uses 
    // strtok also, so it messes up outer while loop's 
    // strtok pointers, thus we use strtok_r on outer

    if ( (connects.size() > 0) && (con_index < connects.size()) ) {
        d = connects.at(con_index);
        change_del(del, d);
    }
    
    else {
        d = ')';
        change_del(del, d);
    }
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
             
            child = build_tree(connects, con_index, cmdstr);
            //gets returned a base pointer when ')' is reached
            
            //if there was already a connector at this recursion level
            // then we set child to rhs of prev
            if (prev != NULL) { 
                prev->set_rhs(child);
            }

            ++con_index;

            if ( con_index < connects.size() ) {
                d = connects.at(con_index);
                change_del(del, d);

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
                change_del(del, d);
                
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
        }
        else if (deltemp == "&&") {
            curr = new Ampersand();
            cmdstr = cmdstr + 1;
        }
        else if (deltemp == "||") {
            curr = new Verticalbars();
            cmdstr = cmdstr + 1;
        }
        else if (deltemp == ")") {
            //hit ) => returning from recursion
            if (prev == NULL) {
                return child;
            }
            else {
                if (helperflag == true) {
                    helper->set_rhs(child);
                }
                else {
                    prev->set_rhs(child);
                }
                return prev;
            }
        }
        else if (deltemp == "<") {
            curr = new InputRedirect();
        }
        else if (deltemp == ">") {
            curr = new OutputRedirect();
        }
        else if (deltemp == "|") {
            curr = new Pipe();
        }
        else if (deltemp == ">>") {
            curr = new OutputAppend();
            cmdstr = cmdstr + 1;
        }

        else {
            cout << "something went wrong: choosing connector" << endl;
        }


        // places command and connector objects in correct place
        if (prev == NULL) {
            curr->set_lhs(child);
            prev = curr;
        }
        else { //don't want a command object to call lhs or rhs
            if ( (prev->check_type()) && !(curr->check_type()) ) {
                if (helper == NULL) {
                    curr->set_lhs(child);
                    prev->set_rhs(curr);
                    helper = curr;
                }
                else {
                    //if (helper1->get_rhs() == NULL) {
                    helper->set_rhs(child);
                    curr->set_lhs(helper);
                    prev->set_rhs(curr);
                    helper = curr;
                }
                helperflag = true;
            }
            else if (helperflag == true) {
                helper->set_rhs(child);
                curr->set_lhs(prev);
                helper = NULL;
                helperflag = false;
                prev = curr;
            }
            else {     
                prev->set_rhs(child);
                curr->set_lhs(prev);
                prev = curr;
            }
        }

        con_index = con_index + 1;
        if (con_index < connects.size()) {
            d = connects.at(con_index);
            change_del(del, d);
        }
        else {
            d = ')';
            change_del(del, d);
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

        small = strtok(token, " ");
        while (small != NULL)
        {
            args.push_back(small);
            small = strtok(NULL, " ");
        }

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

void change_del(char* &del, char d) {
    if (d == '!') {
        d = '|';
        del[0] = d;
        del[1] = d;
        del[2] = '\0';
    }
    else if (d == '}') {
        d = '>';
        del[0] = d;
        del[1] = d;
        del[2] = '\0';
    }
    else if (d == '&') {
        del[0] = d;
        del[1] = d;
        del[2] = '\0';
    }
    else if ( (d == '<') || (d == '>') || (d == '|') || (d == '\"') || (d == ';') 
            || (d == '(') || (d == ')') ) {
        del[0] = d;
        del[1] = '\0';
        del[2] = '\0';
    }
    else {
        cout << "Error: no matching delimiter." << endl;
    }
    
    return;
}



