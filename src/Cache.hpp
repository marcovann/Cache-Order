#ifndef CACHE_HPP
#define CACHE_HPP

#include <shared_mutex>
#include <unordered_map>
#include <vector>

class Order;

class Cache {

    public:

        Cache();

        bool addOrder(Order iOrder);

        bool cancelOrder(const std::string& iOrderId);

        bool hasBondBeenTraded(const std::string& iBondId) const;

        std::vector<std::string> quantityOfBondsTradedMoreThan(uint32_t iQuantity) const;

        uint32_t getTotalOrders(const std::string& iDirection = {}) const;

        int32_t getBuySellTransactionsDifference() const;

        std::string getState(bool verbose = 0) const;

    protected:

        std::unordered_map<std::string, Order>      _orders;
        std::unordered_map<std::string, uint32_t>   _bonds;

        uint32_t _buyOrdersQuantity         = 0;
        uint32_t _sellOrdersQuantity        = 0;

        uint32_t _buyOrdersTransactions     = 0;
        uint32_t _sellOrdersTransactions    = 0;

    private:

        mutable std::shared_mutex mutex_;
};

#endif
