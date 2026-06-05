#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
class GameLogic; // 前向声明

class Menu {
public:
    // 增加 GameLogic 引用，用于获取完成状态
    Menu(sf::RenderWindow& win, int totalLevels, const std::string& levelBasePath,
        GameLogic& logic,int defaultLevel = 1, std::function<void()> onSwitchBgm = nullptr);
    int run();

    // 切换关联的 GameLogic（切换难度时使用）
    void setLevelLogic(GameLogic& newLogic) { m_logic = &newLogic; refresh(); }

private:
    sf::RenderWindow& window;
    sf::Font font;
    std::vector<sf::Text> items;
    int total;
    std::string basePath;
    GameLogic* m_logic;
    std::function<void()> m_onSwitchBgm;

    bool m_goBack = false;

    sf::Texture m_bgTexture;   // 背景纹理
    sf::Sprite  m_bgSprite;    // 背景精灵

    int selectedIndex = 0;
    bool m_dirty = true;          // 用于标记需要更新布局
    int m_baseCharSize = 35;      // 基准字体大小

    void updateTexts();           // 刷新文字内容和颜色
    void refresh();               // 重新生成所有 item 文本
    void handleInput();
    void render();
    // 新增：根据窗口大小调整所有文本位置和字体大小
    void updateLayout();
};