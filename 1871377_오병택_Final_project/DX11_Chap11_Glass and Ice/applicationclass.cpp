////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Light = 0;
	m_Light2 = 0;
	m_WindowModel = 0;
	m_WindowModel2 = 0;
	m_RenderTexture = 0;
	m_RenderTexture2 = 0;
	m_TextureShader = 0;
	m_GlassShader = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char modelFilename[128], textureFilename1[128], textureFilename2[128];
	bool result;


	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the camera object.
	m_Camera = new CameraClass;

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera->Render();



	// 첫번째 모델, 텍스처, normal 설정
    strcpy_s(modelFilename, "data/cube.txt");
    strcpy_s(textureFilename1, "data/stone01.tga");
    strcpy_s(textureFilename2, "data/normal03.tga");

    // Create and initialize the cube model object.
    m_Model = new ModelClass;

    result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename1, textureFilename2);
    if(!result)
    {
		MessageBox(hwnd, L"Could not initialize the cube model object.", L"Error", MB_OK);
        return false;
    }

	// 두번째 모델, 텍스처, normal 설정
    strcpy_s(modelFilename, "data/square.txt");
    strcpy_s(textureFilename1, "data/glass01.tga");
    strcpy_s(textureFilename2, "data/normal03.tga");

    // Create and initialize the window model object.
    m_WindowModel  = new ModelClass;

    result = m_WindowModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename1, textureFilename2);
    if(!result)
    {
		MessageBox(hwnd, L"Could not initialize the window model object.", L"Error", MB_OK);
        return false;
    }

	// 세번째 모델, 텍스처, normal 설정
	strcpy_s(textureFilename1, "data/ice01.tga");
	strcpy_s(textureFilename2, "data/icebump01.tga");

	m_WindowModel2 = new ModelClass;

	result = m_WindowModel2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename1, textureFilename2);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the window model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the render to texture object.
	m_RenderTexture  = new RenderTextureClass;

	result = m_RenderTexture ->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the render to texture object.
	m_RenderTexture2 = new RenderTextureClass;

	result = m_RenderTexture2->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}

	// TextureShader 생성
	m_TextureShader  = new TextureShaderClass;

	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// GlassShader 생성
	m_GlassShader = new GlassShaderClass;

	result = m_GlassShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the glass shader object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	m_Light->SetDiffuseColor(1.5f, 0.5f, 1.5f, 1.0f);
	m_Light->SetDirection(1.0f, 0.5f, 1.0f);


	m_Light2 = new LightClass;
	m_Light2->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light2->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}


void ApplicationClass::Shutdown()
{
	 // Release the glass shader object.
    if(m_GlassShader)
    {
        m_GlassShader->Shutdown();
        delete m_GlassShader;
        m_GlassShader = 0;
    }

    // Release the texture shader object.
    if(m_TextureShader)
    {
        m_TextureShader->Shutdown();
        delete m_TextureShader;
        m_TextureShader = 0;
    }

    // Release the render to texture object.
    if(m_RenderTexture)
    {
        m_RenderTexture->Shutdown();
        delete m_RenderTexture;
        m_RenderTexture = 0;
    }

	// Release the render to texture object.
	if (m_RenderTexture2)
	{
		m_RenderTexture2->Shutdown();
		delete m_RenderTexture2;
		m_RenderTexture2 = 0;
	}

    // Release the window model object.
    if(m_WindowModel)
    {
        m_WindowModel->Shutdown();
        delete m_WindowModel;
        m_WindowModel = 0;
    }

	if (m_WindowModel2)
	{
		m_WindowModel2->Shutdown();
		delete m_WindowModel2;
		m_WindowModel2 = 0;
	}

    // Release the cube model object.
    if(m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	if (m_Light) {
		delete m_Light;
		m_Light = 0;
	}

	if (m_Light2) {
		delete m_Light2;
		m_Light2 = 0;
	}

	return;
}


bool ApplicationClass::Frame(InputClass* Input)
{
	static float rotation = 0.0f;
	bool result;


	// Check if the user pressed escape and wants to exit the application.
	if(Input->IsEscapePressed())
	{
		return false;
	}

	if (Input->IsLeftArrowPressed()) {
		rotation += 0.0174532925f * 0.5f;
	}

	if (Input->IsRightArrowPressed()) {
		rotation -= 0.0174532925f * 0.5f;
	}

	 // Render the cube spinning scene to texture first.
    result = RenderSceneToTexture(rotation);
    if(!result)
    {
        return false;
    }

	return true;
}


bool ApplicationClass::RenderSceneToTexture(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Set the render target to be the render to texture and clear it.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the cube will spin.
	worldMatrix = XMMatrixRotationY(rotation); 
	worldMatrix *= XMMatrixTranslation(0, 0, 0);
	// Render the cube model using the texture shader.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Model->GetTexture(0), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light2->GetDirection(), m_Light2->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	
	// Render the graphics scene.
	result = RenderSceneToTexture2(rotation);
	if (!result)
	{
		return false;
	}


	return true;
}

bool ApplicationClass::RenderSceneToTexture2(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	
	m_RenderTexture2->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RenderTexture2->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Render the window model using the glass shader.
	m_WindowModel->Render(m_Direct3D->GetDeviceContext());

	result = m_GlassShader->Render(m_Direct3D->GetDeviceContext(), m_WindowModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				m_WindowModel->GetTexture(0), m_WindowModel->GetTexture(1), m_RenderTexture->GetShaderResourceView(), 0.01f);
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.

	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}


bool ApplicationClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	static float refractionScale = 0.01f;
	bool result;


	// Set the refraction scale for the glass shader.
    //refractionScale 
	refractionScale += 0.00001f;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the cube will spin.
	worldMatrix = XMMatrixRotationY(rotation); 

	// Render the cube model using the texture shader.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				m_Model->GetTexture(0), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light2->GetDirection(), m_Light2->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	// Translate to back where the window model will be rendered.
	worldMatrix = XMMatrixTranslation(2.0f, 1.0f, -1.5f);
	worldMatrix *= XMMatrixScaling(0.5f, 0.5f, 1.f);


	// Render the window model using the glass shader.
	m_WindowModel->Render(m_Direct3D->GetDeviceContext());

	result = m_GlassShader->Render(m_Direct3D->GetDeviceContext(), m_WindowModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				m_WindowModel->GetTexture(0), m_WindowModel->GetTexture(1), m_RenderTexture->GetShaderResourceView(), refractionScale);
	if(!result)
	{
		return false;
	}

	worldMatrix *= XMMatrixTranslation(-2, 0, 0.0f);
	// Render the window model using the glass shader.
	m_WindowModel2->Render(m_Direct3D->GetDeviceContext());

	result = m_GlassShader->Render(m_Direct3D->GetDeviceContext(), m_WindowModel2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_WindowModel2->GetTexture(0),
									m_WindowModel2->GetTexture(1), m_RenderTexture2->GetShaderResourceView(), refractionScale);




	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}