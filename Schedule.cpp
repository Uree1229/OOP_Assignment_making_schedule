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
    Todo()
    {
        if (!this->font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        {
            throw runtime_error("Failed to load font from C:/Windows/Fonts/arial.ttf ");
        }

    }

    int const get_month() { return this->month; }
    int const get_day() { return this->day; }
    string const get_todo() const { return this->todo; }
    string const get_time() const { return this->time; }
    string const get_output() const { return this->output; }
    sf::Text& get_text() { return this->text; }

    const string& print() {
        char buf[256];
        sprintf(buf, "Time : %s\nTodo : %s\n", this->time.c_str(), this->todo.c_str());
        this->output = buf;
        this->text.setString(output);
        this->text.setFont(this->font);
        this->text.setCharacterSize(24);
        this->text.setFillColor(sf::Color::Black);
        return this->output;
    }
private:
    sf::Font font;
    int month;
    int day;
    string todo;
    string time;
    string output;
    sf::Text text;
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
};

//달 에 있는 일의 객체들을 전부 만들어서 todo를 nothing으로 초기화
class Month {
public:
    Month(int month) {
        for (int i = 0; i < months[month]; i++) {
            Day day = Day();
            Todo todo;
            //todo.set_data(month + 1, i + 1, "Nothing", ""); // 각 날짜에 대한 기본 Todo 객체 생성
            //day.TodoList.push_back(todo);  // 기본 Todo 객체를 TodoList에 추가
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
        string file_name = EXTERN_STUDENT_ID + ".txt";      //로그인 정보로 파일 읽어옴
        ifs.open(file_name);

        if (ifs.is_open()) {        //파일에서 할일 리스트 하나씩 읽어서 출력 및 저장
            string str;
            vector <string> token;
            while (getline(ifs, str)) {
                if (!str.empty()) {  // 비어있지 않은 경우만 처리
                    token = tokenize(str, ' ');
                    Todo todo;
                    todo.set_data(stoi(token[0]), stoi(token[1]), token[3], token[2]);
                    todo.print();
                    monthList[stoi(token[0]) - 1].dayList[stoi(token[1]) - 1].TodoList.push_back(todo);
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
    //void printTodoList(int month, int day) {
        //오류 month가 1이든 2이든 12든지 전부 dayList의 크기가 31임
        //if (month >= 1 && month <= 12 && day >= 1 && day <= months[month - 1]) {
        //    for (vector<Day>::iterator p = monthList[month].dayList.begin(); p != monthList[month].dayList.end(); p++) {
        //        monthList[month - 1].dayList[day - 1].print();
        //    }
        //}
    //}
    /*void printAll() {
        for (int i = 0; i < 12; i++) {
            for (vector<Day>::iterator p = monthList[i].dayList.begin(); p != monthList[i].dayList.end(); p++) {
                p->print();
            }

        }
    }*/
    vector<Month> getMonth() {
        return this->monthList;
    }

private:
    vector<Month> monthList;
};




sf::RectangleShape dayButtons[31];
sf::Text dayText[31];
sf::Text monthText[12];
int buttonNum = -1;

//day 버튼 눌릴 때
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

//버튼 창에서 할 일 누를 때
bool isTodoPressed(sf::Vector2f w, vector<sf::RectangleShape>& v, int& num)
{
    int i = 0;
    for (i = 0; i < v.size(); i++)
    {
        if (v[i].getGlobalBounds().contains(w))
        {
            num = i;
            return true;
        }
    }
    return false;
}

void removeTodo(const std::string& filename, const std::string& substring) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<std::string> lines;

    std::string line;
    while (std::getline(inputFile, line)) {
        // 해당 문자열을 포함하지 않는 라인을 벡터에 추가
        if (line.find(substring) == std::string::npos) {
            lines.push_back(line);
        }
    }

    inputFile.close();

    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    // 벡터에 있는 라인들을 파일에 쓰기
    for (const auto& l : lines) {
        outputFile << l << std::endl;
    }

    outputFile.close();
}


int schedule_main(void) {

    int control = 0;
    int week = 0;
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        return -1;

    //dayButton 31개, dayText 31개
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
        textObj.setPosition(textx[i % 7], texty[week]); //text x,y좌표
        dayText[i] = textObj;
    }

    //한자릿수 dayText 위치 조정
    for (int i = 0; i < 9; i++)
    {
        week = i / 7;
        string strday = to_string(i + 1);
        sf::Text textObj(strday, font, 40);
        textObj.setFillColor(sf::Color::Black);
        textObj.setPosition(textx[i % 7] + 8, texty[week]); //text x,y좌표
        dayText[i] = textObj;
    }

    //monthText 9개(한자릿수)
    for (int i = 0; i < 9; i++)
    {
        string mText = to_string(i + 1);
        sf::Text textObj(mText, font, 120);
        textObj.setFillColor(sf::Color::Black);
        textObj.setPosition(600, 10); //text x,y좌표
        monthText[i] = textObj;
    }
    //monthText 3개(두자릿수)
    for (int i = 9; i < 12; i++)
    {
        string mText = to_string(i + 1);
        sf::Text textObj(mText, font, 120);
        textObj.setFillColor(sf::Color::Black);
        textObj.setPosition(570, 10); //text x,y좌표
        monthText[i] = textObj;
    }

    //왼쪽 화살표
    sf::RectangleShape leftArrow(sf::Vector2f(200, 100));
    leftArrow.setPosition(100, 40); //xpos, ypos
    leftArrow.setFillColor(sf::Color::Green);
    leftArrow.setOutlineColor(sf::Color::Black);
    leftArrow.setOutlineThickness(3.0f);

    sf::Text lArrow("<<", font, 70);
    lArrow.setFillColor(sf::Color::Black);
    lArrow.setPosition(150, 50);

    //오른쪽 화살표
    sf::RectangleShape rightArrow(sf::Vector2f(200, 100));
    rightArrow.setPosition(1000, 40); //xpos, ypos
    rightArrow.setFillColor(sf::Color::Green);
    rightArrow.setOutlineColor(sf::Color::Black);
    rightArrow.setOutlineThickness(3.0f);

    sf::Text rArrow(">>", font, 70);
    rArrow.setFillColor(sf::Color::Black);
    rArrow.setPosition(1060, 50);

    vector<sf::RectangleShape> v_todo;
    vector<sf::Text> t_todo;

    //add button
    sf::RectangleShape addButton(sf::Vector2f(100, 50));
    addButton.setPosition(480, 100);
    addButton.setFillColor(sf::Color(240, 240, 240));
    addButton.setOutlineColor(sf::Color::Black);
    addButton.setOutlineThickness(2.0f);
    //remove button
    sf::RectangleShape removeButton(sf::Vector2f(100, 50));
    removeButton.setPosition(480, 200);
    removeButton.setFillColor(sf::Color(240, 240, 240));
    removeButton.setOutlineColor(sf::Color::Black);
    removeButton.setOutlineThickness(2.0f);

    sf::Text addText("Add", font, 24);
    addText.setPosition(510, 110);
    addText.setFillColor(sf::Color::Black);

    sf::Text removeText("Remove", font, 24);
    removeText.setPosition(485, 210);
    removeText.setFillColor(sf::Color::Black);

    sf::RenderWindow scheWindow(sf::VideoMode(1280, 720), "My Calendar");
    Year year;
    year.setting();
    bool sub = false;

    sf::RenderWindow buttonWindow;

    char buf[256];

    Day today;


    sf::RenderWindow addTodoWindow;

    //시간 입력
    sf::Text timeText("Time : ", font, 40);
    timeText.setFillColor(sf::Color::Black);
    timeText.setPosition(150, 100);

    //시간 입력 박스
    sf::RectangleShape timeBox(sf::Vector2f(300, 50));
    timeBox.setPosition(250, 100);
    timeBox.setFillColor(sf::Color::White);
    timeBox.setOutlineColor(sf::Color::Black);
    timeBox.setOutlineThickness(2.0f);

    //시간 텍스트 
    sf::Text timeString("", font, 40);
    timeString.setFillColor(sf::Color::Black);
    timeString.setPosition(260, 95);

    //할일 메시지 부분
    sf::Text todo_Text("Todo : ", font, 40);
    todo_Text.setFillColor(sf::Color::Black);
    todo_Text.setPosition(150, 200);

    //할일 입력 박스
    sf::RectangleShape todo_Box(sf::Vector2f(400, 50));
    todo_Box.setPosition(250, 250);
    todo_Box.setFillColor(sf::Color::White);
    todo_Box.setOutlineColor(sf::Color::Black);
    todo_Box.setOutlineThickness(2.0f);

    //할일 텍스트
    sf::Text todo_String("", font, 40);
    todo_String.setFillColor(sf::Color::Black);
    todo_String.setPosition(260, 245);

    //확인 버튼
    sf::RectangleShape confirmBox(sf::Vector2f(200, 100));
    confirmBox.setPosition(310, 350);
    confirmBox.setFillColor(sf::Color(240, 240, 240));
    confirmBox.setOutlineColor(sf::Color::Black);
    confirmBox.setOutlineThickness(2.0f);

    sf::Text confirmButton("Confirm", font, 40);
    confirmButton.setFillColor(sf::Color::Black);
    confirmButton.setPosition(360, 375);

    int num = -1;

    while (scheWindow.isOpen())
    {


        sf::Event event;
        sf::Vector2i mousPos = sf::Mouse::getPosition(scheWindow);
        sf::Vector2f worldPos = scheWindow.mapPixelToCoords(mousPos);

        sf::Vector2i mousPosReg = sf::Mouse::getPosition(scheWindow);
        sf::Vector2f worldPosReg = scheWindow.mapPixelToCoords(mousPosReg);


        sf::Vector2i mousPos1 = sf::Mouse::getPosition(buttonWindow);
        sf::Vector2f worldPos1 = scheWindow.mapPixelToCoords(mousPos1);

        sf::Vector2i mousPosReg1 = sf::Mouse::getPosition(buttonWindow);
        sf::Vector2f worldPosReg1 = scheWindow.mapPixelToCoords(mousPosReg1);

        sf::Vector2i mousPos2 = sf::Mouse::getPosition(addTodoWindow);
        sf::Vector2f worldPos2 = scheWindow.mapPixelToCoords(mousPos2);

        sf::Vector2i mousPosReg2 = sf::Mouse::getPosition(addTodoWindow);
        sf::Vector2f worldPosReg2 = scheWindow.mapPixelToCoords(mousPosReg2);

        int a = 0;

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
                else if (!buttonWindow.isOpen() && isButtonPressed(worldPos))
                {

                    sprintf(buf, "Month : %d, Day : %d", month, buttonNum + 1);
                    buttonWindow.create(sf::VideoMode(600, 400), buf);

                    today = year.getMonth()[month - 1].dayList[buttonNum];

                }
            }

        }

        sf::Event event1;


        if (buttonWindow.isOpen())
        {
            while (buttonWindow.pollEvent(event1))
            {
                if (event1.type == sf::Event::Closed)
                {
                    buttonWindow.close();
                }
                else if (event1.type == sf::Event::MouseButtonPressed)
                {
                    if (addText.getGlobalBounds().contains(worldPos1))
                    {

                        addButton.setFillColor(sf::Color(220, 220, 220));
                        addTodoWindow.create(sf::VideoMode(1000, 600), "Add Todo");
                       
                    }
                    else if (removeText.getGlobalBounds().contains(worldPos1))
                    {
                        removeButton.setFillColor(sf::Color(220, 220, 220));
                        for (int i = 0; i < v_todo.size(); i++)
                        {
                            if (i == num)
                            {
                                char buf[256];
                                Todo td = today.TodoList[num];
                                sprintf(buf, "%d %d %s %s", td.get_month(), td.get_day(), td.get_time(), td.get_todo());
                                string file_name = EXTERN_STUDENT_ID + ".txt";
                                removeTodo(file_name, buf);
                                year.setting();
                                break;
                            }
                        }
                    }
                    else if (isTodoPressed(worldPos1, v_todo, num))
                    {
                        if(v_todo.size() > num) // 추가된 부분
                        v_todo[num].setFillColor(sf::Color(220, 220, 220));

                    }
                    else
                    {
                        if (v_todo.size() > num) //추가된 부분
                        v_todo[num].setFillColor(sf::Color::White);
                    }
                }
            }

            sf::Event event2;

            if (addTodoWindow.isOpen())
            {
                while (addTodoWindow.pollEvent(event2))
                {
                    if (event2.type == sf::Event::Closed)
                        addTodoWindow.close();
                }
                addTodoWindow.clear(sf::Color::White);
                addTodoWindow.draw(timeText);
                addTodoWindow.draw(timeBox);
                addTodoWindow.draw(timeString);
                addTodoWindow.draw(todo_Text);
                addTodoWindow.draw(todo_Box);
                addTodoWindow.draw(todo_String);
                addTodoWindow.draw(confirmBox);
                addTodoWindow.draw(confirmButton);

            }



            //데이 창에서 일정 출력
            buttonWindow.clear(sf::Color::White);
            vector<Todo>::iterator p;
            vector<sf::RectangleShape>::iterator pv;
            vector<sf::Text>::iterator pt;
            int i = 0;
            if (today.TodoList.size() != v_todo.size())
            {
                for (p = today.TodoList.begin(); p != today.TodoList.end(); p++)
                {
                    sf::RectangleShape rect_todo(sf::Vector2f(400, 60));
                    rect_todo.setFillColor(sf::Color::White);
                    rect_todo.setOutlineColor(sf::Color::Black);
                    rect_todo.setOutlineThickness(2.0f);
                    rect_todo.setPosition(10, 10 + 60 * i);
                    v_todo.push_back(rect_todo);
                    sf::Text txt(p->get_output(), font, 24);
                    txt.setFillColor(sf::Color::Black);
                    txt.setPosition(10, 10 + 60 * i++);
                    t_todo.push_back(txt);;
                }
            }
            else
            {
                for (pv = v_todo.begin(); pv != v_todo.end(); pv++)
                    buttonWindow.draw(*pv);
                for (pt = t_todo.begin(); pt != t_todo.end(); pt++)
                    buttonWindow.draw(*pt);
            }

            buttonWindow.draw(addButton);
            buttonWindow.draw(removeButton);
            buttonWindow.draw(addText);
            buttonWindow.draw(removeText);

        }



        addTodoWindow.display();

        buttonWindow.display();

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
        scheWindow.draw(monthText[month - 1]); //static month 이용해서 현재 월 출력
        scheWindow.display();


    }


    return 0;
}
