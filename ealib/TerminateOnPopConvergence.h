#ifndef TERMINATE_ON_POP_CONVERGENCE_H
#define	TERMINATE_ON_POP_CONVERGENCE_H


#include	"ITerminationCriteria.h"


namespace ealib
{

	class CLASS_DECLSPEC TerminateOnPopConvergence : public ITerminationCriteria
	{
	public:

		TerminateOnPopConvergence();
		~TerminateOnPopConvergence();

		virtual bool CheckTermination( EAStatistics* stat );

	};


}// end of namespace



#endif // !TERMINATE_ON_POP_CONVERGENCE_H
