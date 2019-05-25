#ifndef DATABASE_H
#define DATABASE_H


#include <vector>
#include <string>
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <cstring>
#include<iostream>
#include<stdio.h>
#include<QtSql>
#include<QDebug>
#include<QFileInfo>


using namespace std;
struct dataa
{
    int id;
    string fio;
    string tel;
    string date;
};

struct logins
{
    string log;
    string pass;
    string status;
};

/*
Функция проверки имени. С 65 по 122 находятся строчные и прописные буквы,за пределами 65 и 122 находятся различные символы,
поэтому они нам не подходят. isalpha проверяет символ и возвращает ненулевое значение, если символ является буквой алфавита,
,поэтому ! вернет 0 в случае,если это не буква.с 65 по 90 находятся заглавные буквы,а с 97 по 122 строчные,поэтому
если встречаются строчные,то мы переводим их в заглавные
*/
string checkFio(string);//Используемые символы - только буквы; Если регистр не тот, меняем на правильный.

/*
isdigit проверяет является ли символ числом и возвращает 1,если это цифра.Счётчик k считает количество цифр в номере
и если количество равно 10 то функция вернёт 1.
*/
string checkTel(string);//длина строки 10 символов, все из которых цифры;

//bool checkDate(string date)//тип данных Date
//{

//}

class DataBase
{
private:
    vector<dataa> db;
    //vector<dataa>::iterator it;
    int id;
public:


    DataBase();
    ~DataBase();

    vector<dataa> add(vector<dataa>,vector<dataa>); // Добавить в базу


    vector<dataa> del(vector<dataa>,int);//удаляем данные с заданным id

    vector<dataa> change(vector<dataa>,vector<dataa>);// меняем данные с заданным id, на заданные данные temp_data

    int find(vector<dataa>,int);

    template<typename temptype>

    vector<dataa> find(int crit, temptype Znach);

    vector<dataa> sort(vector<dataa>);

    vector<dataa> read(); //считывает всю БД из файла

    void write(vector<dataa>); //запись всей БД в файл


    fstream openFile();

    fstream closeFile();

    dataa parsing(string);

    string data2string(vector<dataa>::iterator);

    fstream openLog();

    vector<logins> readLog();

    vector<logins> zamenaLog();

};

#endif // DATABASE_H
