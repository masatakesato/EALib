#include	"Chromosome1D.h"

#include	"Initializer.h"


// TODO: Chromosome1DがBitArrayやString等のオブジェクト配列を扱えるようにする. 2021.08.16



namespace ealib
{


	Chromosome1D::Chromosome1D( const DesignParamArray& parameters )
		: IChromosome()
		, m_ElementSize( 0 )
	{
		int i=0;
		m_DesignParameters = parameters;
		for( const auto& param : m_DesignParameters )
		{
			tstring key	= param.Key();
			if( key.length()>0 )
				m_KeyMap[ key ] = i++;//(*m_pKeyMap)[key] = i++;//
		}

		m_TypeInfo = (*parameters.begin()).TypeID();
		m_ElementSize = GetElementSize( m_TypeInfo );

		// Allocate buffer
		m_GeneArray.Init( m_DesignParameters.Length() * int(m_ElementSize) );

	}



	Chromosome1D::Chromosome1D()
		: IChromosome()
		, m_TypeInfo( -1 )
		, m_ElementSize( 0 )
	{

	}



	Chromosome1D::Chromosome1D( const Chromosome1D& obj )
		: IChromosome( obj )
	{
		m_TypeInfo		= obj.m_TypeInfo;
		m_ElementSize	= obj.m_ElementSize;
		m_KeyMap		= obj.m_KeyMap;//m_pKeyMap		= std::make_unique<std::unordered_map<tstring, int>>( *obj.m_pKeyMap );//

		if( !m_DesignParameters.Empty() )
			m_GeneArray = obj.m_GeneArray;

	}



	Chromosome1D::~Chromosome1D()
	{
		Release();
	}



	Chromosome1D &Chromosome1D::operator=( const Chromosome1D& obj )
	{
		Release();
		
		m_CurrentEval	= obj.m_CurrentEval;
		m_pResult		= nullptr;

		m_TypeInfo		= obj.m_TypeInfo;
		m_ElementSize	= obj.m_ElementSize;
		m_KeyMap		= obj.m_KeyMap;//m_pKeyMap		= std::make_unique<std::unordered_map<tstring, int>>( *obj.m_pKeyMap );//
		
		if( !obj.m_DesignParameters.Empty() )
		{
			m_DesignParameters	= obj.m_DesignParameters;
			m_GeneArray			= obj.m_GeneArray;
		}

		return *this;
	}



	void Chromosome1D::Init( const DesignParamArray& parameters )
	{
		// Init IChromosome members
		IChromosome::Init( parameters );

		// Init Chromosome1D members
		m_TypeInfo		= (*parameters.begin()).TypeID();
		m_ElementSize	= GetElementSize( m_TypeInfo );
		m_GeneArray.Init( m_DesignParameters.Length() * int(m_ElementSize) );

		int i=0;
		m_KeyMap.Clear();
		for( const auto& param : m_DesignParameters )
		{
			tstring key	= param.Key();
			if( key.length()>0 )
				m_KeyMap[key] = i++;//(*m_pKeyMap)[key] = i++;//
		}

	}



	void Chromosome1D::Release()
	{
		// Release IChromosome members
		IChromosome::Release();
		
		// Release Chromsome1D members
		m_TypeInfo		= TYPE_UNKNOWN;
		m_ElementSize	= 0;
		m_GeneArray.Release();
		m_KeyMap.Clear();//m_pKeyMap->clear();//
	}


	
	bool Chromosome1D::SetKey( int i, const tstring& newkey )
	{
		// newkeyが予約済みの場合はキー設定を中止する
		if( m_KeyMap.Exists( newkey ) )
			return false;

		// 現行keyがm_KeyMapに存在する場合は削除する
		m_KeyMap.Remove( m_DesignParameters[i].Key() );

		// { newkey, i }をm_KeyMapに登録する
		m_KeyMap.Put( newkey, i );

		// 対立遺伝子のキー値を更新する
		m_DesignParameters[i].SetKey( newkey );

		return true;
	}

	//bool Chromosome1D::SetKey( int i, const tstring& newkey )
	//{
	//	// newkeyが予約済みの場合はキー設定を中止する
	//	if( m_pKeyMap->find( newkey ) != m_pKeyMap->end() )//if( m_KeyMap.find( newkey ) != m_KeyMap.end() )
	//		return false;

	//	// 現行keyがm_KeyMapに存在する場合は削除する
	//	const tstring& currkey = m_DesignParameters[i].Key();
	//	if( m_pKeyMap->find( currkey ) != m_pKeyMap->end() )//if( m_KeyMap.find( currkey ) != m_KeyMap.end() )
	//		m_pKeyMap->erase( currkey );//m_KeyMap.erase( currkey );

	//	// { newkey, i }をm_KeyMapに登録する
	//	m_pKeyMap->insert( { newkey, i } );//m_KeyMap.insert( { newkey, i } );

	//	// 対立遺伝子のキー値を更新する
	//	m_DesignParameters[i].SetKey( newkey );

	//	return true;
	//}



	bool Chromosome1D::SetKey( const tstring& currkey, const tstring& newkey )
	{
		// currkeyがm_KeyMapに登録されていない場合はキー設定を中止する
		if( !m_KeyMap.Exists( currkey ) )
			return false;

		// newkeyが予約済みの場合もキー設定を中止する
		if( m_KeyMap.Exists( newkey ) )
			return false;

		// m_KeyMapの値を退避する
		auto val = m_KeyMap.At( currkey );

		// 現行keyをm_KeyMapから削除する
		m_KeyMap.Remove( currkey );

		// { newkey, i }をm_KeyMapに登録する
		m_KeyMap.Put( newkey, val );

		// 対立遺伝子のキー値を更新する
		m_DesignParameters[ val ].SetKey( newkey );

		return true;
	}

	//bool Chromosome1D::SetKey( const tstring& currkey, const tstring& newkey )
	//{
	//	// currkeyがm_KeyMapに登録されていない場合はキー設定を中止する
	//	if( m_pKeyMap->find( currkey ) == m_pKeyMap->end() )//if( m_KeyMap.find( currkey ) == m_KeyMap.end() )
	//		return false;

	//	// newkeyが予約済みの場合もキー設定を中止する
	//	if( m_pKeyMap->find( newkey ) != m_pKeyMap->end() )//if( m_KeyMap.find( newkey ) != m_KeyMap.end() )
	//		return false;

	//	// m_KeyMapの値を退避する
	//	auto val = m_pKeyMap->at( currkey );//auto val = m_KeyMap.at( currkey );

	//	// 現行keyをm_KeyMapから削除する
	//	m_pKeyMap->erase( currkey );//m_KeyMap.erase( currkey );

	//	// { newkey, i }をm_KeyMapに登録する
	//	m_pKeyMap->insert( { newkey, val } );//m_KeyMap.insert( { newkey, val } );

	//	// 対立遺伝子のキー値を更新する
	//	m_DesignParameters[val].SetKey( newkey );

	//	return true;
	//}



	IChromosome* Chromosome1D::Clone() const
	{
		return new Chromosome1D( *this );
	}



	void Chromosome1D::CopyGeneFrom( const IChromosome* pSrc )
	{
		m_GeneArray.CopyFrom( ((Chromosome1D*)pSrc)->m_GeneArray );

		if( m_pResult )	m_pResult->CopyFrom( pSrc->GetEvalResult() );
	}



	void Chromosome1D::ClearGene()
	{
		m_GeneArray.Clear();
	}



}// end of namespace