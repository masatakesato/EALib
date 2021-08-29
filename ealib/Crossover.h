#ifndef CROSSOVER_H
#define	CROSSOVER_H

#include	<oreore/common/DLLExport.h>

#include	"Typedefs.h"
#include	"ICrossoverOperator.h"



namespace ealib
{
	class IChromosome;


	class CrossoverException : public std::exception{};


	class CLASS_DECLSPEC Crossover
	{
	public:

		Crossover();
		virtual ~Crossover();

		void BindOperator( ICrossoverOperator* pOperator );
		void UnbindOperator( int type );

		void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs=nullptr );
		void Execute( OreOreLib::Memory<const IChromosome*>& parents, OreOreLib::Memory<IChromosome*>& children, const void* attribs=nullptr );

		bool GetFamilySize( int16 type, int& numparents, int& numchildren ) const;
		bool GetFamilySize( int& numparents, int& numchildren ) const;


	private:

		OreOreLib::StaticArray<ICrossoverOperator*, NUM_TYPES>	m_refOperators;

	};



}// end of namespace



#endif // !CROSSOVER_H
