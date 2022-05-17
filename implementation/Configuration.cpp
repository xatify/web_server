#include "Configuration.hpp"
#include "parsing/UserConfiguration.hpp"

Configuration *Configuration::conf = 0x0;

Configuration* Configuration::instance (const char *config_file) {
	if ((conf == 0x0)) {
		conf = new Configuration (config_file);
	}
	return conf;
}

Configuration::Configuration (const char *config_file) {
	UserHttpConfig = 0x0;

	Tokenizer tokenizer (config_file);
	try {
		tokenizer.tokenize ();
		UserHttpConfig = ComponentCreator::instance ()->create ("http");
		UserHttpConfig->syntax_parse (tokenizer);
	}
	catch (std::exception& e) {
		delete UserHttpConfig;
		UserHttpConfig = 0x0;
		std::cout << e.what () << std::endl;
	}
}

Component *Configuration::getConfigurations () const { return UserHttpConfig; }

const unsigned int Configuration::keepAlive () const { return keepalive; }

const Logger& Configuration::getLogger () const { return logger; }