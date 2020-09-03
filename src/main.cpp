#include <cassert>
#include <iostream>
#include <thread>
#include <time.h>
#include <set>
#include <vector>
#include "Order.hpp"
#include "Cache.hpp"

#define LOOPS 200

void readCache(Cache* cache,
               const std::vector<Order>& orders,
               const std::set<std::string>& states)
{
    for (int i = 0; i < LOOPS; i++)
    {
        int r = rand() % orders.size();
        if (i % 10 == 0)
        {
            std::cout << "ReadCache: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(r));
        }
        std::string state = cache->getState();
        if (!state.empty())
        {
            assert(states.find(state) != states.end());
        }
    }
}

void writeCache(Cache* cache,
               const std::vector<Order>& orders,
               const std::set<std::string>& states)
{
    for (int i = 0; i < LOOPS; i++)
    {
        int r = rand() % orders.size();
        cache->addOrder(orders[r]);
        if (i % 10 == 0)
        {
            std::cout << "WriteCache: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(r));
        }
    }
}

void deleteCache(Cache* cache,
                 const std::vector<Order>& orders,
                 const std::set<std::string>& states)
{
    for (int i = 0; i < LOOPS; i++)
    {
        int r = rand() % orders.size();
        cache->cancelOrder(orders[r].getOrderId());
        if (i % 10 == 0)
        {
            std::cout << "DeleteCache: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(r));
        }
    }
}

void all_valid_states(const std::vector<Order>& orders,
                      Cache* cache,
                      uint32_t current_order,
                      std::set<std::string>& states)
{
    if (current_order == orders.size())
    {
        return;
    }

    for (uint32_t i = current_order; i < orders.size(); i++)
    {
        cache->addOrder(orders[i]);
        states.insert(cache->getState());
        all_valid_states(orders, cache, current_order + 1, states);
        cache->cancelOrder(orders[i].getOrderId());
    }

    return;
}

int main()
{
    srand( (unsigned)time(NULL) );

    std::vector<Order> orders =
    {
        Order("O1", "B1", "B", 10),
        Order("O2", "B2", "B", 15),
        Order("O3", "B3", "S", 12),
        Order("O4", "B3", "B", 17),
        Order("O5", "B5", "S", 13),
        Order("O6", "B5", "B", 22),
        Order("O7", "B6", "S", 16),
        Order("O8", "B7", "B", 23),
        Order("O9", "B8", "B", 25)
    };

    Cache* cache = new Cache();
    std::set<std::string> states = {"0;0;0;0;0;0"};
    all_valid_states(orders, cache, 0, states);
    delete cache;

    Cache* cacheTest = new Cache();
    std::thread t1(readCache, cacheTest, orders, states);
    std::thread t2(writeCache, cacheTest, orders, states);
    std::thread t3(deleteCache, cacheTest, orders, states);

    t1.join();
    t2.join();
    t3.join();

    delete cacheTest;
    std::cout << "Test passed" << std::endl;

    return 0;
}
