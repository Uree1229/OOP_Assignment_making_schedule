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

static int mode = 0; // 0 : default, 1: login, 2: register 
static bool temp_reg = false;
string EXTERN_STUDENT_ID;
static bool connect;
static bool reg;
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



std::vector <string> tokenize(string context, char target) {
	vector <string> token;
	stringstream ss(context);
	string tok;

	while (getline(ss, tok, target)) {
		token.push_back(tok);
	}
	return token;
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
			if (!str.empty()) {  // ºñ¾îÀÖÁö ¾Ê: °æ¿츸 ó¸®
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


bool login_menu(const string& idInput, const string& pwInput) {
	if (main_DB.find_user(idInput) == false) {
		cout << "**ERROR : UserID is wrong**" << idInput << endl;
		return false;
	}
	else {
		if (main_DB.find_index_user(idInput)->get_account().get_PWD() == pwInput) {
			cout << "login success" << endl;
			STUDENT_ID = idInput;
			return true;
		}
		else {
			cout << "Wrong passwd" << endl;
			return false;
		}
	}
}
bool Register_menu(const string& idInput, const string& pwInput, const string& confirmInput) {
	if (main_DB.find_user(idInput) == true) {
		cout << "**ERROR : UserID is already existed**" << endl;
		return false;
	}
	else {
		User user;
		Account account;

		string check_pwd;

		account.set_ID(idInput);
			account.set_PWD(pwInput);
			user.set_ACCOUNT(account);
			main_DB.append_User(user);

			string user_set = "\n"+idInput + " " + pwInput;
			ofstream ofs;
			ofs.open(USER_LIST, ios::app); // Open file in append mode
			if (ofs.is_open()) {
				ofs << user_set; // Use << operator to write string
			}
			ofs.close();
			ofstream out(idInput+".txt");
			cout << " add success";
	}
}

int Register_menu() {

	bool temp = false;
	string idInput;
	string pwInput;
	string confirmPwInput;

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

	// PW ÀԷÂ Çʵå, ¹öư µî; »ý¼º
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

	// PW ÀçÀԷÂ Çʵå, ¹öư µî; »ý¼º
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

	// Confirm ¹öư »ý¼º
	sf::RectangleShape confirmButton(sf::Vector2f(200, 100));
	confirmButton.setPosition(310, 350);
	confirmButton.setFillColor(sf::Color(240, 240, 240));
	confirmButton.setOutlineColor(sf::Color::Black);
	confirmButton.setOutlineThickness(2.0f);

	sf::Text confirmText("Confirm", font, 40);
	confirmText.setFillColor(sf::Color::Black);
	confirmText.setPosition(340, 375);

	sf::Text* focusedText = NULL;
	sf::Vector2i mousPosReg = sf::Mouse::getPosition(registerWindow);
	sf::Vector2f worldPosReg = registerWindow.mapPixelToCoords(mousPosReg);

	while (registerWindow.isOpen()) {

			sf::Event event;
			sf::Vector2i mousPos = sf::Mouse::getPosition(registerWindow);
			sf::Vector2f worldPos = registerWindow.mapPixelToCoords(mousPos);


			while (registerWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed){
					registerWindow.close();
					reg = false;
				}	
				else if (event.type == sf::Event::TextEntered)		//ÅؽºƮ ÀԷÂ ±¸Çö
				{
					{
						if (event.text.unicode < 128)
						{
							if (event.text.unicode == 8)
							{
								if (focusedText == &registerIdString)
									if (!idInput.empty()) idInput.pop_back();
							
								else if (focusedText == &registerPwString)
									if (!pwInput.empty()) pwInput.pop_back();
								
								else if (focusedText == &registerPwConfirmString) 
									if (!confirmPwInput.empty()) confirmPwInput.pop_back();
							
							}
							else if (event.text.unicode != 13){
								if (focusedText == &registerIdString){
									idInput += static_cast<char>(event.text.unicode);
									registerIdString.setString(idInput);
								}
								else if (focusedText == &registerPwString){
									pwInput += static_cast<char>(event.text.unicode);
									registerPwString.setString(string(pwInput.length(), '*'));
								}
								else if (focusedText == &registerPwConfirmString){
									confirmPwInput += static_cast<char>(event.text.unicode);
									registerPwConfirmString.setString(string(confirmPwInput.length(), '*'));
								}
							}
						}
	
					}

				}
				else if (event.type == sf::Event::MouseButtonPressed)	//¾ ¹ڽº Ŭ¸¯ÀÎÁö »ö±ò·Î ±¸ºÐ
				{


					if (registerIdBox.getGlobalBounds().contains(worldPos))
					{
						registerIdBox.setOutlineColor(sf::Color::Red);
						registerPwBox.setOutlineColor(sf::Color::Black);
						registerPwConfirmBox.setOutlineColor(sf::Color::Black);
						focusedText = &registerIdString;

					}
					else if (registerPwBox.getGlobalBounds().contains(worldPos))
					{
						registerIdBox.setOutlineColor(sf::Color::Black);
						registerPwBox.setOutlineColor(sf::Color::Red);
						registerPwConfirmBox.setOutlineColor(sf::Color::Black);
						focusedText = &registerPwString;
					}
					else if (registerPwConfirmBox.getGlobalBounds().contains(worldPos))
					{
						registerIdBox.setOutlineColor(sf::Color::Black);
						registerPwBox.setOutlineColor(sf::Color::Black);
						registerPwConfirmBox.setOutlineColor(sf::Color::Red);
						focusedText = &registerPwConfirmString;
					}
					else if (confirmButton.getGlobalBounds().contains(worldPos))
					{
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							confirmButton.setFillColor(sf::Color(220, 220, 220));
							temp = true;
						}
					}
					else
					{
						registerIdBox.setOutlineColor(sf::Color::Black);
						registerPwBox.setOutlineColor(sf::Color::Black);
						registerPwConfirmBox.setOutlineColor(sf::Color::Black);

					}
				}
				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (temp) {
						if (confirmButton.getGlobalBounds().contains(worldPos)) {
							confirmButton.setFillColor(sf::Color(240, 240, 240));
							if (pwInput != confirmPwInput) {
								cout << "**Error: pwd and confirm pwd is not same**";
							}
							else {
								bool RegisterSuccess = Register_menu(idInput, pwInput, confirmPwInput);
								if (RegisterSuccess) {
									registerWindow.close();
									reg = false;
									temp = false;
								}
							}

							
						}
						temp = false;
					}

				}


			}

			registerWindow.clear(sf::Color(218, 218, 218));
			registerWindow.draw(registerIdBox);
			registerWindow.draw(registerIdText);
			registerWindow.draw(registerIdString);
			registerWindow.draw(registerPwBox);
			registerWindow.draw(registerPwText);
			registerWindow.draw(registerPwString);
			registerWindow.draw(registerPwConfirmBox);
			registerWindow.draw(registerPwConfirmText);
			registerWindow.draw(registerPwConfirmString);

			registerWindow.draw(confirmButton);
			registerWindow.draw(confirmText);


			registerWindow.display();
		}
	
}

