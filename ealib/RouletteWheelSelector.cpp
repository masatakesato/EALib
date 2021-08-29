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



	void RouletteWheelSelector::BindPopulationData( const OreOreLib::Array<IChromosome*>& chromosomes )
	{
		if( m_refChromosomes.Length() != chromosomes.Length() )
		{
			SafeDeleteArray( m_PrefixSum );
			m_PrefixSum	= new float[ chromosomes.Length() ];
		}

		m_refChromosomes.Init( chromosomes );
	}
	

	
	void RouletteWheelSelector::UnbindPopulationData()
	{
		SafeDeleteArray( m_PrefixSum );
		m_MinValue	= 0.0f;

		//m_NumChroms			= 0;
		m_refChromosomes.Release();//	= nullptr;
	}



	void RouletteWheelSelector::Update()
	{
		if( !m_refChromosomes || !m_PrefixSum )
			return;

		// 適応度の最小値を取得する.
		m_MinValue = m_refChromosomes[0]->Fitness();
		for( int i=0; i<m_refChromosomes.Length(); ++i )
		{
			float fitness = m_refChromosomes[i]->Fitness();
			if( fitness < m_MinValue )	m_MinValue	= fitness;
		}

		// 親個体の累積選択率を計算する. 負値を避けるため、適応度の最小値を使って底上げする
		for( int i=0; i<m_refChromosomes.Length(); ++i )
		{
			float prob = m_refChromosomes[i]->Fitness() - m_MinValue + (float)EPSILON_E5;

			// 個体の評価値を累積して適応値を算出する(ルーレット選択の場合だけ必要)
			m_PrefixSum[i]	= prob + (i>0 ? m_PrefixSum[i-1]: 0.0f);
		}// end of i loop

	}



	int RouletteWheelSelector::Execute()
	{
		if( !m_refChromosomes )
			return -1;
		
		float randVal	= float( OreOreLib::genrand_real1() ) * m_PrefixSum[ m_refChromosomes.Length() - 1 ];//pChromosomes[arraySize-1]->Fitness();

		for( int i=0; i<m_refChromosomes.Length(); ++i )
		{
			if( randVal <= m_PrefixSum[i] )
				return i;
		}

		return -1;
	}


}// end of namespace