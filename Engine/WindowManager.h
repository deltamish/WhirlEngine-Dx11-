/* 
Author: Karthik Hebbar M N 
 Website FissionSpark.weebly.com
 Engine Whirl engine
 NOTE:This code is still a prototype */


#include<windowsx.h>
#include<Windows.h>
#include<vector>
#include<list>
#include<iostream>
using namespace std;


#define CHECK_WND = 0
#define CHECK_BORDER = 1

#pragma region Docking System

	struct Border{ // Border information about the Window
	 RECT Horizontal; 
	 RECT Vertical;
	};

	struct Rect{
	 short X,Y;
	 short Width,Height;
	 short BorderX;
	 short BorderY;
	};

	class WindowDesc
	{

	public:
	  WindowDesc(Rect,short,short);  // Consturctor
	  ~WindowDesc(); // Destructor

	  RECT Pos; // Position of window
	  short Width,Height; // Width and Height
	  short thisWindow; // The Current Window ID
	  short AttachedWindow; // The Attached Window ID
	  short MainWindow; // ID of the Main Window
	  
	  // Funcion prototypes
	 
	private:
		Border borders;


	};
#pragma endregion

class WindowManager{


	// Destructor
	~WindowManager();

private:
	vector<HWND> wnds;
	vector<WindowDesc> WindowsDescs;
	HWND hwnd;

public:
	// Constructor 
	vector<HWND> GetList();
	WindowManager();
	void CreateNewWindow(HINSTANCE hinst,WNDCLASSEX * ex,LPCWSTR WndName,HWND * Parent,HWND * ReturnWind,Rect rect,bool);
	void ShowWindows(int cShow);
	int HandleMseeages();
	bool CheckPos(Window * ,short,POINT); 
	void Update();

};