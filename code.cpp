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

    if(!first.isVariable && second.isVariable){
        if(first.value == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            vm.push_back("ADD " + std::to_string(second.value));
            return;
        }

        defineValue(first.value);
        vm.push_back("ADD " + std::to_string(second.value));

    }else if(first.isVariable && !second.isVariable){
        if(second.value == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            vm.push_back("ADD " + std::to_string(second.value));
            return;
        }

        defineValue(second.value);
        vm.push_back("ADD " + std::to_string(first.value));

    }else if(first.isVariable && second.isVariable){
        if(mc->getValueOfIndex(first.value) == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            vm.push_back("ADD " + std::to_string(second.value));
            return;
        }
        else if(mc->getValueOfIndex(second.value) == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            vm.push_back("ADD " + std::to_string(first.value));
            return;
        }
        else{
            vm.push_back("LOAD " + std::to_string(first.value));
            vm.push_back("ADD " + std::to_string(second.value));
            return;
        }

    }else{
        if(first.value == mc->getValueOfIndex(0)){//TODO: what if AC is overflown
            auto id = mc->declareValue(first.value);
            vm.push_back("STORE " + std::to_string(id));
            defineValue(second.value);
            vm.push_back("ADD " + std::to_string(id));
            return;
            
        }else if(second.value == mc->getValueOfIndex(0)){
            auto id = mc->declareValue(second.value);
            vm.push_back("STORE " + std::to_string(id));
            defineValue(first.value);
            vm.push_back("ADD " + std::to_string(id));
            return;
        }else{
            auto id = mc->declareValue(first.value);
            defineValue(first.value);
            vm.push_back("STORE " + std::to_string(id));
            //mc->declareValue(second.value); TODO: we actully might want to declare space for it
            defineValue(second.value);
            vm.push_back("ADD " + std::to_string(id));
            return;
        }

    }
}

void Coder::subVars(){
    if(args.size() > 2){
        std::runtime_error("Stack shouldn't have more than 1 value here");
    }
    auto second = args.top(); args.pop();//right
    auto first = args.top(); args.pop();//left

    //TODO: Serch variables for samealike const values?

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

    if(item.isVariable){//last item was variable so just LOAD
        if(mc->getValueOfIndex(0) == mc->getValueOfIndex(item.value)){//Special case when value == AC
            return;
        }
        vm.push_back("LOAD " + std::to_string(item.value));
        auto value = mc->getValueOfIndex(item.value);
        mc->setValueIn(0,value);
    }
    else{
        auto value = item.value;

        std::bitset<64> bits(value);//bits[0] rightmost, bits[63] leftmost
        auto index = mc->getIndexOfValue(value,MTYPE::CONST);
        if(index > 0){ //already defined TODO: What if overflow?
            if(value < 7 && value > -7){
                defineValue(value);
                mc->setValueIn(0,value);
                return;
            }
            vm.push_back("LOAD " + std::to_string(index));
            mc->setValueIn(0,value);
        }
        else if(index == 0){//Special case when value == AC
            return;
        }
        else {
            //auto id = mc->declareValue(value);
            defineValue(value);
        }
    }
    clearStack();//JUST IN CASE, TODO: Are you sure?
}

void Coder::stackVariable(long long index){
    if(arrayLocal != ""){
        args.push(SmartBlock(true,index,arrayLocal));//arrayItem

    }else{
        args.push(SmartBlock(true,index));
    }

    arrayLocal = "";
}

void Coder::stackValue(long long value){
    args.push(SmartBlock(false,value));
}

void Coder::clearStack(){
    while(!args.empty())   
        args.pop();
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

void Coder::setArrayToAccess(string array){
    arrayLocal = array;
}



long long Coder::loadIdentifier(long long pid){
    vm.push_back("LOAD " + std::to_string(pid));
    return mc->getValueOfIndex(pid);
}

/* COMMAND BLOCK */
void Coder::assignValueToVar(long long id, long long value){//value is in AC
    if(arrayLocal != ""){//ACCESSING ARRAY BY VARIABLE
        auto local = mc->smartGetSpecialIndex(".Local");
        vm.push_back("STORE " + std::to_string(local));
        auto arr = mc->getArray(arrayLocal);
        auto first = arr.getFirstOffsetedIndex();
        //auto index = mc->declareValue(first);
        defineValue(first);
        //vm.push_back("STORE " + std::to_string(index));
        //vm.push_back("LOAD " + std::to_string(pid));
        vm.push_back("ADD " + std::to_string(id));
        
        auto indexToStore = mc->getIndexOfSpecial(mc->special);
        
        mc->setValueIn(indexToStore,first+mc->getValueOfIndex(id));//TODO:COMMENT IF MORE THAN 64 BIT or use some flag w/e
        vm.push_back("STORE " + std::to_string(indexToStore));
        vm.push_back("LOAD " + std::to_string(local) );
        vm.push_back("STOREI " + std::to_string(indexToStore));
        
        mc->setValueIn(0, value);
    }else{
        mc->setValueIn(0, value);
        mc->setValueIn(id, value);
        vm.push_back("STORE " + std::to_string(id));
    }
    arrayLocal = "";
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

void Coder::read(long long pid){
    if(arrayLocal != ""){//ACCESSING ARRAY BY VARIABLE
        //pid variable
        //name array
        auto arr = mc->getArray(arrayLocal);
        auto first = arr.getFirstOffsetedIndex();
        //auto index = mc->declareValue(first);
        defineValue(first);
        //vm.push_back("STORE " + std::to_string(index));
        //vm.push_back("LOAD " + std::to_string(pid));
        vm.push_back("ADD " + std::to_string(pid));
        auto indexToStore = mc->getIndexOfSpecial(mc->special);
        mc->setValueIn(indexToStore,first+mc->getValueOfIndex(pid));//TODO:COMMENT IF MORE THAN 64 BIT or use some flag w/e
        vm.push_back("STORE " + std::to_string(indexToStore));
        vm.push_back("GET");
        vm.push_back("STOREI " + std::to_string(indexToStore));
    }
    else{
        vm.push_back("GET");
        vm.push_back("STORE " + std::to_string(pid));//TODO: Optimize somehow
        mc->setValueIn(pid,1);//to clear undef flag
    }

    mc->setValueIn(0,1);//to clear undef flag
    arrayLocal = "";
}

void Coder::write(){
    if(args.size() != 1){
        std::runtime_error("Stack shouldn't have more than 1 value here");
    }
    auto arg = args.top(); args.pop();//right
    if(arg.isVariable){
        vm.push_back("LOAD " + std::to_string(arg.value));
    }
    else{
        defineValue(arg.value);//TODO: optimize

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