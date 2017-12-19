**Truffle实践应用**
-------------------------------
Truffle为以太坊提供了一个世界一流的开发环境、测试框架和资产管道(pipeline)，旨在使以太
坊开发更容易。 使用Truffle，你会得到：
● 内置智能合约编译，链接，部署和二进制字节码管理。
● 针对快速迭代开发的自动化合约测试。
● 可脚本化，可扩展的部署和迁移框架。
● 网络管理，用于部署到任意数量的公共和私有网络。
● 使用ERC190标准，使用EthPM和NPM进行包安装管理。
● 用于直接合约通信的交互式控制台。
● 支持持续集成的可配置构建管道。
● 外部脚本运行程序可以在Truffle环境中执行脚本。


1. 下载geth 并安装（版本1.7.3：https://gethstore.blob.core.windows.net/builds/geth-windows-amd64-1.7.3-4bb3c89d.exe）

2. 同步testnet的所有区块（非常耗时，极度需要耐心等待）
geth --testnet --syncmode "fast" --rpc --rpcapi db,eth,net,web3,personal --cache=1024  --rpcport 8545 --rpcaddr 127.0.0.1 --rpccorsdomain "*" --bootnodes "enode://20c9ad97c081d63397d7b685a412227a40e23c8bdc6688c6f37e97cfbc22d2b4d1db1510d8f61e6a8866ad7f0e17c02b14182d37ea7c3c8b9c2683aeb6b733a1@52.169.14.227:30303,enode://6ce05930c72abc632c58e2e4324f7c7ea478cec0ed4fa2528982cf34483094e9cbc9216e7aa349691242576d552a2a56aaeae426c5303ded677ce455ba1acd9d@13.84.180.240:30303"

3. 安装truffle（版本：4.0.0-beta.2）
npm install -g truffle@4.0.0-beta.2

4. 创建Voting项目
mkdir voting
cd voting

安装web-pack：
npm install -g webpack@2.2.1
truffle unbox webpack