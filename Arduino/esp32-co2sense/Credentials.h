#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h>

class StringParameter {
    public: 
    StringParameter(int arrayLength);
    void set(const char* value);

    int length;
    char* value;
};

class Credentials {
   public:
    Credentials(void);
    void init();
    void load();
    void save();
    bool incomplete();
    void print();
    void remove();
    void set(char* parameter, const char* value, int parameterSize);

    StringParameter ssid = StringParameter(32);
    StringParameter password = StringParameter(32);
    StringParameter deviceid = StringParameter(40);
    StringParameter username = StringParameter(32);
};

#endif
