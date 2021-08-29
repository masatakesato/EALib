#ifndef I_SELECTOR_H
#define	I_SELECTOR_H

#include	<oreore/common/DLLExport.h>
#include	<oreore/container/Array.h>
#include	<oreore/container/ArrayView.h>



namespace ealib
{
	class IChromosome;


	class CLASS_DECLSPEC ISelector
	{
	public:

		ISelector();
		virtual ~ISelector();

		virtual void BindPopulationData( const OreOreLib::Array<IChromosome*>& chromosomes );
		virtual void UnbindPopulationData();

		// Virtual Functions
		virtual void Update()=0;
		virtual int Execute()=0;
		virtual void Execute( int num, int selections[], int exclude ){}


	protected:

		OreOreLib::ArrayView<IChromosome*>	m_refChromosomes;

	};



}// end of namespace



#endif // !I_SELECTOR_H
