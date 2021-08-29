#include	"DataIO.h"

#include	<sstream>

#include	<oreore/container/ArrayView.h>
//#include	<oreore/container/HashMap.h>

#include	"ChromosomeFactory.h"



namespace ealib
{

	//##################################################################################################//
	//																									//
	//										Helper functions											//
	//																									//
	//##################################################################################################//


	const ChromosomeFactory<g_ChoromosomeTypes> c_ChromosomeFactory;



	template < typename T >
	typename std::enable_if_t< std::is_arithmetic_v<T>, void >
	InitDesignParamFromString(	DesignParameter* pDesignParam,
								const TCHAR* key,
								const TCHAR* str_min, const TCHAR* str_max,
								const TCHAR* str_def )
	{
		pDesignParam->SetKey( key );
		pDesignParam->SetBoundary<T>( str_min, str_max );
		pDesignParam->SetDefaultValue<T>( str_def );
	}


	template < typename T >
	typename std::enable_if_t< std::is_same_v<T, BitArray>, void > 
	InitDesignParamFromString(	DesignParameter* pDesignParam, 
								const TCHAR* key,
								const TCHAR* str_min, const TCHAR* str_max,
								const TCHAR* str_def )
	{
		pDesignParam->SetKey( key );
		pDesignParam->SetBoundary<int>( str_min, str_max );
		pDesignParam->SetDefaultValue<int>( str_def );
		pDesignParam->SetType( TYPE_ID<BitArray> );
	}


	void (* DataIO::c_InitDesignParamFuncs[ NUM_TYPES ] )( DesignParameter*, const TCHAR*, const TCHAR*, const TCHAR*, const TCHAR* )
	{
		InitDesignParamFromString<bool>,
		InitDesignParamFromString<uint8>,
		InitDesignParamFromString<int16>,
		InitDesignParamFromString<uint16>,
		InitDesignParamFromString<int32>,
		InitDesignParamFromString<uint32>,
		InitDesignParamFromString<int64>,
		InitDesignParamFromString<uint64>,
		InitDesignParamFromString<float32>,
		InitDesignParamFromString<float64>,
		InitDesignParamFromString<BitArray>,
	};



	tstring (* DataIO::c_NumericToTString[ NUM_TYPES ] )( const void* )
	{
		NumericToTString<bool>,
		NumericToTString<uint8>,
		NumericToTString<int16>,
		NumericToTString<uint16>,
		NumericToTString<int32>,
		NumericToTString<uint32>,
		NumericToTString<int64>,
		NumericToTString<uint64>,
		NumericToTString<float32>,
		NumericToTString<float64>,
		[]( const void* val )->tstring { return ((BitArray*)val)->ToTString(); }
	};




	//const OreOreLib::HashMap< int16, tstring, 16 > c_TypeStrDict =
	//{
	//	{ TYPE_ID<bool>,		_T("bool") },
	//	{ TYPE_ID<uint8>,		_T("byte") },
	//	{ TYPE_ID<int16>,		_T("short") },
	//	{ TYPE_ID<uint16>,		_T("unsigned short") },
	//	{ TYPE_ID<int32>,		_T("int") },
	//	{ TYPE_ID<uint32>,		_T("unsigned int") },
	//	{ TYPE_ID<int64>,		_T("long") },
	//	{ TYPE_ID<uint64>,		_T("unsigned long") },
	//	{ TYPE_ID<float32>,		_T("float") },
	//	{ TYPE_ID<float64>,		_T("double") },
	//	{ TYPE_ID<BitArray>,	_T("binary string") },
	//};





	//##################################################################################################//
	//																									//
	//									DataIO class private methods									//
	//																									//
	//##################################################################################################//


	DataIO::ParamSet DataIO::SplitString( tstring& input, TCHAR delimiter )
	{
		tistringstream stream( input );
		tstring field;
		ParamSet result;

		while( getline( stream, field, delimiter ) )
		{
			result.AddToTail( field );
		}

		return result;
	}



	int DataIO::CountType( const ParamSetArray& params, int16 type )
	{
		int typeCount = 0;
		for( const auto& data : params )
		{
			if( type == g_TypeInfoDict.At( data[1] ) )
				typeCount++;
		}

		return typeCount;
	}



