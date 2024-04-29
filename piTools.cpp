// Written by Michael Trout | Spring 2024
#include <piTools.h>

// Constructor
GPIO::GPIO()
{
	std::cout << "Constructor called\n";
}

//////////////////// PUBLIC METHODS ////////////////////

void GPIO::ActivateGPIO(int pinMode)
{
	mode = pinMode;

	if (activationCall == 0)
	{
		switch (mode)
		{
			case 0:
				InitWPi();
				break;
			case 1:
				InitBCM();
				break;
			case 2:
				InitPhy();
				break;
			default:
				std::cout << "Error: Unsuported pin type! Pin type must be one of the following:\nWiringPiMode\nBCMMode\nPhysicalMode\n";
				exit(0);
				break;
		}
		activationCall++;
	}
	else
	{
		std::cout << "ActivateGPIO() may only be called once within a program!\n";
	}
}

void GPIO::TerminateGPIO()
{
	CheckActivation();
	CheckTermination();
	ResetGPIO();
	pinList.clear();
	terminationCall--;
}

void GPIO::ResetGPIO()
{
	for (int i : pinList)
	{
		digitalWrite(i, LOW);
	}
}

void GPIO::SetPinMode(int pin, int direction)
{
	if (CheckPin(pin))
	{
		if (direction == 0)
		{
			pinMode(pin, OUTPUT);
			pinList.push_front(pin);
		}
		else if (direction == 1)
		{
			pinMode(pin, INPUT);
			pullUpDnControl(pin, PUD_UP);
		}
		else
		{
			std::cout << "SetPinMode(int pin, int direction) Error: Invalid direction!\nUse one of the following built in variables:\n	IN or OUT\nProgram will now close\n";
			exit(0);
		}
	}
	else
	{
		std::cout << "SetPinMode(int pin, int direction) Error: Invalid pin number!\nCheck GPIO header reference or numbering conversion sheet\nProgram will now close\n";
		exit(0);
	}
}

void GPIO::PinOn(int pin)
{
	CheckTermination();
	CheckActivation();
	digitalWrite(pin, HIGH);
}

void GPIO::PinOff(int pin)
{
	CheckTermination();
	CheckActivation();
	digitalWrite(pin, LOW);
}

void GPIO::SetPin()
{
	// TODO
}

int GPIO::ReadPin(int pin)
{
	CheckTermination();
	CheckActivation();

	if (digitalRead(pin) == HIGH)
	{
		return HIGH;
	}
	else
	{
		return LOW;
	}
}

void GPIO::FlashPin(int pin, int seconds, int blinkRate)
{
	CheckTermination();
	CheckActivation();

	int speed = blinkRate * 100;

	auto finish = std::chrono::system_clock::now() + std::chrono::seconds(seconds);

	do
	{
		digitalWrite(pin, HIGH);
		delay(speed);
		digitalWrite(pin, LOW);
		delay(speed);

	} while (std::chrono::system_clock::now() < finish);
}

int GPIO::ReadDial()
{
	// TODO
	return 1;
}

void GPIO::BackgroundInput(GPIO& instance, int pin, int button)
{
	// The background functions need to be passed an instance of the GPIO object since the member functions are non-static
	CheckTermination();
	CheckActivation();

	std::thread t1(&GPIO::InputLoop, &instance,  pin, button);
	t1.detach();
}

void GPIO::BackgroundFlash(GPIO& instance, int pin, int blinkRate)
{
	CheckTermination();
	CheckActivation();

	std::thread t2(&GPIO::BlinkLoop, &instance, pin, blinkRate);
	t2.detach();
}

void GPIO::BackgroundToggle(GPIO& instance, int pin, int button)
{
	CheckTermination();
	CheckActivation();

	std::thread t3(&GPIO::ToggleLoop, &instance, pin, button);
	t3.detach();
}

//////////////////// PRIVATE METHODS ////////////////////

void GPIO::InitWPi()
{
	if (wiringPiSetup() == -1)
	{
		std::cout << "Initialization failed!\n";
		exit(0);
	}
	else
	{
		std::cout << "Initialization successfull\n";
	}
}

void GPIO::InitBCM()
{
	if (wiringPiSetupGpio() == -1)
	{
		std::cout << "Initialization failed\n";
		exit(0);
	}
	else
	{
		std::cout << "Initialization Successfull\n";
	}
}

