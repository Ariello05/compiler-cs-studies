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

void Coder::addVars(long long pid1, long long pid2){

}

/**
 * Stores constant in memory
 * Does nothing if said value is already defined 
 * Returns address to memory
*/

/*
long long Coder::storeConstant(long long value){

    auto found = mc->getIndexOfValue(value);
    if(found <= mc->getFreeIndex()){
        mc->setValueIn(0,value);

        std::bitset<64> bits(value);//bits[0] rightmost, bits[63] leftmost
        if(value < 4 && value > 0){//SPECIAL CASE 
            auto index = mc->declareValue(value);

            vm.push_back("SUB 0");
            for(int i = 0; i < (int)value; ++i){
                vm.push_back("INC");
            }

            vm.push_back("STORE " + std::to_string(index));
            return index;
        }else{
            auto index = mc->declareValue(value);
            
            auto one = this->storeConstant(1);
            auto two = this->storeConstant(2);
            auto three = this->storeConstant(3);

            vm.push_back("SUB 0");
            int i = 63;
            while(bits[i] == 0 && i>0){
                --i;
            }
            if(i == -1){
                throw std::runtime_error("U wot m8? Why store 0");
                return 0; // AC HAS 0
            }

            
            vm.push_back("INC");

            for(; i >= 0; i-=3){
                if(i >= 3){
                    vm.push_back("SHIFT " + std::to_string(three));
                    int a = bits[i-1];
                    int b = bits[i-2];
                    int c = bits[i-3];
                    string sml = std::to_string(a) + std::to_string(b) + std::to_string(c);
                    std::bitset<3> bits(sml);
                    for(auto j = bits.to_ulong(); j > 0; --j){
                        vm.push_back("INC");
                    }
                }  
                if(i == 2){
                    vm.push_back("SHIFT " + std::to_string(two));
                    int a = bits[1];
                    int b = bits[0];
                    string sml = std::to_string(a) + std::to_string(b);
                    std::bitset<2> bits(sml);
                    for(auto j = bits.to_ulong(); j > 0; --j){
                        vm.push_back("INC");
                    }
                }    
                else if(i == 1){
                    vm.push_back("SHIFT " + std::to_string(one));
                    if(bits[0] == 1){
                        vm.push_back("INC");
                    }
                }    
                else if(i == 0){
                    //if(bits[0] == 1){
                    //    vm.push_back("INC");
                    //}
                }
            }
            vm.push_back("STORE " + std::to_string(index) );
            return index;
        }
    }
    return found;
}
*/

/**
 * Loads/defines constant to AC 
 * 
 */ 
void Coder::loadValue(long long value){
    std::bitset<64> bits(value);//bits[0] rightmost, bits[63] leftmost
    
    //if(value >= 9)
    //{
        auto index = mc->getIndexOfValue(value);
        if(index > 0){ //already defined
            vm.push_back("LOAD" + std::to_string(index));
            mc->setValueIn(0,value);
        }
        else if(index == 0){ //ac

        }
        else {
            mc->setValueIn(0,value);
            //index = mc->declareValue(value);

            auto one = mc->getIndexOfValue(1);
            auto two = mc->getIndexOfValue(2);
            auto three = mc->getIndexOfValue(3);
            
            vm.push_back("SUB 0");
            int i = 63;
            while(bits[i] == 0 && i>0){
                --i;
            }
            if(i == -1){
                return; // AC HAS 0
            }

            
            vm.push_back("INC");

            for(; i >= 0; i-=3){
                if(i >= 3){
                    vm.push_back("SHIFT " + std::to_string(three));
                    int a = bits[i-1];
                    int b = bits[i-2];
                    int c = bits[i-3];
                    string sml = std::to_string(a) + std::to_string(b) + std::to_string(c);
                    std::bitset<3> bits(sml);
                    for(auto j = bits.to_ulong(); j > 0; --j){
                        vm.push_back("INC");
                    }
                }  
                if(i == 2){
                    vm.push_back("SHIFT " + std::to_string(two));
                    int a = bits[1];
                    int b = bits[0];
                    string sml = std::to_string(a) + std::to_string(b);
                    std::bitset<2> bits(sml);
                    for(auto j = bits.to_ulong(); j > 0; --j){
                        vm.push_back("INC");
                    }
                }    
                else if(i == 1){
                    vm.push_back("SHIFT " + std::to_string(one));
                    if(bits[0] == 1){
                        vm.push_back("INC");
                    }
                }    
                else if(i == 0){
                    //if(bits[0] == 1){
                    //    vm.push_back("INC");
                    //}
                }
            }
        
        }

    //}
}

void Coder::assignValueToVar(long long id, long long value){
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