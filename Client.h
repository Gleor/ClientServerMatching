#pragma once
#include <iostream>
#include <vector>
#include <boost/asio.hpp>

#include "Common.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;

// Отправка сообщения на сервер по шаблону.
void SendMessage(
    tcp::socket& aSocket,
    const std::string& aId,
    const std::string& aRequestType,
    const std::string& aMessage);

// Возвращает строку с ответом сервера на последний запрос.
std::string ReadMessage(tcp::socket& aSocket);

// Регистрируем пользователя, получаем его ID.
std::string ProcessRegistration(tcp::socket& aSocket);

// обработка торговой заявки 
std::string ProcessTradeRequest();

// обработка данных из потока
template <typename Type>
Type ParseStream(const std::string& init_message,
           const std::string& error)
{
    Type answer;
    while (true) {
        std::cout << init_message;
        std::flush(std::cout);
        std::cin >> answer;
        while (std::cin.get() != '\n');
        if (std::cin.good()) break;
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << error << '\n';
    }
    return answer;
}