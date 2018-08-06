**06-EOS数据库应用**
----------------------------------------------
上一节的内容介绍了简单的智能合约样例，同时大家应该也学会了简单合约的编译、部署、增加权限控制。那么，如果我们定义复杂的智能合约，需要依赖复杂的数据结构时候，应该如何实现呢？EOS提供了支持多索引的数据库，来满足智能合约对复杂数据类型的读写需求。

另外，上一节作业中，让大家研究eosio.token合约，详细可以[参考这里](https://medium.com/coinmonks/understanding-the-eosio-token-contract-87466b9fdca9)

**编译部署脚本**

为了提升开发效率，准备了一个简单的脚本：[build_release.sh](./docker/build_release.sh)

此脚本，接受两个参数，分别是：
* 部署合约的账户
* 合约cpp、hpp文件所在的目录（当然，此目录是在docker container中的目录）
比如，我们介绍的hello合约，编译、部署时候可以直接执行此脚本如下：
```Bash
sh build_release.sh eosio /hello
```
**注意**：此脚本不会帮助检查输入的合约目录是否存在，感兴趣同学可以增加此判断，如果合约目录不存在，即使用‘eosiocpp -n’来创建默认sample合约。

部署之后，可以做后续的合约调用。


**如何升级非系统合约**

如果同一个合约有代码变动，只需要重新编译、部署即可升级，注意：如果代码没有任何改动重新部署是没必要的，也会提示此合约代码已经存在的错误。



**EOS的数据库实现**
----------------------------------------------

EOS为了方便智能合约与数据库的交互，参考了[Boost库中的 Multi-Index Containers](https://www.boost.org/doc/libs/1_66_0/libs/multi_index/doc/index.html)，并实现了‘eosio::multi_index’类。

EOS的数据库，按照如下层级空间管理的：
 > - **code** - the account name which has write permission
 >    - **scope** - an area where the data is stored
 >       - **table** - a name for the table that is being stored
 >          - **record** - a row in the table

[参考](https://developers.eos.io/eosio-cpp/docs/multi-index-table-tutorial)
[源代码](https://github.com/EOSIO/eos/blob/v1.0.8/contracts/eosiolib/db.h)


**multi_index类模板**

代码路径：eos/contracts/eosiolib/multi_index.hpp    line:154
```c++
template<uint64_t TableName, typename T, typename... Indices>
class multi_index {
    ...
}
```
* 第一个参数是表名
* 第二个参数是表对象的类型
* 其余为可变参数Indices（二级索引），二级索引的数量不能超过16个
具体example用法可以参考源代码中给出的例子。


**multi_index构造函数**

代码路径：eos/contracts/eosiolib/multi_index.hpp  line:644
```c++
multi_index( uint64_t code, uint64_t scope )
:_code(code),_scope(scope),_next_primary_key(unset_next_primary_key)
{}
```

multi_index构造函数有两个参数：code和scope，都是uint64_t类型，可以用来为表建立访问权限。

* code：拥有这张multi_index表的账户，该账户拥有对合约数据的读写权限
* scope：code层级内的范围标识符


**多索引数据库的智能合约样例**

>TODO
更多可参考[官网原文](https://eosio-cpp.readme.io/docs/multi-index-table-tutorial)

**C++ 11引入的一些特性**

在线执行环境：https://en.cppreference.com/w/cpp/language/auto 
```c++
/*
 * cppprimer.cpp
 *
 *  Created on: 2013.11.7
 *      Author: Caroline
 */
 
#include <iostream>
#include <utility>

/*
Lambda函数可以引用在它之外声明的变量. 这些变量的集合叫做一个闭包. 
闭包被定义在Lambda表达式声明中的方括号[]内. 
这个机制允许这些变量被按值或按引用捕获.下面这些例子就是:  

[]        //未定义变量.试图在Lambda内使用任何外部变量都是错误的.
[x, &y]   //x 按值捕获, y 按引用捕获.
[&]       //用到的任何外部变量都隐式按引用捕获
[=]       //用到的任何外部变量都隐式按值捕获
[&, x]    //x显式地按值捕获. 其它变量按引用捕获
[=, &z]   //z按引用捕获. 其它变量按值捕获

*/

int main (void) {
	int i = 42;
	int &lr = i;
	int &&rr = i*42;
	const int &lr1 = i*42;
	int &&rr1 = 42;
	int &&rr2 = std::move(lr);
	std::cout << "i = " << i << std::endl;
	std::cout << "lr = " << lr << std::endl;
	std::cout << "rr = " << rr << std::endl;
	std::cout << "lr1 = " << lr1  <<std::endl;
	std::cout << "rr1  = " << rr1  << std::endl;
	std::cout << "rr2  = " << rr2  << std::endl;
	
	std::cout << "lr  address  = " << &lr  << std::endl;
	std::cout << "rr2 address  = " << &rr2  << std::endl;
	
	int val = 10;
	auto lambda = [&] () -> int { return val * 100; };
	std::cout << "lambda  = " << lambda()  << std::endl;	
}

```
关于lvalue和rvalue的其他[参考](https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers)。


1. **生成wast和abi文件，并部署合约**
```Bash
sh build_release.sh eosio /appdemo
```

2. **执行合约方法**
```Bash
cleos push action eosio create '["eosio","sunny","20","My bio"]' -p eosio@active
```

3. **操作table**

>查看table
```Bash
$cleos get table eosio eosio profile

{
  "rows": [{
      "account": "eosio",
      "username": "sunny",
      "age": 20,
      "bio": "My bio"
    }
  ],
  "more": false
}
```

>调用get方法
```Bash
$cleos push action eosio get '["eosio"]' -p eosio@active

executed transaction: c188b7d7a122e14f028e2d14780f9485a20b6bf02a1daa673c9c0e6e2c509f1f  104 bytes  1461 us
#         eosio <= eosio::get                   {"account":"eosio"}
>> Account: eosio , Username: sunny , Age: 20 , Bio: My bio
warning: transaction executed locally, but may not be confirmed by the network yet
```

>调用byage方法，使用自定义index
```Bash
$cleos push action eosio byage '["20"]' -p eosio@active

executed transaction: d28f565ba80ef36b67803cadcd2d6d63837b05d5026a79c134bff2d4f88fa1bb  96 bytes  1177 us
#         eosio <= eosio::byage                 {"age":15}
>> Checking age: 20
warning: transaction executed locally, but may not be confirmed by the network yet
```
查看nodeosd的日志如下：
```Bash
[(eosio,byage)->eosio]: CONSOLE OUTPUT BEGIN =====================
Checking age: 20
sunny is 20 years old

```

>调用agerange方法，使用自定义index
```Bash
$cleos push action eosio agerange '["15","25"]' -p eosio@active

executed transaction: e250bf8d07ee996b951e303db47d5b33d735c3a8d10db9d1ce76455bff0fe8c8  104 bytes  3749 us
#         eosio <= eosio::agerange              {"young":15,"old":25}
>> sunny is 20 years old
warning: transaction executed locally, but may not be confirmed by the network yet
```



**参考文献**
----------------------------------------------
1. BM发表多索引数据库支持：https://medium.com/@bytemaster/eosio-development-update-272198df22c1 
2. EOS 官方文档：https://developers.eos.io/eosio-cpp/docs/file-structure  https://eosio-cpp.readme.io/docs/multi-index-table-tutorial  https://developers.eos.io/eosio-cpp/docs/hello-world 
3. EOS Database：https://medium.com/fueled-engineering/exploring-the-eos-multi-index-database-557769b1b7a6 
4. EOS 智能合约与多索引数据库（视频）: https://www.youtube.com/watch?v=PamSV-WGcZo
5. EOS 数据库架构： https://eosfans.io/topics/410  http://www.codeblogbt.com/archives/155917
6. Boost Multi-index Containers Library： https://www.boost.org/doc/libs/1_66_0/libs/multi_index/doc/index.html
7. C++ 特性参考：
   >Universal References in C++11: https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers
   https://stackoverflow.com/questions/5481539/what-does-t-double-ampersand-mean-in-c11 
   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1690.html
   >auto关键字：https://en.cppreference.com/w/cpp/language/auto
   >lambda: https://blog.csdn.net/wxn704414736/article/details/78603997




