# MyWebServer



![](https://img.shields.io/badge/language-c++-green.svg)

项目介绍
----

在“qinguoyi / TinyWebServer”的基础上实现了一个WebServer，增加了一些功能





添加功能
----

+ 对互斥锁使用RAII机制，使线程更安全
+ 用TinySTL中的一些容器进行替代
+ 封装了BUFFER类
+ 实现了自己的内存池
+ 实现了LFU缓存
+ 实现了基于Redis的连接池，支持从Redis进行注册和登录校验
+ 通过Redis实现了Token机制，实现了接口的幂等性



快速运行
------------

* 服务器测试环境

  * Ubuntu版本16.04
  * MySQL版本5.7.29

* 浏览器测试环境

  * Windows、Linux均可
  * Chrome
  * FireFox
  * 其他浏览器暂无测试

* 测试前确认已安装MySQL数据库

  ```C++
  // 建立yourdb库
  create database yourdb;
  
  // 创建user表
  USE yourdb;
  CREATE TABLE user(
      username char(50) NULL,
      passwd char(50) NULL
  )ENGINE=InnoDB;
  
  // 添加数据
  INSERT INTO user(username, passwd) VALUES('name', 'passwd');
  ```

* 修改main.cpp中的数据库初始化信息

  ```C++
  //数据库登录名,密码,库名
  string user = "root";
  string passwd = "root";
  string databasename = "yourdb";
  ```

* build

  ```C++
  sh ./build.sh
  ```

* 启动server

  ```C++
  ./server
  ```

* 浏览器端

  ```C++
  ip:9006
  ```

