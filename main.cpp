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
static bool temp_reg = false;


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

class day {

	int date;
	int num;
	sf::RenderWindow days_window;
	sf::RectangleShape asd;


};

class month {

	int first_dow; //"day of week : 다른 날짜들 요일 계산 가능"
	day dates[31];
	sf::RenderWindow months_window;

public:
	void draws() {
		for (int i = 0; i < 31; i++) {
			months_window.draw(dates[i].);
		}

	}
};




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
bool login_menu(const string& idInput, const string& pwInput) {
	if (main_DB.find_user(idInput) == false) {
		cout << "**ERROR : UserID is wrong**" << idInput << endl;
		return false;
	}
	else {
		if (main_DB.find_index_user(idInput)->get_account().get_PWD() == pwInput) {
			cout << "login success" << endl;
			return true;
		}
		else {
			cout << "Wrong passwd" << endl;
			return false;
		}
	}
}

void Schedule_menu() {
	sf::RenderWindow window(sf::VideoMode(1600, 1200), "My Calendar");
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

int Register_menu() {

	sf::RenderWindow registerWindow(sf::VideoMode(820, 500), "Register");
	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
		return -1;
	//-----------------------------Register Field-------------------------------------
	sf::Text registerIdText("ID: ", font, 40);
	registerIdText.setFillColor(sf::Color::Black);
	registerIdText.setPosition(150, 50);

	sf::RectangleShape registerIdBox(sf::Vector2f(400, 50));
	registerIdBox.setPosition(250, 50);
	registerIdBox.setFillColor(sf::Color::White);
	registerIdBox.setOutlineColor(sf::Color::Black);
	registerIdBox.setOutlineThickness(2.0f);

	sf::Text registerIdString("", font, 40);
	registerIdString.setFillColor(sf::Color::Black);
	registerIdString.setPosition(260, 45);

	// PW 입력 필드, 버튼 등을 생성
	sf::Text registerPwText("PW: ", font, 40);
	registerPwText.setFillColor(sf::Color::Black);
	registerPwText.setPosition(150, 150);

	sf::RectangleShape registerPwBox(sf::Vector2f(400, 50));
	registerPwBox.setPosition(250, 150);
	registerPwBox.setFillColor(sf::Color::White);
	registerPwBox.setOutlineColor(sf::Color::Black);
	registerPwBox.setOutlineThickness(2.0f);

	sf::Text registerPwString("", font, 40);
	registerPwString.setFillColor(sf::Color::Black);
	registerPwString.setPosition(260, 145);

	// PW 재입력 필드, 버튼 등을 생성
	sf::Text registerPwConfirmText("Confirm PW: ", font, 40);
	registerPwConfirmText.setFillColor(sf::Color::Black);
	registerPwConfirmText.setPosition(10, 250);

	sf::RectangleShape registerPwConfirmBox(sf::Vector2f(400, 50));
	registerPwConfirmBox.setPosition(250, 250);
	registerPwConfirmBox.setFillColor(sf::Color::White);
	registerPwConfirmBox.setOutlineColor(sf::Color::Black);
	registerPwConfirmBox.setOutlineThickness(2.0f);

	sf::Text registerPwConfirmString("", font, 40);
	registerPwConfirmString.setFillColor(sf::Color::Black);
	registerPwConfirmString.setPosition(260, 245);
	sf::Vector2i mousPosReg = sf::Mouse::getPosition(registerWindow);
	sf::Vector2f worldPosReg = registerWindow.mapPixelToCoords(mousPosReg);


}

int main(void) {

	setup(main_DB);
	//int cmd;
	bool connect = false;
	string idInput;
	string pwInput;
	string confirmPwInput;


	sf::RenderWindow login(sf::VideoMode(820, 500), "Login");
	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
		return -1;

	//-----------------------------logind Field-------------------------------------
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
	pwText.setPosition(150, 200);
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


	// Register 버튼
	sf::RectangleShape registerButton(sf::Vector2f(200, 100));
	registerButton.setPosition(550, 350);
	registerButton.setFillColor(sf::Color(240, 240, 240));
	registerButton.setOutlineColor(sf::Color::Black);
	registerButton.setOutlineThickness(2.0f);

	sf::Text registerText("Register", font, 40);
	registerText.setFillColor(sf::Color::Black);
	registerText.setPosition(580, 375);

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

	// Confirm 버튼 생성
	sf::RectangleShape confirmButton(sf::Vector2f(200, 100));
	confirmButton.setPosition(310, 350);
	confirmButton.setFillColor(sf::Color(240, 240, 240));
	confirmButton.setOutlineColor(sf::Color::Black);
	confirmButton.setOutlineThickness(2.0f);

	sf::Text confirmText("Confirm", font, 40);
	confirmText.setFillColor(sf::Color::Black);
	confirmText.setPosition(340, 375);


	bool temp = false;

	while (login.isOpen())
	{
		sf::Event event;
		sf::Vector2i mousPos = sf::Mouse::getPosition(login);
		sf::Vector2f worldPos = login.mapPixelToCoords(mousPos);


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
				else if (registerButton.getGlobalBounds().contains(worldPos))
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						registerButton.setFillColor(sf::Color(220, 220, 220));
						temp_reg = true;
					}
				}
				else
				{
					idBox.setOutlineColor(sf::Color::Black);
					pwBox.setOutlineColor(sf::Color::Black);
				}
			}
			//

			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (temp) {
					if (loginButton.getGlobalBounds().contains(worldPos)) {
						loginButton.setFillColor(sf::Color(240, 240, 240));
						bool loginSuccess = login_menu(idInput, pwInput);
						if (loginSuccess) {
							login.close();
							connect = true;
							temp = false;
							temp_reg = true;
						}
					}
					else if (registerButton.getGlobalBounds().contains(worldPos)) {
						registerButton.setFillColor(sf::Color(240, 240, 240));
					}
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
		login.draw(registerButton);
		login.draw(registerText);

		login.display();
	}

	if (connect)
	{
		Schedule_menu();
	}


	return 0;
}
