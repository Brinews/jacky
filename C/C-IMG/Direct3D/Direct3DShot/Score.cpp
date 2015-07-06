#include "Score.h"


Score::Score(void)
{
	_score = 0;
}


Score::~Score(void)
{
}


void Score::IncrimentScore()
{
	_score += 10;
}

char* Score::GetScore()
{
	
	std::ostringstream out;
	out << _score;

	char* output =  strdup(out.str().c_str());
	return output;

}

