#include "code.hpp"
#include <bitset>
#include <iostream>

Coder::Coder(std::shared_ptr<MemoryController> mc){
    this->mc = mc;
    auto a = mc->declareValue(1);
    auto b = mc->declareValue(2);
    auto c = mc->declareValue(3);
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

    std::cout << first.value << "\t" << second.value << std::endl;

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
        auto index = mc->getIndexOfValue(value);
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
    args.push(SmartBlock(true,index));
}

void Coder::stackValue(long long value){
    args.push(SmartBlock(false,value));
}

void Coder::clearStack(){
    while(!args.empty())   
        args.pop();
}


long long Coder::loadIdentifier(long long pid){
    vm.push_back("LOAD " + std::to_string(pid));
    return mc->getValueOfIndex(pid);
}


void Coder::assignValueToVar(long long id, long long value){//value is in AC
    mc->setValueIn(0, value);
    mc->setValueIn(id, value);
    vm.push_back("STORE " + std::to_string(id));
}


void Coder::printVM(){
    auto beg = vm.begin();
    auto end = vm.end();
    std::cout << "[VM]:" << std::endl;
    while(beg != end){
        std::cout<< *beg << std::endl;
        beg++;
    }
}

void Coder::defineValue(long long value){
    mc->setValueIn(0,value);
    auto one = mc->getIndexOfValue(1);
    auto two = mc->getIndexOfValue(2);
    auto three = mc->getIndexOfValue(3);

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