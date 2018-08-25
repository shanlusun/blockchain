#include "Games.hpp"

namespace Oasis {
        
    void Games::startpk(const uint64_t id, string pkname, account_name player1, account_name player2) {
        //TODO: fetch player data from Player contract table?
        //eosjs 存在即可解决问题，作为客户端中转获取player信息，然后传给games合约
        


       
    }


    void Games::getpkresult(const uint64_t id) {
        pkIndex pk(_self, _self);

        auto iterator = pk.find(id);
        eosio_assert(iterator != pk.end(), "Id for pk not found");

        auto currentPk = pk.get(id);
        print("player1: ", name{currentPk.player1});
        print(" | player2: ", name{currentPk.player2});
        print(" | PK name: ", currentPk.pkname.c_str());
        print(" | Time: ", currentPk.timestamp);
    }
}