int main_menu() {
	connect = false;
	string idInput;
	string pwInput;


	sf::RenderWindow login(sf::VideoMode(820, 500), "Login");
	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
		return -1;

	//-----------------------------logind Field-------------------------------------
	//ID ¸޼¼Áö ºκÐ
	sf::Text idText("ID: ", font, 40);
	idText.setFillColor(sf::Color::Black);
	idText.setPosition(150, 100);
	//ID ÀԷÂ ¹ڽº
	sf::RectangleShape idBox(sf::Vector2f(400, 50));
	idBox.setPosition(250, 100);
	idBox.setFillColor(sf::Color::White);
	idBox.setOutlineColor(sf::Color::Black);
	idBox.setOutlineThickness(2.0f);
	//ID ÅؽºƮ 
	sf::Text idString("", font, 40);
	idString.setFillColor(sf::Color::Black);
	idString.setPosition(260, 95);

	//PW ¸޽ÃÁö ºκÐ
	sf::Text pwText("PW: ", font, 40);
	pwText.setFillColor(sf::Color::Black);
	pwText.setPosition(150, 200);
	//PW ÀԷÂ ¹ڽº
	sf::RectangleShape pwBox(sf::Vector2f(400, 50));
	pwBox.setPosition(250, 250);
	pwBox.setFillColor(sf::Color::White);
	pwBox.setOutlineColor(sf::Color::Black);
	pwBox.setOutlineThickness(2.0f);
	//PW ÅؽºƮ
	sf::Text pwString("", font, 40);
	pwString.setFillColor(sf::Color::Black);
	pwString.setPosition(260, 245);


	// Register ¹öư
	sf::RectangleShape registerButton(sf::Vector2f(200, 100));
	registerButton.setPosition(550, 350);
	registerButton.setFillColor(sf::Color(240, 240, 240));
	registerButton.setOutlineColor(sf::Color::Black);
	registerButton.setOutlineThickness(2.0f);

	sf::Text registerText("Register", font, 40);
	registerText.setFillColor(sf::Color::Black);
	registerText.setPosition(580, 375);

	//·α×ÀÎ ¹öư
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

	while (login.isOpen())
	{
		sf::Event event;
		sf::Vector2i mousPos = sf::Mouse::getPosition(login);
		sf::Vector2f worldPos = login.mapPixelToCoords(mousPos);


		while (login.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				login.close();
			else if (event.type == sf::Event::TextEntered)		//ÅؽºƮ ÀԷÂ ±¸Çö
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
					if (focusedText == &idString)			//ºñ¹ø: * ·Î º¸À̵µ·Ï ÇÔ
						idString.setString(idInput);
					else if (focusedText == &pwString)
						pwString.setString(string(pwInput.length(), '*'));
				}

			}
			else if (event.type == sf::Event::MouseButtonPressed)	//¾ ¹ڽº Ŭ¸¯ÀÎÁö »ö±ò·Î ±¸ºÐ
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
						temp = true;
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
							
						}
					}
					else if (registerButton.getGlobalBounds().contains(worldPos)) {
						registerButton.setFillColor(sf::Color(240, 240, 240));
						if (reg == false) {
							reg = true;
							Register_menu();
							
						}
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
}

int main(void) {

	setup(main_DB);
	main_menu();

	if (connect)
	{
		
		EXTERN_STUDENT_ID = STUDENT_ID;
		schedule_main();
	}


	return 0;
}