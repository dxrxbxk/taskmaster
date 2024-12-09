#ifndef unique_dir_hpp
#define unique_dir_hpp

#include "common/diagnostics/exception.hpp"

#include <dirent.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- U N I Q U E  D I R --------------------------------------------------

	class unique_dir final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::unique_dir;


			// -- private members ---------------------------------------------

			/* directory */
			::DIR* _dir;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			unique_dir(void) noexcept
			: _dir{nullptr} {
			}

			/* path constructor */
			template <unsigned int N>
			unique_dir(const char (&path)[N])
			: _dir{::opendir(path)} {

				if (_dir == nullptr)
					throw sm::system_error("opendir");
			}

			/* deleted copy constructor */
			unique_dir(const self&) = delete;

			/* move constructor */
			unique_dir(self&& other) noexcept
			: _dir{other._dir} {
				other._dir = nullptr;
			}

			/* destructor */
			~unique_dir(void) noexcept {

				if (_dir == nullptr)
					return;

				::closedir(_dir);
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				if (this == &other)
					return *this;

				if (_dir != nullptr)
					::closedir(_dir);

				_dir = other._dir;
				other._dir = nullptr;

				return *this;
			}


			// -- public methods ----------------------------------------------

			/* read */
			auto read(void) -> ::dirent* {
				return ::readdir(_dir);
			}


	}; // class directory

} // namespace sm



#endif // unique_dir_hpp
