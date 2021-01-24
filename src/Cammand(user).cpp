//
// Created by 18382 on 2021/1/21.
//

#include "Cammand(user).h"
#include <iomanip>
#include <algorithm>
#include <cstring>
nodelist KEYWORD_LIST(KEYWORD_FILE);
nodelist USER_ID_LIST(USER_ID_LIST_FILE);
nodelist ISBN_LIST(ISBN_FILE);
nodelist AUTHOR_LIST(AUTHOR_FILE);
nodelist NAME_LIST(NAME_FILE);
stack<user>USERONLINE;


user::user(const string &user_id_, const string &pass, const string &name_, int pri) {
    strcpy(user_id,user_id_.c_str());
    strcpy(password,pass.c_str());
    strcpy(name,name_.c_str());
    priority = pri;
}



void login(const string &user_id_,string password ){
    if(strlen(password.c_str()) == 0){
        vector<int>possibleoffset;
        USER_ID_LIST.findnode(user_id_,possibleoffset);
        if(possibleoffset.empty())throw("cannot find the userid");
        user tmp(my_read<user>(USER,possibleoffset[0]));
        if (USERONLINE.top().priority > tmp.priority){
            USERONLINE.push(tmp);
        }else{
            //cout<<"1";
            throw("error");
        }
    }else{
        vector<int>psf;
        USER_ID_LIST.findnode(user_id_,psf);
        if(psf.empty()){
            //cout<<"2";
            throw("cannot find the userid");
        }
        user tmp(my_read<user>(USER,psf[0]));
        if(strcmp(tmp.password,password.c_str()) == 0 && strcmp(tmp.user_id,user_id_.c_str()) == 0){
            USERONLINE.push(tmp);
        }else{
            //cout<<"3";
            throw("error");
        }
    }
}

void logout(){
    if(USERONLINE.empty())throw("emptystack when log out");
    else USERONLINE.pop();
}

void register_(user & x){
    vector<int>possibleoffset;
    USER_ID_LIST.findnode(x.user_id,possibleoffset);
    if(!possibleoffset.empty())throw("error");
    int offset = my_write<user>(USER,x);//缺省
    node a(offset,x.user_id);
    USER_ID_LIST.addnode(a);
}

void addaccount(user &x){
    vector<int>possibleoffset;
    USER_ID_LIST.findnode(x.user_id,possibleoffset);
    if(!possibleoffset.empty())throw("error");
    int offset = my_write<user>(USER,x);//缺省
    node a(offset,x.user_id);
    USER_ID_LIST.addnode(a);
}

void deleteaccount(const string & user_id){
    if(user_id == "root")throw("error");
    vector<int>possibleoffset;
    USER_ID_LIST.findnode(user_id,possibleoffset);
    if(possibleoffset.empty())throw("cannot find the userid");
    //user tmp(my_read<user>(USER_ID_LIST_FILE,possibleoffset[0]));
    node newnode(possibleoffset[0],user_id);
    USER_ID_LIST.deletenode(newnode);
}

void changepassword(const char* user_id,const char * newpas,const char* oldpas ){
    if(strcmp(oldpas,newpas) == 0)throw"o";
    vector<int>possibleoffset;
    USER_ID_LIST.findnode(user_id,possibleoffset);
    if(possibleoffset.empty())throw("cannot find the userid");
    user tmp(my_read<user>(USER,possibleoffset[0]));
    if(strlen(oldpas) != 0){
        if(strcmp(oldpas,tmp.password) != 0){
            throw("wrong oldpassword");
        }
    }
    if(strcmp(tmp.password,newpas) == 0)throw("o");
    strcpy(tmp.password,newpas);
    my_write<user>(USER,tmp,possibleoffset[0]);
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

void import(int quantity_, double totleprice ){
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
    printf("%.2lf\n",quantity_ * tmpbook.price);
}


void showfinance(){
    fstream fin;
    fin.open(TOTLEMONEY_FILE,ios::in | ios::out | ios::binary);
    fin.seekg(0);
    totlemoney tmp;
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(totlemoney));
    cout<<"+"<<" ";
    printf("%.2lf ",tmp.benefit);
    cout<<"-"<<" ";
    printf("%.2lf\n",-tmp.expense);
    fin.close();
}

