// to do : to write a basic windows application : 


//First step always is to include the necessary header files and library files.
#include<Windows.h> //includes the necessary files for making the window and corresponding operations

//defining variables
LPCWSTR WindowClassName = L"triangleWindow"; //giving a name to the window 
HWND handleToAWindow = NULL; //creating a handle to the window 

//defining constants
const int Width = 1080;
const int Height = 960;

bool InitializeWindow(HINSTANCE handleInstance, int ShowWindow, int width, int height, bool windowed);
// // to do : what does windowed stand for?  ------> False if the window is full screen, true if it is windowed (which apparently means in the size of a window and not the whole screen)

int messageloop();
//keeps our program running.

//LRESULT stands for signed result of windows message processing.
//WinProc stands for window call back processesing
//name of our windows processing function 
LRESULT CALLBACK WndProc(HWND handleToAWindow, UINT msg, WPARAM wParam, LPARAM lParam);


//to know what is HINSTANCE, check this out https://devblogs.microsoft.com/oldnewthing/20050418-59/?p=35873
//it also explains why we  need a HINSTANCE and a window class name, to register and create a class. 

//main function from which the execution of the program starts. 
int WINAPI WinMain(HINSTANCE handleInstance, HINSTANCE hPreviousInstance, LPSTR longPointerFromCommandLine, int showWindowCommand) {

	// first initialise class and check if it is done. 
	//hInstance  and showWindowCommand are obtained from the attributes given to the WinMain function, Width and Height are global constants.
	//initialize does creating an instance, registering it and showing it. if any of them fail, the following error message is prompted.
	if (!InitializeWindow(handleInstance, showWindowCommand, Width, Height, true)) {
		MessageBox(0, L"Window cannot Initialize", L"Error", MB_OK);

		return 0;
	}

	//it is the interactive part of the application, any press/movement on the window is processed here. This keeps the window from automatcally dissappearing.
	messageloop();
}

//defining functions

//showWindowCommand stands for the way the window is to be displayed, example : SW_SHOWMAXIMIZED, SW_SHOW, SW_SHOWMINIMIZED.
bool InitializeWindow(HINSTANCE handleInstance, int showWindowCommand, int Width, int Height, bool windowed) {

	//in order to create/initialize a window, first you have to create a window class object 
	//This is a windows extended version class 
	WNDCLASSEX windowClassObject;

	//following which we will have to give the various properties associated with the windowClassObject.
	windowClassObject.cbSize = sizeof(WNDCLASSEX);
	windowClassObject.style = CS_VREDRAW | CS_HREDRAW;
	//CS_HREDRAW    Window is refreshed if there is a change in the window's width or if the window is moved horizontally.
	//CS_VREDRAW    The window is redrawn if there is a change in the window's height or if the window is moved vertically
	windowClassObject.lpfnWndProc = WndProc; //the function that processes our window messages
	windowClassObject.cbClsExtra = NULL; //do we need extra bytes for class?
	windowClassObject.cbWndExtra = NULL; //do we need extra bytes for window instance?
	windowClassObject.hInstance = handleInstance;
	windowClassObject.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClassObject.hCursor = LoadCursor(NULL, IDC_HAND);
	//the top left icon, and the cursor type. 
	windowClassObject.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	//------------this one makes the background black
	windowClassObject.lpszMenuName = NULL;
	windowClassObject.lpszClassName = WindowClassName; //initialized and declared as global variable
	windowClassObject.hIconSm = LoadIcon(NULL, IDI_WINLOGO); //this shows the icon to be shown for the application on the task bar.

	//after feeding all the respective properties of the window class object, we give it to the registerclass function to register it.
	if (!RegisterClassEx(&windowClassObject)) {
		MessageBox(NULL, L"Couldn't register the class", L"Error", MB_OK | MB_ICONERROR);

		return false;
	}

	//creates a window with the given properties
	handleToAWindow = CreateWindowEx(NULL, WindowClassName, L"My Basic Windows App", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, handleInstance, NULL);

	//now after creating the window, we need to check before we proceed
	if (!handleToAWindow) {
		MessageBox(NULL, L"Couldn't create the window", L"Error", MB_OK | MB_ICONERROR);
		return false;
		//execution goes back to main, couldnt initialize window.
	}

	//now that the window is 1. registered, 2. created, next we have to show and update.
	ShowWindow(handleToAWindow, showWindowCommand);    //Shows our window
	UpdateWindow(handleToAWindow);
	//what does update do? What UpdateWindow() does is send a WM_PAINT message directly to the windows procedure. 
	//If there isn't anything in the client area of the window, then UpdateWindow() does not send a message. 
	//We then return true so our WinMain() function can get on to the message loop.

	return true;
	//thus window initialized successsfully 
}
//the messageloop is the part of the program that keeps the program running. 
//Here is where interaction with the window is done.
int messageloop() {

	//MSG structure holds the info of the message
	MSG message;

	ZeroMemory(&message, sizeof(MSG));

	//as long as there is a message, this runs
	while (true) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			//PM_REMOVE will remove the message after processed.

			if (message.message == WM_QUIT) {
				break;
				//break out of the while loop, hence ending the message loop
			}

			//else any other messages other than quits!
			TranslateMessage(&message);
			DispatchMessage(&message);
			//Dispatch sends it to WinProc
		}
		else {
			//if no windows messages present then continue doing what the app is supposed to be doing
		}
	}

	//in case all windows messages are over, the function returns a wparam
	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND handleToAWindow, UINT message, WPARAM wParam, LPARAM lParam) {
	//this basically lets the default actions for all windows messages.
	//return DefWindowProc(handleToAWindow, message, wparam, lparam);

	//to customize how to react to various messages : 
	switch (message)
	{

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Exit?",
				L"Sure?", MB_YESNO | MB_ICONQUESTION) == IDYES)
				DestroyWindow(handleToAWindow);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	//rest of them can be done the default windows ways
	return DefWindowProc(handleToAWindow, message, wParam, lParam);
}