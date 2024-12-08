#ifndef timestamp_hpp
#define timestamp_hpp


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- T I M E S T A M P ---------------------------------------------------

	class timestamp final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::timestamp;


			// -- private members ---------------------------------------------

			/* buffer */
			char _buffer[22U];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			timestamp(void) noexcept;

			/* copy constructor */
			timestamp(const self&) noexcept = default;

			/* move constructor */
			timestamp(self&&) noexcept = default;

			/* destructor */
			~timestamp(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) const noexcept -> const char(&)[22U];

			/* size */
			auto size(void) const noexcept -> unsigned;

	}; // class timestamp

} // namespace sm

#endif // timestamp_hpp
