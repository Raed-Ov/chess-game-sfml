//main.cpp
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> // for MouseButtonEvent
#include <iostream>
#include <vector>
#include "grid.h"
#include <fstream>
#include <windows.h>
#include <filesystem>


using namespace sf;


int main()
{
    while(std::filesystem::exists("run"))
    if(std::filesystem::exists("backend/render"))
    {
    RenderWindow screen(VideoMode({800, 800}), "Chess Game");
    Grid guiGrid;
    guiGrid.update();
    guiGrid.draw(screen);
    while (std::filesystem::exists("backend/render"))
    {
            auto event=screen.pollEvent();
            if(event) if (event->is<Event::Closed>())break;                          
    }

    screen.close();   
    }


    return 0;
}
