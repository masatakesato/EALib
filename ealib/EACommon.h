#ifndef	EA_COMMON_H
#define	EA_COMMON_H

#include	<oreore/common/Utility.h>



namespace ealib
{

	// EA共通アトリビュート
	struct EACommonAttribute
	{
		int32	PopulationSize;	// 個体数.
		int32	NumGenerations;	// 世代数(最大)
		int32	EliteSize;		// エリート個体の数

		void Clear()
		{
			PopulationSize			= 0;
			NumGenerations			= 0;
			EliteSize				= 0;
		}
	};



	// SimpleGAの設定
	struct SGAAttribute
	{
		float	CrossoverRate;		// 交叉率 [0.0, 1.0]
		float	MutationRate;		// 突然変異率 [0.0, 1.0]

		void Clear()
		{
			CrossoverRate		= 0.0f;
			MutationRate		= 0.0f;
		}
	};



	enum MIGRATION_MODE
	{
		SELECT_BEST_REPLACE_WORST,
		SELECT_BEST_REPLACE_RANDOM,
		SELECT_RANDOM_REPLACE_WORST,
		SELECT_RANDOM_REPLACE_RANDOM,
	};



	// MultiIslandEAの設定
	struct MIEAAttribute
	{
		int32			IslandSize;
		MIGRATION_MODE	MigrationMode;
		int32			MigrationInterval;	// 個体移住が発生する何世代間隔
		float			MigrationRate;	// 別島への移住率. [0.0, 1.0]

		void Clear()
		{
			IslandSize			= 0;
			MigrationMode		= MIGRATION_MODE::SELECT_BEST_REPLACE_WORST;
			MigrationInterval	= 0;
			MigrationRate		= 0.0f;
		}
	};


}// end of namespace


#endif	// EA_COMMON_H //