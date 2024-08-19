#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#include "MyClass.h"
float speed = 0.2f;//�����ٶ�
sf::Vector2f paddle_size = sf::Vector2f(150, 7);
sf::Vector2u window_size = sf::Vector2u(800, 600);
float ball_r = 4.0f;//��뾶
float ball_velocity_x = 0.16f;//��x������ٶ�
float ball_velocity_y = 0.16f;//��y������ٶ�
const int w_num = 30;//ש��x������
const int h_num = 30;//ש��y������
const float gap = 2.0f;//ש���϶
bool isGameOver = false;


//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Brickle");//������
//    sf::Event event;
//    sf::RectangleShape paddle; //����
//    sf::CircleShape ball; //��
//    auto brick_size = sf::Vector2f(window.getSize().x / w_num, (window.getSize().y*0.5) / h_num);//ש��ߴ�
//
//    //���õ���
//    paddle.setSize(sf::Vector2f(100, 20));
//    paddle.setOrigin(paddle.getSize().x / 2, paddle.getSize().y); //���õ���ê��Ϊ�ײ�����
//    paddle.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y - 10));
//
//    //������
//    ball.setRadius(ball_r);
//    sf::Vector2f ball_velocity(ball_velocity_x, ball_velocity_y);
//    ball.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddle.getSize().y - ball_r * 2);
//
//
//    //����ש��
//    std::vector<std::vector<Brick>> bricks;
//    bricks.resize(w_num);
//    for (int i = 0; i < w_num; ++i) 
//    {
//        bricks[i].resize(h_num);
//        for (int j = 0; j < h_num; ++j)
//        {
//            Brick brick(brick_size);
//            brick.brick.setOrigin(brick.brick.getSize().x / 2, 0); //����ש��ê��Ϊ��������
//            brick.brick.setPosition(brick.brick.getSize().x * i + brick.brick.getSize().x / 2 + 1 * i + 1, brick.brick.getSize().y * j + 1 * j + 1); //ÿ��ש���������϶
//            bricks[i][j] = brick;
//        }  
//    }
//
//    //��ѭ��
//    while (window.isOpen()) 
//    {
//        while (window.pollEvent(event)) 
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//        
//        //�ƶ�����
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
//        //�ƶ���
//        ball.move(ball_velocity);
//        //����߽����ײ���
//        if (ball.getPosition().x <= 0 + ball_r || ball.getPosition().x >= window.getSize().x - ball_r)
//        {
//            ball_velocity.x = -ball_velocity.x;//ˮƽ����
//        }
//        if (ball.getPosition().y <= 0 + ball_r)
//        {
//            ball_velocity.y = -ball_velocity.y;//��ֱ����
//        }
//        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()))
//        {
//            ball_velocity.y = -ball_velocity.y;
//            //ball_velocity.x = -ball_velocity.x;
//        }
//        
//        //����ש�����ײ���
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
//        //��Ϸ�������
//        if (ball.getPosition().y >= window.getSize().y - ball_r)
//        {
//            //��Ϸ��������
//            window.close();
//        }
//        // ��Ⱦ����
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

//����ש��
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
            brick.brick.setOrigin(brick.brick.getSize().x / 2, 0); //����ש��ê��Ϊ��������
            brick.brick.setPosition(brick.brick.getSize().x * i + brick.brick.getSize().x / 2 + gap * (i + 1), brick.brick.getSize().y * j + gap * (j + 1)); //ÿ��ש���������϶
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
    auto brick_size = sf::Vector2f((window_size.x - gap * (w_num + 1)) / w_num, (window_size.y * 0.5 - gap * h_num) / h_num);//ש��ߴ�
    //initial_pattern();
    auto bricks_pattern = get_bricks_pattern("Lv_1.txt");
    auto bricks = set_bricks(bricks_pattern, brick_size);
    Game game(window_size, "hit brick", ball, paddle, bricks);
    game.run();
}

