#include "memc.hpp"
#include <iostream>

MemoryController::MemoryController(){
    blocks.insert(std::make_pair(0,std::make_shared<MemBlock>(MemBlock(MTYPE::AC))));//AKUMULATOR
    indexer = 1;
}

long long MemoryController::declareVar(string name, long long value){
    blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(value, MTYPE::VARIABLE))));//WE HOLD VARIABLE AT SOME INDEX
    variables.insert(std::make_pair(name,indexer));//WE HOLD NAME-INDEX PAIR
    ++indexer;
    return indexer-1;
}

long long MemoryController::declareVar(string name){
    blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(MTYPE::VARIABLE))));//WE HOLD VARIABLE AT SOME INDEX
    variables.insert(std::make_pair(name,indexer));//WE HOLD NAME-INDEX PAIR
    ++indexer;
    return indexer-1;
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

void MemoryController::setValueIn(string name, long long value){
    auto res = variables.find(name)->second;
    auto block = blocks.find(res)->second;
    block->setValue(value);
}

void MemoryController::setValueIn(long long index, long long value){
    auto block = blocks.find(index)->second;
    block->setValue(value);
}

std::shared_ptr<MemBlock> MemoryController::getBlock(string name){
    auto res = variables.find(name)->second;
    return blocks.find(res)->second;
}

std::shared_ptr<MemBlock> MemoryController::getBlock(long long index){
    return blocks.find(index)->second;
}


long long MemoryController::getValueOfVar(string name){
    auto res = variables.find(name)->second;
    auto block = blocks.find(res)->second;
    return block->getValue();
}

long long MemoryController::getValueOfIndex(long long index){
    auto block = blocks.find(index)->second;
    return block->getValue();
}

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

long long MemoryController::getIndexOfVar(string name){
    return variables.find(name)->second;
}

long long MemoryController::getFreeIndex(){
    return indexer+1;
}

void MemoryController::printAll(){

    auto beg = blocks.begin();
    const auto end = blocks.end();
    while(beg != end){
        try{
            auto val = beg->second->getValue();
            std::cout << "[DEBUG]Index:" << beg->first << "\tValue:" << val << std::endl;
        }
        catch(...){
            std::cout << "[DEBUG]Index:" << beg->first << "\tValue:" << "undef" << std::endl;
        }
        ++beg;
    }
}