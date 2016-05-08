#include <iostream>

using namespace std;

#include "base.h"

Base::Base() {};


Command::Command(char ** arr[]) {
    args[] = *arr[];        //no clue if this is how to do it
}
bool Command::execute() {
    bool value;
    pid_t pid = fork();
    if (pid < 0) {
        perror(
    else if (pid = 0) {
        //child performs execution of command
        execvp(args[0], args);
    }
    else {
        //this is the parent process
        
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
   





