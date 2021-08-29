#ifndef	CHROMOSOME_1D_H
#define	CHROMOSOME_1D_H

#include	"IChromosome.h"

//#include	<unordered_map>

#include	<oreore/common/TString.h>
#include	<oreore/container/HashMap.h>

#include	"Initializer.h"
#include	"DesignParameter.h"



namespace ealib
{
	
	template < typename T >
	class /*CLASS_DECLSPEC*/ Chromosome1D : public IChromosome
	{
	public:


		Chromosome1D()
			: IChromosome()
			, m_TypeInfo( -1 )
		{

		}


		Chromosome1D( const DesignParamArray& parameters )
			: IChromosome()
		{
			// Init m_DesignParamerers
			m_DesignParameters = parameters;

			// Init Chromosome1D members
			m_TypeInfo = (*parameters.begin()).TypeID();
			m_GeneArray.Init( m_DesignParameters.Length() );

			// Build key map
			int i=0;
			for( const auto& param : m_DesignParameters )
			{
				tstring key	= param.Key();
				if( key.length()>0 )
					m_KeyMap[ key ] = i++;//(*m_pKeyMap)[key] = i++;//
			}
		}


		Chromosome1D( const Chromosome1D& obj )
			: IChromosome( obj )
		{
			m_TypeInfo		= obj.m_TypeInfo;
			m_KeyMap		= obj.m_KeyMap;//m_pKeyMap		= std::make_unique<std::unordered_map<tstring, int>>( *obj.m_pKeyMap );//

			if( !m_DesignParameters.Empty() )
				m_GeneArray = obj.m_GeneArray;
		}


		virtual ~Chromosome1D()
		{
			Release();
		}


		Chromosome1D &operator=( const Chromosome1D& obj )
		{
			Release();
		
			m_CurrentEval	= obj.m_CurrentEval;
			m_pResult		= nullptr;

			m_TypeInfo		= obj.m_TypeInfo;
			m_KeyMap		= obj.m_KeyMap;//m_pKeyMap		= std::make_unique<std::unordered_map<tstring, int>>( *obj.m_pKeyMap );//
		
			if( !obj.m_DesignParameters.Empty() )
			{
				m_DesignParameters	= obj.m_DesignParameters;
				m_GeneArray			= obj.m_GeneArray;
			}

			return *this;
		}

		
		virtual void Init( const DesignParamArray& parameters )
		{
			// Init IChromosome members
			IChromosome::Init( parameters );

			// Init Chromosome1D members
			m_TypeInfo		= (*parameters.begin()).TypeID();
			m_GeneArray.Init( m_DesignParameters.Length() );

			// Build key map
			int i=0;
			m_KeyMap.Clear();
			for( const auto& param : m_DesignParameters )
			{
				tstring key	= param.Key();
				if( key.length()>0 )
					m_KeyMap[key] = i++;//(*m_pKeyMap)[key] = i++;//
			}

		}


		virtual void Release()
		{
			// Release IChromosome members
			IChromosome::Release();
		
			// Release Chromsome1D members
			m_TypeInfo		= TYPE_UNKNOWN;
			m_GeneArray.Release();
			m_KeyMap.Clear();//m_pKeyMap->clear();//
		}


		bool SetKey( int i, const tstring& newkey )
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


		bool SetKey( const tstring& currkey, const tstring& newkey )
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


		
		T* GeneAs( int i=0 ) /*const*/			{ return (T*)&m_GeneArray[ i ]; }

		T* GeneAs( const tstring& key ) /*const*/	{ return m_GeneArray.begin() + m_KeyMap.At( key ); }//{ return (T*)&m_GeneArray[ m_KeyMap.At( key ) ]; }
	
		// Virtual Functions Override
		using IChromosome::GetDesignParameter;
		virtual DesignParameter* GetDesignParameter( const tstring& key ) const	{ return (DesignParameter*)&m_DesignParameters[ m_KeyMap.At( key ) ]; }

		// Pure Virtual Functions Override
		virtual IChromosome* GetChromosome( int i=0 ) const { return (IChromosome*)this; }
		virtual IChromosome* GetChromosomeByType( int16 type ) const { return type==m_TypeInfo ? (IChromosome*)this : nullptr; }
		virtual int NumChromosomeTypes() const		{ return 1; }
		virtual int16 TypeInfo() const				{ return m_TypeInfo; }
		virtual int Size() const					{ return m_DesignParameters.Length(); }
		virtual void* GetGene( int i=0 ) const		{ return (void*)&m_GeneArray[ i ]; }
		virtual void* GetGene( tstring key ) const	{ return (void*)&m_GeneArray[ m_KeyMap.At(key) ]; }
		virtual IChromosome* Clone() const			{ return new Chromosome1D( *this ); }


		virtual void CopyGeneFrom( const IChromosome* pSrc )
		{
			m_GeneArray.CopyFrom( ((Chromosome1D*)pSrc)->m_GeneArray );
			if( m_pResult )	m_pResult->CopyFrom( pSrc->EvalResult() );
		}

		virtual void ClearGene()
		{
			m_GeneArray.Clear();
		}



	private:

		int16									m_TypeInfo;
		OreOreLib::Array<T>						m_GeneArray;
		OreOreLib::HashMap< tstring, int, 64 >	m_KeyMap;
//		std::unique_ptr<std::unordered_map<tstring, int>>	m_pKeyMap;//std::unordered_map<tstring, int>	m_KeyMap;
		// m_KeyMapは名前重複があった場合の動作は保証しない. DesignParameterでKey(名前)を設定しない場合は、キー検索未登録扱いにする
	};


}// end of namespace


#endif	// CHROMOSOME_1D_H //