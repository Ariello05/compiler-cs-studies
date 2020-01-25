#pragma once

//-10 : 20
/** 
 * Handles accessing
 * 
*/
class Array{//Too lazy to write .cpp
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
        
        if(static_cast<long long>(index) >= begin && static_cast<long long>(index) <= end){
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
