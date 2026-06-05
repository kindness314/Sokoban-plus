#include "Menu.h"
#include "GameLogic.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& win, int totalLevels, const std::string& levelBasePath,
    GameLogic& logic, int defaultLevel, std::function<void()> onSwitchBgm)
    : window(win), total(totalLevels), basePath(levelBasePath), m_logic(&logic), m_onSwitchBgm(onSwitchBgm)
{
    if (!font.loadFromFile("./assets/fonts/JYRB.ttf")) {
        std::cerr << "Menu: 字体加载失败" << std::endl;
    }

    // 加载背景图片
    if (m_bgTexture.loadFromFile("./assets/images/clect.png")) {
        m_bgSprite.setTexture(m_bgTexture);
        // 根据窗口大小缩放（在 updateLayout 中动态调整更佳，这里先预设）
        m_bgSprite.setScale(
            static_cast<float>(window.getSize().x) / m_bgTexture.getSize().x,
            static_cast<float>(window.getSize().y) / m_bgTexture.getSize().y
        );
    }


    items.resize(total);
    for (int i = 0; i < total; ++i) {
        items[i].setFont(font);
        items[i].setCharacterSize(m_baseCharSize);
        items[i].setFillColor(sf::Color::White);
    }
    selectedIndex = std::max(0, std::min(defaultLevel - 1, total - 1));
    refresh(); // 初始化显示文本
  
}

void Menu::refresh() {
    // 根据 GameLogic 中的完成状态重建每一关的文本
    for (int i = 0; i < total; ++i) {
        std::wstring text = L"第" + std::to_wstring(i + 1)+L"关";
        if (m_logic) {
            bool completed = m_logic->isLevelCompleted(i);
            if (completed) {
                int best = m_logic->getBestSteps(i);
                text += L" [完成] 所用步数:" + std::to_wstring(best);
            }
        }
        items[i].setString(text);
    }
    m_dirty = true; // 文本改变可能需要重新布局
    updateTexts();
  
}

void Menu::updateTexts() {
    for (int i = 0; i < total; ++i) {
        if (i == selectedIndex) {
            items[i].setFillColor(sf::Color::Yellow);
        }
        else {
            items[i].setFillColor(m_logic->isLevelCompleted(i) ? sf::Color::Green : sf::Color::White);
        }
        // 添加黑色描边，提高对比度
        items[i].setOutlineThickness(2);
        items[i].setOutlineColor(sf::Color::Black);
    }
}
void Menu::updateLayout() {
    sf::Vector2u winSize = window.getSize();
    float winWidth = static_cast<float>(winSize.x);
    float winHeight = static_cast<float>(winSize.y);

    // 1. 基本参数
    const int ROWS_PER_COL = 10;            // 每列最多显示 10 关
    const float colSpacing = 60.0f;         // 列间距
    const float rowSpacing = 55.0f;         // 行间距
    const float marginX = 60.0f;            // 左右边距
    const float marginTop = 60.0f;          // 上边距

    // 2. 计算列数和每列实际行数
    int cols = (total + ROWS_PER_COL - 1) / ROWS_PER_COL; // 至少 1 列
    if (cols < 1) cols = 1;

    // 3. 动态调整字号：以行距和窗口宽度为约束
    int charSize = static_cast<int>(winHeight / (ROWS_PER_COL * 1.8f));
    if (charSize < 22) charSize = 22;
    if (charSize > 40) charSize = 40;

    // 检查列宽是否足够容纳文字
    float maxTextWidth = 0.0f;
    for (int i = 0; i < total; ++i) {
        items[i].setCharacterSize(charSize);
        float w = items[i].getGlobalBounds().width;
        if (w > maxTextWidth) maxTextWidth = w;
    }

    float availableWidth = winWidth - 2 * marginX - (cols - 1) * colSpacing;
    float columnWidth = availableWidth / cols;
    if (columnWidth < maxTextWidth + 20.0f) {
        // 如果文字太宽，缩小字号
        float scale = columnWidth / (maxTextWidth + 20.0f);
        charSize = static_cast<int>(charSize * scale);
        if (charSize < 18) charSize = 18;
    }

    // 4. 设置最终字号
    for (int i = 0; i < total; ++i) {
        items[i].setCharacterSize(charSize);
    }

    // 5. 重新测量实际最大宽度
    maxTextWidth = 0.0f;
    for (int i = 0; i < total; ++i) {
        float w = items[i].getGlobalBounds().width;
        if (w > maxTextWidth) maxTextWidth = w;
    }
    // 列宽取 “文字宽度 + 内边距” 与 “均分可用宽度” 的较大者
    columnWidth = std::max(maxTextWidth + 20.0f, availableWidth / cols);

    // 6. 计算起始 X（整体水平居中）
    float totalWidth = cols * columnWidth + (cols - 1) * colSpacing;
    float startX = (winWidth - totalWidth) / 2.0f;
    if (startX < marginX) startX = marginX;

    // 7. 计算垂直居中
    float totalHeight = ROWS_PER_COL * rowSpacing;
    float startY = (winHeight - totalHeight) / 2.0f;
    if (startY < marginTop) startY = marginTop;

    // 8. 逐项定位
    for (int i = 0; i < total; ++i) {
        int col = i / ROWS_PER_COL;
        int row = i % ROWS_PER_COL;

        float itemWidth = items[i].getGlobalBounds().width;
        // 在列内居中
        float x = startX + col * (columnWidth + colSpacing) + (columnWidth - itemWidth) / 2.0f;
        float y = startY + row * rowSpacing;
        items[i].setPosition(x, y);
    }

    m_dirty = false;

    // 背景图片拉伸
    if (m_bgTexture.getSize().x > 0) {
        float scaleX = winWidth / m_bgTexture.getSize().x;
        float scaleY = winHeight / m_bgTexture.getSize().y;
        m_bgSprite.setScale(scaleX, scaleY);
        m_bgSprite.setPosition(0.0f, 0.0f);
    }
}

void Menu::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            break;
        }
        if (event.type == sf::Event::Resized) {
            // 窗口大小改变，更新布局
            updateLayout();
        }
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Up:
                if (selectedIndex > 0) {
                    selectedIndex--;
                    updateTexts();
                }
                break;
            case sf::Keyboard::Down:
                if (selectedIndex < total - 1) {
                    selectedIndex++;
                    updateTexts();
                }
                break;
            case sf::Keyboard::F2:
                if (m_onSwitchBgm) m_onSwitchBgm();
                break;
            case sf::Keyboard::Enter:
                return; // 退出事件循环
            case sf::Keyboard::Escape:
                m_goBack = true;   // 设置返回标志
                return;            // 退出事件循环

            default:
                break;
            }
        }
    }
}

void Menu::render() {
    if (m_dirty) updateLayout();

    window.clear();
    if (m_bgTexture.getSize().x > 0) {
        window.draw(m_bgSprite);
    }
    else {
        window.clear(sf::Color(30, 30, 30));
    }
    for (const auto& item : items) {
        window.draw(item);
    }
    window.display();
}

int Menu::run() {
    m_goBack = false;
    updateLayout();
    while (window.isOpen()) {
        handleInput();
        updateLayout();
        if (!window.isOpen()) return -1;

        if (m_goBack) {
            return -2;   // 特殊值：返回难度选择
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {}
            return -2;  // 特殊值表示返回难度选择
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            // 等待按键释放，防止重复触发
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
            return selectedIndex + 1; // 关卡编号从 1 开始
        }
        render();
    }
    return -1;
}