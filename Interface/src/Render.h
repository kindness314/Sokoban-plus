#pragma once

#include<SFML/Graphics.hpp>
#include<string>
#include"GameLogic.h"



class Render {
private:
	sf::RenderWindow& window;    //界面窗口
	sf::Font font;      //字体
	sf::Text StepText;       //步数文本
	sf::Text WinText;        //胜利文本
	sf::RectangleShape stepBackground;   // 步数半透明背景
	sf::Texture playerTexture;   //玩家图片纹理
	sf::Sprite playerSprite;		//玩家精灵

	sf::Texture targetTexture;   // 目标点图片纹理
	sf::Sprite  targetSprite;    // 目标点精灵

	sf::Texture wallTexture;   // 墙壁图片纹理
	sf::Sprite  wallSprite;    // 墙壁精灵

	sf::Texture boxTexture;               // 普通箱子图片
	sf::Texture boxOnTargetTexture;       // 已到目标点的箱子图片（可选）
	sf::Sprite  boxSprite;                // 用于绘制的箱子精灵
	bool m_hasBoxOnTargetTexture = false; // 是否成功加载目标点箱子图片

	sf::Texture playerWinTexture;   // 胜利后的玩家图片
	bool m_hasWinTexture = false;   // 是否成功加载胜利图片
	//sf::Text inputHint;

	static const int TILE_SIZE = 64;
	
	void drawBox(const std::vector<Position>& aim, const GameLogic& GL);
	void drawCurb(TileType curbType, double x, double y);    //绘制元素
	void drawUI(int steps, bool gameWon);        //绘制UI界面
public:
	explicit Render(sf::RenderWindow& window);      //构造函数
	void render(const GameLogic& GL, const Level& level, int steps, bool gameWon);    //渲染函数
	bool isOpen() const;        //暴露窗口状态
	void close();
	void updateWinTextPos();
	//void setInputHint(const std::string& hint);
	sf::RenderWindow& getWindow() { return window; }        //暴露窗口引用
};