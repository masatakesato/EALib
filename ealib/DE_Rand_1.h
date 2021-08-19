#ifndef DE_RAND_1_H
#define	DE_RAND_1_H


#include	"IDEMutationStrategy.h"



namespace ealib
{

	class CLASS_DECLSPEC DE_Rand_1 : public IDEMutationStrategy
	{
	public:

		DE_Rand_1();
		virtual ~DE_Rand_1();

		// Virtual Functions Override
		virtual void Execute( int num, IChromosome** selections, int current );


	private:
		

	};



}// end of namespace



#endif // !DE_RAND_1_H
