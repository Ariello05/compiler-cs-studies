#pragma once
#include <vector>
#include "structs.hpp"

enum ACCESS_TYPE {ASSIGNMENT, CALL};

struct VarAccess{
    ACCESS_TYPE type;
    string variableName;
    long long variableIndex;
};


class LoopController{
    public:
        LoopController();
        void proccessAction(VarAccess va);
        void enterLoop();
        void finishLoop();
        void enterIf();
        void finishIf();
        bool isInIfMode();
        void terminate();//TODO: while break
    private:

        long long loopDepth;
        bool ifMode;
        std::vector<VarAccess> defs;
        //ForLoopBlock flb;
        //bool inKnown;//TODO: stack or vector to control depth recurrence
};