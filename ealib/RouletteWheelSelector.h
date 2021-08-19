#ifndef ROULETTE_WHEEL_SELECTOR_H
#define	ROULETTE_WHEEL_SELECTOR_H


#include	"ISelector.h"



namespace ealib
{

	class CLASS_DECLSPEC RouletteWheelSelector : public ISelector
	{
	public:

		RouletteWheelSelector();
		virtual ~RouletteWheelSelector();

		// Virtual Functions Override 
		virtual void BindPopulationData( int numchroms, IChromosome **pchroms );
		virtual void UnbindPopulationData();

		// Virtual Functions Override
		virtual void Update();
		virtual int Execute();


	private:
		
		float	*m_PrefixSum;
		float	m_MinValue;// ゼロ以下にしないための底上げ値

	};



}// end of namespace



#endif // !ROULETTE_WHEEL_SELECTOR_H
