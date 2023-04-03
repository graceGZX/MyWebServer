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
