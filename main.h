#pragma once
using namespace std;
class User_DB;

extern std::string EXTERN_STUDENT_ID;
static std::string STUDENT_ID;
static std::string STUDENT_PWD;

void register_menu();
void login_menu();
int schedule_main(void);

