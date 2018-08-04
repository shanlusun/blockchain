/* appdemo.cpp
*  Jack DiSalvatore
*/
#include <appdemo.hpp>

void appdemo::hello(account_name account) {
    print("Hello ", name{account});
}

void appdemo::create(const account_name account,
                     const string&      username,
                     uint32_t           age,
                     const string&      bio) {
    require_auth(account);

    profile_table profiles(_self, _self);

    auto itr = profiles.find(account);

    eosio_assert(itr == profiles.end(), "Account already exists");

    profiles.emplace(account, [&](auto& p) {
        p.account  = account;
        p.username = username;
        p.age      = age;
        p.bio      = bio;
    });
}

void appdemo::get(const account_name account) {
    profile_table profiles(_self, _self);

    auto itr = profiles.find(account);

    eosio_assert(itr != profiles.end(), "Account does not exist");

    print("Account: ", name{itr->account}, " , ");
    print("Username: ", itr->username.c_str(), " , ");
    print("Age: ", itr->age , " , ");
    print("Bio: ", itr->bio.c_str());
}

void appdemo::update(const account_name account,
                     const string&      username,
                     uint32_t           age,
                     const string&      bio) {
    require_auth(account);

    profile_table profiles(_self, _self);

    auto itr = profiles.find(account);

    eosio_assert(itr != profiles.end(), "Account does not exist");

    profiles.modify(itr, account, [&](auto& p) {
        p.username = username;
        p.age      = age;
        p.bio      = bio;
    });
}

void appdemo::remove(const account_name account) {
    require_auth(account);

    profile_table profiles(_self, _self);

    auto itr = profiles.find(account);

    eosio_assert(itr != profiles.end(), "Account does not exist");

    profiles.erase(itr);
    print(name{account} , " deleted!");
}

void appdemo::byage(uint32_t age) {
    print("Checking age: ", age, "\n");
    profile_table profiles(_self, _self);

    // get an interface to the 'profiles' containter
    // that looks up a profile by its age
    auto age_index = profiles.get_index<N(age)>();

    // lower_bound is an associative container operator
    // which returns an iterator to the first element that is
    // to lower then the key
    // we use lower bound, because we want to find everyone
    // who is this age, there may be multiple
    auto itr = age_index.lower_bound(age);

    for(; itr != age_index.end() && itr->age == age; ++itr) {
        print(itr->username.c_str(), " is ", itr->age, " years old\n");
    }
}

void appdemo::agerange(uint32_t young, uint32_t old) {
    profile_table profiles(_self, _self);

    auto age_index = profiles.get_index<N(age)>();

    auto begin = age_index.lower_bound(young);
    auto end   = age_index.upper_bound(old);

    for_each(begin, end, [&](auto& p) {
        print(p.username.c_str(), " is ", p.age, " years old\n");
    });
}