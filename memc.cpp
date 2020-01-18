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
    blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(value, MTYPE::VAR))));//WE HOLD VARIABLE AT SOME INDEX
    variables.insert(std::make_pair(name,indexer));//WE HOLD NAME-INDEX PAIR
    ++indexer;
    return indexer-1;
}

long long MemoryController::declareVar(string name){
    if(variables.find(name) != variables.end()){
        throw std::runtime_error("Variable: " + name + " is already defined!");
    }
    blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(MTYPE::VAR,name))));//WE HOLD VARIABLE AT SOME INDEX
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

long long MemoryController::pushSimpleIterator(string name){
    auto f = pushIterator(name,false);
    
    return f.getIteratorIndex();
}

ForLoopBlock MemoryController::pushIterator(string name, bool isSpecial){
    if(!isSpecial){
        if(variables.find(name) != variables.end()){
            throw std::runtime_error("Variable: " + name + " is already defined!");
        }                                                                       //TODO: 0 flag, better way?
        blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(0,MTYPE::ITERATOR,name))));//WE HOLD VARIABLE AT SOME INDEX
        variables.insert(std::make_pair(name,indexer));//ITERATOR IS ALSO A VARIABLE
        ForLoopBlock f(name,indexer);
        iterators.push_back(f);

        ++indexer;
        return f;
    }
    else{
        if(variables.find(name) != variables.end()){
            throw std::runtime_error("Variable: " + name + " is already defined!");
        }                                                                       //TODO: 0 flag, better way?
        blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(0,MTYPE::ITERATOR,name))));//WE HOLD VARIABLE AT SOME INDEX
        variables.insert(std::make_pair(name,indexer));//ITERATOR IS ALSO A VARIABLE
        ++indexer;
        blocks.insert(std::make_pair(indexer,std::make_shared<MemBlock>(MemBlock(0,MTYPE::SPECIAL,name))));//WE HOLD VARIABLE AT SOME INDEX
        ++indexer;
        ForLoopBlock f(name,indexer-2,indexer-1);
        iterators.push_back(f);

        return f;
    }
}

//Returns index of iterator
long long MemoryController::popIterator(){
    auto forblock = iterators.back();
    auto ob = variables.find(forblock.getName());
    if(ob == variables.end()){
        throw std::runtime_error("Error in iterator???");
    }
    auto index = ob->second;
    variables.erase(forblock.getName());//TODO: mark old index as usable again
    iterators.pop_back();
    blocks.erase(index);
    if(forblock.isSpecial()){
        std::cerr<<"XD\n";
        blocks.erase(forblock.getSpecialIndex());//TODO: mark old index as usable again
    }
    //getBlock(index)->undef();
    return index;
}



void MemoryController::setValueIn(string name, long long value){
    auto res = variables.find(name);
    if(res == variables.end()){
        throw std::runtime_error("Couldn't find variable of name: " + name);
    }
    auto block = blocks.find(res->second)->second;//this can't throw unless xD
    block->setValue(value);
}

void MemoryController::setValueIn(long long index, long long value){
    //std::cerr<<index<<'\t'<<value<<'\n';
    auto block = blocks.find(index);
    if(block == blocks.end()){
        throw std::runtime_error("Can't access: " + std::to_string(index));
    }
    block->second->setValue(value);
}

void MemoryController::setValueIn(string name, long long index, long long value){
    auto arr = arrays.find(name);
    if(arr == arrays.end()){
        throw std::runtime_error("No array of such name: " + name);
    }
    auto block = blocks.find(arr->second.getMemBlockIndex(index))->second;//inside throw
    block->setValue(value);
}

std::shared_ptr<MemBlock> MemoryController::getBlock(string name){
    auto res = variables.find(name);
    if(res == variables.end()){
        throw std::runtime_error("Couldn't find variable of name: " + name);
    }
    return blocks.find(res->second)->second;
}

std::shared_ptr<MemBlock> MemoryController::getBlock(long long index){
    auto block = blocks.find(index);
    if(block == blocks.end()){
        throw std::runtime_error("Can't access: " + index);
    }
    return block->second;
}

std::shared_ptr<MemBlock> MemoryController::getBlock(string name, long long index){
    auto arr = arrays.find(name);
    return blocks.find(arr->second.getMemBlockIndex(index))->second;//inside throw
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

long long MemoryController::getIndexOfCurrentIterator(){
    auto forblock = iterators.back();
    auto index = variables.find(forblock.getName());
    if(index == variables.end()){
        throw std::runtime_error("Error in iterator???");
    }
    return index->second;
}

bool MemoryController::isUndef(long long id){
    auto block = getBlock(id);
    if(block == nullptr){
        throw std::runtime_error("No memory allocated at this id:" +std::to_string(id) );
    }
    return !block->isDefined();
}

bool MemoryController::isIterator(string name){
    auto begin = iterators.begin();
    const auto end = iterators.end();
    while(begin != end){
        if((*begin).getName() == name){
            return true;
        }
        ++begin;
    }
    return false;
}

bool MemoryController::isIterator(long long index){
    auto memblock = getBlock(index);
    if(memblock->getType() != MTYPE::ITERATOR){
        return false;
    }
    return true;
}


Array MemoryController::getArray(string name){
    auto res = arrays.find(name);
    if(res == arrays.end()){
        throw std::runtime_error("Accesing undeclared array: " + name);
    }
    return res->second;
}

void MemoryController::clearFlagsInArray(long long start){
    auto begin = arrays.begin();
    const auto end = arrays.end();
    while(begin != end){
        auto arr = begin->second;
        if(arr.getFirstOffsetedIndex() == start){
            auto last = arr.getLastOffsetedIndex();
            for(auto i = start+arr.getBegin(); i <= last; ++i){//TODO: ?
                setValueIn(i,0);
            }
            return;
        }
        ++begin;
    }
    throw std::runtime_error("Can't access array");
}

void MemoryController::clearFlagsInArray(string name){
    auto item = arrays.find(name);
    if(item == arrays.end()){
        throw std::runtime_error("Can't access array");
    }
    auto start = item->second.getFirstOffsetedIndex();
    auto last = item->second.getLastOffsetedIndex();
    for(auto i = start; i <= last; ++i){
                setValueIn(i,0);
    }
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
            case MTYPE::VAR:
                name = "VARIABLE\t";
                break;
            case MTYPE::SPECIAL:
                name = "SPECIAL\t";
                break;
            case MTYPE::ITERATOR:
                name = "ITERATOR\t";
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