//// Author: Karthik Hebbar M N
//// Website FissionSpark.weebly.com
//// Engine Whirl engine

#include"DXApp.h"


DXAPP::DXAPP()
{
	


}

void DXAPP::CreateD3DDevice(HWND hwnd,RECT rect)
{
	D3D_FEATURE_LEVEL featurelevels[]  = { 
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_3

	};
	ID3D11Device * d3dDevice = {0}; // Links with the GPU
	ID3D11DeviceContext * devCon = {0}; // Links with Post processing and Rendering 
	D3D11CreateDevice(0,D3D_DRIVER_TYPE_HARDWARE,0,0,featurelevels,5,D3D11_SDK_VERSION,&d3dDevice,&RefFeatureLevel,&devCon);

	// Check MSAA Quality 
	UINT msaa = 0;
	d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,4,&msaa);

	// Swap chains
	DXGI_SWAP_CHAIN_DESC desc;
	SecureZeroMemory(&desc,sizeof(desc));
	
	// Describe back buffer
	desc.BufferCount =1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.Height = rect.bottom - rect.top;
	desc.BufferDesc.Width = rect.right - rect.left;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator =1;
	desc.OutputWindow = hwnd;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	desc.Windowed = true;
	desc.SampleDesc.Count = 4;
	desc.SampleDesc.Quality = msaa  -1;
	
	
	

	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	// Get the Swap chain
	 // d3ddev >  DXGIDEVICE > adaptoe > fctory > SwapChain
	IDXGIDevice * device = {0};
	IDXGIAdapter * adaptor = {0};
	IDXGIFactory * factory = {0};

	d3dDevice->QueryInterface(__uuidof(IDXGIDevice),(void**)&device);
	
	device->GetParent(__uuidof(IDXGIAdapter),(void**)&adaptor);
	adaptor->GetParent(__uuidof(IDXGIFactory),(void**)&factory);

	IDXGISwapChain * swapChain;
	factory->CreateSwapChain(d3dDevice,&desc,&swapChain);

	
	device->Release();
	adaptor->Release();
	factory->Release();


	#pragma region  Render Target

	// Create Render TargetView
	ID3D11RenderTargetView * RenderTarget  = {0};
	ID3D11Texture2D * backbuffer = {0};
	
	// allocate memory


	swapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)(&backbuffer));                // Get buffer tot the texture that acts as back buffer
	HRESULT hr = d3dDevice->CreateRenderTargetView(backbuffer,NULL,&RenderTarget);
	
	backbuffer->Release();                                                                                 // Release COM objects
      #pragma endregion

	#pragma region Depth Bufffer
	 
	D3D11_TEXTURE2D_DESC depthDesc;                                       // Describe backbuffer
	SecureZeroMemory(&depthDesc,sizeof(depthDesc));                       // Allocate memory

	depthDesc.ArraySize = 1;
	depthDesc.MipLevels = 1;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.Width =  rect.right - rect.left;
	depthDesc.Height = rect.bottom - rect.top;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Quality = desc.SampleDesc.Quality;
	depthDesc.SampleDesc.Count = desc.SampleDesc.Count;

	ID3D11Texture2D * depthBuffer = {0};
	ID3D11DepthStencilView * depthView = {0};

	d3dDevice->CreateTexture2D(&depthDesc,NULL,&depthBuffer);                 // Create the depthbuffer texture and store in VRAM and return the pointer to it
	HRESULT hr1 = d3dDevice->CreateDepthStencilView(depthBuffer,NULL,&depthView);           // Create Depth view using the pointer 

	depthBuffer->Release();
	#pragma endregion

	#pragma region Bind to output
	devCon->OMSetRenderTargets(1,&RenderTarget,depthView);
	D3D11_VIEWPORT vp;
	SecureZeroMemory(&vp,sizeof(vp));
	vp.Width =  rect.right - rect.left;
	vp.Height = rect.bottom - rect.top; 
	vp.MinDepth =0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	devCon->RSSetViewports(1,&vp);
#pragma endregion

	d3devices.push_back(d3dDevice);
	d3dcon.push_back(devCon);
	d3dwnds.push_back(hwnd);
	RenderTargets.push_back(RenderTarget);
	SwapChains.push_back(swapChain);
	DepthViews.push_back(depthView);

	if(FAILED(hr))
	{
		MessageBox(hwnd,L"Couldnoit create RenderTexture View",L"ERROR",0);
	}

	if(FAILED(hr1))
	{
		MessageBox(hwnd,L"Couldnoit create BackBuffer View",L"ERROR",0);
	}



}

void DXAPP::Clear(HWND hwnd,D3DXCOLOR col)
{
	if(d3dwnds.size() >= 2)
	{
		for(int i=0; i < d3dwnds.size();++i)
		{
			if(d3dwnds.at(i) == hwnd)
			{
			
				d3dcon.at(i)->ClearRenderTargetView(RenderTargets.at(i),col);
				d3dcon.at(i)->ClearDepthStencilView(DepthViews.at(i),D3D11_CLEAR_DEPTH,1.0f,0);
			
			}
		}
	}else{
	
		d3dcon.at(0)->ClearRenderTargetView(RenderTargets.at(0),col);
		d3dcon.at(0)->ClearDepthStencilView(DepthViews.at(0),D3D11_CLEAR_DEPTH,1,0);
	} 
}



void DXAPP::SwapChain()  //Swap tjhe back and front buffer .This must be called at the end of rendering process 
{
	if(d3dwnds.size() >= 2)
	{
		for(int i=0; i < d3dwnds.size();++i)
		{
			SwapChains.at(i)->Present(0,0);
		}
	}else{
		SwapChains.at(0)->Present(0,0);	
 	}
}


ID3D11DeviceContext* DXAPP::GetDevicCon(HWND hwnd)
{
	for(int i=0;i<d3dwnds.size();++i)
	{
		if(d3dwnds.at(i) == hwnd)
			return d3dcon.at(i);
	}

	return NULL;
}