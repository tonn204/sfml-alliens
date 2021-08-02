#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main()
{
    const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
    const float PLAYER_Y_POS = 500.0f;

    int score = 0;

    int score_table[8] = {
        30, 60, 90, 120,
        150, 180, 210, 240
    };

    float enemy_velocity = 0.1f;
    float player_velocity = 0.5f;

    bool playe_defeat_sound = false;
    bool game_paused = false;
    bool space_pressed = false;
    bool bullet_is_collyding = false;
    bool enemy_going_right[5] = {
        true, true, true, true, true
    };

    bool score_up[8] = {
        false, false, false, false,
        false, false, false, false
    };

    std::vector<sf::Sprite> bullets;

    // Window init
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Aliens invasion");
    window.setVerticalSyncEnabled(true); /* Necesary for correct working of deltaTime,
                                         alternative window.setFrameRate(n); n - number of frames*/

    sf::Font my_font;
    if (!my_font.loadFromFile("myFont.ttf"))
    {
        std::cout << "Failed to load a font" << std::endl;
    }

    sf::Text text;
    text.setFont(my_font);
    text.setCharacterSize(20);
    std::string score_text;

    sf::Text defeat_text;
    defeat_text.setFont(my_font);
    defeat_text.setCharacterSize(15);
    defeat_text.setPosition(sf::Vector2f(220.0f, 300.0f));

    sf::Music main_music;
    if (!main_music.openFromFile("main_music.wav"))
    {
        std::cout << "Failed to load main_music.wav" << std::endl;
    }
   
    main_music.setLoop(true);
    main_music.play();

    sf::SoundBuffer explosion_buffer;
    if (!explosion_buffer.loadFromFile("explosion.wav"))
    {
        std::cout << "Failed to load file explosion.wav" << std::endl;
    }
    sf::Sound explosion_sound;
    explosion_sound.setBuffer(explosion_buffer);
    
    sf::SoundBuffer defeat_buffer;
    if (!defeat_buffer.loadFromFile("defeat.wav"));
    sf::Sound defeat_sound;
    defeat_sound.setBuffer(defeat_buffer);

    sf::SoundBuffer score_up_buffer;
    if (!score_up_buffer.loadFromFile("score_up.wav"));
    sf::Sound score_up_sound;
    score_up_sound.setBuffer(score_up_buffer);

    sf::Texture background_texture;
    if (!background_texture.loadFromFile("background.jpg"))
    {
        std::cout << "Failed to load background" << std::endl;
    }

    sf::Sprite backgorund(background_texture, sf::IntRect(0, 0, 800, 600));

    // Player init
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("player.png"))
    {
        std::cout << "Failed to laod player texture" << std::endl;
    }
    sf::Sprite player;
    player.setTexture(player_texture);
    player.setPosition(sf::Vector2f(400.0f, PLAYER_Y_POS));
    player.setScale(sf::Vector2f(2.0f, 2.0f));

    sf::Texture bullet_texture;
    if (!bullet_texture.loadFromFile("bullet.png"))
    {
        std::cout << "Failed to load bullet texture" << std::endl;
    }
    sf::Sprite bullet;

    sf::Texture enemy_texture;
    if (!enemy_texture.loadFromFile("enemy.png"))
    {
        std::cout << "Failed to load enemy image" << std::endl;
    }

    srand(time(NULL));

    sf::Sprite enemies[5];
    for (int i = 0; i < 5; i++)
    {
        enemies[i].setTexture(enemy_texture);
        enemies[i].scale(sf::Vector2f(2.0f, 1.5f));
        enemies[i].setPosition(sf::Vector2f(float(rand() % 740 + 0), float(rand() % 300)));
    }

    sf::Clock game_clock;

    while (window.isOpen())
    {
        float deltaTime = game_clock.restart().asMilliseconds();   

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (event.key.code == sf::Keyboard::Space && !space_pressed)
                {
                    if (!game_paused)
                    {
                        space_pressed = true;
                        std::cout << "Space pressed!!!" << std::endl;
                        bullets.push_back(bullet);
                        bullets.back().setPosition(sf::Vector2f(player.getPosition().x + 16, PLAYER_Y_POS));
                        bullets.back().setTexture(bullet_texture);
                        std::cout << bullets.size() << std::endl;
                    }
                    else
                    {
                        game_paused = false;
                        playe_defeat_sound = false;
                        score = 0;
                        player_velocity = 0.5f;
                        for (int i = 0; i < 8; i++)
                        {
                            score_up[i] = false;
                        }
                        enemy_velocity = 0.1f;
                        for (int i = 0; i < 5; i++)
                        {
                            enemies[i].setPosition(sf::Vector2f(float(rand() % 740 + 0), float(rand() % 300)));
                        }
                        player.setPosition(sf::Vector2f(400.0f, PLAYER_Y_POS)); 
                        main_music.play();
                    }

                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Space && space_pressed)
                {
                    space_pressed = false;
                }
                break;
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.move(sf::Vector2f(-player_velocity * deltaTime, 0.0f));
            if (player.getPosition().x <= 0.0f)
                player.setPosition(sf::Vector2f(0.0f, PLAYER_Y_POS));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.move(sf::Vector2f(player_velocity * deltaTime, 0.0f));
            if (player.getPosition().x >= 740.0f)
                player.setPosition(sf::Vector2f(740.0f, PLAYER_Y_POS));
        }

        for (int i = 0; i < 5; i++)
        {
            if (enemy_going_right[i])
            {
                enemies[i].move(sf::Vector2f(enemy_velocity * deltaTime, 0.0f));
                if (enemies[i].getPosition().x >= 740.0f)
                {
                    enemies[i].setPosition(sf::Vector2f(740.0f, enemies[i].getPosition().y + 10.0f));
                    enemy_going_right[i] = false;
                }              
            }
            else
            {
                enemies[i].move(sf::Vector2f(-(enemy_velocity) * deltaTime, 0.0f));
                if (enemies[i].getPosition().x <= 0.0f)
                {
                    enemies[i].setPosition(sf::Vector2f(0.0f, enemies[i].getPosition().y + 10.0f));
                    enemy_going_right[i] = true;
                }
            }
        }

        for (int i = 0; i < bullets.size(); i++)
        {      
            bullets[i].move(0.0f, -1.0f * deltaTime);
            for (int z = 0; z < 5; z++)
            {
                if (enemies[z].getGlobalBounds().intersects(bullets[i].getGlobalBounds()))
                {
                    explosion_sound.play();
                    enemies[z].setPosition(sf::Vector2f(float(rand() % 740 + 0), float(rand() % 300)));
                    score++;
                    bullets.erase(bullets.begin() + i);
                    bullet_is_collyding = true;
                    break;
                }
            }
            if (bullet_is_collyding)
                break;

            if (bullets[i].getPosition().y <= 0.0f)
                bullets.erase(bullets.begin() + i);
        }    

        bullet_is_collyding = false;

        score_text = std::to_string(score);
        text.setString("Score: " + score_text);
        
      
        for (int i = 0; i < 8; i++)
        {
            if (score == score_table[i] && !score_up[i])
            {
                score_up[i] = true;
                enemy_velocity += 0.1f;
                score_up_sound.play();
            }
        } 

        for (int i = 0; i < 5; i++)
        {
            if (enemies[i].getPosition().y >= 420.0f)
            {
                enemy_velocity = 0.0f;
                player_velocity = 0.0f;
                game_paused = true;
                if (!playe_defeat_sound)
                {
                    playe_defeat_sound = true;
                    main_music.stop();
                    defeat_sound.play();
                }
            }
        }

        defeat_text.setString("YOU LOOSE, YOUR SCORE WAS: " + score_text + "\n" + "Press space to continue");

        window.clear();
        window.draw(backgorund);
        window.draw(player);
        for (int i = 0; i < 5; i++)
        {
            window.draw(enemies[i]);
        }
        for (int i = 0; i < bullets.size(); i++)
        {
            window.draw(bullets[i]);
        }      
        if (!game_paused)
            window.draw(text);
        else
            window.draw(defeat_text);
        window.display();
    }
    return 0;
}