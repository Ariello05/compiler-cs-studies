#include "forController.hpp"
ForLoopController::ForLoopController(){
    inKnown = false;
}
void ForLoopController::setForLoopBlock(ForLoopBlock flb_arg){
    flb = flb_arg;
    if(flb.isKnown()){
        inKnown = true;
    }
    current = 0;
}   

void ForLoopController::proccessCondition(SmartBlock left, CONDITION_TYPE type, SmartBlock right){
    if(left.type == VARIABLE){
        if(left.name == flb.getName()){//is iterator
            if(right.type == CONSTVALUE){
                runSwitch(flb.getFirstIteration(),type,right.value,true);
            }
        }
    }else{
        current = -1;//unkown mode
        return;//TODO: more
    }
}


bool ForLoopController::inKnownFor(){
    return inKnown;
}

void ForLoopController::terminate(){
    inKnown = false;
    defs.clear();
}


void ForLoopController::proccessAction(VarAccess va){
    //...
    //std::cerr << va.variableName << std::endl;
}

void ForLoopController::finishForLoop(){
    inKnown = false;
}

void ForLoopController::putAction(VarAccess def){
}

void ForLoopController::runSwitch(long long left, CONDITION_TYPE type, long long right, bool leftIterator){
    switch(type){//EQ,NEQ,LE,LEQ,GE,GEQ
        case _EQ:{
            if(leftIterator){
                if(flb.isIncreasing()){// i from 1 to x   if i eq 5
                    current = right - left;
                }else{
                    current = left - right;
                }
            }
        }break;

        case _NEQ:{//TODO:

        }break;

        case _LE:{

        }break;

        case _LEQ:{

        }break;

        case _GE:{

        }break;

        case _GEQ:{

        }break;
    }
    //std::cerr << "success" <<current<<std::endl;

}