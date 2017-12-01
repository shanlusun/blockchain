直接使用已经配置好开发环境的box
-----------------------
1. 参考[setup_dev](../setup_dev)的1~5步骤
2. 下载fabric.box (https://pan.baidu.com/s/1mi9VINu)，
   然后执行：
  ```Bash
  vagrant.exe box add fabric.box  --name fabric
  ```
3. 修改[Vagrantfile](./Vagrantfile):
  ```diff
  -Vagrant.configure("2") do |config|
  -   config.vm.box = "ubuntu/xenial64"            
  +Vagrant.configure("2") do |config|
  +   config.vm.box = "fabric"
  ```
  注释掉setup.sh的调用，此时不需要安装额外的工具，fabric.box已包含相关工具
  ```diff
  cd #{SRCMOUNT}/devenv
  -./setup.sh
  ```
  将修改后的Vagrantfile复制到fabric/devenv目录覆盖原有文件

4. 在fabric/devenv目录执行：
  ```Bash
  vagrant.exe up
  ```
5. 在fabric/devenv目录执行：
  ```Bash
  vagrant.exe ssh
  ```


适合执着完成setup的同学
-----------------------
  
如果在setup本地环境之后，执行：
  ```Bash
  vagrant.exe ssh
  ```

看到如下图片，说明本地环境有一点小问题：

![issue-1](https://github.com/shanlusun/blockchain/blob/master/hyperledger-fabric/setup_dev_pro/images/issue-1.PNG "issue-1")

这个问题google一下会发现[Hyperledger官方有说明](https://github.com/hyperledger-archives/fabric/wiki/Troubleshooting-devenv-provisioning "Hyperledger官方说明")
但是并没有给出解决方法。
>这个问题提示的**影响**：
>>无法在fabric目录执行make docker或make一些其他组件（peer，order和tools）

----------
[setup_dev](../setup_dev)过程中可能有一些问题：
在执行第6步：vagrant.exe up过程中可能遇到的错误提示：

1. 提示ubuntu/xenial64 下载timeout
2. 安装docker-ce，现有repository地址访问失败或timeout
3. zookeeper版本3.4.9和kafka版本0.9.0.1已经不存在了

如何解决：
----------
>**注意**：
>>当前目录已经提供[Vagrantfile](./Vagrantfile)和[setup.sh](./setup.sh)，可以直接复制到fabric/devenv目录中使用）

1. 使用迅雷直接下载一个可用的ubuntu镜像，这里给出候选：https://vagrantcloud.com/hyperledger/boxes/fabric-baseimage/versions/0.3.0/providers/virtualbox.box
  将下载文件重命名为virtualbox.box，然后执行：
  ```Bash
  vagrant.exe box add virtualbox.box  --name fabric
  ```
  修改[Vagrantfile](./Vagrantfile):
  ```diff
  -Vagrant.configure("2") do |config|
  -   config.vm.box = "ubuntu/xenial64"            
  +Vagrant.configure("2") do |config|
  +   config.vm.box = "fabric"
  ```
2. 修改[setup.sh](./setup.sh)，增加阿里云repository：
  ```Bash
  add-apt-repository "deb [arch=amd64] http://mirrors.aliyun.com/docker-ce/linux/ubuntu
$(lsb_release -cs) stable"
  ```
3. 修改[setup.sh](./setup.sh)，更新zookeeper和kafka的版本至存在的新版本

此外建议GO的版本更新至1.9.2和Node的版本更新至8.9.0
以上改动做完之后可以执行：
```Bash
vagrant.exe provision
```
最后再执行：
```Bash
vagrant.exe ssh
```
看到如下界面说明，一切工作正常：

![success-1](https://github.com/shanlusun/blockchain/blob/master/hyperledger-fabric/setup_dev_pro/images/success-1.PNG "success-1")

