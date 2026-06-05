#include"Render.h"
#include<cmath>

Render::Render(sf::RenderWindow& win) :window(win) {
	font.loadFromFile("./assets/fonts/JYRB.ttf");

	StepText.setFont(font);
	StepText.setCharacterSize(30);
	StepText.setFillColor(sf::Color::White);
	StepText.setPosition(10, 10);

	WinText.setFont(font);
	WinText.setCharacterSize(60);
	WinText.setOutlineThickness(10);
	WinText.setFillColor(sf::Color::Yellow);
	WinText.setString(L"你赢了 !");

	if (!playerTexture.loadFromFile("./assets/images/player1.png")) {
	}
	

	if (playerWinTexture.loadFromFile("./assets/images/player2.png")) {
		m_hasWinTexture = true;
	}
	playerSprite.setTexture(playerTexture);

	if (!wallTexture.loadFromFile("./assets/images/wall.png")) {
		
	}
	wallSprite.setTexture(wallTexture);
	

	if (!boxTexture.loadFromFile("./assets/images/red.png")) {
		
	}

	bool targetLoaded = targetTexture.loadFromFile("./assets/images/target.png");
	if (targetLoaded) {
		targetSprite.setTexture(targetTexture);
		float tsx = (TILE_SIZE * 0.8f) / targetTexture.getSize().x;
		float tsy = (TILE_SIZE * 0.8f) / targetTexture.getSize().y;
		targetSprite.setScale(tsx, tsy);
	}
	else {
		// 可选：设置一个后备标志，在 drawCurb 中不使用精灵
	}

	// 加载已到目标点的箱子图片（如果加载失败，就用普通箱子替代）
	if (boxOnTargetTexture.loadFromFile("./assets/images/green.png")) {
		m_hasBoxOnTargetTexture = true;
	}

	// 初始化箱子精灵（默认使用普通箱子纹理）
	boxSprite.setTexture(boxTexture);

	float wallScaleX = static_cast<float>(TILE_SIZE) / wallTexture.getSize().x;
	float wallScaleY = static_cast<float>(TILE_SIZE) / wallTexture.getSize().y;
	wallSprite.setScale(wallScaleX, wallScaleY);

	float scale = (TILE_SIZE * 0.8f) / playerTexture.getSize().x;  // 假设图片宽高比一致
	playerSprite.setScale(scale, scale);

	stepBackground.setFillColor(sf::Color(0, 0, 0, 150));

	sf::FloatRect r = WinText.getLocalBounds();
	WinText.setOrigin(r.left + r.width / 2.0f, r.top + r.height / 2.0f);
	WinText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	
}
void Render::updateWinTextPos() {
	sf::FloatRect r = WinText.getLocalBounds();
	WinText.setOrigin(r.left + r.width / 2.0f, r.top + r.height / 2.0f);
	WinText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
}
void Render::drawCurb(TileType curbType, double x, double y) {
	sf::RectangleShape rect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	rect.setPosition(x, y);

	//根据curbType设置颜色
	
	//地板

	if (curbType == TileType::Empty || curbType == TileType::Target) {
		rect.setFillColor(sf::Color(50, 50, 50));
		window.draw(rect);
	}

	//墙壁

	if (curbType == TileType::Wall) {
		wallSprite.setPosition(static_cast<float>(x), static_cast<float>(y));
		window.draw(wallSprite);
	}

	if (curbType == TileType::Target) {
		// 居中绘制目标点图片
		sf::FloatRect bounds = targetSprite.getGlobalBounds();
		float offsetX = (TILE_SIZE - bounds.width) / 2.0f;
		float offsetY = (TILE_SIZE - bounds.height) / 2.0f;
		targetSprite.setPosition(x + offsetX, y + offsetY);
		window.draw(targetSprite);
	}
}