void showfinancetime(int times){
    fstream fin;
    fin.open(TRANSACTION_FILE,ios::in | ios::out | ios::binary);
    fin.seekg(0,ios::end);
    int x = fin.tellg();
    int t = x - sizeof (transaction);
    fin.seekg(t);
    int cur,next;
    cur = fin.tellg();
    next = cur - sizeof(transaction);
    int cnt = 1;
    double shouru =0,zhichu = 0;
    for(; cnt <= times ; cnt++){
        if(cur < 0)throw("showfinancetimes1");
        transaction tmp;
        fin.seekg(cur);
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(transaction));
        if(tmp.totalprice > 0){
            shouru += tmp.totalprice;
//            cout<<"+"<<" ";
//            printf("%.2lf\n",tmp.totalprice);
        }
        else {
            zhichu += tmp.totalprice;
//            cout<<"-"<<" ";
//            printf("%.2lf\n",-tmp.totalprice);
        }
        cur = next;
        next -= sizeof(transaction);
    }
    cout<<"+"<<" ";
    printf("%.2lf ",shouru);
    cout<<"-"<<" ";
    printf("%.2lf\n",-zhichu);
    fin.close();
}

void show(const string &key,nodelist &a){
    vector<int>possibleoffset;
    a.findnode(key,possibleoffset);
    if(possibleoffset.empty()) {
        cout<<"\n";
        return;
    }
    vector<Book>bookstack;
    int i = 0;
    fstream fin(BOOK_FILE,ios::in | ios :: out | ios ::binary);
    while(i < possibleoffset.size()){
        if(!fin)throw("error");
        fin.seekg(possibleoffset[i]);
        Book tmp;
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(Book));
        bookstack.push_back(tmp);
        i++;
    }
    fin.close();
    if(bookstack.empty()){
        cout<<"\n";
        return;
    }
    sort(bookstack.begin(),bookstack.end());
    for(auto & it : bookstack){
        it.show();
    }
}




void show(){
    fstream fin(BOOK_FILE,ios::in | ios :: out | ios ::binary);
    fin.seekg(0,ios::end);
    int offsetmax = fin.tellg();
    vector<Book>bookstack;
    int cur = 0;
    while(cur < offsetmax){
        Book tmp;
        fin.seekg(cur);
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(Book));
        bookstack.push_back(tmp);
        cur += sizeof(Book);
    }
    if(bookstack.empty()){
        cout<<"\n";
        return;
    }
    sort(bookstack.begin(),bookstack.end());
    for(auto & i : bookstack){
        i.show();
    }
    fin.close();
}

void modifyISBN(const string &key){
//    vector<int>possibleoffset;
//    a.findnode(key,possibleoffset);
//    if(possibleoffset.empty())throw"error";
    int offset = USERONLINE.top().select;//
    fstream fin(BOOK_FILE,ios::in | ios :: out | ios ::binary);
    fin.seekg(offset);
    Book tmp;
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(Book));
    vector<int>possibleoffset;
    vector<int>possibleoffset2;
    ISBN_LIST.findnode(key,possibleoffset2);
    if(!possibleoffset2.empty())throw("e");
    //ISBN_LIST.findnode(tmp.ISBN,possibleoffset);
    //if(!possibleoffset.empty()) {
        node a(offset, tmp.ISBN);
        ISBN_LIST.deletenode(a);
    //}
//    if(!possibleoffset.empty()) {
//        Book tmpt;
//        fstream fout;
//        fout.open(BOOK_FILE,ios::in|ios::out|ios::binary);
//        fout.seekg(offset);
//        fout.read(reinterpret_cast<char*>(&tmpt),sizeof(Book));
//        if(strlen(tmpt.ISBN) != 0 || strlen(tmpt.key_word) == 0 || strlen(tmpt.author) == 0 || strlen(tmpt.name) == 0)throw"e";
//    }
    node b(offset,key);
    ISBN_LIST.addnode(b);
    strcpy(tmp.ISBN,key.c_str());
    fin.seekp(offset);
    fin.write(reinterpret_cast<char*>(&tmp),sizeof(Book));
    fin.close();
}




void modifyNAME(const string &key){
    int offset = USERONLINE.top().select;
    fstream fin(BOOK_FILE,ios::in | ios :: out | ios ::binary);
    fin.seekg(offset);
    Book tmp;
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(Book));
    vector<int>possibleoffset;
   NAME_LIST.findnode(tmp.name,possibleoffset);
//    if(possibleoffset.empty()) {
//        node x(offset,key);
//        NAME_LIST.addnode(x);
//    }
    if(!possibleoffset.empty()) {
        node a(offset, tmp.name);
        NAME_LIST.deletenode(a);
    }
    node b(offset,key);
    NAME_LIST.addnode(b);
    strcpy(tmp.name,key.c_str());
    fin.seekp(offset);
    fin.write(reinterpret_cast<char*>(&tmp),sizeof(Book));
    fin.close();
}



