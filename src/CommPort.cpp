/*******************************************************
 * File name:
 *
 * Description:
 *
 *
 * Created by: Author1
 * Edited by: Contributor1, Contributor2
 *
 * Created:   yyyy-mm-dd
 * Last Edit: 2018-05-18
 */


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
// Standard library
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// OpenCV library
#include "opencv2/core.hpp"

// local library/files
#include "CommPort.h"

/* ************************************************************************** */
/* Section: Types                                                             */
/* ************************************************************************** */
using namespace std;
using std::printf;
using std::exit;
using std::cout;

using cv::Point;

/* ************************************************************************** */
/* Section: Function Definitions                                              */
/* ************************************************************************** */
CommPort::CommPort()
{
 printf("CV Communication Test Program\n");
 pid_t pid = fork();

 if (!pid)
 {
   //如果是子进程
   execl("CVComm", "CVComm", (char *) NULL); //CVComm为可执行文件名
   return;
 }
 else if (pid > 0)
 {
   printf("Main PID: %d\n", getpid());
   mkfifo("/tmp/cv_uplink", 0666);
   cv_uplink = open("/tmp/cv_uplink", O_RDONLY);
   mkfifo("/tmp/cv_downlink", 0666);
   cv_downlink = open("/tmp/cv_downlink", O_WRONLY);

   if (cv_uplink < 0 || cv_downlink < 0)
   {
     exit(1);
   }

   uint8_t color;
   //uint8_t bfr[]={0x01,0x00,0x00,0x00,0x00};
   //int16_t errorx, errory;

   read(cv_uplink, &color, 1);
   printf("Main: Self color is %s\n", (color == 1) ? "RED" : "BLUE");

   //ifstream fin("input.txt");
   /*while(1)
   {
       fin.close();
       fin.clear();
       fin.open("input.txt");
       fin>>errorx>>errory;
       //cout<<errorx<<" "<<errory<<endl;
       bfr[1]=errorx>>8; bfr[2]=errorx; bfr[3]=errory>>8; bfr[4]=errory;
       write(cv_downlink,bfr,5);
       usleep(100 * 1000);
   }*/

   //getchar();
 }
 else
 {
   printf("Process Creation Failed.\n");
   exit(1);
 }

 baudRate = 9600;
} // default constructor

CommPort::CommPort(int baudRate)
{
 this->baudRate = baudRate;
}

CommPort::~CommPort() { } //destructor

int CommPort::getBaudRate()
{
 return this->baudRate;
} //getBaudRate

void CommPort::setBaudRate(int baud_rate)
{
 this->baudRate = baud_rate;
} //setBaudRate

void CommPort::sendDifference(Point point_of_difference)
{
 const int constant = 10;
 uint8_t bfr[] = {0x01, 0x00, 0x00, 0x00, 0x00};
 int16_t errorx, errory;
 errorx = point_of_difference.x*constant;
 errory = point_of_difference.y*constant;
 bfr[1] = errorx >> 8;
 bfr[2] = errorx;
 bfr[3] = errory >> 8;
 bfr[4] = errory;
 std::cout << "Writing To serial\n";
 write(cv_downlink, bfr, 5);
} //sendDifference

bool CommPort::requestTeamInfo()
{
 std::cout << "Requesting Team Information.\n";
} //requestTeamInfo

// End of file