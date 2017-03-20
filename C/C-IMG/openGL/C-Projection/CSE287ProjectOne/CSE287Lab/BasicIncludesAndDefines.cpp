#include "BasicIncludesAndDefines.h"


color getRandomColor()
{
	float red = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float green = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float blue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	return color(red, green, blue, 1.0f);

} // end getRandomColor

void print(const glm::vec2 & v0)
{
	std::cout << "[ " << v0.x << " " << v0.y << " ]" << std::endl;
}

void print(const glm::vec3 & v0)
{
	std::cout << "[ " << v0.x << " " << v0.y << " " << v0.z << " " << " ]" << std::endl;
}

void print(const glm::vec4 & v0)
{
	std::cout << "[ " << v0.x << " " << v0.y << " " << v0.z << " " << v0.w << " ]" << std::endl;
}

void print(const glm::mat2 & m)
{
	std::cout << "\n";

	for (int row = 0; row < 2; row++) {
		std::cout << "|\t";
		for (int col = 0; col < 2; col++) {
			std::cout << m[col][row] << "\t";
		}
		std::cout << "|\n";
	}
	std::cout << "\n";

}

void print(const glm::mat3 & m)
{
	std::cout << "\n";
	for (int row = 0; row < 3; row++) {
		{
			std::cout << "|\t";
			for (int col = 0; col < 3; col++) {
				std::cout << m[col][row] << "\t";
			}
		}
		std::cout << "|\n";
	}
	std::cout << "\n";
}

void print(const glm::mat4 & m)
{
	std::cout << "\n";

	for (int row = 0; row < 4; row++) {
		std::cout << "|\t";
		for (int col = 0; col < 4; col++) {
			std::cout << m[col][row] << "\t";
		}
		std::cout << "|\n";
	}
	std::cout << "\n";

}





