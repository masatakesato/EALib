#include	"Chromosome2D.h"

#include	<algorithm>

#include	<oreore/container/ArrayView.h>

#include	"Chromosome1DFactory.h"



namespace ealib
{

	const Chromosome1DFactory<g_ChoromosomeTypes>	c_Chrom1DFactory;


	Chromosome2D::Chromosome2D()
		: IChromosome()
		//, m_NumChromTypes( 0 )
		//, m_ChromosomeArray{}
		//, m_pKeyMap( std::make_unique< std::unordered_map<tstring, Index2D> >() )
		//, m_pIndexMap( std::make_unique< std::unordered_map<int, Index2D> >() )
	{
		ClearTypeToIndex();
	}



	Chromosome2D::Chromosome2D( const DesignParamArray& designParams )
		: IChromosome()
		//, m_NumChromTypes( 0 )
		//, m_ChromosomeArray{}
		//, m_pKeyMap( std::make_unique< std::unordered_map<tstring, Index2D> >() )
		//, m_pIndexMap( std::make_unique< std::unordered_map<int, Index2D> >() )
	{
		Init( designParams );
	}
	


	Chromosome2D::Chromosome2D( const Chromosome2D& obj )
	{
		if( !obj.m_DesignParameters.Empty() )
			m_DesignParameters = obj.m_DesignParameters;

		m_CurrentEval	= obj.m_CurrentEval;
		m_pResult		= nullptr;

		//m_NumChromTypes	= 0;
		//memset( m_ChromosomeArray, 0, sizeof(IChromosome*) * NUM_TYPES );//	= nullptr;
		ClearTypeToIndex();

		//if( obj.m_NumChromTypes > 0 )
		DeepCopyChromosomeArray( obj );
	}



	Chromosome2D::~Chromosome2D()
	{
		Release();
	}



	Chromosome2D &Chromosome2D::operator=( const Chromosome2D& obj )
	{
		Release();
		
		if( !obj.m_DesignParameters.Empty() )
			m_DesignParameters = obj.m_DesignParameters;

		m_CurrentEval	= obj.m_CurrentEval;
		m_pResult		= nullptr;

		//m_NumChromTypes		= 0;
		//memset( m_ChromosomeArray, 0, sizeof(IChromosome*) * NUM_TYPES );//	= nullptr;
		//if( obj.m_NumChromTypes > 0 )
		DeepCopyChromosomeArray( obj );

		return *this;
	}



	void Chromosome2D::Init( const DesignParamArray& designParams )
	{
		// Init IChromosome members
		IChromosome::Init( designParams );

		// Init Chromosome2D members
		DeepRemoveChromosomeArray();

		int	numParamsPerType[ NUM_TYPES ] = { 0 };

		// Preparation for Chromosome1D array generation 
		for( int i=0; i<m_DesignParameters.Length(); ++i )
		{
			// Assign SequentialID to each design parameter ( for accessing chromosome/degisnparam by index )
			m_DesignParameters[i].SetSequentialID( i );

			// Accumulate num of parameters for each type
			int16 type = m_DesignParameters[i].TypeID();
			if( type >=0 && type <NUM_TYPES )	numParamsPerType[ type ]++;
		}

		// Sort m_DesignParameters by type
		std::sort( m_DesignParameters.begin(), m_DesignParameters.end(), []( const DesignParameter& a, const DesignParameter& b ){ return ( a.TypeID() < b.TypeID() ); } );


		// Initialize Type-to-ChromosomeArray Index convertion table
		int chromTypeCount = 0;

		ClearTypeToIndex();
		int16 type = m_DesignParameters[0].TypeID();

		for( int i=0; i<m_DesignParameters.Length(); ++i )
		{
			if( i==m_DesignParameters.Length()-1 )
			{
				m_TypeToIndex[ type ] = chromTypeCount++;
				break;
			}

			int16 type_next = m_DesignParameters[i+1].TypeID();

			if( type != type_next )
			{
				m_TypeToIndex[ type ] = chromTypeCount++;
				type = type_next;
			}
		}


		// 最初に、"遺伝子の型"が何種類あるか、また各種類何個ずつ含まれるか調べる
		//m_NumChromTypes	= 0;

//		for( int type=0; type<NUM_TYPES; ++type )
//		{
//			numParamsPerType[type]	= (int)std::count_if( m_DesignParameters.begin(), m_DesignParameters.end(), [&]( const DesignParameter& a ){ return a.TypeID()==type; } );

//			if( numParamsPerType[type] > 0 )
//				m_ActiveTypes[ m_NumChromTypes++ ] = type;

//		}// end of type loop


		// Allocate ChromosomeArray
		m_ChromosomeArray.Init( chromTypeCount );



		int paramStartIdx = 0;
		for( int i=0; i<chromTypeCount; ++i )
		{
			int type = m_DesignParameters[ paramStartIdx ].TypeID();
			int numParams	= numParamsPerType[ type ];

			// Create Chromosome1D
			OreOreLib::ArrayView<DesignParameter> partialParams( &m_DesignParameters[ paramStartIdx ], numParams );
			m_ChromosomeArray[i] =	c_Chrom1DFactory.Create( partialParams );

			// Register individual designparameter/gene index for Key/Index search
			for( int j=0; j<m_ChromosomeArray[i]->Size(); ++j )
			{
				const DesignParameter *pDParam = &m_DesignParameters[ paramStartIdx + j ];
				Index2D targetIndex( i, j );
				tstring key = pDParam->Key();
				if( key.length()>0 )	m_KeyMap[key] = targetIndex;//(*m_pKeyMap)[key] = targetIndex;//
				m_IndexMap[ pDParam->SequentialID() ] = targetIndex;//(*m_pIndexMap)[ pDParam->SequentialID() ] = targetIndex;//
			}
			paramStartIdx += numParams;

		}// end of i loop
		
	}



