#include <eosiolib/eosio.hpp>
using namespace eosio;
using std::string;

class hello : public eosio::contract {
  public:
      using contract::contract;

      /// @abi action 
      void hi( account_name user, account_name target ) {
         require_auth( user );
         print( "Hello, from:", name{user}, ", target:", name{target});

         //action所带参数具体如下：
         //action(permssion_level, other_contract_account_name, method, args).send();
         action(
             //这里{to, active}必须授权给当前合约{_self, eosio.code}，
             //当前合约才能使用to账户的active权限调用其他合约
             permission_level{ target, N(active) },
             N(eosio), N(callme),
             std::make_tuple(target)
         ).send();

         action(
             permission_level{ target, N(active) },
             N(eosio), N(create),
             std::make_tuple(target, string("hello contract"), 20, string("hello bio"))
         ).send();
      }
};

EOSIO_ABI( hello, (hi) )
