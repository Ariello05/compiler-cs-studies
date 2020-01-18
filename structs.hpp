#pragma once
#include <vector>
#include <string>
#include <exception>
#include <iostream>

using std::string;

/**
 * ARRAY - array item
 * VARIABLE - variable
 * SPECIAL - special
 * CONST - local constant 
 * AC - register
*/
enum MTYPE {ARRAY, VAR, CONST, AC, SPECIAL, ITERATOR};

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

    void undef(){
        isDef = false;
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

class ForLoopBlock{
    public:
    ForLoopBlock(string iterator, long long iteratorIndex, long long special = 0){
        this->iterator = iterator;
        this->specialIndex = special;
        this->iteratorIndex = iteratorIndex;
        if(special != 0){
            this->special = true;//XD: special = true is not warned in god damn compiler, xD
        }else{
            this->special = false;
        }
    }

    bool isSpecial(){
        return special;
    }

    string getName(){
        return iterator;
    }

    long long getIteratorIndex(){
        return iteratorIndex;
    }

    long long getSpecialIndex(){
        if(!isSpecial()){
            throw std::runtime_error("Can't access special value of non special ForLoopBlock");
        }
        return specialIndex;
    }

    private:
    bool special;// 1 TO t(a) requires to hold 2 values, iterator and special to hold value of t(a)
    string iterator;
    long long iteratorIndex;
    long long specialIndex;
};
