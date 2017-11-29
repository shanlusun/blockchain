如果在setup本地环境之后，执行：vagrant.exe ssh
看到如下图片，说明本地环境有一点小问题：

这个问题google一下会发现Hyperledger官方有说明：https://github.com/hyperledger-archives/fabric/wiki/Troubleshooting-devenv-provisioning
但是并没有给出解决方法。
这个提示的影响：无法执行make docker 或者make一些其他组件时候会失败。


setup_dev过程中可能有一些问题：

在执行第6步：vagrant.exe up过程中可能遇到的错误提示：
1.提示ubuntu/xenial64 下载timeout
2.安装docker-ce，现有repository地址访问失败或timeout
3.zookeeper版本3.4.9和kafka版本0.9.0.1已经不存在了

如何解决：
1.使用迅雷直接下载一个可用的ubuntu镜像，这里给出候选：https://vagrantcloud.com/hyperledger/boxes/fabric-baseimage/versions/0.3.0/providers/virtualbox.box
  将下载文件重命名为virtualbox.box，然后执行：vagrant.exe box add virtualbox.box  --name fabric
  修改Vagrantfile:
Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/xenial64"
	↓
Vagrant.configure("2") do |config|
  config.vm.box = "fabric"
2.修改setup.sh，增加阿里云repository：add-apt-repository "deb [arch=amd64] http://mirrors.aliyun.com/docker-ce/linux/ubuntu
$(lsb_release -cs) stable"
3.修改setup.sh，更新zookeeper和kafka的版本至存在的新版本

此外建议GO的版本更新至1.9.2和Node的版本更新至8.9.0
