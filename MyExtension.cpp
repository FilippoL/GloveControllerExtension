#include "MyExtension.h"
bool MyExtension::connected = false;

MyExtension::MyExtension(const std::string& path) :_path(path)
{

	// Initialize the Python interpreter.
	Py_Initialize();

	// Convert the file name to a Python string.
	pName = PyString_FromString(_path.c_str());

	// Import the file as a Python module.
	pModule = PyImport_Import(pName);

	// Create a dictionary for the contents of the module.
	pDict = PyModule_GetDict(pModule);

	baud = 9600;

	scale = 0.f;


}

void MyExtension::Start() {
	if (connected)
	{
		/*Once device is connected and paired is time to launch one thread for reading the sensors values*/
		Updating_thread = new std::thread(&MyExtension::UpdateVariablesFromSerial, this);
		Updating_thread->detach();
	}

}

void MyExtension::Stop() {
	if (connected)
	{
		/*If stopped from user, kill the thread.
		This is safe to do as long as all the cleanup and release is handled within the thread.*/
		threadsNeedToDie = true;
	}
}

void MyExtension::Connect() {
	clock_t start;
	duration = 0.0;
	start = clock();

	while (duration < timeout)
	{

		// Get the choosen method from the dictionary.
		pFunc = PyDict_GetItemString(pDict, "Serial_connect");

		// Create a Python tuple to hold the arguments to the method.
		pArgs = PyTuple_New(1);

		// Convert to a Python integer.
		pValue = PyInt_FromLong(baud);

		// Set the Python int as the first and second arguments to the method.
		PyTuple_SetItem(pArgs, 0, pValue);

		PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

		if (pResult != NULL)
		{
			std::string result;
			result = PyString_AsString(pResult);

			result == "True" ? connected = true : connected = false;

			if (connected)
			{
#ifdef _DEBUG
				std::cout << "\n*********************CONNECTED*********************\n";
#endif
				break;
			}

			duration = (clock() - start) / (double)CLOCKS_PER_SEC;
			std::cout << duration;
		}
	}

	if (!connected)
	{
#ifdef _DEBUG
		std::cout << "\n*********************CONNECTION TIMED OUT*********************\n";
#endif
	}
	duration = 0.0;
	delete Connecting_thread;
	return;
}

void MyExtension::UpdateVariablesFromSerial() {	//Hanlded by Updating_thread
	while (!threadsNeedToDie)
	{
		// Get the choosen method from the dictionary.
		pFunc = PyDict_GetItemString(pDict, "GetVars");

		// Create a Python tuple to hold the arguments to the method.
		pArgs = NULL;

		PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

		// Print a message if calling the method failed.
		if (pResult == NULL)
		{
			for (int i = 0; i < MAX_FINGERS; i++)
			{
				fingers_value[i] = 0;
			}

			button_A_pressed = false, button_B_pressed = false;
			pitch = 0, roll = 0;
			break;
		}

		/*Variable that will hold the whole buffer of chars*/
		std::string result;

		/*Get the function resul and cast it from Python Object to std::string*/
		result = PyString_AsString(pResult);

		/*Temporary variables to hold the string value before converting it*/
		std::string str_pitch, str_roll, str_thumb, str_index, str_middle, str_ring, str_pinky, str_btnA, str_btnB;

		/*Start reading whats in between the b(egin) - e(nd) tokens*/
		str_pitch = result.substr(result.find("b") + 1, result.find("e") - 1);
		pitch = StringToNumber(str_pitch);
		result.erase(0, result.find("e") + 1); //Clear up to where has already been read

		str_roll = result.substr(result.find("b") + 1, result.find("e") - 1);
		roll = StringToNumber(str_roll);
		result.erase(0, result.find("e") + 1);


		str_thumb = result.substr(result.find("b") + 1, result.find("e") - 1);
		fingers_value[F_THUMB] = StringToNumber(str_thumb);
		result.erase(0, result.find("e") + 1);

		str_thumb = result.substr(result.find("b") + 1, result.find("e") - 1);
		fingers_value[F_INDEX] = StringToNumber(str_index);
		result.erase(0, result.find("e") + 1);

		str_thumb = result.substr(result.find("b") + 1, result.find("e") - 1);
		fingers_value[F_MIDDLE] = StringToNumber(str_middle);
		result.erase(0, result.find("e") + 1);

		str_thumb = result.substr(result.find("b") + 1, result.find("e") - 1);
		fingers_value[F_RING] = StringToNumber(str_ring);
		result.erase(0, result.find("e") + 1);

		str_thumb = result.substr(result.find("b") + 1, result.find("e") - 1);
		fingers_value[F_PINKY] = StringToNumber(str_pinky);
		result.erase(0, result.find("e") + 1);

		str_btnA = result.substr(result.find("b") + 1, result.find("e") - 1);
		StringToNumber(str_btnA) == 1 ? button_A_pressed = true : button_A_pressed = false;
		result.erase(0, result.find("e") + 1);

		str_btnB = result.substr(result.find("b") + 1, result.find("e") - 1);
		StringToNumber(str_btnB) == 1 ? button_B_pressed = true : button_B_pressed = false;
		result.erase(0, result.find("e") + 1);

#ifdef _DEBUG
		std::cout << "Thumb: " << fingers_value[F_THUMB] << std::endl
			<< "Index: " << fingers_value[F_INDEX] << std::endl
			<< "Middle: " << fingers_value[F_MIDDLE] << std::endl
			<< "Ring: " << fingers_value[F_RING] << std::endl
			<< "Pinky: " << fingers_value[F_PINKY] << std::endl << std::endl
			<< "Pitch: " << pitch << std::endl
			<< "Roll : " << roll << std::endl << std::endl
			<< "Button A : " << button_A_pressed << std::endl
			<< "Button B : " << button_B_pressed << std::endl << std::endl;
#endif


	}

	/*Is no more connected*/
	connected = false;

	/*Delete the thread object from the heap*/
	delete Updating_thread;

	/*Safely release a detached thread is done just by returning.
	In this case it will not only return the thread function, but it will
	also process the shutdown for serial comunication and python by calling
	SerialClose() on the main thread. */
	return SerialClose();
};

void MyExtension::SerialClose() {
	// Get the choosen method from the dictionary.
	pFunc = PyDict_GetItemString(pDict, "Release");

	// Create a Python tuple to hold the arguments to the method.
	pArgs = NULL;

	PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

	// Print a message if calling the method failed.
	if (pResult == NULL)
	{
		std::cout << "Did not shotdown properly\n";
	}

	// Destroy the Python interpreter.
	Py_Finalize();
};

void MyExtension::PairController(float _timeout) {
	timeout = _timeout;
	Connecting_thread = new std::thread(&MyExtension::Connect, this);
	Connecting_thread->detach();
};

MyExtension::~MyExtension() {

};