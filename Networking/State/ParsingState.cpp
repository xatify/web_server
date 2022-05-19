#include "ParsingState.hpp"

#include "../Connections/DataConnection.hpp"
#include "../../Request/Request.hpp"
#include "CloseState.hpp"
#include "ErrorState.hpp"
#include "ExecuteState.hpp"
#include <unistd.h>
#include <string>


ParsingState::ParsingState (dataConnection *context): State (context) {}


// this must be accessed 
// in the configuration
#define MAXLINESIZE 1024 


// states translation:
// if event == read and read returns 0  				-> CloseState;
// if (request RL and HF are parsed)                    -> ExecuteState;
// if (still parsing and error in format of request)	-> ExecuteState; 
void ParsingState::handle () {
    std::string &input = context->getInput ();
    if (context->getEvent () == READ) {
        // this part neeeds redesigning
        ssize_t  size = 1024;
        char *buff = new char [size];
        size_t rd = ::read (context->getFd (), buff, size);
        if (rd <= 0) {
            delete [] buff;
            context->TranslationTo (new CloseState ());
            return;
        }
        input.append (buff, buff + rd);
        delete [] buff;
    }
    if (!(context->getRequest ()->isParsed ())) {
        std::string::size_type n = input.find ("\r\n");
        if (n != std::string::npos) {
            if (context->getRequest ()->parse (input.substr (0, n)) == false)
                context->TranslationTo (new ErrorState ());
            else {
                input.assign (input.begin () + n + 2, input.end ());
                return;
            }
        }
        else if (input.size () > MAXLINESIZE) {
            context->TranslationTo (new ErrorState ());
        }
    }
    else
        context->TranslationTo (new ExecuteState ());
}
