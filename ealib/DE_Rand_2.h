#ifndef DE_RAND_2_H
#define	DE_RAND_2_H


#include	"IDEMutationStrategy.h"



namespace ealib
{

	class CLASS_DECLSPEC DE_Rand_2 : public IDEMutationStrategy
	{
	public:

		DE_Rand_2();
		virtual ~DE_Rand_2();
		
		// Virtual Functions Override
		virtual void Execute( OreOreLib::Memory<const IChromosome*>& selections, int current );

	};



}// end of namespace



#endif // !DE_RAND_2_H
