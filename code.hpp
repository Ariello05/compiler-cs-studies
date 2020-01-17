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

                //ARRAY(CONST) is treated as variable
enum IDENTIFIER {ARRAYVAR,CONSTVALUE,VARIABLE};
class SmartBlock{//Helper
    public:
    SmartBlock(IDENTIFIER type, long long value=0 , long long varName =0, long long arrayName = 0){
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
            case CONSTVALUE:
                if(value == 0){
                    throw std::runtime_error("Define value!");
                }
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
    }
    SmartBlock& operator=(SmartBlock & two){
        this->type = two.type;
        this->value = two.value;
        this->arrayStartIndex = two.arrayStartIndex;
        this->variableIndex = two.variableIndex;
        return *this;
    }
    IDENTIFIER type;
    long long variableIndex;
    long long arrayStartIndex;
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
    //void clearStack();

    //void accessArrayWithVariable(long long variable);
    //long long loadIdentifier(long long pid);

    void stackValue(long long value);
    void stackVariable(string var);
    void stackArrayWithConst(long long value, string array);
    void stackArrayWithVariable(string var, string array);

    //void setArrayToAccess(string array);
    
    /* COMMAND BLOCK */
    void assignValueToVar(long long injectPoint);
    void endif();
    void startelse();
    void stackJump();
    void endWhile(long long start);
    void endDoWhile();
    void handleToFor(string iterator);
    void handleDownToFor(string iterator);
    void endFor(bool dec);
    void read();
    void write();//USES STACK
    void end();


    //void declareVar(long long pid);

    /* DEBUG AND HELP BLOCK */
    void printVM();
    long long getCurrentPosition();


private:
    void loadArrayWithVariable(long long arrayIndex, long long varIndex);//Load value of array at variable
    void defineValue(long long value);
    void defineValue(std::vector<string> & storeCode, long long value);

    std::vector<string> vm;
    std::shared_ptr<MemoryController> mc;
    std::stack<SmartBlock> args;
    //std::stack<long long> loopJumps;
    std::stack<long long> jumps;

};