//
// Created by 18382 on 2021/1/21.
//

#include "Cammand(user).h"

user::user(const string &user_id_, const string &pass, const string &name_, int pri) {
    strcpy(user_id,user_id_.c_str());
    strcpy(password,pass.c_str());
    strcpy(name,name_.c_str());
    priority = pri;
}

stack<user>USERONLINE;

void login(const string &user_id_,string password ){
    if(strlen(password.c_str()) == 0){
        vector<int>possibleoffset;
        USER_ID_LIST.findnode(user_id_,possibleoffset);
        if(possibleoffset.empty())throw("cannot find the userid");
        user tmp(my_read<user>(USER_ID_LIST_FILE,possibleoffset[0]));
        if (USERONLINE.top().priority > tmp.priority){
            USERONLINE.push(tmp);
        }else{
            throw("error");
        }
    }else{
        vector<int>psf;
        USER_ID_LIST.findnode(user_id_,psf);
        if(psf.empty())throw("cannot find the userid");
        user tmp(my_read<user>(USER_ID_LIST_FILE,psf[0]));
        if(strcmp(tmp.password,password.c_str()) == 0 && strcmp(tmp.user_id,user_id_.c_str()) == 0){
            USERONLINE.push(tmp);
        }else{
            throw("error");
        }
    }
}

void logout(){
    if(USERONLINE.empty())throw("emptystack when log out");
    else USERONLINE.pop();
}

void register_(user & x){
    int offset = my_write<user>(USER_ID_LIST_FILE,x);//缺省
    node a(offset,x.user_id);
    USER_ID_LIST.addnode(a);
}

void addaccount(user &x){
    int offset = my_write<user>(USER_ID_LIST_FILE,x);//缺省
    node a(offset,x.user_id);
    USER_ID_LIST.addnode(a);
}

void deleteaccount(const char* user_id){
    if(user_id == "root")throw("error");
    vector<int>possibleoffset;
    USER_ID_LIST.findnode(user_id,possibleoffset);
    if(possibleoffset.empty())throw("cannot find the userid");
    //user tmp(my_read<user>(USER_ID_LIST_FILE,possibleoffset[0]));
    node newnode(possibleoffset[0],user_id);
    USER_ID_LIST.deletenode(newnode);
}

void changepassword(const char* user_id,const char * newpas,const char* oldpas ){
    vector<int>possibleoffset;
    USER_ID_LIST.findnode(user_id,possibleoffset);
    if(possibleoffset.empty())throw("cannot find the userid");
    user tmp(my_read<user>(USER_ID_LIST_FILE,possibleoffset[0]));
    if(strlen(oldpas) != 0){
        if(strcmp(oldpas,tmp.password) != 0){
            throw("wrong oldpassword");
        }
    }
    strcpy(tmp.password,newpas);
    my_write<user>(USER_ID_LIST_FILE,tmp,possibleoffset[0]);
}