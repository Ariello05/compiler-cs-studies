#pragma once
#include <vector>
#include <string>
#include <exception>

using std::string;

enum MTYPE {ARRAY, VARIABLE, CONST, AC};

class MemBlock{
    public:
     
    MemBlock(long long value, MTYPE type){
        this->value = value;
        this->type = type;
        isDef = true;
    }
    MemBlock(MTYPE type){
        this->type = type;
        isDef = false;
        if(type == CONST){
            throw std::runtime_error("Can't declare undefined const");
        }
    }

    void setValue(long long value){
        this->value = value;
        isDef = true;
    }

    long long getValue(){
        if(!isDefined()){
            throw std::runtime_error("Can't access undefined value");
        }
        return value;
    }   

    bool isDefined(){
        return isDef;
    }

    MTYPE getType(){
        return type;
    }

    private:

    long long value;
    MTYPE type;
    bool isDef;
};

