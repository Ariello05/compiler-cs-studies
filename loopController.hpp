#pragma once
#include <vector>
#include "structs.hpp"

enum ACCESS_TYPE {ASSIGNMENT, CALL};

class VarAccess{
    public://too small class for enc.

    ACCESS_TYPE type;
    string variableName;
    long long variableIndex;
};

/**
    * Controls definitions inside loops 
    * eg. If there is undef under 'if' which won't be defined in other 'if' it will throw error
*/
class LoopController{
    public:
        LoopController();
        void proccessAction(VarAccess va);
        void enterLoop();
        void finishLoop();
        long long getLoopDepth();
        void enterIf();
        void finishIf();
        bool isInIfMode();
        void terminate();//obsolete
    private:

        long long loopDepth;//0 - no loops
        bool ifMode;
        std::vector<VarAccess> defs;
};