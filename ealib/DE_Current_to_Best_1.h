#ifndef DE_CURRENT_TO_BEST_1_H
#define	DE_CURRENT_TO_BEST_1_H


#include	"IDEMutationStrategy.h"



namespace ealib
{

	class CLASS_DECLSPEC DE_Current_to_Best_1 : public IDEMutationStrategy
	{
	public:

		DE_Current_to_Best_1();
		virtual ~DE_Current_to_Best_1();

		// Virtual Functions Override
		virtual void Execute( OreOreLib::Memory<IChromosome*> selections, int current );

	};



}// end of namespace



#endif // !DE_CURRENT_TO_BEST_1_H
