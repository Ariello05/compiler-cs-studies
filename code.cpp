#include "code.hpp"
#include <bitset>
#include <iostream>

Coder::Coder(std::shared_ptr<MemoryController> mc){
    this->mc = mc;
    auto a = mc->declareValue(1);
    auto b = mc->declareValue(2);
    auto c = mc->declareValue(3);
    mc->declareSpecial(mc->special);
    vm.push_back("SUB 0");
    vm.push_back("INC");
    vm.push_back("STORE " + std::to_string(a));
    vm.push_back("INC");
    vm.push_back("STORE " + std::to_string(b));
    vm.push_back("INC");
    vm.push_back("STORE " + std::to_string(c));
}


/* DECLARATION BLOCK */

void Coder::declareArray(string name, long long begin, long long end){
    mc->declareArray(name,begin,end);
}


void Coder::declareVariable(string name){
    mc->declareVar(name);
}

/* EXPRESSION BLOCK */
void Coder::addVars(){
    if(args.size() > 2){
        std::runtime_error("Stack shouldn't have more than 1 value here");
    }
    auto second = args.top(); args.pop();
    auto first = args.top(); args.pop();

    //TODO: Serch variables for samealike const values?
    switch(first.type){
        case CONSTVALUE:{
            switch(second.type){
                case CONSTVALUE:{// 1 + 1
                    auto id = mc->declareValue(first.value);
                    defineValue(first.value);
                    vm.push_back("STORE " + std::to_string(id));
                    //mc->declareValue(second.value); TODO: we actully might want to declare space for it
                    defineValue(second.value);
                    vm.push_back("ADD " + std::to_string(id));
                }
                break;

                case VARIABLE:{// 1 + a
                    defineValue(first.value);
                    vm.push_back("ADD " + std::to_string(second.variableIndex));
                }
                break;

                case ARRAYVAR:{// 1 + t(a)
                }
                break;
            }
        }
        break;

        case VARIABLE:{
            switch(second.type){
                case CONSTVALUE:// a + 1
                    defineValue(second.value);
                    vm.push_back("ADD " + std::to_string(first.variableIndex));
                    break;
                case VARIABLE:// a + b
                    vm.push_back("LOAD " + std::to_string(first.variableIndex));
                    vm.push_back("ADD " + std::to_string(second.variableIndex));
                    break;
                case ARRAYVAR:// a + t(a)
                    break;
            }
        }
        break;

        case ARRAYVAR:{
            switch(second.type){
                case CONSTVALUE:// t(a) + 1
                    break;
                case VARIABLE:// t(a) + b
                    break;
                case ARRAYVAR:// t(a) + t(b)
                    break;
            }
        }
        break;
    }
}

void Coder::subVars(){
    if(args.size() > 2){
        std::runtime_error("Stack shouldn't have more than 1 value here");
    }
    auto second = args.top(); args.pop();//right
    auto first = args.top(); args.pop();//left

    //TODO: Serch variables for samealike const values?
    /*//TODO: COPY ADDING
    if(!first.isVariable && second.isVariable){
        if(first.value == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            vm.push_back("SUB " + std::to_string(second.value));
            return;
        }

        defineValue(first.value);
        vm.push_back("SUB " + std::to_string(second.value));

    }else if(first.isVariable && !second.isVariable){
        if(second.value == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            auto id = mc->declareValue(second.value);
            vm.push_back("STORE " + std::to_string(id));
            vm.push_back("LOAD " + std::to_string(first.value));
            vm.push_back("SUB " + std::to_string(id));
            return;
        }

        auto id = mc->declareValue(second.value);
        defineValue(second.value);
        vm.push_back("STORE " + std::to_string(id));
        vm.push_back("LOAD " + std::to_string(first.value));
        vm.push_back("SUB " + std::to_string(id));

    }else if(first.isVariable && second.isVariable){
        if(mc->getValueOfIndex(first.value) == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            vm.push_back("SUB " + std::to_string(second.value));
            return;
        }
        //else if(mc->getValueOfIndex(second.value) == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
        //    vm.push_back("SUB " + std::to_string(first.value));
        //    return;
        //}
        else{
            vm.push_back("LOAD " + std::to_string(first.value));
            vm.push_back("SUB " + std::to_string(second.value));
            return;
        }

    }else{
        if(second.value == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            auto id = mc->declareValue(second.value);
            vm.push_back("STORE " + std::to_string(id));
            defineValue(first.value);
            vm.push_back("SUB " + std::to_string(id));
            return;   
        }
        else{
            auto id = mc->declareValue(second.value);
            defineValue(second.value);
            vm.push_back("STORE " + std::to_string(id));
            //mc->declareValue(second.value); TODO: we actully might want to declare space for it
            defineValue(first.value);
            vm.push_back("SUB " + std::to_string(id));
            return;
        }

    }
    */
}

