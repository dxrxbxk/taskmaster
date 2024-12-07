#ifndef running_hpp
#define running_hpp


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- R U N N I N G -------------------------------------------------------

	class running final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::running;


			// -- private members ---------------------------------------------

			/* state */
			bool _state;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			running(void) noexcept
			: _state{true} {
			}

			/* deleted copy constructor */
			running(const self&) = delete;

			/* deleted move constructor */
			running(self&&) = delete;

			/* destructor */
			~running(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}


		public:

			// -- public static methods ---------------------------------------

			/* state */
			static auto state(void) noexcept -> bool {
				return self::_shared()._state;
			}

			/* stop */
			static auto stop(void) noexcept -> void {
				self::_shared()._state = false;
			}

			/* start */
			static auto start(void) noexcept -> void {
				self::_shared()._state = true;
			}

	}; // class running

} // namespace sm

#endif // running_hpp
