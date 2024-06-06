#pragma once
#include <iostream>
#include <deque>
#include <queue>

#include "json.hpp"
#include "Common.hpp"

class Core
{
public:
    // Регистрирует нового пользователя и возвращает его ID.
    std::string RegisterNewUser(const std::string& aUserName);

    // Запрос имени клиента по ID
    std::string GetUserName(const std::string& aUserId);

    // Запрос баланса клиента по ID
    std::string GetBalance(const std::string& aUserId);

    // Проверяет данные торговой заявки
    std::string CheckTradeRequestValidity(const nlohmann::json& trade_app);

    // Создаёт торговую заявку и возвращает статус
    std::string CreateTradeReq(const std::string& aUserId,
                               const std::string& TradeApplication);

    // Проверяет доступных заявок и совершение сделок
    void StartTrading();

private:
    // Пользователи (Id, Name)
    std::unordered_map<size_t, User> mUsers;

    // Торговые сделки
    std::deque<TradeRequest> trade_requests_;

    // Id заявок, указатель на заявку
    std::unordered_map<size_t, TradeReqPtr> trade_id_to_request_;

    // Словари заявок (Id) с сортировкой по цене
    std::map<int, std::queue<size_t>> sell_reqs_by_price_;
    std::map<int, std::queue<size_t>> buy_reqs_by_price_;

    // Удаление тогровой заявки
    void RemoveTradeReq(int trade_id);

    // Совершает транзакцию между пользователями
    void ProcessTransaction(int buy_user_id,
                            int sell_user_id, 
                            int USD, 
                            int RUB);

    // Совершает торговую сделку между пользователями
    void MatchTradeRequests(int buy_trade_id, int sell_trade_id);
};

Core& GetCore();