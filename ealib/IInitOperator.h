#ifndef I_INITIALIZE_OPERATOR_H
#define	I_INITIALIZE_OPERATOR_H


#include	"DLLExport.h"


namespace ealib
{
	class IChromosome;

	class CLASS_DECLSPEC IInitializeOperator
	{
	public:

		const int16 TypeID;

		IInitializeOperator( int type=-1 )
			: TypeID( type )
		{

		}


		virtual ~IInitializeOperator()
		{

		}


		virtual void Execute( IChromosome* chromosome )=0;

	};


}// end of namespace



#endif // !I_INITIALIZE_OPERATOR_H