//
// Created by Joel Sprunger on 3/16/23.
//

#ifndef CACHE_LRUEXPIRINGCACHE_H
#define CACHE_LRUEXPIRINGCACHE_H
#include "Cache.h"
#include <ctime>
#include <stdexcept>
#include <ostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>
#include <vector>



class LruExpiringCache : public Cache
{
public:

    LruExpiringCache(int maxSize, int mageAgeSeconds)
    : Cache(maxSize, mageAgeSeconds), most_recently_used{0} {start();}

    LruExpiringCache()
    : Cache(1000000, 30), most_recently_used{0}{start();}

    virtual ~LruExpiringCache(){stop();}

    void add(string key, int value, int max_age_seconds) override;

    void remove(const string &key) override;

    int get(const string &key) override;

    bool cacheFull() const override;

    void cleanCache();

    void cacheCleaner();
    void start()
    {
        cache_on = true;
        t = thread([this]{cacheCleaner();});
    }
    void stop(){cache_on = false; t.join();}

    friend ostream &operator<<(ostream &os, const LruExpiringCache &cache);

private:
    thread t;
    int most_recently_used;
    string findLruItem() const;
    int findLowestUsage() const;
    atomic<bool> cache_on;
};

#endif //CACHE_LRUEXPIRINGCACHE_H
