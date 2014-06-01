/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

using namespace profundimetro;


uint16_t crc16_update(uint16_t crc, uint8_t a)
    {
        int i;

        crc ^= a;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = (crc >> 1);
        }

        return crc;
    }


Task::Task(std::string const& name)
    : TaskBase(name)
{
	StartData.data = msg_start;
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine)
{
	StartData.data = msg_start;
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;
    return true;
}
bool Task::startHook()
{
    	if (! TaskBase::startHook())
       		return false;

	base::Time time = base::Time::now();

	StartData.time = time;
	_outRaw.write(StartData);
    	return true;
}
void Task::updateHook()
{
    	int index;
	int len;
	int i = 0;
	base::Time time = base::Time::now();

	iodrivers_base::RawPacket sendData;
	sendData.time = time;
	std::vector<uint8_t> msg = {0xFA,0x49,0x01,0xA1,0xA7};
	sendData.data = msg;
	_outRaw.write(sendData);
	while (_inputRaw.read(rawpacket) == RTT::NewData){
		index = 0;
		current_packet.insert(current_packet.end(), rawpacket.data.begin(), rawpacket.data.end());
		len = rawpacket.data.size();
		while ( len > 0 ){
			 std::vector<int>::iterator it;

 			 // iterator to vector element:
  			it = std::find (current_packet.begin(), current_packet.end(), 0xFA);
			int index= std::distance(current_packet.begin(), it);
			if(*it==current_packet.end()){
				current_packet.erase(current_packet.begin(),it);
				break;
			}

			if(*(it+1)&(1>>7)){
				StartData.time = time;
				_outRaw.write(StartData);
				//current_packet.erase(current_packet.begin(),it+1);
				continue;
			}

			if(*(it+1)!=0x49){
				//current_packet.erase(current_packet.begin(),it+1);
				continue;
			}

			uint16_t crc = 0xffff;

			for(int i=0;i<7;i++)
				crc = crc16_update(crc, *(it+i));
			
			uint16_t *crc_real = it+8;
			
			if(crc_real!=crc){
				//current_packet.erase(current_packet.begin(),it+9);

			
				
				
			
			
	
			
			
			
		len -= bytes_received;
		}
	}

}
void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}
