/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GameScene.h"
#include "AppMacros.h"

USING_NS_CC;

static float PointToLineDistance(const Vec2& point, const Vec2& lineStart, const Vec2& lineEnd)
{
    Vec2 line = lineEnd - lineStart;
    float len2 = line.lengthSquared();
    if (len2 == 0)
    {
        return point.distance(lineStart);
    }
    
    float t = ((point.x - lineStart.x) * line.x + (point.y - lineStart.y) * line.y) / len2;
    t = std::max(0.0f, std::min(1.0f, t));
    
    Vec2 projection = lineStart + line * t;
    return point.distance(projection);
}

Scene* GameScene::scene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    _gold = 200;
    _lives = 10;
    _wave = 0;
    _isPlacingTower = false;
    _selectedTowerType = -1;
    _gridSize = Vec2(60, 60);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = LayerColor::create(Color4B(50, 50, 50, 255));
    this->addChild(background, -1);
    
    initUI();
    initPath();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    this->scheduleUpdate();
    this->schedule(schedule_selector(GameScene::spawnWave), 10.0f, CC_REPEAT_FOREVER, 3.0f);
    
    return true;
}

void GameScene::initUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    _goldLabel = Label::createWithTTF(StringUtils::format("Gold: %d", _gold), "fonts/arial.ttf", 24);
    _goldLabel->setPosition(Vec2(origin.x + 80, origin.y + visibleSize.height - 30));
    this->addChild(_goldLabel, 10);
    
    _livesLabel = Label::createWithTTF(StringUtils::format("Lives: %d", _lives), "fonts/arial.ttf", 24);
    _livesLabel->setPosition(Vec2(origin.x + visibleSize.width - 80, origin.y + visibleSize.height - 30));
    this->addChild(_livesLabel, 10);
    
    _waveLabel = Label::createWithTTF(StringUtils::format("Wave: %d", _wave), "fonts/arial.ttf", 24);
    _waveLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 30));
    this->addChild(_waveLabel, 10);
    
    auto tower1Btn = MenuItemFont::create("Tower 1 (50G)", CC_CALLBACK_1(GameScene::towerButtonCallback, this, 0));
    tower1Btn->setFontSizeObj(20);
    tower1Btn->setPosition(Vec2(origin.x + 80, origin.y + 50));
    
    auto tower2Btn = MenuItemFont::create("Tower 2 (100G)", CC_CALLBACK_1(GameScene::towerButtonCallback, this, 1));
    tower2Btn->setFontSizeObj(20);
    tower2Btn->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 50));
    
    auto tower3Btn = MenuItemFont::create("Tower 3 (75G)", CC_CALLBACK_1(GameScene::towerButtonCallback, this, 2));
    tower3Btn->setFontSizeObj(20);
    tower3Btn->setPosition(Vec2(origin.x + visibleSize.width - 80, origin.y + 50));
    
    auto menu = Menu::create(tower1Btn, tower2Btn, tower3Btn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);
    
    _gameOverLabel = Label::createWithTTF("Game Over!", "fonts/arial.ttf", 48);
    _gameOverLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    _gameOverLabel->setVisible(false);
    this->addChild(_gameOverLabel, 20);
}

void GameScene::initPath()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    _pathPoints.clear();
    _pathPoints.push_back(Vec2(origin.x - 50, origin.y + visibleSize.height / 2));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.2f, origin.y + visibleSize.height / 2));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.2f, origin.y + visibleSize.height * 0.7f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.7f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.3f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.8f, origin.y + visibleSize.height * 0.3f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.8f, origin.y + visibleSize.height / 2));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width + 50, origin.y + visibleSize.height / 2));
    
    _pathDrawNode = DrawNode::create();
    _pathDrawNode->drawPolyline(_pathPoints.data(), _pathPoints.size(), false, Color4F(139, 69, 19, 255));
    _pathDrawNode->drawPolygon(_pathPoints.data(), _pathPoints.size(), Color4F(160, 82, 45, 200), 1, Color4F(139, 69, 19, 255));
    this->addChild(_pathDrawNode, 1);
}