/* CONDITION BLOCK*/
void Coder::processEQ(){
    subVars();//after this value is in AC
    auto before = vm.size();
    vm.push_back("JZERO " + std::to_string(before+2));
    vm.push_back("JUMP ");
    jumps.push(vm.size()-1);//we say that this needs to be updated later
}

void Coder::processNEQ(){
    subVars();//after this value is in AC
    vm.push_back("JZERO ");
    auto before = vm.size();
    jumps.push(before-1);//we say that this needs to be updated later
}

void Coder::processLE(){
    subVars();//after this value is in AC
    auto before = vm.size();
    vm.push_back("JEG " + std::to_string(before+2));
    vm.push_back("JUMP ");
    jumps.push(vm.size()-1);//we say that this needs to be updated later
}

void Coder::processGE(){
    subVars();//after this value is in AC
    auto before = vm.size();
    vm.push_back("JPOS " + std::to_string(before+2));
    vm.push_back("JUMP ");
    jumps.push(vm.size()-1);//we say that this needs to be updated later
}

void Coder::processLEQ(){
    subVars();//after this value is in AC
    vm.push_back("JPOS ");
    auto before = vm.size();
    jumps.push(before-1);//we say that this needs to be updated later
}

void Coder::processGEQ(){
    subVars();//after this value is in AC
    vm.push_back("JNEG ");
    auto before = vm.size();
    jumps.push(before-1);//we say that this needs to be updated later
}

/**
 * Loads/defines value/variable to AC 
 * Uses stack
 */ 
void Coder::getValue(){
    if(args.size() > 1){
        std::runtime_error("Stack shouldn't have more than 1 value here");
    }
    SmartBlock item = args.top();
    args.pop(); 

    switch(item.type){
        case CONSTVALUE:{
            defineValue(item.value);
        }break;

        case VARIABLE:{
            vm.push_back("LOAD " + std::to_string(item.variableIndex));
            //auto value = mc->getValueOfIndex(item.variableIndex);//TODO: check if value is defined
            //mc->setValueIn(item.variableIndex,0);
        }break;

        case ARRAYVAR:{
            loadArrayWithVariable(item.arrayStartIndex,item.variableIndex);
        }break;
    }
      
}

/*
void Coder::accessArrayWithVariable(string variable){
    auto addressBegin = mc->getIndexOfArrayElement(arrayLocal,0);
    auto arrayIndex = mc->declareValue(addressBegin);
    defineValue(addressBegin);
    vm.push_back("STORE " + std::to_string(arrayIndex));
    auto varIndex = mc->getIndexOfVar(variable);
    vm.push_back("LOAD " + std::to_string(varIndex));
}*/


/*
long long Coder::loadIdentifier(long long pid){
    vm.push_back("LOAD " + std::to_string(pid));
    return mc->getValueOfIndex(pid);
}*/

void Coder::stackValue(long long value){
    args.push(SmartBlock(IDENTIFIER::CONSTVALUE,value));
}

void Coder::stackVariable(string var){
    auto index = mc->getIndexOfVar(var);
    args.push(SmartBlock(IDENTIFIER::VARIABLE,0,index));//TODO: value
}

void Coder::stackArrayWithConst(long long value, string array){
    auto index = mc->getIndexOfArrayElement(array, value);
    args.push(SmartBlock(IDENTIFIER::VARIABLE,0,index));//TODO: value
}

void Coder::stackArrayWithVariable(string var, string array){
    auto indexVar = mc->getIndexOfVar(var);
    auto indexArray = mc->getArray(array).getFirstOffsetedIndex();
    args.push(SmartBlock(IDENTIFIER::ARRAYVAR,0,indexVar,indexArray));//TODO: value
}


