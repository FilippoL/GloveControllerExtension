#pragma once
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
	#undef _DEBUG
	#include <Python.h>
	#define _DEBUG
#else
#include <Python.h>
	#include <Python.h>
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <time.h>


#define MAX_FINGERS 5



class MyExtension
{

private: 
	/*Default path to the python script*/
	std::string _path;
	/* Create some Python objects that will later be assigned values.*/
	PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue;

	/*Private sensor values*/
	float pitch, roll;

	/*Private Array of current fingers Flex value*/
	float fingers_value[MAX_FINGERS];

	/*Private bools for buttons value*/
	bool button_A_pressed, button_B_pressed;

	/*Private scale and timeout*/
	float scale;
	float timeout;


	static bool connected;
	long baud;

	/*Time since try connection started */
	double duration;


	/*Variable for exiting the thread loop*/
	bool threadsNeedToDie;

	/*The two main threads*/
	std::thread * Connecting_thread;
	std::thread * Updating_thread;

	/*Threads function*/
	void Connect();
	void UpdateVariablesFromSerial();

	/*Function for shutting everything down*/
	void SerialClose();

	/*Private Tool Function*/
	float StringToNumber(std::string s) {
		int Numb;
		std::stringstream str(s);
		str >> Numb;
		return float(Numb);
	}

public:

	/*Use this function when you want to start updating variables
	from the serial, ONLY to be called after PairController() */
	void Start();

	/*Use this function when you want to stop updating variables
	from the serial, this will automatically both close the serial 
	and destroy the python interpreter*/
	void Stop();

	/*Contructor with default parameter assigned*/
	MyExtension(const std::string& path);

	/*Deconstructor*/
	~MyExtension();

	/*Set Baud rate*/
	inline void SetBaud(long _baud) {
		baud = _baud;
	};

	/*Function to be called when enetering pairing mode, it will look for connection
	till when the timout in SECONDS is reached*/
	void PairController(float _timeout);


	/*Enum of fingers for indexing*/
	enum fingers { F_THUMB, F_INDEX, F_MIDDLE, F_RING, F_PINKY };


/******************** INLINE GETTERS *******************************/
	
	/*Returns the flex of the specified finger*/
	inline float GetFlex(fingers f) { return fingers_value[f]; };

	/*True if connection established, false otherwise*/
	inline bool isConnected() { return connected; };

	/*Returns the time elapsed since the pairing mode has been triggered (seconds)*/
	inline double GetTimeElapsed() { return duration; };

	/*Returns the max timeout given to the pairing mode*/
	inline double GetTimeout() { return timeout; };

	/*Returns the Roll value*/
	inline float GetRoll() { return roll / scale; };

	/*Returns the Pitch value*/
	inline float GetPitch() { return pitch / scale; };

	/*Set the scale, default scale = 1 */
	inline void SetScale(float s) {scale = s;};

	/*Get the scale, default scale = 1 */
	inline float GetScale() { return scale; };

	/*Button A Status*/
	inline bool GetButtonAPressed() { return button_A_pressed; };

	/*Button B Status*/
	inline bool GetButtonBPressed() { return button_B_pressed; };

};


