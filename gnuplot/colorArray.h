#pragma once

namespace JSL
{

	class ColourArray
	{
		public:
		
			std::vector<double> GetNextColour()
			{
				++Index;
				return ColourList[(Index-1) % ColourList.size()];
				prevColour = "black";
			}
			std::string HoldColour()
			{
				return prevColour;
			}
			ColourArray()
			{
				Index = 0;
				ColourList = {      {0, 0.447, 0.741},
         							{0.85, 0.325,0.098},
			        				{0.929,  0.694 ,0.125},
 	       							{0.494, 0.184 ,  0.556},
        							{0.466, 0.674,  0.188},
        							{0.301, 0.745, 0.933},
        							{0.635, 0.078,0.184}
				};
			}
			void Save(std::string color)
			{
				prevColour = color;
			}
		private:
			int Index;
			std::vector<std::vector<double>> ColourList;
			std::string prevColour;
	};


}