#ifndef unique_dir_hpp
#define unique_dir_hpp

#include "diagnostics/exception.hpp"

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


	// -- D I R  E N T I T Y --------------------------------------------------

	class dir_entity final {


		// -- friends ---------------------------------------------------------

		/* unique_dir as friend */
		friend class unique_dir;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::dir_entity;


			// -- private members ---------------------------------------------

			/* entity */
			struct ::dirent* _entity;


			// -- private lifecycle -------------------------------------------

			/* deleted default constructor */
			dir_entity(void) = delete;

			/* dirent constructor */
			dir_entity(struct ::dirent* entity)
			: _entity{entity} {
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* copy constructor */
			dir_entity(const self&) noexcept = default;

			/* move constructor */
			dir_entity(self&&) noexcept = default;

			/* destructor */
			~dir_entity(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* is regular */
			auto is_regular(void) const noexcept -> bool {
				return _entity->d_type == DT_REG;
			}

			/* is directory */
			auto is_directory(void) const noexcept -> bool {
				return _entity->d_type == DT_DIR;
			}

			/* is symbolic link */
			auto is_symlink(void) const noexcept -> bool {
				return _entity->d_type == DT_LNK;
			}

			/* name */
			auto name(void) const noexcept -> const char* {
				return _entity->d_name;
			}

			/* is current or parent */
			auto is_current_or_parent(void) const noexcept -> bool {

				// check for . or ..
				return  _entity->d_name[0U] == '.'
					&& (_entity->d_name[1U] == '\0'
					|| (_entity->d_name[1U] == '.'
					&&  _entity->d_name[2U] == '\0'));
			}


	}; // class dir_entity

} // namespace sm



#endif // unique_dir_hpp
