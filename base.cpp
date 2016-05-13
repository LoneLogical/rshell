#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;

#include "base.h"

Base::Base() {}

Command::Command(char** arr) {
    //int size = arr.size();
    //char* arr1[] = new char* array[size];
    //for (unsigned int i = 0; i < size; ++i) {
    //    arr1[i] = arr.at(i);
    //}
    this->args = arr;
}
bool Command::execute() {
    pid_t child_pid;
    int status;
    bool temp = true;

    child_pid = fork();

    if(child_pid == -1) {
    	perror("fork");
    	temp = false;
    }

    else if (child_pid >= 0) {
        //fork succeeded
        if (child_pid == 0) {
            //child process
            if(execvp(args[0], args) == -1) {
                perror("execvp");
                exit(1);
            }    
        }
        else {
            //parent process
            if(waitpid(child_pid, &status, 0) == -1) {
                perror("wait");
            }
            
            if ( WEXITSTATUS(status) == 0 ) {
            	temp = true;
            }
            else {
                temp = false;;
            }
        }
    }   
    return temp;
}

Exit::Exit(char** arr) : Command(arr) {}
bool Exit::execute() {
   exit(0);
} 

Connector::Connector() : Base() {}
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

Semicolon::Semicolon() : Connector() {}
bool Semicolon::execute() {
    get_lhs()->execute();
    return get_rhs()->execute();
}

Ampersand::Ampersand() : Connector() {}
bool Ampersand::execute() {
    if (get_lhs()->execute() == true) {
        return get_rhs()->execute();
    }
    else {
        return false;
    }
}

Verticalbars::Verticalbars() : Connector() {}
bool Verticalbars::execute() {
     if (get_lhs()->execute() == true) {
        return true;
    }
    else {
        return get_rhs()->execute();
    }
}
   





