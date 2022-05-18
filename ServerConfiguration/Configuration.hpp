#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include "../Networking/Logger/Logger.hpp"
#include "./ConfigfileParsing/UserConfiguration.hpp"

class Configuration {
	private:
		static Configuration		*conf;
		Component 					*UserHttpConfig;
		//const unsigned int			keepalive = 75;
		Logger						logger;
		Configuration (const char *config_file);

	public:
		static	Configuration* instance (const char *config_file= 0x0);
		const Logger& getLogger () const;
		//const unsigned int keepAlive() const;
		Component *getConfigurations () const;
};


#endif