void modifyAUTHOR(const string &key){
    int offset = USERONLINE.top().select;
    fstream fin(BOOK_FILE,ios::in | ios :: out | ios ::binary);
    fin.seekg(offset);
    Book tmp;
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(Book));
    vector<int>possibleoffset;
//    vector<int>possibleoffset2;
//    AUTHOR_LIST.findnode(key,possibleoffset2);
//    if(!possibleoffset2.empty())throw("e");
    AUTHOR_LIST.findnode(tmp.author,possibleoffset);
//    if(possibleoffset.empty()) {
//        node x(offset,key);
//        AUTHOR_LIST.addnode(x);
//    }
    if(!possibleoffset.empty()) {
        node a(offset, tmp.author);
        AUTHOR_LIST.deletenode(a);
    }
    node b(offset,key);
    AUTHOR_LIST.addnode(b);
    strcpy(tmp.author,key.c_str());
    fin.seekp(offset);
    fin.write(reinterpret_cast<char*>(&tmp),sizeof(Book));
    fin.close();
}




void modifyKEYWORD(const string &command){
    vector<string>oldkey;
    int offset = USERONLINE.top().select;
    fstream fin;
    fin.open(BOOK_FILE,ios::in | ios::out| ios::binary);
    fin.seekg(offset);
    Book tmp;
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(Book));
    string key = tmp.key_word;
    splitkey(key,oldkey);
    sort(oldkey.begin(),oldkey.end());
    if(!oldkey.empty()){
        for(auto & k : oldkey){
            node a1(offset,k);
            KEYWORD_LIST.deletenode(a1);
        }
    }
    vector<string>keys;
    splitkey(command,keys);
    for(int i = 1 ; i < keys.size() - 1;i++){
        if(strcmp(keys[i].c_str(),keys[i + 1].c_str()) == 0)throw("e");
    }
    for(auto & j : keys){
        node a2(offset,j);
        KEYWORD_LIST.addnode(a2);
    }
    strcpy(tmp.key_word,command.c_str());
    fin.seekp(offset);
    fin.write(reinterpret_cast<char*>(&tmp),sizeof(Book));
}




void modifyPRICEC(double price){
    int offset = USERONLINE.top().select;
    fstream fin(BOOK_FILE,ios::in | ios :: out | ios ::binary);
    fin.seekg(offset);
    Book tmp;
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(Book));
    tmp.price = price;
    fin.seekp(offset);
    fin.write(reinterpret_cast<char*>(&tmp),sizeof(Book));
    fin.close();
}



