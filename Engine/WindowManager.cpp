//// Author: Karthik Hebbar M N
//// Website FissionSpark.weebly.com
//// Engine Whirl engine

#include"WindowManager.h"
#pragma region Docking System

WindowDesc::WindowDesc(Rect rect,short thisWind,short MainWnd)
{
	SecureZeroMemory(&Pos,sizeof(Pos));
	SecureZeroMemory(&borders,sizeof(borders));


	Width = rect.Width;
	Height = rect.Height;

	Pos.left = rect.X;
	Pos.top = rect.Y;
	Pos.right = Pos.left + Width;
	Pos.bottom = Pos.top + Height;

	borders.Horizontal.left = rect.X + rect.BorderX;
	borders.Horizontal.right = Pos.right - rect.BorderX;
	borders.Horizontal.top = rect.Y;
	borders.Horizontal.bottom = rect.Y + rect.BorderY;

	borders.Vertical.right = Pos.right;
	borders.Vertical.left = Pos.right - rect.BorderX;
	borders.Vertical.top = Pos.top + rect.BorderY;
	borders.Vertical.bottom = Pos.bottom - rect.BorderY;
	
	thisWindow = thisWind;
	MainWindow = MainWnd;

}
#pragma endregion

#pragma region Winodw Creatioin and Update
WindowManager::WindowManager()
{
	
	
}

void WindowManager::CreateNewWindow(HINSTANCE hinst,WNDCLASSEX * ex,LPCWSTR WndName,HWND * Parent,HWND * ReturnWind,Rect rect,bool MainWindow)
{
	if(Parent == NULL)
	{
		hwnd = CreateWindowEx(NULL,ex->lpszClassName,WndName,WS_OVERLAPPEDWINDOW,rect.X,rect.Y,rect.Width,rect.Height,NULL,NULL,hinst,NULL);
		wnds.push_back(hwnd);
		*ReturnWind = hwnd;

	}else{
		
		hwnd = CreateWindowEx(NULL,ex->lpszClassName,WndName, WS_EX_LAYERED ,rect.X,rect.Y,rect.Width,rect.Height,*Parent,NULL,hinst,NULL);
	//	SetParent(hwnd,*Parent);
		wnds.push_back(hwnd);
		*ReturnWind = hwnd;
	}
}

void WindowManager::ShowWindows(int show)
{
	for(vector<HWND>::iterator it = wnds.begin();it != wnds.end() ; ++it)
	{
		ShowWindow(*it,show);
	}
}


int WindowManager::HandleMseeages()
{
	MSG msg;

 for(int i=0;i< wnds.size();++i)
	{
		if(PeekMessage(&msg,wnds[i],0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
				return 0;
		}
	}
 

 return 1;
}



vector<HWND> WindowManager::GetList()
{
	return wnds;
}

#pragma endregion