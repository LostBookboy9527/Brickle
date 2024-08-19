#include <cmath> 
#include <ctime>
#include <thread>
#include <cstdlib>
#include "MyClass.h"
// #region LineSegment
LineSegment::LineSegment(const Point& startPoint, const Point& endPoint)
    : start(startPoint), end(endPoint) {}

bool LineSegment::isIntersect(const LineSegment& ls1, const LineSegment& ls2) {
    return onOppositeSides(ls1.start, ls1.end, ls2.start, ls2.end) &&
        onOppositeSides(ls2.start, ls2.end, ls1.start, ls1.end);
}

bool LineSegment::isRectIntersect(const Point& current_pos, const Point& next_pos, const sf::FloatRect& brick_rect)
{
    sf::Vector2f top_left(brick_rect.left, brick_rect.top);
    sf::Vector2f top_right(brick_rect.left + brick_rect.width, brick_rect.top);
    sf::Vector2f bottom_left(brick_rect.left, brick_rect.top + brick_rect.height);
    sf::Vector2f bottom_right(brick_rect.left + brick_rect.width, brick_rect.top + brick_rect.height);

    auto pre_reace = LineSegment::LineSegment(current_pos, next_pos);
    auto rect_side1 = LineSegment::LineSegment(top_left, top_right);
    auto rect_side2 = LineSegment::LineSegment(top_right, bottom_right);
    auto rect_side3 = LineSegment::LineSegment(bottom_right, bottom_left);
    auto rect_side4 = LineSegment::LineSegment(bottom_left, top_left);
    return LineSegment::isIntersect(pre_reace, rect_side1) || LineSegment::isIntersect(pre_reace, rect_side2) ||
        LineSegment::isIntersect(pre_reace, rect_side3) || LineSegment::isIntersect(pre_reace, rect_side4);
}

double LineSegment::crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool LineSegment::onOppositeSides(const Point& a, const Point& b, const Point& c, const Point& d) {
    double cp1 = crossProduct(a, b, c);
    double cp2 = crossProduct(a, b, d);
    return (cp1 * cp2 < 0);
}
// #endregion


// #region Brick
Brick::Brick():isVisible(true) {}

Brick::Brick(sf::Vector2f size) : isVisible(true) 
{
    brick.setSize(size);
}

void Brick::setNotVisible() 
{
    isVisible = false;
}
// #endregion


// #region Ball
Ball::Ball() = default;

Ball::Ball(float redius, sf::Vector2f initialVelocity, sf::Vector2f initialPosition)
{
    ballShape.setRadius(redius);
    ballShape.setPosition(initialPosition);
    auto speed = initialVelocity.x * std::sqrt(2);
    ballShape_r = redius;

    // 随机角度范围 [-60°, 60°]
    float angle = (std::rand() % 120+30) * (3.14159265f / 180.0f); // 将角度转换为弧度

    // 根据角度和速率计算新的速度向量
    velocity.y = -speed * std::sin(angle);
    velocity.x = speed * std::cos(angle) * []() {return std::rand() % 2 ? 1 : -1; }();
}

void Ball::move()
{
    ballShape.move(velocity);
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(ballShape);
}

void Ball::collision_detection_window(sf::RenderWindow& window)
{
    if (ballShape.getPosition().x <= 0 + ballShape_r || ballShape.getPosition().x >= window.getSize().x - ballShape_r)
    {
        velocity.x = -velocity.x;//水平反弹
    }
    if (ballShape.getPosition().y <= 0 + ballShape_r)
    {
        velocity.y = -velocity.y;//垂直反弹
    }
}

