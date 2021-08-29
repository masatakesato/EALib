#ifndef FLIP_BIT_MUTATION_H
#define	FLIP_BIT_MUTATION_H

#include	"IMutationOperator.h"



namespace ealib
{
	// Flip bit mutation( for BitArray )
	class CLASS_DECLSPEC FlipBitMutation : public IMutationOperator
	{
	public:
	
		FlipBitMutation();
		~FlipBitMutation();
	
		virtual void Execute( IChromosome* chromosome, float mutate_prob, const void* attribs=nullptr );
		virtual void Execute( int numchroms, IChromosome* chromosomes[], float mutate_prob, const void* attribs=nullptr );

	};


}// end of namespace


#endif // !FLIP_BIT_MUTATION_H
