#include <piTools.h>

const int LED1 = 0;
const int LED2 = 3;
const int BUTTON1 = 1;
const int BUTTON2 = 2;
const int ExitBtn = 25;

int main(int argc, char* args[])
{
	GPIO Test;

	Test.ActivateGPIO(WiringPiMode);
	Test.SetPinMode(LED1, OUT);
	Test.SetPinMode(BUTTON1, IN);
	Test.SetPinMode(LED2, OUT);
	Test.SetPinMode(BUTTON2, IN);
	Test.SetPinMode(ExitBtn, IN);
	
	Test.BackgroundToggle(Test, LED2, BUTTON2);

	bool running = true;
	while (running)
	{
		if (Test.ReadPin(BUTTON1) == HIGH)
		{
			Test.FlashPin(LED1, 10, 5);

		}
		else if (Test.ReadPin(ExitBtn) == HIGH)
		{
			// Leave the program loop and safely exit program by closing the loop
			running = false;
		}
	}
	Test.TerminateGPIO();
	return 0;
}
