#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <string>

namespace Oasis {
    using namespace eosio;
    using std::string;

    class Games : public contract {
        using contract::contract;

        public:

            Games(account_name self):contract(self) {}

            //@abi action
            void startpk(const uint64_t id, string pkname, account_name player1, account_name player2);

            //@abi action
            void getpkresult(const uint64_t id);
            
            //@abi action
            void removepk(account_name account, const uint64_t id);
            
            inline int64_t rand();

            //@abi table pk i64
            struct pk {
                uint64_t pk_id;
                string pkname;
                account_name player1;
                account_name player2;
                account_name winner;
                uint64_t timestamp;  //uint64_t current_time()

                uint64_t primary_key() const { return pk_id; }

                EOSLIB_SERIALIZE(pk, (pk_id)(pkname)(player1)(player2)(winner)(timestamp))
            };

            typedef multi_index<N(pk), pk> pkIndex;

            //@abi table item i64
            struct item {
                uint64_t item_id;
                string name;
                uint64_t power;
                uint64_t health;
                string ability;
                uint64_t level_up;

                uint64_t primary_key() const { return item_id; }

                EOSLIB_SERIALIZE(item, (item_id)(name)(power)(health)(ability)(level_up))
            };

            //@abi table player i64
            struct player {
                uint64_t account_name;
                string username;
                uint64_t level;
                int64_t health_points = 1000;
                int64_t energy_points = 1000;
                vector<string> abilities;
                vector<item> inventory;

                uint64_t primary_key() const { return account_name; }

                EOSLIB_SERIALIZE(player, (account_name)(username)(level)(health_points)(energy_points)(abilities)(inventory))
            };

            typedef multi_index<N(player), player> playerIndex;
    };

    int64_t Games::rand() { // Refer to: https://github.com/generEOS/eosio.random/blob/master/random.cpp
        checksum256 result;
        auto mixedBlock = tapos_block_prefix() * tapos_block_num();

        const char *mixedChar = reinterpret_cast<const char *>(&mixedBlock);
        sha256( (char *)mixedChar, sizeof(mixedChar), &result);
        const char *p64 = reinterpret_cast<const char *>(&result);
        return int64_t(*p64);
    }

    EOSIO_ABI(Games, (startpk)(getpkresult)(removepk));
}
