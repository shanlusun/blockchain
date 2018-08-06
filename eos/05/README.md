**05-EOS智能合约介绍**
----------------------------------------------
这里提到的数据库指的是EOS智能合约访问的存储，由于存储的使用需要了解智能合约，为了实践的考虑，我们先简单介绍智能合约。

**eosiocpp工具**

eosiocpp 是一个 C++ 生成 WASM 和 ABI 文件的编译工具，有很多依赖库，因此需要在eosio/eos-dev 的环境中才可以运行。
主要功能：
* -g 生成abi文件: cpp->abi  **注意**：不要使用hpp文件生成abi
* -o 生成wast文件: cpp->wast
* -n 生成一个默认的合约sample

**添加常用的命令alias**

将常用的工具命令设定alias，添加到'.bash_profile'中:
```Bash
#LOCAL EOS COMMANDS
alias cleos='docker-compose -f docker-compose-local-eosio1.0.yaml exec keosd /opt/eosio/bin/cleos -u http://nodeosd:8888 --wallet-url http://localhost:8900'
alias eosiocpp='docker-compose -f docker-compose-local-eosio1.0.yaml exec keosd /opt/eosio/bin/eosiocpp'
```
后续新工具可以继续追加alias定义。


**什么是ABI？**
>ABI是Application Binary Interface的缩写，它的意思是程序二进制接口。 一个ABI是不同二进制片段的连接纽带。 它定义了函数被调用的规则：参数在调用者和被调用者之间如何传递，返回值怎么提供给调用者，库函数怎么被应用，以及程序怎么被加载到内存。 因此ABI是用来约束链接器的：一个ABI是无关的代码如何在一起工作的规则。

**注意**：EOS中的ABI出了约束action之外，还可以约束table，详细[可参考](https://medium.com/fueled-engineering/exploring-the-eos-multi-index-database-557769b1b7a6)


接下来开始合约之旅：
1. **创建第一个智能合约**
```Bash
eosiocpp -n hello
```

/opt/eosio/bin/data-dir/contracts

执行之后会在，nodeosd的container根目录建立hello目录，并包含cpp和hpp文件：
```Bash
docker exec -it docker_nodeosd_1 ls /hello
```

2. **生成wast和abi文件**
```Bash
eosiocpp -g /hello/hello.abi /hello/hello.cpp 

eosiocpp -o /hello/hello.wast /hello/hello.cpp 
```

3. **部署合约**

```Bash
cleos set contract eosio /hello/ /hello/hello.wast /hello/hello.abi -p eosio@active
```
**注意**：如果有如下类似提示，说明需要先unlock wallet，然后重试：
```Bash
Reading WAST/WASM from /hello/hello.wast...
Assembling WASM...
Publishing contract...
Error 3120006: No available wallet
Ensure that you have created a wallet and have it open
```

4. **调用合约方法**

```Bash
cleos push action eosio hi '["user"]' -p eosio@active 
```

5. **合约权限验证**

在‘hi’方法中增加权限检查：
```cpp
void hi( account_name user ) {
   require_auth( user );
   print( "Hello, ", name{user} );
}
```
再次调用方法‘hi’：
```Bash
$cleos push action eosio hi '["user"]' -p eosio@active

Error 3090004: missing required authority
Ensure that you have the related authority inside your transaction!;
If you are currently using 'cleos push action' command, try to add the relevant authority using -p option.
```

正确的调用方式是：

```Bash
$cleos push action eosio hi '["eosio"]' -p eosio@active

executed transaction: 6ed1c553b8877791e238b4990d85e3295761e1b9b6bde9639d3613b9666f4edd  104 bytes  940 us
#         eosio <= eosio::hi                    {"user":"eosio"}
>> Hello, eosio
warning: transaction executed locally, but may not be confirmed by the network yet
```




**参考文献**
----------------------------------------------
1. BM发表多索引数据库支持：https://medium.com/@bytemaster/eosio-development-update-272198df22c1 
2. EOS 官方文档：https://developers.eos.io/eosio-cpp/docs/file-structure  https://eosio-cpp.readme.io/docs/multi-index-table-tutorial  https://developers.eos.io/eosio-cpp/docs/hello-world 
3. EOS Database：https://medium.com/fueled-engineering/exploring-the-eos-multi-index-database-557769b1b7a6 
4. EOS 智能合约与多索引数据库（视频）: https://www.youtube.com/watch?v=PamSV-WGcZo
5. NSJAMES的教程参考：https://github.com/nsjames/Scatter-Tutorials/blob/master/basics.md  https://www.youtube.com/watch?v=EbWDHrm2ETY 



