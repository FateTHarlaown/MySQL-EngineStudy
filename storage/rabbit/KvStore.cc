//
// Created by NorSnow_ZJ on 2018/12/10.
//

#include <functional>
#include "KvStore.h"

const char *KvStore::kBeginKey = "BeginKey";
const char *KvStore::kEndKey = "EndKey";

bool KvStore::get(const std::string &key, std::string &val) {
    Task t(std::bind(&KvStore::doGet, this, std::cref(key), std::ref(val)));
    auto f = t.get_future();
    {
        std::lock_guard<std::mutex> guard(mu_);
        queue_.push(std::move(t));
        cond_.notify_one();
    }
    return f.get();
}

bool KvStore::set(const std::string &key, const std::string &val) {
    Task t(std::bind(&KvStore::doSet, this, std::cref(key), std::ref(val)));
    auto f = t.get_future();
    {
        std::lock_guard<std::mutex> guard(mu_);
        queue_.push(std::move(t));
        cond_.notify_one();
    }
    return f.get();
}

bool KvStore::add(const std::string &key, const std::string &val) {
    Task t(std::bind(&KvStore::doAdd, this, std::cref(key), std::ref(val)));
    auto f = t.get_future();
    {
        std::lock_guard<std::mutex> guard(mu_);
        queue_.push(std::move(t));
        cond_.notify_one();
    }
    return f.get();
}

bool KvStore::del(const std::string &key) {
    Task t(std::bind(&KvStore::doDel, this, std::cref(key)));
    auto f = t.get_future();
    {
        std::lock_guard<std::mutex> guard(mu_);
        queue_.push(std::move(t));
        cond_.notify_one();
    }
    return f.get();
}

bool KvStore::next(const std::string &curKey, std::string &nextKey, std::string &val) {
    Task t(std::bind(&KvStore::doNext, this, std::cref(curKey), std::ref(nextKey), std::ref(val)));
    auto f = t.get_future();
    {
        std::lock_guard<std::mutex> guard(mu_);
        queue_.push(std::move(t));
        cond_.notify_one();
    }
    return f.get();
}

void KvStore::start() {
    run_ = true;
    worker_ = std::make_shared<std::thread>(std::bind(&KvStore::run, this));
    worker_->detach();
}

void KvStore::stop() {
    run_ = false;
}

bool KvStore::doGet(const std::string &key, std::string &val) {
    auto it = kvMap_.find(key);
    if (it == kvMap_.end()) {
        return false;
    }
    val = it->second;
    return true;
}

bool KvStore::doSet(const std::string &key, const std::string &val) {
    auto it = kvMap_.find(key);
    if (it == kvMap_.end()) {
        return false;
    }
    it->second = val;
    return true;
}

bool KvStore::doAdd(const std::string &key, const std::string &val) {
    auto it = kvMap_.find(key);
    if (it != kvMap_.end()) {
        return false;
    }
    kvMap_.insert(KVMap::value_type(key, val));
    return true;
}

bool KvStore::doDel(const std::string &key) {
    auto it = kvMap_.find(key);
    if (it == kvMap_.end()) {
        return false;
    }
    kvMap_.erase(key);
    return true;
}

bool KvStore::doNext(const std::string &curKey, std::string &nextKey, std::string &val) {
    auto it = kvMap_.lower_bound(curKey);
    if (it == kvMap_.end()) {
        return false;
    }
    if (curKey == it->first) {
        it++;
    }
    if (it == kvMap_.end()) {
        nextKey = kEndKey;
    } else {
        nextKey = it->first;
        val = it->second;
    }
    return true;
}

void KvStore::run() {
    while (run_) {
        Task t;
        {
            std::lock_guard<std::mutex> guard(mu_);
            while (queue_.empty()) {
                cond_.wait(mu_);
            }
            t = std::move(queue_.front());
            queue_.pop();
        }
        t();
    }
}