/* COMMAND BLOCK */
void Coder::assignValueToVar(long long injectPoint){//value is in AC    
    

    if(args.size() != 1){
        throw std::runtime_error("Invalid stack of args in assign");
    }
    auto block = args.top(); args.pop();

    switch(block.type){
        case ARRAYVAR:{
            std::vector<string> injectString;
            auto arrayIndex = block.arrayStartIndex;
            auto varIndex = block.variableIndex;
            defineValue(injectString, arrayIndex);
            injectString.push_back("ADD " + std::to_string(varIndex));
            auto indexToStore = mc->getIndexOfSpecial(mc->special);
            mc->setValueIn(indexToStore,0);
            mc->clearFlagsInArray(arrayIndex);
            injectString.push_back("STORE " + std::to_string(indexToStore));//STORE into special
            vm.push_back("STOREI " + std::to_string(indexToStore));//STORE at wherever it points
            mc->setValueIn(0, 0);//update AC
            vm.insert(vm.begin()+injectPoint,injectString.begin(),injectString.end());
        }
        break;

        case CONSTVALUE:{
            throw std::runtime_error("Can't assign value to constant!");
        }
        break;

        case VARIABLE:{
            auto id = block.variableIndex;
            if(mc->isIterator(id)){
                throw std::runtime_error("Can't modify value of iterator");
            }
            mc->setValueIn(0, 0);//update AC
            mc->setValueIn(id, 0);//update variable of index id
            vm.push_back("STORE " + std::to_string(id));//store to variable of inddex id
        }
        break;
    }
}

void Coder::endif(){
    if(jumps.size() <= 0){
        throw std::runtime_error("Jump stack is empty!");
    }
    auto lastjump = jumps.top();
    jumps.pop();
    auto current = vm.size();
    vm[lastjump] += std::to_string(current);
}


void Coder::startelse(){
    if(jumps.size() <= 0){
        throw std::runtime_error("Jump stack is empty!");
    }
    auto lastjump = jumps.top();
    jumps.pop();
    auto current = vm.size();
    vm[lastjump] += std::to_string(current+1);

    auto currentE = vm.end();
    vm.insert(currentE,"JUMP ");
    jumps.push(vm.size()-1);
}

/**
 * Used mainly before condition in while loop 
*/ 
void Coder::stackJump(){
    
    jumps.push(vm.size()-1);//Stack jump
}

void Coder::endWhile(long long start){
    if(jumps.size() != 1){
        throw std::runtime_error("Jump stack should be of size 1!");   
    }
    auto current = vm.size();
    auto ifjump = jumps.top();
    jumps.pop();
    vm[ifjump] += std::to_string(current+1);
    vm.push_back("JUMP " + std::to_string(start));
}

void Coder::endDoWhile(){//TODO: fix this abomination
    if(jumps.size() != 2){
        throw std::runtime_error("Jump stack should be of size 2!");   
    }
    auto ifjump = jumps.top();
    jumps.pop();
    auto beforeCommand = jumps.top(); jumps.pop();

    vm.push_back("JUMP " + std::to_string(beforeCommand+1));
    vm[ifjump] += std::to_string(vm.size());
}

void Coder::handleToFor(string iterator){
    if(args.size() != 2){
        throw std::runtime_error("Args stack should be of size 2!");   
    }
    auto second = args.top();
    args.pop();
    auto first = args.top(); args.pop();

    switch(first.type){
        case CONSTVALUE:{
            switch(second.type){
                case CONSTVALUE:{// 1 + 1
                    if(first.value > second.value){
                        throw std::runtime_error("In FOR TO: Left variable can't be bigger than right");
                    }
                    auto iteratorIndex = mc->pushIterator(iterator);
                    auto endValueIndex = mc->declareValue(second.value);
                    defineValue(second.value);
                    vm.push_back("STORE " + std::to_string(endValueIndex));
                    defineValue(first.value);
                    vm.push_back("STORE " +std::to_string(iteratorIndex));
                    jumps.push(vm.size());//BEFORE COMPARISON
                    //vm.push_back("LOAD " +std::to_string(iteratorIndex) );
                    vm.push_back("SUB " + std::to_string(endValueIndex));
                    jumps.push(vm.size());//WHERE TO END?
                    vm.push_back("JPOS ");    //TODO: STACK FOR JUMP INDEXER   
                }
                break;

                case VARIABLE:{// 1 + a

                }
                break;

                case ARRAYVAR:{// 1 + t(a)
                }
                break;
            }
        }
        break;

        case VARIABLE:{
            switch(second.type){
                case CONSTVALUE:// a + 1
                    break;
                case VARIABLE:// a + b
                    break;
                case ARRAYVAR:// a + t(a)
                    break;
            }
        }
        break;

        case ARRAYVAR:{
            switch(second.type){
                case CONSTVALUE:// t(a) + 1
                    break;
                case VARIABLE:// t(a) + b
                    break;
                case ARRAYVAR:// t(a) + t(b)
                    break;
            }
        }
        break;
    }

}

