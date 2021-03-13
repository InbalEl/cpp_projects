/******************************************************************************
* Project name:					 	req_disp             ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Dec 02, 2020          *      ****
* Project completion				Dec 03, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include "dispatcher.hpp"
#include "distributor.hpp"
#include "bio_access.h"

static const char *dev_name = "/dev/nbd0";
static size_t size = 128 * 1024 * 1024;

/*********************************  MAIN  ************************************/

using namespace ilrd::rd90;

int main()
{
    int bio_fd = BioDevOpen(dev_name, size);
    if ((-1) == bio_fd)
    {
        return (1);
    }
    
    std::cout << "bio_fd = " << bio_fd << std::endl;
    Reactor<Epoll> epoll_react(new Epoll(3));
    std::shared_ptr<DistModulo> dist(new DistModulo);
    std::cout << "epoll reactor created" << std::endl;
    std::cout << "Modulo distributor created" << std::endl;
    RequestDispatcher dispatch(epoll_react, bio_fd, dist);
    std::cout << "Dispatcher created" << std::endl;

    dispatch.RegisterIoT("127.0.0.1");
    std::cout << "Registring IoT" << std::endl;
    epoll_react.Run();

    return (0);
}

/*****************************************************************************/
