#include "Core.h"

std::string Core::RegisterNewUser(const std::string& aUserName)
{
    size_t newUserId = mUsers.size();
    mUsers[newUserId].name = aUserName;
    mUsers[newUserId].balance = {};

    return std::to_string(newUserId);
}

std::string Core::GetUserName(const std::string& aUserId)
{
    const auto userIt = mUsers.find(stoi(aUserId));
    if (userIt == mUsers.cend())
    {
        return Status::UnknownUser;
    }
    else
    {
        return userIt->second.name;
    }
}

std::string Core::GetBalance(const std::string& aUserId)
{
    const auto userIt = mUsers.find(stoi(aUserId));
    if (userIt == mUsers.cend())
    {
        return Status::UnknownUser;
    }
    else
    {
        return userIt->second.balance.ToString();
    }
}

std::string Core::CheckTradeRequestValidity(const nlohmann::json& trade_app)
{
    if (trade_app["Type"].empty())
    {
        return Status::EmptyData;
    }

    if (trade_app["Type"] != "sell" && trade_app["Type"] != "buy")
    {
        return Status::InvalidType;
    }

    return Status::Success;
}

std::string Core::CreateTradeReq(const std::string& aUserId,
                      const std::string& TradeRequest)
{
    // парсим json с данными о заявке
    auto req = nlohmann::json::parse(TradeRequest);

    // Проверяем данные
    std::string check_result = CheckTradeRequestValidity(req);
    if (check_result != Status::Success)
    {
        return check_result;
    }   

    TradeRequest::Type req_type = (req["Type"] == "sell") ? TradeRequest::Type::Sell : TradeRequest::Type::Buy;
    int price = req["Price"];
    int amount = req["Amount"];

    // Добаляем заявку
    trade_requests_.emplace_back(stoi(aUserId),
                                 req_type, 
                                 amount,
                                 price);

    int trade_id = trade_requests_.size();
    trade_id_to_request_[trade_id] = &trade_requests_.back();

    if (req_type == TradeRequest::Type::Buy)
    {
        buy_reqs_by_price_[price].push(trade_id);
    }
    else if (req_type == TradeRequest::Type::Sell)
    {
        sell_reqs_by_price_[price].push(trade_id);
    }

    return check_result;
}

void Core::RemoveTradeReq(int trade_id)
{
    // удаляем заявку из очереди
    int price = trade_id_to_request_[trade_id]->price_;
    if (trade_id_to_request_[trade_id]->type_ == TradeRequest::Type::Buy) {
        buy_reqs_by_price_[price].pop();

        // если последняя заявка в очереди - удаляем из словаря
        if (buy_reqs_by_price_[price].empty()) 
        {
            buy_reqs_by_price_.erase(price);
        }
    }
    else 
    {
        sell_reqs_by_price_[price].pop();
        if (sell_reqs_by_price_[price].empty()) 
        {
            sell_reqs_by_price_.erase(price);
        }
    }

    trade_id_to_request_.erase(trade_id);
}

void Core::ProcessTransaction(int buy_user_id,
                              int sell_user_id, 
                              int USD, 
                              int RUB) 
{
    auto& user_buy = mUsers[buy_user_id];
    auto& user_sell = mUsers[sell_user_id];

    user_buy.balance.USD += USD;
    user_buy.balance.RUB -= RUB;

    user_sell.balance.USD -= USD;
    user_sell.balance.RUB += RUB;
}

void Core::MatchTradeRequests(int buy_trade_id, int sell_trade_id)
{
    TradeReqPtr buy_req = trade_id_to_request_[buy_trade_id];
    TradeReqPtr sell_req = trade_id_to_request_[sell_trade_id];

    // вычисляем колчиество
    int min_amount = std::min(buy_req->amount_, sell_req->amount_);

    // совершаем сделку
    ProcessTransaction(buy_req->user_id_,
                       sell_req->user_id_,
                       min_amount,
                       min_amount * buy_req->price_);

    sell_req->amount_ -= min_amount;

    // если заявка закрыта - удаляем её
    if (sell_req->amount_ == 0)
    {
        RemoveTradeReq(sell_trade_id);
    }
    buy_req->amount_ -= min_amount;
    if (buy_req->amount_ == 0)
    {
        RemoveTradeReq(buy_trade_id);
    }
}

void Core::StartTrading()
{
    // флаг наличия сделок
    bool trade_requests_available = true;
    while (trade_requests_available)
    {
        if (!buy_reqs_by_price_.empty() && !sell_reqs_by_price_.empty()) {
            
            // макс цена на покупку 
            const auto max_buy = buy_reqs_by_price_.rbegin();

            // мин цена на продажу
            const auto min_sell = sell_reqs_by_price_.begin();

            trade_requests_available = max_buy->first >= min_sell->first;
            if (trade_requests_available)
            {
                MatchTradeRequests(max_buy->second.front(),
                                   min_sell->second.front());
            }
        }
        else
        {
            trade_requests_available = false;
        }
    }
}

Core& GetCore()
{
    static Core core;
    return core;
}