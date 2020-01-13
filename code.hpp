#pragma once
#include <string>
#include <vector>
#include <memory>
#include "memc.hpp"

using std::string;
namespace machine{
    static const string ADD = "ADD ";
    static const string LOAD = "LOAD ";
    static const string SHIFT = "SHIFT ";

}

class Coder{
public:
    Coder(std::shared_ptr<MemoryController> mc);
    void addVars(long long pid1, long long pid2);
    void loadValue(long long value);
    //long long storeConstant(long long value);
    void printVM();
    void assignValueToVar(long long id, long long value);
    //void declareVar(long long pid);

private:
    std::vector<string> vm;
    std::shared_ptr<MemoryController> mc;
};