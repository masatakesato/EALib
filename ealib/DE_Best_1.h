#ifndef DE_BEST_1_H
#define	DE_BEST_1_H


#include	"IDEMutationStrategy.h"



namespace ealib
{

	class CLASS_DECLSPEC DE_Best_1 : public IDEMutationStrategy
	{
	public:

		DE_Best_1();
		virtual ~DE_Best_1();
		
		// Virtual Functions Override
		virtual void Execute( OreOreLib::Memory<const IChromosome*>& selections, int current );

	};



}// end of namespace



#endif // !DE_BEST_1_H
