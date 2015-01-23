//// Author: Karthik Hebbar M N
//// Website FissionSpark.weebly.com
//// Engine Whirl engine

#include<d3d11.h>
#include<D3DX11.h>
#include<D3DX10.h>
#include<d3dx9.h>
#include<xnamath.h>
#include<vector>
#include<iostream>

using namespace std;


class DXAPP
{

private:
	vector<ID3D11Device*> d3devices;
	vector<HWND> d3dwnds;
	vector<ID3D11DeviceContext*> d3dcon;
	vector<ID3D11RenderTargetView*> RenderTargets;
	vector<ID3D11DepthStencilView*> DepthViews;
	vector<IDXGISwapChain*> SwapChains;	

	D3D_FEATURE_LEVEL RefFeatureLevel;
public:
	DXAPP(); // Constructor
	 // Destructor
	void CreateD3DDevice(HWND hwnd,RECT rect);
	void Clear(HWND hwnd,D3DXCOLOR color);
	ID3D11DeviceContext* GetDevicCon(HWND hwnd);
	void SwapChain();  //Swap tjhe back and front buffer .This must be called at the end of rendering process 
	
};