//
//  AD_Util.h
//  ArmorDetect
//														Dates
//  Created by: Jason Lu			10/12/17.
//			Edited: Frank Eyenga 	12/17/2017 
//

#include "opencv2/videoio.hpp"
#include "AD_Util.h"
#include "test_suite.h"

using std::cout;

int main(int argc, char* argv[])
{
	checkArgs(argc, argv);
	
	if (strcmp("-F", argv[1]) == 0)
		test_FindBlueRange(argv+2, argc - 2);
	else
		test_FindBlueRange(strtol(argv[2], nullptr, 0));

	return EXIT_SUCCESS;
}



void checkArgs(int argc, char* const argv[])
{
	 if(argc == 3 && (strcmp("-F", argv[1]) == 0 || 
	 								 (strcmp("-C", argv[1]) == 0 && isdigit(argv[2][0])) ))
		{ return;}
		else if (argc > 3 && strcmp("-F", argv[1]) == 0)
		{ return; }
		
		cout << "Usage: ";
	 	cout << argv[0];
	 	cout << " -C [camera id number] | -F [path to image(s)]\n";
	 	 
	 	std::exit(EXIT_FAILURE);
}

// End of file
