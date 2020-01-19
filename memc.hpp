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
    Array(unsigned long long memblockstart, long long begin, long long end){
        if(begin > end){
            throw std::runtime_error("Array can't have begin lesser than end");
        }
        this->memblockstart = memblockstart;
        this->begin = begin;
        this->end = end;
        if(begin <= 0 && end >= 0){
            size = (unsigned long long)(abs(begin))+ (unsigned long long)(abs(end)) +1 ;
        }else if(begin <= 0 && end <= 0){
            size = (unsigned long long)(abs(end)) - (unsigned long long)(abs(begin)) + 1; //abs(begin - end) +1 ;
        }else if(begin >= 0 && end >= 0){
            size = (unsigned long long)(end) - (unsigned long long)(begin) +1;
        }
        //std::cerr << begin << '\t' << end << '\t' << size << '\n';
        panic = false;
    }

    long long getBegin(){
        return begin;
    }

    unsigned long long getFirstOffsetedIndex(){
        return memblockstart - begin;
    }

    unsigned long long getLastOffsetedIndex(){
        return memblockstart - begin + end;
    }

    bool hasIndex(unsigned long long index){
        if(index >= begin && index <= end){
            return true;
        }else{
            return false;
        }
    }

    unsigned long long getMemBlockIndex(long long item){
        if(item < begin || item > end){
            throw std::runtime_error("Access of Array out of bounds");
        }
        auto offset = (unsigned long long)(item - begin);
        //std::cerr << offset << '\t' << item << '\t' << size << '\n';
        if(offset >= size || offset < 0){
            throw std::runtime_error("Access of Array out of bounds");
        }

        return offset + memblockstart;
    }

    unsigned long long getSize(){
        return size;
    }

    void setupIndex(unsigned long long key, unsigned long long index){
        auto s = iterators.insert(std::make_pair(key,index));
        if(!s.second) throw std::runtime_error("Duplicate!");
    }

    unsigned long long getIndex(unsigned long long key){
        auto index = iterators.find(key);
        return index->second;
    }

    void removeIndex(unsigned long long key){
        auto index = iterators.find(key);
        iterators.erase(index);
    }

    bool panic;

private:
    unsigned long long memblockstart;
    std::map<unsigned long long, unsigned long long> iterators;//holds indexes to memory which points to element of Array

    long long begin;
    long long end;
    unsigned long long size;
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

    unsigned long long declareVar(string name, long long value);
    unsigned long long declareVar(string name);
    unsigned long long declareValue(long long value);
    unsigned long long declareArray(string name, long long begin, long long end);
    unsigned long long declareSpecial(string name);
    unsigned long long pushSimpleIterator(string name);
    ForLoopBlock pushIterator(string name, bool isSpecial);

    unsigned long long popIterator();
    unsigned long long smartGetSpecialIndex(string name);

    void setValueIn(string name, long long value);
    void setValueIn(unsigned long long index, long long value);
    void setValueIn(string name, unsigned long long index, long long value);

    std::shared_ptr<MemBlock> getBlock(string name);
    std::shared_ptr<MemBlock> getBlock(unsigned long long index);
    std::shared_ptr<MemBlock> getBlock(string name, unsigned long long index);

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
    std::vector<ForLoopBlock> iterators;//TODO: what if eg. for i .. endfor ... for i .. endfor
    unsigned long long indexer;
    unsigned long long restr = 0;
};