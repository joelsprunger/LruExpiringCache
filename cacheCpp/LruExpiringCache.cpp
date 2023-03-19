//
// Created by Joel Sprunger on 3/16/23.
//

#include "LruExpiringCache.h"
typedef recursive_mutex mutex_t;
mutex_t mtx;

void LruExpiringCache::add(string key, int value, int max_age_seconds) {
    scoped_lock<mutex_t> lock{mtx};
    if (max_age_seconds > Cache::mage_age_seconds)
        max_age_seconds = Cache::mage_age_seconds;
    time_t current_time = time(nullptr);
    // check to see if the cache is full
    if (cacheFull())
    {
        // remove oldest item
        remove(findLruItem());
    }

    Cache::Items[key] = make_shared<Item>(key, value, current_time + max_age_seconds, 0);

}

void LruExpiringCache::remove(const string &key)
{
    scoped_lock<mutex_t> lock{mtx};
    if (Items.find(key) == Items.end()){
        return;
    }
    else
    {
        Items.erase(key);
    }
}

int LruExpiringCache::get(const string &key) {
    scoped_lock<mutex_t> lock{mtx};
    if (Items.find(key) == Items.end()){
        throw runtime_error("***ERROR***: The item does not exist in the cache");
    }
    else
    {
        most_recently_used++;
        Items[key]->usage = most_recently_used;
        return Items[key]->value;
    }
}

int LruExpiringCache::findLowestUsage() const{
    scoped_lock<mutex_t> lock{mtx};
    map<string, unique_ptr<Item>> tempItems;
    int lowest_usage{-1};
    for (auto& item : Items){
        int usage =  Items.at(item.first)->usage;
        if ((lowest_usage == -1)||(usage < lowest_usage))
        {
            lowest_usage = usage;
        }
    }
    return lowest_usage;
}

string LruExpiringCache::findLruItem() const {
    scoped_lock<mutex_t> lock{mtx};
    map<string, shared_ptr<Item>> LruItems;
    int lowest_usage = findLowestUsage();
    string LruItemKey = "";
    for (auto& item : Items){
        int usage =  Items.at(item.first)->usage;
        if (usage == lowest_usage)
        {
            LruItems[item.first] = item.second;
        }
    }
    int lowest_expiration = -1;
    for (auto& item : LruItems) {
        int expiration = LruItems.at(item.first)->expiration;
        if (lowest_expiration == -1 || expiration < lowest_expiration) {
            lowest_expiration = expiration;
            LruItemKey = item.first;
        }
    }
    return LruItemKey;
}


bool LruExpiringCache::cacheFull() const {
    scoped_lock<mutex_t> lock{mtx};
    int counter{0};
    for (auto& item : Items){
        counter++;
    }
    if (counter < max_size)
        return false;
    return true;
}

void LruExpiringCache::cleanCache(){
    scoped_lock<mutex_t> lock{mtx};
    time_t current_time = time(nullptr);
    vector<string> keys;
    for (auto& item : Items)
    {
        int expiration = item.second->expiration;
        if (expiration < current_time)
            keys.push_back(item.first);
    }
    for (auto key : keys){
        remove(key);
    }
}

void LruExpiringCache::cacheCleaner()
{
    while (cache_on)
    {
        this_thread::sleep_for(chrono::seconds(1));
        cleanCache();
    }

}

ostream &operator<<(ostream &os, const LruExpiringCache &cache) {
    scoped_lock<mutex_t> lock{mtx};
    os << static_cast<const Cache &>(cache);
    return os;
}




