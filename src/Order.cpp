#include "Order.hpp"

const std::string Order::_buy  = "B";
const std::string Order::_sell = "S";

Order::Order()
{

}

Order::Order(std::string iOrderId, std::string iBondId, std::string iDirection, uint32_t iQuantity)
    : _orderId(iOrderId), _bondId(iBondId), _direction(iDirection), _quantity(iQuantity)
{

}

std::string Order::getOrderId() const
{
    return _orderId;
}

std::string Order::getBondId() const
{
    return _bondId;
}

std::string Order::getDirection() const
{
    return _direction;
}

uint32_t Order::getQuantity() const
{
    return _quantity;
}
