#ifndef SPX_H
#define	SPX_H

#include	"ICrossoverOperator.h"



namespace ealib
{

	// Simplex Crossover
	class CLASS_DECLSPEC SPX : public ICrossoverOperator
	{
	public:

		SPX( const DesignParamArray& designParams );
		~SPX();

		void Init( const DesignParamArray& designParams );
		void Release();

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );


	private:

		double							m_Mu;
		OreOreLib::Array<IChromosome*>	m_Vertices;
		IChromosome*					m_CenterOfMass;


	};



}// end of namespace



#endif // !SPX_H