	void Chromosome2D::Release()
	{
		// Release IChromosome members
		IChromosome::Release();

		// Release Chromosome2D members
		DeepRemoveChromosomeArray();
	}



	bool Chromosome2D::SetKey( int i, const tstring& newkey )
	{
		// newkeyが予約済みの場合はキー設定を中止する
		if( m_KeyMap.Exists( newkey ) )
			return false;

		Index2D index = m_IndexMap.At( i );//m_pIndexMap->at( i );//

		// 現行keyがm_KeyMapに存在する場合は削除する
		const tstring& currkey = m_ChromosomeArray[ index.first ]->GetDesignParameter( index.second )->Key();
		m_KeyMap.Remove( currkey );

		// { newkey, i }をm_KeyMapに登録する
		m_KeyMap.Put( newkey, index );

		// 対立遺伝子のキー値を更新する
		m_ChromosomeArray[ index.first ]->GetDesignParameter( index.second )->SetKey( newkey );

		return true;
	}


	//bool Chromosome2D::SetKey( int i, const tstring& newkey )
	//{
	//	// newkeyが予約済みの場合はキー設定を中止する
	//	if( m_pKeyMap->find( newkey ) != m_pKeyMap->end() )//if( m_KeyMap.find( newkey ) != m_KeyMap.end() )
	//		return false;

	//	Index2D index = m_pIndexMap->at( i );//auto val = m_IndexMap.at( i );

	//	// 現行keyがm_KeyMapに存在する場合は削除する
	//	const tstring& currkey = m_ChromosomeArray[ index.first ]->GetDesignParameter( index.second )->Key();
	//	if( m_pKeyMap->find( currkey ) != m_pKeyMap->end() )//if( m_KeyMap.find( currkey ) != m_KeyMap.end() )
	//		m_pKeyMap->erase( currkey );//m_KeyMap.erase( currkey );

	//	// { newkey, i }をm_KeyMapに登録する
	//	m_pKeyMap->insert( { newkey, index } );//m_KeyMap.insert( { newkey, index } );

	//	// 対立遺伝子のキー値を更新する
	//	m_ChromosomeArray[ index.first ]->GetDesignParameter( index.second )->SetKey( newkey );

	//	return true;
	//}



	bool Chromosome2D::SetKey( const tstring& currkey, const tstring& newkey )
	{
		// currkeyがm_KeyMapに登録されていない場合はキー設定を中止する
		if( !m_KeyMap.Exists( currkey ) )
			return false;

		// newkeyが予約済みの場合もキー設定を中止する
		if( m_KeyMap.Exists( newkey ) )
			return false;

		// m_KeyMapの値を退避する
		Index2D index = m_KeyMap.At( currkey );

		// 現行keyをm_KeyMapから削除する
		m_KeyMap.Remove( currkey );

		// { newkey, i }をm_KeyMapに登録する
		m_KeyMap.Put( newkey, index );

		// 対立遺伝子のキー値を更新する
		m_ChromosomeArray[ index.first ]->GetDesignParameter( index.second )->SetKey( newkey );

		return true;
	}

	//bool Chromosome2D::SetKey( const tstring& currkey, const tstring& newkey )
	//{
	//	// currkeyがm_KeyMapに登録されていない場合はキー設定を中止する
	//	if( m_pKeyMap->find( currkey ) == m_pKeyMap->end() )//if( m_KeyMap.find( currkey ) == m_KeyMap.end() )
	//		return false;

