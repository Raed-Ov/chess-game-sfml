//main.cpp
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> // for MouseButtonEvent
#include <iostream>
#include <vector>
#include "grid.h"
#include <fstream>
#include <windows.h>


using namespace sf;


int main()
{
    int render=1;
    std::ifstream data("backend/renderWindow.txt");
    Sleep(100);
    data>>render;

    if(render)
    {
    RenderWindow screen(VideoMode({800, 800}), "Chess Game");
    Grid guiGrid;
    guiGrid.update();
    guiGrid.draw(screen);
    while (render)
    {
            data.seekg(0);
            data>>render;
            auto event=screen.pollEvent();
            if(event) if (event->is<Event::Closed>())break;                          
    }

    data.close();
    screen.close();   
    return 0;
    }
}
