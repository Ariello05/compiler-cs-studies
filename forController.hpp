#pragma once
#include <vector>
#include "structs.hpp"

enum ACCESS_TYPE {ASSIGNMENT, CALL};

struct VarAccess{
    ACCESS_TYPE type;
    string variableName;
    long long variableIndex;
    long long iteration_run;
};


enum CONDITION_TYPE {_EQ,_NEQ,_LE,_LEQ,_GE,_GEQ};
class ForLoopController{
    public:
        ForLoopController();
        void setForLoopBlock(ForLoopBlock flb_arg);
        void proccessCondition(SmartBlock letf, CONDITION_TYPE type, SmartBlock right);
        bool inKnownFor();
        void proccessAction(VarAccess va);
        void finishForLoop();
        void putAction(VarAccess def);
        void terminate();//TODO: while break
    private:

        void runSwitch(long long left, CONDITION_TYPE type, long long right, bool leftIterator);
        long long current;
        std::vector<VarAccess> defs;
        ForLoopBlock flb;
        bool inKnown;//TODO: stack or vector to control depth recurrence
};