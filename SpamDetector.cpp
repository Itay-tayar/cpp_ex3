//
// Created by Itay on 18/01/2020.
//

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include "HashMap.hpp"

namespace fs = boost::filesystem;

#define INVALID_USAGE std::cerr << "Usage: SpamDetector <database path> <massage path> <threshold>" << std::endl;
#define INVALID_INPUT std::cerr << "Invalid input" << std::endl;
#define EXIT exit(EXIT_FAILURE);

#define NOT_SPAM std::cout << "NOT_SPAM" << std::endl

#define SPAM std::cout << "SPAM" << std::endl

static const int CORRECT_NUM_OF_ARGS = 4;

/**
 * check if the given file exists
 * @param path1 first file to check
 * @param path2 second file to check
 */
void fileValidation(const std::string &path1, const std::string &path2)
{
    if (!fs::exists(path1) || !fs::exists(path2))
    {
        INVALID_INPUT
        EXIT
    }
}

/**
 * Converting a given string to int if possible, if not then exit.
 * @param s string to convert
 * @return converted integer
 */
int convertStrToInt(const std::string &s)
{
    try
    {
        int i = std::stoi(s);
        if (i < 0)
        {
            INVALID_INPUT
            EXIT
        }
        return i;
    }
    catch (std::invalid_argument const &e)
    {
        INVALID_INPUT
        EXIT
    }
}

/**
 * Convert a given string to lower case string
 * @param str string to convert
 * @return converted string
 */
std::string &convertToLower(std::string &str)
{
    for (auto &c: str)
    {
        c = tolower(c);
    }
    return str;
}

/**
 * load the database of bad words and their score into hash map
 * @param path string of the path to database
 * @return hash map with bad words as key and their score as value
 */
HashMap<std::string, int> createDB(const std::string &path)
{
    std::ifstream database(path);
    if (!database.is_open())
    {
        INVALID_INPUT
        EXIT
    }
    std::string line;
    std::vector<std::string> keys;
    std::vector<int> values;
    typedef boost::tokenizer< boost::char_separator<char>> tokenizer;

    while (getline(database, line)) // get each line in the database
    {
        boost::char_separator<char> sep{","};
        tokenizer tok{line, sep};
        std::vector<std::string> vec =  std::vector<std::string>( tok.begin(), tok.end());
        if (vec.size() != 2)
        {
            database.close();
            INVALID_INPUT
            EXIT
        }
        keys.push_back(convertToLower(vec[0])); //Insert the bad word to key vector
        values.push_back(convertStrToInt(vec[1])); //Insert the score to value vector
    }
    database.close();
    HashMap<std::string, int> badWordMap = HashMap<std::string, int>(keys, values);
    return badWordMap;
}

/**
 * load the message and search for the the bad words that in the dataset.
 * Then sum all of the scores of the bad words in the message.
 * @param path1 path string of the database
 * @param path2 path string of the message
 * @return The total score of the the bad words in the message
 */
int calculateSumOfBadWords(const std::string &path1, const std::string &path2)
{
    HashMap<std::string, int> badWords= createDB(path1);
    int sumOfBadWords = 0;
    std::string line;
    std::ifstream message(path2);
    if (!message.is_open())
    {
        message.close();
        INVALID_INPUT
        EXIT
    }
    while (getline(message, line))
    {
        line = convertToLower(line);
        for (auto & pair : badWords)
        {
            std::string key = pair.first;
            size_t pos = line.find(key);
            while( pos != std::string::npos) // Repeat till end is reached
            {
                sumOfBadWords += pair.second;
                pos =line.find(key, pos + key.size());
                // Get the next occurrence from the current position
            }
        }
    }
    message.close();
    return sumOfBadWords;

}

/**
 * the main function of the program
 * @param argc number of arguments in the input
 * @param argv array arguments in the input
 * @return EXIT_SUCCESS if the program succeeded, EXIT_FAILURE otherwise.
 */
int main (int argc, char *argv[])
{
    if (argc != CORRECT_NUM_OF_ARGS)
    {
        INVALID_USAGE
        EXIT
    }
    fileValidation(argv[1], argv[2]);
    int threshold = convertStrToInt(argv[3]);
    if (threshold == 0)
    {
        INVALID_INPUT
        EXIT
    }
    int sumOfBadWords = calculateSumOfBadWords(argv[1], argv[2]);
    if (sumOfBadWords >= threshold)
    {
        SPAM;
    } 
    else
    {
        NOT_SPAM;
    }
    return EXIT_SUCCESS;
}