void GameScene::spawnEnemy()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    int type = rand() % 3;
    auto enemy = Enemy::create(type);
    enemy->moveAlongPath(_pathPoints, enemy->_speed);
    enemy->setPosition(_pathPoints[0]);
    
    this->addChild(enemy, 5);
    _enemies.push_back(enemy);
}

void GameScene::spawnWave()
{
    _wave++;
    _waveLabel->setString(StringUtils::format("Wave: %d", _wave));
    
    int enemyCount = 5 + _wave * 2;
    float delay = 0.0f;
    
    for (int i = 0; i < enemyCount; i++)
    {
        this->scheduleOnce([this](float dt) {
            if (_lives > 0)
            {
                spawnEnemy();
            }
        }, delay, "spawnEnemy");
        delay += 0.8f;
    }
}

void GameScene::placeTower(const Vec2& position)
{
    auto tower = Tower::create(_selectedTowerType);
    if (_gold >= tower->getCost())
    {
        Vec2 gridPos = Vec2(floor(position.x / _gridSize.x) * _gridSize.x + _gridSize.x / 2,
                            floor(position.y / _gridSize.y) * _gridSize.y + _gridSize.y / 2);
        
        if (!isPositionOccupied(gridPos) && !isPositionOnPath(gridPos))
        {
            tower->setPosition(gridPos);
            this->addChild(tower, 3);
            _towers.push_back(tower);
            _occupiedPositions.push_back(gridPos);
            _gold -= tower->getCost();
            _goldLabel->setString(StringUtils::format("Gold: %d", _gold));
        }
    }
    
    _isPlacingTower = false;
    _selectedTowerType = -1;
}

void GameScene::towerButtonCallback(Ref* sender, int type)
{
    auto tower = Tower::create(type);
    if (_gold >= tower->getCost())
    {
        _isPlacingTower = true;
        _selectedTowerType = type;
    }
}

bool GameScene::isPositionOccupied(const Vec2& position)
{
    for (auto pos : _occupiedPositions)
    {
        if (pos.distance(position) < 10.0f)
        {
            return true;
        }
    }
    return false;
}

bool GameScene::isPositionOnPath(const Vec2& position)
{
    for (int i = 0; i < _pathPoints.size() - 1; i++)
    {
        Vec2 p1 = _pathPoints[i];
        Vec2 p2 = _pathPoints[i + 1];
        
        float distance = PointToLineDistance(position, p1, p2);
        if (distance < 50.0f)
        {
            return true;
        }
    }
    return false;
}

void GameScene::checkGameOver()
{
    if (_lives <= 0)
    {
        _gameOverLabel->setVisible(true);
        this->unscheduleAllCallbacks();
    }
}

bool GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    auto touch = touches[0];
    Vec2 location = touch->getLocation();
    
    if (_isPlacingTower)
    {
        placeTower(location);
        return true;
    }
    
    return false;
}

void GameScene::update(float dt)
{
    if (_lives <= 0)
    {
        return;
    }
    
    for (auto tower : _towers)
    {
        auto target = tower->findTarget(_enemies);
        if (target)
        {
            tower->shoot(target);
        }
    }
    
    for (auto it = _enemies.begin(); it != _enemies.end();)
    {
        auto enemy = *it;
        
        if (enemy->reachedEnd())
        {
            _lives--;
            _livesLabel->setString(StringUtils::format("Lives: %d", _lives));
            enemy->removeFromParent();
            it = _enemies.erase(it);
            checkGameOver();
        }
        else if (enemy->isDead())
        {
            _gold += enemy->getReward();
            _goldLabel->setString(StringUtils::format("Gold: %d", _gold));
            enemy->removeFromParent();
            it = _enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
