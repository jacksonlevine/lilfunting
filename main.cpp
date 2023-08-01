#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <functional>

enum OutType {
  ERR_OUT,
  RES_OUT
};

class Out {
public:
  OutType type;
  std::string value;
};

std::map<std::string, std::function<Out(std::vector<std::string>, int)>> funcs;

std::map<std::string, std::string> variables;


std::string pop_first_word(std::string& str) {
    std::istringstream stream(str);
    std::string firstWord;
    if(str.find(' ') != std::string::npos)
    {
      if (stream >> firstWord) {
          str = stream.str().substr(stream.tellg());
          if (str[0] == ' ') { // remove leading space
              str.erase(0, 1);
          }
      } else {
          str.clear();
      }
    } else {
      std::string st(str);
      str.clear();
      return st;
    }
    return firstWord;
}

void interpret_file(const char* filename) {
  std::ifstream file(filename);
  std::stringstream buf;
  buf << file.rdbuf();

  const char* c = ((buf.str() + ' ') + '\0').c_str();
  int ind = 0;

  bool the_end = false;
  while(!the_end) {
    std::string thisline = "";
    if(c[ind] == '\n' && ind < buf.str().length() - 1)
    {
      ind += 1;
    }
    while(c[ind] != '\n' && c[ind] != '\0')
    {
      thisline.push_back(c[ind]);
      ind++;
    }
    //std::cout << thisline << std::endl;
    if(thisline.size() > 4)
    {


      std::vector<std::string> tokens;
      while(thisline.size() > 0)
      {
        tokens.push_back(pop_first_word(thisline));
      }
      if(tokens.size() > 0)
      {

      
        std::string command = tokens[0];

        //Replace all .variables with their literal values

        for(int i = 0; i < tokens.size(); i++) {
          std::string& s = tokens[i];
          if(*s.begin() == '.') {
            std::string new_s = s;
            new_s.erase(new_s.begin());
            if(variables.find(new_s) != variables.end()) {
              std::string replacement = variables[new_s];
              tokens[i] = replacement;
            } else {
              std::cerr << (std::string("Attempted to use reference to non-existent variable \"") + new_s) + "\"." << std::endl;
            }
              
          }
        }


        if(funcs.find(command) == funcs.end()) {
          std::cerr << ((std::string("Attempted to use non-existent function ") + "\"" ) + command ) + "\"" << std::endl;
        } else {
          //Call the function with the rest of the string arguments
          tokens.erase(tokens.begin());

          funcs[command](tokens, tokens.size());
        }
      }
    }

    if(c[ind] == '\0')
    {
      the_end = true;
    }
  }
}


int main() {

    
    funcs["let"] = [](std::vector<std::string> args, int size){
      Out result;
      result.type = RES_OUT;
      result.value = "success";

      if(size != 2)
      {
        result.type = ERR_OUT;
        result.value = std::string("\'let\' takes 2 arguments. You provided ") + std::to_string(size);
        return result;
      }

      std::string var = args[0];
      std::string val = args[1];

      if(variables.find(var) != variables.end())
      {
        result.type = ERR_OUT;
        result.value = (std::string("Attempted to define variable ") + var) + " more than once.";
        return result;
      }

      variables[var] = val;
      return result;
    };
    funcs["set"] = [](std::vector<std::string> args, int size){
      Out result;
      result.type = RES_OUT;
      result.value = "success";

      if(size != 2)
      {
        result.type = ERR_OUT;
        result.value = std::string("\'set\' takes 2 arguments. You provided ") + std::to_string(size);
        return result;
      }

      std::string var = args[0];
      std::string val = args[1];

      if(variables.find(var) != variables.end())
      {
        variables[var] = val;
      } else {
        result.type = ERR_OUT;
        result.value = (std::string("Attempted to set non-existent variable ") + var) + ".";
        return result;
      }

      return result;
    };
    funcs["print"] = [](std::vector<std::string> args, int size){

      Out result;
      result.type = RES_OUT;
      result.value = "success";

      if(size != 1)
      {
        result.type = ERR_OUT;
        result.value = std::string("\'print\' takes 1 argument. You provided ") + std::to_string(size);
        return result;
      }

      std::string print_this = args[0];

      std::cout << print_this << std::endl;
      return result;
    };


  interpret_file("./rando.txt");


  



  std::string _;
  std::cin >> _;
  return 1;
}