void Run_Program(string &a){
    int len = a.length();
    string type;
    int i = 0;
    for( ; i < len  && a[i] != ' ';i++){
        type += a[i];
    }
    if(type == "su"){
        string user_id,passwd;
        for (++i; i < len && a[i] != ' '; ++i) user_id += a[i];
        for (++i; i < len && a[i] != ' '; ++i) passwd += a[i];
        for (; i < len && a[i] == ' '; ++i);
        if(i >= len && strlen(user_id.c_str()) <= 30 &&  strlen(user_id.c_str()) > 0 && strlen(passwd.c_str()) <= 30 ){
            login(user_id,passwd);
            return;
        }else{
            throw("error");
        }
        return;
    }
    else if(type == "logout"){
        if(USERONLINE.empty())throw("e");
        for (; i < len && a[i] == ' '; ++i);
        if(i >= len )logout();
        return;
    }
    else if(type == "useradd"){
        string user_id, passwd, name; char pri;
        for (++i; i < len && a[i] != ' '; ++i) user_id += a[i];
        for (++i; i < len && a[i] != ' '; ++i) passwd += a[i];
        if(i == len )throw("error");
        pri = a[++i];
        ++i;
        for (++i; i < len && a[i] != ' '; ++i) name += a[i];
        for (; i < len && a[i] == ' '; ++i);
        if(i >= len && strlen(user_id.c_str()) > 0 && strlen(passwd.c_str()) > 0 && strlen(name.c_str()) > 0 &&
                strlen(user_id.c_str()) <= 30 && strlen(passwd.c_str()) <= 30 && strlen(name.c_str()) < 30 && pri - '0' < USERONLINE.top().priority)
        {
            user tmp(user_id,passwd,name,pri-'0');
            addaccount(tmp);
            return;
        }
        else throw("error");
    }
    else if(type == "register"){
        string user_id, passwd, name;
        for (++i; i < len && a[i] != ' '; ++i) user_id += a[i];
        for (++i; i < len && a[i] != ' '; ++i) passwd += a[i];
        for (++i; i < len && a[i] != ' '; ++i) name += a[i];
        for (; i < len && a[i] == ' '; ++i);
        if(i >= len && strlen(user_id.c_str()) > 0 && strlen(passwd.c_str()) > 0 && strlen(name.c_str()) > 0 &&
           strlen(user_id.c_str()) <= 30 && strlen(passwd.c_str()) <= 30 && strlen(name.c_str()) < 30)
        {
            user tmp(user_id,passwd,name,1);
            register_(tmp);
            return;
        }else{
            throw("error");
            return;
        }
    }
    else if(type == "delete"){
        if(USERONLINE.empty())throw("e");
        string user_id;
        for (++i; i < len && a[i] != ' '; ++i) user_id += a[i];
        for (; i < len && a[i] == ' '; ++i);
        if(i >= len && strlen(user_id.c_str()) > 0 && strlen(user_id.c_str()) <= 30 && USERONLINE.top().priority >= 7){
            deleteaccount(user_id);
        }
        else{
            throw"er";
        }
        return;
    }
    else if(type == "passwd"){
        if(USERONLINE.empty())throw("e");
        string user_id, oldpass, newpass;
        for (++i; i < len && a[i] != ' '; ++i) user_id += a[i];
        for (++i; i < len && a[i] != ' '; ++i) oldpass += a[i];
        for (++i; i < len && a[i] != ' '; ++i) newpass += a[i];
        for (; i < len && a[i] == ' '; ++i);
        if(i >= len && strlen(user_id.c_str()) > 0 && strlen(oldpass.c_str()) > 0 && strlen(user_id.c_str()) <= 30 && strlen(oldpass.c_str()) <= 30 && strlen(newpass.c_str()) <= 30)
        {
            if(newpass.empty() && USERONLINE.top().priority == 7){
                changepassword(user_id.c_str(),oldpass.c_str(),newpass.c_str());
                return;
            }
            else if(USERONLINE.top().priority >= 1){
                changepassword(user_id.c_str(),newpass.c_str(),oldpass.c_str());
                return;
            }else throw("error");
        }
        return;
    }
    else if(type == "select"){
        if(USERONLINE.empty())throw("e");
        string ISBN;
        for (++i; i < len && a[i] != ' '; ++i) ISBN += a[i];
        for (; i < len && a[i] == ' '; ++i);
        if(i >= len && strlen(ISBN.c_str()) > 0 && strlen(ISBN.c_str()) <= 20){
            selectbook(ISBN);//函数内部判断是否足够权限
            return;
        }
        else throw("error");
        return;
    }
    else if(type == "import"){
        if(USERONLINE.empty())throw("e");
        string value1, value2; int quantity; double cost_price;
        for (++i; i < len && a[i] != ' '; ++i) {
            if(a[i] < '0' || a[i] > '9')throw"3";
            value1 += a[i];
        }
        int cnt = 0;
        for (++i; i < len && a[i] != ' '; ++i) {
            if(a[i] == '.')cnt++;
            if((a[i] != '.' &&(a[i] < '0' || a[i] > '9')) || cnt >= 2 )throw"3";
            value2 += a[i];
        }
        if (strlen(value1.c_str()) > 0 && strlen(value2.c_str()) > 0) {
            stringstream str1(value1), str2(value2);
            str1 >> quantity, str2 >> cost_price;
            for (; i < len && a[i] == ' '; ++i);
            if (i >= len ) {
                import(quantity,cost_price);
                return;
            } else throw("error");
        } else throw("error");
        return;
    }
    else if(type == "modify"){
        if(USERONLINE.top().select == -1)throw"o";
        while(i < len){
            string command;
            for (++i; a[i] != ' ' && a[i] != '\0' && a[i] != '\n'; ++i) command += a[i];
            switch (command[1]) {
                case 'I':{
                    command = command.substr(6);
                    //command = command.substr(1,strlen(command.c_str()) - 2);
                    modifyISBN(command);
                    break;
                }
                case 'n':{
                    command = command.substr(6);
                    command = command.substr(1,strlen(command.c_str()) - 2);
                    modifyNAME(command);
                    break;
                }
                case 'a':{
                    command = command.substr(8);
                    command = command.substr(1,strlen(command.c_str()) - 2);
                    modifyAUTHOR(command);
                    break;
                }
                case 'k':{
                    command = command.substr(9);
                    command = command.substr(1,strlen(command.c_str()) - 2);
                    modifyKEYWORD(command);
                    break;
                }
                case 'p':{
                    command = command.substr(7);
                    stringstream str;
                    str<<command;
                    double price;
                    str>>price;
                    modifyPRICEC(price);
                    break;
                }
                default:throw("error");
            }
        }
        return;
    }
    else if(type == "show"){
        if(USERONLINE.empty())throw("e");
        if(i >= len || a[i + 1] == '-') {
            string command;
            for (++i; i < len; i++) {
                command += a[i];
            }
            //show操作
            if (USERONLINE.top().priority >= 1) {
                if (command.empty()){
                    show();
                    return;
                }
                else {
                    switch (command[1]) {
                        case 'I': {
                            command = command.substr(6);
                            show(command, ISBN_LIST);
                            return;
                        }
                        case 'n': {
                            command = command.substr(6);
                            command = command.substr(1,strlen(command.c_str()) - 2);
                            show(command, NAME_LIST);
                            return;
                        }
                        case 'a': {
                            command = command.substr(8);
                            command = command.substr(1,strlen(command.c_str()) - 2);
                            show(command, AUTHOR_LIST);
                            return;
                        }
                        case 'k': {
                            command = command.substr(9);
                            command = command.substr(1,strlen(command.c_str()) - 2);
                            if(strlen(command.c_str()) > 60)throw("t");
                            vector<string>key;
                            splitkey(command,key);
                            show(command, KEYWORD_LIST);
                            if(key.size() > 1 || key.empty())throw"ee";
                            return;
                        }
                        default:
                            throw ("error");
                    }
                }
            }
            return;
        }else{
            string type2;
            for (++i; i < len && a[i] != ' '; ++i) type2 += a[i];
            if(type2 == "finance"){
                string value;
                int times;
                for (++i; i < len && a[i] != ' '; ++i) value += a[i];
                for (; i < len && a[i] == ' '; ++i);
                if(i >= len && USERONLINE.top().priority == 7){
                    if(value.empty()){
                        showfinance();
                        return;
                    }
                    else {
                        stringstream str(value);
                        str >> times;//快速转化
                        showfinancetime(times);
                        return;
                    }
                } else throw"error";
            }else throw"error";
        }
    }
    else if(type == "buy"){
        string ISBN, value; int quantity;
        for (++i; i < len && a[i] != ' '; ++i) ISBN += a[i];
        for (++i; i < len && a[i] != ' '; ++i) value += a[i];
        stringstream str(value); str >> quantity;
        for (; i < len && a[i] == ' '; ++i) ;
        if (i >= len && strlen(value.c_str()) > 0 && strlen(ISBN.c_str()) > 0 && strlen(ISBN.c_str()) <= 20 && USERONLINE.top().priority >= 1) {
            {
                buy(ISBN,quantity);
                return;
            }
        } else throw("error");
    }
    else if(type == "report"){}
    else if(type == "log"){}
    else if(type == "exit" || type == "quit" ){
        fstream fin,fout;
        exit(0);
    }
    else if(type == "clear"){
        clear();
    }
    else throw("errorlast");

}


