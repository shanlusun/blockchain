**Truffle实践应用**
-------------------------------
Truffle为以太坊提供了一个世界一流的开发环境、测试框架和资产管道(pipeline)，旨在使以太
坊开发更容易。 使用Truffle，你会得到：
* 内置智能合约编译，链接，部署和二进制字节码管理。
* 针对快速迭代开发的自动化合约测试。
* 可脚本化，可扩展的部署和迁移框架。
* 网络管理，用于部署到任意数量的公共和私有网络。
* 使用ERC190标准，使用EthPM和NPM进行包安装管理。
* 用于直接合约通信的交互式控制台。
* 支持持续集成的可配置构建管道。
* 外部脚本运行程序可以在Truffle环境中执行脚本。


1. 下载geth 并安装版本[1.7.3](https://gethstore.blob.core.windows.net/builds/geth-windows-amd64-1.7.3-4bb3c89d.exe)

2. 同步testnet的所有区块（非常耗时，极度需要耐心等待）
```Bash
geth --testnet --syncmode "fast" --rpc --rpcapi db,eth,net,web3,personal --cache=1024  --rpcport 8545 --rpcaddr 127.0.0.1 --rpccorsdomain "*" --bootnodes "enode://20c9ad97c081d63397d7b685a412227a40e23c8bdc6688c6f37e97cfbc22d2b4d1db1510d8f61e6a8866ad7f0e17c02b14182d37ea7c3c8b9c2683aeb6b733a1@52.169.14.227:30303,enode://6ce05930c72abc632c58e2e4324f7c7ea478cec0ed4fa2528982cf34483094e9cbc9216e7aa349691242576d552a2a56aaeae426c5303ded677ce455ba1acd9d@13.84.180.240:30303"
```
3. 安装truffle（版本：4.0.0-beta.2）
```Bash
npm install -g truffle@4.0.0-beta.2
```
4. 创建Voting项目
```Bash
mkdir voting
cd voting
```
安装web-pack：
```Bash
npm install -g webpack@2.2.1
truffle unbox webpack
```

5. 复制Voting.sol和index.js, index.html到相应目录
Voting.sol -> ./contracts/Voting.sol
index.js -> ./app/javascripts/app.js 需要更新
index.html -> ./app/index.html  更新index.html **line 39**: <script src="app.js"></script>

6. 创建2_deploy_contracts.js帮助部署Voting

7. 更新app.js使用truffle 引入abi定义

8. 部署Voting到 Ropsten testnet
```Bash
~/voting$ truffle console
truffle(development)> web3.personal.newAccount('verystrongpassword')
'0x95a94979d86d9c32d1d2ab5ace2dcc8d1b446fa1'
truffle(development)> web3.eth.getBalance('0x95a94979d86d9c32d1d2ab5ace2dcc8d1b446fa1')
{ [String: '0'] s: 1, e: 0, c: [ 0 ] }
truffle(development)> web3.personal.unlockAccount('0x95a94979d86d9c32d1d2ab5ace2dcc8d1b446fa1', 'verystrongpassword', 15000)
```
**注意**：其中web3.personal.unlockAccount第三个参数‘15000’是本次unlock的可用时间，
geth也提供相似的[API](https://github.com/ethereum/go-ethereum/wiki/Management-APIs#personal_unlockaccount)

9. 给testnet的account冲ether
* 跟别人申请：[reddit](https://www.reddit.com/r/ethdev/comments/72ltwj/the_new_if_you_need_some_ropsten_testnet_ethers/?sort=new&limit=500)
* 自己挖矿
```Bash
nohup geth --testnet --datadir "/home/work/ethereum/data" --syncmode "fast" --cache=512 > geth.log 2>&1 

geth attach /home/work/ethereum/data/geth.ipc
miner.setEtherbase('0x95a94979d86d9c32d1d2ab5ace2dcc8d1b446fa1')
miner.start(4)
```

10. 使用truffle部署contract（**注意**：确保account已经unlock）
```Bash
~/voting$ truffle migrate
```
大概需要等待1分钟左右...

11. 使用truffle终端与contract交互
```Bash
~/voting$ truffle console
truffle(development)> Voting.deployed().then(function(contractInstance) {contractInstance.voteForCandidate('Rama').then(function(v) {console.log(v)})})


truffle(development)> Voting.deployed().then(function(contractInstance) {contractInstance.totalVotesFor.call('Rama').then(function(v) {console.log(v)})})

```

12. 使用web页面与contract交互
```Bash
~/voting$ npm run dev
```
**注意**：如果chrome安装了MetaMask插件的同学，第一次运行可以先关闭插件，运行成功之后在研究与MetaMask插件的交互。

Dapp-voting升级版本：voting-pro
-----------------------------
1. unlockAccount
```Bash
~/voting-pro$ truffle console
truffle(development)> web3.personal.unlockAccount('0x95a94979d86d9c32d1d2ab5ace2dcc8d1b446fa1', 'verystrongpassword', 15000)
```
**注意**：更新账户地址，密码

2. 部署到testnet
```Bash
~/voting-pro$ truffle migrate
Using network 'development'.

Running migration: 2_deploy_contracts.js
  Deploying Voting...
  ... 0x546a05e9094a73f3660d063e3bcc805851544a48f16c6946f769e3ee9f23cc8c
  Voting: 0xa154447aee1072c471ac43513c09b8e177503ed5
Saving successful migration to network...
  ... 0x8240ca1ed7d6418faa362802f590d8f080189e0b43cd47f9f526cc2c05c0e9a1
Saving artifacts...

```

3. 终端交互：
```Bash
~/voting-pro$ truffle console
truffle(development)> Voting.deployed().then(function(contract) {contract.buy({value: web3.toWei('1', 'ether'), from: web3.eth.accounts[0]})})
```

4. 使用web页面与contract交互
```Bash
~/voting-pro$ npm run dev
```

另外，关于truffle debug 请参考：http://truffleframework.com/docs/getting_started/debugging
（可能遇到的错误参考：https://github.com/trufflesuite/truffle/issues/608）


使用第三方TRopsten Testnet Provider（无需本地geth同步区块）
-----------------------------

1. 安装依赖库

```Bash
~/voting$ npm install truffle-hdwallet-provider --save
```
注意：truffle-hdwallet-provider 依赖Python 3.6

2. 到 https://infura.io 注册获取api-key

3. 在truffle.js的配置中，按照下面做相应修改
```javascript
var HDWalletProvider = require("truffle-hdwallet-provider");

var infura_apikey = "XXXXXX"; //需要到https://infura.io注册，即可获得
var mnemonic = "twelve words you can find in metamask/settings/reveal seed words blabla"; // 12个单词的助记词

module.exports = {
  networks: {
    development: {
      host: "localhost",
      port: 8545,
      network_id: "*" // Match any network id
    },
    ropsten: {
      provider: new HDWalletProvider(mnemonic, "https://ropsten.infura.io/"+infura_apikey),
      network_id: 3
    }
  }
};
```

4. 部署合约到 Ropsten network
```Bash
~/voting$ truffle migrate --network ropsten
```

5. 使用MetaMask提交Transaction
注释掉app/index.html 第37行，不再加载cdn的web3，使用metamask的web3
<!-- <script src="https://cdn.jsdelivr.net/npm/web3@0.20.1/dist/web3.js"></script> -->

6. 运行
```Bash
~/voting$ npm run dev
```

7. 登陆MetaMask，浏览器打开页面 http://localhost:8080/