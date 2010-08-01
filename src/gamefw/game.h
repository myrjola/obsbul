/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include <SFML/Window.hpp>

#include "gamefw.h"

#ifndef GAME_H
#define GAME_H


class Game
{
public:
    Game();

    ~Game();

    void update();

private:
    sf::ContextSettings m_main_window_context;
    sf::Window m_main_window;
};

#endif // GAME_H
