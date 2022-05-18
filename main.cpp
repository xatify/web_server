#include <iostream>
//#include "./Networking/Connections/Connection.hpp"
//#include "./Sockets/socket.hpp"

#include "ServerConfiguration/ConfigfileParsing/UserConfiguration.hpp"
#include "ServerConfiguration/Configuration.hpp"
int main (int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Configuration file is required as first argument\n";
        exit (1);
    }

    //ConnectionsHandler hander;
    Component *HttpConfig = Configuration::instance (argv[1])->getConfigurations ();
    if (HttpConfig) {
        std::vector <Component*> vservers = HttpConfig->getSubComponents ("server");
        if (!vservers.empty ()) {
            std::vector<Listen *> listens;
            bool default_listen = false;
            for (std::vector <Component*>::iterator it = vservers.begin (); it != vservers.end (); ++it) {
                
                Listen *listen = dynamic_cast<Listen *> ((*it)->getSimpleAttribute ("listen"));
                if (!listen) {
                    Component *listenComp = ComponentCreator::instance ()->create ("listen");
                    (*it)->addSubComponent (listenComp);
                    listen = dynamic_cast<Listen*> (listenComp);
                    if (!default_listen) {
                        default_listen = true;
                        listens.push_back (listen);
                    }
                }
                else
                    listens.push_back (listen);
            }
            
            
            for (std::vector<Listen *>::iterator it = listens.begin (); it != listens.end (); ++it) {
                (*it)->pretty_print ();
                std::cout << "\n";
            }
            // for (std::vector <Listen *>::iterator it = listens.begin (); it != listens.end (); ++it) {
            //     Addr addr (it->getP)
            //     Listen *p;
            // }
        }
    }
    //while (1);
}