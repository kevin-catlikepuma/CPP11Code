#pragma once
#include "cpprest/json.h"
#include <map>

#define BET U("bet")
#define DOUBLE U("double")
#define INSURANCE U("insurance")
#define RESULT U("result")
#define NAME U("Name")
#define BALANCE U("Balance")
#define HAND U("Hand")
#define SUIT U("suit")
#define VALUE U("value")
#define CARDS U("cards")
#define CAPACITY U("Capacity")
#define ID U("Id")
#define PLAYERS U("Players")
#define DEALER U("DEALER")
#define DATA U("Data")


struct Card
{
    int suit;
    int value;

    static Card FromJSON(const web::json::object & object)
    {
        Card result;
        result.suit = object.at(SUIT).as_integer();
        result.value = object.at(VALUE).as_integer();
        return result;
    }

    web::json::value AsJSON() const 
    {
        web::json::value result = web::json::value::object();
        result[SUIT] = web::json::value::number(suit);
        result[VALUE] = web::json::value::number(value);
        return result;
    }
};

struct BJHand
{
    std::vector<Card> cards;
    double bet;
    double insurance;

	BJHand() : bet(0.0), insurance(0) { Card cd; cd.suit = 123; cd.value = 234; cards.emplace_back(cd); }

	void Clear() { cards.clear(); bet = 0.0; insurance = 0.0; }

    void AddCard(Card card)
    { 
        cards.push_back(card); 
    }

    static BJHand FromJSON(const web::json::object &object)
    {
        BJHand res;

        web::json::value cs = object.at(CARDS);

        for (auto iter = cs.as_array().begin(); iter != cs.as_array().end(); ++iter)
        {
            if (!iter->is_null())
            {
                Card card;
                card = Card::FromJSON(iter->as_object());
                res.cards.push_back(card);
            }
        }

        auto iBet = object.find(BET);
        if (iBet == object.end())
        {
            throw web::json::json_exception(U("BET key not found"));
        }

        res.bet = iBet->second.as_double();
        auto iInsurance = object.find(INSURANCE);
        if (iInsurance == object.end())
        {
            throw web::json::json_exception(U("INSURANCE key not found"));
        }
        res.insurance = iInsurance->second.as_double();
        auto iResult = object.find(RESULT);
        if (iResult == object.end())
        {
            throw web::json::json_exception(U("RESULT key not found"));
        }
        return res;
    }

    web::json::value AsJSON() const 
    {
        web::json::value res = web::json::value::object();
     
        res[BET] = web::json::value::number(bet);
        res[INSURANCE] = web::json::value::number(insurance);

        web::json::value jCards = web::json::value::array(cards.size());

        int idx = 0;
        for (auto iter = cards.begin(); iter != cards.end(); ++iter)
        {
            jCards[idx++] = iter->AsJSON();
        }
        
        res[CARDS] = jCards;
        return res;
    }
};

struct Player
{
    utility::string_t Name;
    BJHand Hand;
    double Balance;

    Player() {}
    Player(const utility::string_t &name) : Name(name), Balance(1000.0) {}

    static Player FromJSON(const web::json::object &object)
    {
        Player result(U(""));

        auto iName = object.find(NAME);
        if (iName == object.end())
        {
            throw web::json::json_exception(U("NAME key not found"));
        }
        const web::json::value &name = iName->second;
        auto iBalance = object.find(BALANCE);
        if (iBalance == object.end())
        {
            throw web::json::json_exception(U("BALANCE key not found"));
        }
        const web::json::value &balance = iBalance->second;
        auto iHand = object.find(HAND);
        if (iHand == object.end())
        {
            throw web::json::json_exception(U("HAND key not found"));
        }
        const web::json::value &hand = iHand->second;

        result.Name = name.as_string();
        result.Balance = balance.as_double();
        result.Hand = BJHand::FromJSON(hand.as_object());
        return result;
    }

    web::json::value AsJSON() const 
    {
        web::json::value result = web::json::value::object();
        result[NAME] = web::json::value::string(Name);
        result[BALANCE] = web::json::value::number(Balance);
        result[HAND] = Hand.AsJSON();
        return result;
    }
};

struct BJTable
{
    int Id;
    size_t Capacity;
    std::vector<Player> Players;

    BJTable() : Capacity(0) {}
    BJTable(int id, size_t capacity) : Id(id), Capacity(capacity) { Players.push_back(Player(DEALER)); }

    static BJTable FromJSON(const web::json::object &object)
    {
        BJTable result;
        auto iID = object.find(ID);
        if (iID == object.end())
        {
            throw web::json::json_exception(U("ID key not found"));
        }
        result.Id = (int)iID->second.as_double();
        auto iCapacity = object.find(CAPACITY);
        if (iCapacity == object.end())
        {
            throw web::json::json_exception(U("CAPACITY key not found"));
        }
        result.Capacity = (size_t)iCapacity->second.as_double();

        
        auto iPlayers = object.find(PLAYERS);
        if (iPlayers == object.end())
        {
            throw web::json::json_exception(U("PLAYTERS key not found"));
        }
        web::json::value players = iPlayers->second;
        int i = 0;

        for (auto iter = players.as_array().begin(); iter != players.as_array().end(); ++iter, i++)
        {
            result.Players.push_back(Player::FromJSON(iter->as_object()));
        }

        return result;
    }

    web::json::value AsJSON() const 
    {
        web::json::value result = web::json::value::object();
        result[ID] = web::json::value::number((double)Id);
        result[CAPACITY] = web::json::value::number((double)Capacity);

        web::json::value jPlayers = web::json::value::array(Players.size());

        size_t idx = 0;
        for (auto iter = Players.begin(); iter != Players.end(); ++iter)
        {
            jPlayers[idx++] = iter->AsJSON();
        }
        result[PLAYERS] = jPlayers;
        return result;
    }
};

inline web::json::value TablesAsJSON(const std::map<utility::string_t, std::shared_ptr<BJTable>> &tables)
{
    web::json::value result = web::json::value::array();

    size_t idx = 0;
    for (auto tbl = tables.begin(); tbl != tables.end(); tbl++)
    {
        result[idx++] = tbl->second->AsJSON();
    }
    return result;
}

