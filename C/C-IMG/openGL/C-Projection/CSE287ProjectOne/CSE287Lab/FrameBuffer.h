#pragma once

#include "BasicIncludesAndDefines.h"
#include "Lights.h"

extern color currentMaterial;

//extern std::vector<std::shared_ptr<LightSource>> lights;

extern glm::vec4 eyePositionInWorldCoords;

extern color currentMaterial;

/**
* Preprocessor statement for text substitution
*
* Number of bytes per pixel. Set to three when 
* storing red, green, and blue values. Set to four
* when storing red, green, blue, and alpha values.
*/
#define BYTES_PER_PIXEL 4

/**
* Structure to hold the width and height of the rendering window
* 
* Structures are very similar to classes with the exceptions that:
* 1. Members of a class are private by default and members of struct 
* are public by default. 
* 2. When deriving a struct from a class/struct, default access-specifier 
* for a base class/struct is public. And when deriving a class, default 
* access specifier is private.
*/
struct Window {
	int width;
	int height;
};

/**
* Class which controls memory that stores a color value for every pixel
* in a rendering window with a specified width and height. setBufferSize
* is used to match the size of the memory to the size of the window. 
* clearColorBuffer to the color that is specifed using setClearColor. 
* showColorBuffer copies the memory into the color buffer for display 
* by the graphics card.
*/
class FrameBuffer
{
public:

	/**
	* Constructor. Allocates memory for storing pixel values.
	*
	* @param width of the rendering window in pixels
	* @param height of the rendering window in pixels
	*/
	FrameBuffer(const int width, const int height);
	
	/**
	* Deallocates dynamically memory associated with the class.
	*/
	~FrameBuffer(void);

	/**
	* Sizes the color buffer to match the window size. Deallocates any 
	* memory that was previsouly allocated. 
	*
	* @param width of the rendering window in pixels
	* @param height of the rendering window in pixels
	*/
	void setFrameBufferSize(const int width, const int height);

	/**
	* Sets the color to which the window will be cleared. Does NOT
	* actually clear the window
	*/
	void setClearColor( const color & clearColor );

	/**
	* Clears the window to the clear color.
	*/
	void clearColorAndDepthBuffers( );

	/**
	* Copies memory into frame buffer and updates the window
	* using an OpenGL command.
	*/ 
	void showColorBuffer();

	/**
	* Returns the width of the rendering window in pixels
	* @ return width of the rendering window
	*/
	int getWindowWidth(){ return window.width;}

	/**
	* Returns the height of the rendering window in pixels
	* @ return height of the rendering window
	*/
	int getWindowHeight(){ return window.height;}

	/**
	* Sets an individual pixel value in the color buffer. Origin (0,0)
	* is the lower left hand corner of the window.
	*
	* @param x coordinate of the pixel.
	* @param y coordinate of the pixel.
	* @param color to which the pixel is to be set.
	*/
	void setPixel(const int x, const int y, const color & rgba);

	/**
	* Sets an individual pixel value in the color buffer. Origin (0,0)
	* is the lower left hand corner of the window.
	*
	* @param x coordinate of the pixel.
	* @param y coordinate of the pixel.
	* @param color to which the pixel is to be set.
	*/
	void processFragment(const float x, const float y, const color & rgba);

	/**
	* Sets an individual pixel value in the color buffer. Origin (0,0)
	* is the lower left hand corner of the window.
	*
	* @param x coordinate of the pixel.
	* @param y coordinate of the pixel.
	* @param color to which the pixel is to be set.
	*/
	void processFragment(const float x, const float y, const GLubyte rgba[]);

	/**
	* Sets the vertex data for the triangle that is being rendered currently. Set one set of 
	* data for each triangle vertix. Individual fragment values are calculated by interpolating 
	* the data attributes for the three verteces. 
	*/
	//void setV0V1V2(const VertexData & zero, const VertexData & one, const VertexData & two);

protected:

	/**
	* Finds the interpolated depth value for a specfied window position based on the
	* triangle currently being rendered.
	* @param x coordinate of the pixel.
	* @param y coordinate of the pixel.
	* @ return Interpolated depth value for the pixel position
	*/
	float findInterpolatedDepthValue(const float x, const float y);

	/**
	* Uses barycentric interpolation to find weights for the vertex attributes for the 
	* triangle currently being rendered.
	* @param point for which weights are being determined.
	* @ return weights associated with vertex attributes of the three vertices that manke
	* up the current triangle
	*/
	glm::vec3 FrameBuffer::barycentricInterpolation(const glm::vec3 & point);


	std::shared_ptr<GLubyte> FrameBuffer::getInterpolatedValuesForFragment(const glm::vec3 & point);

	/**
	* Set the depth value for a specified pixel.
	* @param x coordinate of the pixel.
	* @param y coordinate of the pixel.
	* @param depth to which the pixel is to be set.
	*/
	void setDepth(const float x, const float y, const float depth);

	/**
	* Set the depth value for a specified pixel.
	* @param x coordinate of the pixel.
	* @param y coordinate of the pixel.
	* @param depth to which the pixel is to be set.
	*/
	void setDepth(const int x, const int y, const float depth);

	/**
	* Returns the depth value for a specified pixel position.
	* @param x coordinate of the pixel.
	* @param y coordinate of the pixel.
	* @ return depth that is stored for the pixel position
	*/
	float getDepth(const int x, const int y);

	/**
	* Returns the depth value for a specified pixel position.
	* @param x coordinate of the pixel.
	* @param y coordinate of the pixel.
	* @ return depth that is stored for the pixel position
	*/
	float getDepth(const float x, const float y);

	/**
	* Struct that maintains the width and height of the rendering window
	*/
	Window window;

	/**
	* Color to which memory is cleared when clearColorBuffer is called.
	*/
	GLubyte clearColor[BYTES_PER_PIXEL];

	/**
	* Storage for red, green, blue, alpha color values 
	*/
	GLubyte* colorBuffer;

	/*
	* Storage for fragment depth values
	*/
	float* depthBuffer;

	// Parameters of the Triangle that is currently being rendered

	// Vertex data for each triangle vertex in counter-clockwise order.
	//VertexData v0;
	//VertexData v1;
	//VertexData v2;

	// Total area for the triangle currently being rendered
	// Used for barycentric interpolation of vertex attributes
	float totalTriangleArea;

	// Normal vector for the triangle currently being rendered
	// Used for depth value interpolation
	glm::vec3 triangleNormal;

	// Dot product of a point on the triangle 
	// and the triangle normal vector.
	// Used for depth value interpolation
	float normalPointDot;

}; // end FrameBuffer class

