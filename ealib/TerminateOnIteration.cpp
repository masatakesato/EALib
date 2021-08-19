#include	"TerminateOnIteration.h"

#include	"EAStatistics.h"



namespace ealib
{


	TerminateOnIteration::TerminateOnIteration()
	{

	}



	TerminateOnIteration::~TerminateOnIteration()
	{

	}



	bool TerminateOnIteration::CheckTermination( EAStatistics* stat )
	{
		return stat->CurrentIteration() >= stat->MaxIteration();

	}


}// end of namespace
