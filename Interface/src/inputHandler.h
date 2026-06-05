#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_set>

enum class GameAction {
	None,
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	Restart,
	Undo,             //退格
	NextLevel,        //下一关
	PrevLevel,        //上一关
	BackToMenu,       //返回选关菜单
	BackToDifficulty, //返回难度选择
	Quit,
	SwitchBgm         // 切换背景音乐
};

class InputHandler {
public:
	

	GameAction handleInput(sf::RenderWindow& window);
	std::unordered_set<sf::Keyboard::Key> pressedKeys;
private:
	GameAction mapKeyToAction(sf::Keyboard::Key key); 
	
};