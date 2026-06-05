#include "GameDo.h"
#include "Menu.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <functional>


void showSplashScreen(sf::RenderWindow& window, const std::string& imagePath,std::function<void()> onSwitchBgm) {
    sf::Texture splashTexture;
    // 加载图片（失败则直接返回，跳过启动画面）
    if (!splashTexture.loadFromFile(imagePath)) {
        return;
    }

    sf::Sprite splashSprite(splashTexture);
    // 缩放至窗口大小
    splashSprite.setScale(
        static_cast<float>(window.getSize().x) / splashTexture.getSize().x,
        static_cast<float>(window.getSize().y) / splashTexture.getSize().y
    );

    window.clear();
    window.draw(splashSprite);
    window.display();

    // 等待 2 秒，同时响应关闭和跳过事件
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            // 按任意键或点击鼠标立即结束启动画面
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F2) {
                    if (onSwitchBgm) onSwitchBgm();   // 切换音乐但不跳过
                    continue;
                }
                return;   // 其他键直接跳过启动画面
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                return;   // 鼠标点击跳过
            }
        }
        if (clock.getElapsedTime().asSeconds() >= 5.0f) {
            return;
        }
    }
}

enum class Difficulty { Easy = 0, Medium, Hard };

Difficulty selectDifficulty(sf::RenderWindow& window, std::function<void()> onSwitchBgm) {
    sf::Font font;
    if (!font.loadFromFile("./assets/fonts/JYRB.ttf")) {
        // 字体加载失败处理，可直接返回默认难度
    }

    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    if (bgTexture.loadFromFile("./assets/images/clect.png")) {
        bgSprite.setTexture(bgTexture);
        bgSprite.setScale(
            static_cast<float>(window.getSize().x) / bgTexture.getSize().x,
            static_cast<float>(window.getSize().y) / bgTexture.getSize().y
        );
    }


    const unsigned int defaultWidth = 1280, defaultHeight = 720;
    window.setSize(sf::Vector2u(defaultWidth, defaultHeight));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        (desktop.width - defaultWidth) / 2,
        (desktop.height - defaultHeight) / 2
    ));
    window.setView(window.getDefaultView());            //设置窗口大小

    sf::Text title(L"选择难度：", font, 80);
    title.setFillColor(sf::Color::White);
    title.setPosition(400, 150);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Black);

    std::vector<sf::Text> options(3);
    options[0].setString(L"简单");
    options[1].setString(L"中等");
    options[2].setString(L"困难");
    int selected = 0;

    for (int i = 0; i < 3; ++i) {
        options[i].setFont(font);
        options[i].setCharacterSize(60);
        options[i].setPosition(530.0f, 270.0f + i * 80.0f);
        //添加黑色描边
        options[i].setOutlineThickness(2);
        options[i].setOutlineColor(sf::Color::Black);
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return static_cast<Difficulty>(-1);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F2) {
                    if (onSwitchBgm) onSwitchBgm();
                    continue;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    if (selected > 0) selected--;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    if (selected < 2) selected++;
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    return static_cast<Difficulty>(selected);
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    return static_cast<Difficulty>(-1);
                }
            }
        }

        // 更新高亮颜色
        for (int i = 0; i < 3; ++i) {
            options[i].setFillColor(i == selected ? sf::Color::Yellow : sf::Color::White);
        }

        window.clear();
        if (bgTexture.getSize().x > 0) {
            window.draw(bgSprite);
        }
        else {
            window.clear(sf::Color(30, 30, 30));
        }
        window.draw(title);
        for (auto& opt : options) window.draw(opt);
        window.display();
    }
    return static_cast<Difficulty>(-1);
}       //难度界面

