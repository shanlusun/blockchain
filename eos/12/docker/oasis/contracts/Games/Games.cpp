#include "Games.hpp"

namespace Oasis {
        
    void Games::startpk(const uint64_t id, string pkname, account_name player1, account_name player2) {
        //TODO: fetch player data from Player contract table?
        playerIndex players(N(player), N(player));

        auto iterator1 = players.find(player1);
        eosio_assert(iterator1 != players.end(), "Address for player1 not found");

        auto iterator2 = players.find(player2);
        eosio_assert(iterator2 != players.end(), "Address for player2 not found");

        int64_t random = rand();
        print("random: ", random);

        auto currentPlayer1 = players.get(player1);
        auto currentPlayer2 = players.get(player2);

        print(" | player1: Username: ", currentPlayer1.username.c_str());
        print(" Level: ", currentPlayer1.level);
        print(" Health: ", currentPlayer1.health_points);
        print(" Energy: ", currentPlayer1.energy_points);

        print(" | player2: Username: ", currentPlayer2.username.c_str());
        print(" Level: ", currentPlayer2.level);
        print(" Health: ", currentPlayer2.health_points);
        print(" Energy: ", currentPlayer2.energy_points);

        double level = (currentPlayer1.level - currentPlayer2.level)*0.5;
        double health = (currentPlayer1.health_points - currentPlayer2.health_points)*0.3;
        double energy = (currentPlayer1.energy_points - currentPlayer2.energy_points)*0.2;
        int64_t result = (level + health + energy) * (random%4 == 0 ? -1 : 1);

        if (result == 0) // The two players have same power to fight
        {
            result = (random%4 == 0 ? -1 : 1);
        }

        account_name winner;
        if (result > 0)
        {
            winner = player1;
            print(" | Winner is: ", name{player1});
        } else {
            winner = player2;
            print(" | Winner is: ", name{player2});
        }

        //save the pk result to pk table
        pkIndex pk(_self, _self);

        pk.emplace(player1, [&](auto &pk) {
            pk.pk_id = id;
            pk.pkname = pkname;
            pk.player1 = player1;
            pk.player2 = player2;
            pk.winner = winner;
            pk.timestamp = current_time();
        });
    }

    void Games::getpkresult(const uint64_t id) {
        pkIndex pk(_self, _self);

        auto iterator = pk.find(id);
        eosio_assert(iterator != pk.end(), "Id for pk not found");

        auto currentPk = pk.get(id);
        print("winner: ", name{currentPk.winner});
        print(" | player1: ", name{currentPk.player1});
        print(" | player2: ", name{currentPk.player2});
        print(" | PK name: ", currentPk.pkname.c_str());
        print(" | Time: ", currentPk.timestamp);
    }
}
