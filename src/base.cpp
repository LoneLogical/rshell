#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#include "base.h"

Base::Base() {}

Command::Command(char** arr) {
    this->args = arr;
}
bool Command::execute(int inputfd, int outputfd) {
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
            //1st dup2 call changes input value of command
            //2nd dup2 call changes output value of command
            //if inputfd == 0 or outputfd == 1, nothing gets changed for that call
            if (dup2(inputfd, STDIN_FILENO) == -1) {
                perror("dup2");
                temp = false;
            }
            if (dup2(outputfd, STDOUT_FILENO) == -1) {
                perror("dup2");
                temp = false;
            }

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
char** Command::get_info() {
    return this->args;
}
void Command::set_lhs(Base* left) {} //do nothing functions
void Command::set_rhs(Base* right) {}

Test::Test(char** arr) : Command(arr) {
	this->args = arr;
}

bool Test::execute(int inputfd, int outputfd) {
	struct stat sb;
	char flag;
	int pathIndex = 1; // assumes path is the second index

	if(strcmp(args[0], "-e") == 0) {
		flag = 'e';
	}
	else if(strcmp(args[0], "-f") == 0) {
		flag = 'f';
	}
	else if(strcmp(args[0], "-d") == 0) {
		flag = 'd';
	}
	else { // default: if no flag exists, it is -e
		flag = 'e';
		pathIndex = 0; // assumes the path is the first index since there is no flag
	}

	bool fdExists = (stat(args[pathIndex], &sb) == 0);

    if (dup2(outputfd, STDOUT_FILENO) == -1) {
        perror("dup2");
        return false;
    }

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
bool Exit::execute(int inputfd, int outputfd) {
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
char** Connector::get_info() {return NULL;} //do nothing function


Semicolon::Semicolon() : Connector() {}
bool Semicolon::execute(int inputfd, int outputfd) {
    get_lhs()->execute(inputfd, outputfd);
    return get_rhs()->execute(inputfd, outputfd);
}
bool Semicolon::check_type() {
    return true;
}

Ampersand::Ampersand() : Connector() {}
bool Ampersand::execute(int inputfd, int outputfd) {
    if (get_lhs()->execute(inputfd, outputfd) == true) {
        return get_rhs()->execute(inputfd, outputfd);
    }
    else {
        return false;
    }
}
bool Ampersand::check_type() {
    return true;
}

Verticalbars::Verticalbars() : Connector() {}
bool Verticalbars::execute(int inputfd, int outputfd) {
    if (get_lhs()->execute(inputfd, outputfd) == true) {
        return true;
    }
    else {
        return get_rhs()->execute(inputfd, outputfd);
    }
}
bool Verticalbars::check_type() {
    return true;
}

InputRedirect::InputRedirect() : Connector() {}
bool InputRedirect::execute(int inputfd, int outputfd) {
    //rhs should be a textfile, so we can't call execvp on it
    //instead, we just grab the file name and call open here
    char** temparr = get_rhs()->get_info();
    
    int infd = open(temparr[0], O_RDONLY);
    if (infd == -1) {
        perror("open");
        return false;
    }
    return get_lhs()->execute(infd, outputfd);
}
bool InputRedirect::check_type() {
    return false;
}

OutputRedirect::OutputRedirect() : Connector() {}
bool OutputRedirect::execute(int inputfd, int outputfd) {
    //rhs should be a textfile, so we can't call execvp on it
    //instead, we just grab the file name and call open here
    char** temparr = get_rhs()->get_info();

    int outfd = open(temparr[0], O_WRONLY | O_TRUNC | O_CREAT,
            S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    if (outfd == -1) {
        perror("open");
        return false;
    }
    return get_lhs()->execute(inputfd, outfd); 
}
bool OutputRedirect::check_type() {
    return false;
}

OutputAppend::OutputAppend() : Connector() {}
bool OutputAppend::execute(int inputfd, int outputfd) {
    //rhs should be a textfile, so we can't call execvp on it
    //instead, we just grab the file name and call open here
    char** temparr = get_rhs()->get_info();

    int outfd = open(temparr[0], O_WRONLY | O_APPEND | O_CREAT,
            S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    if (outfd == -1) {
        perror("open");
        return false;
    }
    return get_lhs()->execute(inputfd, outfd);
}
bool OutputAppend::check_type() {
    return false;
}
   
Pipe::Pipe() : Connector() {}
bool Pipe::execute(int inputfd, int outputfd) {
    int pipefd[2];
    bool truth;

    //pipe the array to allocate the file descriptors
    if (pipe(pipefd) == -1) {//perror and return false;
        perror("pipe");
        return false;
    }    
    //have left hand side's execute output to write end of pipe 
    truth = get_lhs()->execute(inputfd, pipefd[1]);
    if (truth == false) { 
        return false;
    }
     
    if (close(pipefd[1]) == -1) {
        perror("close");
        return false;
    }
    
    truth = get_rhs()->execute(pipefd[0], outputfd);

    if (close(pipefd[0]) == -1) {
        perror("close");
        return false;
    }

    if (truth == false) {
       return false;
    }
    
    return true;
}
bool Pipe::check_type() {
    return false;
}



