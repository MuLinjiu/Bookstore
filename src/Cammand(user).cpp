//
// Created by 18382 on 2021/1/21.
//

#include "Cammand(user).h"
#include "Bookstore.h"
#include <iomanip>
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

void selectbook(const string & ISBN_){
    if(USERONLINE.empty())throw("select failed");
    else {
        if (USERONLINE.top().priority >= 3) {
            vector<int> possibleoffset;
            ISBN_LIST.findnode(ISBN_, possibleoffset);
            if (possibleoffset.empty()) {
                Book tmp(0, 0, ISBN_, "", "", "");
                fstream fin;
                fin.open(BOOK_FILE,ios::in | ios::out | ios::binary);
                if (!fin)throw ("error");
                fin.seekp(0, ios::end);
                int offset = fin.tellg();
                fin.write(reinterpret_cast<char *>(&tmp), sizeof(Book));
                USERONLINE.top().select = offset;
                node tmp1(offset, ISBN_);
                ISBN_LIST.addnode(tmp1);
            } else {
                USERONLINE.top().select = possibleoffset[0];
            }
        }
        else throw("inadequate pri");
    }
}

void import(int quantity_, int totleprice ){
    if(USERONLINE.top().priority >= 3){
        int offset = USERONLINE.top().select;
        if(offset == -1)throw("error");
        fstream fin,fout;
        fin.open(BOOK_FILE,ios::in | ios::out | ios::binary);
        if(!fin)throw("import");
        fin.seekg(offset);
        Book tmp;
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(Book));
        tmp.quantity += quantity_;
        fin.seekp(offset);
        fin.write(reinterpret_cast<char*>(&tmp),sizeof(Book));//书的数量更新；
        fin.close();
        transaction tmp1(tmp.ISBN,USERONLINE.top().user_id,quantity_,-totleprice);
        fin.open(TRANSACTION_FILE,ios::in | ios::out | ios::binary);
        if(!fin)throw("import2");
        fin.seekp(0,ios::end);
        fin.write(reinterpret_cast<char *>(&tmp1),sizeof(transaction));//更新一笔交易
        fin.close();
        fout.open(TOTLEMONEY_FILE,ios::in | ios::out | ios::binary);
        if(!fout)throw("import3");
        fout.seekg(0);
        totlemoney tmp2;
        fout.read(reinterpret_cast<char*>(&tmp2),sizeof(totlemoney));
        tmp2.expense -= totleprice;
        fout.seekp(0);
        fout.write(reinterpret_cast<char*>(&tmp2),sizeof(totlemoney));//更新总支出
        fout.close();
    }
    else throw("error");
}


void buy(const string &ISBN_,int quantity_){
    vector<int>possibleoffset;
    ISBN_LIST.findnode(ISBN_,possibleoffset);
    if(possibleoffset.empty())throw("buy1");
    int offset = possibleoffset[0];
    Book tmpbook;
    fstream fin,fout;
    fin.open(BOOK_FILE,ios::in | ios::out | ios::binary);
    if(!fin)throw("buy2");
    fin.seekg(offset);
    fin.read(reinterpret_cast<char *>(&tmpbook),sizeof(Book));
    if(tmpbook.quantity < quantity_)throw("buy3");
    tmpbook.quantity -= quantity_;
    fin.seekp(offset);
    fin.write(reinterpret_cast<char*>(&tmpbook),sizeof(Book));//更新书的数量
    fin.close();
    transaction tmp(ISBN_,USERONLINE.top().user_id,quantity_,quantity_ * tmpbook.price);
    fin.open(TRANSACTION_FILE,ios::in | ios::out | ios::binary);
    if(!fin)throw("buy4");
    fin.seekp(0,ios::end);
    fin.write(reinterpret_cast<char *>(&tmp),sizeof(transaction));//更新一笔交易
    fin.close();
    fout.open(TOTLEMONEY_FILE,ios::in | ios::out | ios::binary);
    if(!fout)throw("import3");
    fout.seekg(0);
    totlemoney tmp2;
    fout.read(reinterpret_cast<char*>(&tmp2),sizeof(totlemoney));
    tmp2.benefit += quantity_ * tmpbook.price;
    fout.seekp(0);
    fout.write(reinterpret_cast<char*>(&tmp2),sizeof(totlemoney));//更新总支出
    fout.close();
}


void showfinance(){
    fstream fin;
    fin.open(TOTLEMONEY_FILE,ios::in | ios::out | ios::binary);
    fin.seekg(0);
    totlemoney tmp;
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(totlemoney));
    cout<<"+"<<" "<<tmp.benefit<<" "<<"-"<<" "<<-tmp.expense<<endl;
    fin.close();
}

void showfinancetime(int times){
    fstream fin;
    fin.open(TRANSACTION_FILE,ios::in | ios::out | ios::binary);
    fin.seekg(ios::end - sizeof(transaction));
    int cur,next;
    cur = fin.tellg();
    next = cur - sizeof(transaction);
    int cnt = 1;
    for(; cnt <= times ; cnt++){
        if(cur < 0)throw("showfinancetimes1");
        transaction tmp;
        fin.seekg(cur);
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(transaction));
        if(tmp.totalprice > 0)cout<<"+"<<" "<<setprecision(2)<<tmp.totalprice<<endl;
        else cout<<"-"<<" "<<setprecision(2)<<-tmp.totalprice<<endl;
        cur = next;
        next -= sizeof(transaction);
    }
    fin.close();
}

void Run_Program(){

}