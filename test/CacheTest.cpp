#include <vector>

#include "../src/Order.hpp"
#include "../src/Cache.hpp"
#include "gtest/gtest.h"

class CacheTest : public Cache
{
    public:

        bool isOrderPresent(const std::string& orderId)
        {
            return _orders.count(orderId);
        }
};

TEST(Cache, AddOrder)
{
    CacheTest cache;

    Order order1("O1", "B1", "B", 10);
    Order order2("O2", "B1", "S", 15);
    cache.addOrder(order1);

    EXPECT_TRUE(cache.isOrderPresent("O1"));
    EXPECT_FALSE(cache.isOrderPresent("O2"));

    cache.addOrder(order1);
    cache.addOrder(order2);

    EXPECT_TRUE(cache.isOrderPresent("O1"));
    EXPECT_TRUE(cache.isOrderPresent("O2"));
}

TEST(Cache, CancelOrder)
{
    CacheTest cache;

    Order order1("O1", "B1", "B", 10);
    Order order2("O2", "B2", "S", 15);
    cache.addOrder(order1);
    cache.addOrder(order2);

    EXPECT_TRUE(cache.isOrderPresent("O1"));
    EXPECT_TRUE(cache.isOrderPresent("O2"));

    cache.cancelOrder("O2");

    EXPECT_TRUE(cache.isOrderPresent("O1"));
    EXPECT_FALSE(cache.isOrderPresent("O2"));
}

TEST(Cache, HasBondBeenTraded)
{
    CacheTest cache;

    Order order1("O1", "B1", "B", 10);
    Order order2("O2", "B2", "S", 15);
    Order order3("O3", "B3", "B", 20);
    cache.addOrder(order1);
    cache.addOrder(order2);

    EXPECT_TRUE(cache.hasBondBeenTraded("B1"));
    EXPECT_TRUE(cache.hasBondBeenTraded("B2"));
    EXPECT_FALSE(cache.hasBondBeenTraded("B3"));

    cache.addOrder(order3);

    EXPECT_TRUE(cache.hasBondBeenTraded("B3"));
}

TEST(Cache, QuantityOfBondsTradedMoreThan)
{
    CacheTest cache;

    std::vector<Order> orders =
    {
        Order("O1", "B1", "B", 10),
        Order("O2", "B2", "S", 15),
        Order("O3", "B3", "B", 20),
        Order("O4", "B4", "B", 10)
    };

    for (const auto& order : orders)
    {
        cache.addOrder(order);
    }

    std::vector<std::string> expectedResult = {"B3"};
    EXPECT_TRUE(cache.quantityOfBondsTradedMoreThan(15) == expectedResult);

    Order order1("O5", "B4", "B", 8);
    cache.addOrder(order1);

    expectedResult.push_back("B4");
    std::vector<std::string> actualResult = cache.quantityOfBondsTradedMoreThan(15);
    sort(actualResult.begin(), actualResult.end());
    EXPECT_TRUE(actualResult == expectedResult);
}

TEST(Cache, GetTotalOrders)
{
    CacheTest cache;

    Order order1("O1", "B1", "B", 10);
    Order order2("O2", "B2", "S", 15);
    Order order3("O3", "B3", "B", 20);
    cache.addOrder(order1);
    cache.addOrder(order2);

    EXPECT_EQ(cache.getTotalOrders("B"), 10);
    EXPECT_EQ(cache.getTotalOrders("S"), 15);
    EXPECT_EQ(cache.getTotalOrders(), 25);

    cache.addOrder(order3);

    EXPECT_EQ(cache.getTotalOrders("B"), 30);
    EXPECT_EQ(cache.getTotalOrders("S"), 15);
    EXPECT_EQ(cache.getTotalOrders(), 45);
}

TEST(Cache, GetBuySellTransactionsDifference)
{
    CacheTest cache;

    Order order1("O1", "B1", "B", 10);
    Order order2("O2", "B2", "S", 15);
    Order order3("O3", "B3", "B", 20);
    cache.addOrder(order1);
    cache.addOrder(order2);

    EXPECT_EQ(cache.getBuySellTransactionsDifference(), 0);

    cache.addOrder(order3);

    EXPECT_EQ(cache.getBuySellTransactionsDifference(), 1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