void clear(){
    fstream fin,fout;
    fin.open(USER_ID_LIST_FILE,ios::out);
    fin.close();
    fin.open(USER,ios::out);
    fin.close();
    fin.open(TOTLEMONEY_FILE,ios::out);
    fin.close();
    fin.open(TRANSACTION_FILE,ios::out);
    fin.close();
    fin.open(BOOK_FILE,ios::out);
    fin.close();
    fin.open(ISBN_FILE,ios::out);
    fin.close();
    fin.open(AUTHOR_FILE,ios::out);
    fin.close();
    fin.open(NAME_FILE,ios::out);
    fin.close();
    fin.open(KEYWORD_FILE,ios::out);
    fin.close();
    user root("root","sjtu","root",7);
    int offset = my_write<user>(USER,root);
    node tmp(offset,"root");
    USER_ID_LIST.addnode(tmp);
    totlemoney tmpq;
    fout.open(TOTLEMONEY_FILE,ios::in | ios ::out | ios::binary);
    fout.seekp(0);
    fout.write(reinterpret_cast<char*>(&tmpq),sizeof(totlemoney));
    fout.close();
}


void splitkey(string str,vector<string>&a){
    stringstream s(str);
    string tmp;
    while(getline(s,tmp,'|'))a.push_back(tmp);
}



int getselect(){
    return USERONLINE.top().select;
}