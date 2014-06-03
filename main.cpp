#include <iostream>
#include <string>

#include "vigenere.hpp"
#include "ascii.hpp"
#include "clo.hpp"
#include <typeinfo> // typeid

using namespace std;

int main(int argc,char* argv[])
{

    try
    {
        CLO Options(argc, argv);

        string input = Options.Input();
        string output;

        string key = Options.key();

        if(Options.Flag(CLO::HELP))
        {
            cout << Options.LoadFromFile("aide.txt") << endl;
        }

        if(Options.Flag(CLO::ENCIPHER))
        {
            cout << "Encipher..." << endl;
            output = vigenere::encipher(ascii::fr_ref, input, key);
        }
        else if(Options.Flag(CLO::DECIPHER))
        {
            cout << "Decipher..." << endl;
            if(Options.Flag(CLO::KEY_SIZE)) {
                key = vigenere::break_key(ascii::fr_ref, input, Options.key_size());
            }
            else if(Options.Flag(CLO::UNKNOWN_KEY)) {
                key = vigenere::break_key(ascii::fr_ref, input);
            }
            output = vigenere::decipher(ascii::fr_ref, input, key);
        }

        if(Options.Flag(CLO::OUTPUT_FILE))
        {
            Options.SaveInFile(Options.OutputFilename(),output);
            cout << "Output saved in the following file : " << Options.OutputFilename() << "." << endl;
        }

        cout << "Key size : " << key.size() << endl;
        cout << "Key : " << key << endl;

        if(Options.Flag(CLO::VERBOSE))
        {
            cout << "Output : " << output << endl;
        }
    }
    catch(const std::exception &E)
    {
        std::cout << "\n> Error : " << E.what() << "\n"
                  << "> Exception : " << typeid(E).name() << std::endl;

        return 1;
    }
    catch(...)
    {
        std::cout << "\n> Unknown error." << std::endl;

        return 1;
    }


	return 0;
}
