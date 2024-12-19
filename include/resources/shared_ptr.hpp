#ifndef shared_ptr_hpp
#define shared_ptr_hpp

#include "memory/malloc.hpp"
#include "memory/malloc_guard.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- S H A R E D  P T R --------------------------------------------------

	template <typename T>
	class shared_ptr final {


		// -- friends ---------------------------------------------------------

		/* shared_ptr as friend */
		template <typename U>
		friend class sm::shared_ptr;

		/* make shared as friend */
		template <typename U, typename... Ts>
		friend auto make_shared(Ts&& ...) -> sm::shared_ptr<U>;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::shared_ptr<T>;


			// -- private members ---------------------------------------------

			/* pointer to data */
			T* _data;

			/* reference counter */
			sm::usize* _ref;


			// -- private lifecycle -------------------------------------------

			/* data constructor */
			shared_ptr(T* data, sm::usize* ref) noexcept
			: _data{data}, _ref{ref} {
			}


			// -- private methods ---------------------------------------------

			/* free */
			void _free(void) noexcept {

				// check for allocated data
				if (_data == nullptr)
					return;

				// check for allocated counter
				if (--(*_ref) > 0)
					return;

				// destroy data
				_data->~T();

				// deallocate data
				sm::free(_data);

				// deallocate counter
				sm::free(_ref);
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			shared_ptr(void) noexcept
			: _data{nullptr},
			  _ref{nullptr} {
			}

			/* copy constructor */
			shared_ptr(const self& other) noexcept
			: _data{other._data}, _ref{other._ref} {

				// check for valid data
				if (_data == nullptr)
					return;

				// increment reference counter
				++(*_ref);
			}

			/* move constructor */
			shared_ptr(self&& other) noexcept
			: _data{other._data}, _ref{other._ref} {

				// invalidate other
				other._data = nullptr;
				other._ref  = nullptr;
			}

			/* destructor */
			~shared_ptr(void) noexcept {

				// deallocate memory
				this->_free();
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self& other) noexcept -> self& {

				// check for self assignment
				if (this == &other)
					return *this;

				// deallocate memory
				this->_free();

				// assign data
				_data = other._data;
				_ref  = other._ref;

				// check for valid data
				if (_data == nullptr)
					return *this;

				// increment reference counter
				++(*_ref);

				// done
				return *this;
			}

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				// check for self assignment
				if (this == &other)
					return *this;

				// deallocate memory
				this->_free();

				// move data
				_data = other._data;
				_ref  = other._ref;

				// invalidate other
				other._data = nullptr;
				other._ref  = nullptr;

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

			/* arrow operator */
			auto operator->(void) noexcept -> T* {
				return _data;
			}

			/* const arrow operator */
			auto operator->(void) const noexcept -> const T* {
				return _data;
			}

			/* dereference operator */
			auto operator*(void) noexcept -> T& {
				return *_data;
			}

			/* const dereference operator */
			auto operator*(void) const noexcept -> const T& {
				return *_data;
			}

	}; // class shared_ptr


	// -- non-member functions ------------------------------------------------

	/* make shared */
	template <typename T, typename... Ts>
	auto make_shared(Ts&& ...args) -> sm::shared_ptr<T> {

		// allocate data
		auto data = sm::malloc<T>();

		// make data guard
		sm::malloc_guard dguard{data};

		// allocate reference counter
		auto ref = sm::malloc<sm::usize>();

		// make reference guard
		sm::malloc_guard rguard{ref};

		// construct object
		new (data) T{std::forward<Ts>(args)...};

		// initialize reference counter
		*ref = 1U;

		// release guards
		dguard.complete();
		rguard.complete();

		// return shared pointer
		return sm::shared_ptr<T>{data, ref};
	}

} // namespace sm

#endif // shared_ptr_hpp
