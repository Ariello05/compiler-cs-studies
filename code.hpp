#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include "memc.hpp"
#include <fstream>
#include "loopController.hpp"
#include "structs.hpp"

using std::string;
namespace machine{
    static const string ADD = "ADD ";
    static const string LOAD = "LOAD ";
    static const string SHIFT = "SHIFT ";

}

class Coder{
public:
    Coder(std::shared_ptr<MemoryController> mc);

    /* EXPRESSION BLOCK*/
    void addVars();//USES STACK
    void subVars();//USES STACK
    void getValue();//USES STACK
    void mulVars();
    void divVars();
    void modVars();

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

    void verifyStack();
    void stackValue(long long value);
    void stackVariable(string var);
    void stackArrayWithConst(long long value, string array);
    void stackArrayWithVariable(string var, string array);

    //void setArrayToAccess(string array);
    
    /* COMMAND BLOCK */
    void assignValueToVar(long long injectPoint);
    void startif();
    void endif();
    void startelse();
    void stackJump();
    void stackJump(long long index);
    void endWhile();
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

    void setOutput(string str);
    void incDepth();
    void decDepth();

private:
    bool alreadyInAC(unsigned long long index);
    bool divSwitch(SmartBlock first, SmartBlock second, unsigned long long old_b, unsigned long long one, unsigned long long two);
    void loadArrayVar(unsigned long long arrayIndex, unsigned long long varIndex);//Load value of array at variable
    void defineValueUnsinged(unsigned long long value);
    void defineValueUnsinged(std::vector<string> & storeCode, long long value);
    void defineValue(long long value);
    void defineValue(std::vector<string> & storeCode, long long value);

    bool forward;
    string filename;
    string ignore;

    std::vector<string> vm;
    std::shared_ptr<MemoryController> mc;
    std::stack<SmartBlock> args;
    LoopController lc;
    //std::stack<long long> loopJumps;
    std::stack<long long> jumps;

};