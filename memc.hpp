#pragma once
#include <map>
#include <string>
#include <memory>
#include <exception>
#include "structs.hpp"


using std::string;

/**
 * Class controlls values of the memory blocks in virtual machine  
 * Accessing:
 * index            ->          const
 * string           ->          variable
 * string,index     ->          array
*/
class MemoryController{
public:
    MemoryController();

    long long declareVar(string name, long long value);
    long long declareVar(string name);
    long long declareValue(long long value);

    void setValueIn(string name, long long value);
    void setValueIn(long long index, long long value);

    std::shared_ptr<MemBlock> getBlock(string name);
    std::shared_ptr<MemBlock> getBlock(long long index);

    long long getValueOfVar(string name);
    long long getValueOfIndex(long long index);

    long long getIndexOfValue(long long value);
    long long getIndexOfVar(string name);

    long long getFreeIndex();

    void printAll();


private:

    std::map<long long, std::shared_ptr<MemBlock>> blocks;
    std::map<string, long long> variables;
    long long indexer;
};