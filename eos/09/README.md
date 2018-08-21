**EOS实践课程(一)**
----------------------------------------------
如果同学们看过电影《头号玩家》，那么你会很容易理解我们将要实现的游戏，你也应该会相信未来的数字世界一定是基于区块链技术。

没错！我们要一起实现一个简单版本的“Oasis”（绿洲），这是一个基于区块链的游戏世界。从最基本的玩家属性设计开始，实现代币发放，虚拟道具交易，具体游戏场景等。
希望每位同学都能为这个项目做出贡献。

>build_release.sh升级
之前这个脚本只能给定合约的名字，合约需要在当前目录并且只有一层路径。
现在稍加改动之后可以使用合约路径（多层级路径，最后一级为合约名目录），使用方式如下：（其中Players目录为合约cpp文件所在目录）
```Bash
sh build_release.sh player ./oasis/contracts/Players
```

>游戏设计
主要包含以下模块：
* Player
* Market
* Product
* Game
* eosio.token OAS

>主要模块如下：
![图1](./images/module.jpeg "module")

>类定义如下：
![图2](./images/class.jpeg "class")

1. 创建一个新账户player
```Bash
cleos create account eosio player EOS8eHNwPjCvcQRnUP1feykKmKexWkRz5zXznK3GTJFPibut7kiaM EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai
```

2. 部署合约Player
```Bash
sh build_release.sh player ./oasis/contracts/Players
```

3. 调用合约添加player
```Bash
# cleos push action {account} {action_name} '{data}' -p {account}@active
cleos push action player add '["player","player_1"]' -p player@active
```


4. 查询Player
直接查看table：
```Bash
cleos get table player player player 

{
  "rows": [{
      "account_name": "12415831931030077440",
      "username": "player_1",
      "level": 1,
      "health_points": 1000,
      "energy_points": 1000,
      "abilities": [],
      "inventory": []
    }
  ],
  "more": false
}

```
调用合约Action：getplayer
```Bash
cleos push action player getplayer '["player"]' -p player@active

executed transaction: cf4f8d0df009e161f3c4052b477646790ad5f71806f6158db0a6399288f3c96b  104 bytes  1274 us
#        player <= player::getplayer            {"account":"player"}
>> Username: player_1 Level: 1 Health: 1000 Energy: 1000 No Abilities Empty inventory
```

5. 更新Player
```Bash
cleos push action player update '["player",10,65,110]' -p player@active
```
查看更新后的Player：
```Bash
cleos push action player getplayer '["player"]' -p player@active

executed transaction: f3623349ec91f7493d2e0f16bc9bcfa46a36484b14c18737317471528feaa612  104 bytes  1289 us
#        player <= player::getplayer            {"account":"player"}
>> Username: player_1 Level: 10 Health: 935 Energy: 890 No Abilities Empty inventory
```

6. 增加技能
```Bash
cleos push action player addability '["player","running"]' -p player@active
```
查看Player技能：
```Bash
cleos push action player getplayer '["player"]' -p player@active

executed transaction: 7781bc99fdd3a4141495586c9408151d0e4e5d692780d68b474012e5dd262ff1  104 bytes  1527 us
#        player <= player::getplayer            {"account":"player"}
>> Username: player_1 Level: 10 Health: 935 Energy: 890 Abilities: running  Empty inventory

```

7. 增加item
```Bash
cleos push action player additem '{"account":"player","purchased_item":{"item_id":1,"name":"knife","power":50,"health":300,"ability":"heal fast","level_up":0}}' -p player@active

executed transaction: f68d78e24e56cb53be226fd8e503fda09833f20f7dafc89feca0cf3fe24660f8  152 bytes  2295 us
#        player <= player::additem              {"account":"player","purchased_item":{"item_id":1,"name":"knife","power":50,"health":300,"ability":"...
>> Item Id: 1 | Name: knife | Power: 50 | Health: 300 | Ability: heal fast | Level up: 0
```
查看Player新的item：
```Bash
cleos push action player getplayer '["player"]' -p player@active

executed transaction: 30a1cdebf7136f3739d372cf16a1aa82918d9a096c2ab9582fd39db9be121324  104 bytes  2635 us
#        player <= player::getplayer            {"account":"player"}
>> Username: player_1 Level: 10 Health: 1235 Energy: 940 Abilities: running heal fast  Items: knife == 
```




**参考文献**
----------------------------------------------
1. 一个供参考的Oasis: https://github.com/dabdevelop/playerone
