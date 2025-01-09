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
			x = boxX;
			y = boxY;
			timeEntered = false;
		}			

        void changeStartTime(double start) { startTime = start;}

		void Draw() 
		{
			DrawRectangle(boxX, boxY, boxWidth, boxHeight, GRAY);
			DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, BLACK);
			DrawText(timeText.c_str(), x, y, 50, BLACK);
		}

		void getTimerLength(int screenWidth, int screenheight)
		{
			while (!timeEntered)
			{
				try
				{
					std::cout << "Enter a timer length: " << std::endl;
					std::cin >> time;
					timeText = std::to_string(time);
					
					int textWidth = MeasureText(timeText.c_str(), 50);
					
					//Update Box Size 
					boxX = boxX + boxWidth/2;
					boxWidth = textWidth + 20;
					boxX = boxX - boxWidth/2;

					//Update Text position
					x = boxX + (boxWidth - textWidth)/2;
					y = boxY + (boxHeight - 50)/2;
					timeEntered = true;
				}
				catch(std::string e)
				{
					std::cout << "Enter a valid number please" << std::endl;
					std::cout << "You entered " << e << std::endl;
				}
			}
		}

        void updateTimer()
        {
            timeText = std::to_string((int)(time - GetTime()));
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
		int time;		
		std::string timeText;
		double startTime;
		bool timeEntered;
};

class Button
{
	public:
		virtual ~Button() = default;
		virtual void Draw() = 0;

		int getWidth() { return width;}
		int getHeight() { return height;}
		int getX() { return x;}
		int getY() { return y;}

		void changeWidth(std::string newWidth) 
		{
			int tempTextWidth = MeasureText(newWidth.c_str(), 20);
			width = tempTextWidth + 20;
			x = x - tempTextWidth/2;
		}
		void changeHeight(std::string newHeight)
		{
			int tempTextHeight= MeasureText(newHeight.c_str(), 20);
			height = tempTextHeight + 20;
		}

		Rectangle getRectangle() const 
		{
			return Rectangle{(float)x, (float)y, (float)width, (float)height};
		}
		
	protected:
		int width,
		    height,
		    x,
		    y;
};

class TimeButton : public Button
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

class ActionButton : public Button
{
	public:
		ActionButton(std::string newText, int newWidth, int newHeight, int newX, int newY)
		{
			std::cout << MeasureText(newText.c_str(), 20) << std::endl;
			width = newWidth;
			height = newHeight;
			x = newX;
			y = newY;
			text = newText;
		}

       //Gets 
        bool getStart() { return start;}

        //Changes
		void changeWidth(std::string newWidth) 
		{
			int tempTextWidth = MeasureText(newWidth.c_str(), 20);
			width = tempTextWidth + 20;
			x = x + tempTextWidth/2;
			textWidth = tempTextWidth;
		}
		void changeHeight(std::string newHeight) 
		{
			int tempTextHeight= MeasureText(newHeight.c_str(), 20);
			height = tempTextHeight + 20;
		}
        void changeStart(bool value)
        {
            start = value;
        }

		void Draw() override
		{
			DrawRectangle(x, y, width, height, BLACK);
			DrawText(text.c_str(), x + textWidth/16, y+10, 20, WHITE); 
		}
        
	private:
		std::string text;
		int textWidth;
        bool start;
};

//Functions 
void DrawWindow(Color color, std::shared_ptr<Timer>& timer, std::shared_ptr<ActionButton>& startStop, std::vector<std::shared_ptr<TimeButton>>& timeButtons)
{
	ClearBackground(color);

	//Draw Buttons and Timer
	//Timer
    if (startStop->getStart()){
        timer->updateTimer();
    }
	timer->Draw();
	//Time Buttons 
	//Action Button
	startStop->Draw();
}

void CheckClicks(std::shared_ptr<ActionButton>& stopStart, std::vector<std::shared_ptr<TimeButton>>& timeButtons)
{
	//std::cout << "Checking Clicks" << std::endl;
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			std::cout << "Mouse Clicked" << std::endl;
			Vector2 mousePos= GetMousePosition();

				//Check for rectangle collision on stop start button 
			if (CheckCollisionPointRec(mousePos, stopStart->getRectangle())){
					std::cout << "Start Stop Button clicked" << std::endl;
                    if (stopStart->getStart() == true){
                        stopStart->changeStart(false);
                    }
                    else{
                        stopStart->changeStart(true);
                    }
			}

			for (const auto& button : timeButtons)
			{
				if (mousePos.x == button->getX() && mousePos.y == button->getY()){
					std::cout << "Button Clicked" << std::endl;
				}
			}
	}
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


	InitWindow(screenWidth, screenHeight, "Class Timer");
	SetTargetFPS(60);

	timer->getTimerLength(screenWidth, screenHeight);
	startStop->changeWidth("Start/Stop");

	while (!WindowShouldClose())
	{
		BeginDrawing();
			DrawWindow(BLUE, timer, startStop, timeButtons);
			CheckClicks(startStop, timeButtons);
		EndDrawing();
	}
	
	return 0;
}
