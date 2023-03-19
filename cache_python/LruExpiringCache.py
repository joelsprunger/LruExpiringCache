import datetime
import threading
import time
from typing import Any


class CacheManager:
    def __init__(self, max_size: int = 1000000, max_age_seconds: int = 30):
        self.max_size = max_size
        self.max_age_seconds = max_age_seconds
        self.t1: threading.Thread
        self.cache_on: bool = False

    def __enter__(self):
        self.cache = LruExpiringCache(self.max_size, self.max_age_seconds)
        self.cache_on = True
        self._start_cache()
        return self.cache

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._stop_cache()

    def _start_cache(self):
        self.t1 = threading.Thread(target=self._manage_cache)
        self.t1.start()

    def _stop_cache(self):
        self.cache_on = False
        self.t1.join()

    def _manage_cache(self):
        while self.cache_on:
            time.sleep(1)
            self.cache.clean_cache()


class Item:
    def __init__(self, value: Any, max_age_seconds: int = 30):
        self.value: Any = value
        self.max_age_seconds: int = max_age_seconds
        self.usage: int = 0
        self.expiration: datetime = datetime.datetime.now() + datetime.timedelta(seconds=max_age_seconds)


class LruExpiringCache(dict):
    def __init__(self, max_size: int = 1000000, max_age_seconds: int = 30):
        super().__init__()
        self.max_size = max_size
        self.max_age_seconds = max_age_seconds
        self.usage_counter = 0
        self.lock = threading.RLock()

    def add(self, key: Any, value: Any, max_age_seconds: int):
        with self.lock:
            if len(self) >= self.max_size:
                self.remove(self._next_out())
            if max_age_seconds > self.max_age_seconds:
                max_age_seconds = self.max_age_seconds
            self[key] = Item(value=value, max_age_seconds=max_age_seconds)

    def remove(self, key: Any):
        with self.lock:
            if key in self.keys():
                del self[key]

    def get(self, key: Any) -> Any:
        with self.lock:
            try:
                self[key].usage = self.usage_counter + 1
                self.usage_counter += 1
                return self[key]
            except KeyError:
                raise KeyError(f"***{key} is not in the cache!***")

    def _next_out(self) -> Any:
        with self.lock:
            # which items have the lowest usage?
            lowest_usage: int = -1
            for _, item in self.items():
                if (item.usage < lowest_usage) or (lowest_usage == -1):
                    lowest_usage = item.usage
            keys: list[Any] = []
            for key, item in self.items():
                if item.usage == lowest_usage:
                    keys.append(key)
            # which items expires next?
            next_expiration: datetime = self[keys[0]].expiration
            first_out_key: Any = keys[0]
            for key in keys:
                if self[key].expiration < next_expiration:
                    next_expiration = self[key].expiration
                    first_out_key = key
            # remove that item
            return first_out_key

    def clean_cache(self):
        with self.lock:
            current_time: datetime = datetime.datetime.now()
            keys: list[Any] = []
            for key, item in self.items():
                if item.expiration < current_time:
                    keys.append(key)
            for key in keys:
                self.remove(key)

    def __repr__(self):
        with self.lock:
            out_str: str = ""
            for key, item in self.items():
                out_str += f"{key}: value {item.value}, usage {item.usage}, expiration {item.expiration}\n"
            return out_str

