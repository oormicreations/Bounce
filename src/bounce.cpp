#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

float getRandomOffset()
{
    float sign = ((float)std::rand() / RAND_MAX) > 0.5 ? 1.0 : -1.0;
    return sign * 0.075 * ((float)std::rand() / RAND_MAX);
}

int main()
{
    float x_pos = 0.0;
    float y_pos = 0.0;
    float x_max = 600.0;
    float y_max = 300.0;
    float shape_sz = 20.0;
    float x_dir = 1.0;
    float y_dir = 1.0;
    float x_speed = 4.0;
    float y_speed = 4.0;

    float x_screen = sf::VideoMode::getDesktopMode().width;
    float y_screen = sf::VideoMode::getDesktopMode().height;

    bool hide_text = false;
    sf::Vector2f mblur[10];

    sf::RenderWindow window(sf::VideoMode(x_max, y_max), "Bounce!");
    window.setPosition(sf::Vector2i((x_screen / 2) - (x_max / 2), (y_screen / 2) - (y_max / 2)));
    window.setFramerateLimit(30);

    sf::CircleShape shape(shape_sz);
    shape.setFillColor(sf::Color::Yellow);

    sf::Font font;
    if (!font.loadFromFile("./res/Ubuntu-L.ttf"))
    {
        std::cout << "Could not load font" << std::endl;
    }

    sf::Text x_pos_text;
    x_pos_text.setFont(font);
    x_pos_text.setCharacterSize(15);
    x_pos_text.setFillColor(sf::Color(80, 80, 80));
    x_pos_text.setPosition(sf::Vector2f(x_max - 100, 10));

    sf::Text y_pos_text(x_pos_text);
    y_pos_text.setPosition(sf::Vector2f(x_max - 100, 25));

    sf::Text speed_text(x_pos_text);
    speed_text.setPosition(sf::Vector2f(x_max - 100, 40));

    sf::Text sz_text(x_pos_text);
    sz_text.setPosition(sf::Vector2f(x_max - 100, 55));

    sf::Text help_text(x_pos_text);
    help_text.setPosition(sf::Vector2f(10, y_max - 25));
    help_text.setString("Use Up-Dn arrow keys for speed change, L-R for size, 'h' to hide text");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                x_speed -= 0.1;
                y_speed -= 0.1;
            };

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                x_speed += 0.1;
                y_speed += 0.1;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                shape_sz += 1.0;
                if (shape_sz > y_max / 3)
                    shape_sz = y_max / 3;
                shape.setRadius(shape_sz);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                shape_sz -= 1.0;
                if (shape_sz < 5)
                    shape_sz = 5;
                shape.setRadius(shape_sz);
            }

            //Never use isKeyPressed event for catching characters
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == 0x68) //h
                {
                    //std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
                    hide_text = !hide_text;
                }
            }
        }

        window.clear(sf::Color(33, 40, 44));

        x_pos += x_speed * x_dir;
        y_pos += y_speed * y_dir;

        sf::Vector2f pos(x_pos, y_pos);
        for (int i = 8; i >= 0; i--)
        {
            mblur[i + 1] = mblur[i];
        }
        mblur[0] = pos;

        if (((int)x_pos > (int)(x_max - 2 * shape_sz)) || (x_pos < 0))
        {
            x_dir = -x_dir + getRandomOffset();
        }

        if (((int)y_pos > (int)(y_max - 2 * shape_sz)) || (y_pos < 0))
        {
            y_dir = -y_dir + getRandomOffset();
        }

        // shape.setPosition(sf::Vector2f(x_pos, y_pos));
        // window.draw(shape);

        for (int i = 0; i < 10; i++)
        {
            shape.setPosition(mblur[i]);
            float div = (float)(i*i*i)/2.0;
            shape.setFillColor(sf::Color(255, 255, 0, 255/(div+1)));
            window.draw(shape);
        }

        if (!hide_text)
        {
            x_pos_text.setString(std::to_string(x_pos));
            y_pos_text.setString(std::to_string(y_pos));
            speed_text.setString(std::to_string(x_speed));
            sz_text.setString(std::to_string(shape_sz));

            window.draw(x_pos_text);
            window.draw(y_pos_text);
            window.draw(speed_text);
            window.draw(sz_text);
            window.draw(help_text);
        }

        window.display();
    }

    return 0;
}
