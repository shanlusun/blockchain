#include "Players.hpp"

namespace Oasis {
    void Players::add(account_name account, string& username) {
        /**
        * We require that only the owner of an account can use this action
        * or somebody with the account authorization
        */
        require_auth(account);

        /**
        * We access the "player" table as creating an object of type "playerIndex"
        * As parameters we pass code & scope - _self from the parent contract
        */
        playerIndex players(_self, _self);

        /**
        * We must verify that the account doesn't exist yet
        * If the account is not found the iterator variable should be players.end()
        */
        auto iterator = players.find(account);
        eosio_assert(iterator == players.end(), "Address for account already exists");

        /**
        * We add the new player in the table
        * The first argument is the payer of the storage which will store the data
        */
        players.emplace(account, [&](auto &player) {
            player.account_name = account;
            player.username = username;
            player.level = 1;
            player.health_points = 1000;
            player.energy_points = 1000;
        });
    }

    void Players::update(account_name account, uint64_t level, int64_t healthPoints, int64_t energyPoints) {
        require_auth(account);

        playerIndex players(_self, _self);

        auto iterator = players.find(account);
        eosio_assert(iterator != players.end(), "Address for account not found");

        /**
        * We add the new player in the table
        * The first argument is the payer of the storage which will store the data
        */
        players.modify(iterator, account, [&](auto &player) {
            player.level = level;

            if ((player.health_points - healthPoints) < 0)
            {
                player.health_points = 0;
            }
            else
            {
                player.health_points -= healthPoints;
            }

            if ((player.energy_points - energyPoints) < 0)
            {
                player.energy_points = 0;
            }
            else
            {
                player.energy_points -= energyPoints;
            }
        });
    }
        
    void Players::getplayer(const account_name account) {
        playerIndex players(_self, _self);

        auto iterator = players.find(account);
        eosio_assert(iterator != players.end(), "Address for account not found");

        /**
        * The "get" function returns a constant reference to the object
        * containing the specified secondary key
        */
        auto currentPlayer = players.get(account);
        print("Username: ", currentPlayer.username.c_str());
        print(" Level: ", currentPlayer.level);
        print(" Health: ", currentPlayer.health_points);
        print(" Energy: ", currentPlayer.energy_points);
        
        if (currentPlayer.abilities.size() > 0) {
            print(" Abilities: ");

            for (uint32_t i = 0; i < currentPlayer.abilities.size(); i++) {
                print(currentPlayer.abilities.at(i).c_str(), ", ");
            }
        } else {
            print(" No Abilities");
        }

        if (currentPlayer.inventory.size() > 0) {
            print(" Items: ");

            for (uint32_t i = 0; i < currentPlayer.inventory.size(); i++) {
                item currentItem = currentPlayer.inventory.at(i);
                print(currentItem.name.c_str(), " == ");
            }
        } else {
            print(" Empty inventory");
        }
    }

    void Players::addability(const account_name account, string& ability) {
        require_auth(account);

        playerIndex players(_self, _self);

        auto iterator = players.find(account);
        eosio_assert(iterator != players.end(), "Address for account not found");

        players.modify(iterator, account, [&](auto& player) {
            player.abilities.push_back(ability);
        });
    }

    void Players::additem(const account_name account, item purchased_item) {
        playerIndex players(_self, _self);

        auto iterator = players.find(account);
        eosio_assert(iterator != players.end(), "Address for account not found");

        players.modify(iterator, account, [&](auto& player) {
            if (player.energy_points + purchased_item.power > 0) {
                player.energy_points += purchased_item.power; 
            }

            if (player.health_points + purchased_item.health > 0) {
                player.health_points += purchased_item.health;
            }

            if (player.level + purchased_item.level_up > 0) {
                player.level += purchased_item.level_up;
            }
            
            player.abilities.push_back(purchased_item.ability);
            player.inventory.push_back(item{
                purchased_item.item_id,
                purchased_item.name,
                purchased_item.power,
                purchased_item.health,
                purchased_item.ability,
                purchased_item.level_up
            });
        });

        print("Item Id: ", purchased_item.item_id);
        print(" | Name: ", purchased_item.name.c_str());
        print(" | Power: ", purchased_item.power); 
        print(" | Health: ", purchased_item.health);
        print(" | Ability: ", purchased_item.ability.c_str());
        print(" | Level up: ", purchased_item.level_up);
    }
}
