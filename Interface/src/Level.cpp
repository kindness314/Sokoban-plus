#include"Level.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sstream>

bool Level::loadFromFile(const std::string& filepath){
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cerr << "无法打开关卡文件:" << filepath << "\n";
		return false;
	}
	std::vector<std::string> lines;
	std::string line;
	int maxWidth = 0;
	while (std::getline(file, line)) {
		if (line.empty()) continue;
		lines.push_back(line);
		maxWidth = std::max(maxWidth, static_cast<int>(line.size()));
	}
	file.close();

	if (lines.empty()) {
		std::cerr << "关卡文件为空！\n";
		return false;
	}
	int rows = lines.size();
	int cols = maxWidth;

	m_map.assign(rows, std::vector<TileType>(cols, TileType::Empty));
	playerStart = { 0, 0 };
	boxStarts.clear();
	targets.clear();
	for (int y = 0; y < rows; ++y) {
		const std::string& rowStr = lines[y];
		for (int x = 0; x < cols; ++x) {
			char c = (x < static_cast<int>(rowStr.size()) ? rowStr[x] : '-');   // 如果行长度不足，默认为空地
			switch (c) {
			case '#':
				m_map[y][x] = TileType::Wall;
				break;
			case '.':
				m_map[y][x] = TileType::Target;
				targets.push_back({ x, y });
				break;
			case '-':
				m_map[y][x] = TileType::Empty;
				break;
			case '@':
				playerStart = { x, y };
				m_map[y][x] = TileType::Empty;   // 玩家站在普通地板上
				break;
			case '$':
				boxStarts.push_back({ x, y });
				m_map[y][x] = TileType::Empty;   // 箱子初始在普通地板上
				break;
			case '+':   // 玩家在目标上
				playerStart = { x, y };
				m_map[y][x] = TileType::Target;
				targets.push_back({ x, y });
				break;
			case '*':   // 箱子在目标上
				boxStarts.push_back({ x, y });
				m_map[y][x] = TileType::Target;
				targets.push_back({ x, y });
				break;
			default:
				// 非法字符视为空地
				m_map[y][x] = TileType::Empty;
				break;
			}
		}
	}
	return true;
}
int Level::width() const {
	if (m_map.empty()) return 0;
	return static_cast<int>(m_map[0].size());
}
int Level::height() const {
	return static_cast<int>(m_map.size());
}
TileType Level::tileAt(Position p) const {
	if (p.y < 0 || p.y >= height() || p.x < 0 || p.x >= width())
		return TileType::Wall;   // 地图外视为墙，防止走出边界
	return m_map[p.y][p.x];
}

bool Level::isWall(Position p) const {
	return tileAt(p) == TileType::Wall;
}

bool Level::isTarget(Position p) const {
	return tileAt(p) == TileType::Target;
}