	tstring DataIO::CreateSnapshotString( const Population* population )
	{
		IChromosome *chromosome = population->GetIndividual( 0 );

		tstring names( _T( "#" ) );
		tstring param_set;


		//============== Create param names part ================//
		for( int i=0; i<chromosome->Size(); ++i )
		{
			tstring key = chromosome->GetDesignParameter( i )->Key();

			if( key.empty() )
				key = tstring( _T( "param_" ) ) + to_tstring( i );

			names += key;

			names += ( i==chromosome->Size()-1 ) ? _T( "\n" ) : _T( "," );
		}


		//============ Create param sets part =================//
		for( int i=0; i<population->PopulationSize(); ++i )
		{
			chromosome = population->GetIndividual( i );

			for( int j=0; j<chromosome->Size(); ++j )
			{

				int16 type = chromosome->GetDesignParameter( j )->TypeID();

				if( type!= TYPE_UNKNOWN )
					param_set += c_NumericToTString[ type ]( chromosome->GetGene( j ) );

				if( j<chromosome->Size()-1 )
					param_set += _T( "," );

				/*param_set += ( j==chromosome->Size()-1 ) ? _T( "\n" ) : _T( "," );*/

			}// end of j loop

			if( i<population->PopulationSize()-1 )
				param_set +=  _T( "\n" );

		}// end of i loop

		return names + param_set;
	}




	//##################################################################################################//
	//																									//
	//									DataIO class public methods										//
	//																									//
	//##################################################################################################//



	bool DataIO::LoadCSV( const tstring& filepath, ParamSetArray& lines )
	{
		tifstream ifs( filepath );
		if( !ifs.is_open() )
			return false;

		tstring line;
		while( getline( ifs, line ) )
		{
			// ignore line starting with "#"
			if( line.find(_T("#")) == 0 )
				continue;

			auto line_elms = SplitString( line, ',' );

			// ignore missing data
			if( line_elms.Length() != 5 )
				continue;	

			// ignore unknown type data
			if( !g_TypeInfoDict.Exists( line_elms[1] ) )
				continue;

			lines.AddToTail( line_elms );
		}


		return !lines.Empty();

	}



	IChromosome* DataIO::LoadChromosome( const tstring& filepath )
	{

		IChromosome* pChromosome = nullptr;

		//====================== Load CSV file ======================//
		ParamSetArray params;
		if( !LoadCSV( filepath, params ) )
			return nullptr;


		//==================== Create DesignParameters ===================//
		DesignParamArray designParams( params.Length() );

		for( int i=0; i<params.Length(); ++i )
		{
			OreOreLib::ArrayView<tstring> data( params[i] );
			
			int16 type = g_TypeInfoDict.Exists( data[1] ) ? g_TypeInfoDict.At( data[1] ) : TYPE_UNKNOWN;
			if( type == TYPE_UNKNOWN )	continue;

			designParams[i].SetType( type );
			designParams[i].SetSamplingType( SamplingType::Enumerated );
			designParams[i].SetBoundaryType( BoundaryType::Inclusive, BoundaryType::Inclusive );
			designParams[i].SetKey( data[0] );

			// Initialize arithmetic parameters
			c_InitDesignParamFuncs[ type ]
			(
				&designParams[i],
				data[0].c_str(),// key
				data[2].c_str(), data[3].c_str(), data[4].c_str()// min, max, default_value
			);
		}

		
		//==================== Create Chromsomes ===================//
		return c_ChromosomeFactory.Create( designParams );
	}



	bool DataIO::LoadChromosome2D( Chromosome2D& chromosome, const ParamSetArray& params )
	{
		//==================== Create DesignParameters ===================//
		DesignParamArray designParams( params.Length() );
		int typecounts[ NUM_TYPES ] = { 0 };

		for( int i=0; i<params.Length(); ++i )
		{
			OreOreLib::ArrayView<tstring> data( params[i] );

			int16 type = g_TypeInfoDict.Exists( data[1] ) ? g_TypeInfoDict.At( data[1] ) : TYPE_UNKNOWN;
			if( type == TYPE_UNKNOWN )	continue;

			designParams[i].SetType( type );
			designParams[i].SetSamplingType( SamplingType::Enumerated );
			designParams[i].SetBoundaryType( BoundaryType::Inclusive, BoundaryType::Inclusive );
			designParams[i].SetKey( data[0] );

			c_InitDesignParamFuncs[ type ]
			(
				&designParams[i],
				data[0].c_str(),
				data[2].c_str(), data[3].c_str(), data[4].c_str()
			);

			typecounts[ type ]++;
		}


		//==================== Init Chromsomes ===================//
		int numtypes = 0;
		for( int i=0; i<NUM_TYPES; ++i )
			numtypes += int(typecounts[i]>0);

		if( numtypes == 0 )
			return false;

		// Allocate
		chromosome.Init( designParams );


		return true;
	}



	bool DataIO::ExportSnapshot( const Population* population, const tstring& filepath )
	{
		tofstream stream( filepath );

		if( !stream.bad() )
		{
			stream << CreateSnapshotString( population ) << std::endl;
			stream.close();
		}
		
		return true;
	}



	bool DataIO::ExportSnapshots( int num, const Population populations[], const tstring& filepath )
	{

		tofstream stream( filepath );

		if( !stream.bad() )
		{
			for( int i=0; i<num; ++i )
			{
				stream <<  CreateSnapshotString( &populations[i] ) << std::endl;
			}
			stream.close();
		}

		return true;
	}




}// end of namespace