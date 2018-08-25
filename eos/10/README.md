**EOS实践课程(二)**
----------------------------------------------
在上一节中，我们介绍了Player角色的设计和合约实现，也增加了Player的道具和技能，这一节我们将继续实现后续模块。

>游戏设计
主要包含以下模块：
* Player (已实现初步版本)
* Market
* Product
* Game
* eosio.token OAS

>主要模块如下：
![图1](./images/module.jpeg "module")

>类定义如下：
![图2](./images/class.jpeg "class")


在开始Marketplace的实现之前，我们需要先实现Oasis世界里的token，叫做'OAS'。
可以直接使用eos官方的[eosio.token合约](https://github.com/EOSIO/eos/tree/v1.0.8/contracts/eosio.token)。

**注意**：本节的实践交互基于上节的Player合约和创建的玩家，如果重启了本地测试网络，请重新执行上节09的实践命令，然后再继续本节。
```Bash
cleos create account eosio player EOS8eHNwPjCvcQRnUP1feykKmKexWkRz5zXznK3GTJFPibut7kiaM EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai

sh build_release.sh player ./oasis/contracts/Players

cleos create account eosio player1 EOS8eHNwPjCvcQRnUP1feykKmKexWkRz5zXznK3GTJFPibut7kiaM EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai

#添加player1
cleos push action player add '["player1","player1"]' -p player1@active

#查看player1
cleos push action player getplayer '["player1"]' -p player1@active

```

1. 创建一个新账户token
```Bash
cleos create account eosio token EOS8eHNwPjCvcQRnUP1feykKmKexWkRz5zXznK3GTJFPibut7kiaM EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai
```

2. 部署合约eosio.token
```Bash
sh build_release.sh token ./oasis/contracts/eosio.token
```
**注意**：我们这里使用的eosio.token代码与eos源码中的eosio.token代码不同，已经做了多处修改和调整。修改的原因是：源码原生eosio.token为系统代码，不能通过eosiocpp工具生成完整的abi文件，原生eosio.token需要使用cmake编译产生对应abi文件，因此也可以选择直接使用eos源码原生的eosio.token合约的abi。[参考文献2](https://github.com/EOSIO/eos/issues/4636)


3. 创建token:OAS
```Bash
cleos push action token create '{"issuer":"token","maximum_supply":"1000000.0000 OAS","can_freeze":"0","can_recall":"0","can_whitelist":"0"}' -p token@active
```

4. 给player账户转一些OAS
```Bash
# Issue tokens to account player and token
cleos push action token issue '{"to":"token","quantity":"5000.0000 OAS","memo":"init"}' -p token@active

cleos push action token issue '{"to":"player1","quantity":"5000.0000 OAS","memo":"init"}' -p token@active

```

5. 查看账户余额和eosio.token合约创建的table
```Bash
#通过api查询
cleos get currency balance token token OAS
5000.0000 OAS
cleos get currency balance token player1 OAS 
5000.0000 OAS
#通过table查询
cleos get table token player1 accounts

{
  "rows": [{
      "balance": "5000.0000 OAS"
    }
  ],
  "more": false
}

#查询OAS token状态
cleos get table token OAS stat

{
  "rows": [{
      "supply": "10000.0000 OAS",
      "max_supply": "1000000.0000 OAS",
      "issuer": "token"
    }
  ],
  "more": false
}
```


6. 创建market账户
```Bash
cleos create account eosio market EOS8eHNwPjCvcQRnUP1feykKmKexWkRz5zXznK3GTJFPibut7kiaM EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai
```

7. 部署Marketplace合约
```Bash
sh build_release.sh market ./oasis/contracts/Marketplace
```

8. 创建新的Product
```Bash
cleos push action market add '{"account":"market","newProduct":{"product_id":1,"name":"magic ball","power":120,"health":10,"ability":"see the future","level_up":3,"quantity":10,"price":150}}' -p market@active

executed transaction: d1870a33e1007d7df6a004083b944cb65039948c4275f0d6f366541d082f9da4  176 bytes  3141 us
#        market <= market::add                  {"account":"market","newProduct":{"product_id":1,"name":"magic ball","power":120,"health":10,"abilit...
```

9. 使用ID查看Product
```Bash
cleos push action market getbyid '[1]' -p market@active

executed transaction: 4191cd3b38c6bcebf28e7b1462f070ef9368c9263c88ca43e3e86ef4b358c9b4  104 bytes  1544 us
#        market <= market::getbyid              {"productId":1}
>> Id: 1 | Name: magic ball | Power: 120 | Health: 10 | Ability: see the future | Level up: 3 | Quantity: 10 | Price: 150
```

10. 再增加一个Product
```Bash
cleos push action market add '{"account":"market","newProduct":{"product_id":2,"name":"potion","power":50,"health":300,"ability":"heal fast","level_up":0,"quantity":4,"price":350}}' -p market@active

executed transaction: 2c84840cb48e5f007e3cf8d4de08df439778689c1f36c7acfde3b972d207528b  168 bytes  3089 us
#        market <= market::add                  {"account":"market","newProduct":{"product_id":2,"name":"potion","power":50,"health":300,"ability":"...
```

11. 按照ID更新Product
```Bash
# Update potion quantity to 7
cleos push action market update '["market",2,3]' -p market@active
```
查看更新后的Product：
```Bash
cleos push action market getbyid '[2]' -p market@active

executed transaction: 28770407080941e1b10bc0bfed8615930c9c2d0d1ca14494d4254a654499774a  104 bytes  1267 us
#        market <= market::getbyid              {"productId":2}
>> Id: 2 | Name: potion | Power: 50 | Health: 300 | Ability: heal fast | Level up: 0 | Quantity: 7 | Price: 350
```

12. 从Marketplace买东西
```Bash
# Buy product from Marketplace
cleos push action market buy '["player1",1]' -p player1@active

Error 3090003: provided keys, permissions, and delays do not satisfy declared authorizations
Ensure that you have the related private keys inside your wallet and your wallet is unlocked.

#nodeos节点日志：
3090003 unsatisfied_authorization: provided keys, permissions, and delays do not satisfy declared authorizations
transaction declares authority '{"actor":"player1","permission":"active"}', but does not have signatures for it under a provided delay of 0 ms, provided permissions [{"actor":"market","permission":"eosio.code"}], and provided keys []
    {"auth":{"actor":"player1","permission":"active"},"provided_delay":0,"provided_permissions":[{"actor":"market","permission":"eosio.code"}],"provided_keys":[],"delay_max_limit_ms":3888000000}
    thread-0  authorization_manager.cpp:409 check_authorization

    {"_pending_console_output.str()":"buy,self is: marketId: 1 | Name: magic ball | Power: 120 | Health: 10 | Ability: see the future | Level up: 3 | Quantity: 8 | Price: 150"}
    thread-0  apply_context.cpp:62 exec_one
```


13. 将player1账户授予market合约active的权限
```Bash
cleos set account permission player1 active '{"threshold": 1,"keys": [{"key": "EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai","weight": 1}],"accounts": [{"permission":{"actor":"market","permission":"eosio.code"},"weight":1}]}' owner -p player1@owner
```

14. 重试购买商品
```Bash
# Buy product from Marketplace
cleos push action market buy '["player1",1]' -p player1@active

executed transaction: dda5dd36ebaab05d0b1eba30367444baab301211860546477fac5d4780b321d6  112 bytes  14174 us
#        market <= market::buy                  {"buyer":"player1","productId":1}
>> buy function,self is: market | Id: 1 | Name: magic ball | Power: 120 | Health: 10 | Ability: see the future | Level up: 3 | Quantity: 7 | Price: 150
#         token <= token::transfer              {"from":"player1","to":"market","quantity":"0.0150 OAS","memo":"buyTest"}
#       player1 <= token::transfer              {"from":"player1","to":"market","quantity":"0.0150 OAS","memo":"buyTest"}
#        market <= token::transfer              {"from":"player1","to":"market","quantity":"0.0150 OAS","memo":"buyTest"}
#        player <= player::additem              {"account":"player1","purchased_item":{"item_id":1,"name":"magic ball","power":120,"health":10,"abil...
>> Item Id: 1 | Name: magic ball | Power: 120 | Health: 10 | Ability: see the future | Level up: 3

```

15. 查看player1的变化
```Bash
#账户余额变化
cleos get currency balance token player1 OAS
4999.9850 OAS

#查看背包装备变化(Abilities: see the future,  Items: magic ball)
cleos push action player getplayer '["player1"]' -p player1@active

executed transaction: 2a883770499e72479e75b2e2482bb5ab645ca6823bc8ef804a592bae78e3e61a  104 bytes  3024 us
#        player <= player::getplayer            {"account":"player1"}
>> Username: player1 Level: 4 Health: 1010 Energy: 1120 Abilities: see the future,  Items: magic ball == 

#查看产品库存(Quantity: 9)
cleos push action market getbyid '[1]' -p market@active

executed transaction: a0c1b4fbe733b5a14c0f58a37804edc42eed5db52634e656f379a5b4e3e206d2  104 bytes  4532 us
#        market <= market::getbyid              {"productId":1}
>> Id: 1 | Name: magic ball | Power: 120 | Health: 10 | Ability: see the future | Level up: 3 | Quantity: 9 | Price: 150
1086307ms thread-0   main.cpp:382                  print_result         ]

```



**参考文献**
----------------------------------------------
1. 一个供参考的Oasis: https://github.com/dabdevelop/playerone
2. eosio.token合约问题: https://github.com/EOSIO/eos/issues/4636 https://github.com/EOSIO/eos/issues/3471 