void Render::drawBox(const std::vector<Position>& aim, const GameLogic& GL) {
	float desiredSize = TILE_SIZE * 0.8f;  // 箱子占格子的比例，可调整

	for (size_t i = 0; i < aim.size(); ++i) {
		// 根据箱子是否在目标点上切换纹理
		if (GL.isBoxOnTarget(aim[i]) && m_hasBoxOnTargetTexture) {
			boxSprite.setTexture(boxOnTargetTexture);
		}
		else {
			boxSprite.setTexture(boxTexture);
		}

		// 动态缩放，让图片大小一致
		sf::Texture& currentTex = (GL.isBoxOnTarget(aim[i]) && m_hasBoxOnTargetTexture)
			? boxOnTargetTexture : boxTexture;
		float scaleX = desiredSize / currentTex.getSize().x;
		float scaleY = desiredSize / currentTex.getSize().y;
		boxSprite.setScale(scaleX, scaleY);

		// 计算像素位置（格子左上角）
		float px = aim[i].x * TILE_SIZE;
		float py = aim[i].y * TILE_SIZE;

		// 居中绘制箱子
		boxSprite.setPosition(
			px + (TILE_SIZE - boxSprite.getGlobalBounds().width) / 2.0f,
			py + (TILE_SIZE - boxSprite.getGlobalBounds().height) / 2.0f
		);
		window.draw(boxSprite);
	}
}

void Render::drawUI(int steps, bool gameWon) {
	unsigned int stepCharSize = static_cast<unsigned int>(window.getSize().y / 25.0f);
	if (stepCharSize < 12) stepCharSize = 12;
	if (stepCharSize > 40) stepCharSize = 40;   // 防止过大
	StepText.setCharacterSize(stepCharSize);
	StepText.setString((L"步数:") + std::to_wstring(steps));
	// 计算文字边界
	sf::FloatRect textBounds = StepText.getLocalBounds();
	// 调整背景大小（留内边距）
	stepBackground.setSize(sf::Vector2f(textBounds.width + 20, textBounds.height + 10));
	// 定位在右上角
	float bgX = window.getSize().x - stepBackground.getSize().x - 10;
	float bgY = 10;
	stepBackground.setPosition(bgX, bgY);
	StepText.setPosition(bgX + 10, bgY + 5);

	window.draw(stepBackground);
	window.draw(StepText);

	// 胜利：全屏半透明遮罩 + 居中文字
	if (gameWon) {
		
		unsigned int winCharSize = static_cast<unsigned int>(window.getSize().y / 10.0f);
		if (winCharSize < 24) winCharSize = 24;
		if (winCharSize > 80) winCharSize = 80;
		WinText.setCharacterSize(winCharSize);

		sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
		overlay.setFillColor(sf::Color(0, 0, 0, 128));
		window.draw(overlay);

		updateWinTextPos();   // 重新居中文字
		window.draw(WinText);
	}

}

void Render::render(const GameLogic& GL,const Level& level, int steps, bool gameWon) {
	window.clear(sf::Color::Black);

	for (int r = 0; r < level.height(); ++r) {
		for (int c = 0; c < level.width(); ++c) {
			double x = c * TILE_SIZE;
			double y = r * TILE_SIZE;
			Position p = { c, r };
			TileType s = level.tileAt(p);
			drawCurb(s, x, y);
		}
	}

	drawBox(GL.getBoxPositions(),GL);

	Position Pp = GL.getPlayerPos();
	float pixelX = Pp.x * TILE_SIZE;
	float pixelY = Pp.y * TILE_SIZE;

	// 根据胜利状态切换纹理
	if (gameWon && m_hasWinTexture) {
		playerSprite.setTexture(playerWinTexture);
	}
	else {
		playerSprite.setTexture(playerTexture);
	}

	// 重新缩放，保证图片大小一致
	float desiredSize = TILE_SIZE * 0.8f;
	sf::Texture& currentTex = (gameWon && m_hasWinTexture) ? playerWinTexture : playerTexture;
	float scaleX = desiredSize / currentTex.getSize().x;
	float scaleY = desiredSize / currentTex.getSize().y;
	playerSprite.setScale(scaleX, scaleY);

	// 居中绘制
	playerSprite.setPosition(
		pixelX + (TILE_SIZE - playerSprite.getGlobalBounds().width) / 2.0f,
		pixelY + (TILE_SIZE - playerSprite.getGlobalBounds().height) / 2.0f
	);
	window.draw(playerSprite);

	drawUI(steps, gameWon);
	window.display();
}

bool Render::isOpen() const {
	return window.isOpen();
}

void Render::close() {
	window.close();
}
