#include "autorize.h"
#include<iostream>
#include<fstream>
#include<QDebug>
using namespace std;


string autorizes(QString log,QString pass){
    fstream file;
    file.open("C:\\repos\\lab2\\Server\\log.txt",ios::binary | ios::in);
    if(!file.is_open())
    {
        return 0;
    }
    char buff[256];
    char buff2[256];
    char buff3[256];
    while(!file.eof())
    {
        file.getline(buff, 256);
                if (buff[0] == '{' || buff[0] == '}')
                    continue;
                char*ptr = strtok(buff, ":");
                while (ptr != nullptr)
                {
                    ptr = strtok(nullptr, "\"");
                    ptr = strtok(nullptr, "\"");
                    strcpy(buff,ptr);
                    ptr = strtok(nullptr, ":");
                    ptr = strtok(nullptr, "\"");
                    ptr = strtok(nullptr, "\"");
                    if(ptr!=nullptr)
                    {
                    strcpy(buff2,ptr);
                    }
                    ptr = strtok(nullptr, ":");
                    ptr = strtok(nullptr, "\"");
                    ptr = strtok(nullptr, "\"");
                    strcpy(buff3,ptr);
                    ptr = nullptr;
                }
        if(log == buff && pass == buff2)
        {
            return buff3;
        }
    }
        return "login or password is incorrect";
}
