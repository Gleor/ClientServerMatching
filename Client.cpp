#include "Client.h"

using boost::asio::ip::tcp;

void SendMessage(
    tcp::socket& aSocket,
    const std::string& aId,
    const std::string& aRequestType,
    const std::string& aMessage)
{
    nlohmann::json req;
    req["UserId"] = aId;
    req["ReqType"] = aRequestType;
    req["Message"] = aMessage;

    std::string request = req.dump();
    boost::asio::write(aSocket, boost::asio::buffer(request, request.size()));
}

std::string ReadMessage(tcp::socket& aSocket)
{
    boost::asio::streambuf buf;
    boost::asio::read_until(aSocket, buf, "\0");
    std::istream is(&buf);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}

std::string ProcessRegistration(tcp::socket& aSocket)
{
    std::string name;
    std::cout << "Hello! Enter your name: ";
    std::cin >> name;

    // Для регистрации Id не нужен, заполним его нулём
    SendMessage(aSocket, "0", Requests::Registration, name);
    return ReadMessage(aSocket);
}
 
std::string ProcessTradeRequest()
{
    nlohmann::json tradeRequest;
    
    // получаем тип сделки
    tradeRequest["Type"] = ParseStream<std::string>("Sell or buy: ",
                                                    "Error: parse type");

    // получаем количество валюты                                      
    tradeRequest["Amount"] = ParseStream<int>("Amount: ",
                                              "Error: parse amount");

    // получаем цену
    tradeRequest["Price"] = ParseStream<int>("Price: ",
                                             "Error: parse price");
    return tradeRequest.dump();
}