#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourcePath.hpp"
#include <fstream>
#include <ctime>

void textConstructor(sf::Text *text, int size, std::string message, int xPos, int yPos, sf::Font *font)
{
 text->setFont(*font);
 text->setCharacterSize(size);
 text->setFillColor(sf::Color::White);
 text->setString(message);
 text->setPosition(xPos, yPos);
}

void reset(bool &start, sf::Clock &clock, std::vector<float> &lapTime, sf::Text &lap, float &lastTime, sf::Text &message)
{
    start = false;
    clock.restart();
    lapTime.clear();
    lapTime.push_back(0);
    lap.setString("Laps");
    lastTime = 0;
    message.setString("Time(s):0.000000");
}

std::string getCurrentTime()
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    return std::ctime(&end_time);
}

int main()
{
    //vars
    bool start = false;
    bool restart = false;
    float lastTime = 0;
    int startIndex = 0;
    
    //Keeps track of lap times
    std::vector<float> lapTime;
    lapTime.push_back(0);
    
    //Record file that stores times
    std::ofstream records;
    
    //Font properties
    sf::Font font;
    //Avaliable fonts: "sansation.ttf", "CamingoCode-Regular.ttf"
    std::string fontStyle = "CamingoCode-Regular.ttf";
    font.loadFromFile(resourcePath() + fontStyle);
    
    //Size with title
    sf::RenderWindow window(sf::VideoMode(820, 490), "Stopwatch", sf::Style::Close);
    window.setKeyRepeatEnabled(true);
    window.sf::Window::setFramerateLimit(60);
    
    //Message properties
    sf::Text timeText;
    textConstructor(&timeText, 75, "Time(s):0.000000", 20, 0, &font);
    
    sf::Text instructionsText;
    textConstructor(&instructionsText, 50, "Inputs\n[1] Start/Stop\n[2] Lap\n[3] Restart\n[4] Record", 20, 150, &font);
    
    sf::Text lapText;
    textConstructor(&lapText, 50, "Laps", 500, 150, &font);
    
    //Starts the clock
    sf::Clock clock;
    
    //Main loop
    while (window.isOpen())
    {
        sf::Event event;
        
        //Sets message to time if start = true
        if(start)
        {
            if(restart)
            {
                clock.restart();
                restart = false;
            }
            timeText.setString("Time(s):" + std::to_string(clock.getElapsedTime().asSeconds() + lastTime));
        }
        
        //Reads User input
        while (window.pollEvent(event))
        {
            //Closing the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                
                //////////     Start/Stop function      //////////
                if (event.key.code == sf::Keyboard::Num1)
                {
                    if(start)
                    {
                        lastTime += clock.getElapsedTime().asSeconds();
                    }
                    start = !start;
                    restart = true;
                }
                
                //////////         Lap function         //////////
                else if((event.key.code == sf::Keyboard::Num2) && start)
                {
                    lapTime.push_back(clock.getElapsedTime().asSeconds() + lastTime);
                    
                   lapText.setString(lapText.getString() + "\n" + std::to_string(lapTime.size()-1) + ":" + std::to_string(lapTime[lapTime.size()-1] - lapTime[lapTime.size()-2]));
                }
                
                //////////       Restart function       //////////
                else if(event.key.code == sf::Keyboard::Num3)
                {
                    reset(start, clock, lapTime, lapText, lastTime, timeText);
                }
                
                //////////        Record function       //////////
                else if(event.key.code == sf::Keyboard::Num4)
                {
                    records.open("Records.txt", std::ios_base::app);
                    lapText.setString("Laps Recorded");
                    records << "Loged at " << getCurrentTime() << '\n';
                    
                    for(int i = 1; i < lapTime.size(); i++)
                    {
                        records << i << ". " << lapTime[i] - lapTime[i - 1] << '\n';
                    }
                    records << '\n';
                    records.close();
                    reset(start, clock, lapTime, lapText, lastTime, timeText);
                }
                
                //////////       Scroll up function     //////////
                else if(event.key.code == sf::Keyboard::Down)
                {
                    if(startIndex + 1 <= lapTime.size() && startIndex < lapTime.size() - 4)
                    {
                        startIndex++;
                        lapText.setString("Laps");
                        for(int i = startIndex; i < lapTime.size(); i++)
                        {
                            lapText.setString(lapText.getString() + "\n" + std::to_string(i) + ":" + std::to_string(lapTime[i] - lapTime[i-1]));
                        }
                    }
                }
                
                //////////     Scroll down function     //////////
                else if(event.key.code == sf::Keyboard::Up)
                {
                    if(startIndex - 1 > 0)
                    {
                        startIndex--;
                        lapText.setString("Laps");
                        for(int i = startIndex; i < lapTime.size(); i++)
                        {
                            lapText.setString(lapText.getString() + "\n" + std::to_string(i) + ":" + std::to_string(lapTime[i] - lapTime[i-1]));
                        }
                    }
                }
            }
        }
        // Clear everything from the last run of the while loop
        window.clear();
        
        // Draw our text
        window.draw(timeText);
        window.draw(instructionsText);
        window.draw(lapText);
        
        // Show everything drawn
        window.display();
    }// This is the end of the "while" loop
    return 0;
}
