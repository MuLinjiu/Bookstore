//
// Created by 18382 on 2021/1/21.
//

#ifndef NODELIST_CPP_CAMMAND_USER_H
#define NODELIST_CPP_CAMMAND_USER_H
#include <iostream>
#include <cstring>
#include <vector>
#include<stack>
#include<nodelist.h>


using namespace std;
class user;
enum jobtype{BOSS,CUSTOMER,STAFF};
class user{
public:
    int priority = -1;
    char user_id[31];
    char password[31];
    char name[31];
    bool if_login = false;
    user() {};
    virtual jobtype gettype() = 0;
};

class customer:public user{
    customer(char id[],char pass[],char name1[]):user(){
        priority = 1;
        strcpy(user_id,id);
        strcpy(password,pass);
        strcpy(name,name1);
    }
    jobtype gettype()override{
        return(CUSTOMER);
    }
};

class staff:public user{
    staff(char id[],char pass[],char name1[]):user(){
        priority = 3;
        strcpy(user_id,id);
        strcpy(password,pass);
        strcpy(name,name1);
    }
    jobtype gettype()override{
        return(STAFF);
    }
};

class root:public user{
    root():user(){
        priority = 7;
        strcpy(user_id,"root");
        strcpy(password,"sjtu");
    }
    jobtype gettype()override{
        return(BOSS);
    }
};
void login(string &user_id_,string password = "");
#endif //NODELIST_CPP_CAMMAND_USER_H
