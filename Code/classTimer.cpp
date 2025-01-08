/*
 * Making a graphical class style timer 
 *
 * Christopher Bennett
 *
 * */


#include <iostream>
#include "raylib.h"
#include <vector>
#include <string>
#include <memory>


class Timer
{
	public:
		Timer(int screenWidth, int screenHeight)
		{
			boxWidth = 100, boxHeight = 80;
			boxX = screenWidth/2 - boxWidth/2, boxY = screenHeight/2 - boxHeight/2;
			timeEntered = false;
		}			

		void Draw() 
		{
			DrawRectangle(boxX, boxY, boxWidth, boxHeight, GRAY);
			DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, BLACK);
			DrawText(timeText.c_str(), x, y, 50, BLACK);
		}

		void getTimerLength()
		{
			while (!timeEntered)
			{
				try
				{
					std::cout << "Enter a timer length: ";
					std::cin >> time;
					timeText = std::to_string(time);
				}
				catch(std::string e)
				{
					std::cout << "Enter a valid number please" << std::endl;
					std::cout << "You entered " << e << std::endl;
				}
			}
		}

		int getX() { return x;}
		int getY() { return y;}
		int getBoxX() { return boxX;}
		int getBoxY() { return boxY;}
		int getBoxWidth() { return boxWidth;}
		int getBoxHeight() { return boxHeight;}
			

	private:
		int 
			boxWidth, boxHeight,
			boxX, boxY,
			x, y;
		double time;		
		std::string timeText;
		bool timeEntered;
};

class Button
{
	public:
		virtual ~Button() = default;
		virtual void Draw() = 0;

	protected:
		int width,
		    height,
		    x,
		    y;
};

class TimeButton : protected Button
{
	public:
		TimeButton(int newTime, int newWidth, int newHeight, int newX, int newY)
		{	
			time = newTime;
			width = newWidth;
			height = newHeight;
			x = newX;
			y = newY;
		}

		void Draw() override
		{
			DrawRectangle(x, y, width, height, WHITE);
		}

	private:
		int time;

};

class ActionButton : protected Button
{
	public:
		ActionButton(std::string newText, int newWidth, int newHeight, int newX, int newY)
		{
			width = newWidth;
			height = newHeight;
			x = newX;
			y = newY;
			text = newText;
		}

		void Draw() override
		{
			DrawRectangle(x, y, width, height, BLACK);
		}

	private:
		std::string text;
};

//Functions 
void DrawWindow(Color color, std::shared_ptr<Timer>& timer, std::shared_ptr<ActionButton>& startStop, std::vector<std::shared_ptr<TimeButton>>& timeButtons)
{
	ClearBackground(color);

	//Draw Buttons and Timer
	//Timer
	timer->
	timer->Draw();
	//Time Buttons 
	//Action Button
}


int main()
{
	std::cout << "We Ball" << std::endl;

	double startTime = GetTime();
	const int screenWidth = 1000, screenHeight = 800;
	const int timerBoxWidth = 100, timerBoxHeight = 80;
	const int timerBoxX = screenWidth/2 - timerBoxWidth/2, timerBoxY = screenHeight/2 - timerBoxHeight/2;
	std::shared_ptr <Timer> timer = std::make_shared<Timer>(screenWidth, screenHeight);  
	std::pair<int,int> stopStartPos = {timer->getX() + timer->getBoxWidth() + 30, timer->getY()};
	std::shared_ptr <ActionButton> startStop = std::make_shared<ActionButton>("Start/Stop", 50, 50, stopStartPos.first, stopStartPos.second);
	std::vector<std::shared_ptr<TimeButton>> timeButtons;

	//timer->getTimerLength();

	InitWindow(screenWidth, screenHeight, "Class Timer");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{

		BeginDrawing();
			DrawWindow(BLUE, timer, startStop, timeButtons);
		EndDrawing();
	}
	
	return 0;
}
