#ifndef TERMINATE_ON_ITERATION_H
#define	TERMINATE_ON_ITERATION_H


#include	"ITerminationCriteria.h"


namespace ealib
{

	class CLASS_DECLSPEC TerminateOnIteration : public ITerminationCriteria
	{
	public:

		TerminateOnIteration();
		~TerminateOnIteration();

		virtual bool CheckTermination( EAStatistics* stat );

	};


}// end of namespace



#endif // !TERMINATE_ON_ITERATION_H
