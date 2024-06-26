#include "N2K_I2C_API.h"
#include "main.cpp"

#define REQUEST_MESSAGE_LENGTH 4


I2CRequestHandler I2CRequestHandlers[] = {
    {0, &status},
    {1, &newPGN},
    {8, &spesificPGN}
};

void HandleI2CMessage(byte* request)
{
    int i;
    long pgn;
    for (i = 0; I2CRequestHandlers[i].request != request[0]; i++);
    
    for (int p = 1; p <= REQUEST_MESSAGE_LENGTH; p++) 
    {
        pgn = (pgn << 8) | request[p];  // Bitshifting to get the PGN from the three bytes in the request
    }

    I2CRequestHandlers[i].Handler(pgn);
    
}

void status(long pgn)
{
    byte status = 8;
    Wire.slaveWrite(&status, 1);    
}

void newPGN(long pgn)
{
    byte status = 8;
    
    Wire.slaveWrite(&status, 1);
}

void spesificPGN(long pgn)
{
    if(pgn = 129026){
        
        String response = n2kCOGSOG->N2KtoJSON();
        int length = response.length();
        uint8_t bytearray[length + 2];
        response.getBytes(bytearray + 2, length + 1);
        bytearray[0] = (byte) length >> 8;
        bytearray[1] = (byte) length;
        Wire.flush();
        Wire.slaveWrite(bytearray, length + 2);
    };
}

void requestData()
{
}

void receiveData(int byteCount)
{
    byte request[byteCount];
    int index = 0;
    while (Wire.available())
    {
        request[index] = Wire.read();
        index++;
    };
    HandleI2CMessage(request);
}
