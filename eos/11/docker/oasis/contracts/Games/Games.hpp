#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
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


    };

    EOSIO_ABI(Games, (startpk)(getpkresult));
}
