#ifndef TERMINATION_CRITERIA_H
#define	TERMINATION_CRITERIA_H

#include	<oreore/common/DLLExport.h>



namespace ealib
{

	class EAStatistics;

	enum DEFAULT_TERMINATE_MODE
	{
		TERMINATE_UNKNOWN = -1,
		TERMINATE_UPON_GENERATION,
		TERMINATE_UPON_CONVERGENCE,
		TERMINATE_UPON_POP_CONVERGENCE,
		NUM_DEFAULT_TERMINATE_MODE,
	};



	class CLASS_DECLSPEC ITerminationCriteria
	{
	public:

		ITerminationCriteria()
		{

		}


		~ITerminationCriteria()
		{

		}


		virtual bool CheckTermination( EAStatistics* stat )=0;
	};



}// end of namespace



#endif // !TERMINATION_CRITERIA_H
