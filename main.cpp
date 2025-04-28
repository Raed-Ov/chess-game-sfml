//main.cpp
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> // for MouseButtonEvent
#include <iostream>
#include <vector>
#include "grid.h"


using namespace sf;


int main()
{
    RenderWindow screen(VideoMode({800, 800}), "Chess Game");
    Grid guiGrid;
    
    while (screen.isOpen())
    {
           while (const std::optional<Event> event = screen.pollEvent())
                 if (event ) 
                    if (event->is<Event::Closed>())
                        {screen.close(); return 0;} 
            guiGrid.update();
            guiGrid.draw(screen);                            
    }
        
    return 0;
}
