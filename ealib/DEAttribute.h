#ifndef DE_ATTRIBUTE_H
#define	DE_ATTRIBUTE_H


namespace ealib
{

	struct DEAttribute
	{
		float	F;	// Differential Weight. [0,2]
		float	CR;	// Crossover Probability. [0,1]

		float	F2;	// Differential Weight for DE_CURRENT_TO_BEST_1, DE_RAND_TO_BEST_1 operation. notimplemented yet. 2017.06.18

		void Clear()
		{
			F	= 0.0f;
			CR	= 0.0f;
			F2	= 0.0f;
		}
	};



	struct jDEAttribute
	{
		float	Fl;	// Lower Differential Weight
		float	Fu;	// Upper Differential Weight
		float	T1;
		float	T2;

		void Clear()
		{
			Fl	= 0.0f;
			Fu	= 0.0f;
			T1	= 0.0f;
			T2	= 0.0f;
		}
	};



	struct JADEAttribute
	{
		float	C;// MuF/MuCR's learning rate. 0.1 is recommended
		float	P;// best individual percentage. used for DE/current-to-pbest/1 mutation

		void Clear()
		{
			P	= 0.25f;
			C	= 0.1f;// set recommended value as default
		}
	};



	typedef JADEAttribute SHADEAttribute;

}// end of namespace

#endif // !DE_ATTRIBUTE_H
