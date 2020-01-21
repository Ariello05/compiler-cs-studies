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
        //std::cerr<<name << isDef << "\n";
    }
    MemBlock(MTYPE type, string name=""){
        this->name = name;
        this->type = type;
        isDef = false;
        if(type == CONST){
            throw std::runtime_error("Can't declare undefined const");
        }
        //std::cerr<<name << isDef << "\n";

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
        //std::cerr<<name << isDef << "\n";
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
    ForLoopBlock(string iterator, unsigned long long iteratorIndex, bool increasing, unsigned long long special = 0){
        this->iterator = iterator;
        this->specialIndex = special;
        this->iteratorIndex = iteratorIndex;
        if(special != 0){
            this->special = true;//XD: special = true is not warned in god damn compiler, xD
        }else{
            this->special = false;
        }
        valueKnown = false;
        firstIteration = 0;
        increases = increasing;
    }

    ForLoopBlock(){
        valueKnown = false;
    }

    void setKnownInfo(long long firstIteration ){
        this->firstIteration = firstIteration;
        valueKnown = true;
    }

    long long getFirstIteration(){
        if(!valueKnown){
            throw std::runtime_error("Can't access unkown start point of iterator");
        }
        return firstIteration;
    }

    bool isIncreasing(){
        return increases;
    }

    bool isKnown(){
        return valueKnown;
    }

    bool isSpecial(){
        return special;
    }

    string getName(){
        return iterator;
    }

    unsigned long long getIteratorIndex(){
        return iteratorIndex;
    }

    unsigned long long getSpecialIndex(){
        if(!isSpecial()){
            throw std::runtime_error("Can't access special value of non special ForLoopBlock");
        }
        return specialIndex;
    }

private:

    bool special;// 1 TO t(a) requires to hold 2 values, iterator and special to hold value of t(a)
    string iterator;

    bool valueKnown;
    long long firstIteration;
    bool increases;

    unsigned long long iteratorIndex;
    unsigned long long specialIndex;
};


                //ARRAY(CONST) is treated as variable
enum IDENTIFIER {ARRAYVAR,CONSTVALUE,VARIABLE};
class SmartBlock{//Helper
    public:
    SmartBlock(IDENTIFIER type, unsigned long long value=0 , unsigned long long varName =0, unsigned long long arrayName = 0, string name = ""){
        this->type = type;
        switch(type){
            case ARRAYVAR:
                if(varName == 0){
                    throw std::runtime_error("Define varName!");
                }
                if(arrayName == 0){
                    throw std::runtime_error("Define arrayName!");
                }
                break;
            case CONSTVALUE://TODO : rethink
                //if(value == 0){
                //    throw std::runtime_error("Define value!");
                //}
                break;
            case VARIABLE:
                if(varName == 0){
                    throw std::runtime_error("Define varName!");
                }
                break;
        }
        this->value = value;
        this->arrayStartIndex = arrayName;
        this->variableIndex = varName;
        this->name = name;
    }
    SmartBlock& operator=(SmartBlock & two){
        this->type = two.type;
        this->value = two.value;
        this->arrayStartIndex = two.arrayStartIndex;
        this->variableIndex = two.variableIndex;
        return *this;
    }
    IDENTIFIER type;
    unsigned long long variableIndex;
    unsigned long long arrayStartIndex;
    long long value;
    string name;
};