	//	// newkeyが予約済みの場合もキー設定を中止する
	//	if( m_pKeyMap->find( newkey ) != m_pKeyMap->end() )//if( m_KeyMap.find( newkey ) != m_KeyMap.end() )
	//		return false;

	//	// m_KeyMapの値を退避する
	//	Index2D index = m_pKeyMap->at( currkey );//auto val = m_KeyMap.at( currkey );

	//	// 現行keyをm_KeyMapから削除する
	//	m_pKeyMap->erase( currkey );//m_KeyMap.erase( currkey );

	//	// { newkey, i }をm_KeyMapに登録する
	//	m_pKeyMap->insert( { newkey, index } );//m_KeyMap.insert( { newkey, val } );

	//	// 対立遺伝子のキー値を更新する
	//	m_ChromosomeArray[ index.first ]->GetDesignParameter( index.second )->SetKey( newkey );

	//	return true;
	//}



	IChromosome* Chromosome2D::Clone() const
	{
		return new Chromosome2D( *this );
	}



	void Chromosome2D::CopyGeneFrom( const IChromosome* pSrc )
	{
		auto psrc = (Chromosome2D*)pSrc;

		//for( int i=0; i<m_NumChromTypes; ++i )
		//{
		//	int type = psrc->m_ActiveTypes[i];
		//	m_ChromosomeArray[ type ]->CopyGeneFrom( psrc->m_ChromosomeArray[ type ] );
		//}

		for( auto* chrom : m_ChromosomeArray )//int i=0; i<m_ChromosomeArray.Length(); ++i )
		{
			int16 type = chrom->TypeInfo();//m_ChromosomeArray[i]->TypeInfo();
			chrom->CopyGeneFrom( psrc->GetChromosomeByType( type ) );//m_ChromosomeArray[i]->CopyGeneFrom( psrc->GetChromosomeByType( type ) );
		}

		
		if( m_pResult )	m_pResult->CopyFrom( psrc->m_pResult );
	}



	void Chromosome2D::ClearGene()
	{
		//for( int i=0; i<m_NumChromTypes; ++i )
		//	m_ChromosomeArray[ m_ActiveTypes[i] ]->ClearGene();
		for( auto* chrom : m_ChromosomeArray )
			chrom->ClearGene();
	}



	// Override IChromosome's Initialize()
	void Chromosome2D::Initialize( Initializer* pInit )
	{
		//for( int i=0; i<m_NumChromTypes; ++i )
		//	m_ChromosomeArray[ m_ActiveTypes[i] ]->Initialize( pInit );
		for( auto* chrom : m_ChromosomeArray )
			chrom->Initialize( pInit );
	}



	IChromosome* Chromosome2D::GetChromosome( int i ) const
	{
		return m_ChromosomeArray[i];//  m_ActiveTypes[i] );//return *( m_ChromosomeArray + m_ActiveTypes[i] );
	}



	IChromosome* Chromosome2D::GetChromosomeByType( int16 type ) const
	{
		return m_ChromosomeArray[ m_TypeToIndex[type] ];
	}



	void Chromosome2D::ClearTypeToIndex()
	{
		for( int i=0; i<NUM_TYPES; ++i )
			m_TypeToIndex[i] = -1;
	}



	void Chromosome2D::DeepCopyChromosomeArray( const Chromosome2D& src )
	{
		if( !src.m_ChromosomeArray )
			return;

		// release current allocated resources.
		DeepRemoveChromosomeArray();

		m_ChromosomeArray.Init( src.NumChromTypes() );

		for( int i=0; i<m_ChromosomeArray.Length(); ++i )
			m_ChromosomeArray[i] = src.m_ChromosomeArray[i]->Clone();

		m_KeyMap		= src.m_KeyMap;//m_pKeyMap		= std::make_unique< std::unordered_map<tstring, Index2D > >( *src.m_pKeyMap );//
		m_IndexMap		= src.m_IndexMap;//m_pIndexMap		= std::make_unique< std::unordered_map<int, Index2D > >( *src.m_pIndexMap );//
		m_TypeToIndex	= src.m_TypeToIndex;
	}



	void Chromosome2D::DeepRemoveChromosomeArray()
	{
		//if( m_ChromosomeArray )
		//{
		//	for( int i=0; i<NUM_TYPES; ++i )
		//	{
		//		if( m_ChromosomeArray[i] )
		//			SafeDelete( m_ChromosomeArray[i] );
		//	}
		//}

		for( auto* chrom: m_ChromosomeArray )
			SafeDelete( chrom );
		m_ChromosomeArray.Release();

		//m_NumChromTypes	= 0;
		ClearTypeToIndex();
		
		m_KeyMap.Clear();//m_pKeyMap->clear();//
		m_IndexMap.Clear();//m_pIndexMap->clear();//
	}



}// end of namespace