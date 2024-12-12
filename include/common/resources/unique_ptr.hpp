#ifndef UNIQUE_PTR_HPP
# define UNIQUE_PTR_HPP

# include "common/memory/malloc.hpp"

namespace sm {

template <typename T>
class unique_ptr {

	private:

		// -- private types ---------------------------------------------------

		/* self type */
		using self = sm::unique_ptr<T>;

		T* _data;

		unique_ptr(T* data) noexcept
		: _data{data} {
		};

		void _free(void) noexcept {
			if (_data) {
				_data->~T();
				sm::free(_data);
			}
		}

	public:

		unique_ptr(void) noexcept
		: _data{nullptr} {
		};

		~unique_ptr(void) noexcept {
			_free();
		};

		unique_ptr(const self&) = delete;

		unique_ptr(self&& other) noexcept
		: _data{other._data} {
			other._data = nullptr;
		};

		auto operator=(const self&) = delete;

		auto operator=(self&& other) noexcept -> self& {
			if (this == &other)
				return *this;
			_free();
			_data = other._data;
			other._data = nullptr;
			return *this;
		};

		auto operator->(void) noexcept -> T* {
			return _data;
		};

		explicit operator bool (void) {
			return (_data != nullptr);
		};

		
	template <typename U, typename ...Ts>
	friend auto make_unique(Ts&& ...) -> unique_ptr<U>;
};


	template <typename T, typename ...Ts>
	auto make_unique(Ts&& ...args) -> unique_ptr<T> {
		T* data = sm::malloc<T>(1);
		new(data) T{std::forward<Ts>(args)...};
		return unique_ptr<T>(data);
	};

} // namespace sm

#endif // UNIQUE_PTR_HPP
