# This is a sample Python script.
import time

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.
from LruExpiringCache import *

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    with CacheManager(5, 30) as lru_cache:
        lru_cache = LruExpiringCache(5, 30)

        lru_cache.add("alpha", 11, 1)
        lru_cache.add("beta", 22, 1)
        lru_cache.add("gamma", 33, 3)
        lru_cache.add("epsilon", 44, 5)
        lru_cache.add("xi", 55, 10)
        lru_cache.get("alpha")
        lru_cache.add("foxtrot", 66, 40)
        print(lru_cache)

        print(lru_cache.get("xi").value)
        print(lru_cache.get("epsilon").value)
        # print(lru_cache.get("omega"))

        lru_cache.remove("xi")
        time.sleep(5)


        print(lru_cache)

    print("Done")
# See PyCharm help at https://www.jetbrains.com/help/pycharm/
