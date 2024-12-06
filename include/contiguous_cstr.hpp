#ifndef contiguous_cstr_hpp
#define contiguous_cstr_hpp

#include "memory/malloc.hpp"
#include "types.hpp"


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
			sm::u32 _size;

			/* capacity */
			sm::u32 _capacity;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			contiguous_cstr(void) noexcept
			: _data{nullptr}, _size{0U}, _capacity{0U} {
			}


			/* copy constructor */
			contiguous_cstr(const self& other) {
			}

			/* move constructor */
			contiguous_cstr(self&& other) noexcept
			: _data{other._data}, _size{other._size}, _capacity{other._capacity} {

				// invalidate other
				other._init();
			}

			/* destructor */
			~contiguous_cstr(void) noexcept {

				if (_data == nullptr)
					return;

				for (sm::u32 i = 0U; i < _size; ++i)
					sm::free(_data[i]);

				sm::free(_data);
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self& other) -> self& {
				return *this;
			}

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
				for (sm::u32 i = 0U; i < _size; ++i)
					sm::free(_data[i]);

				// reset size
				_size = 0U;
			}

			/* reserve */
			auto reserve(const sm::u32& capacity) -> void {

				// check current capacity
				if (capacity <= _capacity)
					return;

				// reallocate
				self::_reserve(capacity);
			}


		private:

			/* reserve */
			auto _reserve(const sm::u32& capacity) -> void {

				// reallocate
				_data = sm::realloc(_data, capacity + 1U);

				// update capacity
				_capacity = capacity;

				// null terminate
				_data[_capacity] = nullptr;
			}


		public:

			/* push */
			auto push(const char* data, const sm::u32& size) -> void {

				// check if we need to allocate
				if (_size == _capacity) {

					// calculate new capacity
					const sm::u32 cap = _capacity == 0U ? 1U : (_capacity * 2U);

					// reallocate
					self::_reserve(cap);
				}

				// allocate new string
				//_data[_size] = sm::malloc<char>(arg.size() + 1U);

				// copy string
				//std::copy(arg.begin(), arg.end(), _data[_size]);

				// null terminate
				//_data[_size][arg.size()] = '\0';

				// increment size
				++_size;
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

				for (sm::u32 i = 0U; i < _size; ++i)
					sm::free(_data[i]);

				sm::free(_data);
			}

	}; // class contiguous_cstr

} // namespace sm

#endif // contiguous_cstr_hpp
