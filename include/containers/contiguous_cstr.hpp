#ifndef contiguous_cstr_hpp
#define contiguous_cstr_hpp

#include "memory/malloc.hpp"
#include "memory/memcpy.hpp"
#include "memory/malloc_guard.hpp"
#include "string/strlen.hpp"
#include "types.hpp"

#include <iostream>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C O N T I G U O U S  C S T R ----------------------------------------

	class contiguous_cstr final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::contiguous_cstr;


			// -- private members ---------------------------------------------

			/* data */
			char** _data;

			/* size */
			sm::usize _size;

			/* capacity */
			sm::usize _capacity;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			contiguous_cstr(void) noexcept
			: _data{nullptr}, _size{0U}, _capacity{0U} {
			}

			/* deleted copy constructor */
			contiguous_cstr(const self&) = delete;

			/* move constructor */
			contiguous_cstr(self&& other) noexcept
			: _data{other._data}, _size{other._size}, _capacity{other._capacity} {

				// invalidate other
				other._init();
			}

			/* destructor */
			~contiguous_cstr(void) noexcept {
				self::_free();
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				// check for self assignment
				if (this == &other)
					return *this;

				// invalidate this
				self::_free();

					// move other
					_data = other._data;
					_size = other._size;
				_capacity = other._capacity;

				// invalidate other
				other._init();

				// done
				return *this;
			}


			// -- public modifiers --------------------------------------------

			/* clear */
			auto clear(void) noexcept -> void {

				// free cstrs
				for (sm::usize i = 0U; i < _size; ++i)
					sm::free(_data[i]);

				// reset size
				_size = 0U;
			}

			/* reserve */
			auto reserve(const sm::usize& capacity) -> void {

				// check current capacity
				if (capacity <= _capacity)
					return;

				// reallocate
				self::_reserve(capacity);
			}


		private:

			/* reserve */
			auto _reserve(const sm::usize& capacity) -> void {

				// reallocate
				_data = sm::realloc(_data, capacity + 1U);

				// update capacity
				_capacity = capacity;

				// null terminate
				for (sm::usize i = _size; i <= _capacity; ++i)
					_data[i] = nullptr;
			}


		public:

			/* push */
			auto push(const char* data, const sm::usize& size) -> void {

				// check if we need to allocate
				if (_size == _capacity) {

					// calculate new capacity
					const sm::usize cap = _capacity == 0U ? 1U : (_capacity * 2U);

					// reallocate
					self::_reserve(cap);
				}

				// allocate new string
				_data[_size] = sm::malloc<char>(size + 1U);

				// copy string
				sm::memcpy(_data[_size], data, size);

				// null terminate
				_data[_size][size] = '\0';

				// increment size
				++_size;
			}

			/* push */
			auto push(const char* data) -> void {
				self::push(data, sm::strlen(data));
			}


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> char** {
				return _data;
			}

			/* data */
			auto data(void) const noexcept -> const char* const* {
				return _data;
			}


			// -- public operators --------------------------------------------

			/* operator[] */
			auto operator[](const sm::usize& index) noexcept -> char* {
				return _data[index];
			}

			/* operator[] */
			auto operator[](const sm::usize& index) const noexcept -> const char* {
				return _data[index];
			}


			// -- public methods ----------------------------------------------

			/* debug */
			auto debug(void) const noexcept -> void {

				if (_data == nullptr) {
					std::cout << "cstr is not allocated" << std::endl;
					return;
				}

				for (sm::usize i = 0U; i < _size; ++i)
					std::cout << _data[i] << std::endl;

				if (_data[_size] != nullptr)
					std::cout << "not null terminated" << std::endl;
				else
					std::cout << "null terminated" << std::endl;
			}


		private:

			// -- private methods ---------------------------------------------

			/* init */
			auto _init(void) noexcept -> void {
					_data = nullptr;
					_size = 0U;
				_capacity = 0U;
			}

			/* free */
			auto _free(void) noexcept -> void {

				if (_data == nullptr)
					return;

				for (sm::usize i = 0U; i < _size; ++i)
					sm::free(_data[i]);

				sm::free(_data);
			}

	}; // class contiguous_cstr

} // namespace sm

#endif // contiguous_cstr_hpp
