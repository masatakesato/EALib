#ifndef INITIALIZER_H
#define	INITIALIZER_H


#include	"Typedefs.h"
#include	"IInitOperator.h"



namespace ealib
{
	class IChromosome;

	class CLASS_DECLSPEC Initializer
	{
	public:

		Initializer();
		virtual ~Initializer();

		void BindOperator( IInitializeOperator* pOperator );
		void UnbindOperator( int type );
		void Execute( IChromosome* chromosome );


	protected:

		IInitializeOperator*	m_refOperators[ NUM_TYPES ];

	};


}// end of namespace



#endif // !INITIALIZER_H