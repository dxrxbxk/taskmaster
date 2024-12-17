#ifndef unique_ptr_hpp
#define unique_ptr_hpp

#include "common/memory/malloc.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- U N I Q U E  P T R --------------------------------------------------

	template <typename T>
	class unique_ptr {


		// -- friends ---------------------------------------------------------

		/* unique_ptr as friend */
		template <typename U>
		friend class sm::unique_ptr;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::unique_ptr<T>;


			// -- private members ---------------------------------------------

			/* pointer to data */
			T* _data;


			// -- private lifecycle -------------------------------------------

			/* data constructor */
			unique_ptr(T* data) noexcept
			: _data{data} {
			}


			// -- private methods ---------------------------------------------

			/* free */
			void _free(void) noexcept {

				// check for allocated data
				if (_data == nullptr)
					return;

				// destroy data
				_data->~T();
				// deallocate memory
				sm::free(_data);
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			unique_ptr(void) noexcept
			: _data{nullptr} {
			}

			/* deleted copy constructor */
			unique_ptr(const self&) = delete;

			/* move constructor */
			unique_ptr(self&& other) noexcept
			: _data{other._data} {

				// invalidate other
				other._data = nullptr;
			}

			/* move constructor (derived) */
			template <typename D>
			unique_ptr(sm::unique_ptr<D>&& other) noexcept
			: _data{static_cast<T*>(other._data)} {

				static_assert(std::is_base_of<T, D>::value, "invalid type conversion");

				// invalidate other
				other._data = nullptr;
			}

			/* destructor */
			~unique_ptr(void) noexcept {
				self::_free();
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				// check for self assignment
				if (this == &other)
					return *this;

				// deallocate self data
				self::_free();

				// move data
				_data = other._data;
				other._data = nullptr;

				// done
				return *this;
			}


			// -- public conversion operators ---------------------------------

			/* bool conversion operator */
			explicit operator bool (void) {
				return _data != nullptr;
			}


			// -- public operators --------------------------------------------

			/* not operator */
			auto operator!(void) noexcept -> bool {
				return _data == nullptr;
			}

			/* dereference operator */
			auto operator->(void) noexcept -> T* {
				return _data;
			}

			/* const dereference operator */
			auto operator->(void) const noexcept -> const T* {
				return _data;
			}



		// -- friends ---------------------------------------------------------

		/* make unique as friend */
		template <typename U, typename... Ts>
		friend auto make_unique(Ts&& ...) -> sm::unique_ptr<U>;

	}; // class unique_ptr


	// -- non-member functions ------------------------------------------------

	/* make unique */
	template <typename T, typename... Ts>
	auto make_unique(Ts&& ...args) -> sm::unique_ptr<T> {

		// allocate memory
		T* data = sm::malloc<T>();

		// need to implement allocation guard (possible memory leak)

		// construct object
		::new(data) T{std::forward<Ts>(args)...};

		return sm::unique_ptr<T>{data};
	}

} // namespace sm

#endif // unique_ptr_hpp
