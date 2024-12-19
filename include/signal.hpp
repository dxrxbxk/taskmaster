#ifndef signal_hpp
#define signal_hpp

#include "events/monitor.hpp"
#include "resources/unique_fd.hpp"

#include <signal.h>
#include <unordered_map>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- S I G N A L ---------------------------------------------------------

	class signal final : public sm::listener {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::signal;

			using map_type = std::unordered_map<std::string, int>;


			// -- private members ---------------------------------------------

			/* pipe */
			sm::unique_fd _pipe[2U];

			map_type _signals;


			// -- private static methods --------------------------------------

			/* signal handler */
			static void _handler(const int) noexcept;

			/* record */
			static auto _record(const int&) -> void;

			/* record */
			template <int... sigs>
			static auto _record(void) -> void {
				(self::_record(sigs), ...);
			}

			// -- private methods ---------------------------------------------
			
			void _fill_map(void);

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			signal(void);

			/* deleted copy constructor */
			signal(const self&) = delete;

			/* deleted move constructor */
			signal(self&&) = delete;

			/* destructor */
			~signal(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


		public:

			// -- public static methods ---------------------------------------

			/* shared */
			static auto shared(void) -> self&;

			static auto to_int(const std::string& name) -> int;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on event */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;




			#ifndef SIGUNUSED
			#define SIGUNUSED SIGSYS
			#endif

			enum : unsigned {
				TOTAL_KEYWORDS = 68,
				MIN_WORD_LENGTH = 2,
				MAX_WORD_LENGTH = 9,
				MIN_HASH_VALUE = 3,
				MAX_HASH_VALUE = 137
			};

			struct signal_entry {
				const char* key;
				int value;
			};


			static auto has_signal(const char *str, const sm::usize& len) -> int {

					static unsigned char asso_values[] = {
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138,  45,
						 40, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138,  75,  70,
						 95,  15,   5, 138,  10,   0,  15,  15,  30,  45,
						  0,  40,   0,   5,   0,  60,   5,  50, 138,  25,
						 85,   0, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
						138, 138, 138, 138, 138, 138, 138, 138
					};

					sm::usize hval = len;

					switch (hval) {

						default:
							hval += asso_values[(unsigned char)str[4]+2];
							// fallthrough
						case 4U:
						case 3U:
						case 2U:
							hval += asso_values[(unsigned char)str[1]+2];
							break;
					}

					sm::usize hash = hval + asso_values[(unsigned char)str[len - 1U]];


					static constexpr signal_entry table[] {
						{"", 0},
						{"", 0},
						{"", 0},
						{"INT",       SIGINT},
						{"TRAP",      SIGTRAP},
						{"", 0},
						{"SIGINT",    SIGINT},
						{"SIGTRAP",   SIGTRAP},
						{"SYS",       SIGSYS},
						{"STOP",      SIGSTOP},
						{"", 0},
						{"SIGSYS",    SIGSYS},
						{"SIGSTOP",   SIGSTOP},
						{"SIGWINCH",  SIGWINCH},
						{"SIGSTKFLT", SIGSTKFLT},
						{"", 0},
						{"", 0},
						{"", 0},
						{"URG",       SIGURG},
						{"KILL",      SIGKILL},
						{"WINCH",     SIGWINCH},
						{"SIGURG",    SIGURG},
						{"SIGKILL",   SIGKILL},
						{"", 0},
						{"SEGV",      SIGSEGV},
						{"", 0},
						{"VTALRM",    SIGVTALRM},
						{"SIGSEGV",   SIGSEGV},
						{"PWR",       SIGPWR},
						{"SIGVTALRM", SIGVTALRM},
						{"", 0},
						{"SIGPWR",    SIGPWR},
						{"", 0},
						{"", 0},
						{"TERM",      SIGTERM},
						{"", 0},
						{"", 0},
						{"SIGTERM",   SIGTERM},
						{"", 0},
						{"TTIN",      SIGTTIN},
						{"", 0},
						{"STKFLT",    SIGSTKFLT},
						{"SIGTTIN",   SIGTTIN},
						{"IOT",       SIGIOT},
						{"CONT",      SIGCONT},
						{"", 0},
						{"SIGIOT",    SIGIOT},
						{"SIGCONT",   SIGCONT},
						{"ILL",       SIGILL},
						{"ALRM",      SIGALRM},
						{"", 0},
						{"SIGILL",    SIGILL},
						{"SIGALRM",   SIGALRM},
						{"HUP",       SIGHUP},
						{"QUIT",      SIGQUIT},
						{"", 0},
						{"SIGHUP",    SIGHUP},
						{"SIGQUIT",   SIGQUIT},
						{"BUS",       SIGBUS},
						{"POLL",      SIGPOLL},
						{"", 0},
						{"SIGBUS",    SIGBUS},
						{"SIGPOLL",   SIGPOLL},
						{"", 0},
						{"TSTP",      SIGTSTP},
						{"", 0},
						{"", 0},
						{"SIGTSTP",   SIGTSTP},
						{"", 0},
						{"TTOU",      SIGTTOU},
						{"", 0},
						{"", 0},
						{"SIGTTOU",   SIGTTOU},
						{"FPE",       SIGFPE},
						{"PIPE",      SIGPIPE},
						{"", 0},
						{"SIGFPE",    SIGFPE},
						{"SIGPIPE",   SIGPIPE},
						{"", 0},
						{"ABRT",      SIGABRT},
						{"", 0},
						{"", 0},
						{"SIGABRT",   SIGABRT},
						{"", 0},
						// SIGUNUSED is not in signal.h
						// may be defined in bits/signum-arch.h
						{"SIGUNUSED", SIGUNUSED},
						{"", 0},
						{"", 0},
						{"IO",        SIGIO},
						{"", 0},
						{"CHLD",      SIGCHLD},
						{"SIGIO",     SIGIO},
						{"", 0},
						{"SIGCHLD",   SIGCHLD},
						{"", 0},
						{"XFSZ",      SIGXFSZ},
						{"", 0},
						{"UNUSED",    SIGUNUSED},
						{"SIGXFSZ",   SIGXFSZ},
						{"", 0},
						{"PROF",      SIGPROF},
						{"", 0},
						{"", 0},
						{"SIGPROF",   SIGPROF},
						{"", 0},
						{"USR2",      SIGUSR2},
						{"", 0},
						{"", 0},
						{"SIGUSR2",   SIGUSR2},
						{"", 0},
						{"USR1",      SIGUSR1},
						{"", 0},
						{"", 0},
						{"SIGUSR1",   SIGUSR1},
						{"", 0}, {"", 0}, {"", 0}, {"", 0},
						{"", 0}, {"", 0}, {"", 0}, {"", 0},
						{"", 0}, {"", 0}, {"", 0}, {"", 0},
						{"", 0}, {"", 0}, {"", 0}, {"", 0},
						{"", 0}, {"", 0}, {"", 0}, {"", 0},
						{"", 0},
						{"XCPU",      SIGXCPU},
						{"", 0},
						{"", 0},
						{"SIGXCPU",   SIGXCPU}
					};

					if ((len > MAX_WORD_LENGTH || len < MIN_WORD_LENGTH)
						|| hash > MAX_HASH_VALUE)
						throw sm::runtime_error("invalid signal");

					const auto& entry = table[hash];

					if (*str != *entry.key
					|| ::strcmp(str + 1U, entry.key + 1U) != 0)
						throw sm::runtime_error("invalid signal");

					return entry.value;
				}

	}; // class signal

} // namespace sm

#endif // signal_hpp
