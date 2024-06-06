#pragma once
#include <cassert>

#include "json.hpp"
#include "Common.hpp"
#include "Core.h"

void TestTradeDefault()
{
    GetCore().RegisterNewUser("Alex"); // id 0
    assert(GetCore().GetBalance("0") == "USD: 0\nRUB: 0");

    GetCore().RegisterNewUser("Bob"); // id 1
    assert(GetCore().GetBalance("1") == "USD: 0\nRUB: 0");

    GetCore().RegisterNewUser("Jon"); // id 2
    assert(GetCore().GetBalance("2") == "USD: 0\nRUB: 0");

    GetCore().RegisterNewUser("Mark"); // id 3
    assert(GetCore().GetBalance("3") == "USD: 0\nRUB: 0");

    GetCore().CreateTradeReq("0", R"({"Price":62,"Type":"buy","Amount":10})");
    GetCore().StartTrading();
    assert(GetCore().GetBalance("0") == "USD: 0\nRUB: 0");

    GetCore().CreateTradeReq("1", R"({"Price":63,"Type":"buy","Amount":20})");
    GetCore().StartTrading(); 

    assert(GetCore().GetBalance("1") == "USD: 0\nRUB: 0");

    GetCore().CreateTradeReq("2", R"({"Price":61,"Type":"sell","Amount":50})");
    GetCore().StartTrading();

    assert(GetCore().GetBalance("0") == "USD: 10\nRUB: -620");
    assert(GetCore().GetBalance("1") == "USD: 20\nRUB: -1260");
    assert(GetCore().GetBalance("2") == "USD: -30\nRUB: 1880");

    // закрываем частичную заявку
    GetCore().CreateTradeReq("3", R"({"Price":61,"Type":"buy","Amount":20})");
    GetCore().StartTrading();

    assert(GetCore().GetBalance("2") == "USD: -50\nRUB: 3100");
    assert(GetCore().GetBalance("3") == "USD: 20\nRUB: -1220");

    GetCore().CreateTradeReq("0", R"({"Price":61,"Type":"buy","Amount":10})");
    GetCore().StartTrading();

    GetCore().CreateTradeReq("0", R"({"Price":62,"Type":"buy","Amount":10})");
    GetCore().StartTrading();

    GetCore().CreateTradeReq("1", R"({"Price":61,"Type":"sell","Amount":20})");
    GetCore().StartTrading();

    assert(GetCore().GetBalance("0") == "USD: 30\nRUB: -1850");
    assert(GetCore().GetBalance("1") == "USD: 0\nRUB: -30");

}

void RunTests()
{
    TestTradeDefault();

    std::cout << "All tests completed" << std::endl;    
}