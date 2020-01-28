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
    ForLoopBlock pushSimpleIterator(string name, bool for_to);//simplified iterator
    ForLoopBlock pushIterator(string name, bool isSpecial, bool for_to);

    unsigned long long popIterator();
    unsigned long long smartGetSpecialIndex(string name);//gets index of special value, or createss new index if special value wasn't declared

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

    void clearFlagsInArray(unsigned long long start);//puts panic mode
    void clearFlagsInArray(string name);//puts panic mode

    MTYPE getTypeOfIndex(unsigned long long index);
    unsigned long long getFreeIndex();
    void printUndefined();
    void printAll();

    const string forward = ".AssignForward";
    const string primary = ".Local1";
    const string secondary = ".Local2";

    unsigned long long getRestrict();
    void freeRestrict();//helps with the problem of accessing yet undefined constants in certain cases

private:
    //Most of the work is done in blocks which holds pairs index in memory - memblocks(VALUE)
    //variables and arrays map is only used as helper to faster identify correct indexes
    std::map<unsigned long long, std::shared_ptr<MemBlock>> blocks;//memblocks mapped by index
    std::map<string, unsigned long long> variables;//variables, pair of name and index to memblock
    std::map<string, Array> arrays;//holds arrayinfo
    std::map<string, unsigned long long> specials;//specials are semi-const variables used in mul/div/mod or some other special cases
    std::vector<ForLoopBlock> iterators;//forloopblock helper class
    unsigned long long indexer;
    unsigned long long restr = 0;
};