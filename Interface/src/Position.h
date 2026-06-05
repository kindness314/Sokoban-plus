#pragma once
struct Position {
	int x;
	int y;
	bool operator==(const Position& a)const {
		return x == a.x && y == a.y;
	}
	bool operator<(const Position& other) const {
		if (x != other.x) return x < other.x;
		return y < other.y;
	}
};