#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "main.h"
#include <vector>
#include <SFML/Graphics.hpp>

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
	string get_ID() const { return this->ID; }
	string get_PWD() const { return this->passwd; }
	void set_ID(string id) { this->ID = id; }
	void set_PWD(string pwd) { this->passwd = pwd; }
};

class User {

private:
	Account account;

public:
	Account get_account() const { return this->account; }
	void set_ACCOUNT(Account account) { this->account = account; }
};
class User_DB {

private:
	vector <User> DB;

public:
	bool find_user(string name) {

		vector <User> ::iterator i;
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
	void append_User(User user) { DB.push_back(user); }
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
		cout << "Please enter PASSWORD you want\n> ";
		cin >> STUDENT_PWD;
		cin.ignore();
		while (STUDENT_PWD != check_pwd) {
			cout << "Please enter PASSWORD one more time\n> ";
			cin >> check_pwd;
			cin.ignore();
			if (STUDENT_PWD != check_pwd)cout << "ERROR : Password is not coincide\n" << endl;
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

void setup(User_DB& db) {

	string file_name = USER_LIST;
	ifstream ifs;
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

//sf::Text focusing()
//{
//	
//}


int main(void) {

	//setup(main_DB);
	//int cmd;
	bool connect = false;
	string idInput;
	string pwInput;
	//while (true) {
	//	cout << "1.login 2.register\n> ";
	//	cin >> cmd;
	//	start_menu(cmd);

	//}

	sf::RenderWindow window(sf::VideoMode(1600, 1200), "My Calendar");
	sf::RenderWindow login(sf::VideoMode(820, 500), "Login");
	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
		return -1;


	//ID 메세지 부분
	sf::Text idText("ID: ", font, 40);
	idText.setFillColor(sf::Color::Black);
	idText.setPosition(150, 100);
	//ID 입력 박스
	sf::RectangleShape idBox(sf::Vector2f(400, 50));
	idBox.setPosition(250, 100);
	idBox.setFillColor(sf::Color::White);
	idBox.setOutlineColor(sf::Color::Black);
	idBox.setOutlineThickness(2.0f);
	//ID 텍스트 
	sf::Text idString("", font, 40);
	idString.setFillColor(sf::Color::Black);
	idString.setPosition(260, 95);

	//PW 메시지 부분
	sf::Text pwText("PW: ", font, 40);
	pwText.setFillColor(sf::Color::Black);
	pwText.setPosition(150, 250);
	//PW 입력 박스
	sf::RectangleShape pwBox(sf::Vector2f(400, 50));
	pwBox.setPosition(250, 250);
	pwBox.setFillColor(sf::Color::White);
	pwBox.setOutlineColor(sf::Color::Black);
	pwBox.setOutlineThickness(2.0f);
	//PW 텍스트
	sf::Text pwString("", font, 40);
	pwString.setFillColor(sf::Color::Black);
	pwString.setPosition(260, 245);

	//로그인 버튼
	sf::RectangleShape loginButton(sf::Vector2f(200, 100));
	loginButton.setPosition(310, 350);
	loginButton.setFillColor(sf::Color(240, 240, 240));
	loginButton.setOutlineColor(sf::Color::Black);
	loginButton.setOutlineThickness(2.0f);
	sf::Text textButton("Login", font, 40);
	textButton.setFillColor(sf::Color::Black);
	textButton.setPosition(360, 375);

	sf::Text* focusedText = NULL;

	bool temp = false;

	//로그인창
	while (login.isOpen())
	{
		sf::Event event;
		while (login.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				login.close();
			else if (event.type == sf::Event::TextEntered)		//텍스트 입력 구현
			{
				{
					if (event.text.unicode < 128)
					{
						if (event.text.unicode == 8)
						{
							if (focusedText == &idString)
							{
								if (!idInput.empty())
									idInput.pop_back();
							}
							else if (focusedText == &pwString)
								if (!pwInput.empty())
									pwInput.pop_back();
						}
						else if (event.text.unicode != 13)
							if (focusedText == &idString)
								idInput += static_cast<char>(event.text.unicode);
							else if (focusedText == &pwString)
								pwInput += static_cast<char>(event.text.unicode);
					}
					if (focusedText == &idString)			//비번은 * 로 보이도록 함
						idString.setString(idInput);
					else if (focusedText == &pwString)
						pwString.setString(string(pwInput.length(), '*'));
				}

			}
			else if (event.type == sf::Event::MouseButtonPressed)	//어떤 박스 클릭인지 색깔로 구분
			{
				sf::Vector2i mousPos = sf::Mouse::getPosition(login);
				sf::Vector2f worldPos = login.mapPixelToCoords(mousPos);

				if (idBox.getGlobalBounds().contains(worldPos))
				{
					idBox.setOutlineColor(sf::Color::Red);
					pwBox.setOutlineColor(sf::Color::Black);
					focusedText = &idString;

				}
				else if (pwBox.getGlobalBounds().contains(worldPos))
				{
					idBox.setOutlineColor(sf::Color::Black);
					pwBox.setOutlineColor(sf::Color::Red);
					focusedText = &pwString;
				}
				else if (loginButton.getGlobalBounds().contains(worldPos))
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						loginButton.setFillColor(sf::Color(220, 220, 220));
						temp = true;
					}
				}
				else
				{
					idBox.setOutlineColor(sf::Color::Black);
					pwBox.setOutlineColor(sf::Color::Black);
				}
			}
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (temp)
				{
					loginButton.setFillColor(sf::Color(240, 240, 240));
					temp = false;
				}
			}
		}

		login.clear(sf::Color(218, 218, 218));
		login.draw(idBox);
		login.draw(idText);
		login.draw(idString);
		login.draw(pwBox);
		login.draw(pwText);
		login.draw(pwString);
		login.draw(loginButton);
		login.draw(textButton);

		login.display();
	}
	if (connect)
	{
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear(sf::Color::White);
			window.display();
		}
	}


	return 0;
}
