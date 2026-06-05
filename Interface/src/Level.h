#pragma once
#include<vector>
#include<string>
#include"Position.h"

enum class TileType {
	Empty,      //0 普通地板
	Wall,       //1 普通墙
	Target      //2 目标点(箱子终点)
};

class Level {
public:
	bool loadFromFile(const std::string& filepath);
	int width() const;
	int height() const;
	bool isWall(Position p) const;
	bool isTarget(Position p) const;
	TileType tileAt(Position p) const;

	Position playerStart;            //出生坐标
	std::vector<Position> boxStarts; //箱子起始位置
	std::vector<Position> targets;   //目标点位置

private:
	std::vector<std::vector<TileType>> m_map;
};
