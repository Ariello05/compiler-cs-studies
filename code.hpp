#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include "memc.hpp"

using std::string;
namespace machine{
    static const string ADD = "ADD ";
    static const string LOAD = "LOAD ";
    static const string SHIFT = "SHIFT ";

}

class SmartBlock{//Helper
    public:
    SmartBlock(bool isVariable, long long value, string arrayName = ""){
        this->isVariable = isVariable;
        this->value = value;
        this->arrayName = arrayName;
    }
    SmartBlock& operator=(SmartBlock & two){
        this->isVariable = two.isVariable;
        this->value = two.value;
        this->arrayName = two.arrayName;
        return *this;
    }
    bool isVariable;
    string arrayName;
    long long value;
};

class Coder{
public:
    Coder(std::shared_ptr<MemoryController> mc);

    /* EXPRESSION BLOCK*/
    void addVars();//USES STACK
    void subVars();//USES STACK
    void getValue();//USES STACK

    /* CONDITION BLOCK*/
    void processEQ();
    void processNEQ();
    void processLE();
    void processGE();
    void processLEQ();
    void processGEQ();

    /* DECLERATION BLOCK */
    void declareArray(string name, long long begin, long long end);
    void declareVariable(string name);

    /* VALUE BLOCK */
    void stackVariable(long long index);
    void stackValue(long long value);
    void clearStack();

    void accessArrayWithVariable(long long variable);
    long long loadIdentifier(long long pid);
    void setArrayToAccess(string array);
    
    /* COMMAND BLOCK */
    void assignValueToVar(long long id, long long value);
    void endif();
    void startelse();
    void stackJump();
    void endWhile(long long start);
    void endDoWhile();
    void read(long long pid);
    void write();//USES STACK
    void end();

    //void declareVar(long long pid);

    /* DEBUG AND HELP BLOCK */
    void printVM();
    long long getCurrentPosition();


private:
    void defineValue(long long value);//returns address to memory

    string arrayLocal;
    std::vector<string> vm;
    std::shared_ptr<MemoryController> mc;
    std::stack<SmartBlock> args;
    std::stack<long long> loopJumps;
    std::stack<long long> jumps;

};