void GPIO::InitPhy()
{
	if (wiringPiSetupPhys() == -1)
	{
		std::cout << "Initialization failed\n";
		exit(0);
	}
	else
	{
		std::cout << "Initialization successfull\n";
	}
}

void GPIO::CheckActivation()
{
	if (activationCall < 0)
	{
		std::cout << "ActivateGPIO(int pinMode) has not been called!\nProgram will now terminate!\n";
		exit(0);
	}
}

void GPIO::CheckTermination()
{
	if (terminationCall < 0)
	{
		std::cout << "TerminateGPIO() has been called!\nFunctions can not be called after being terminated\nProgram will now terminae\n";
		exit(0);
	}
}

void GPIO::InputLoop(int pin, int button)
{
	// It seems as though the mutex is not neccessary
	// removing the lock guard allows multiple threads to access this same code.
	// If it is discovered that the lock guard is neccessary, the next line should be uncommented
	//std::lock_guard<std::mutex> inputLock(inputMutex);

	while (terminationCall == 0)
	{
		if (digitalRead(button) == HIGH)
		{
			digitalWrite(pin, HIGH);
		}
		else
		{
			digitalWrite(pin, LOW);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}	
}

void GPIO::BlinkLoop(int pin, int blinkRate)
{
	// It seems as though the mutex is not neccessary
	// removing the lock guard allows multiple threads to access this same code.
	// If it is discovered that the lock guard is neccessary, the next line should be uncommented
	//std::lock_guard<std::mutex> blinkLock(blinkMutex);

	int speed = blinkRate * 100;

	while (terminationCall == 0)
	{
		digitalWrite(pin, HIGH);
		delay(speed);
		digitalWrite(pin, LOW);
		delay(speed);
	}
}

void GPIO::ToggleLoop(int pin, int button)
{
	// This function only allows one input per 50 milliseconds to get around button bounce
	auto lastPress = std::chrono::system_clock::now();
	auto currentPress = std::chrono::system_clock::now() + std::chrono::milliseconds(50);
	while (terminationCall == 0)
	{
		if (digitalRead(button) == HIGH)
		{
			currentPress = std::chrono::system_clock::now();

			if (currentPress > (lastPress + std::chrono::milliseconds(50)))
			{
				digitalWrite(pin, !digitalRead(pin));
			}

			lastPress = currentPress;
		}
	}
}

// The Check functions make sure that a valid pin number was entered
bool GPIO::CheckPin(int pin)
{
	switch (mode)
	{
		case 0:
			return CheckWPi(pin);
			break;
		case 1:
			return CheckBCM(pin);
			break;
		case 2:
			return CheckPhy(pin);
			break;
		default:
			return false;
			break;
	}
}

bool GPIO::CheckWPi(int pin)
{
	if (pin < 0)
	{
		return false;
	}
	else if (pin >= 0 && pin <= 16)
	{
		return true;
	}
	else if (pin >= 21 && pin <= 29)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GPIO::CheckBCM(int pin)
{
	if (pin <= 1)
	{
		return false;
	}
	else if (pin >= 2 && pin <= 26)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GPIO::CheckPhy(int pin)
{
	switch (pin)
	{
		case 3:
			return true;
			break;
		case 5:
			return true;
			break;
		case 7:
			return true;
			break;
		case 8:
			return true;
			break;
		case 10:
			return true;
			break;
		case 11:
			return true;
			break;
		case 12:
			return true;
			break;
		case 13:
			return true;
			break;
		case 15:
			return true;
			break;
		case 16:
			return true;
			break;
		case 18:
			return true;
			break;
		case 19:
			return true;
			break;
		case 21:
			return true;
			break;
		case 22:
			return true;
			break;
		case 23:
			return true;
			break;
		case 24:
			return true;
			break;
		case 26:
			return true;
			break;
		case 29:
			return true;
			break;
		case 31:
			return true;
			break;
		case 32:
			return true;
			break;
		case 33:
			return true;
			break;
		case 35:
			return true;
			break;
		case 36:
			return true;
			break;
		case 37:
			return true;
			break;
		case 38:
			return true;
			break;
		case 40:
			return true;
			break;
		default:
			return false;
			break;
	}
}

// Destructor
GPIO::~GPIO()
{
	std::cout << "Destructor called\n";
}
