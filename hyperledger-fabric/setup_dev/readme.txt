Widnows环境配置:
1.下载gitbash并安装：https://git-for-windows.github.io/
运行git命令：避免windows下换行符与Unix系列的区别；防止git路径过深带来的问题；
  git config --global core.autocrlf false
  git config --global core.longpaths true

2.下载go并安装：https://golang.org/dl/（1.9.2）
配置关键环境变量：
  $GOROOT是go安装目录，举例：C:\Users\sunny\OneDrive\Go
  $GOPATH是自定义的目录，举例：C:\Users\sunny\OneDrive\GOPATH
  在GOPATH目录下面建立src，bin，pkg目录，在src目录下建立github.com\hyperledger目录

3.下载 vagrant工具，并安装：https://www.vagrantup.com/downloads.html
  注意：如果使用的是win10，建议安装2.0.0版本
        如果使用的是win7，建议安装1.9.5版本
4.下载 vitualbox并安装、运行：http://sw.bos.baidu.com/sw-search-sp/software/6bda11b7d3256/VirtualBox-5.1.30.18389-Win.exe
  注意：win10或win7建议安装版本：5.1.30 18389
5.进入 $GOPATH\src\github.com\hyperledger，执行：
  git clone https://github.com/hyperledger/fabric.git
  进入 $GOPATH\src\github.com\hyperledger\fabric，执行：
  git checkout v1.0.3
6.进入 $GOPATH\src\github.com\hyperledger\fabric\devenv，执行：vagrant.exe up
  （此过程比较长，耐心等待）
7.进入虚拟机环境：vagrant.exe ssh
8.下载fabric官方的docker image：sh download_docker_images.sh



