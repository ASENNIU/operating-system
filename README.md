# Operating-System
本repo主要用与记录自己对操作系统的学习，包含两个部分：

- 操作系统基本概念与原理。这一部分来源于两个方向，一是对MIT 6.s081指导书的学习和解读，二是对Linux内核知识的理解和梳理。（对应main分支）
- MIT 6.s081 Lab（2020版）。本repo的实现参考了网上一些优秀的案例，代码均可以正确执行。（对应除main分支外的其余分支）。



# 操作系统基本概念与原理

这一部分主要用于深入理解操作系统。引用MIT 6.S081授课教授和李沐老师的话：

- 在这门课上，我们将花费大量的时间去探究操作系统的设计目的、思考和权衡。以及它的实现。（原话忘了，这里是记忆滤镜版）                                                                                                                      ——罗伯特.莫里斯
- 系统设计是一门艺术。

在学习的过程中参考了一些优秀的书籍和博客：

- xv6: a simple, Unix-like teaching operating system（6.s080的教材与指导书）
- [xv6手册与代码笔记](https://www.zhihu.com/column/c_1345025252318007298)（对xv6的解读）
- 现代操作系统：原理与实现（个人觉得国人写的很好的操作系统教材，支持！）
- Linux内核设计与实现 Ed3

已经整理过的部分（由于水平有限，一是难免有错误或者不严谨的部门，二是大部门都是学习来的，不能算原创，故说整理）（这里对操作系统该重要概念的划分参考xv6）：

- scheduling
  - Linux内核调度模型

# MIT 6.s081 Lab

本repo使用的实验版本主要为2020版（utils为2021）。个人认为该lab的意义在于达到这样一种境界：理解可执行程序在计算机系统的运行。所以意义在于理解操作系统对程序的支持与限制，程序如何在计算机系统上运行，局限性在于6.s081课程和lab并未涉及程序的编译、链接和装载。

本repo的实现参考了一些优秀的开源实现：

- [想学理论的调包侠](https://www.zhihu.com/people/kunkun.jyk)
- [Miigon's blog](https://blog.miigon.net/categories/mit6-s081/)

已完成的部分：

- Utilities

- System calls

- Page tables

- Traps

- Lazy allocation

- Copy-on-Write

- Multithreading

- Lock

  ​