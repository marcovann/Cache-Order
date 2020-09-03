#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <thread>
#include <vector>

#include "Cache.hpp"
#include "Order.hpp"

Cache::Cache()
{

}

bool Cache::addOrder(Order iOrder) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    if (_orders.find(iOrder.getOrderId()) == _orders.end())
    {
        _orders[iOrder.getOrderId()] = iOrder;
        if (_bonds.find(iOrder.getBondId()) == _bonds.end())
        {
            _bonds[iOrder.getBondId()] = iOrder.getQuantity();
        }
        else
        {
            _bonds[iOrder.getBondId()] += iOrder.getQuantity();
        }

        if (iOrder.getDirection() == Order::_buy)
        {
            _buyOrdersQuantity += iOrder.getQuantity();
            _buyOrdersTransactions += 1;
        }
        else if (iOrder.getDirection() == Order::_sell)
        {
            _sellOrdersQuantity += iOrder.getQuantity();
            _sellOrdersTransactions += 1;
        }

        return true;
    }
    return false;
}

bool Cache::cancelOrder(const std::string& iOrderId)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    const auto& orderIt = _orders.find(iOrderId);
    if (orderIt == _orders.end())
    {
        return false;
    }

    auto order = orderIt->second;
    _bonds[order.getBondId()] -= order.getQuantity();
    if (_bonds[order.getBondId()] == 0)
    {
        _bonds.erase(order.getBondId());
    }
    if (order.getDirection() == Order::_buy)
    {
        _buyOrdersQuantity -= order.getQuantity();
        _buyOrdersTransactions -= 1;
    }
    else if (order.getDirection() == Order::_sell)
    {
        _sellOrdersQuantity -= order.getQuantity();
        _sellOrdersTransactions -= 1;
    }
    _orders.erase(iOrderId);
    return true;
}

std::vector<std::string> Cache::quantityOfBondsTradedMoreThan(uint32_t iQuantity) const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    std::vector<std::string> bondIds;
    for (const auto& bond : _bonds)
    {
        if (bond.second > iQuantity)
        {
            bondIds.push_back(bond.first);
        }
    }
    return bondIds;
}

bool Cache::hasBondBeenTraded(const std::string& iBondId) const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return _bonds.find(iBondId) != _bonds.end();
}

uint32_t Cache::getTotalOrders(const std::string& iDirection) const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    if (iDirection.empty())
    {
        return _buyOrdersQuantity + _sellOrdersQuantity;
    }
    else if (iDirection == Order::_buy)
    {
        return _buyOrdersQuantity;
    }
    else if (iDirection == Order::_sell)
    {
        return _sellOrdersQuantity;
    }
    else
    {
        return 0;
    }
}

int32_t Cache::getBuySellTransactionsDifference() const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return _buyOrdersTransactions - _sellOrdersTransactions;
}

std::string Cache::getState(bool verbose) const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    std::stringstream ss;

    if (verbose) ss << "Orders: ";
    ss << _orders.size() << ";";

    if (verbose) ss << "Bonds: ";
    ss << _bonds.size() << ";";

    if (verbose) ss << "BuyOrdersQuantity: ";
    ss << _buyOrdersQuantity << ";";

    if (verbose) ss << "SellOrdersQuantity: ";
    ss << _sellOrdersQuantity << ";";

    if (verbose) ss << "BuyOrdersTransactions: ";
    ss << _buyOrdersTransactions << ";";

    if (verbose) ss << "SellOrdersTransactions: ";
    ss << _sellOrdersTransactions;

    return ss.str();
}
