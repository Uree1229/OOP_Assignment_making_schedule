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




vector <string> tokenize(string context, char target);

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
    string const get_output() const { return this->output; }

    const string& print() {
        char buf[256];
        sprintf(buf, "Time : %s\nTodo : %s\n", this->time.c_str(), this->todo.c_str());
        this->output = buf;
        return this->output;
    }
private:
    sf::Font font;
    int month;
    int day;
    string todo;
    string time;
    string output;
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
                    token = tokenize(str, '$');
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
bool isTodoPressed(sf::Vector2f w, vector<sf::RectangleShape*>& v, int& num)
{
    int i = 0;
    for (i = 0; i < v.size(); i++)
    {
        if (v[i]->getGlobalBounds().contains(w))
        {
            num = i;
            return true;
        }
    }
    return false;
}


//텍스트 파일에서 일정 삭제
void removeTodo(const string& filename, const string& substring) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    vector<string> lines;

    string line;
    while (getline(inputFile, line)) {
        // 해당 문자열을 포함하지 않는 라인을 벡터에 추가
        if (line.find(substring) == string::npos) {
            lines.push_back(line);
        }
    }

    inputFile.close();

    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    // 벡터에 있는 라인들을 파일에 쓰기
    for (const auto& l : lines) {
        outputFile << l << endl;
    }

    outputFile.close();
}

