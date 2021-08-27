#ifndef SPX_H
#define	SPX_H

#include	"ICrossoverOperator.h"

// Simplex Crossover for Real-coded Genetic Algolithms
// https://www.jstage.jst.go.jp/article/tjsai/16/1/16_1_147/_pdf


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


virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs );


		virtual void Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs );


	private:

		double							m_Epsilon;

		IChromosome*					m_G;
		OreOreLib::Array<float>			m_rs;
		OreOreLib::Array<IChromosome*>	m_xs;
		OreOreLib::Array<IChromosome*>	m_Cs;


	};



}// end of namespace



#endif // !SPX_H
