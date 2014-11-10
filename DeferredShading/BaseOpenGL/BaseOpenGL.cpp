#include "stdafx.h"



#include "Camera.h"
#include "ShaderProgram.h"
#include "BasicRenderableObject.h"
#include "MultipleRenderTarget.h"



/////////////////////////////////////////////////////////
// Variables globales pour la gestion de l'application //
/////////////////////////////////////////////////////////

int				g_width = 800;
int				g_height = 600;

double			g_PreviousXPosition;
double			g_PreviousYPosition;
bool			g_LeftButtonPressed;
bool			g_RightButtonPressed;

double			g_CurrentTime;
double			g_PreviousTime;

Camera*			g_Camera;

BasicRenderableObject*				g_MainObject;
BasicRenderableObject*				g_FullScreenQuad;
MultipleRenderTarget*				g_MultipleRenderTarget;



///////////////////////////
// Fonctions auxiliaires //
///////////////////////////



void initialiseGLState()
{
	printGLErrors("initialiseGLState() start");

	glClearColor(0.75, 0.75, 0.75, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	g_Camera = new Camera((float)g_width, (float)g_height);
	g_MultipleRenderTarget = new MultipleRenderTarget();
	g_MultipleRenderTarget->Create(g_width, g_height);

	printGLErrors("initialiseGLState() end");
}

void initialiseObjects()
{
	printGLErrors("initialiseObjects() start");

	// creation du VBO
	g_MainObject = new BasicRenderableObject();

	g_MainObject->initShader( ShaderProgram::createDeferredShadingShader() );
	g_MainObject->init();
	g_MainObject->loadMtl("Models/sibenik_blenderexport.mtl");
	g_MainObject->loadObj("Models/sibenik_blenderexport.obj");
	//g_MainObject->loadMtl("Models/sponza/sponza.mtl");
	//g_MainObject->loadObj("Models/sponza/sponza.obj");
	g_MainObject->fillInVBO();
	g_MainObject->createVertexArrayObject();

	g_FullScreenQuad = new BasicRenderableObject();

	g_FullScreenQuad->initShader( ShaderProgram::createPostProcessShader() );
	g_FullScreenQuad->init();
	g_FullScreenQuad->generateFullScreenQuad();
	g_FullScreenQuad->fillInVBO();
	g_FullScreenQuad->createVertexArrayObject();
	//g_FullScreenQuad->setTextureTarget(g_MultipleRenderTarget->GetTextureTargetId());

	printGLErrors("initialiseObjects() end");
}

void display()
{
	g_CurrentTime = glfwGetTime();

	// Deferred shading workflow

	// Geometry pass
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	g_MultipleRenderTarget->ActivateForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_MainObject->draw(g_Camera->GetViewMatrix(), g_Camera->GetProjectionMatrix());
	
	g_MultipleRenderTarget->Desactivate();

	// Light pass
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	// Show G-Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_MultipleRenderTarget->ActivateForReading();

	// Details
	GLsizei thumbnailWidth = g_width / 4;
	GLsizei thumbnailHeight = g_height / 4;
	GLsizei halfWidth = (GLsizei)(thumbnailWidth / 2.0f);
    GLsizei halfHeight = (GLsizei)(thumbnailHeight / 2.0f);

	g_MultipleRenderTarget->SetReadBuffer(MultipleRenderTarget::TEXTURE_POSITION);
    glBlitFramebuffer(0, 0, g_width, g_height,
                    0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    g_MultipleRenderTarget->SetReadBuffer(MultipleRenderTarget::TEXTURE_NORMAL);
    glBlitFramebuffer(0, 0, g_width, g_height, 
                    0, halfHeight, halfWidth, thumbnailHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    g_MultipleRenderTarget->SetReadBuffer(MultipleRenderTarget::TEXTURE_TEXCOORD);
    glBlitFramebuffer(0, 0, g_width, g_height, 
                    halfWidth, halfHeight, thumbnailWidth, thumbnailHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    g_MultipleRenderTarget->SetReadBuffer(MultipleRenderTarget::TEXTURE_DIFFUSE);
    glBlitFramebuffer(0, 0, g_width, g_height, 
                    halfWidth, 0, thumbnailWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);	

	g_PreviousTime = g_CurrentTime;

	printGLErrors("Error during drawing");
}

void resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
	g_Camera->UpdateProjectionMatrix((float)width, (float)height);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_Camera->translateLocal(0.0f, 0.0f, 1.0f);
	}
	if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_Camera->translateLocal(0.0f, 0.0f, -1.0f);
	}
	if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_Camera->translateLocal(1.0f, 0.0f, 0.0f);
	}
	if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		g_Camera->translateLocal(-1.0f, 0.0f, 0.0f);
	}

	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS)
		{
			g_LeftButtonPressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			g_LeftButtonPressed = false;
		}
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if(action == GLFW_PRESS)
		{
			g_RightButtonPressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			g_RightButtonPressed = false;
		}
	}
}

void mouseMove_callback(GLFWwindow* window, double x, double y)
{
	if(g_LeftButtonPressed)
	{
		double xMove = x - g_PreviousXPosition;
		double yMove = y - g_PreviousYPosition;

		g_Camera->rotateLocal((float)xMove * 0.5f, 0.0f, 1.0f, 0.0f);
		g_Camera->rotateLocal((float)yMove * 0.5f, 1.0f, 0.0f, 0.0f);
	}

	g_PreviousXPosition = x;
	g_PreviousYPosition = y;
}



//////////
// Main //
//////////



int main(int argc, char* argv[])
{
	GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(g_width, g_height, "OpenGL Window", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseMove_callback);
	glfwSetMouseButtonCallback(window, mouseButton_callback);

	// Initialize GLEW
	glewExperimental = GL_TRUE; 

	GLenum err = glewInit();

	if(GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}
	else
	{
		if (GLEW_VERSION_3_3)
		{
			std::cout << "Driver supports OpenGL 3.3" << std::endl;

			std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
			std::cout << "Vendor: " << glGetString (GL_VENDOR) << std::endl;
			std::cout << "Renderer: " << glGetString (GL_RENDERER) << std::endl;
			std::cout << "Version: " << glGetString (GL_VERSION) << std::endl;
			std::cout << "GLSL: " << glGetString (GL_SHADING_LANGUAGE_VERSION) << std::endl;
		}
	}

	// Initialize our GL state
	initialiseGLState();

	// Create and initialize the objets in the scene
	initialiseObjects();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
		display();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
