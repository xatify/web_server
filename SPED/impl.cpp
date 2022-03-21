

enum Event_type {
	ACCEPT_EVENT 	= 01,
	READ_EVENT		= 02,
	WRITE_EVENT		= 04,
	TIME_OUT_EVENT	= 010,
	SIGNAL_EVENT	= 020,
	CLOSE_EVENT		= 040
};

class Event_Handler {
	public:
		virtual int handle_accept (void) = 0;
		virtual int handle_input (void) = 0;
		virtual int handle_output (void) = 0;
		virtual int handle_timeout (void) = 0;
		virtual int handle_close (void) = 0;

		virtual HANDLE get_handle (void) const = 0;
};

class Logging_Acceptor: public Event_Handler {
	public:
		Logging_Acceptor (const INET_Addr &addr);
		virtual void handle_event (Event_type et);
		virtual HANDLE get_handle (void) const {
			return acceptor_.get_handle ();
		}
	private:
		SOCK_Acceptor acceptor_;

};

Logging_Acceptor::Logging_Acceptor (const INET_Addr &addr): acceptor_ (addr) {
	Initiation_Dispatcher::instance () -> register_handler (this, ACCEPT_EVENT);
}
void Logging_Acceptor::handle_event (Event_type et) {
	assert (et == ACCEPT_EVENT);

	SOCK_Stream new_connection;

	acceptor_.accept (new_connection);

	Logging_Handler *handler = new Logging_Handler (new_connection);
}

class Logging_Handler: public Event_Handler {
	public:
		Logging_Handler (SOCK_Stream *cs);
		virtual void handle_event (Event_type et);
		virtual HANDLE get_handle (void) const {
			return peer_stream_.get_handle ();
		}
	private:
		SOCK_Stream peer_stream_;
};

Logging_Handler::Logging_Handler (SOCK_Stream &cs): peer_stream_ (cs) {
	Initiation_Dispatcher::instance ()->register_handler (this, READ_EVENT);
}

void Logging_Handler::handle_event (Event_type et) {
	if (et == READ_EVENT) {
		Log_Record = log_record;

		peer_stream_.recv ((void *)log_record, sizeof (log_record));
		log_record.write (STDOUT);
	}
	else if (et == CLOSE_EVENT) {
		peer_stream.close ();
		delete (void  *)this;
	}
}


class Initiation_Dispatcher {
	public:
		int register_handler (Event_Handler *eh, Event_type et);
		int remove_handler (Event_Handler *eh, Event_type et);
		int handle_events (Time_Value *timeout = 0);
};


const unsigned short PORT = 10000;

int main () {
	INET_Addr server_addr (PORT);

	Logging_Acceptor la (server_addr);

	for (;;) {
		Initiation_Dispatcher::instance () -> handle_events ();
	}
	return 0;
}