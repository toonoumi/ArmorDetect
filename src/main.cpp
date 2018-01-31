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

void checkArgs(int argc, char* const argv[]);

int main(int argc, char* argv[])
{
	 int test_case = argv[1][0];
	 char** test_cmd = argv + 2;

	switch(test_case)
	{
		case '1':
			checkArgs(argc - 2 , test_cmd);
			bool color;
			color = (test_cmd[0][1] == 'b');

			if (strcmp("-F", test_cmd[1]) == 0)
			  test_FindHSVRange(argv + 4, argc - 4, color);
			else
			  test_FindHSVRange(strtol(test_cmd[2], nullptr, 0), color);
			 
			 break;	 
		case '2':
			test_threshMask(strtol(test_cmd[0], nullptr, 0));
	}
	  
  return EXIT_SUCCESS;
}

void checkArgs(int argc, char* const argv[])
{
  if (argc >= 3 && argv[0][0] == '-')
    {
      int color = argv[0][1];
      switch (color)
        {
          case 'r':
          case 'b':

            if (strcmp("-F", argv[1]) == 0 || (strcmp("-C", argv[1]) == 0 &&
                                               isdigit(argv[2][0])))
              return;
        }
    }

  cout << "Usage: ";
  cout << argv[0];
  cout << " -[r, b] -C [camera id number] | -F [path to image(s)]\n";

  std::exit(EXIT_FAILURE);
}

// End of file
