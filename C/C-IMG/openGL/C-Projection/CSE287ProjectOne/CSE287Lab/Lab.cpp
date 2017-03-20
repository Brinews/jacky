#include "Lab.h"
#include <time.h> 

#include "ConvexPolygon.h"
#include "SolidSphere.h"
#include <vector>

using namespace std;

//******** Global Variables ***********

// Frame buffer for holding pixel color and depth values
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Ray tracer that generates the rendered image
RayTracer rayTrace(frameBuffer);

// Surfaces or object in the scene to be rendered
std::vector<std::shared_ptr<Surface>> surfaces;

// Light sources in the scene
std::vector<std::shared_ptr<LightSource>> lights;

//***********************************

/**
* Acts as the display function for the window. 
*/
static void RenderSceneCB()
{
	// Get time before rendering the scene
	int frameStartTime = glutGet( GLUT_ELAPSED_TIME ); 

	// Clear the color buffer
	//frameBuffer.clearColorAndDepthBuffers( ); // Not necessary for ray tracing

	// Ray trace the scene to determine the color of all the pixels in the scene
	rayTrace.raytraceScene( surfaces, lights);

	// Display the color buffer
	frameBuffer.showColorBuffer();

	int frameEndTime = glutGet( GLUT_ELAPSED_TIME ); // Get end time

	// Calculate and display time to render the scene
	float totalTimeSec = (frameEndTime - frameStartTime) / 1000.0f;
	std::cout << "Render time: " << totalTimeSec << " sec." << std::endl;

} // end RenderSceneCB


// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height)
{
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize( width, height );

	rayTrace.setCameraFrame(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

	rayTrace.calculatePerspectiveViewingParameters(45.0f);

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB


// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch(key) {

	case('f'): case('F') : // 'f' key to toggle full screen
		glutFullScreenToggle();
		break;
	case(27): // Escape key
		glutLeaveMainLoop();
		break;
	case('0') :
		rayTrace.recursionDepth = 0;
		break;
	case('1') :
		rayTrace.recursionDepth = 1;
		break;
	case('2') :
		rayTrace.recursionDepth = 2;
		break;
	case('d') :
		lights[0]->lightColor = glm::vec4(0.35, 0.35, 0.35, 1.0);
		break;
	case('n') :
		lights[0]->lightColor = glm::vec4(0.95, 0.95, 0.95, 1.0);
		break;
	case('i') :
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end KeyboardCB


// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y)
{
	switch(key) {
	
	case(GLUT_KEY_RIGHT):
		
		break;
	case(GLUT_KEY_LEFT):
		
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end SpecialKeysCB





void buildScene()
{
	// Initialize random seed - used to create random colors
	srand((unsigned int)time(NULL));

	// Create objects in the scene
	std::shared_ptr<Sphere> redBall = std::make_shared<Sphere>(glm::vec3(-5.0f, -1.0f, -12.0f), 1.0f, color(0.8f, 0.3f, 0.3f, 1.0f));

	// Add objects to vector of objects in the scene
	surfaces.push_back(redBall);

	// Create objects in the scene
	std::shared_ptr<Sphere> blueBall = std::make_shared<Sphere>(glm::vec3(5.0f, -1.0f, -12.0f), 1.0f, color(0.3f, 1.0f, 0.3f, 1.0f));

	// Add objects to vector of objects in the scene
	surfaces.push_back(blueBall);

	std::shared_ptr<Plane> plane = std::make_shared<Plane>(glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),color(0.980f, 0.922f, 0.843f, 1.0f));

	surfaces.push_back(plane);

	vector<glm::vec3> list;
	list.push_back(glm::vec3(1, 1, -12));
	list.push_back(glm::vec3(-1, 1, -12));
	list.push_back(glm::vec3(-1, 0, -12));
	list.push_back(glm::vec3(1, 0, -12));

	std::shared_ptr<ConvexPolygon> polygon = std::make_shared<ConvexPolygon>(list, color(1.0f, 0.180f, 0.904f, 1.0f));
	
	surfaces.push_back(polygon);

	std::shared_ptr<SolidSphere> sold = std::make_shared<SolidSphere>(glm::vec3(2.0f, 2.5f, -10.0f), 
			color(0.180f, 0.022f, 0.743f, 1.0f), 2, 10, 10);

	surfaces.push_back(sold);


	// Create light sources
	std::shared_ptr<LightSource> ambientLight = std::make_shared<LightSource>(color(0.15, 0.15, 0.15, 1.0));
	std::shared_ptr<PositionalLight> lightPos = std::make_shared<PositionalLight>(glm::vec3(5, 5, 0), glm::vec4(.75, .75, .75, 1));

	std::shared_ptr<DirectionalLight> lightDir = std::make_shared<DirectionalLight>(glm::vec3(-5, -5, 0), glm::vec4(.75, .75, .75, 1));
	//DirectionalLight lightDir(vec3(-5, -5, 0), vec4(.75, .75, .75, 1));

	// Add light sources to vector of light sources
	lights.push_back(lightPos);
	lights.push_back(lightDir);
	lights.push_back(ambientLight);
}


// Register as the "idle" function to have the screen continously
// repainted. Due to software rendering, the frame rate will not
// be fast enough to support motion simulation
static void animate()  
{
	glutPostRedisplay();

} // end animate


int main(int argc, char** argv)
{
	// freeGlut and Window initialization ***********************

    // Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
    glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA );

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("Ray Trace");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(color(1.0f,0.5f,1.0f,1.0f));

	rayTrace.setDefaultColor(color(0.0f, 0.0f, 0.0f, 1.0f));

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);		
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);

	buildScene();

	// Enter the GLUT main loop. Control will not return until the window is closed.
    glutMainLoop();

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.

	return 0;

} // end main