Setup fabric-samples的运行环境
-----------------------

1. Windows安装 node (v8.9.0):
https://nodejs.org/download/release/v8.9.0/node-v8.9.0-x64.msi (其他版本 https://nodejs.org/en/download/releases/)


2. 拉取sample code，在$GOPATH\src\github.com/hyperledger目录执行：
```Bash
git clone -b sunshanlu https://github.com/shanlusun/fabric-samples.git
cd fabric-samples/balance-transfer
```

3. 安装相关工具：
```Bash
npm install --global --production windows-build-tools
npm install --global node-gyp
npm install --global node-pre-gyp
npm install --global grpc
```
建议npm使用5.2.0版本：
```Bash
npm install -g npm@5.2.0
```
Widnows手动下载 jq工具: https://github.com/stedolan/jq/releases/download/jq-1.5/jq-win64.exe (其他版本 https://stedolan.github.io/jq/download/)

手动复制到 C:\Program Files\Git\mingw64\bin\jq.exe (因为默认mingw64\bin路径已经添加系统环境变量，因此jq.exe 可以直接被调用)

4. 安装fabric-sample依赖的包
在fabric-samples/balance-transfer目录执行：
```Bash
npm install 
```
(如果失败，需要重试，重试前注意需要清空npm-cache，路径类似：C:\Users\sunny\AppData\Roaming\npm-cache\_cacache)

npm install成功之后，就可以按照课程中演示的操作，运行balance-transfer了。

