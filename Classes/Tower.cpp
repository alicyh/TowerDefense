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

#include "Tower.h"

Tower* Tower::create(int type)
{
    Tower* tower = new(std::nothrow) Tower();
    if (tower && tower->init(type))
    {
        tower->autorelease();
        return tower;
    }
    CC_SAFE_DELETE(tower);
    return nullptr;
}

bool Tower::init(int type)
{
    if (!Sprite::init())
    {
        return false;
    }
    
    initTower(type);
    this->scheduleUpdate();
    
    return true;
}

void Tower::initTower(int type)
{
    switch (type)
    {
        case 0:
            _range = 150;
            _damage = 20;
            _attackSpeed = 1.0f;
            _cost = 50;
            this->setColor(Color3B::GREEN);
            break;
        case 1:
            _range = 100;
            _damage = 40;
            _attackSpeed = 0.8f;
            _cost = 100;
            this->setColor(Color3B::ORANGE);
            break;
        case 2:
            _range = 200;
            _damage = 15;
            _attackSpeed = 1.5f;
            _cost = 75;
            this->setColor(Color3B::PURPLE);
            break;
        default:
            _range = 150;
            _damage = 20;
            _attackSpeed = 1.0f;
            _cost = 50;
            this->setColor(Color3B::GREEN);
            break;
    }
    
    auto drawNode = DrawNode::create();
    drawNode->drawSolidRect(Vec2(-20, -20), Vec2(20, 20), Color4F::WHITE);
    drawNode->drawSolidRect(Vec2(-18, -18), Vec2(18, 18), Color4F(this->getColor()));
    this->addChild(drawNode, -1);
    
    _rangeIndicator = DrawNode::create();
    _rangeIndicator->drawCircle(Vec2::ZERO, _range, 0.0f, 30, false, Color4F(0, 1, 0, 0.3f));
    _rangeIndicator->setVisible(false);
    this->addChild(_rangeIndicator, -2);
    
    _attackTimer = 0.0f;
}

void Tower::update(float dt)
{
    _attackTimer += dt;
}

void Tower::shoot(Enemy* target)
{
    if (_attackTimer >= _attackSpeed && target && !target->isDead())
    {
        target->takeDamage(_damage);
        _attackTimer = 0.0f;
        
        auto bullet = DrawNode::create();
        bullet->drawSolidCircle(Vec2::ZERO, 5.0f, 0.0f, 10, Color4F::YELLOW);
        bullet->setPosition(this->getPosition());
        this->getParent()->addChild(bullet, 2);
        
        Vec2 targetPos = target->getPosition();
        auto moveTo = MoveTo::create(0.2f, targetPos);
        auto remove = CallFunc::create([bullet]() {
            bullet->removeFromParent();
        });
        auto sequence = Sequence::create(moveTo, remove, nullptr);
        bullet->runAction(sequence);
    }
}

Enemy* Tower::findTarget(const std::vector<Enemy*>& enemies)
{
    Enemy* closestEnemy = nullptr;
    float closestDistance = _range;
    
    for (auto enemy : enemies)
    {
        if (enemy->isDead() || enemy->reachedEnd())
        {
            continue;
        }
        
        float distance = this->getPosition().distance(enemy->getPosition());
        if (distance <= _range && distance < closestDistance)
        {
            closestDistance = distance;
            closestEnemy = enemy;
        }
    }
    
    return closestEnemy;
}

void Tower::showRange()
{
    _rangeIndicator->setVisible(true);
}
