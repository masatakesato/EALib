#ifndef DESIGN_PARAMETER_H
#define	DESIGN_PARAMETER_H

#include	"Typedefs.h"

#include	<oreore/common/Variant.h>
#include	<oreore/container/Array.h>
#include	<oreore/container/StaticArray.h>
#include	<oreore/container/ArrayView.h>



namespace ealib
{

	namespace SamplingType
	{
		const uint8	Enumerated	= 1;	// 自由に値を設定できる
		const uint8	Bounded		= 2;	// 一定区間毎に自由に1つ設定できる
		const uint8	Discretized	= 3;	// 一定間隔毎の離散値
	}



	namespace BoundaryType
	{
		const uint8	None		= 0;
		const uint8	Inclusive	= 1;
		const uint8	Exclusive	= 2;
	}




	class DesignParameter
	{
	public:

		DesignParameter( int16 type=int16(TYPE_UNKNOWN) )
			: m_TypeID( type )
			, m_Key( _T( "" ) )
			, m_DefaultValue( 0 )
			, m_LowerBoundary( 0 )
			, m_UpperBoundary( 0 )
			, m_Interval( 0 )
			, m_LowerBoundaryType( BoundaryType::Inclusive )
			, m_UpperBoundaryType( BoundaryType::Inclusive )
			, m_SamplingType( SamplingType::Enumerated )
		{

		}


		template < typename T >
		DesignParameter( const tstring& key, const T& defaultvalue, const T& lower, const T& upper, const T& interval,
			uint8 lowertype, uint8 uppertype, uint8 sampletype )
			: m_TypeID( TYPE_ID<T> )
			, m_Key( key )
			, m_DefaultValue( defaultvalue )
			, m_LowerBoundary( lower )
			, m_UpperBoundary( upper )
			, m_Interval( interval )
			, m_LowerBoundaryType( lowertype )
			, m_UpperBoundaryType( uppertype )
			, m_SamplingType( sampletype )
		{

		}


		template < typename T >
		DesignParameter( int16 type, const tstring& key, const T& defaultvalue, const T& lower, const T& upper, const T& interval,
			uint8 lowertype, uint8 uppertype, uint8 sampletype )
			: m_TypeID( type )
			, m_Key( key )
			, m_DefaultValue( defaultvalue )
			, m_LowerBoundary( lower )
			, m_UpperBoundary( upper )
			, m_Interval( interval )
			, m_LowerBoundaryType( lowertype )
			, m_UpperBoundaryType( uppertype )
			, m_SamplingType( sampletype )
		{

		}



		//DesignParameter( uint8 lowertype, uint8 uppertype, uint8 sampletype )
		//	: m_LowerBoundaryType( lowertype )
		//	, m_UpperBoundaryType( uppertype )
		//	, m_SamplingType( sampletype )
		//{

		//}


		~DesignParameter()
		{


		}



		void SetLowerBoundaryType( uint8 type )
		{
			m_LowerBoundaryType = type;
		}


		void SetUpperBoundaryType( uint8 type )
		{
			m_UpperBoundaryType = type;
		}


		void SetBoundaryType( uint8 lowertype, uint8 uppertype )
		{
			m_LowerBoundaryType = lowertype;
		}


		void SetSamplingType( uint8 type )
		{
			m_SamplingType  = type;
		}


		void SetType( int16 type )
		{
			m_TypeID = type;
		}


		void SetKey( const tstring& key )
		{
			m_Key	= key;
		}


		void SetKey( const TCHAR* key )
		{
			m_Key	= key;
		}


		template < typename T >
		void SetDefaultValue( const T& val )
		{
			m_TypeID = TYPE_ID<T>;
			m_DefaultValue = val;
		}


		template < typename T >
		void SetDefaultValue( const TCHAR* cval )
		{
			m_TypeID = TYPE_ID<T>;
			m_DefaultValue = TCharToNumeric<T>( cval );
		}


		template < typename T >
		void SetLowerBoundary( const T& val )
		{
			m_TypeID = TYPE_ID<T>;
			m_LowerBoundary = val;
		}


		template < typename T >
		void SetLowerBoundary( const TCHAR* cval )
		{
			m_TypeID = TYPE_ID<T>;
			m_LowerBoundary = TCharToNumeric<T>( cval );
		}


		template < typename T >
		void SetUpperBoundary( const T& val )
		{
			m_TypeID = TYPE_ID<T>;
			m_UpperBoundary = val;
		}


		template < typename T >
		void SetUpperBoundary( const TCHAR* cval )
		{
			m_TypeID = TYPE_ID<T>;
			m_UpperBoundary = TCharToNumeric<T>( cval );
		}


		template < typename T >
		void SetBoundary( const T& lower, const T& upper )
		{
			m_TypeID = TYPE_ID<T>;
			m_LowerBoundary = lower;
			m_UpperBoundary = upper;
		}


		template < typename T >
		void SetBoundary( const TCHAR* clower, const TCHAR* cupper )
		{
			m_TypeID = TYPE_ID<T>;
			m_LowerBoundary = TCharToNumeric<T>( clower );
			m_UpperBoundary = TCharToNumeric<T>( cupper );
		}


		template < typename T >
		void SetInterval( const T& val )
		{
			m_TypeID = TYPE_ID<T>;
			m_Interval = val;
		}


		template < typename T >
		void SetInterval( const TCHAR* cval )
		{
			m_TypeID = TYPE_ID<T>;
			m_Interval = TCharToNumeric<T>( cval );
		}


		void SetSequentialID( int32 seqid )
		{
			m_SequentialID = seqid;
		}



		int16 TypeID() const
		{
			return m_TypeID;
		}


