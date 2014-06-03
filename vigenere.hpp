#ifndef HPP_VIGENERE
#define HPP_VIGENERE

#include <string>
#include <vector>
#include <cmath> 		// fabs
#include <algorithm> 	// for_each, rotate_copy, transform, min_element, fill
#include <iterator> 	// distance, begin, end
#include <numeric> 		// accumulate

namespace vigenere {

template <typename Alpha>							
class counter {
public:
	template <typename Iterator>
	counter(const Alpha& alpha, Iterator input_first, Iterator input_last, size_t key_size)
	: 
		alpha_(alpha),
		freqs_(key_size*Alpha::size(), 0),
		key_(key_size, 0),
		data_size_(0)
	{
		count(input_first, input_last);
	}
	
	template <typename Iterator>
	counter(const Alpha& alpha, Iterator input_first, Iterator input_last)
	:
		alpha_(alpha),
		freqs_(1*Alpha::size(), 0),
		key_(1, 0),
		data_size_(0)
    {
		size_t input_size = std::distance(input_first, input_last);
		count(input_first, input_last);
		while(key_.size() < input_size && !is_target_ic(ic(), alpha_.ic())) {
			resize(key_.size()+1);
			count(input_first, input_last);
		}
	}
	
private:
	template <typename Iterator>
	void count(Iterator input_first, Iterator input_last)
	{
		std::for_each(input_first, input_last, [this](char c) {
			if(Alpha::isalpha(c)) {
				add(c);
			}
		});
		normalize();
	}
	
	void resize(size_t new_size)
	{
		key_.resize(new_size);
		freqs_.resize(new_size*Alpha::size());
		std::fill(std::begin(freqs_), std::end(freqs_), 0);
		data_size_ = 0;
	}
	
	void add(char c)
	{
		++freqs(data_size_%key_.size(), Alpha::index(c));
		++data_size_;
	}
	
	void normalize()
	{
		data_size_ /= key_.size();
	}
	
	bool is_target_ic(double ic, double ref_ic)
	{
		return 0.07 * ref_ic > fabs(ic-ref_ic) || ic > ref_ic;
	}
	
public:
	size_t& freqs(size_t i, size_t j) 
	{
		return freqs_[i*Alpha::size()+j];
	}
	
	double ic()
	{
		double ic = 0.;
		for(size_t i = 0; i < key_.size(); ++i) {
			for(size_t j = 0; j < Alpha::size(); ++j) {
				ic += freqs(i, j) * (freqs(i, j) - 1);
			}
		}
		ic /= key_.size() * data_size_ * (data_size_ - 1);
		return ic;
	}
	
	std::string break_key()
	{
		std::vector<double> norms(Alpha::size(), 0.);
		std::vector<double> diff(Alpha::size(), 0.);
		for(size_t i = 0; i < key_.size(); ++i) {
			for(size_t j = 0; j < Alpha::size(); ++j) {
				std::rotate_copy(&freqs(i, 0), &freqs(i, j), 
							&freqs(i, Alpha::size()), diff.begin());
							
				std::transform(diff.begin(), diff.end(), alpha_.frequencies(), diff.begin(), 
					[=](double a, double b) {
						return (a/data_size_-b)*(a/data_size_-b);
					});
					
				norms[j] = std::accumulate(diff.begin(), diff.end(), 0.);
			}
			key_[i] = Alpha::at(std::distance(norms.begin(), 
													   std::min_element(norms.begin(), norms.end())));
		}
		return key_;
	}
	
private:
	Alpha alpha_;
	std::vector<size_t> freqs_;
	std::string key_;
	size_t data_size_;
};

template <typename Alpha, typename... Args>							
counter<Alpha> make_counter(const Alpha& alpha, Args&&... args)
{
	return counter<Alpha>(alpha, std::forward<Args>(args)...);
}

template <typename Alpha>		
std::string encipher(const Alpha&, const std::string& input, const std::string& key)
{
	std::string output = input;
	for(size_t i = 0, j = 0; i != input.size(); ++i) {
		if(Alpha::isalpha(input[i])) {
			output[i] = Alpha::shift(input[i], key[j++]);
			j %= key.size();
		}
	}
	return output;
}

template <typename Alpha>		
std::string decipher(const Alpha& alpha, const std::string& input, const std::string& key)
{
	std::string inversed_key(key.size(), 0);
	std::transform(key.begin(), key.end(), inversed_key.begin(), Alpha::inverse);
	return encipher(alpha, input, inversed_key);
}

template <typename Alpha>		
std::string break_key(const Alpha& alpha, const std::string& input)
{
	return make_counter(alpha, std::begin(input), std::end(input)).break_key();
}

template <typename Alpha>		
std::string break_key(const Alpha& alpha, const std::string& input, size_t key_size)
{
	return make_counter(alpha, std::begin(input), std::end(input), key_size).break_key();
}

} // namespace	 

#endif // include guard
