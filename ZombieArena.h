#pragma once
#include "Zombie.h"
#include <SFML/Graphics.hpp>


extern sf::RenderWindow *g_window;

int createBackground(sf::VertexArray& rVA, sf::IntRect arena);
Zombie* createHorde(int numZombies, sf::IntRect arena);
__attribute__((used, retain)) void spawn();

