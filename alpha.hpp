#ifndef HPP_ALPHA
#define HPP_ALPHA

struct Alpha {
	static int alpha_index(int input) = 0;
	static int alpha_at(int input) = 0;
	static int shift_alpha(int input, int key) = 0;
	static int inverse_alpha(int input) = 0;
	static bool isalpha(int input) = 0;
};

#endif // HPP_ALPHA
