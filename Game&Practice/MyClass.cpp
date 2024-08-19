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

    // ����Ƕȷ�Χ [-60��, 60��]
    float angle = (std::rand() % 120+30) * (3.14159265f / 180.0f); // ���Ƕ�ת��Ϊ����

    // ���ݽǶȺ����ʼ����µ��ٶ�����
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
        velocity.x = -velocity.x;//ˮƽ����
    }
    if (ballShape.getPosition().y <= 0 + ballShape_r)
    {
        velocity.y = -velocity.y;//��ֱ����
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
        float hitRatio = relativeHitPoint / (paddle.paddleShape.getSize().x / 2);//-1������Ե���� 1���Ҳ��Ե��

        // �������ٶȲ���
        float originalSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        float minBounceAngle = 20.0f; // ��С�����Ƕȣ��Զ�Ϊ��λ��
        float originAngle = 90.0f;
        float bounceAngle = std::max(minBounceAngle, std::abs(hitRatio) * originAngle);
        float radian = bounceAngle * (3.14159265f / 180.0f);

        // ������ײλ�õ����ٶ�����
        if (std::abs(hitRatio) <= 0.5f)
        {
            velocity.y = -velocity.y;
            return;
        }
        if (hitRatio > 0) { // ײ�������Ҳ�
            velocity.x = originalSpeed * std::sin(radian) * (velocity.x > 0 ? 1 : -1);
        }
        else { // ײ���������
            velocity.x = originalSpeed * std::sin(radian) * (velocity.x > 0 ? 1 : -1);
        }
        velocity.y = -originalSpeed * std::cos(radian); // y������Ϊ��
    }
}

void Ball::collision_detection_brick(std::vector<std::vector<Brick>>& bricks)
{
    sf::Vector2f nextPosition = ballShape.getPosition() + velocity; // ������һ��λ��

    for (auto& row : bricks) 
    {
        for (auto& brick : row) 
        {
            if (!brick.isVisible) continue;

            if (LineSegment::isRectIntersect(ballShape.getPosition(), nextPosition, brick.brick.getGlobalBounds()))
            {
                brick.setNotVisible();
                velocity.y = -velocity.y; // ��ת�ٶ�
                return;
            }
        }
    }
}

void Ball::collision_detection_over(sf::RenderWindow& window)
{
    if (ballShape.getPosition().y >= window.getSize().y - ballShape_r)
    {
        //��Ϸ��������
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
    paddleShape.setOrigin(paddleShape.getSize().x / 2, paddleShape.getSize().y);//���õ���ê��Ϊ�ײ�����
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
    // ������Ϸ�����򡢵��塢ש�飩
    window.draw(ball.ballShape);
    window.draw(paddle.paddleShape);
    // ����ש��
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
        // �����¼�
        update();  // ������Ϸ�߼�
        render();  // ��Ⱦ��Ϸ����
    }
}
// #endregion


