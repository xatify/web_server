#include "Configuration.hpp"
#include "./ConfigfileParsing/UserConfiguration.hpp"

Configuration *Configuration::conf = 0x0;

Configuration* Configuration::instance (const char *config_file) {
	if (conf == 0x0) {
		conf = new Configuration (config_file);
	}
	return conf;
}

Configuration::Configuration (const char *config_file) {
	UserHttpConfig = 0x0;

	try {
		Tokenizer tokenizer (config_file);
		tokenizer.tokenize ();
		UserHttpConfig = ComponentCreator::instance ()->create ("http");
		UserHttpConfig->syntax_parse (tokenizer);
		
		// set listen directive in vserver that don't have them to 0.0.0.0:80
		std::vector<Component *> vservers = UserHttpConfig->getSubComponents ("server");
		if (vservers.empty ())
			throw std::runtime_error ("no server block in configfile\n");
		for (std::vector<Component *>::iterator it = vservers.begin (); it != vservers.end (); ++it) {
			if (!((*it)->getSimpleAttribute ("listen")))
				(*it)->addSubComponent (ComponentCreator::instance ()->create ("listen"));
		}
	}
	catch (std::exception& e) {
		delete UserHttpConfig;
		UserHttpConfig = 0x0;
		std::cout << e.what () << std::endl;
	}
}

Component *Configuration::getConfigurations () const { return UserHttpConfig; }

const unsigned int Configuration::keepAlive () const { return keepalive; }

Logger& Configuration::getLogger () { return logger; }