//텍스트 파일에 일정 추가하는 함수
void writeTodo(const string& filename, const string& str) {
    ofstream outFile;

    outFile.open(filename, ios::app);

    //파일 안열리면 팝업 창으로 알림 메세지 출력
    if (!outFile.is_open())
    {
        sf::RenderWindow window(sf::VideoMode(600, 400), "Error Message");
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        {
            cerr << "Failed to load font" << endl;
        }
        sf::RectangleShape Button(sf::Vector2f(150, 100));
        Button.setPosition(220, 155);
        Button.setFillColor(sf::Color(240, 240, 240));
        Button.setOutlineColor(sf::Color::Black);
        Button.setOutlineThickness(2.0f);

        sf::Text textButton("Ok", font, 40);
        textButton.setFillColor(sf::Color::Black);
        textButton.setPosition(270, 175);

        sf::Text text("Can't open the file", font, 30);
        text.setFillColor(sf::Color::Black);
        text.setPosition(175, 50);

        while (window.isOpen())     //오류 알림 팝업
        {
            sf::Event event;
            sf::Vector2i mousPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousPos);


            while (window.pollEvent(event))     //OK 버튼 클릭하거나 엔터 누르면 윈도우 닫음
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (Button.getGlobalBounds().contains(worldPos))
                    {
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            Button.setFillColor(sf::Color(220, 220, 220));
                            window.close();
                        }
                    }
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        Button.setFillColor(sf::Color(220, 220, 220));
                        window.close();
                    }
                }
            }

            window.clear(sf::Color::White);
            window.draw(Button);
            window.draw(textButton);
            window.draw(text);

            window.display();
        }
    }
    outFile << str << endl;

    outFile.close();

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

    vector<sf::RectangleShape*> v_todo;
    vector<sf::Text*> t_todo;

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

    //일정 추가 버튼
    sf::Text addText("Add", font, 24);
    addText.setPosition(510, 110);
    addText.setFillColor(sf::Color::Black);

    //일정 삭제 버튼
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
    timeText.setPosition(100, 100);

    //시간 입력 박스
    sf::RectangleShape timeBox(sf::Vector2f(500, 50));
    timeBox.setPosition(220, 100);
    timeBox.setFillColor(sf::Color::White);
    timeBox.setOutlineColor(sf::Color::Black);
    timeBox.setOutlineThickness(2.0f);

    //시간 텍스트 
    sf::Text timeString("", font, 40);
    timeString.setFillColor(sf::Color::Black);
    timeString.setPosition(230, 95);

    //할일 메시지 부분
    sf::Text todoText("Todo : ", font, 40);
    todoText.setFillColor(sf::Color::Black);
    todoText.setPosition(100, 250);

    //할일 입력 박스
    sf::RectangleShape todoBox(sf::Vector2f(500, 50));
    todoBox.setPosition(220, 250);
    todoBox.setFillColor(sf::Color::White);
    todoBox.setOutlineColor(sf::Color::Black);
    todoBox.setOutlineThickness(2.0f);

    //할일 텍스트
    sf::Text todoString("", font, 40);
    todoString.setFillColor(sf::Color::Black);
    todoString.setPosition(230, 245);

    //확인 버튼
    sf::RectangleShape confirmBox(sf::Vector2f(200, 100));
    confirmBox.setPosition(400, 350);
    confirmBox.setFillColor(sf::Color(240, 240, 240));
    confirmBox.setOutlineColor(sf::Color::Black);
    confirmBox.setOutlineThickness(2.0f);

    //확인 텍스트
    sf::Text confirmButton("Confirm", font, 40);
    confirmButton.setFillColor(sf::Color::Black);
    confirmButton.setPosition(430, 375);

    int num = -1;
    bool colorBack = false;


    sf::Text* focusedText = NULL;
    string timeInput = "";
    string todoInput = "";


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

        //월 별 달력창
        while (scheWindow.pollEvent(event)) //month 윈도우 이벤트
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
                    v_todo.clear();
                    t_todo.clear();
                    today = year.getMonth()[month - 1].dayList[buttonNum];

                }
            }

        }

        sf::Event event1;


        if (buttonWindow.isOpen())
        {

            while (buttonWindow.pollEvent(event1))
            {
                //일정 창 닫으면 생성된 객체 제거
                if (event1.type == sf::Event::Closed)
                {
                    buttonWindow.clear();
                    buttonWindow.close();
                }
                else if (event1.type == sf::Event::MouseButtonPressed)
                {
                    if (addButton.getGlobalBounds().contains(worldPos1))
                    {

                        addButton.setFillColor(sf::Color(220, 220, 220));
                        addTodoWindow.create(sf::VideoMode(1000, 600), "Add Todo");
                        colorBack = true;

                    }
                    //일정 제거 버튼 액션
                    else if (removeButton.getGlobalBounds().contains(worldPos1))
                    {
                        removeButton.setFillColor(sf::Color(220, 220, 220));
                        if (num >= 0 && num < today.TodoList.size())        //일정과 관련된 벡터 전부 초기화하고, year 객체도 저장된 텍스트를 기준으로 다시 초기화해서 사용
                        {
                            char buf[256];
                            Todo td = today.TodoList[num];
                            sprintf(buf, "%d$%d$%s$%s", td.get_month(), td.get_day(), td.get_time().c_str(), td.get_todo().c_str());
                            string file_name = EXTERN_STUDENT_ID + ".txt";
                            removeTodo(file_name, buf);
                            today.TodoList.erase(today.TodoList.begin() + num);
                            v_todo.clear();
                            t_todo.clear();
                            if (today.TodoList.size() > v_todo.size())      //벡터에 다시 일정과 관련된 요소 push back
                            {
                                for (int i = 0; i < today.TodoList.size(); i++)
                                {
                                    sf::RectangleShape* rect_todo = new sf::RectangleShape(sf::Vector2f(400, 60));
                                    rect_todo->setFillColor(sf::Color::White);
                                    rect_todo->setOutlineColor(sf::Color::Black);
                                    rect_todo->setOutlineThickness(2.0f);
                                    rect_todo->setPosition(10, 10 + 60 * i);
                                    v_todo.push_back(rect_todo);
                                    sf::Text* txt = new sf::Text(today.TodoList[i].get_output(), font, 24);
                                    txt->setFillColor(sf::Color::Black);
                                    txt->setPosition(10, 10 + 60 * i);
                                    t_todo.push_back(txt);
                                }
                            }
                            //다시 그리기
                            buttonWindow.clear(sf::Color::White);
                            for (int i = 0; i < today.TodoList.size(); i++)
                                buttonWindow.draw(*v_todo[i]);
                            for (int i = 0; i < today.TodoList.size(); i++)
                                buttonWindow.draw(*t_todo[i]);
                            year = Year();
                            year.setting();
                        }
                        colorBack = true;
                    }
                    //일정이 담긴 벡터 전부 돌려서 어느 것이 클릭됐는 지 알아보고 색칠
                    else if (isTodoPressed(worldPos1, v_todo, num))
                    {
                        vector<sf::RectangleShape*>::iterator p;
                        for (p = v_todo.begin(); p != v_todo.end(); p++)
                            (*p)->setFillColor(sf::Color::White);
                        v_todo[num]->setFillColor(sf::Color(220, 220, 220));

                    }
                    else
                    {
                        vector<sf::RectangleShape*>::iterator p;
                        for (p = v_todo.begin(); p != v_todo.end(); p++)
                            (*p)->setFillColor(sf::Color::White);
                    }
                }
                if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (colorBack)
                    {
                        addButton.setFillColor(sf::Color(240, 240, 240));
                        removeButton.setFillColor(sf::Color(240, 240, 240));
                        colorBack = false;
                    }
                }
            }

            sf::Event event2;
            //일정 추가 창 요소 그리기
            if (addTodoWindow.isOpen())
            {
                while (addTodoWindow.pollEvent(event2))
                {
                    if (event2.type == sf::Event::Closed)           //창 닫히면 객체 및 변수 다시 초기화
                    {
                        timeBox.setOutlineColor(sf::Color::Black);
                        todoBox.setOutlineColor(sf::Color::Black);
                        addTodoWindow.clear();
                        addTodoWindow.close();
                        focusedText = NULL;
                        timeInput = "";
                        todoInput = "";
                        timeString.setString(timeInput);
                        todoString.setString(todoInput);
                    }
                    else if (event2.type == sf::Event::TextEntered)      //텍스트 입력 구현
                    {
                        {
                            if (event2.text.unicode < 128)
                            {
                                if (event2.text.unicode == 8)
                                {
                                    if (focusedText == &timeString)
                                    {
                                        if (!timeInput.empty())
                                            timeInput.pop_back();
                                    }
                                    else if (focusedText == &todoString)
                                        if (!todoInput.empty())
                                            todoInput.pop_back();
                                }
                                else if (event2.text.unicode != 13 && event2.text.unicode != 9 && event2.text.unicode != 36)
                                    if (focusedText == &timeString)
                                        timeInput += static_cast<char>(event2.text.unicode);
                                    else if (focusedText == &todoString)
                                        todoInput += static_cast<char>(event2.text.unicode);
                            }
                            if (focusedText == &timeString)
                                timeString.setString(timeInput);
                            else if (focusedText == &todoString)
                                todoString.setString(todoInput);
                        }

                    }
                    else if (event2.type == sf::Event::MouseButtonPressed)   //어떤 박스 클릭인지 색깔로 구분
                    {
                        if (timeBox.getGlobalBounds().contains(worldPos2))
                        {
                            timeBox.setOutlineColor(sf::Color::Red);
                            todoBox.setOutlineColor(sf::Color::Black);
                            focusedText = &timeString;

                        }
                        else if (todoBox.getGlobalBounds().contains(worldPos2))
                        {
                            timeBox.setOutlineColor(sf::Color::Black);
                            todoBox.setOutlineColor(sf::Color::Red);
                            focusedText = &todoString;
                        }
                        else if (confirmBox.getGlobalBounds().contains(worldPos2))
                        {
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !timeInput._Equal("") && !todoInput._Equal(""))  // 비어 있으면 입력 안됨
                            {
                                char buf[256];
                                string file_name = EXTERN_STUDENT_ID + ".txt";
                                confirmBox.setFillColor(sf::Color(220, 220, 220));
                                sprintf(buf, "%d$%d$%s$%s", month, buttonNum + 1, timeInput.c_str(), todoInput.c_str());
                                writeTodo(file_name, buf);
                                v_todo.clear();
                                t_todo.clear();
                                year = Year();
                                year.setting();
                                today = year.getMonth()[month - 1].dayList[buttonNum];
                                if (today.TodoList.size() > v_todo.size())
                                {
                                    for (int i = 0; i < today.TodoList.size(); i++)
                                    {
                                        sf::RectangleShape* rect_todo = new sf::RectangleShape(sf::Vector2f(400, 60));
                                        rect_todo->setFillColor(sf::Color::White);
                                        rect_todo->setOutlineColor(sf::Color::Black);
                                        rect_todo->setOutlineThickness(2.0f);
                                        rect_todo->setPosition(10, 10 + 60 * i);
                                        v_todo.push_back(rect_todo);
                                        sf::Text* txt = new sf::Text(today.TodoList[i].get_output(), font, 24);
                                        txt->setFillColor(sf::Color::Black);
                                        txt->setPosition(10, 10 + 60 * i);
                                        t_todo.push_back(txt);
                                    }
                                }
                                buttonWindow.clear(sf::Color::White);
                                for (int i = 0; i < today.TodoList.size(); i++)
                                    buttonWindow.draw(*v_todo[i]);
                                for (int i = 0; i < today.TodoList.size(); i++)
                                    buttonWindow.draw(*t_todo[i]);

                                timeBox.setOutlineColor(sf::Color::Black);
                                todoBox.setOutlineColor(sf::Color::Black);
                                addTodoWindow.close();
                                focusedText = NULL;
                                timeInput = "";
                                todoInput = "";
                                timeString.setString(timeInput);
                                todoString.setString(todoInput);
                            }

                        }
                        else
                        {
                            timeBox.setOutlineColor(sf::Color::Black);
                            todoBox.setOutlineColor(sf::Color::Black);
                            focusedText = NULL;
                        }
                    }
                    else if (event2.type == sf::Event::KeyPressed)
                    {
                        if (event2.key.code == sf::Keyboard::Tab)
                        {
                            if (focusedText == &timeString)
                            {
                                timeBox.setOutlineColor(sf::Color::Black);
                                todoBox.setOutlineColor(sf::Color::Red);
                                focusedText = &todoString;
                            }
                            else
                            {
                                timeBox.setOutlineColor(sf::Color::Red);
                                todoBox.setOutlineColor(sf::Color::Black);
                                focusedText = &timeString;
                            }
                        }
                        //엔터 눌러서 저장, 입력 비어있으면 저장안됨
                        else if (event2.key.code == sf::Keyboard::Enter && !timeInput._Equal("") && !todoInput._Equal(""))
                        {
                            confirmBox.setFillColor(sf::Color(240, 240, 240));
                            char buf[256];
                            string file_name = EXTERN_STUDENT_ID + ".txt";
                            confirmBox.setFillColor(sf::Color(220, 220, 220));
                            sprintf(buf, "%d$%d$%s$%s", month, buttonNum + 1, timeInput.c_str(), todoInput.c_str());
                            writeTodo(file_name, buf);
                            v_todo.clear();
                            t_todo.clear();
                            year = Year();
                            year.setting();
                            today = year.getMonth()[month - 1].dayList[buttonNum];
                            if (today.TodoList.size() > v_todo.size())
                            {
                                for (int i = 0; i < today.TodoList.size(); i++)
                                {
                                    sf::RectangleShape* rect_todo = new sf::RectangleShape(sf::Vector2f(400, 60));
                                    rect_todo->setFillColor(sf::Color::White);
                                    rect_todo->setOutlineColor(sf::Color::Black);
                                    rect_todo->setOutlineThickness(2.0f);
                                    rect_todo->setPosition(10, 10 + 60 * i);
                                    v_todo.push_back(rect_todo);
                                    sf::Text* txt = new sf::Text(today.TodoList[i].get_output(), font, 24);
                                    txt->setFillColor(sf::Color::Black);
                                    txt->setPosition(10, 10 + 60 * i);
                                    t_todo.push_back(txt);
                                }
                            }
                            buttonWindow.clear(sf::Color::White);
                            for (int i = 0; i < today.TodoList.size(); i++)
                                buttonWindow.draw(*v_todo[i]);
                            for (int i = 0; i < today.TodoList.size(); i++)
                                buttonWindow.draw(*t_todo[i]);

                            timeBox.setOutlineColor(sf::Color::Black);
                            todoBox.setOutlineColor(sf::Color::Black);
                            addTodoWindow.close();
                            focusedText = NULL;
                            timeInput = "";
                            todoInput = "";
                            timeString.setString(timeInput);
                            todoString.setString(todoInput);
                        }

                    }
                }
                addTodoWindow.clear(sf::Color::White);
                addTodoWindow.draw(timeText);
                addTodoWindow.draw(timeBox);
                addTodoWindow.draw(timeString);
                addTodoWindow.draw(todoText);
                addTodoWindow.draw(todoBox);
                addTodoWindow.draw(todoString);
                addTodoWindow.draw(confirmBox);
                addTodoWindow.draw(confirmButton);

                addTodoWindow.display();
            }



            //데이 창에서 일정 출력


            if (today.TodoList.size() > v_todo.size())
            {
                for (int i = 0; i < today.TodoList.size(); i++)
                {
                    sf::RectangleShape* rect_todo = new sf::RectangleShape(sf::Vector2f(400, 60));
                    rect_todo->setFillColor(sf::Color::White);
                    rect_todo->setOutlineColor(sf::Color::Black);
                    rect_todo->setOutlineThickness(2.0f);
                    rect_todo->setPosition(10, 10 + 60 * i);
                    v_todo.push_back(rect_todo);
                    sf::Text* txt = new sf::Text(today.TodoList[i].get_output(), font, 24);
                    txt->setFillColor(sf::Color::Black);
                    txt->setPosition(10, 10 + 60 * i);
                    t_todo.push_back(txt);
                }
            }


            buttonWindow.clear(sf::Color::White);
            for (int i = 0; i < today.TodoList.size(); i++)
                buttonWindow.draw(*v_todo[i]);
            for (int i = 0; i < today.TodoList.size(); i++)
                buttonWindow.draw(*t_todo[i]);


            buttonWindow.draw(addButton);
            buttonWindow.draw(removeButton);
            buttonWindow.draw(addText);
            buttonWindow.draw(removeText);

            buttonWindow.display();
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
        scheWindow.draw(monthText[month - 1]); //static month 이용해서 현재 월 출력
        scheWindow.display();


    }


    return 0;
}
