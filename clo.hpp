#ifndef H_CLOPTION
#define H_CLOPTION

#include <iostream> //std::cout
#include <cstdlib> // strtol
#include <fstream> // std::ifstream, std::ofstream
#include <sstream> // std::ostringstream
#include <string> // std::string
#include <stdexcept> // std::logic_error, std::runtime_error

//////////////////////////////////////////////////////
// Minimalistic command line options parser for this program.
//////////////////////////////////////////////////////
class CLO
{
    public:

	CLO(int argc, char *argv[])
	: options_flags(0)
	{
		std::string Buf;
		for(int i = 1; i < argc;)
		{
			Buf = argv[i];

			/* Input File*/
			if(Buf == "-i")
			{
				input = ""; // ignore any previous input
				if(Flag(INPUT_FILE))
					throw std::logic_error(
						"The -i option has been found more than once.");

				if(i+1 < argc && argv[i+1][0] != '-')
					input=LoadFromFile(argv[++i]);
				else
					throw std::logic_error(
						"Missing input file name.");

				options_flags |= INPUT_FILE;
			}

			/* Output File */
			if(Buf == "-o")
			{
				if(Flag(OUTPUT_FILE))
					throw std::logic_error(
						"The -o option has been found more than once.");
				if(i+1 < argc && argv[i+1][0] != '-')
					output_filename = argv[++i];
				else
					throw std::logic_error(
						"Missing output file name.");

				options_flags |= OUTPUT_FILE;
			}

			/* ENCIPHER */
			else if(Buf == "-c")
			{
				if(Flag(ENCIPHER) || Flag(DECIPHER))
					throw std::logic_error(
						"The -c and/or -d options have been found more than once.");

				if(i+1 < argc && argv[i+1][0] != '-')
					key_ = argv[++i];
				else
					throw std::logic_error(
						"Missing ciphers's key.");

				options_flags |= ENCIPHER;
			}

			/* DECIPHER */
			else if(Buf == "-d")
			{
				if(Flag(ENCIPHER) || Flag(DECIPHER))
					throw std::logic_error(
						"The -c and/or -d options have been found more than once.");
				if((i+1 < argc) && (argv[i+1][0] != '-'))
				{

					char* endptr = 0;
					key_size_ = strtol(argv[++i],&endptr,10);
					if(endptr!=argv[i] && key_size_ != 0)
					{
						options_flags |= KEY_SIZE;
						std::cout << "The key size is known : ";
						std::cout << "data cryptanalysis and deduction of the key." << std::endl;
					}
					else
						key_ = argv[i];
				}
				else
				{
					options_flags |= UNKNOWN_KEY;
						std::cout << "The key size is unknown : ";
						std::cout << "data cryptanalysis and deduction of the key." << std::endl;
				}

				options_flags |= DECIPHER;
			}
			else if(Buf == "-v")
			{
				options_flags |= VERBOSE;
			}
			else if(Buf == "-h")
			{
				options_flags |= HELP;
			}

			/* Append to input */
			else if(!Flag(INPUT_FILE))
				input += Buf;

			++i;
		}

	   /* Check */
	   if(input.empty())
			throw std::logic_error("Empty input.");

	   if(((options_flags & ENCIPHER) == 0) && ((options_flags & DECIPHER) == 0))
			throw std::logic_error("None of -c or -d options found.");
	}

	const std::string &Input(void) const
	{
		return input;
	}

	std::string key(void) const
	{
		return key_;
	}
	
	size_t key_size() const
	{
		return key_size_;
	}

	 const std::string &OutputFilename(void) const
	{
		return output_filename;
	}

	unsigned int GetFlags(void) const
	{
		return options_flags;
	}

	bool Flag(const int f)
	{
		return ((options_flags & f) != 0);
	}

	std::string LoadFromFile(const std::string &Name)
	{
		std::ifstream Ifs(Name.c_str());
		if(!Ifs)
			throw std::runtime_error(
				"Can't open input file.");
		std::ostringstream Oss;
		Oss << Ifs.rdbuf();
		return Oss.str();
	}

	void SaveInFile(const std::string &Name, const std::string &String)
	{
	    std::ofstream Ofs(Name.c_str());
	    if(!Ofs)
	        throw std::runtime_error("Can't open output file.");
	    Ofs << String;
	}

	static const int ENCIPHER ;
	static const int DECIPHER;
	static const int KEY_SIZE;
	static const int UNKNOWN_KEY;
	static const int HELP;
	static const int INPUT_FILE;
	static const int OUTPUT_FILE;
	static const int VERBOSE;

    private:

	std::string input;
	std::string key_;
	std::string output_filename;
	size_t 		key_size_;
	unsigned int options_flags;


};

const int CLO::ENCIPHER 		= 1;
const int CLO::DECIPHER 	= 1 << 1;
const int CLO::KEY_SIZE 	= 1 << 2;
const int CLO::UNKNOWN_KEY 	= 1 << 3;
const int CLO::HELP 		= 1 << 4;
const int CLO::INPUT_FILE 	= 1 << 5;
const int CLO::OUTPUT_FILE 	= 1 << 6;
const int CLO::VERBOSE 		= 1 << 7;

#endif