std::vector<std::string> scanMusicFiles(const std::string& folder) {
    std::vector<std::string> files;
    const std::vector<std::string> extensions = { ".ogg", ".wav", ".flac", ".mp3" };

    std::filesystem::path dirPath(folder);
    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath))
        return files;

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
                files.push_back(entry.path().string());   // 直接 string
            }
        }
    }
    std::sort(files.begin(), files.end());
    return files;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720),L"推箱子");
    window.setFramerateLimit(60);

    // 背景音乐初始化
    std::vector<std::string> bgmFiles = scanMusicFiles("./assets/music");
    sf::Music bgMusic;
    int currentBgmIndex = 0;

    if (!bgmFiles.empty()) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        currentBgmIndex = std::rand() % bgmFiles.size();
        if (!bgMusic.openFromFile(bgmFiles[currentBgmIndex])) {
            currentBgmIndex = 0;
            if (!bgMusic.openFromFile(bgmFiles[0]))
                bgmFiles.clear();
        }
        if (!bgmFiles.empty()) {
            bgMusic.setLoop(true);
            bgMusic.setVolume(60);
            bgMusic.play();
        }
    }

    // ★ 全局音乐切换回调，供所有界面使用
    auto switchBgm = [&]() {
        if (bgmFiles.empty()) return;
        // 保存当前音量，切换后恢复
        float currentVolume = bgMusic.getVolume();
        currentBgmIndex = (currentBgmIndex + 1) % bgmFiles.size();
        bgMusic.stop();
        if (!bgMusic.openFromFile(bgmFiles[currentBgmIndex])) {
            currentBgmIndex = (currentBgmIndex - 1 + bgmFiles.size()) % bgmFiles.size();
            bgMusic.openFromFile(bgmFiles[currentBgmIndex]);
        }
        bgMusic.setLoop(true);
        bgMusic.setVolume(currentVolume);
        bgMusic.play();
        };

    showSplashScreen(window, "./assets/images/background.png", switchBgm);

    while (window.isOpen()) {
        // ---------- 1. 选择难度 ----------
        Difficulty diff = selectDifficulty(window, switchBgm);
        if (!window.isOpen() || static_cast<int>(diff) == -1) break;

        std::string levelFolder;
        std::string saveFile;
        switch (diff) {
        case Difficulty::Easy:
            levelFolder = "./assets/levels/easy";
            saveFile = "./saves/sokoban_easy.sav";
            break;
        case Difficulty::Medium:
            levelFolder = "./assets/levels/medium";
            saveFile = "./saves/sokoban_medium.sav";
            break;
        case Difficulty::Hard:
            levelFolder = "./assets/levels/hard";
            saveFile = "./saves/sokoban_hard.sav";
            break;
        }
        const std::string levelBasePath = levelFolder + "/level";

        // ---------- 2. 初始化 GameLogic ----------
        GameLogic logic;
        auto levelPaths = logic.scanLevels(levelFolder);
        logic.setLevelList(levelPaths);
        logic.loadFromFile(saveFile);
        int totalLevels = logic.getTotalLevels();

        if (totalLevels == 0) {
            // 可以显示错误提示然后返回难度选择
            continue;
        }

        // ---------- 3. 选关与游戏循环 ----------
        bool backToDifficulty = false;
        while (window.isOpen() && !backToDifficulty) {
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                // 等待按键抬起
            }
            int savedLevel = logic.getCurrentLevelIndex() + 1;

            if (savedLevel < 1 || savedLevel > totalLevels) savedLevel = 1;

            // 重置窗口为菜单默认大小
            window.setSize(sf::Vector2u(1280,720));
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            window.setPosition(sf::Vector2i(
                (desktop.width - 1280) / 2,
                (desktop.height - 720) / 2
            ));
            window.setView(window.getDefaultView());  // 恢复默认视图

            Menu menu(window, totalLevels, levelBasePath, logic, savedLevel,switchBgm);
            int selected = menu.run();
            if (!window.isOpen()) break;
            if (selected == -2) {          // 菜单中按了 F1 返回难度选择
                backToDifficulty = true;
                break;
            }
            if (selected == -1) break;     // 窗口关闭

            // 进入游戏
            GameDo game(window, levelBasePath, selected, saveFile, logic, bgMusic, switchBgm);
            GameDo::Result res = game.run();

            if (res == GameDo::Result::BackToDifficulty) {
                backToDifficulty = true;
                break;
            }
            if (res == GameDo::Result::Quit || !window.isOpen()) {
                window.close();   // 确保退出外层循环
                break;
            }
            // BackToMenu 继续循环显示菜单，之前已经保存过进度
            logic.saveToFile(saveFile);
        }

        if (backToDifficulty) {
            // 返回难度选择前保存一次
            logic.saveToFile(saveFile);
            continue;   // 重新开始难度选择
        }
        if (!window.isOpen()) break;
    }
    bgMusic.stop();
    return 0;
}