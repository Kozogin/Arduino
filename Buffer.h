#ifndef BUFFER_H
#define BUFFER_H

#define SerialMon Serial
#define SerialAT Serial1

class Buffer
{
private:
    char inChar1 = ' '; 
    char inChar2 = ' ';
    std::string messageGsm = "";  

public:

    Buffer(){
        messageGsm = "";
    }

    void bufferRead()
    {
        try{
            while (SerialAT.available())
            {
                inChar1 = SerialAT.read();
                SerialMon.write(inChar1);
                messageGsm += inChar1;
            }
            while (SerialMon.available())
            {
                inChar2 = SerialMon.read();
                SerialAT.write(inChar2);
            }
        }
		catch (...)
		{
			std::cout << "Buffer::bufferRead()    --------***********------------***********     " << "\n";
		}
    }

    void resetMessageGsm(){
        messageGsm = "";
    }

    std::string getMessageGsm(){
        return messageGsm;
    }

};

#endif