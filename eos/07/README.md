**07-智能合约介绍(二)**
----------------------------------------------
在智能合约平台中，合约之间的调用十分重要，这种调用关系给智能合约带来了很多灵活性和开放性。EOS同样支持合约之间的调用，并且有权限控制，这一小节一起来实践一下合约的调用情况。

**编译部署脚本**

编译部署脚本已修复(一个路径问题)：[build_release.sh](./docker/build_release.sh)


**合约调用其他合约**

首先，在开始前我们需要创建一些账户，这些账户在后续操作中会提现出不同权限的调用情况不同。
```Bash
cleos create account eosio hello EOS8eHNwPjCvcQRnUP1feykKmKexWkRz5zXznK3GTJFPibut7kiaM EOS8eHNwPjCvcQRnUP1feykKmKexWkRz5zXznK3GTJFPibut7kiaM

cleos create account eosio caller EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai

cleos create account eosio target EOS87VhatKMMjL7SKLnaDAEG2ED3jp7Y6moh3XrLNwiyHdrYGWdXD EOS87VhatKMMjL7SKLnaDAEG2ED3jp7Y6moh3XrLNwiyHdrYGWdXD

cleos create account eosio test EOS7n2SbswS5Nwakbc79JUxLy5hTXq7de6L7p63yRrMKdMBjxFtQz EOS7n2SbswS5Nwakbc79JUxLy5hTXq7de6L7p63yRrMKdMBjxFtQz

```
**注意**：用到的keys，都需提前创建好、记录好私钥，并import到Wallet中，因此大家使用的key的公钥会与上面的不同，请注意替换成自己创建的keys。

1. 分别部署hello合约和appdemo合约

在docker目录下执行：
```Bash
sh build_release.sh eosio /appdemo

sh build_release.sh hello /hello
```

2. 使用hello合约中的hi方法

```Bash
cleos push action hello hi '["caller", "target"]' -p caller
             
Error 3090003: provided keys, permissions, and delays do not satisfy declared authorizations
Ensure that you have the related private keys inside your wallet and your wallet is unlocked.
```
>日志错误如下：
```Bash
3554229ms thread-0   http_plugin.cpp:406           handle_exception     ] Exception Details: 3090003 unsatisfied_authorization: provided keys, permissions, and delays do not satisfy declared authorizations
transaction declares authority '{"actor":"target","permission":"active"}', but does not have signatures for it under a provided delay of 0 ms, provided permissions [{"actor":"hello","permission":"eosio.code"}], and provided keys []
    {"auth":{"actor":"target","permission":"active"},"provided_delay":0,"provided_permissions":[{"actor":"hello","permission":"eosio.code"}],"provided_keys":[],"delay_max_limit_ms":3888000000}
    thread-0  authorization_manager.cpp:409 check_authorization

    {"_pending_console_output.str()":"Hello, from:caller, target:target"}
    thread-0  apply_context.cpp:62 exec_one
```

这里遇到错误是因为，target账户并没有授权给hello合约去直接调用appdemo合约。

3. 让target账户授权给hello合约特定权限

```Bash
hello合约 -> target账户 -> appdemo合约
   |            |
   <- - - - - - -
        授权
```

```Bash
cleos set account permission target active '{"threshold": 1,"keys": [],"accounts": [{"permission":{"actor":"hello","permission":"eosio.code"},"weight":1}]}' owner -p target@owner
```
>执行后可以查看target账户变化
```Bash
cleos get account target -j
```

4. 重新调用hello合约

```Bash
cleos push action hello hi '["caller", "target"]' -p caller

```
>查看appdemo中的profile表的变化
```Bash
cleos get table eosio eosio profile

{
  "rows": [{
      "account": "target",
      "username": "hello contract",
      "age": 20,
      "bio": "hello bio"
    }
  ],
  "more": false
}
```

**注意**：由于我们在hello合约中调用了appdemo中的create方法，因此多次调用会报错：
```Bash
cleos push action hello hi '["caller", "target"]' -p caller@active

Error 3050003: eosio_assert_message assertion failure
```

日志如下：

```Bash
1491136ms thread-0   http_plugin.cpp:406           handle_exception     ] Exception Details: 3050003 eosio_assert_message_exception: eosio_assert_message assertion failure
assertion failure with message: Account already exists
    {"s":"Account already exists"}
    thread-0  wasm_interface.cpp:930 eosio_assert

    {"_pending_console_output.str()":""}
    thread-0  apply_context.cpp:62 exec_one
```








**参考文献**
----------------------------------------------
1. EOS 合约调用(视频)：https://www.youtube.com/watch?v=E3Tx2DseLGE&feature=youtu.be&t=1h03m50s 

2. 合约调用权限管理：https://blog.csdn.net/itleaks/article/details/80535318
https://www.jianshu.com/p/8bd118dc865b
