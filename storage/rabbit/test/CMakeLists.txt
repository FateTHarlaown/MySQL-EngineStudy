INCLUDE_DIRECTORIES(..)
set(TEST_SOURCE
                test.cpp
                KvStore_test.cc
                ../KvStore.cc
                ../Record.cc)
set(TEST_LIBRARIES gtest gmock pthread)
add_executable(rabbit_unitest ${TEST_SOURCE})
target_link_libraries(rabbit_unitest ${TEST_LIBRARIES})