// Written by Michael Trout | Spring 2024
#pragma once

// built in libraries
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <list>

// third party libraries
#include <wiringPi.h>

// Global variables
const int WiringPiMode = 0;
const int BCMMode = 1;
const int PhysicalMode = 2;
const int IN = 1;
const int OUT = 0;

class GPIO
{
	public:
		// Variables
		//const int HIGH = 1;
		//const int LOW = 0;


		// Methods
		GPIO();

		void ActivateGPIO(int pinMode);
		void TerminateGPIO();
		void ResetGPIO();
		void SetPinMode(int pin, int direction);
		void PinOn(int pin);
		void PinOff(int pin);
		void SetPin();
		int ReadPin(int pin);
		void FlashPin(int pin, int seconds, int blinkRate);
		void BackgroundInput(GPIO& instance, int pin, int button);
		void BackgroundFlash(GPIO& instance, int pin, int blinkRate);
		void BackgroundToggle(GPIO& instance, int pin, int button);
		int ReadDial();

		~GPIO();

	private:
		// Variables
		int mode = -1;
		int activationCall = 0;
		int terminationCall = 0;
		std::mutex inputMutex;
		std::mutex blinkMutex;
		std::list<int> pinList;

		// Methods
		void InitWPi();
		void InitBCM();
		void InitPhy();
		void CheckActivation();
		void CheckTermination();
		void InputLoop(int pin, int button);
		void BlinkLoop(int pin, int blinkRate);
		void ToggleLoop(int pin, int button);
		bool CheckPin(int pin);
		bool CheckWPi(int pin);
		bool CheckBCM(int pin);
		bool CheckPhy(int pin);
};
