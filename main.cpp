#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "main.h"
#include <vector>

using namespace std;

//define

#define MAX_TODO 100
#define MAX_USER_SIZE 100
#define USER_LIST "UserList.txt"

//global variables

ifstream ifs;
static string STUDENT_ID;
static string STUDENT_PWD;
static int mode = 0; // 0 : default, 1: login, 2: register 



//class User
class Account {

private:
	string ID;
	string passwd;

public:
	 string get_ID() const{ return this->ID; }
	 string get_PWD() const { return this->passwd; }
	 void set_ID(string id) { this->ID = id; }
	 void set_PWD(string pwd) { this->passwd = pwd; }
};

class User {

private:
	Account account;

public :
	 Account get_account() const{ return this->account; }
	 void set_ACCOUNT(Account account) { this->account = account; }
};
class User_DB{

private:	
	vector <User> DB;

public:
	bool find_user(string name) {

		vector <User> :: iterator i;
		for (i = DB.begin(); i != DB.end(); i++) {
			if (i->get_account().get_ID() == name) {
				return true;
			}
		}
		return false;
	}

	vector <User> ::iterator find_index_user(string name) {
		vector <User> ::iterator i;
		for (i = DB.begin(); i != DB.end(); i++) {
			if (i->get_account().get_ID() == name) {
				return i;
			}
		}
		return this->DB.end();
	}
	void append_User(User user) {DB.push_back(user);}
};

//global variables 2

User_DB main_DB;

//functions

vector <string> tokenize(string context, char target) {
	vector <string> token;
	stringstream ss(context);
	string tok;

	while (getline(ss, tok, target)) {
		token.push_back(tok);
	}
	return token;
}


void control_user_list() {

}

void login_menu() {

	cout << "Enter UserID\n> ";
	cin >> STUDENT_ID;
	cin.ignore();
	if (main_DB.find_user(STUDENT_ID) == false) {
		cout << "**ERROR : UserID is wrong**";
	}
	else {
		cout << "Enter Password\n> ";
		string pwd;
		cin >> pwd;
		cin.ignore();
		if (main_DB.find_index_user(STUDENT_ID)->get_account().get_PWD() == pwd) {
			cout << "login success\n";
		}
		else {
			cout << "Wrong passwd\n";

		}
	}
}

void register_menu() {
	
	User user;
	Account account;

	string check_pwd;
	
	cout << "Register_menu\n\nPlease enter ID you want\n> ";
	cin >> STUDENT_ID;
	cin.ignore();
	if (main_DB.find_user(STUDENT_ID) == true) {
		cout << "ERROR : That ID is Already existed\n" << endl;
	}
	else {
		cout <<"Please enter PASSWORD you want\n> ";
		cin >> STUDENT_PWD;
		cin.ignore();
		while (STUDENT_PWD != check_pwd) {
			cout << "Please enter PASSWORD one more time\n> ";
			cin >> check_pwd;
			cin.ignore();
			if(STUDENT_PWD != check_pwd)cout << "ERROR : Password is not coincide\n" << endl;
		}
		cout << "success";

		account.set_ID(STUDENT_ID);
		account.set_PWD(STUDENT_PWD);
		user.set_ACCOUNT(account);
		main_DB.append_User(user);

		string user_set = "\n" + STUDENT_ID + " " + STUDENT_PWD;
		ofstream ofs;
		ofs.open(USER_LIST, ios::app); // Open file in append mode
		if (ofs.is_open()) {
			ofs << user_set << endl; // Use << operator to write string
		}
		ofs.close();
	}
}

void start_menu(int cmd) {

	switch (cmd) {
	case 1:
		login_menu();
		break;

	case 2:
		register_menu();
		break;
	case 3:
		exit(1);
		break;
	default:
		cout << "It's wrong Input\n1.login 2.register 3. exit\n> ";
		break;
	}
}

void setup(User_DB & db) {

	string file_name = USER_LIST;
	ifstream ifs1;
	ifs.open(file_name);

	if (ifs.is_open()) {
		string str;
		User user;
		Account account;
		vector <string> token;
		bool find = false;
		//cout << "Open " + file_name <<endl;
			while (getline(ifs, str)) {
				if (!str.empty()) {  // 비어있지 않은 경우만 처리
					token = tokenize(str, ' ');
					account.set_ID(token[0]);
					account.set_PWD(token[1]);
					user.set_ACCOUNT(account);
					db.append_User(user);
				}
		}
	}
	else {
		cout << "**ERROR : File Name is wrong**" << endl;
	}
	ifs.close();
}

int main(void) {

	setup(main_DB);
	int cmd;
	while (true) {
		cout << "1.login 2.register\n> ";
		cin >> cmd;
		start_menu(cmd);
		
	}

	return 0;
}