void Coder::handleDownToFor(string iterator){
 if(args.size() != 2){
        throw std::runtime_error("Args stack should be of size 2!");   
    }
    auto second = args.top();
    args.pop();
    auto first = args.top(); args.pop();

    switch(first.type){
        case CONSTVALUE:{
            switch(second.type){
                case CONSTVALUE:{// 1 + 1
                    if(first.value < second.value){
                        throw std::runtime_error("In FOR TO: Right variable can't be bigger than left");
                    }
                    auto iteratorIndex = mc->pushIterator(iterator);
                    auto endValueIndex = mc->declareValue(second.value);
                    defineValue(second.value);
                    vm.push_back("STORE " + std::to_string(endValueIndex));
                    defineValue(first.value);
                    vm.push_back("STORE " +std::to_string(iteratorIndex));
                    jumps.push(vm.size());//BEFORE COMPARISON
                    //vm.push_back("LOAD " +std::to_string(iteratorIndex) );
                    vm.push_back("SUB " + std::to_string(endValueIndex));
                    jumps.push(vm.size());//WHERE TO END?
                    vm.push_back("JNEG ");    //TODO: STACK FOR JUMP INDEXER   
                }
                break;

                case VARIABLE:{// 1 + a

                }
                break;

                case ARRAYVAR:{// 1 + t(a)
                }
                break;
            }
        }
        break;

        case VARIABLE:{
            switch(second.type){
                case CONSTVALUE:// a + 1
                    break;
                case VARIABLE:// a + b
                    break;
                case ARRAYVAR:// a + t(a)
                    break;
            }
        }
        break;

        case ARRAYVAR:{
            switch(second.type){
                case CONSTVALUE:// t(a) + 1
                    break;
                case VARIABLE:// t(a) + b
                    break;
                case ARRAYVAR:// t(a) + t(b)
                    break;
            }
        }
        break;
    }
}

void Coder::endFor(bool dec){
    auto indexToChange = jumps.top(); jumps.pop();
    auto comparisonJump = jumps.top(); jumps.pop();
    auto iterator = mc->popIterator();
    vm.push_back("LOAD " + std::to_string(iterator));
    if(dec) vm.push_back("DEC");
    else vm.push_back("INC");
    vm.push_back("STORE " + std::to_string(iterator));
    vm.push_back("JUMP " + std::to_string(comparisonJump));
    auto endjump = vm.size();
    vm[indexToChange] += std::to_string(endjump);
}

void Coder::read(){
    if(args.size() != 1){
        throw std::runtime_error("Invalid stack size in read");
    }
    auto item = args.top(); args.pop();

    switch(item.type){
        case CONSTVALUE:
            throw std::runtime_error("Can't read into num");
        break;

        case VARIABLE:{
            vm.push_back("GET");
            vm.push_back("STORE " + std::to_string(item.variableIndex));//TODO: Optimize somehow
            mc->setValueIn(item.variableIndex,0);//to clear undef flag
            mc->setValueIn(0,0);//to clear undef flag
        }break;

        case ARRAYVAR:{
            loadArrayWithVariable(item.arrayStartIndex, item.variableIndex);
            auto indexToStore = mc->getIndexOfSpecial(mc->special);
            mc->clearFlagsInArray(item.arrayStartIndex);
            vm.push_back("STORE " + std::to_string(indexToStore));
            vm.push_back("GET");
            vm.push_back("STOREI " + std::to_string(indexToStore));
        }break;
    }
}

void Coder::write(){
    if(args.size() != 1){
        std::runtime_error("Stack shouldn't have more than 1 value here");
    }
    auto item = args.top(); args.pop();//right

    switch(item.type){
        case CONSTVALUE:{
            defineValue(item.value);
        }break;

        case VARIABLE:{
            vm.push_back("LOAD " + std::to_string(item.variableIndex));
        }break;

        case ARRAYVAR:{
            loadArrayWithVariable(item.arrayStartIndex,item.variableIndex);
        }break;
    }
    vm.push_back("PUT");
}


