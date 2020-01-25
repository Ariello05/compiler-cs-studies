#include "loopController.hpp"
LoopController::LoopController(){
loopDepth = 0;
}

void LoopController::proccessAction(VarAccess va){
    if(va.type == ACCESS_TYPE::ASSIGNMENT){
        auto begin = defs.begin();
        auto end = defs.end();

        while(begin != end){

            if(begin->variableName == va.variableName && begin->type == ACCESS_TYPE::CALL){
                begin = defs.erase(begin);
                end = defs.end();
            }else{
                ++begin;
            }
        }
        
    }else{
        defs.push_back(va);
    }

}

void LoopController::enterLoop(){
    ++loopDepth;
}

void LoopController::finishLoop(){
    --loopDepth;
    if(loopDepth == 0){
        auto begin = defs.begin();
        const auto end = defs.end();

        while(begin != end){
            if(begin->type == ACCESS_TYPE::CALL){
                throw std::runtime_error("Undefined variable inside loop block, namely: " + begin->variableName);
            }
            ++begin;
        }
    }

}

void LoopController::enterIf(){
    ifMode = true;
}

void LoopController::finishIf(){
    if(loopDepth <= 1){
        ifMode = false;
        
        if(loopDepth == 0){
        auto begin = defs.begin();
        const auto end = defs.end();

        while(begin != end){
            if(begin->type == ACCESS_TYPE::CALL){
                throw std::runtime_error("Undefined variable after if statement block, namely: " + begin->variableName);
            }
            ++begin;
        }
    }
    }
}

bool LoopController::isInIfMode(){
    return ifMode;
}
