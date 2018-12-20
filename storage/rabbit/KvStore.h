//
// Created by NorSnow_ZJ on 2018/12/10.
//

#ifndef MYSQL_KV_H
#define MYSQL_KV_H

#include <map>
#include <thread>
#include <mutex>
#include <queue>
#include <future>
#include <atomic>

class KvStore {
public:
    using KVMap = std::map<std::string, std::string>;
    using Task = std::packaged_task<bool(void)>;
    using TaskQueue = std::queue<Task>;
    using WorkerPtr = std::shared_ptr<std::thread>;

    const static char *kBeginKey;
    const static char *kEndKey;

    bool get(const std::string &key, std::string &val);
    bool set(const std::string &key, const std::string &val);
    bool add(const std::string &key, const std::string &val);
    bool del(const std::string &key);
    bool next(const std::string &curKey, std::string &nextKey, std::string &val);
    void start();
    void stop();

private:
    void run();
    bool doGet(const std::string &key, std::string &val);
    bool doSet(const std::string &key, const std::string &val);
    bool doAdd(const std::string &key, const std::string &val);
    bool doDel(const std::string &key);
    bool doNext(const std::string &curKey, std::string &nextKey, std::string &val);

    std::atomic_bool run_;
    KVMap kvMap_;
    //TODO:临时使用一个简单的带锁的queue来充当任务队列
    std::mutex mu_;
    std::condition_variable_any cond_;
    TaskQueue queue_;
    WorkerPtr worker_;
};


#endif //MYSQL_KV_H
