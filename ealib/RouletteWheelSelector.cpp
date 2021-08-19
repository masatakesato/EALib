#include	"RouletteWheelSelector.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MersenneTwister.h>
#include	"IChromosome.h"
#include	"Population.h"



namespace ealib
{

	RouletteWheelSelector::RouletteWheelSelector() : ISelector()
	{
		m_PrefixSum = nullptr;
		m_MinValue	= 0.0f;
	}



	RouletteWheelSelector::~RouletteWheelSelector()
	{
		UnbindPopulationData();
	}



	void RouletteWheelSelector::BindPopulationData( int numchroms, IChromosome **pchroms )
	{
		if( m_NumChroms != numchroms )
		{
			SafeDeleteArray( m_PrefixSum );

			m_NumChroms	= numchroms;
			m_PrefixSum	= new float[m_NumChroms];
		}

		m_refChromosomes	= pchroms;
	}
	

	
	void RouletteWheelSelector::UnbindPopulationData()
	{
		SafeDeleteArray( m_PrefixSum );
		m_MinValue	= 0.0f;

		m_NumChroms			= 0;
		m_refChromosomes	= nullptr;
	}



	void RouletteWheelSelector::Update()
	{
		if( !m_refChromosomes || !m_PrefixSum )
			return;

		// 適応度の最小値を取得する.
		m_MinValue = m_refChromosomes[0]->GetFitness();
		for( int i=0; i<m_NumChroms; ++i )
		{
			float fitness = m_refChromosomes[i]->GetFitness();
			if( fitness < m_MinValue )	m_MinValue	= fitness;
		}

		// 親個体の累積選択率を計算する. 負値を避けるため、適応度の最小値を使って底上げする
		for( int i=0; i<m_NumChroms; ++i )
		{
			float prob = m_refChromosomes[i]->GetFitness() - m_MinValue + (float)EPSILON_E5;

			// 個体の評価値を累積して適応値を算出する(ルーレット選択の場合だけ必要)
			m_PrefixSum[i]	= prob + (i>0 ? m_PrefixSum[i-1]: 0.0f);
		}// end of i loop

	}



	int RouletteWheelSelector::Execute()
	{
		if( !m_refChromosomes )
			return -1;
		
		float randVal	= float( OreOreLib::genrand_real1() ) * m_PrefixSum[m_NumChroms-1];//pChromosomes[arraySize-1]->GetFitness();

		for( int i=0; i<m_NumChroms; ++i )
		{
			if( randVal <= m_PrefixSum[i] )
				return i;
		}

		return -1;
	}


}// end of namespace