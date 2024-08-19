#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class LineSegment {
public:
    typedef sf::Vector2f Point;

    LineSegment(const Point& startPoint, const Point& endPoint);

    static bool isIntersect(const LineSegment& ls1, const LineSegment& ls2);

    static bool isRectIntersect(const Point& current_pos, const Point& next_pos, const sf::FloatRect& brick_rect);
private:
    Point start, end;

    static double crossProduct(const Point& a, const Point& b, const Point& c);
    static bool onOppositeSides(const Point& a, const Point& b, const Point& c, const Point& d);
};


class Brick
{
public:
    sf::RectangleShape brick;
    bool isVisible;

    Brick();
    Brick(sf::Vector2f size);

    void setNotVisible();
};


class Paddle
{
public:
    sf::RectangleShape paddleShape;
    sf::Vector2f size;
    float speed;//µ²°åËÙ¶È

    Paddle();
    Paddle(sf::Vector2f initialSize, sf::Vector2f initialPosition, float initialSpeed);

    void move(sf::RenderWindow& window);
};


class Ball
{
public:
    sf::CircleShape ballShape;
    sf::Vector2f velocity;
    float ballShape_r;

    Ball();
    Ball(float redius, sf::Vector2f initialVelocity, sf::Vector2f initialPosition);

    void move();
    void draw(sf::RenderWindow& window);
    void collision_detection_window(sf::RenderWindow& window);
    void collision_detection_paddle(Paddle &paddle);
    void collision_detection_brick(std::vector<std::vector<Brick>>& bricks);
    void collision_detection_over(sf::RenderWindow& window);
};


class Game
{
public:
    sf::RenderWindow window;
    Ball ball;
    Paddle paddle;
    std::vector<std::vector<Brick>> bricks;

    Game();
    Game(sf::Vector2u window_size, const std::string& window_name, Ball& ball, Paddle& paddle, std::vector<std::vector<Brick>>& bricks);

    void update();
    void render();
    void run();
};



