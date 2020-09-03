#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

class Order {

    public:

        Order();

        Order(std::string iOrderId,
              std::string iBondId,
              std::string iDirection,
              uint32_t iQuantity);

        std::string getOrderId() const;

        std::string getBondId() const;

        std::string getDirection() const;

        uint32_t getQuantity() const;

    private:

        std::string _orderId;
        std::string _bondId;
        std::string _direction;
        uint32_t    _quantity;

    public:

        static const std::string _buy;
        static const std::string _sell;
};

#endif
