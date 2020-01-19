#pragma once
#include <map>
#include <string>
#include <memory>
#include <exception>
#include <vector>
#include "structs.hpp"


using std::string;

//-10 : 20
/** 
 * Handles accessing
 * 
*/
class Array{
public:
    Array(long long memblockstart, long long begin, long long end){
        if(begin > end){
            throw std::runtime_error("Array can't have begin lesser than end");
        }
        this->memblockstart = memblockstart;
        this->begin = begin;
        this->end = end;
        if(begin <= 0 && end >= 0){
            size = abs(begin) + abs(end) + 1;
        }else if(begin <= 0 && end <= 0){
            size = abs(begin - end) + 1;
        }else if(begin >= 0 && end >= 0){
            size = end - begin + 1;
        }

    }

    long long getBegin(){
        return begin;
    }

    long long getFirstOffsetedIndex(){
        return memblockstart - begin;
    }

    long long getLastOffsetedIndex(){
        return memblockstart - begin + end;
    }

    long long getMemBlockIndex(long long item){
        auto offset = item - begin;
        if(offset >= size || offset < 0){
            throw std::runtime_error("Access of Array out of bounds");
        }

        return offset + memblockstart;
    }

    void setupIndex(long long key, long long index){
        auto s = iterators.insert(std::make_pair(key,index));
        if(!s.second) throw std::runtime_error("Duplicate!");
    }

    long long getIndex(long long key){
        auto index = iterators.find(key);
        return index->second;
    }

    void removeIndex(long long key){
        auto index = iterators.find(key);
        iterators.erase(index);
    }

private:
    long long memblockstart;
    std::map<long long, long long> iterators;//holds indexes to memory which points to element of Array

    long long begin;
    long long end;
    long long size;
};


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
    long long declareArray(string name, long long begin, long long end);
    long long declareSpecial(string name);
    long long pushSimpleIterator(string name);
    ForLoopBlock pushIterator(string name, bool isSpecial);

    long long popIterator();
    long long smartGetSpecialIndex(string name);

    void setValueIn(string name, long long value);
    void setValueIn(long long index, long long value);
    void setValueIn(string name, long long index, long long value);

    std::shared_ptr<MemBlock> getBlock(string name);
    std::shared_ptr<MemBlock> getBlock(long long index);
    std::shared_ptr<MemBlock> getBlock(string name, long long index);

    long long getValueOfVar(string name);
    long long getValueOfIndex(long long index);
    long long getValueOfIndexInArray(string name, long long index);

    long long getIndexOfValue(long long value);
    long long getIndexOfValue(long long value, MTYPE type);
    long long getIndexOfVar(string name);
    long long getIndexOfArrayElement(string name, long long element);
    long long getIndexOfSpecial(string name);
    long long getIndexOfCurrentIterator();
    bool isUndef(long long id);
    bool isIterator(string name);
    bool isIterator(long long index);
    Array getArray(string name);

    void clearFlagsInArray(long long start);
    void clearFlagsInArray(string name);

    MTYPE getTypeOfIndex(long long index);
    long long getFreeIndex();
    void printUndefined();
    void printAll();

    const string forward = ".AssignForward";
    const string primary = ".Local1";
    const string secondary = ".Local2";

    void freeRestrict();

private:
    //Most of the work is done in blocks which holds pairs index in memory - memblocks(VALUE)
    //variables and arrays map is only used as helper to faster identify correct indexes
    std::map<long long, std::shared_ptr<MemBlock>> blocks;
    std::map<string, long long> variables;
    std::map<string, Array> arrays;
    std::map<string, long long> specials;
    std::vector<ForLoopBlock> iterators;//TODO: what if eg. for i .. endfor ... for i .. endfor
    long long indexer;
    long long restr = 0;
};