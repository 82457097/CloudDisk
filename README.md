# 项目介绍 
CSDN博客地址https://blog.csdn.net/weixin_44816732/article/details/104346199

# 项目架构
Nginx+fastDFS+fastCGI+MySQL+Redis

# 项目更新记录
- 0827更新
  - 增加写日志文件功能，已验证可用。
  - 优化log类，修复一些bug。

- 0826更新
  - 修复拷贝文件传给fastDFS后，临时文件未删除bug。
  - 修复一个内存泄漏问题。

- 0825更新
  - 完成cpp版本文件上传功能。
  - 修复web页面显示bug。
  - 完成数据库连接功能，成功写入数据。（但需要手动先创建数据库表项）

- 0824更新
  - 完成上传文件cgi的编写测试，上传功能OK，页面显示有问题。

- 0820更新
  - 增加用Cpp重构后的源文件（未编译测试）
  
- 0819更新
  - 修改完善Install.sh文件

- 0818更新
  - 增加一键安装配置脚本Install.sh

- 0218更新
  - 修改Makefile
  - 增加展示所有文件的新功能（未测试成功）
  - 增加源文件showFiles.c showFiles.h

- 0217更新
  - 增加doc 
  - 预备的数据库表
  - 上传所有用到的安装包
  - 增加项目所需要的html文件

- 0216更新
  - 上传源文件夹src
  - 上传头文件夹include
  - 更新Makefile

