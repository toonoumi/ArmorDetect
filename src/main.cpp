//
//  main.cpp
//  ArmorDetect
//
// Created by Jason Lu on 2/25/18.
// Edited: Frank Eyenga 	04/12/2018
//

#include "opencv2/videoio.hpp"
#include "AD_Util.h"
#include "ArmorDetect.hpp"
#include "test_suite.h"

using std::cout;
using std::vector;

void checkArgs(int argc, char* const argv[]);
void checkArgsTest1(int argc, char* const argv[]);
void checkArgsTest2(int argc, char* const argv[]);

int main(int argc, char** argv)
{
	 checkArgs(argc, argv);

	 int test_case = argv[1][0];
	 char** test_cmd = argv + 2;

//	switch(test_case)
//	{
//		case '1':
//			checkArgsTest1(argc - 2 , test_cmd);
//			bool isRed;
//			isRed = (test_cmd[0][1] == 'r');

//			if (strcmp("-F", test_cmd[1]) == 0)
//			  test_FindHSVRange(test_cmd + 2, argc - 4, isRed);
//			else
//			  test_FindHSVRange(strtol(test_cmd[2], nullptr, 0), isRed);
			 
//			 break;
//		case '2':
//			checkArgsTest2(argc - 2 , test_cmd);
//			HSVRange range{.LowH = 75, .HighH = 130,
//						  .LowS = 85, .HighS = 255,
//						  .LowV = 50, .HighV = 255};
						  
//			if (strcmp("-F", test_cmd[0]) == 0)
//				test_threshMask(test_cmd, argc - 2, range);
//			else
//				test_threshMask(strtol(test_cmd[1], nullptr, 0), range);

//	}
	  
  return EXIT_SUCCESS;
}

void checkArgs(int argc, char* const argv[])
{
	if(argc == 1)
	{
		ArmorDetect ad;
	  	exit(EXIT_SUCCESS);
	}
	
	if(argc < 3)
	{
	  	cout << "Usage: ";
	  	cout << argv[0] << "[Test Number] [Test command(s)\n]";
	  	exit(EXIT_FAILURE);
	}
	
} 

void checkArgsTest1(int argc, char* const argv[])
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

  cout << "Unregonized commands/inputs for given test\n";
  cout << "Usage: -[r, b] -C [camera id number] | -F [path to image(s)]\n";

  std::exit(EXIT_FAILURE);
}

void checkArgsTest2(int argc, char* const argv[])
{
  if (argc >= 2 && (strcmp("-F", argv[0]) == 0 || (strcmp("-C", argv[0]) == 0 &&
                                               		isdigit(argv[1][0]))))
    {
    	return;
    }

  cout << "Unregonized commands/inputs for given test\n";
  cout << "Usage: -C [camera id number] | -F [path to image(s)]\n";

  std::exit(EXIT_FAILURE);
}
