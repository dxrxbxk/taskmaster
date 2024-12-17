#ifndef error_hpp
#define error_hpp

#include "string/strlen.hpp"
#include "memory/memcpy.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- E R R O R -----------------------------------------------------------

	class error final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::error;


			// -- private members ---------------------------------------------

			/* data */
			char* _data;

			/* size */
			sm::usize _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			error(void) noexcept
			: _data{nullptr}, _size{0U} {
			}

			/* message constructor */
			error(const char* msg) noexcept
			: _data{nullptr}, _size{sm::strlen(msg)} {

				if (_size == 0U) {

					msg = "unknown error";
					_size = 13U;
				}

				// allocate memory
				_data = static_cast<char*>(::operator new(_size + 1U));

				if (_data == nullptr)
					return;

				sm::memcpy(_data, msg, _size);

				_data[_size] = '\0';
			}

			/* deleted copy constructor */
			error(const self&) = delete;

			/* move constructor */
			error(self&&) noexcept = default;

			/* destructor */
			~error(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public modifiers --------------------------------------------

			/* push error message */
			auto push(const char* msg) noexcept -> void {

				constexpr const char sep[]         = " \x1b[32m>\x1b[0m ";
				constexpr const sm::usize sep_size = sizeof(sep) - 1U;

				// push front
				sm::usize len = sm::strlen(msg);

				char* data = static_cast<char*>(::operator new(_size + len + sep_size + 1U));

				if (data == nullptr)
					return;

				char* offset = data;

				sm::memcpy(offset, msg, len);
				offset += len;
				sm::memcpy(offset, sep, sep_size);
				offset += sep_size;

				if (_data != nullptr) {
					sm::memcpy(offset, _data, _size);
					offset += _size;
					::operator delete(_data);
				}

				*offset = '\0';

				_data = data;
				_size += len + sep_size;
			}


			// -- public accessors --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const char* {
				return _data;
			}

			/* has error */
			auto has_error(void) const noexcept -> bool {
				return _data != nullptr;
			}





	}; // class error

} // namespace sm

#endif // error_hpp
