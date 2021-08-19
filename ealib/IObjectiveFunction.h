#ifndef I_OBJECTIVE_FUNCTION_H
#define	I_OBJECTIVE_FUNCTION_H


#include	"DLLExport.h"
#include	"IChromosome.h"



namespace ealib
{

	class CLASS_DECLSPEC IObjectiveFunction
	{
	public:

		IObjectiveFunction(){}
		virtual ~IObjectiveFunction(){}
		virtual float Execute( IChromosome* chromosome, const void* attribs=0 )=0;

	};


}// end of namespace



#endif // !I_OBJECTIVE_FUNCTION_H
