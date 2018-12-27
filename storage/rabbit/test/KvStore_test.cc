//
// Created by NorSnow_ZJ on 2018/12/27.
//

#include <gtest/gtest.h>
#include "KvStore.h"
#include <vector>

class KvStore_test : public testing::Test {
protected:
    KvStore *store;
    
    virtual void SetUp() {
        store = new KvStore();
        store->start();
    }
    
    virtual void TearDown() {
        store->stop();
        delete store;
    }
};

TEST_F(KvStore_test, testAddGet) {
    struct TestData {
        std::string key;
        std::string val;
    };
    std::vector<TestData> kvs = {
        {"123", "abc"},
        {"qwe", "!@#"},
        {"!~!", "<<>>"},
        {"..", "+++"},
        {"123!@#$%", "oooo009()"},
        {"0000", "11111"}
    };
    
    
    for (const auto & kv : kvs) {
        EXPECT_TRUE(store->add(kv.key, kv.val));
    }
    for (const auto & kv : kvs) {
        EXPECT_FALSE(store->add(kv.key, kv.val));
    }
    
    std::string val;
    for (const auto  & kv : kvs) {
        EXPECT_TRUE(store->get(kv.key, val));
        EXPECT_EQ(kv.val, val);
    }
    
    EXPECT_FALSE(store->get("NO KEY", val));
}

TEST_F(KvStore_test, testNext) {
    struct TestData {
        std::string key;
        std::string val;
        std::string nextKey;
        std::string nextVal;
    };
    std::vector<TestData> kvs  = {
       {"1", "a", "2", "b"},
       {"2", "b", "3", "c"},
       {"3", "c", "4", "d"},
       {"4", "d", "5", "e"},
       {"5", "e", KvStore::kEndKey, ""}
   };
    
    for (const auto & kv : kvs) {
        EXPECT_TRUE(store->add(kv.key, kv.val));
    }
    
    std::string nextKey, nextVal;
    for (size_t i = 0; i < kvs.size()-1; ++i) {
        EXPECT_TRUE(store->next(kvs[i].key, nextKey, nextVal));
        EXPECT_EQ(nextKey, kvs[i].nextKey);
        EXPECT_EQ(nextVal, kvs[i].nextVal);
    }
    
    EXPECT_TRUE(store->next(kvs[kvs.size()-1].key, nextKey, nextVal));
    EXPECT_EQ(nextKey, KvStore::kEndKey);
    EXPECT_FALSE(store->next("no key", nextKey, nextVal));
}

TEST_F(KvStore_test, testSet) {
    struct TestData {
        std::string key;
        std::string val;
        std::string setVal;
    };
    std::vector<TestData> kvs  = {
        {"1", "a", "q"},
        {"2", "b", "w"},
        {"3", "c", "e"},
        {"4", "d", "r"},
        {"5", "e", "t"}
    };
    
    for (const auto & kv : kvs) {
        EXPECT_TRUE(store->add(kv.key, kv.val));
    }
    
    std::string val;
    for (const auto & kv : kvs) {
        EXPECT_TRUE(store->set(kv.key, kv.setVal));
        EXPECT_TRUE(store->get(kv.key, val));
        EXPECT_EQ(kv.setVal, val);
    }
    
    EXPECT_FALSE(store->set("no key", "no"));
}

TEST_F(KvStore_test, testDelete) {
    struct TestData {
        std::string key;
        std::string val;
    };
    std::vector<TestData> kvs = {
        {"1", "a"},
        {"2", "b"},
        {"3", "c"},
        {"4", "d"},
        {"5", "e"}
    };
    
    for (const auto & kv : kvs) {
        EXPECT_TRUE(store->add(kv.key, kv.val));
    }
    
    EXPECT_FALSE(store->del("no key"));
    
    std::string val;
    for (const auto & kv : kvs) {
        EXPECT_TRUE(store->del(kv.key));
        EXPECT_FALSE(store->get(kv.key, val));
    }
}