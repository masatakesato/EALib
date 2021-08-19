#ifndef RANDOM_INITIALIZER_H
#define	RANDOM_INITIALIZER_H


//#include	<oreore/memory/SharedPtr.h>


#include	"Initializer.h"




namespace ealib
{
	class IChromosome;

	class CLASS_DECLSPEC RandomInitializer : public Initializer
	{
	public:

		RandomInitializer();
		virtual ~RandomInitializer();


	private:

		template < typename TList >	struct BuildOperators;


		using Initializer::BindOperator;
		using Initializer::UnbindOperator;

	};


}// end of namespace



#endif // !INITIALIZER_H