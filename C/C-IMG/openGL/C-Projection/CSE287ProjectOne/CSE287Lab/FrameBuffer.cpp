#include "FrameBuffer.h"

/**
* Constructor. Allocates memory for storing pixel values.
*/
FrameBuffer::FrameBuffer( const int width, const int height)
:totalTriangleArea(0.0f)
{
	setFrameBufferSize(width, height);

} // end FrameBuffer constructor


/**
* Deallocates dynamically memory associated with the class.
*/
FrameBuffer::~FrameBuffer(void)
{
	// Free the memory associated with the color buffer
	delete [] colorBuffer;
	delete[] depthBuffer;

} // end FrameBuffer destructor


/**
* Sizes the color buffer to match the window size. Deallocates any
* memory that was previsouly allocated.
*/
void FrameBuffer::setFrameBufferSize( const int width, const int height) {

	// Save the dimensions of the window
	window.width = width;
    window.height = height;

	// Set pixel storage modes. 
	// (https://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Free the memory previously associated with the color buffer
	delete [] colorBuffer;
	delete[] depthBuffer;

	// Allocate the color buffer to match the size of the window
	colorBuffer = new GLubyte[width*BYTES_PER_PIXEL*height];
	depthBuffer = new float[width*height];

} // end setFrameBufferSize


/**
* Sets the color to which the window will be cleared. Does NOT
* actually clear the window
*/
void FrameBuffer::setClearColor( const color & clear ) {

	clearColor[0] = (GLubyte)(clear.r * 255.0f);
	clearColor[1] =	(GLubyte)(clear.g * 255.0f);
	clearColor[2] =	(GLubyte)(clear.b * 255.0f);
	clearColor[3] =	(GLubyte)(clear.a * 255.0f);

} // end setClearColor


/**
* Clears the window to the clear color.
*/
void FrameBuffer::clearColorAndDepthBuffers() {

	for(int y = 0; y < window.height ; ++y) {
		for(int x = 0; x < window.width; ++x) {

			std::memcpy( colorBuffer + BYTES_PER_PIXEL * (x + y * window.width ), 
						 clearColor,  BYTES_PER_PIXEL );
			//depthBuffer[y * window.width + x] = 1.0f;
		
		}
	}

} // end clearFrameBuffer


/**
* Copies memory into frame buffer and updates the window
* using an OpenGL command.
*/
void FrameBuffer::showColorBuffer()
{
	// Insure raster position is lower left hand corner of the window. (OpenGL command)
	glRasterPos2d(-1, -1);

	// Copy color buffer to raster (OpenGL command)
	glDrawPixels( window.width, window.height, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer );
	
	// Flush all drawing commands and swapbuffers (Glut command)
	glutSwapBuffers();

} // end showFrameBuffer


/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void FrameBuffer::setPixel(const int x, const int y, const color & rgba) {

	color clampedColor = clamp(rgba, 0.0f, 1.0f);


	GLubyte c[] = { (GLubyte)(clampedColor.r * 255),
		(GLubyte)(clampedColor.g * 255),
		(GLubyte)(clampedColor.b * 255),
		(GLubyte)(clampedColor.a * 255) };


	std::memcpy(colorBuffer + BYTES_PER_PIXEL * (x+ y * window.width), c, BYTES_PER_PIXEL);
	//setPixel( x, y, c);

} // end setPixel

/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void FrameBuffer::processFragment(const float x, const float y, const color & rgba) {
	
	color clampedColor = clamp(rgba, 0.0f, 1.0f);
	
	GLubyte c[] = { (GLubyte)(clampedColor.r * 255),
		(GLubyte)(clampedColor.g * 255),
		(GLubyte)(clampedColor.b * 255),
		(GLubyte)(clampedColor.a * 255) };

	processFragment(x, y, c);

} // end setPixel

/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void FrameBuffer::processFragment(const float x, const float y, const GLubyte rgba[]) {

	if (x >= 0 && x < window.width && y >= 0 && y < window.height) {

		float z = findInterpolatedDepthValue(x, y);

		//if (z < getDepth(x, y)) {

			//rgba = getInterpolatedValuesForFragment(glm::vec3(x, y, z));

			// Write to the color buffer
			int xint = (int)(x + 0.5f);
			int yint = (int)(y + 0.5f);
			std::memcpy(colorBuffer + BYTES_PER_PIXEL * (xint + yint * window.width), rgba, BYTES_PER_PIXEL);
			//setDepth(xint, yint, z);
		//}
	}

} // end setPixel


