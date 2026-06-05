#pragma once
#include"Position.h"
#include"Level.h"
#include<vector>


class GameLogic {
public:
    bool loadLevel(const std::string& filepath);
    void resetLevel();
    bool movePlayer(int dx, int dy);
    bool undo();
    bool isBoxOnTarget(Position boxPos) const;
    const Level& getLevel() const;
    Position getPlayerPos() const;
    const std::vector<Position>& getBoxPositions() const;
    int getSteps() const;
    bool isWin() const;

    // 关卡列表与选关
    void setLevelList(const std::vector<std::string>& filePaths);
    int  getTotalLevels() const;
    int  getCurrentLevelIndex() const;
    bool loadLevelByIndex(int index);
    bool loadNextLevel();
    bool loadPreviousLevel();

    // 完成状态与最佳步数
    void markCurrentLevelCompleted(int steps);
    bool isLevelCompleted(int index) const;
    int  getBestSteps(int index) const;

    // 存档
    bool saveToFile(const std::string& filepath) const;
    bool loadFromFile(const std::string& filepath);

    std::vector<std::string> scanLevels(const std::string& folder);

private:
    Level m_level;
    Position m_playerPos;
    std::vector<std::string> m_levelPaths; //保存所有关卡的文件路径
    std::vector<Position> m_boxPositions;
    std::vector<bool> m_levelCompleted;  // 索引对应关卡是否完成
    std::vector<int> m_bestSteps;        //-1==未完成
    int m_currentLevelIndex = 0;
    int m_steps = 0;
    // 撤回用
    struct State {
        Position player;
        std::vector<Position> boxes;
        int steps;
    };
    std::vector<State> m_history;

    // 辅助
    int findBoxAt(Position p) const;  // 返回箱子索引，没有返回 -1
};