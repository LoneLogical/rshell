#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#include "base.h"

Base::Base() {};
Base::~Base() {};

Command::Command(char ** arr[]) {
    args = *arr[];        //no clue if this is how to do it
}
Command::~Command() {
    delete[] args;
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
            
            if ( WEXITSTATUS(status) >= 0 ) {
                return true;
            }
            else {
                return false;
            }
        }
    }   
    else {
        //failure
        perror("fork");
        return false;
    }    
}

Exit::Exit() {

}
Exit::~Exit() {
    
}

Connector::Connector() : Base() {};
Connector::~Connector() {};
}
Base* Connector::get_lhs() {
    return lhs;
}
Base* Connector::get_rhs() {
    return rhs;
}
void Connector::set_lhs(Base* left) {
    this->lhs = left;
}
void Connector::set_rhs(Base* right) {
    this->rhs = right;
}

Semicolon::Semicolon() : Connector() {};
Semicolon::~Semicolon() {
    delete rhs; 
    delete lhs;
}
bool Semicolon::execute() {
    get_lhs()->execute();
    return get_rhs()->execute();
}

Ampersand::Ampersand() : Connector() {};
Ampersand::~Ampersand() {
    delete rhs;
    delete lhs;
}
bool Ampersand::execute() {
    if (get_lhs()->execute() == true) {
        return get_rhs()->execute();
    }
    else {
        return false;
    }
}

Verticalbars::Verticalbars() : Connector() {};
Verticalbars::~Verticalbars() {
    delete rhs;
    delete lhs;
}
bool Verticalbars::execute() {
     if (get_lhs()->execute() == true) {
        return true;
    }
    else {
        return get_rhs()->execute();
    }
}
   





