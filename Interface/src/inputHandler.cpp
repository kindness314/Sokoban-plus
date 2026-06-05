#include"inputHandler.h"


GameAction InputHandler::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    if (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            return GameAction::Quit;
        }

        if (event.type == sf::Event::KeyPressed) {
            auto key = event.key.code;
            if (pressedKeys.find(key) == pressedKeys.end()) {
                pressedKeys.insert(key);
                return mapKeyToAction(key);
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            pressedKeys.erase(event.key.code);
        }
    }
    return GameAction::None;
}


GameAction InputHandler::mapKeyToAction(sf::Keyboard::Key key) {

    switch (key) {

    case sf::Keyboard::W:
    case sf::Keyboard::Up:
        return GameAction::MoveUp;

        // 下
    case sf::Keyboard::S:
    case sf::Keyboard::Down:
        return GameAction::MoveDown;

        // 左
    case sf::Keyboard::A:
    case sf::Keyboard::Left:
        return GameAction::MoveLeft;

        // 右
    case sf::Keyboard::D:
    case sf::Keyboard::Right:
        return GameAction::MoveRight;

        // 重置游戏
    case sf::Keyboard::R:
        return GameAction::Restart;

        // 退回菜单
    case sf::Keyboard::M:
    case sf::Keyboard::Escape:
        return GameAction::BackToMenu;

    case sf::Keyboard::Z:
    case sf::Keyboard::BackSpace:
        return GameAction::Undo;

        // 下一关
    case sf::Keyboard::N:
        return GameAction::NextLevel;

        // 上一关
    case sf::Keyboard::P:
        return GameAction::PrevLevel;
        //返回难度选择
    case sf::Keyboard::F1:
        return GameAction::BackToDifficulty;

        //切换音乐
    case sf::Keyboard::F2:
        return GameAction::SwitchBgm;
 
    default:
        return GameAction::None;
    }
}
