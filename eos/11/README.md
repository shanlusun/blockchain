**EOS实践课程(三)**
----------------------------------------------
在上一节中，我们介绍了Marketplace合约、Product的定义，并且发布了token：OAS，实践了player购买product的buy方法，但是遇到一些问题，课后已经更新10课程的说明文档，所有问题均已解决。


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



**Marketplace合约需要改进**

1. 库存更新时候，如果给过大的负数会溢出
```Bash
#ID=2的产品默认库存为10，如果更新时候给一个'-11'的更新即会溢出，得到一个巨大数额库存
cleos push action market update '["market",2,-11]' -p market@active

cleos push action market getbyid '[2]' -p market@active

executed transaction: a26c9bc6b2c9fedd16a51edc6ee9346ca1c8b7be60fc1a8800dbbb1a6ab50ab1  104 bytes  1874 us
#        market <= market::getbyid              {"productId":2}
>> Id: 2 | Name: potion | Power: 50 | Health: 300 | Ability: heal fast | Level up: 0 | Quantity: 18446744073709551615 | Price: 350
1997717ms thread-0   main.cpp:382                  print_result         ]
```
>解决办法：
更改Marketplace合约中quantity类型为有符号整数，并在更新库存前增加判断条件。注意：product table的struct定义中也需要更新类型定义，并且需要重新部署合约。

>更新合约后：
```Bash
cleos push action market update '["market",1,-11]' -p market@active

executed transaction: b7ba8ce6901e33a8dc04ebb9025445edca1884d8b8b4672f03131a479c088b46  120 bytes  1766 us
#        market <= market::update               {"account":"market","product_id":1,"quantity":-14}
>>  Can not update, target quantity is: -1
```


**Games合约**
考虑Games合约如何获取Players合约中的player数据？ 
下一小节会给出实现方式。





**参考文献**
----------------------------------------------
1. 一个供参考的Oasis: https://github.com/dabdevelop/playerone https://github.com/oasis-eos/oasis
2. 合约随机数：https://eosio-cpp.readme.io/docs/random-number-generation
https://github.com/generEOS/eosio.random/blob/master/random.cpp


