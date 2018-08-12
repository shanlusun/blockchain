**08-eosjs SDK介绍**
----------------------------------------------
前几节课着重介绍了EOS的智能合约，大家也许会了通过cleos命令调用合约，但是在实际应用场景客户端需要集成SDK的方式与合约交互，因此EOS也提供了客户端SDK：eosjs

eosjs与以太坊的web3.js都是SDK，可以说要想在EOS平台做开发，那么一定要学会如何使用eosjs。

>安装nodejs
开始之前，大家注意要预先安装nodejs和npm，可以选择比较新的版本即可。
我本机使用的版本如下：
```Bash
$node -v

v10.6.0
```

```Bash
$npm -v

6.1.0
```
>IDE推荐使用WebStorm

**eosjs-samples**
接下来的步骤从零开始建立project，使用eosjs。

1. 创建新的project
```Bash
mkdir eosjs-samples && cd eosjs-samples

npm init
```

2. 安装eosjs
```Bash
npm install eosjs

```

3. 构造eosjs config


4. 调用[eosjs的API接口](https://github.com/EOSIO/eosjs-api/blob/master/docs/api.md#eos--object)








nodeos的 [RESTful API参考](https://developers.eos.io/eosio-nodeos/reference)






**参考文献**
----------------------------------------------
1. eosjs github: https://github.com/EOSIO/eosjs

2. eosjs API: https://github.com/EOSIO/eosjs-api/blob/master/docs/api.md#eos--object 

3. nodeos API: https://developers.eos.io/eosio-nodeos/reference 

4. eosjs API demos: https://medium.com/coinmonks/how-to-use-eosjs-api-1-770b037b22ad
