//
//  CommPort.h
//  ArmorDetect
//
//  Created by Jason Lu on 3/30/18.
//
#include<iostream>
#ifndef CommPort_h
#define CommPort_h
class CommPort{
private:
    int baudRate=9600;
public:
    CommPort(){
        
    }
    ~CommPort(){
        
    }
    CommPort(int baudRate){
        this.baudRate=baudRate;
    }
    int getBaudRate(){
        return this.baudRate;
    }
    void setBaudRate(int baud_rate){
        this.baudRate=baud_rate;
    }
    void sendDifference(Point point_of_difference){
        
    }
    bool requestTeamInfo(){
        std::cout<<"Requesting Team Information.\n";
    }
};

#endif /* CommPort_h */
