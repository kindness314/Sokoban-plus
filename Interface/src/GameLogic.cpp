#include"GameLogic.h"
#include<algorithm>
#include<fstream>

#include <filesystem>
std::vector<std::string> GameLogic::scanLevels(const std::string& folder) {
	std::vector<std::string> paths;
	for (auto& entry : std::filesystem::directory_iterator(folder)) {
		if (entry.path().extension() == ".txt")
			paths.push_back(entry.path().string());
	}
	std::sort(paths.begin(), paths.end()); // 按名称排序，保证顺序
	return paths;
}


bool GameLogic::loadLevel(const std::string& path) {
	if (!m_level.loadFromFile(path))
		return false;
	resetLevel();
	return true;
}
void GameLogic::resetLevel() {
	m_playerPos = m_level.playerStart;
	m_boxPositions = m_level.boxStarts;
	m_steps = 0;
	m_history.clear();
}
bool GameLogic::movePlayer(int dx, int dy) {
	Position newPos = { m_playerPos.x + dx,m_playerPos.y + dy };
	if (m_level.isWall(newPos))
		return false;
	int boxIdx = findBoxAt(newPos);
	if (boxIdx != -1) {
		Position newBoxPos = { newPos.x + dx, newPos.y + dy };
		if (m_level.isWall(newBoxPos))
			return false;
		if (findBoxAt(newBoxPos) != -1)
			return false;
		m_history.push_back({ m_playerPos, m_boxPositions, m_steps });

		// 移动箱子
		m_boxPositions[boxIdx] = newBoxPos;
		// 移动玩家
		m_playerPos = newPos;
		m_steps++;
		return true;
	}
	m_history.push_back({ m_playerPos, m_boxPositions, m_steps });
	m_playerPos = newPos;
	m_steps++;
	return true;
}
bool GameLogic::undo() {
	if (m_history.empty())
		return false;
	State prev = m_history.back();
	m_history.pop_back();
	m_playerPos = prev.player;
	m_boxPositions = prev.boxes;
	m_steps = prev.steps;
	return true;
}
const Level& GameLogic::getLevel() const {
	return m_level;
}
Position GameLogic::getPlayerPos() const {
	return m_playerPos;
}
const std::vector<Position>& GameLogic::getBoxPositions() const {
	return m_boxPositions;
}
int GameLogic::getSteps() const {
	return m_steps;
}
bool GameLogic::isWin() const {
	for (const auto& target : m_level.targets) {
		bool boxOnTarget = std::any_of(m_boxPositions.begin(), m_boxPositions.end(), [&](const Position& box) {
			return box == target;
			});
		if (!boxOnTarget) return false;
	}
	return true;
}
int GameLogic::findBoxAt(Position p) const {
	for (size_t i = 0; i < m_boxPositions.size(); ++i) {
		if (m_boxPositions[i] == p) {
			return static_cast<int>(i);
		}
	}
	return -1;
}
bool GameLogic::isBoxOnTarget(Position boxPos) const {
	if (findBoxAt(boxPos) == -1) return false;
	return m_level.isTarget(boxPos);
}

//关卡列表管理
void GameLogic::setLevelList(const std::vector<std::string>& filePaths) {
	m_levelPaths = filePaths;
	m_levelCompleted.assign(filePaths.size(), false); //初始化
	m_bestSteps.assign(filePaths.size(), -1);         //初始化
	if (m_currentLevelIndex < 0 || m_currentLevelIndex >= static_cast<int>(filePaths.size()))
		m_currentLevelIndex = 0;
}
void GameLogic::markCurrentLevelCompleted(int steps) {
	int idx = m_currentLevelIndex;
	if (idx < 0 || idx >= static_cast<int>(m_levelCompleted.size())) return;//边界排查

	m_levelCompleted[idx] = true;
	if (m_bestSteps[idx] == -1 || steps < m_bestSteps[idx]) {
		m_bestSteps[idx] = steps;
	}
}
//判断关卡是否已完成
bool GameLogic::isLevelCompleted(int index) const {
	if (index < 0 || index >= static_cast<int>(m_levelCompleted.size())) return false;
	return m_levelCompleted[index];
}
int GameLogic::getBestSteps(int index) const {
	if (index < 0 || index >= static_cast<int>(m_bestSteps.size())) return -1;
	return m_bestSteps[index];
}

//选关函数
int GameLogic::getTotalLevels() const {
	return static_cast<int>(m_levelPaths.size());
}

int GameLogic::getCurrentLevelIndex() const {
	return m_currentLevelIndex;
}

//按索引读取游戏并更新Index
bool GameLogic::loadLevelByIndex(int index) {
	if (index < 0 || index >= static_cast<int>(m_levelPaths.size()))
		return false;
	if (!m_level.loadFromFile(m_levelPaths[index]))
		return false;
	m_currentLevelIndex = index;
	resetLevel();          // 重置玩家、箱子、步数、历史
	return true;
}

bool GameLogic::loadNextLevel() {
	return loadLevelByIndex(m_currentLevelIndex + 1);
}

bool GameLogic::loadPreviousLevel() {
	return loadLevelByIndex(m_currentLevelIndex - 1);
}

//存、读档 Save&Load
bool GameLogic::saveToFile(const std::string& filepath) const {
	std::ofstream out(filepath);
	if (!out) return false;

	// 第一行：当前关卡索引
	out << m_currentLevelIndex << "\n";

	// 之后每行一个关卡：完成标志(1/0) 最少步数
	for (size_t i = 0; i < m_levelCompleted.size(); ++i) {
		out << (m_levelCompleted[i] ? "1" : "0") << " " << m_bestSteps[i] << "\n";
	}
	return true;
}

bool GameLogic::loadFromFile(const std::string& filepath) {
	std::ifstream in(filepath);
	if (!in) return false;   // 没有存档文件则返回失败，由调用者处理

	int index = 0;
	if (!(in >> index)) return false;

	// 读取每一关的完成状态和最佳步数
	for (size_t i = 0; i < m_levelCompleted.size(); ++i) {
		int completedFlag, best;
		if (!(in >> completedFlag >> best)) break;
		m_levelCompleted[i] = (completedFlag != 0);
		m_bestSteps[i] = best;
	}

	// 恢复到记录的关卡
	return loadLevelByIndex(index);
}
