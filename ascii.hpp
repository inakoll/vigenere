#ifndef HPP_ASCII
#define HPP_ASCII

namespace ascii { 
	namespace details {
		inline int lowercase_at(int input) {
			return input + 'a';
		}

		inline int uppercase_at(int input) {
			return input + 'A';
		}

		inline int alpha_offset(int input) {
			return (input & 'a') | 1; // ascii 'bit hack' "offset = A or a" depending on input case
		}
		
		template<size_t size_>
		struct Alpha_imp {
		public:
			static int index(int input) 
			{
				return (input | 32) - 'a';
			}
			
			static int at(int input)
			{
				return ascii::details::uppercase_at(input);
			}
			
			static int shift(int input, int key)
			{
				return ((index(input) + index(key)) % size_ + alpha_offset(input));
			}
			
			static int inverse(int input)
			{
				return (size_ - index(input)) + alpha_offset(input);
			}
			
			static bool isalpha(int input)	
			{
				// kind of optimized version of std::isalpha for ascii letters (no locale support)
				return ((unsigned int)index(input)) < size_;
			}
			
			static constexpr size_t size() {return size_;}
			using frequencies_t = double (&)[size_];
			double ic() {return ic_;}
			frequencies_t frequencies() {return frequencies_;}
			
			double ic_;
			double frequencies_[size_];
		};
	}
}

namespace ascii {
using Alpha = ascii::details::Alpha_imp<26>;
const static Alpha fr_ref = 
		{0.0778, 
		{9.42,1.02,2.64,3.39,15.87,
		 0.95,1.04,0.77,8.41,0.89,
		 0.00,5.34,3.24,7.15,5.14,
		 2.86,1.06,6.46,7.90,7.26,
		 6.24,2.15,0.00,0.30,0.24,0.32}
		};
}

#endif // include guard
