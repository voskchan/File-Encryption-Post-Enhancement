// Encryption.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>


#include <filesystem>
#include <random>







/// <summary>
/// encrypt or decrypt a source string using the provided key
/// </summary>
/// <param name="source">input string to process</param>
/// <param name="key">key to use in encryption / decryption</param>
/// <returns>transformed string</returns>
std::string encrypt_decrypt(const std::string& source, const std::string& key)
{
  // get lengths now instead of calling the function every time.
  // this would have most likely been inlined by the compiler, but design for perfomance.
  const auto key_length = key.length();
  const auto source_length = source.length();

  // assert that our input data is good
  assert(key_length > 0);
  assert(source_length > 0);

  std::string output = source;

  // loop through the source string char by char
  for (size_t i = 0; i < source_length; ++i)
  { // TODO: student need to change the next line from output[i] = source[i]
    // transform each character based on an xor of the key modded constrained to key length using a mod

      //modulus for key length is no longer required as key will be the same length as input
      //left in cases of potential break
      output[i] = source[i] ^ key[i % key_length];
  }

  // our output length must equal our source length
  assert(output.length() == source_length);

  // return the transformed string
  return output;
}

std::string read_file(const std::string& filename)
{
  std::string file_text = "John Q. Smith\nThis is my test string";

  // TODO: implement loading the file into a string

  std::ifstream aFile(filename);

  std::getline(aFile, file_text, '\0');


  return file_text;
}

std::string get_student_name(const std::string& string_data)
{
  std::string student_name;

  // find the first newline
  size_t pos = string_data.find('\n');
  // did we find a newline
  if (pos != std::string::npos)
  { // we did, so copy that substring as the student name
    student_name = string_data.substr(0, pos);
  }

  return student_name;
}

void save_data_file(const std::string& filename, const std::string& student_name, const std::string& data)
{
  //  TODO: implement file saving
  //  file format
  //  Line 1: student name
  //  Line 2: timestamp (yyyy-mm-dd)
  //  Line 4+: data

  std::fstream saveFile;

  saveFile.open(filename, std::ios::out);

  time_t my_time = time(NULL);


  if (saveFile.is_open()) {
      std::cout << "File is open\n";

      saveFile << "Student Name: " << student_name << "\n";

#pragma warning(suppress : 4996) // suppress depreciation warning for ctime
      saveFile << "Time: " << ctime(&my_time);

      saveFile << "Data: " << data << "\n";

      saveFile.close();
  }
  else {
      std::cout << "Error in file opening";

  }

}


//Method included to seperate key from encrypted data file
//would ideally be stored in place seperate from encrypted data
void export_key_to_file(const std::string& filename, const std::string& key) {
    
    std::fstream keyFile;

    keyFile.open(filename, std::ios::out);

    if (keyFile.is_open()) {
        std::cout << "Key file open\n";

        keyFile << key;

        keyFile.close();
    }
    else {
        std::cout << "Error in file opening";
    }
}

//Used to generate a random key the length of the inputted data for each use. 
//Creates a highly secure key as each char in input file is matched to a key char for XOR cipher
std::string createKeyOfLength(int length) {

    //set a list of useable characters for the string
    const std::string useChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    //used to create a random number generator that is more random than rand
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(0, useChars.size() - 1);

    std::string key;

    for (int i = 0; i < length; i++) {
        key += useChars[distribution(generator)];
    }

    return key;
}

int main()
{
  std::cout << "Encyption Decryption Test!" << std::endl;

  // input file format
  // Line 1: <students name>
  // Line 2: <Lorem Ipsum Generator website used> https://pirateipsum.me/ (could be https://www.lipsum.com/ or one of https://www.shopify.com/partners/blog/79940998-15-funny-lorem-ipsum-generators-to-shake-up-your-design-mockups)
  // Lines 3+: <lorem ipsum generated with 3 paragraphs> 
  //  Fire in the hole bowsprit Jack Tar gally holystone sloop grog heave to grapple Sea Legs. Gally hearties case shot crimp spirits pillage galleon chase guns skysail yo-ho-ho. Jury mast coxswain measured fer yer chains man-of-war Privateer yardarm aft handsomely Jolly Roger mutiny.
  //  Hulk coffer doubloon Shiver me timbers long clothes skysail Nelsons folly reef sails Jack Tar Davy Jones' Locker. Splice the main brace ye fathom me bilge water walk the plank bowsprit gun Blimey wench. Parrel Gold Road clap of thunder Shiver me timbers hempen halter yardarm grapple wench bilged on her anchor American Main.
  //  Brigantine coxswain interloper jolly boat heave down cutlass crow's nest wherry dance the hempen jig spirits. Interloper Sea Legs plunder shrouds knave sloop run a shot across the bow Jack Ketch mutiny barkadeer. Heave to gun matey Arr draft jolly boat marooned Cat o'nine tails topsail Blimey.

  const std::string file_name = "inputdatafile.txt";
  const std::string encrypted_file_name = "encrypteddatafile.txt";
  const std::string decrypted_file_name = "decrytpteddatafile.txt";
  const std::string genKey_file_name = "generatedkeyfile.txt";


  //system now checks if file exists before continuing with program, prevents crashes from file not existing
  if (std::filesystem::exists(file_name)) {

      std::cout << "File exists." << std::endl;

      const std::string source_string = read_file(file_name);


      const std::string key = createKeyOfLength(source_string.length());

      std::cout << "generated key: " << key << std::endl;

      export_key_to_file(genKey_file_name, key);
      // get the student name from the data file
      const std::string student_name = get_student_name(source_string);

      // encrypt sourceString with key
      const std::string encrypted_string = encrypt_decrypt(source_string, key);

      // save encrypted_string to file
      save_data_file(encrypted_file_name, student_name, encrypted_string);

      // decrypt encryptedString with key
      const std::string decrypted_string = encrypt_decrypt(encrypted_string, key);

      // save decrypted_string to file
      save_data_file(decrypted_file_name, student_name, decrypted_string);

      std::cout << "Read File: " << file_name << " - Encrypted To: " << encrypted_file_name << " - Decrypted To: " << decrypted_file_name << std::endl;
  }
  else {
      std::cout << "Input file could not be found";
  }

  // students submit input file, encrypted file, decrypted file, source code file, and key used
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
