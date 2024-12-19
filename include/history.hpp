#ifndef history_hpp
#define history_hpp

#include <vector>
#include <string>
#include <unordered_set>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- H I S T O R Y -------------------------------------------------------

	class history final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::history;


			// -- private members ---------------------------------------------

			/* history */
			std::vector<std::string> _history;

			/* set */
			std::unordered_set<std::string> _set;

			/* history position */
			std::size_t _history_pos;



		public:

			/* default constructor */
			history(void)
			: _history{}, _set{}, _history_pos{0U} {
			}

			/* destructor */
			~history(void) noexcept = default;



			// -- public modifiers --------------------------------------------

			/* add */
			auto add(const std::string& line) -> void {

				_history_pos = _history.size();

				if (line.empty())
					return;

				if (_set.find(line) != _set.end())
					return;

				_history.push_back(line);
				_set.insert(line);
				++_history_pos;
			}


			// -- public operators --------------------------------------------

			/* ++operator */
			auto operator++(void) -> void {

				_history_pos += _history_pos < _history.size() ? 1U : 0U;
			}

			/* --operator */
			auto operator--(void) -> void {

				_history_pos -= _history_pos > 0U ? 1U : 0U;
			}


			// -- public accessors --------------------------------------------

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _history.empty();
			}

			/* at begin */
			auto at_begin(void) const noexcept -> bool {
				return _history_pos == _history.size();
			}

			/* current */
			auto current(void) const -> std::string {
				return _history[_history_pos];
			}

	}; // class history

} // namespace sm

#endif // history_hpp
