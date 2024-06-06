#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>

static short port = 5555;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string Balance = "Bal";
    static std::string TradeReq = "Trade";
}

namespace Status
{
    static std::string Success = "Success";
    static std::string UnknownUser = "Error: unknown user";
    static std::string EmptyData = "Error: data empty";
    static std::string InvalidType = "Error: invalid type";
}

struct Balance {
    int USD = {};
    int RUB = {};

    std::string ToString() const
    {
        return "USD: " + std::to_string(USD) + '\n' +
               "RUB: " + std::to_string(RUB);
    }
};

struct User
{
    std::string name;
    Balance balance;
};

struct TradeRequest
{
    enum Type
    {
        Sell,
        Buy
    };

    TradeRequest(int id,
             TradeRequest::Type type,
             int amount,
             int price)
        : user_id_(id)
        , type_(type)
        , amount_(amount)
        , price_(price) {
    }

    int user_id_;
    Type type_;
    int amount_;
    int price_;
};

using TradeReqPtr = TradeRequest*;

#endif //CLIENSERVERECN_COMMON_HPP