/** 
* Function (not a method) that findes the area of a triangle
*/
float findTriangleArea(const glm::vec3 & vertex0, const glm::vec3 &  vertex1, const glm::vec3 &  vertex2)
{
	return glm::abs(0.5f * glm::length(glm::cross(vertex1 - vertex0, vertex2 - vertex0)));

} // end findTriangleArea


/**
* Uses barycentric interpolation to find weights for the vertex attributes for the
* triangle currently being rendered.
*/
glm::vec3 FrameBuffer::barycentricInterpolation(const glm::vec3 & point)
{
	glm::vec3 weights; // Will hold the weight for each of the vertices

	// Find the areas of each of the sub-triangles
	//float areaPpP1P2 = findTriangleArea(point, v1.position.xyz, v2.position.xyz);
	//float areaP0PpP2 = findTriangleArea(v0.position.xyz, point, v2.position.xyz);
	//float areaP0P1Pp = findTriangleArea(v0.position.xyz, v1.position.xyz, point);

	// Calculate a weight for each of the vertices relative to this pixel
	//weights[0] = areaPpP1P2 / totalTriangleArea;
	//weights[1] = areaP0PpP2 / totalTriangleArea;
	//weights[2] = areaP0P1Pp / totalTriangleArea;

	return weights;

} // end barycentricInterpolation


std::shared_ptr<GLubyte> FrameBuffer::getInterpolatedValuesForFragment(const glm::vec3 & point)
{
	// Determine weights for vertex attributes using barycentric interpolaion
	glm::vec3 weights = barycentricInterpolation(point);

	// Find interpolated color for the fragment
	//color fragColor = weights[0] * v0.material +
	//	              weights[1] * v1.material + 
	//				  weights[2] * v2.material;

	//fragColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	//for (int j = 0; j < (int)lights.size(); j++) {

	//	fragColor += lights[j]->illuminate(eyePositionInWorldCoords.xyz, v0);

	//}

	//fragColor = glm::clamp(fragColor, 0.0f, 1.0f);


	// Convert the from floating point representation
	//GLubyte red = (GLubyte)(255.0f * fragColor.r);
	//GLubyte green = (GLubyte)(255.0f * fragColor.g);
	//GLubyte blue = (GLubyte)(255.0f * fragColor.b);
	//GLubyte alpha = (GLubyte)(255.0f * fragColor.a);

	std::shared_ptr<GLubyte> byteColor;// = { red, green, blue, alpha };

	return byteColor;

} // end getInterpolatedValuesForFragment

/**
* Sets the vertex data for the triangle that is being rendered currently. Set one set of
* data for each triangle vertix. Individual fragment values are calculated by interpolating
* the data attributes for the three verteces.
*/
/*
void FrameBuffer::setV0V1V2(const VertexData & zero, const VertexData & one, const VertexData & two)
{
	// Store the vertex attributes for interpolation accross the face of the triangle
	v0 = zero;
	v1 = one;
	v2 = two;

	// Create two vectors in the plane of the triangle
	glm::vec3 dif1 = v1.position.xyz - v0.position.xyz;
	glm::vec3 dif2 = v2.position.xyz - v0.position.xyz;

	// Find and store a normal vector for the polygon
	triangleNormal = glm::normalize(glm::cross(dif1, dif2));

	// Find and store the dot product of the 
	glm::vec3 a = v0.position.xyz;
	normalPointDot = -glm::dot(a, triangleNormal);

	// Calculate the total area of the triangle being rendered for use in barycentric interpolation
	totalTriangleArea = findTriangleArea(v0.position.xyz, v1.position.xyz, v2.position.xyz);

} // end setV0V1V2
*/

// Finds the interpolated depth coordinate based on the depths of the corners of the triangle
float FrameBuffer::findInterpolatedDepthValue(const float x, const float y)
{
	// Solve for the interpolated depth value
	return (-triangleNormal.x * x - triangleNormal.y * y - normalPointDot) / triangleNormal.z;

} // end findInterpolatedDepthValue


/**
* Set the depth value for a specified pixel.
*/
void FrameBuffer::setDepth(const float x, const float y, const float depth) {
	
	setDepth((int)(x + 0.5f), (int)(y + 0.5f), depth);

} // end setDepth

/**
* Set the depth value for a specified pixel.
*/
void FrameBuffer::setDepth(const int x, const int y, const float depth) {
	
	depthBuffer[y * window.width + x] = depth;

} // end setDepth

/**
* Returns the depth value for a specified pixel position.
*/
float FrameBuffer::getDepth(const int x, const int y) {

	return depthBuffer[y * window.width + x];

} // end getDepth

/**
* Returns the depth value for a specified pixel position.
*/
float FrameBuffer::getDepth(const float x, const float y) {

	return getDepth((int)(x + 0.5f), (int)(y + 0.5f));

} // end getDepth
