# TouhouKeyboard
a custom keyboard designed by tsailooy which is constructed with some function key in TouhouProject

# 客制化键盘项目
以stm32为主控，主要用到gpio口对按键进行检测。检测算法使用轮询将按键信息加入FIFO队列，从而避免按键冲突。

## 硬件
- stm32F103C8T6最小系统板
- 机械键盘青轴
- 使用嘉立创EDA设计PDB板（白嫖嘉立创打板）  
  ![image](http://m.qpic.cn/psc?/V52nyJjM3JAJCp1gOHpz4QMS6h0jZFC5/bqQfVz5yrrGYSXMvKr.cqZHH6Wsa9lUuQKyIKTV3emPLcaqLNNlSqo9EE.BJvOBSVDYHQBlIzbPJ8bNwSKe4xkZmBjVwPG6DMA543EQuD9A!/b&bo=1ALpAQAAAAADBxw!&rf=viewer_4)