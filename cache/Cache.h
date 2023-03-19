//
// Created by Joel Sprunger on 3/16/23.
//

#ifndef CACHE_CACHE_H
#define CACHE_CACHE_H
#include "Item.h"
#include <map>
#include <memory>
#include <ostream>

class Cache{
public:
    Cache(int maxSize, int mageAgeSeconds) : max_size(maxSize), mage_age_seconds(mageAgeSeconds) {}
    virtual void add(string key, int value, int max_age_seconds) = 0;
    virtual void remove(const string& key) = 0;
    virtual int get(const string& key) = 0;
    virtual bool cacheFull() const = 0;
    friend ostream &operator<<(ostream &os, const Cache &cache) {
        os << "Cache Items:" << endl;
        for (auto item : cache.Items){
            os << "\t" << item.first << ", " << cache.Items.at(item.first)->value
               << ", " << cache.Items.at(item.first)->usage
               << ", " << cache.Items.at(item.first)->expiration << endl;
        }
        return os;
    }

protected:
    map<string, shared_ptr<Item>> Items;
    int max_size;
    int mage_age_seconds;

};



#endif //CACHE_CACHE_H
