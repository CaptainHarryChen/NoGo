# NoGo项目说明

本项目为北京大学2020秋季学期计算概论A期末大作业

游戏不围棋规则：

棋盘：与9*9围棋棋盘相同(坐标从0开始，先x后y, 原点在左上角)

1. 棋盘同九路围棋棋盘，9×9；

2. 黑子先手，双方轮流落子，落子后棋子不可移动；

3. 对弈的目标不是吃掉对方的棋子

4. 恰恰相反，如果一方落子后吃掉了对方的棋子，则落子一方判负；

5. 对弈禁止自杀，落子自杀一方判负；

6. 对弈禁止空手(pass)，空手一方判负；

7. 对弈结果只有胜负，没有和棋。
8. 吃子定义：一个棋子在棋盘上，与它直线紧邻的空点是这个棋子的“气”。 棋子直线紧邻的点上，如果有同色棋子存在，则它们便相互连接成一个不可分割的整体。它们的气也应一并计算。 棋子直线紧邻的点上，如果有异色棋子存在，这口气就不复存在。如所有的气均为对方所占据，便呈无气状态。无气状态的棋子不能在棋盘上存在，也就是提子。把无气之子提出盘外的手段叫“提子”。

9. 附加规则：为保证先后手平衡，黑棋第一手禁止走棋盘中心位置（在botzone上无此规则） 



程序运行平台：Windows 10

AI测试平台：[botzone](https://botzone.org.cn/game/NoGo)

## 思路

AI算法使用蒙特卡洛树搜索（MCTS）

界面使用OpenGL的glut工具库完成，模仿MFC的面向对象结构设计UI

AI与界面分为两个线程工作，通过一个共用消息队列进行信息交互



## 功能简介

拥有开始游戏，重新开始游戏，选择黑棋白棋，读取存档，保存存档功能。

棋盘中会显示鼠标位置是否能够下子



## 文件说明

Graphic.h/Graphic.cpp

包含Point, Rect, Text结构体，以及图片读取与绘制，字体绘制等基础函数

save_queue.h

从网络上学到的用于多线程交互的安全消息队列

SaveManager.h/SaveManager.cpp

SaveManager类，用于管理游戏存档

Button.h/Button.cpp

Button类，实现游戏界面的各个按钮

CheckerBoard.h/CheckerBoard.cpp

CheckerBoard类，实现游戏界面的棋盘

MenuBoard.h/MenuBoard.cpp

MenuBoard类，实现游戏界面菜单栏，信息栏

GameAI.h/GameAI.cpp

GameAI类，各种AI的基类，方便更换不同算法的AI

GameAI_MCTS.h/GameAI_MCTS.cpp

GameAI_MCTS类，实现蒙特卡洛树搜索的AI类，单独一个进程运行

GameAI_Minmax.h/GameAI_Minmax.cpp

GameAI_Minmax类，实现minmax搜索的AI类，单独一个进程运行，基本失败

GameRule.h/GameRule.cpp

GameRule类，进行游戏的规则处理，内部使用并查集处理连通块，包括判断操作是否合法等操作

MinMaxNode.h/MinMaxNode.cpp

为MinMax算法写的结点类

GameScene.h/GameScene.cpp

与glut对接的主类，控制整个游戏进程

main.cpp

程序运行开端，初始化glut，与GameScene对接

img文件夹

包含游戏所需的材质

save文件夹

游戏存档文件夹

