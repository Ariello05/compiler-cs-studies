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

/**
    * Class encodes commands into virtual machine notion
    *
*/
class Coder{
public:
    Coder(std::shared_ptr<MemoryController> mc);

    /* EXPRESSION BLOCK*/
    void addVars();
    void subVars();
    void getValue();
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
    void verifyStack();//verifies if variables are defined etc.
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
    void write();
    void end();


    //void declareVar(long long pid);

    /* DEBUG AND HELP BLOCK */
    void printVM();//prints result
    long long getCurrentPosition();//current virutal machine index k
    void setOutput(string str);//output file

    void incDepth();
    void decDepth();

private:
    bool alreadyInAC(unsigned long long index);//optimization
    bool divSwitch(SmartBlock first, SmartBlock second, unsigned long long old_b, unsigned long long one, unsigned long long two);
    void loadArrayVar(unsigned long long arrayIndex, unsigned long long varIndex);//Load value of array at variable
    void defineValueUnsinged(unsigned long long value);//define constant
    void defineValueUnsinged(std::vector<string> & storeCode, long long value);//define constant but don't put definition here
    void defineValue(long long value);//define constant
    void defineValue(std::vector<string> & storeCode, long long value);//define constant but don't put definition here

    bool forward;//forward arrayvar call, used in assign to shorten one STORE command
    string filename;//output 
    string ignore;//used to help with assingments under if blocks; eg. a ASSIGN a doesn't clear unassingment flag

    std::vector<string> vm;//virtual machine commands
    std::shared_ptr<MemoryController> mc;//memory controller
    std::stack<SmartBlock> args;//smart blocks on stack, they contain all necessary info to distinguish types
    LoopController lc;//loop controller checks and controls unassingemnts under loops and if statments
    //std::stack<long long> loopJumps;
    std::stack<long long> jumps;//jump stack

};