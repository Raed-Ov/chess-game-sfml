#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> // for MouseButtonEvent
#include <iostream>

using namespace sf;


int main()
{
    RenderWindow screen(VideoMode({800, 800}), "Chess Game");

    while (screen.isOpen())
    {
        while (const std::optional<Event> event = screen.pollEvent())
        {
            if (event) 
            {
                if (event->is<Event::Closed>())
                {
                    screen.close();
                }
                else if (event->is<Event::MouseButtonPressed>())
                {
                    // Access the mouse button event data
                    auto mouseButtonEvent = event->getIf<Event::MouseButtonPressed>();
                    if (mouseButtonEvent->button == Mouse::Button::Left)
                    {
                        int mouseX = mouseButtonEvent->position.x/100;
                        int mouseY = mouseButtonEvent->position.y/100;
                        std::cout << "Left mouse button pressed at: (" << mouseX << ", " << mouseY << ")" << std::endl;
                    }
                }
            }
        }
    }

    return 0;
}
