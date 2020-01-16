#pragma once
#include <vector>
#include <string>
#include <exception>

using std::string;

/**
 * ARRAY - array item
 * VARIABLE - variable
 * SPECIAL - special
 * CONST - local constant 
 * AC - register
*/
enum MTYPE {ARRAY, VARIABLE, CONST, AC, SPECIAL};

/**
 * Container for virtual machine memory blocks 
 * 
*/ 
class MemBlock{
    public:
     
    MemBlock(long long value, MTYPE type, string name=""){
        this->name = name;
        this->value = value;
        this->type = type;
        isDef = true;
    }
    MemBlock(MTYPE type, string name=""){
        this->name = name;
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

    string getName(){
        return name;
    }

    MTYPE getType(){
        return type;
    }

    private:

    string name;
    long long value;
    MTYPE type;
    bool isDef;
};

