# STL_Containers
## 简介
基于C++11实现了STL的大部分容器\
在大二学完数据结构和C++后，一直想找个合适的项目练手，后来看到了侯捷的《STL源码剖析》这本书，受益匪浅，当时跟着写了一段但没有写完，现在来用C++11重构当时的代码并且实现一些新算法。\
完成度如下
| Container      | Description |
| ----------- | ----------- |
| vector      | √       |
| list   | √       |
| deque | √ |
| stack | √ |
| queue | √ |
| priority_queue | × |
| set | √ |
| map | √ |
| multiset | √ |
| multimap | √ |
| unordered_set | √ |
| unordered_map | √ |
| unordered_multiset | √ |
| unordered_multimap | √ |
## 参考
SGI-STL的源码：[SGI-STL](https://github.com/steveLauwh/SGI-STL)\
侯捷.STL源码剖析[M]：华中科技大学出版社，2002.\
有关STL的迭代器的[博客](https://juejin.cn/post/7109086946160279559)，写的很详细\
关于红黑树的插入和删除的[博客](http://blog.csdn.net/v_JULY_v/article/details/6105630)
## 测试
与标准模板库中的容器相对比进行正确性和性能测试，部分结果如下
![image](https://github.com/HuiQianPeng0401/phq_stl/assets/57929041/6fc67253-14ce-417b-9d57-69b108730c98)
![image](https://github.com/HuiQianPeng0401/phq_stl/assets/57929041/b3aa15f2-714e-4e5f-b311-4b5dba82fdb5)
![image](https://github.com/HuiQianPeng0401/phq_stl/assets/57929041/4e1f8731-6cb5-42a9-aaba-51b6a95fdc81)
