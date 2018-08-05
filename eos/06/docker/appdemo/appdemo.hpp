/* appdemo.hpp
*  Jack DiSalvatore
*/
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

#include <string>

using namespace eosio;
using std::string;

class appdemo : public contract {
public:
    using contract::contract;

    appdemo(account_name self)
            : contract(self) {}

    // @abi action
    void hello(const account_name account);

    // @abi action
    void create(const account_name account,
                const string&      username,
                uint32_t           age,
                const string&      bio);

    // @abi action
    void get(const account_name account);

    // @abi action
    void update(const account_name account,
                const string&      username,
                uint32_t           age,
                const string&      bio);

    // @abi action
    void remove(const account_name account);

    // @abi action
    void byage(uint32_t age);

    // @abi action
    void agerange(uint32_t young, uint32_t old);

private:
    // @abi table profile i64
    struct profile {
        account_name    account;
        string          username;
        uint32_t        age;
        string          bio;

        account_name primary_key() const { return account; }
        uint64_t     by_age() const { return age; }

        EOSLIB_SERIALIZE(profile, (account)(username)(age)(bio))
    };

    typedef eosio::multi_index< N(profile), profile,
            // N(name of interface)
            indexed_by< N(age),
                        const_mem_fun<profile, uint64_t, &profile::by_age>
            >
    > profile_table;

};

EOSIO_ABI(appdemo, (hello)(create)(get)(update)(remove)(byage)(agerange))