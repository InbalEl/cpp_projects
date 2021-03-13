/******************************************************************************
* Project name:					 	iot_agent            ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Jan 11, 2020          *      ****
* Project completion				Jan 13, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include "connection.hpp"
#include "storage.hpp"
#include "iot.hpp"

/*********************************  MAIN  ************************************/

using namespace ilrd::rd90;

int main(/* int argc, char **argv */)
{
    // take as args: storage size, connection type, number of IoT's

    // loop over fork() with growing port number, creating iot's
    size_t nof_iots = 1;
    size_t port = 29000;
    int postfix = 0;
    size_t storage_size = 128;
    for (size_t i = 0; i < nof_iots; i++)
    {
        //fork();
        // inside child:
        std::shared_ptr<TcpConn> tcp(new TcpConn(port, postfix));
        std::shared_ptr<IStorage> storage(new Heap(storage_size));
        IoT dev(tcp, storage, 1024);        
        dev.Activate();
        
        //inside parent:
        ++postfix;
    }
    
    // If we forked we should wait!

    return (0);
}

/*****************************************************************************/