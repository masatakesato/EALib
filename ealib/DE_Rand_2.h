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
		virtual void Execute( int num, IChromosome** selections, int current );


	private:
		

	};



}// end of namespace



#endif // !DE_RAND_2_H
