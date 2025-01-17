/*
 * Making graphical class timer with C
 *
 * Christoper Bennett 2025
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "raylib.h"

//Structs
typedef struct 
{
	int width, height;
	char* title;
} Screen;
bool ScreenInit(Screen* screen)
{
	screen->width = 1000;
	screen->height = 800;
	screen->title = malloc(sizeof(char) * 10);
	strncpy(screen->title, "Timer", 9);
	screen->title[9] = '\0';

	//Ensure screen members are allocated
	//Make sure all members are none NULL
	if (screen->height == 0 || screen->width == 0 || screen->title == NULL)
	{
		fprintf(stderr, "Memory Allocation Failed\n");
		return false;
	}else
	{
		return true;
	}
}

typedef struct
{
	int boxWidth, boxHeight,
	    boxX, boxY,
	    x, y;
	int time;
	char* timeText;
	bool timeEntered;
} Timer;
void TimerInit(Timer* timer , Screen* screen)
{
	timer->boxWidth = 100;
	timer->boxHeight = 50;
	timer->boxX = screen->width / 2 - timer->boxWidth / 2;
	timer->boxY = screen->height / 2 - timer->boxHeight / 2; 
	timer->x = timer->boxX  - MeasureText(timer->timeText, 40);
	timer->y = timer->boxY;
	timer->time = 0;
	timer->timeText = malloc(sizeof(char) * 100);
	strncpy(timer->timeText, "0", 99);
	timer->timeText[99] = '\0';
	timer->timeEntered = false;
}

typedef struct
{
	int x, y, width, height;
	int time;
	char* timeText;
} TimeButton;
void TimeButtonInit(TimeButton* timeButton)
{
	timeButton->width = 50;
	timeButton->height = 50;
}

typedef struct
{
	int x, y;
	int width, height;
	double startTime;
	bool isRunning;
} startStopButton;
void startStopButtonInit(startStopButton* startStopButton, Timer* timer)
{
	startStopButton->x = timer->boxX + timer->boxWidth + 40;
	startStopButton->y = timer->boxY;
	startStopButton->width = MeasureText("Start/Stop", 40) + 20;
	startStopButton->height = 50;
	startStopButton->isRunning = false;
}

//Prototypes
bool ScreenInit(Screen* screen);
void TimerInit(Timer* timer, Screen* screen);
void DrawTimer(Timer* timer);
bool makeTimeButtons(TimeButton* timeButtons);
void RunTimer(Screen* screen, Timer* timer, TimeButton* timeButtons, startStopButton* startStop);
void addTime(Timer* timer, int time);
void checkClicks(TimeButton* timeButtons, Timer* timer, startStopButton* startStop);
void updateTimer(Timer* timer, startStopButton* startStop);
void freeMemory(Screen* screen, Timer* timer, TimeButton* timeButtons, startStopButton* startStop);


int main()
{
	printf("We Ball\n");

	// Initialize the screen
	Screen* screen = (Screen*)malloc(sizeof(Screen));
	if (screen == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}
	Timer* timer = (Timer*)malloc(sizeof(Timer));
	if (timer == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}
	startStopButton* startStop = (startStopButton*)malloc(sizeof(startStopButton));
	if (startStop == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}

	TimeButton* timeButtons = (TimeButton*)malloc(sizeof(TimeButton) * 8);
	if (timeButtons == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}

	//Run the timer
	RunTimer(screen, timer, timeButtons, startStop);
	
	//Free memory
	freeMemory(screen, timer, timeButtons, startStop);
	return 0;
}




//Functions 
//Draw Functions
void DrawTimer(Timer* timer)
{
	DrawRectangleLines(timer->boxX, timer->boxY, timer->boxWidth, timer->boxHeight, MAROON);
	DrawText(timer->timeText, timer->x + 10, timer->y + 10, 40, BLACK);
}
void DrawTimeButton(TimeButton* timeButton)
{
	DrawRectangle(timeButton->x, timeButton->y, timeButton->width, timeButton->height, MAROON);

	int textWidth = MeasureText(timeButton->timeText, 40);
	DrawText(timeButton->timeText, timeButton->x - textWidth/2 + timeButton->width/2, timeButton->y +10, 40, BLACK);
}
void DrawStartStopButton(startStopButton* startStopButton)
{
	DrawRectangle(startStopButton->x, startStopButton->y, startStopButton->width, startStopButton->height, MAROON);
	DrawText("Start/Stop", startStopButton->x + 10, startStopButton->y + 10, 40, BLACK);
}

//Update Functions
void addTime(Timer* timer, int time)
{
	if (time != 0){
		timer->time = time + timer->time;
		snprintf(timer->timeText, 99, "%d", timer->time);
		timer->timeText[99] = '\0';
		timer->timeEntered = true;
		return;	
	}else
	{
		timer->time = 0;
		strncpy(timer->timeText, "0", 99);
		timer->timeText[99] = '\0';
		timer->timeEntered = false;
		return;
	}
}

void updateTimer(Timer* timer, startStopButton* startStop)
{
	if (startStop->isRunning)
	{
		int elapsedTime = GetTime() - startStop->startTime;
		if (timer->time <= elapsedTime)
		{
			timer->time = 0;
			strncpy(timer->timeText, "0", 99);
			timer->timeText[99] = '\0';
			startStop->isRunning = false;
		}else
		{
			int newTime = timer->time - elapsedTime;
			snprintf(timer->timeText, 99, "%d", newTime);
			timer->timeText[99] = '\0';
		}
	}
}


//Main Functions
void RunTimer(Screen* screen, Timer* timer, TimeButton* timeButtons, startStopButton* startStop)
{
	if (ScreenInit(screen)){

		InitWindow(screen->width, screen->height, screen->title);
		SetTargetFPS(60);
		TimerInit(timer, screen);
		startStopButtonInit(startStop, timer);
		if (!makeTimeButtons(timeButtons)){
			printf("Memory allocation failed\n");
			return;
		}

		while (!WindowShouldClose())
		{
			BeginDrawing();
				ClearBackground(RAYWHITE);
				DrawTimer(timer);
				DrawStartStopButton(startStop);
				checkClicks(timeButtons, timer, startStop);
				for (int i = 0; i < 8; i++)
				{
					DrawTimeButton(&timeButtons[i]);
				}
				updateTimer(timer, startStop);
			EndDrawing();
		}
		CloseWindow();
	}else
	{
		printf("Screen initialization failed\n");
	}
}

bool makeTimeButtons(TimeButton* timeButtons)
{
	if (timeButtons == NULL)
	{
		printf("Memory allocation failed\n");
		return false;
	}
	int times[8] = {0, 5, 10, 20, 30, 60, 300, 600};
	for (int i = 0; i < 8; i++)
	{
		TimeButtonInit(&timeButtons[i]);

		timeButtons[i].x = 50 + i * 100;
		timeButtons[i].y = 50;
		timeButtons[i].time = times[i];
		timeButtons[i].timeText = malloc(sizeof(char) * 10);
		snprintf(timeButtons[i].timeText, 9, "%d", timeButtons[i].time);
		timeButtons[i].timeText[9] = '\0';
		timeButtons[i].width = MeasureText(timeButtons[i].timeText, 40) + 20;
		timeButtons[i].height = 50;
	}
	return true;
}

void checkClicks(TimeButton* timeButtons, Timer* timer, startStopButton* startStop)
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		Vector2 mouse = GetMousePosition();
		Rectangle rec = {startStop->x, startStop->y, startStop->width, startStop->height};
		if (CheckCollisionPointRec(mouse, rec))
		{
			printf("Start/Stop Button Clicked\n");
			startStop->isRunning = !startStop->isRunning;
			if (!startStop->isRunning){startStop->startTime = GetTime();}
			if (startStop->isRunning){
				startStop->startTime = GetTime();
				timer->time = atoi(timer->timeText);
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			Vector2 mouse = GetMousePosition();
			Rectangle rec = {timeButtons[i].x, timeButtons[i].y, timeButtons[i].width, timeButtons[i].height};
			if (CheckCollisionPointRec(mouse, rec))
			{
				//printf("Button %d clicked\n", timeButtons[i].time);
				addTime(timer, timeButtons[i].time);
				return;
			}
		}
	}
}

void freeMemory(Screen* screen, Timer* timer, TimeButton* timeButtons, startStopButton* startStop)
{
	//freeScreen(screen);
	if (screen){
		free(screen->title);
		free(screen);
	}
	//freeTimer(timer);
	if (timer){
		free(timer->timeText);
		free(timer);
	}
	//freeTimeButton(timeButtons);
	if (timeButtons){
		for (int i = 0; i < 8; i++)
		{
			free(timeButtons[i].timeText);
		}
	}
	//freeStartStopButton(startStop);
	if (startStop){
		free(startStop);
	}
}
