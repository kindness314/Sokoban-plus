#include"gameDo.h"



GameDo::GameDo(sf::RenderWindow& win, const std::string& levelBasePath, int levelNumber,
	const std::string& savePath, GameLogic& extLogic, sf::Music& music,
	std::function<void()> onSwitchBgm)
	: window(win), render(window), logic(extLogic),
	m_levelBasePath(levelBasePath), m_currentLevel(levelNumber),
	m_savePath(savePath), m_music(music),
	m_onSwitchBgm(onSwitchBgm), m_menuVolume(60.0f)
{

	if (!logic.loadLevelByIndex(levelNumber - 1)) {
		window.close();
		return;
	}
	adjustWindow();			//载入关卡

	if(!m_pushBuffer.loadFromFile("./assets/audio/push.wav")) {
		
	}
	m_pushSound.setBuffer(m_pushBuffer);
	m_pushSound.setVolume(70);			//载入推箱子音效

	if (!m_winBuffer.loadFromFile("./assets/audio/win.wav")) {
		// 加载失败不影响游戏
	}
	m_winSound.setBuffer(m_winBuffer);
	m_winSound.setVolume(80);			//载入胜利音效

}



GameDo::Result GameDo::run() {
	m_quitToMenu = false;
	m_quitToDifficulty = false;
	m_justWon = false;

	// 保存进入前的音量，并确保退出时恢复
	float oldVolume = m_music.getVolume();
	m_music.setVolume(20.0f);
	struct VolumeGuard {
		sf::Music& music;
		float restoreVolume;
		~VolumeGuard() { music.setVolume(restoreVolume); }
	} guard{ m_music, oldVolume };

	while (window.isOpen() && render.isOpen()) {
		GameAction action = input.handleInput(window);
		if (action != GameAction::None) {
			processAction(action);
			if (m_quitToMenu) return Result::BackToMenu;      // 析构 guard 恢复音量
			if (m_quitToDifficulty) return Result::BackToDifficulty;
		}
		if (!window.isOpen()) return Result::Quit;
		render.render(logic, logic.getLevel(), logic.getSteps(), logic.isWin());
		trySaveIfWon();
	}
	return Result::Quit;
}


void GameDo::processAction(GameAction action) {

	if (logic.isWin()) {
		if (action == GameAction::MoveUp || action == GameAction::MoveDown ||
			action == GameAction::MoveLeft || action == GameAction::MoveRight ||
			action == GameAction::Undo) {
			return;   // 直接忽略移动和撤销动作
		}
	}

	switch (action) {			//按键后端处理
	case GameAction::MoveUp: {
		auto old = logic.getBoxPositions();
		logic.movePlayer(0, -1);
		if (logic.getBoxPositions() != old) m_pushSound.play();
		break;
	}
	case GameAction::MoveDown: {
		auto old = logic.getBoxPositions();
		logic.movePlayer(0, 1);
		if (logic.getBoxPositions() != old) m_pushSound.play();
		break;
	}
	case GameAction::MoveLeft: {
		auto old = logic.getBoxPositions();
		logic.movePlayer(-1, 0);
		if (logic.getBoxPositions() != old) m_pushSound.play();
		break;
	}
	case GameAction::MoveRight: {
		auto old = logic.getBoxPositions();
		logic.movePlayer(1, 0);
		if (logic.getBoxPositions() != old) m_pushSound.play();
		break;
	}
	case GameAction::SwitchBgm:
		if (m_onSwitchBgm) m_onSwitchBgm();
		break;
	case GameAction::Restart:
		logic.resetLevel();
		m_justWon = false;
		break;
	case GameAction::Quit:
		render.close();
		break;
	case GameAction::Undo:
		logic.undo();
		break;
	case GameAction::NextLevel:
		loadLevelByNumber(m_currentLevel + 1);
		break;
	case GameAction::PrevLevel:
		if (m_currentLevel > 1) loadLevelByNumber(m_currentLevel - 1);
		break;
	case GameAction::BackToMenu:
		m_quitToMenu = true;
		break;
	case GameAction::BackToDifficulty:
		m_quitToDifficulty = true;  
		break;

	default: break;
	
	}
}


void GameDo::adjustWindow() {
	const Level& level = logic.getLevel();
	const int TILE_SIZE = 64;
	int mapWidth = level.width() * TILE_SIZE;
	int mapHeight = level.height() * TILE_SIZE;

	// 获取屏幕当前分辨率
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	// 留出一些边距，避免窗口完全撑满屏幕（如 90% 屏幕可用区域）
	int maxWinWidth = static_cast<int>(desktop.width * 0.9f);
	int maxWinHeight = static_cast<int>(desktop.height * 0.9f);

	// 如果地图尺寸超出最大限制，则按比例缩小（保持格子方形）
	float scale = 1.0f;
	if (mapWidth > maxWinWidth || mapHeight > maxWinHeight) {
		float scaleW = static_cast<float>(maxWinWidth) / mapWidth;
		float scaleH = static_cast<float>(maxWinHeight) / mapHeight;
		scale = std::min(scaleW, scaleH);
	}

	int finalW = static_cast<int>(mapWidth * scale);
	int finalH = static_cast<int>(mapHeight * scale);

	window.setSize(sf::Vector2u(finalW, finalH));
	// 视图范围保持为地图逻辑像素大小，让 SFML 自动缩放
	window.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(mapWidth), static_cast<float>(mapHeight))));
	render.updateWinTextPos();

	sf::Vector2u winSize = window.getSize();
	window.setPosition(sf::Vector2i(
		(desktop.width - static_cast<int>(winSize.x)) / 2,
		(desktop.height - static_cast<int>(winSize.y)) / 2
	));


}

void GameDo::trySaveIfWon() {
	if (!logic.isWin() || m_justWon) return;
	m_justWon = true;
	m_winSound.play();                              // 播放胜利音效
	logic.markCurrentLevelCompleted(logic.getSteps());
	logic.saveToFile(m_savePath);
}


void GameDo::loadLevelByNumber(int num) {
	if (logic.loadLevelByIndex(num - 1)) {
		m_currentLevel = num;
		m_justWon = false;
		adjustWindow();
	}
}