void Ball::collision_detection_paddle(Paddle& paddle)
{
    /*if (ballShape.getGlobalBounds().intersects(paddle.paddleShape.getGlobalBounds()))
    {
        velocity.y = -velocity.y;
    }*/

    if (ballShape.getGlobalBounds().intersects(paddle.paddleShape.getGlobalBounds())) {
        float paddleCenter = paddle.paddleShape.getPosition().x ;
        float relativeHitPoint = (ballShape.getPosition().x) - paddleCenter;
        float hitRatio = relativeHitPoint / (paddle.paddleShape.getSize().x / 2);//-1（左侧边缘）到 1（右侧边缘）

        // 保持总速度不变
        float originalSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        float minBounceAngle = 20.0f; // 最小反弹角度（以度为单位）
        float originAngle = 90.0f;
        float bounceAngle = std::max(minBounceAngle, std::abs(hitRatio) * originAngle);
        float radian = bounceAngle * (3.14159265f / 180.0f);

        // 根据碰撞位置调整速度向量
        if (std::abs(hitRatio) <= 0.5f)
        {
            velocity.y = -velocity.y;
            return;
        }
        if (hitRatio > 0) { // 撞击挡板右侧
            velocity.x = originalSpeed * std::sin(radian) * (velocity.x > 0 ? 1 : -1);
        }
        else { // 撞击挡板左侧
            velocity.x = originalSpeed * std::sin(radian) * (velocity.x > 0 ? 1 : -1);
        }
        velocity.y = -originalSpeed * std::cos(radian); // y轴向上为负
    }
}

void Ball::collision_detection_brick(std::vector<std::vector<Brick>>& bricks)
{
    sf::Vector2f nextPosition = ballShape.getPosition() + velocity; // 计算下一个位置

    for (auto& row : bricks) 
    {
        for (auto& brick : row) 
        {
            if (!brick.isVisible) continue;

            if (LineSegment::isRectIntersect(ballShape.getPosition(), nextPosition, brick.brick.getGlobalBounds()))
            {
                brick.setNotVisible();
                velocity.y = -velocity.y; // 反转速度
                return;
            }
        }
    }
}

void Ball::collision_detection_over(sf::RenderWindow& window)
{
    if (ballShape.getPosition().y >= window.getSize().y - ballShape_r)
    {
        //游戏结束处理
        window.close();
    }
}
// #endregion


// #region Paddle
Paddle::Paddle() = default;

Paddle::Paddle(sf::Vector2f initialSize, sf::Vector2f initialPosition, float initialSpeed)
{
    speed = initialSpeed;
    paddleShape.setSize(initialSize);
    paddleShape.setOrigin(paddleShape.getSize().x / 2, paddleShape.getSize().y);//设置挡板锚点为底部中央
    paddleShape.setPosition(initialPosition);
}

void Paddle::move(sf::RenderWindow& window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        paddleShape.move(sf::Vector2f(speed, 0));
        if (paddleShape.getPosition().x >= window.getSize().x - paddleShape.getSize().x / 2)
        {
            paddleShape.setPosition(sf::Vector2f(window.getSize().x - paddleShape.getSize().x / 2, paddleShape.getPosition().y));
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        paddleShape.move(sf::Vector2f(-speed, 0));
        if (paddleShape.getPosition().x <= paddleShape.getSize().x / 2)
        {
            paddleShape.setPosition(sf::Vector2f(paddleShape.getSize().x / 2, paddleShape.getPosition().y));
        }
    }
}
// #endregion


// #region Game
Game::Game() : window(sf::VideoMode(800, 600), "Brickle"){}

Game::Game(sf::Vector2u window_size, const std::string& window_name, Ball& ball, Paddle& paddle, std::vector<std::vector<Brick>>& bricks)
    : window(sf::VideoMode(window_size.x, window_size.y), window_name), ball(ball), paddle(paddle), bricks(bricks) {}

void Game::update()
{
    paddle.move(window);
    ball.move();
    ball.collision_detection_window(window);
    ball.collision_detection_paddle(paddle);
    ball.collision_detection_brick(bricks);
    ball.collision_detection_over(window);
}

void Game::render()
{
    window.clear();
    // 绘制游戏对象（球、挡板、砖块）
    window.draw(ball.ballShape);
    window.draw(paddle.paddleShape);
    // 绘制砖块
    for (const auto& row : bricks) {
        for (const auto& brick : row) {
            if (brick.isVisible) {
                window.draw(brick.brick);
            }
        }
    }
    window.display();
}

void Game::run()
{

    while (window.isOpen()) {
        // 处理事件
        update();  // 更新游戏逻辑
        render();  // 渲染游戏画面
    }
}
// #endregion


