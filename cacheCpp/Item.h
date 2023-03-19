//
// Created by Joel Sprunger on 3/16/23.
//

#ifndef CACHE_ITEM_H
#define CACHE_ITEM_H
#include <string>

using namespace std;

class Item {
public:
    Item(const string &key, int value, int expiration, int usage)
    : key(key), value(value), expiration(expiration), usage(usage) {}

    string key;
    int value;
    time_t expiration; // absolute time
    int usage;
};


#endif //CACHE_ITEM_H
