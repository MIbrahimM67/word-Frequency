#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <fstream>
std::string  punctuationRemover(const std::string& Word)
{
    std::string FixedWord;
    for (int i = 0; i < Word.length(); i++)
    {
        if (Word[i] != ',' && Word[i] != '!' && Word[i] != '&' && Word[i] != '@' && Word[i] != '(' && Word[i] != ')' &&
            Word[i] != '-' && Word[i] != ':' && Word[i] != ';' && Word[i] != '/' && Word[i] != '?' && Word[i] != '+' &&
            Word[i] != '\'' && Word[i] != '.' && Word[i] != '"' && Word[i] != '[' && Word[i] != ']' && Word[i] != '{' &&
            Word[i] != '}' && Word[i] != '<' && Word[i] != '>' && Word[i] != '*' && Word[i] != '%' && Word[i] != '^' &&
            Word[i] != '=' && Word[i] != '#' && Word[i] != '~' && Word[i] != '`' && Word[i] != '|' && Word[i] != '\\')
        {
            FixedWord += Word[i];
        }
    }
    return FixedWord;

}
std::string lowercaseConvertor(const std::string& Word)
{
    //int Code=0;
    std::string FixedWord;
    for (int i = 0; i < Word.length(); i++)
    {   
        if (Word[i] >= 'A' && Word[i] <= 'Z') 
        {
            FixedWord += Word[i] + 32; 
        }
        else
        {
            FixedWord += Word[i]; 
        }
    }
    return FixedWord;
}
void StopwordsLoader(std::map<std::string, int>& StopWords)
{
    std::fstream Loader;
    std::string word;
    Loader.open("stopwords.txt", std::ios::in);
    if (Loader.is_open())
    {
        while (Loader.good())
        {
            Loader >> word;
            StopWords[word] = StopWords[word] + 1;
        }
    }
    else
        std::cout << "Stop words cant be loaded, please make sure the file is placed in the same directory " << std::endl;

}
bool StopWordFound( std::map<std::string, int>& StopWords, const std::string& word)  // checks if the string contains stop word or not
{
    std::map<std::string, int>::iterator It;
    It = StopWords.find(word);
    if (It == StopWords.end())
    {
        return false;
    }
    else
        return true;

}

void WordCloudGenerator(std::map<std::string, std::map<std::string,int>>& FrequencyTree)
{
   std:: fstream cloud;
   int Freq=0;
   cloud.open("Cloud.html", std::ios::out);
   cloud <<
       "<head>\n"
       << "  <meta charset=\"UTF-8\">\n"        // writing  the basic html page starting format
       << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
       << "  <title>Word Cloud</title>\n"
       << "  <style>\n"   // styling our words
       << "  .word-cloud {\n"
       << "      display: flex;\n"
       << "      flex-wrap: wrap;\n"
       << "      gap: 10px;\n"
       << "    }\n"
       << "    .word {\n"
       << "      font-size: 16px;\n"
       << "    }\n"
       << "  </style>\n"
       << "</head>\n"
       << "<body style=\"background-color:black;\">\n"
       << "  <div class=\"word-cloud\">\n";  // creating a div 
   std::map<std::string, std::map<std::string, int>> :: iterator It;
   std::map<std::string, int>::iterator It2 ;
   It = FrequencyTree.begin();
   
   while (It != FrequencyTree.end())
   {
       It2 = It->second.begin(); // points to the map inside the  main map i.e, it points to the vairations map

       while (It2 != It->second.end())
       {
           Freq = 2*(It2->second) + 16; // calculating the font size of the words based on their frequency
           if (Freq >= 18 && Freq < 25)
           {
               cloud << "    <span class=\"word\" style=\"font-size: " << Freq << "px;color:yellow;\">" << It2->first << "</span>\n";

           }
           else if (Freq >= 25 && Freq < 35)
           {
               cloud << "    <span class=\"word\" style=\"font-size: " << Freq << "px;color:blue;\">" << It2->first << "</span>\n";
           }
           else if (Freq >= 35 && Freq < 45)
           {
               cloud << "    <span class=\"word\" style=\"font-size: " << Freq << "px;color:crimson;\">" << It2->first << "</span>\n";
           }
           else if (Freq >= 45 && Freq < 55)
           {
               cloud << "    <span class=\"word\" style=\"font-size: " << Freq << "px;color:red;\">" << It2->first << "</span>\n";

           }
           else if (Freq >= 55)
           {
               cloud << "    <span class=\"word\" style=\"font-size: " << Freq << "px;color:darkred;\">" << It2->first << "</span>\n";

           }
           else
           {
               cloud << "    <span class=\"word\" style=\"font-size: " << Freq << "px;color:white;\">" << It2->first << "</span>\n";

           }

           ++It2;
       }
       ++It;

   }

   cloud << "  </div>\n" // closing of the html page
       << "</body>\n"
       << "</html>\n";
   std::cout << "Cloud Generated!" << std::endl;;

}

int main(int argc,char * argv[])
{  
    std::map<std::string,std:: map<std::string,int>> FrequencyTree;
    std::map<std::string, int> StopWords;
    std::fstream file;
    std::string filename;
    std::string word;
    StopwordsLoader(StopWords);
    if (argc == 2)
    {
        filename= argv[1];
    }
    else
    {
        std::cout << "Enter the File name" << std::endl;
        std::cin >> filename;
    }
    std::string VarationsConserver = "";
    std::map<std::string, std::map<std::string, int>>::iterator it;
    file.open(filename, std::ios::in);
    if (file.is_open())
    {
        while (file.good())
        {   
            file >> word;
            word = punctuationRemover(word);
            VarationsConserver = word; // storing the orignal form of the word 
            word = lowercaseConvertor(word);
            if (!(StopWordFound(StopWords, word)))
            {
                        
                    FrequencyTree[word][VarationsConserver] ++; // pushing it into the nested map structure
                   // bonus task 3 since the varaitions of the words are conserved
            }           
        }
        // writing it into the excel form
        std::fstream Output;
        Output.open("Result.csv", std::ios::out);
        int counter=0;
        if (Output.is_open())
        {
            std::map<std::string, std:: map<std::string,int>>::iterator It;
            std::map<std::string, int>::iterator It2;
            It = FrequencyTree.begin();
            It2 = It->second.begin();
            Output << "LOWER CASE FORM";
            Output << std::endl;
            while (It != FrequencyTree.end())
            {
                counter = 0;
                It2 = It->second.begin();
                Output << It->first << ",";
                while (It2 != It->second.end())
                {
                   counter += It2->second;
                    Output << It2->first << "," << It2->second << ",";
                    It2++;
                }
                Output <<"Total="<< counter << std::endl; // keeping a counter to calculate the sum of all the variations to calculate the total
               Output << std::endl;
                ++It;
            }
        }
        std::cout << "Excel Sheet Generated!" << std:: endl;
        WordCloudGenerator(FrequencyTree); // bonus task 2

    }
    else
    {
        std::cout << "File Not Found!" << std::endl;
    }
 
        
}

