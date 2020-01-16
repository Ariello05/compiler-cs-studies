#include "memc.hpp"
#include <iostream>

MemoryController::MemoryController(){
    blocks.insert(std::make_pair(0,std::make_shared<MemBlock>(MemBlock(MTYPE::AC))));//AKUMULATOR
    indexer = 1;
}

// lets just don't call it
long long MemoryController::declareVar(string name, long long value){
    if(variables.find(name) != variables.end()){
        
    }
    blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(value, MTYPE::VARIABLE))));//WE HOLD VARIABLE AT SOME INDEX
    variables.insert(std::make_pair(name,indexer));//WE HOLD NAME-INDEX PAIR
    ++indexer;
    return indexer-1;
}

long long MemoryController::declareVar(string name){
    if(variables.find(name) != variables.end()){
        throw std::runtime_error("Variable: " + name + " is already defined!");
    }
    blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(MTYPE::VARIABLE,name))));//WE HOLD VARIABLE AT SOME INDEX
    variables.insert(std::make_pair(name,indexer));//WE HOLD NAME-INDEX PAIR
    ++indexer;
    return indexer-1;
}

long long MemoryController::declareArray(string name, long long begin, long long end){
    auto blockstart = indexer;
    if(begin > end){
        throw std::runtime_error("In array, end can't be less than begin");
    }
    if(arrays.find(name) != arrays.end()){
        throw std::runtime_error("Array: " + name + " is already defined!");
    }

    for(auto i = begin; i<=end; ++i){
        blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(MTYPE::ARRAY, name + "(" + std::to_string(i) + ")"))));//WE HOLD VARIABLE AT SOME INDEX
        ++indexer;
    }
    arrays.insert(std::make_pair(name,Array(blockstart,begin,end)));

    return blockstart;
}


long long MemoryController::declareValue(long long value){
    /*
    auto beg = blocks.begin();
    const auto end = blocks.end();

    while(beg != end){
        try{
            if(beg->second->getValue() == value)//VALUE ALREADY EXISTS
            {
                return beg->first;
            }
        }
        catch(...){

        }
        ++beg;
    }
    */
    blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(value, MTYPE::CONST))));//WE HOLD VARIABLE AT SOME INDEX
    ++indexer;
    return indexer-1;
}

long long MemoryController::declareSpecial(string name){
    if(specials.find(name) != specials.end()){
        throw std::runtime_error("Declaring already existing special!");
    }
    specials.insert(std::make_pair(name,indexer));
    blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(MTYPE::SPECIAL))));//WE HOLD VARIABLE AT SOME INDEX
    ++indexer;
    return indexer-1;
}

long long MemoryController::smartGetSpecialIndex(string name){
    if(specials.find(name) == specials.end()){
        return declareSpecial(name);
    }
    else{
        return getIndexOfSpecial(name);
    }
}



void MemoryController::setValueIn(string name, long long value){
    auto res = variables.find(name)->second;
    auto block = blocks.find(res)->second;
    block->setValue(value);
}

void MemoryController::setValueIn(long long index, long long value){
    auto block = blocks.find(index)->second;
    block->setValue(value);
}

void MemoryController::setValueIn(string name, long long index, long long value){
    auto arr = arrays.find(name);
    auto block = blocks.find(arr->second.getMemBlockIndex(index))->second;
    block->setValue(value);
}

std::shared_ptr<MemBlock> MemoryController::getBlock(string name){
    auto res = variables.find(name)->second;
    return blocks.find(res)->second;
}

std::shared_ptr<MemBlock> MemoryController::getBlock(long long index){
    return blocks.find(index)->second;
}

std::shared_ptr<MemBlock> MemoryController::getBlock(string name, long long index){
    auto arr = arrays.find(name);
    return blocks.find(arr->second.getMemBlockIndex(index))->second;
}

