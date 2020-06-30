// Henry Cevallos
//This program will run a spell checker on the document passed
//This spellchecker will use a hash table to reference each word in
//in the document to a dictionary. The program will suggest corrections
//for every misspelled word

#include "double_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//erases any punctuations at the front or end of the string passed and sets it to all lowercase
void format(string& line)
{
  if(ispunct(line[0])) //gets rid of punctuation at beggining
    line.erase(0,1);
    
  if(ispunct(line[line.length()-1]))  //get rid of punctuation at end
    line.erase(line.length()-1,1);   
        
  for(char& ch : line)  //changes every char in line to lowercase
    ch = tolower(ch);
}

//This function will spell check the document passed
int testSpellingWrapper(int argument_count, char** argument_list) {

  const string document_filename(argument_list[1]);
  const string dictionary_filename(argument_list[2]);
  
  HashTableDouble<string> hash_table; //create hash table
  fstream input(dictionary_filename); //opens dictionary file
  string data;
  while(input >> data)
  {
    format(data); //makes sure that every word follows format
    hash_table.Insert(data);  //Insert the string into the hash table
  }
  input.close();  //close dictionary file
  input.open(document_filename);  //open doocument file
  
  while(input >> data)
  {
    format(data); //makes sure every word follows format
    
    if(!hash_table.Contains(data))  //If the word is not found in the dictionary hash table, then spell check
    { 
      cout << data << " is INCORRECT" << endl;
      
      //The following checks with a new letter, it makes a word
      for(int i = 0; i <= data.length(); i ++)  //Loops through the index of every char in string
      {
       for(int alphabet = 97; alphabet <= 122; alphabet++)  //loops through alphabet ASCII
        {
          string new_word = data;
          string new_letter(1, (char)alphabet); //creates string with a letter of alphabet
          new_word.insert(i, new_letter); //inserts letter at position i
          if(hash_table.Contains(new_word)) //checks if with the new letter, it makes a word
            cout << "** " << data << " -> " << new_word << " ** case A" << endl;
        }
      }

      //The following checks if with a letter removed, it makes a word
      for(int i = 0; i < data.length(); i ++)  //Loops through the index of every char in string
      {
       string new_word = data;
       new_word.erase(i,1); //Removes the char at position i of string
       if(hash_table.Contains(new_word))  //checks if with letter removed, it makes a word
            cout << "** " << data << " -> " << new_word << " ** case B" << endl;
      }

      //The following checks if with letters swapped, it makes a word
      for(int i = 0; i < data.length()-1; i ++)  //Loops through the index of every char in string
      {
       string new_word = data;
       swap(new_word[i], new_word[i+1]);  //swaps adjacent letters
       if(hash_table.Contains(new_word))  //checks if with letters swapped, it makes a word
            cout << "** " << data << " -> " << new_word << " ** case C" << endl;
      }
    }
    
   else cout << data << " is CORRECT" << endl;  //No errors found with string
  }
}



// Sample main for program spell_check.
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>" << endl;
    return 0;
  }
  
  testSpellingWrapper(argc, argv);

  return 0;
}
