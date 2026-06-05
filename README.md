# 推箱子 Sokoban
## github仓库https://github.com/kindness314/Sokoban-plus
经典推箱子游戏，将全部箱子推到目标点即可过关。  
支持 Windows。

## 操作说明

打开`Interface/x64/Release/Interface.exe`运行游戏

|    按键   |                       功能                         |
|-----------|---------------------------------------------------|
|  ↑ ↓ ← →  |              移动角色 / 在菜单中切换选项            |
|  W A S D  |              移动角色 / 在菜单中切换选项            |
|   Enter   |                      确认选择                      |
|Z/BackSpace|                     撤销上一步                     |
|     R     |                    重置当前关卡                    |
|   M/ESC   | 返回上一级（游戏中返回选关界面，选关界面返回难度界面） |
|     F1    |             直接从游戏中返回难度选择界面             |
|     F2    |                      切换音乐                      |
|     P     |                       上一关                       |
|     N     |                       下一关                       |

## 游戏流程

1. **难度选择界面**：游戏启动后，首先选择一个难度（简单/中等/困难等），使用方向键移动光标，Enter 确定。
2. **选关界面**：选定难度后，进入该难度下的关卡列表。已通关的关卡会显示“Completed”和最佳步数。
3. **游戏界面**：选定关卡后开始游戏，屏幕显示当前步数。按 ESC 可以返回选关界面，按 F1 可直接跳回难度选择界面。
4. **通关**：所有箱子推到目标点上即可过关，自动保存最优步数。

## 游戏规则
- 角色**不能穿墙**。
- 角色一次**只能推动一个箱子**。
- 箱子**不能推入墙壁**，也**不能推入另一个箱子**。
- **所有箱子**都停在目标点上即通关。
- 通关后记录你的步数；若刷新了该关卡的最少步数记录，会自动更新。

## 自定义关卡
你可以在 `levels/easy(medium、hard)` 文件夹中新建 `.txt` 文件来制作自己的关卡。

### 关卡文件符号
| 符号 |         含义          |
|-----|------------------------|
| `#` |           墙           |
| `-` |          地板          |
| `.` |         目标点         |
| `@` | 玩家出生点（站在地板上） |
| `$` | 箱子出生点（站在地板上） |
| `+` | 玩家出生点（站在目标上） |
| `*` | 箱子出生点（站在目标上） |

### 关卡示例
#######
#-.--.#
#.$@-.#
#..*--#
#######

text
*注：地图会自动补成矩形，短行右侧默认为地板。*

### 项目结构
.\Sokoban\Interface\
├── SFML-2.6.2/                   (SFML 库文件夹，含 include、lib、bin 等)
├── assets/
│   ├── fonts/
│   │   └── JYRB.ttf
│   ├── images/
│   │   ├── background.png
│   │   ├── clect.png
│   │   ├── player1.png
│   │   ├── player2.png
│   │   ├── red.png
│   │   ├── green.png
│   │   ├── target.png
│   │   └── wall.png
│   ├── audio/
│   │   ├── push.wav
│   │   └── win.wav
│   ├── levels/
│   │   ├── easy/
│   │   │   ├── level1.txt
│   │   │   └── ... (其他关卡文件)
│   │   ├── medium/
│   │   │   └── ...
│   │   └── hard/
│   │       └── ...
│   └── music/
│       ├── 1.mp3
│       └── ...
├── saves/                        (存档文件夹)
│   ├── sokoban_easy.sav
│   ├── sokoban_medium.sav
│   └── sokoban_hard.sav
├── src/                          (所有源代码)
│   ├── gameDo.h
│   ├── gameDo.cpp
│   ├── GameLogic.h
│   ├── GameLogic.cpp
│   ├── inputHandler.h
│   ├── inputHandler.cpp
│   ├── Level.h
│   ├── Level.cpp
│   ├── Menu.h
│   ├── Menu.cpp
│   ├── Position.h
│   ├── Render.h
│   ├── Render.cpp
│   └── RunGame.cpp              (包含 main 函数)
├── x64/                          (编译输出)
│   ├── Debug/
│   │   ├── Interface.exe         (调试版)
│   │   └── sfml-graphics-d-2.dll 等 DLL
│   └── Release/
│       ├── Interface.exe         (发布版)
│       └── ...
├── Interface.sln                 (解决方案文件)
├── Interface.vcxproj             (项目文件)
├── Interface.vcxproj.filters
└── Interface.vcxproj.user


## 存档位置
游戏进度和最佳步数自动保存在 **`sokoban_easy(medium,hard).sav`** 文件中（与可执行文件同目录）。删除此文件可重置所有进度。

## 常见问题
**Q: 为什么我摁了W/A/S/D角色不动呢**
A: 请切换到英文输入法下在进行操作。

**Q: 启动游戏提示缺少 DLL？**  
A: 请确保已安装 SFML 运行库，或从 SFML 官网下载对应版本的 DLL 放入游戏目录。

**Q: 卡关怎么办？**  
A: 按 R 键重置当前关卡，或按 N 跳到下一关，也可以按 ESC 返回选关界面重选，或按 F1 回到难度菜单。

**Q: 如何清空某个关卡的最佳记录？**
A: 用文本编辑器打开游戏目录下的 `sokoban_easy(medium,hard).sav`，找到对应关卡所在行（按难度和关卡排序），将“完成标志”改为 0，“步数”改为 -1 即可。例如将 1 56 改为 0 -1。修改前建议备份文件。

**Q: 为什么有些关卡明明箱子都推到目标上了，却没有通关？**  
A: 最常见的三个原因：
- 地图中存在未覆盖的目标点（可能被玩家角色遮挡，可移动角色后再观察）。
- 关卡设计中目标点数多于箱子数（需检查 `.txt` 文件中 `.`、`+`、`*` 的总数与 `$`、`*` 的总数是否一致）。
- 关卡文件的某个目标点符号被写成了全角字符或被误认为其他字符，请用纯文本编辑器检查。
  
**Q: 撤销（Z 键）能撤回多少步？**  
A: 撤销无次数限制，可从通关状态一路退回到关卡初始状态。但重置关卡（R）或切换关卡后会清空撤销历史。

**Q: 我可以和朋友分享我的自制关卡吗？**  
A: 可以。只需将你制作的 `.txt` 关卡文件发送给朋友，放在相同的难度文件夹内即可。如果朋友没有该难度文件夹，新建一个同名文件夹即可。分享存档文件（`.sav`）则会同步你的通关记录。

## 制作团队
- 逻辑 & 系统：[李俊昊]
- 美术 & 交互：[董泽轩]

# 免责声明 / Disclaimer

本项目（推箱子游戏）所使用的**美术图片**（包括但不限于 `player1.png`、`player2.png`、`wall.png`、`red.png`、`green.png`、`target.png`、`clect.png`、`background.png`）以及**音效文件**（`push.wav`、`win.wav`）和**背景音乐**文件，均来源于网络公开资源或第三方素材库。

这些素材的版权归**原作者或版权方**所有。

本项目**仅用于个人学习、课程作业及非商业用途**，不涉及任何商业盈利行为。  
若任何素材的版权方认为本项目存在侵权行为，请及时联系，我们将立即移除相关内容。

感谢所有素材提供者对本项目的间接支持。

---

*The images, sound effects, and music used in this Sokoban project are sourced from publicly available online resources or third-party material libraries.*  
*All copyrights belong to their respective owners.*  
*This project is intended solely for personal learning, academic coursework, and non-commercial purposes. If any copyright holder believes an infringement has occurred, please contact us for prompt removal.*

最后，
祝您玩得开心、玩得愉快！