		const tstring& Key() const
		{
			return m_Key;
		}


		template < typename T >
		T DefaultValue() const
		{
			return m_DefaultValue;
		}


		template < typename T >
		T LowerBoundary() const
		{
			return m_LowerBoundary;
		}


		template < typename T >
		T UpperBoundary() const
		{
			return m_UpperBoundary;
		}


		template < typename T >
		T Interval() const
		{
			return m_Interval;
		}


		int32 SequentialID() const
		{
			return m_SequentialID;
		}



	private:

		int16 m_TypeID = -1;

		tstring	m_Key;
		Variant	m_DefaultValue;
		Variant	m_LowerBoundary;
		Variant	m_UpperBoundary;
		Variant	m_Interval;

		uint8	m_LowerBoundaryType;
		uint8	m_UpperBoundaryType;
		uint8	m_SamplingType;

		int32	m_SequentialID = -1;
	};



//	using DesignParamArray =  OreOreLib::Memory<DesignParameter>;
	using DesignParamArray = OreOreLib::Array<DesignParameter>;



	//##################### helper functions(temporary implementation) #########################//

	template < typename T >
	typename std::enable_if_t< std::is_arithmetic_v<T>, void >
	SetBoundaryFromStr( DesignParameter& param, const TCHAR* str_min, const TCHAR* str_max )
	{
		param.SetBoundary<T>( str_min, str_max );
	}


	template < typename T >
	typename std::enable_if_t< std::is_same_v<T, BitArray>, void > 
	SetBoundaryFromStr( DesignParameter& param, const TCHAR* str_min, const TCHAR* str_max )
	{
		param.SetBoundary<int>( str_min, str_max );
		param.SetType( TYPE_ID<BitArray> );
	}





	inline static bool AddDesignParam( DesignParamArray& params, const DesignParameter& p )
	{
		int idx = (int)OreOreLib::FindIf( params, [&]( const DesignParameter& x ){ return x.Key()==p.Key(); } );

		if( idx>=0 )
			params[idx] = p;
		else
			params.AddToTail( p );

		return true;
	}



	inline static bool RemoveDesignParam( DesignParamArray& params, const tstring& key )
	{
		int idx = (int)OreOreLib::FindIf( params, [&]( const DesignParameter& x ){ return x.Key()==key; } );

		if( idx==-1 )
			return false;

		params.Remove( idx );

		return true;
	}



	inline static int FindDesignParameter( const DesignParamArray& params, const tstring& key )
	{
		return (int)OreOreLib::FindIf( params, [&]( const DesignParameter& x ){ return x.Key()==key; } );
	}



	template < typename T >
	inline int NumElementsByType( const DesignParamArray& params )
	{
		int typeCount = 0;
		for( const auto& p : params )
		{
			if( p.TypeID() == TYPE_ID<T> )
				typeCount++;
		}

		return typeCount;
	}



	inline int NumDesignParamTypes( const DesignParamArray& params )
	{
		int16 type = params[0].TypeID();
		int numTypes = 0;

		for( int i=0; i<params.Length(); ++i )
		{
			if( i==params.Length()-1 )
			{
				numTypes++;
				break;
			}

			int16 type_next = params[i+1].TypeID();

			if( type != type_next )
			{
				numTypes++;
				type = type_next;
			}
		}

		return numTypes;
	}



	template < typename T >
	inline static bool FilterDesignParamsByType( DesignParamArray& out, const DesignParamArray& params )
	{
		int numParams = NumElementsByType<T>( params );

		if( numParams==0 )
			return false;

		out.Init( numParams );

		auto* dest = out.begin();
		for( const auto& p : params )
		{
			if( p.TypeID() == TYPE_ID<T> )
				*dest++ = p;
		}

		return true;
	}





	template < typename T >
	inline static bool SetBoundary( DesignParamArray& params, const tstring& key, const T& lower, const T& upper )
	{
		int idx = (int)OreOreLib::FindIf( params, [&]( const DesignParameter& x ){ return x.Key()==key; } );

		if( idx==-1 )
			return false;

		//params[idx].SetBoundary( lower, upper );
		if( std::is_same_v<T, BitArray> )
		{
			params[idx].SetBoundary<int>( (int)lower, (int)upper );
			params[idx].SetType( TYPE_ID<BitArray> );
		}
		else
		{
			params[idx].SetBoundary<T>( lower, upper );
		}

		return true;
	}



	template < typename T >
	inline static bool SetBoundaryFromString( DesignParamArray& params, const tstring& key, const TCHAR* str_lower, const TCHAR* str_upper )
	{
		int idx = (int)OreOreLib::FindIf( params, [&]( const DesignParameter& x ){ return x.Key()==key; } );

		if( idx==-1 )
			return false;

		//SetBoundaryFromStr<T>(	params[idx], str_lower, str_upper );
		if( std::is_same_v<T, BitArray> )
		{
			params[idx].SetBoundary<int>( str_lower, str_upper );
			params[idx].SetType( TYPE_ID<BitArray> );
		}
		else
		{
			params[idx].SetBoundary<T>( str_lower, str_upper );
		}

		return true;
	}










	//class DesignParameters
	//{
	//public:

	//	// Constructor
	//	DesignParameters();

	//	// Destructor
	//	~DesignParameters();


	//	void Add( const DesignParameter& param );
	//	void Remove( const tstring& key );
	//	void Clear();

	//	void Display();



	// operatorでキーアクセスできるようにする
	// operatorでインデックスアクセスできるようにする

	//private:

	//	OreOreLib::Memory<DesignParameter>	m_Params;



	//};




}// end of namespace



#endif // !DESIGN_PARAMETER_H
