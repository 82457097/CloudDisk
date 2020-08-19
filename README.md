# 项目介绍 
CSDN博客地址https://blog.csdn.net/weixin_44816732/article/details/104346199

# 项目架构
Nginx+fastDFS+fastCGI+MySQL+Redis

# 项目更新记录
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

# 一、项目介绍
先附上项目Github地址[https://github.com/82457097/CloudDisk](https://github.com/82457097/CloudDisk)；
这是我在网上找到的一个项目，觉得不错就模仿着做了一下~
分布式网络云盘，其中分布式是指的分布式存储系统FastDFS，也是一个开源项目，没有做很深的研究，大概了解了一下，可以看我的这篇博客介绍[fastDFS分布式文件系统介绍和安装](https://blog.csdn.net/weixin_44816732/article/details/104183493)；然后在利用ngnix反向代理服务器，接收并处理用户的请求，其中还使用了FastCGI+spawn-fcgi配合nginx工作；
然后呢还用到了MySQL和Redis数据库，其中MySQL用于存储文件的信息，Redis呢主要用于存储用户注册登录之类的鉴权信息，这一块我还没有实现~目前呢只实现了一些基本的文件上传下载等功能。
# 二、项目架构
项目架构的话先看一张我扒来的图吧~不是很清楚。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200216204218158.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDgxNjczMg==,size_16,color_FFFFFF,t_70)
图上的内容就是该项目的组网架构，主要工作流程是用户通过浏览器（或者客户端）发送操作请求到web服务器，也就是由nginx反向代理的web服务器。然后fastCGI+spawn-fcgi模块创建进程来接收通过web服务器而来的用户请求，并交由fastDFS文件系统处理，并返回处理后的结果给web服务器，web服务器再将结果展示给用户。
# 三、项目准备
该项目需要一个Linux服务器，推荐阿里云，环境搞坏了可以直接重装系统，分分钟就好了，很节约时间，哈哈~
我这里只列一下需要安装哪些组件，后面我会把整理好的安装包全部传到GitHub里面~
- nginx version: nginx/1.16.1 （nginx可以直接去官网下载[www.nginx.org](http://www.nginx.org)，很方便，不过要注意一下版本，版本不能比1.16.1高）
- [关于nginx的介绍](https://blog.csdn.net/weixin_44816732/article/details/104214112)
- fastdfs （项目GitHub地址[https://github.com/happyfish100](https://github.com/happyfish100)）
- [关于fastDFS的介绍](https://blog.csdn.net/weixin_44816732/article/details/104183493)
- fastCGI （项目GitHub地址[https://github.com/FastCGI-Archives/FastCGI.com](https://github.com/FastCGI-Archives/FastCGI.com)）
- [fastCGI的工作原理](https://blog.csdn.net/weixin_44816732/article/details/104237535)
- spawn-fcgi 下载地址[http://download.lighttpd.net/spawn-fcgi/releases-1.6.x/spawn-fcgi-1.6.4.tar.gz](http://download.lighttpd.net/spawn-fcgi/releases-1.6.x/spawn-fcgi-1.6.4.tar.gz)
- fastdfs-nginx-module 项目GitHub地址[https://github.com/happyfish100/fastdfs-nginx-module](https://github.com/happyfish100/fastdfs-nginx-module)
- MySQL apt或者yum就行
- Redis 同上
# 四、环境搭建
环境搭建这一块，我之前的文章已经总结好了，按顺序一个一个装就行了~
- [nginx安装](https://blog.csdn.net/weixin_44816732/article/details/104226693)
- [fastDFS安装](https://blog.csdn.net/weixin_44816732/article/details/104183493)
- [fastdfs-nginx-module安装](https://blog.csdn.net/weixin_44816732/article/details/104302194)**这个是重点！！！一定要配置好！！！**
- [fastCGI安装](https://blog.csdn.net/weixin_44816732/article/details/104237535)
- spawn-fcgi安装：下载好直接解压./configure然后make和makeinstall就OK
- [MySQL安装](https://blog.csdn.net/weixin_44816732/article/details/104173717)
- [Redis安装](https://blog.csdn.net/weixin_44816732/article/details/104168349)
# 五、环境测试
搭建好环境之后，就可以测试一下环境是否没有问题了。
- 我们要更改一下nginx配置文件测试fastcgi模块
- `$ cd /usr/local/nginx/conf/`
- `$ vim nginx.conf`
- 加一个测试location
```cpp
location /test {
    fastcgi_pass 127.0.0.1:7890; 	//记住这个端口号7890
    include fastcgi.conf;
}
```
- 然后编译src中的test.c `$ gcc test.c -o test.cgi -lfcgi`
- 然后用spawn-fcgi开启test.cgi `$ spawn -a 127.0.0.1 -p 7890 -f ./test.cgi`
- 然后打开网页的测试一下 在浏览器输入你的服务器IP/test就行，看看web服务器是否会返回消息~
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200216214927334.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDgxNjczMg==,size_16,color_FFFFFF,t_70)
- 然后再测试fastdfs-nginx-module模块参照[这个链接测试就行](https://blog.csdn.net/weixin_44816732/article/details/104302194)

# 六、文件上传流程
fastDFS系统文件的上传下载原理看这篇文章[fastDFS分布式文件系统文件上传和下载流程](https://blog.csdn.net/weixin_44816732/article/details/104214628)；
下面这张图是上传文件的代码流程~
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200216215121572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDgxNjczMg==,size_16,color_FFFFFF,t_70)
# 七、文件上传测试
因为我不会写前端界面，所以这里的web界面使用网上找的现成的界面，只需要把它部署到nginx上面就好了，具体设置按照下面修改nginx.conf文件；

```cpp
//在目录/usr/local/nginx下创建一个work目录，然后将zfFile2文件目录拷贝进去，并在nginx配置文件里更改location如下~
location / {
    root   work/zyFile2;
    index  demo.html;
}

location /upload {
    fastcgi_pass 127.0.0.1:10003;	//记住端口号 监听的端口要和配置的一致
    include fastcgi.conf;
}
```
配置好重启nginx `$ nginx -s reload`在浏览器输入ip会得到如下功能界面，在这里你就可以进行上传操作了，但是之前你需要把src里面的upload.cgi程序编译出来~
然后`$ spawn -a 127.0.0.1 -p 10003 -f ./upload.cgi`
然后成功的操作就会和下图一样~数据库也能找到相关文件的信息；
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020021621534862.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDgxNjczMg==,size_16,color_FFFFFF,t_70)
# 八、项目总结
做这个项目还是花了比较长的时间的，目前也只是完成了基本的上传文件功能，其它功能待进一步开发，都不是什么大的问题了；前期因为缺乏经验。搭环境遇到了无数的坑，但是有些坑踩过了之后，也是一种成长，省的以后在工作中再踩了~总体来说，做这个项目收获还是比较大的，主要的收获也不是编码能力的提升，而是对项目的架构能力，了解了一些通用的项目架构框架，了解了许多开源组件的性能特点，也知道了如何去选择合适的组件，针对什么样的业务场景，需要注意哪些点。。。
