#define _CRT_SECURE_NO_WARNINGS
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
#define XSIZE 115
#define YSIZE 60

//global variables
static int month = 12;
const int months[12] = { 31, 28 ,31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static int xpos[7] = { 150, 295, 440, 585, 730, 875, 1020 };
static int ypos[5] = { 180,290,400,510,620 };
static int textx[7] = { 187, 332, 477, 622, 767, 912, 1057 };
static int texty[5] = { 180,290,400,510,620 };



std::vector <string> tokenize(string context, char target);

class Todo {
public:
    void set_month(int month) { this->month = month; }
    void set_day(int day) { this->day = day; }
    void set_todo(string todo) { this->todo = todo; }
    void set_time(string time) { this->time = time; }
    void set_data(int month, int day, string todo, string time) {
        this->month = month;
        this->day = day;
        this->todo = todo;
        this->time = time;
    }

    int const get_month() { return this->month; }
    int const get_day() { return this->day; }
    string const get_todo() const { return this->todo; }
    string const get_time() const { return this->time; }

    void print() {
        cout << "Month : " << month << " Date : " << day << " Time : " <<time << endl;
        cout << "Todo : " << todo << endl;
    }
private:
    int month;
    int day;
    string todo;
    string time;
};

class Day {
public:
    vector <Todo> TodoList;
    int num; // num of Todo


    bool check(string time) {
        vector<Todo>::iterator p;
        for (p = TodoList.begin(); p != TodoList.end(); p++) {
            if (p->get_time() == time) return true;
            
        }
        return false;
    }
    void print() {
        vector<Todo>::iterator p;
        for (p = TodoList.begin(); p != TodoList.end(); p++) {
            cout << "Month : " << p->get_month() << " Date : " << p->get_day() << " Time : " << p->get_time()<< endl;
            cout << "Todo : " << p->get_todo() <<endl;
        }
    }
};

class Month {
public:
    Month(int month) {
        for (int i = 0; i < months[month]; i++) {
            Day day = Day();
            Todo todo;
            todo.set_data(month + 1, i + 1, "Nothing", ""); // °¢ ³¯¥¿¡ ´ëÇÑ ±⺻ Todo °´ü »ý¼º
            day.TodoList.push_back(todo);  // ±⺻ Todo °´ü¸¦ TodoList¿¡ Ã߰¡
            dayList.push_back(day);
        }
    }


    vector <Day> dayList;

};


class Year {
public:
    Year() {
        for (int i = 0; i < 12; i++) {
            Month month = Month(i);
            monthList.push_back(month);
        }
    }
    void setting() {
        
        ifstream ifs;
        string file_name = EXTERN_STUDENT_ID + ".txt";
        ifs.open(file_name);

        if (ifs.is_open()) {
            string str;
            vector <string> token;
            while (getline(ifs, str)) {
                if (!str.empty()) {  // ºñ¾îÀÖÁö ¾Ê: °æ¿츸 ó¸®
                    token = tokenize(str, ' ');
                    Todo todo;
                    todo.set_data(stoi(token[0]), stoi(token[1]), token[3], token[2]);
                    todo.print();
                    monthList[stoi(token[0])-1].dayList[stoi(token[1])-1].TodoList.push_back(todo);
                }
            }
            //printAll();
            ifs.close();
        }
        else {
            cout << "**ERROR : File Name is wrong**" << endl;
            return;
        }
    }
    void printTodoList(int month, int day) {
        if (month >= 1 && month <= 12 && day >= 1 && day <= months[month - 1]) {
            for (vector<Day>::iterator p = monthList[month].dayList.begin(); p != monthList[month].dayList.end(); p++) {
                monthList[month - 1].dayList[day - 1].print();
            }
            
        }
    }
    void printAll() {
        for (int i = 0; i < 12; i++) {
            for (vector<Day>::iterator p = monthList[i].dayList.begin(); p!= monthList[i].dayList.end();p++){
                p->print();
            }
            
        }
    }
private:
    vector<Month> monthList;
};




sf::RectangleShape dayButtons[31];
sf::Text dayText[31];
sf::Text monthText[12];
int buttonNum = -1;

bool isButtonPressed(sf::Vector2f worldPos)
{
    for (int i = 0; i < 31; i++)
        if (dayButtons[i].getGlobalBounds().contains(worldPos))
        {
            buttonNum = i;
            return true;
        }
    return false;
}

int schedule_main(void) {

    int control = 0;
    int week = 0;
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        return -1;

    //dayButton 31°³, dayText 31°³
    for (int i = 0; i < 31; i++)
    {
        week = i / 7;
        sf::RectangleShape obj(sf::Vector2f(XSIZE, YSIZE)); //XSIZE, YSIZE
        obj.setPosition(xpos[i % 7], ypos[week]); //xpos, ypos
        obj.setFillColor(sf::Color(218, 218, 218));
        obj.setOutlineColor(sf::Color::Black);
        obj.setOutlineThickness(2.0f);
        dayButtons[i] = obj;

        string strday = to_string(i + 1);
        sf::Text textObj(strday, font, 40);
        textObj.setFillColor(sf::Color::Black);
        textObj.setPosition(textx[i % 7], texty[week]); //text x,yÁÂǥ
        dayText[i] = textObj;
    }

    //ÇÑÀڸ´¼ö dayText 'ġ vd
    for (int i = 0; i < 9; i++)
    {
        week = i / 7;
        string strday = to_string(i + 1);
        sf::Text textObj(strday, font, 40);
        textObj.setFillColor(sf::Color::Black);
        textObj.setPosition(textx[i % 7] + 8, texty[week]); //text x,yÁÂǥ
        dayText[i] = textObj;
    }

    //monthText 9°³(ÇÑÀڸ´¼ö)
    for (int i = 0; i < 9; i++)
    {
        string mText = to_string(i + 1);
        sf::Text textObj(mText, font, 120);
        textObj.setFillColor(sf::Color::Black);
        textObj.setPosition(600, 10); //text x,yÁÂǥ
        monthText[i] = textObj;
    }
    //monthText 3°³(µÎÀڸ´¼ö)
    for (int i = 9; i < 12; i++)
    {
        string mText = to_string(i + 1);
        sf::Text textObj(mText, font, 120);
        textObj.setFillColor(sf::Color::Black);
        textObj.setPosition(570, 10); //text x,yÁÂǥ
        monthText[i] = textObj;
    }

    //¿ÞÂÊ ȭ»ìǥ
    sf::RectangleShape leftArrow(sf::Vector2f(200, 100));
    leftArrow.setPosition(100, 40); //xpos, ypos
    leftArrow.setFillColor(sf::Color::Green);
    leftArrow.setOutlineColor(sf::Color::Black);
    leftArrow.setOutlineThickness(3.0f);

    sf::Text lArrow("<<", font, 70);
    lArrow.setFillColor(sf::Color::Black);
    lArrow.setPosition(150, 50);

    //¿8¥ÂÊ ȭ»ìǥ
    sf::RectangleShape rightArrow(sf::Vector2f(200, 100));
    rightArrow.setPosition(1000, 40); //xpos, ypos
    rightArrow.setFillColor(sf::Color::Green);
    rightArrow.setOutlineColor(sf::Color::Black);
    rightArrow.setOutlineThickness(3.0f);

    sf::Text rArrow(">>", font, 70);
    rArrow.setFillColor(sf::Color::Black);
    rArrow.setPosition(1060, 50);



    sf::RenderWindow scheWindow(sf::VideoMode(1280, 720), "My Calendar");
    
    Year year;
    year.setting();

    while (scheWindow.isOpen())
    {
   
       
        
        sf::Event event;
        sf::Vector2i mousPos = sf::Mouse::getPosition(scheWindow);
        sf::Vector2f worldPos = scheWindow.mapPixelToCoords(mousPos);

        sf::Vector2i mousPosReg = sf::Mouse::getPosition(scheWindow);
        sf::Vector2f worldPosReg = scheWindow.mapPixelToCoords(mousPos);

        while (scheWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                scheWindow.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (leftArrow.getGlobalBounds().contains(worldPos))
                {
                    if (month > 1)
                    {
                        scheWindow.clear(sf::Color(240, 240, 240));
                        month--;
                    }
                }
                else if (rightArrow.getGlobalBounds().contains(worldPos))
                {
                    if (month < 12)
                    {
                        scheWindow.clear(sf::Color(240, 240, 240));
                        month++;
                    }
                }
                else if (isButtonPressed(worldPos))
                {
                    
                    char buf[256];

                    sprintf(buf, "Month : %d, Day : %d", month, buttonNum + 1);
                    sf::RenderWindow buttonWindow(sf::VideoMode(600, 400), buf);
                    
                    year.printTodoList(month, buttonNum + 1);
                    
                    while (buttonWindow.isOpen())
                    {
                        sf::Event event1;
                        sf::Vector2i mousPos1 = sf::Mouse::getPosition(scheWindow);
                        sf::Vector2f worldPos1 = scheWindow.mapPixelToCoords(mousPos);

                        sf::Vector2i mousPosReg1 = sf::Mouse::getPosition(scheWindow);
                        sf::Vector2f worldPosReg1 = scheWindow.mapPixelToCoords(mousPos);

                        while (buttonWindow.pollEvent(event1))
                        {

                            if (event1.type == sf::Event::Closed)
                                buttonWindow.close();
                        }
                    }
                }
            }

        }
        scheWindow.clear(sf::Color(240, 240, 240));

        //button draw
        for (int i = 0; i < months[month - 1]; i++)
        {
            scheWindow.draw(dayButtons[i]);
            scheWindow.draw(dayText[i]);
        }

        //leftArrow draw
        if (month != 1)
        {
            scheWindow.draw(leftArrow);
            scheWindow.draw(lArrow);
        }

        //rightArrow draw
        if (month != 12)
        {
            scheWindow.draw(rightArrow);
            scheWindow.draw(rArrow);
        }

        //month draw
        scheWindow.draw(monthText[month - 1]); //static month À̿ëÇؼ­ ÇöÀç ¿ù Ãâ·Â

        scheWindow.display();

    }


    return 0;
}