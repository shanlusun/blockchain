**Truffle Debug Sample**
-------------------------------
注意：truffle 版本需要高于version 4.0 推荐使用：truffle@4.1.3

1. 创建目录，初始化truffle
```Bash
mkdir simple-storage
cd simple-storage
truffle init
```
会生成contracts/ 和migrations/ 目录

2. 在contracts/ 目录, 创建一个合约文件 Store.sol 内容如下:
```javascript
pragma solidity ^0.4.17;

contract SimpleStorage {
  uint myVariable;

  function set(uint x) public {
    while(true) {
      myVariable = x;
    }
  }

  function get() constant public returns (uint) {
    return myVariable;
  }
}
```
3. 在migrations/ 目录, 创建文件2_deploy_contracts.js 内容为:
```javascript
var SimpleStorage = artifacts.require("SimpleStorage");

module.exports = function(deployer) {
  deployer.deploy(SimpleStorage);
};
```
4. 在终端命令行编译合约
```Bash
truffle compile
```

5. 运行 truffle develop 启动testrpc来测试我们的新合约:
```Bash
truffle develop
```

6. 在truffle console里部署合约
```Bash
truffle(develop)>migrate
```
输出信息如下：
```Bash
truffle(develop)> migrate
Using network 'develop'.

Running migration: 1_initial_migration.js
  Deploying Migrations...
  ... 0xa012f9b51087d213e319c08d748a64b98207813ad645a2283a823aca6795870a
  Migrations: 0x8cdaf0cd259887258bc13a92c0a6da92698644c0
Saving successful migration to network...
  ... 0xd7bc86d31bee32fa3988f1c1eabce403a1b5d570340a3a9cdba53a472ee8c956
Saving artifacts...
Running migration: 2_deploy_contracts.js
  Deploying SimpleStorage...
  ... 0x88cb350d1d3401eb353a8033eb1b8b5508b9f73e6bc4c7bae20d97c8849dcda5
  SimpleStorage: 0x345ca3e014aaf5dca488057592ee47305d9b3e10
Saving successful migration to network...
  ... 0xf36163615f41ef7ed8f4a8f192149a0bf633fe1a2398ce001bf44c43dc7bdda0
Saving artifacts...
```
**注意**：如果退出truffle console，重新进入需要再次运行：migrate --reset

7. 与合约交互：

* 查询存储的值：
```Bash
truffle(develop)> SimpleStorage.deployed().then(function(instance){return instance.get.call();}).then(function(value){return value.toNumber()});

0
```

* 设置存储的值：
```Bash
truffle(develop)> SimpleStorage.deployed().then(function(instance){return instance.set(4);});

Error: VM Exception while processing transaction: out of gas
    at Object.InvalidResponse (C:\Users\sunny\AppData\Roaming\npm\node_modules\truffle\build\webpack:\~\web3\lib\web3\errors.js:38:1)
    at ...
       ...
    (internal/process/next_tick.js:138:11)
    at process._tickDomainCallback (internal/process/next_tick.js:218:9)

```
这是一个transaction，但是输出错误信息如下：
**Error: VM Exception while processing transaction: out of gas** 

* 再次获取存储的值：
SimpleStorage.deployed().then(function(instance){return instance.get.call();}).then(function(value){return value.toNumber()});

8. 启动truffle develop日志终端
另打开一个新的终端，在实验目录运行：
```Bash
$ truffle develop --log

Connected to existing Truffle Develop session at http://127.0.0.1:9545/

```

9. 复现问题，debug问题
* 设置存储的值：
```Bash
truffle(develop)> SimpleStorage.deployed().then(function(instance){return instance.set(4);});
```
* 查看truffle 日志终端：
```Bash
2018-03-27T06:03:20.668Z develop:testrpc eth_sendTransaction
2018-03-27T06:03:23.560Z develop:testrpc
2018-03-27T06:03:23.560Z develop:testrpc   **Transaction: 0x8a7d3343dd2aaa0438157faae678ca57cc6485825bb4ed2ebefe90609dd268ce**
2018-03-27T06:03:23.560Z develop:testrpc   Gas usage: 6721975
2018-03-27T06:03:23.560Z develop:testrpc   Block Number: 5
2018-03-27T06:03:23.560Z develop:testrpc   Block Time: Tue Mar 27 2018 14:03:20 GMT+0800 (中国标准时间)
2018-03-27T06:03:23.560Z develop:testrpc   Runtime Error: out of gas
2018-03-27T06:03:23.560Z develop:testrpc
```

* debug问题：truffle debug <Transaction ID>
```Bash
truffle(develop)> debug 0x8a7d3343dd2aaa0438157faae678ca57cc6485825bb4ed2ebefe90609dd268ce
```

终端输出如下：
```Bash
Gathering transaction data...

Addresses affected:
 0x345ca3e014aaf5dca488057592ee47305d9b3e10 - SimpleStorage

Commands:
(enter) last command entered (step next)
(o) step over, (i) step into, (u) step out, (n) step next
(;) step instruction, (p) print instruction, (h) print this help, (q) quit
(b) toggle breakpoint, (c) continue until breakpoint
(+) add watch expression (`+:<expr>`), (-) remove watch expression (-:<expr>)
(?) list existing watch expressions
(v) print variables and values, (:) evaluate expression - see `v`


Store.sol:

1: pragma solidity ^0.4.17;
2:
3: contract SimpleStorage {
   ^^^^^^^^^^^^^^^^^^^^^^^^

debug(develop:0xd7ff6d16...)>
```

可以按照命令，单步执行调试，并print堆栈信息。