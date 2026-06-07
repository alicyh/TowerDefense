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

#include "Enemy.h"

Enemy* Enemy::create(int type)
{
    Enemy* enemy = new(std::nothrow) Enemy();
    if (enemy && enemy->init(type))
    {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool Enemy::init(int type)
{
    if (!Sprite::init())
    {
        return false;
    }
    
    initEnemy(type);
    
    this->scheduleUpdate();
    
    return true;
}

void Enemy::initEnemy(int type)
{
    switch (type)
    {
        case 0:
            _hp = 100;
            _maxHp = 100;
            _reward = 10;
            _speed = 100.0f;
            this->setColor(Color3B::RED);
            break;
        case 1:
            _hp = 200;
            _maxHp = 200;
            _reward = 25;
            _speed = 80.0f;
            this->setColor(Color3B::BLUE);
            break;
        case 2:
            _hp = 50;
            _maxHp = 50;
            _reward = 5;
            _speed = 150.0f;
            this->setColor(Color3B::YELLOW);
            break;
        default:
            _hp = 100;
            _maxHp = 100;
            _reward = 10;
            _speed = 100.0f;
            this->setColor(Color3B::RED);
            break;
    }
    
    auto drawNode = DrawNode::create();
    drawNode->drawSolidCircle(Vec2::ZERO, 20.0f, 0.0f, 30, Color4F::WHITE);
    drawNode->drawSolidCircle(Vec2::ZERO, 18.0f, 0.0f, 30, Color4F(this->getColor()));
    this->addChild(drawNode, -1);
    
    _currentPathIndex = 0;
    _reachedEnd = false;
}

void Enemy::moveAlongPath(const std::vector<Vec2>& path, float speed)
{
    _path = path;
    _speed = speed;
    _currentPathIndex = 0;
    
    if (!_path.empty())
    {
        this->setPosition(_path[0]);
    }
}

void Enemy::takeDamage(int damage)
{
    _hp -= damage;
    if (_hp <= 0)
    {
        _hp = 0;
    }
}

void Enemy::update(float dt)
{
    if (_reachedEnd || isDead())
    {
        return;
    }
    
    if (_currentPathIndex < _path.size() - 1)
    {
        Vec2 currentPos = this->getPosition();
        Vec2 nextPos = _path[_currentPathIndex + 1];
        Vec2 direction = nextPos - currentPos;
        float distance = direction.length();
        
        if (distance > 1.0f)
        {
            direction.normalize();
            Vec2 newPos = currentPos + direction * _speed * dt;
            
            if (newPos.distance(nextPos) <= _speed * dt)
            {
                newPos = nextPos;
                _currentPathIndex++;
            }
            
            this->setPosition(newPos);
        }
        else
        {
            _currentPathIndex++;
        }
    }
    else
    {
        _reachedEnd = true;
    }
}
