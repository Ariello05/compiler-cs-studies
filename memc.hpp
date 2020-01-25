#pragma once
#include <map>
#include <string>
#include <memory>
#include <exception>
#include <vector>
#include "structs.hpp"
#include "array.hpp"


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

    unsigned long long declareVar(string name, long long value);
    unsigned long long declareVar(string name);
    unsigned long long declareValue(long long value);
    unsigned long long declareArray(string name, long long begin, long long end);
    unsigned long long declareSpecial(string name);
    ForLoopBlock pushSimpleIterator(string name, bool for_to);
    ForLoopBlock pushIterator(string name, bool isSpecial, bool for_to);

    unsigned long long popIterator();
    unsigned long long smartGetSpecialIndex(string name);

    void setValueIn(string name, long long value);
    void setValueIn(unsigned long long index, long long value);
    void setValueIn(string name, unsigned long long index, long long value);

    std::shared_ptr<MemBlock> getBlock(string name);
    std::shared_ptr<MemBlock> getBlock(unsigned long long index);
    std::shared_ptr<MemBlock> getBlock(string name, unsigned long long index);

    ForLoopBlock getIterator(string name);
    ForLoopBlock getLastIterator();

    long long getValueOfVar(string name);
    long long getValueOfIndex(unsigned long long index);
    long long getValueOfIndexInArray(string name, unsigned long long index);

    unsigned long long getIndexOfValue(long long value);
    unsigned long long getIndexOfValue(long long value, MTYPE type);
    unsigned long long getIndexOfVar(string name);
    unsigned long long getIndexOfArrayElement(string name, unsigned long long element);
    unsigned long long getIndexOfSpecial(string name);
    unsigned long long getIndexOfCurrentIterator();
    bool isUndef(unsigned long long id);
    bool isIterator(string name);
    bool isIterator(unsigned long long index);
    Array & getArray(string name);

    void clearFlagsInArray(unsigned long long start);
    void clearFlagsInArray(string name);

    MTYPE getTypeOfIndex(unsigned long long index);
    unsigned long long getFreeIndex();
    void printUndefined();
    void printAll();

    const string forward = ".AssignForward";
    const string primary = ".Local1";
    const string secondary = ".Local2";

    void freeRestrict();

private:
    //Most of the work is done in blocks which holds pairs index in memory - memblocks(VALUE)
    //variables and arrays map is only used as helper to faster identify correct indexes
    std::map<unsigned long long, std::shared_ptr<MemBlock>> blocks;
    std::map<string, unsigned long long> variables;
    std::map<string, Array> arrays;
    std::map<string, unsigned long long> specials;
    std::vector<ForLoopBlock> iterators;
    unsigned long long indexer;
    unsigned long long restr = 0;
};