//Depracated for now
long long MemoryController::getValueOfVar(string name){
    auto res = variables.find(name)->second;
    auto block = blocks.find(res)->second;
    return block->getValue();
}
//Still usable
long long MemoryController::getValueOfIndex(long long index){
    auto block = blocks.find(index)->second;
    return block->getValue();
}
//Depracated for now
long long MemoryController::getValueOfIndexInArray(string name, long long index){
    auto res = arrays.find(name)->second;
    auto block = blocks.find(res.getMemBlockIndex(index))->second;
    return block->getValue();
}

//Depracated for now
long long MemoryController::getIndexOfValue(long long value){

    auto beg = blocks.begin();
    const auto end = blocks.end();
    while(beg != end){
        try{
            if(beg->second->getValue() == value)
            {
                return beg->first;
            }
        }
        catch(...){}
        ++beg;
    }
    return -1;
    //string err = "This value:" + std::to_string(value) + "\t is not allocated!";
    //throw std::runtime_error(err);
}

//Still usable for MTYPE==CONST
long long MemoryController::getIndexOfValue(long long value, MTYPE type){

    auto beg = blocks.begin();
    const auto end = blocks.end();
    while(beg != end){
        try{
            if(beg->second->getType() != type){
                ++beg;
                continue;
            }

            if(beg->second->getValue() == value)
            {
                return beg->first;
            }
        }
        catch(...){}
        ++beg;
    }
    return -1;
    //string err = "This value:" + std::to_string(value) + "\t is not allocated!";
    //throw std::runtime_error(err);
}

long long MemoryController::getIndexOfVar(string name){
    auto var = variables.find(name);
    if(var == variables.end()){
        throw std::runtime_error("Accesing undeclared variable: " + name);
    }
    return var->second;
}

long long MemoryController::getIndexOfArrayElement(string name, long long element){
    auto res = arrays.find(name);
    if(res == arrays.end()){
        throw std::runtime_error("Accesing undeclared array: " + name);
    }
    return res->second.getMemBlockIndex(element);
}
long long MemoryController::getIndexOfSpecial(string name){
    auto res = specials.find(name)->second;
    return blocks.find(res)->first;
}

Array MemoryController::getArray(string name){
    auto res = arrays.find(name);
    if(res == arrays.end()){
        throw std::runtime_error("Accesing undeclared array: " + name);
    }
    return res->second;
}

MTYPE MemoryController::getTypeOfIndex(long long index){
    return blocks.find(index)->second->getType();
}


long long MemoryController::getFreeIndex(){
    return indexer+1;
}

void MemoryController::printAll(){

    auto beg = blocks.begin();
    const auto end = blocks.end();
    while(beg != end){
        string name;
        switch(beg->second->getType()){
            case MTYPE::AC:
                name = "AC\t\t\t";
                break;
            case MTYPE::ARRAY:
                name = "ARRAY\t\t";
                break;
            case MTYPE::CONST:
                name = "CONST\t\t";
                break;
            case MTYPE::VARIABLE:
                name = "VARIABLE\t";
                break;
            case MTYPE::SPECIAL:
                name = "SPECIAL\t";
                break;
        }

        try{
            auto val = beg->second->getValue();
    
            std::cout << "[" << name << "| Index:" << beg->first << "\tValue:" << val << ']' << std::endl;
        }
        catch(...){
            std::cout << "[" << name << "| Index:" << beg->first << "\tValue:" << "undef" << ']' << std::endl;
        }
        ++beg;
    }
}

void MemoryController::printUndefined(){
    auto beg = blocks.begin();
    const auto end = blocks.end();
    std::vector<string> vars;
    while(beg != end){
        if(!beg->second->isDefined()){
            vars.push_back(beg->second->getName());
        }
        ++beg;
    }
    if(vars.size() >= 1){
        std::cerr << "\033[1;33m[WARNING]\033[0m Undefined variables namely:" <<std::endl;
        auto beg = vars.begin();
        const auto end = vars.end();
        while(beg != end){
            std::cerr << *beg << std::endl;
            ++beg;
        }

    }
}