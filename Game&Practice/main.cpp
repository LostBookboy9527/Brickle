#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#include "MyClass.h"
float speed = 0.2f;//挡板速度
sf::Vector2f paddle_size = sf::Vector2f(150, 7);
sf::Vector2u window_size = sf::Vector2u(800, 600);
float ball_r = 4.0f;//球半径
float ball_velocity_x = 0.16f;//球x轴飞行速度
float ball_velocity_y = 0.16f;//球y轴飞行速度
const int w_num = 30;//砖块x轴数量
const int h_num = 30;//砖块y轴数量
const float gap = 2.0f;//砖块间隙
bool isGameOver = false;


//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Brickle");//主窗口
//    sf::Event event;
//    sf::RectangleShape paddle; //挡板
//    sf::CircleShape ball; //球
//    auto brick_size = sf::Vector2f(window.getSize().x / w_num, (window.getSize().y*0.5) / h_num);//砖块尺寸
//
//    //设置挡板
//    paddle.setSize(sf::Vector2f(100, 20));
//    paddle.setOrigin(paddle.getSize().x / 2, paddle.getSize().y); //设置挡板锚点为底部中央
//    paddle.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y - 10));
//
//    //设置球
//    ball.setRadius(ball_r);
//    sf::Vector2f ball_velocity(ball_velocity_x, ball_velocity_y);
//    ball.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddle.getSize().y - ball_r * 2);
//
//
//    //设置砖块
//    std::vector<std::vector<Brick>> bricks;
//    bricks.resize(w_num);
//    for (int i = 0; i < w_num; ++i) 
//    {
//        bricks[i].resize(h_num);
//        for (int j = 0; j < h_num; ++j)
//        {
//            Brick brick(brick_size);
//            brick.brick.setOrigin(brick.brick.getSize().x / 2, 0); //设置砖块锚点为顶部中央
//            brick.brick.setPosition(brick.brick.getSize().x * i + brick.brick.getSize().x / 2 + 1 * i + 1, brick.brick.getSize().y * j + 1 * j + 1); //每块砖块间留出空隙
//            bricks[i][j] = brick;
//        }  
//    }
//
//    //主循环
//    while (window.isOpen()) 
//    {
//        while (window.pollEvent(event)) 
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//        
//        //移动挡板
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//        {
//            paddle.move(sf::Vector2f(speed, 0));
//            if (paddle.getPosition().x >= window.getSize().x - paddle.getSize().x / 2)
//            {
//                paddle.setPosition(sf::Vector2f(window.getSize().x - paddle.getSize().x / 2, paddle.getPosition().y));
//            }
//        }
//        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
//        {
//            paddle.move(sf::Vector2f(-speed, 0));
//            if (paddle.getPosition().x <= paddle.getSize().x / 2)
//            {
//                paddle.setPosition(sf::Vector2f(paddle.getSize().x / 2, paddle.getPosition().y));
//            }
//        }
//
//        //移动球
//        ball.move(ball_velocity);
//        //球与边界的碰撞检测
//        if (ball.getPosition().x <= 0 + ball_r || ball.getPosition().x >= window.getSize().x - ball_r)
//        {
//            ball_velocity.x = -ball_velocity.x;//水平反弹
//        }
//        if (ball.getPosition().y <= 0 + ball_r)
//        {
//            ball_velocity.y = -ball_velocity.y;//垂直反弹
//        }
//        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()))
//        {
//            ball_velocity.y = -ball_velocity.y;
//            //ball_velocity.x = -ball_velocity.x;
//        }
//        
//        //球与砖块的碰撞检测
//        for (int i = 0; i < w_num; ++i)
//        {
//            for (int j = 0; j < h_num; ++j)
//            {
//                if (bricks[i][j].isVisible == true && ball.getGlobalBounds().intersects(bricks[i][j].brick.getGlobalBounds()))
//                {
//                    bricks[i][j].isVisible = false;
//                    ball_velocity.y = -ball_velocity.y;
//                }
//            }
//        }
//        //游戏结束检测
//        if (ball.getPosition().y >= window.getSize().y - ball_r)
//        {
//            //游戏结束处理
//            window.close();
//        }
//        // 渲染操作
//        window.clear();
//        
//        for (int i = 0; i < w_num; ++i)
//        {
//            for (int j = 0; j < h_num; ++j)
//            {
//                if(bricks[i][j].isVisible==true)
//                    window.draw(bricks[i][j].brick);
//            }
//        }
//        window.draw(ball);
//        window.draw(paddle);
//        window.display();
//    }
//    return 0;
//}

void initial_pattern()
{
    std::string pattern_name("Lv_1.txt");
    std::ofstream file(pattern_name);

    if (!file) 
    {
        std::cerr << "Unable to open file " << pattern_name << std::endl;
        return;
    }

    for (int i = 0; i < h_num; ++i) 
    {
        for (int j = 0; j < w_num; ++j) 
        {
            file << '*';
        }
        file << '\n';
    }

    file.close();
    std::cout << "Matrix written to " << pattern_name << std::endl;
}

//创建砖块
auto get_bricks_pattern(const std::string& file_name)
{
    std::ifstream file(file_name);
    if (!file)
    {
        std::cerr<< "Unable to open file" << std::endl;
        std::exit(1);
    }
    std::vector<std::vector<char>> bricks_pattern; 
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<char> row;
        for (char ch : line) {
            row.push_back(ch);
        }
        bricks_pattern.push_back(row);
    }
    file.close();

    return bricks_pattern;
}
auto set_bricks(std::vector<std::vector<char>> bricks_pattern, sf::Vector2f brick_size)
{
    std::vector<std::vector<Brick>> bricks;
    bricks.resize(w_num);

    for (int i = 0; i < w_num; ++i)
    {
        bricks[i].resize(h_num);
        for (int j = 0; j < h_num; ++j)
        {
            Brick brick(brick_size);
            brick.brick.setOrigin(brick.brick.getSize().x / 2, 0); //设置砖块锚点为顶部中央
            brick.brick.setPosition(brick.brick.getSize().x * i + brick.brick.getSize().x / 2 + gap * (i + 1), brick.brick.getSize().y * j + gap * (j + 1)); //每块砖块间留出空隙
            if (bricks_pattern[i][j] == '*')
            {
                brick.isVisible = true;
            }
            else
            {
                brick.isVisible = false;
            }
            bricks[i][j] = brick;
        }
    }
    return bricks;
}


int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Paddle paddle(paddle_size, sf::Vector2f(window_size.x / 2, window_size.y - 10), speed);
    Ball ball(ball_r, sf::Vector2f(ball_velocity_x, ball_velocity_y), sf::Vector2f(paddle.paddleShape.getPosition().x, paddle.paddleShape.getPosition().y - paddle.paddleShape.getSize().y - ball_r * 2));
    auto brick_size = sf::Vector2f((window_size.x - gap * (w_num + 1)) / w_num, (window_size.y * 0.5 - gap * h_num) / h_num);//砖块尺寸
    //initial_pattern();
    auto bricks_pattern = get_bricks_pattern("Lv_1.txt");
    auto bricks = set_bricks(bricks_pattern, brick_size);
    Game game(window_size, "hit brick", ball, paddle, bricks);
    game.run();
}

