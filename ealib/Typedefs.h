#ifndef TYPEDEFS_H
#define	TYPEDEFS_H

#include	<oreore/common/Utility.h>
#include	<oreore/container/BitArray.h>
#include	<oreore/container/HashMap.h>

#include	"DataSizes.h"



namespace ealib
{

	using g_ChoromosomeTypes = OreOreLib::TypeList::MakeTypeList_t<

		bool,
		uint8,
		int16,
		uint16,
		int32,
		uint32,
		int64,
		uint64,
		float32,
		float64,
		BitArray
	>;
	

	const int16 TYPE_UNKNOWN = -1;

	const int NUM_TYPES = OreOreLib::TypeList::Length< g_ChoromosomeTypes >::value;
	
	template < typename T >
	constexpr int16 TYPE_ID = OreOreLib::TypeList::IndexOf< g_ChoromosomeTypes, T >::value;

	template < int i >
	using DATA_TYPE = typename OreOreLib::TypeList::TypeAt< g_ChoromosomeTypes, i >::type;


	const DataSizes< g_ChoromosomeTypes > g_DataSizes;
	static size_t GetElementSize( int type)	{ return g_DataSizes.Size( type ); }


	//using fArray = OreOreLib::Array<float>;


	const OreOreLib::HashMap< tstring, int16/*, 16*/ > g_TypeInfoDict =// const std::unordered_map<tstring, int16>
	{
		{ _T("bool"),			TYPE_ID<bool> },
		{ _T("byte"),			TYPE_ID<uint8> },
		{ _T("short"),			TYPE_ID<int16> },
		{ _T("unsigned short"),	TYPE_ID<uint16> },
		{ _T("int"),			TYPE_ID<int32> },
		{ _T("unsigned int"),	TYPE_ID<uint32> },
		{ _T("long"),			TYPE_ID<int64> },
		{ _T("unsigned long"),	TYPE_ID<uint64> },
		{ _T("float"), 			TYPE_ID<float32> },
		{ _T("double"), 		TYPE_ID<float64> },
		{ _T("binary string"), 	TYPE_ID<BitArray> },
	};



}// end of namespace



#endif // !TYPEDEFS_H
