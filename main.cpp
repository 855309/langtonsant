/*
    author: @fikret0
    date: November 13, 2021

    GitHub: @fikret0
*/

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#define WIDTH 800
#define HEIGHT 800

#define XCELLS 200
#define YCELLS 200

int antx = XCELLS / 2, anty = YCELLS / 2;
int movx = 0, movy = 1;

int cellw = WIDTH / XCELLS, cellh = HEIGHT / YCELLS;

bool stopped = false;

std::vector<std::vector<int>> grid;

sf::RenderWindow mwindow(sf::VideoMode(WIDTH, HEIGHT), "Langton's Ant");

bool darkmode = false;

float defaultdelay = 0.005f;
float fastdelay = 0.f;
float delay = defaultdelay;

/*
    0: make 1 and turn left
    1: make 0 and turn right
*/

sf::Clock gameclock;
void timemanager(){  
    float elapsed = gameclock.getElapsedTime().asSeconds();

    if (elapsed >= delay){
        std::vector<std::vector<int>> tmp = grid;
        int cval = tmp[anty][antx];

        if(cval == 0){
            tmp[anty][antx] = 1;
            if(movx == 0 && movy == 1){
                movx = -1;
                movy = 0;
            }
            else if(movx == 1 && movy == 0){
                movx = 0;
                movy = 1;
            }
            else if(movx == 0 && movy == -1){
                movx = 1;
                movy = 0;
            }
            else if(movx == -1 && movy == 0){
                movx = 0;
                movy = -1;
            }
        }
        else if(cval == 1){
            tmp[anty][antx] = 0;
            if(movx == 0 && movy == 1){
                movx = 1;
                movy = 0;
            }
            else if(movx == 1 && movy == 0){
                movx = 0;
                movy = -1;
            }
            else if(movx == 0 && movy == -1){
                movx = -1;
                movy = 0;
            }
            else if(movx == -1 && movy == 0){
                movx = 0;
                movy = 1;
            }
        }

        grid = tmp;

        if(antx + movx >= 0 && antx + movx < XCELLS){
            antx += movx;
        }
        else{
            movx = -movx;
            antx += movx;
        }
        
        if(anty + movy >= 0 && anty + movy < YCELLS){
            anty += movy;
        }
        else{
            movy = -movy;
            anty += movy;
        }

        gameclock.restart();
    }
}

void initgrid(){
    for(int yc = 0; yc < YCELLS; yc++){
        std::vector<int> ln;
        for(int xc = 0; xc < XCELLS; xc++){
            ln.push_back(0);
        }

        grid.push_back(ln);
    }
}

void cleargrid(){
    grid.clear();
    initgrid();
}

void correcttitle(){
    if(stopped){
        mwindow.setTitle("Langton's Ant - Stopped");
    }
    else{
        mwindow.setTitle("Langton's Ant");
    }
}

void togglestop(){
    stopped = !stopped;
    if(stopped){
        mwindow.setTitle("Langton's Ant - Stopped");
    }
    else{
        mwindow.setTitle("Langton's Ant");
    }
}

int main(int argc, char **argv){
    std::cout << "GitHub: @fikret0 (c) 2021" << std::endl << "Langton's Ant" << std::endl << "--dark for dark mode" << std::endl << std::endl 
        << "Space: Toggle start/stop" << std::endl
        << "C: Clear the grid" << std::endl
        << "Tab: Fast forward" << std::endl;

    for(int ai = 0; ai < argc; ai++){
        std::string arg = argv[ai];
        if(arg == "--dark"){
            darkmode = true;
        }
    }

    sf::Color gridcolor(200, 200, 200);
    sf::Color antcolor(255, 0, 0);
    sf::Color cellcolor(0, 0, 0);
    sf::Color bgcolor(255, 255, 255);

    if(darkmode){
        gridcolor = sf::Color(55, 55, 55);
        cellcolor = sf::Color(255, 255, 255);
        bgcolor = sf::Color(0, 0, 0);
    }

    // togglestop();
    cleargrid();

    while (mwindow.isOpen()){
        sf::Event event;
        while (mwindow.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                mwindow.close();
            }
            else if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Space){ // toggle stop / start
                    togglestop();
                }
                else if(event.key.code == sf::Keyboard::C){ // clear
                    cleargrid();
                }
                else if(event.key.code == sf::Keyboard::Tab){ // fast forward
                    if(delay != fastdelay){
                        delay = fastdelay;
                        mwindow.setTitle("Langton's Ant - Fast Forward");
                    }
                }
            }
            else if(event.type == sf::Event::KeyReleased){
                if(event.key.code == sf::Keyboard::Tab){ // restore default speed
                    delay = defaultdelay;
                    correcttitle();
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased){

            }
        }

        mwindow.clear(bgcolor);

        if(!stopped){
            timemanager();
        }
        else{
            gameclock.restart();
        }

        float th = 0.f;

        /* Draw the grid */
        for(int xc = 0; xc <= XCELLS; xc++){
            int c = cellw * xc;

            sf::RectangleShape line;
            line.setPosition(c, 0);
            line.setSize(sf::Vector2f(th, HEIGHT));

            line.setFillColor(gridcolor);
            line.setOutlineThickness(0);

            mwindow.draw(line);
        }

        for(int yc = 0; yc <= YCELLS; yc++){
            int c = cellh * yc;

            sf::RectangleShape line;
            line.setPosition(0, c);
            line.setSize(sf::Vector2f(WIDTH, th));

            line.setFillColor(gridcolor);
            line.setOutlineThickness(0);

            mwindow.draw(line); 
        }

        /* Draw the cells */
        for(int ln = 0; ln < YCELLS; ln++){
            std::vector<int> l = grid[ln];
            for(int cn = 0; cn < XCELLS; cn++){
                int col = l[cn];

                if(col == 1){
                    sf::RectangleShape cell;
                    cell.setPosition(cn * cellw + th, ln * cellh + th);
                    cell.setSize(sf::Vector2f(cellw - th, cellh - th));

                    cell.setFillColor(cellcolor);
                    cell.setOutlineThickness(0);
                
                    mwindow.draw(cell);
                }
            }
        }

        /* Draw Ant */
        sf::RectangleShape ant;
        ant.setPosition(antx * cellw + th, anty * cellh + th);
        ant.setSize(sf::Vector2f(cellw - th, cellh - th));

        ant.setFillColor(antcolor);
        ant.setOutlineThickness(0);
                
        mwindow.draw(ant);

        mwindow.display();
    }
}