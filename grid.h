#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "sprites.h"
#include <fstream>

using namespace sf;
#pragma once

class Grid
{
    static constexpr float squareSize = 100.0f;
    static constexpr float borderThickness = 5.0f; // Thicker border so grid is visible
    std::string fileGrid[8][8];
    RectangleShape squares[8][8];
    ChessTextures sprites;

    void updateFileGrid()
   {
    std::ifstream data("backend/data.txt");
    for(int i=0;i<8;i++)
       for(int j=0;j<8;j++)
           data>>fileGrid[i][j];       
   }
public:
    Grid()
    {
        for (int row = 0; row < 8; ++row)
            for (int col = 0; col < 8; ++col)
            {
                squares[row][col].setSize({squareSize - borderThickness, squareSize - borderThickness});
                squares[row][col].setPosition({col * squareSize + borderThickness / 2.0f, row * squareSize + borderThickness / 2.0f});
                squares[row][col].setOutlineThickness(borderThickness);
                squares[row][col].setOutlineColor(Color::Black);

                if ((row + col) % 2 == 0)
                    squares[row][col].setFillColor(Color(190,190,190));
                else
                    squares[row][col].setFillColor(Color(190,190,190)); // pink
            }
    }

    void update()
    {
      updateFileGrid();  
      for(int i=0;i<8;i++)
          for(int j=0;j<8;j++)
              squares[i][j].setTexture(&sprites.getSprite(fileGrid[i][j]).getTexture());
    }

    void draw(RenderWindow& screen)
    {
        screen.clear(Color(190,190,190));
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                screen.draw(squares[i][j]);    
        screen.display();
    }
};