#ifndef DE_BEST_2_H
#define	DE_BEST_2_H


#include	"IDEMutationStrategy.h"



namespace ealib
{

	class CLASS_DECLSPEC DE_Best_2 : public IDEMutationStrategy
	{
	public:

		DE_Best_2();
		virtual ~DE_Best_2();

		// Virtual Functions Override
		virtual void Execute( int num, IChromosome** selections, int current );


	private:
		

	};



}// end of namespace



#endif // !DE_BEST_2_H
