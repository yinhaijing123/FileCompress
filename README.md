#项目：文件压缩 #
----------

一、**项目描述：**

- 利用Huffman树是加权路径长度最短的二叉树，用Huffman编码来替换原文件字符，从而达到压缩的目的。
- 解压，利用配置文件重构Huffman树，对压缩文件进行解压缩。

二、**各模块实现步骤**

- 压缩文件：

	1. 首先需要统计各字符的个数；
	2. 用字符个数构建Huffman树；
	3. 根据Huffman树，为每个字符生成Huffman编码；
	4. 根据Huffman编码，进行压缩。
 
- 解压文件：

      1. 首先要重建Huffman树；
      2. 然后在遍历Huffman树，还原原字符。