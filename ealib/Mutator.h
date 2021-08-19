#ifndef MUTATOR_H
#define	MUTATOR_H

#include	"DLLExport.h"
#include	"Typedefs.h"
#include	"IMutationOperator.h"




namespace ealib
{
	class IChromosome;


	class CLASS_DECLSPEC Mutator
	{
	public:

		Mutator();
		virtual ~Mutator();

		void BindOperator( IMutationOperator* pOperator );
		void UnbindOperator( int type );
		void Execute( IChromosome* chromosome, float mutate_prob );


	private:

		IMutationOperator*	m_refOperators[ NUM_TYPES ];



	};



}// end of namespace



#endif // !MUTATOR_H
