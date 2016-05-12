#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#include "base.h"

Base::Base() {};


Command::Command(char ** arr[]) {
    args = *arr[];        //no clue if this is how to do it
}
bool Command::execute() {
    pid_t child_pid;
    int status;

    child_pid = fork();

    if (pid >= 0) {
        //fork succeeded
        if (child_pid == 0) {
            //child process
            execvp(args[0], args);
            printf("Unknown command!\n");
            exit(0);    
        }
        else {
            //parent process
            waitpid(child_pid, &status, 0);
            
            if (WEXITSTATUS(status)   
                return true;
        }
    }   
    else {
        //failure
        perror("fork");
        return false;
    }    
}

Exit::


Connector::Connector(Base* left, Base* right) {
    this->lhs = left;
    this->rhs = right;
}

Base* Connector::get_lhs() {
    return lhs;
}
Base* Connector::get_rhs() {
    return rhs;
}

Semicolon::Semicolon(Base* left, Base* right)
    : Connector(left, right) {};

bool Semicolon::execute() {
    get_lhs()->execute();
    return get_rhs()->execute();
}

Ampersand::Ampersand(Base* left, Base* right)
    : Connector(left, right) {};

bool Ampersand::execute() {
    if (get_lhs()->execute() == true) {
        return get_rhs()->execute();
    }
    else {
        return false;
    }
}
Verticalbars::Verticalbars(Base* left, Base* right)
    : Connector(left, right) {};

bool Verticalbars::execute() {
     if (get_lhs()->execute() == true) {
        return true;
    }
    else {
        return get_rhs()->execute();
    }
}
   





