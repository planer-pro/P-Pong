#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define leftBtnPin 53
//#define centerBtnPin 11//on future
#define rightBtnPin 51

bool startGame = false;
bool part = false;

int rocketPosX = 54;
int ballPosX = 64;
int ballPosY = 58;
int randAngleBallPosX;
int direction;
int lose = 0;
int hit = 0;

unsigned long oldTime;
unsigned long pauseStart;
unsigned long pauseDiff = 0;
unsigned long menuFlex;

void setup()
{
	Serial.begin(57600);

	pinMode(leftBtnPin, INPUT_PULLUP);
	//pinMode(centerBtnPin, INPUT_PULLUP);
	pinMode(rightBtnPin, INPUT_PULLUP);

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();
	display.dim(1);
	display.setTextColor(WHITE);

	display.setCursor(0, 0);
	display.setTextSize(1);
	display.println("P-Pong v1.0");
	display.println("Designed by Planer");
	display.setCursor(52, 57);
	display.println("2017");
	display.display();
	delay(5000);

	randomSeed(analogRead(A0));
}

void loop()
{
	if (millis() >= menuFlex && !startGame)
	{
		part = !part;

		display.clearDisplay();
		display.setCursor(20, 20);
		display.setTextSize(1);
		display.print("PRESS ");

		if (part) display.print("ANY KEY");

		display.setCursor(20, 40);
		display.print("to start PONG");
		display.display();

		menuFlex = millis() + 300;
	}

	if ((!digitalRead(leftBtnPin) || !digitalRead(rightBtnPin)) && !startGame)
	{
		startGame = true;
		oldTime = millis();
	}

	if (startGame)
	{
		if (!digitalRead(leftBtnPin))
		{
			rocketPosX -= 2;
			rocketPosX = max(rocketPosX, 0);
		}

		if (!digitalRead(rightBtnPin))
		{
			rocketPosX += 2;
			rocketPosX = min(rocketPosX, 107);
		}

		if (!digitalRead(leftBtnPin) && !digitalRead(rightBtnPin))//pause in future
		{
			pauseStart = millis();

			display.clearDisplay();
			display.setCursor(50, 27);
			display.print("PAUSE");
			display.display();

			delay(100);//debounce

			while (!digitalRead(leftBtnPin) && !digitalRead(rightBtnPin)) {}

			delay(100);//debounce

			while (digitalRead(leftBtnPin) && digitalRead(rightBtnPin)) {}

			delay(100);//debounce

			while (!digitalRead(leftBtnPin) && !digitalRead(rightBtnPin)) {}

			pauseDiff += millis() - pauseStart;
		}

		if (ballPosY >= 58)
		{
			if (ballPosX >= rocketPosX && ballPosX <= rocketPosX + 20)
			{
				hit++;
				randAngleBallPosX = random(-4, 5);
				direction = -1;
			}
			else
			{
				lose++;
				hit = 0;
				display.clearDisplay();
				display.setCursor(0, 0);
				display.print("lost:");
				display.println(lose);
				display.setCursor(28, 27);
				display.setTextSize(2);
				display.println("loser)");
				display.display();
				delay(2000);
				display.setTextSize(1);
				ballPosX = rocketPosX + 10;
				ballPosY = 57;
				oldTime = millis();
				pauseDiff = 0;
			}
		}

		if (ballPosY <= 0)
		{
			randAngleBallPosX = random(-4, 5);
			direction = 1;
		}

		if (ballPosX <= 0)
		{
			randAngleBallPosX = random(0, 5);
		}

		if (ballPosX >= 127)
		{
			randAngleBallPosX = random(-4, 0);
		}

		ballPosX = ballPosX + randAngleBallPosX;
		ballPosY = ballPosY + direction;

		display.clearDisplay();
		//display.drawFastHLine(rocketPosX, 63, 20, WHITE);
		display.drawRect(rocketPosX, 61, 20, 3, WHITE);
		display.drawCircle(ballPosX, ballPosY, 2, WHITE);
		display.setCursor(0, 0);
		display.print("lost:");
		display.print(lose);
		display.print(" ");
		display.print("hit:");
		display.print(hit);
		display.print(" ");
		//display.setCursor(80, 0);
		display.print("time:");
		display.print((millis() - oldTime - pauseDiff) / 1000);
		display.display();
	}
}