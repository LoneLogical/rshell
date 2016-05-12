#include <iostream>
#include <string>
#include "base.h"
#include <vector>

using namespace std;

int main() {
	string cmd;
	cout << "$ ";
	getline(cin, cmd);

	stringstream data;
	data << cmd;
	
	string tempData;
	vector<string> temp_arr;

	bool firstConnector = false;

	Base* left = NULL;
	Base* prev = NULL;

	while(!firstConnector) {
		data >> tempData;
		if(tempData.back() == ';') {
			// finish off the buffer
			tempData.pop_back();
			temp_arr.push_back(tempData);

			// move contents of buffer into a new command's data field
			char* cmd_arr[temp_arr.size()];
			for(int i = 0; i < temp_arr.size(); ++i) {
				cmd_arr[i] = temp_arr.at(i).c_str();
			}

			// create new Command and Semicolon objects initialized with array
			left = new Command(cmd_arr);
			prev = new Semicolon();
			prev->set_LHS(left);

			// clear vector
			temp_arr.clear();
			firstConnector = true;
		}
		else if(tempData == "&&") {
			char* cmd_arr[temp_arr.size()];
			for(int i = 0; i < temp_arr.size(); ++i) {
				cmd_arr[i] = temp_arr.at(i).c_str();
			}

			left = new Command(cmd_arr);
			prev = new Ampersand();
			prev->set_LHS(left);

			temp_arr.clear();
			firstConnector = true;
		}

		else if(tempData == "||") {
			char* cmd_arr[temp_arr.size()];
			for(int i = 0; i < temp_arr.size(); ++i) {
				cmd_arr[i] = temp_arr.at(i).c_str();
			}

			left = new Command(cmd_arr);
			prev = new Verticalbars();
			prev->set_LHS(left);

			temp_arr.clear();
			firstConnector = true;
		}
		else {
			temp_arr.push_back(tempData);
		}
	}

	while(data >> tempData) {
		if(tempData.back() == ';') {
			tempData.pop_back();
			Connector* a = new Semicolon();
			a->set_lhs(temp_arr);
			// clear temp array
		}
		else {
			temp_arr.push_back();
		}


	cout << "cmd: " << cmd << endl;
	return 0;
}
// this git works
