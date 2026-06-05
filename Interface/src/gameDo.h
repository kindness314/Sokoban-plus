#pragma once

#include"GameLogic.h"
#include"Render.h"
#include"inputHandler.h"
#include<string>
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
class GameDo {
public:
	enum class Result { Quit, BackToMenu, BackToDifficulty };
	//难度枚举
	GameDo(sf::RenderWindow& win, const std::string& levelBasePath, int levelNumber,
		const std::string& savePath, GameLogic& logic, sf::Music& music,
		std::function<void()> onSwitchBgm);
	Result run();

private:
	sf::RenderWindow& window;
	Render render;
	GameLogic& logic;
	InputHandler input;

	sf::Music& m_music;
	std::function<void()> m_onSwitchBgm;

	sf::SoundBuffer m_pushBuffer;   // 推动音效的音频缓冲区
	sf::Sound       m_pushSound;    // 推动音效的播放器

	sf::SoundBuffer m_winBuffer;   // 胜利音效缓冲区
	sf::Sound       m_winSound;    // 胜利音效播放器

	
	float m_menuVolume;    // 菜单音量（用于恢复）

	bool m_quitToDifficulty = false;

	std::string m_levelBasePath; // 关卡文件前缀
	int m_currentLevel;          // 当前关卡号
	bool m_quitToMenu = false;

	std::string m_savePath;   // 存档文件路径
	bool m_justWon = false;   // 防止重复保存
	void trySaveIfWon();      // 检查胜利并保存存档

	void processAction(GameAction action);
	void adjustWindow();
	void loadLevelByNumber(int num);
};