void Coder::printVM(){
    auto beg = vm.begin();
    auto end = vm.end();
    //std::cout << "[VM:]" << std::endl;
    while(beg != end){
        std::cout<< *beg << std::endl;
        beg++;
    }
}

void Coder::end(){
    vm.push_back("HALT");
}

long long Coder::getCurrentPosition(){
    return vm.size();
}

//Load value of array at variable
void Coder::loadArrayWithVariable(long long arrayIndex, long long varIndex){
    defineValue(arrayIndex);
    vm.push_back("ADD " + std::to_string(varIndex));
    vm.push_back("LOADI " + std::to_string(0));
}

void Coder::defineValue(long long value){
    mc->setValueIn(0,value);
    auto one = mc->getIndexOfValue(1,CONST);
    auto two = mc->getIndexOfValue(2,CONST);
    auto three = mc->getIndexOfValue(3,CONST);

    bool negative = 0;
    if(value < 0){
        negative = 1;
        value *= -1;
    }
    
    std::bitset<64> bits(value);//bits[0] rightmost, bits[63] leftmost
    
    //TODO: Optimize this definition
    vm.push_back("SUB 0");
    int i = 63;
    while(bits[i] == 0 && i>0){
        --i;
    }
    if(i == -1){
        return; // AC HAS 0
    }

    if(negative) vm.push_back("DEC");
    else vm.push_back("INC");
    for(; i >= 0; i-=3){
        if(i >= 3){
            vm.push_back("SHIFT " + std::to_string(three));
            int a = bits[i-1];
            int b = bits[i-2];
            int c = bits[i-3];
            string sml = std::to_string(a) + std::to_string(b) + std::to_string(c);
            std::bitset<3> bits(sml);
            for(auto j = bits.to_ulong(); j > 0; --j){
                if(negative) vm.push_back("DEC");
                else vm.push_back("INC");
            }
        }  
        if(i == 2){
            vm.push_back("SHIFT " + std::to_string(two));
            int a = bits[1];
            int b = bits[0];
            string sml = std::to_string(a) + std::to_string(b);
            std::bitset<2> bits(sml);
            for(auto j = bits.to_ulong(); j > 0; --j){
                if(negative) vm.push_back("DEC");
                else vm.push_back("INC");
            }
        }    
        else if(i == 1){
            vm.push_back("SHIFT " + std::to_string(one));
            if(bits[0] == 1){
                if(negative) vm.push_back("DEC");
                else vm.push_back("INC");
            }
        }    
        else if(i == 0){
        }
    }    
}

void Coder::defineValue(std::vector<string> & storeCode, long long value){
    mc->setValueIn(0,0);
    auto one = mc->getIndexOfValue(1,CONST);
    auto two = mc->getIndexOfValue(2,CONST);
    auto three = mc->getIndexOfValue(3,CONST);

    bool negative = 0;
    if(value < 0){
        negative = 1;
        value *= -1;
    }
    
    std::bitset<64> bits(value);//bits[0] rightmost, bits[63] leftmost
    
    //TODO: Optimize this definition
    storeCode.push_back("SUB 0");
    int i = 63;
    while(bits[i] == 0 && i>0){
        --i;
    }
    if(i == -1){
        return; // AC HAS 0
    }

    if(negative) storeCode.push_back("DEC");
    else storeCode.push_back("INC");
    for(; i >= 0; i-=3){
        if(i >= 3){
            storeCode.push_back("SHIFT " + std::to_string(three));
            int a = bits[i-1];
            int b = bits[i-2];
            int c = bits[i-3];
            string sml = std::to_string(a) + std::to_string(b) + std::to_string(c);
            std::bitset<3> bits(sml);
            for(auto j = bits.to_ulong(); j > 0; --j){
                if(negative) storeCode.push_back("DEC");
                else storeCode.push_back("INC");
            }
        }  
        if(i == 2){
            storeCode.push_back("SHIFT " + std::to_string(two));
            int a = bits[1];
            int b = bits[0];
            string sml = std::to_string(a) + std::to_string(b);
            std::bitset<2> bits(sml);
            for(auto j = bits.to_ulong(); j > 0; --j){
                if(negative) storeCode.push_back("DEC");
                else storeCode.push_back("INC");
            }
        }    
        else if(i == 1){
            storeCode.push_back("SHIFT " + std::to_string(one));
            if(bits[0] == 1){
                if(negative) storeCode.push_back("DEC");
                else storeCode.push_back("INC");
            }
        }    
        else if(i == 0){
        }
    }    
}