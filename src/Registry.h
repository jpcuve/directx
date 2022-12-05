#pragma once

#include <map>
#include <string>

struct Entry {

};

class Registry {
    std::map<std::string, Entry> entries;
public:
    void Init();
    void Done();
};
