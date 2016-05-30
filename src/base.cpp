#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

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
    pid_t pid;
    bool temp = true;

    pid = fork();

    if(pid == -1) {
    	perror("fork");
    	temp = false;
    }

    else if (pid >= 0) {
        //fork succeeded
        if (pid == 0) {
            //child process
            if(execvp(args[0], args) == -1) {
                perror("execvp");
                temp = false;
                exit(1);
            }    
        }
        else {
            //parent process
            int status;
            if(waitpid(pid, &status, 0) == -1) {
                perror("wait");
            }
            
            if ( WEXITSTATUS(status) != 0 ) {
            	temp = false;
            }
            else {
                temp = true;
            }
        }
    }   
    return temp;
}
bool Command::check_type() {  //checks base ptr to see if it's a connector
    return false;
}
void Command::set_lhs(Base* left) {} //do nothing functions
void Command::set_rhs(Base* right) {}

Test::Test(char** arr) : Command(arr) {
	this->args = arr;
}

bool Test::execute() {
	struct stat sb;

	char flag;
	int pathIndex = 1; // assumes path is the second index

	if(!strcmp(args[0], "-e")) {
		flag = 'e';
	}
	else if(!strcmp(args[0], "-f")) {
		flag = 'f';
	}
	else if(!strcmp(args[0], "-d")) {
		flag = 'd';
	}
	else { // default: if no flag exists, it is -e
		flag = 'e';
		pathIndex = 0; // assumes the path is the first index since there is no flag
	}

	bool fdExists = (stat(args[pathIndex], &sb) == 0);

	if(flag == 'f') {
		if(!fdExists) {
			cout << "(False)" << endl;
			return fdExists;
		}
		else {
			if(S_ISREG(sb.st_mode)) {
				cout << "(True)" << endl;
				return fdExists;
			}
			else {
				cout << "(False)" << endl;
				return false;
			}
		}
	}
	else if(flag == 'd') {
		if(!fdExists) {
			cout << "(False)" << endl;
			return fdExists;
		}
		else {
			if(S_ISDIR(sb.st_mode)) {
				cout << "(True)" << endl;
				return fdExists;
			}
			else {
				cout << "(False)" << endl;
				return false;
			}
		}
	}
	else { // case for -e flag
		if(fdExists) {
			cout << "(True)" << endl;
			return fdExists;
		}
		else {
			cout << "(False)" << endl;
			return fdExists;
		}
	}
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
bool Connector::check_type() { //checks base ptr to see if it's a connector
    return true;
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
   





