#include <iostream>
#include "LruExpiringCache.h"



int main() {
    LruExpiringCache cache(5, 30);
    //signal(SIGUSR1, signal_handler);

//    thread thread_obj(cache_cleaner);

    cache.add("alpha", 22, 1);
    cout << cache;
    cache.add("beta", 33, 1);
    cout << cache;
    cache.add("gamma", 44, 1);
    cout << cache;
    cache.add("delta", 55, 10);
    cout << cache;
    cache.add("foxtrot", 66, 16);
    cout << cache;

    cout << "accessing alpha: " << cache.get("alpha") << endl;
    cout << cache;
    cache.get("alpha");
    cout << cache;
    cache.get("alpha");
    cout << cache;
    cache.get("alpha");
    cout << cache;

    cache.get("beta");
    cout << cache;
    cache.get("beta");
    cout << cache;

//    try{
//    cache.get("betta");
//    }
//    catch(runtime_error& err){
//        cout << err.what() << endl;
//    }

    cache.get("foxtrot");
    cout << cache;

    cache.add("epsilon", 77, 35);
    cout << cache;

    //string yes;
    //cout << "Would you like to remove expired items? yes/Yes";
    //cin >> yes;

    cout << cache;
    this_thread::sleep_for(chrono::seconds(10));
    cout << cache;
    this_thread::sleep_for(chrono::seconds(7));
    cout << cache;

    return 0;
}