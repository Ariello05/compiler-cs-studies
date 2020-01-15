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
    SmartBlock(bool isVariable, long long value){
        this->isVariable = isVariable;
        this->value = value;
    }
    SmartBlock& operator=(SmartBlock & two){
        this->isVariable = two.isVariable;
        this->value = two.value;
        return *this;
    }
    bool isVariable;
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

    
    long long loadIdentifier(long long pid);
    
    /* COMMAND BLOCK */
    void assignValueToVar(long long id, long long value);
    void endif(int offset = 0);
    void startelse();

    //void declareVar(long long pid);

    /* DEBUG AND HELP BLOCK */
    void printVM();


private:
    void defineValue(long long value);//returns address to memory


    std::vector<string> vm;
    std::shared_ptr<MemoryController> mc;
    std::stack<SmartBlock> args;
    std::stack<long long> loopJumps;
    std::stack<long long> jumps;

};