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

class FunctionCall
{
public:
  std::string func;
  std::vector<std::string> args;
};

std::map<std::string, std::function<Out(std::vector<std::string>, int)>> funcs;

std::map<std::string, std::string> variables;

std::vector<FunctionCall> all_tokens;

void interpret_file(const char* filename) {
  std::ifstream file(filename);
  std::stringstream buf;
  buf << file.rdbuf();

  std::string code = buf.str();

  std::istringstream stream(code);
      std::string line;
      while (std::getline(stream, line)) {
          std::istringstream lineStream(line);
          std::string word;
          int index = 0;
          FunctionCall* currFuncCall = 0;
          while (lineStream >> word) {
              //std::cout << word;
              //std::cout << " ";
              if(index == 0)
              {
                FunctionCall f;
                f.func = word;
                all_tokens.push_back(f);
                currFuncCall = &(all_tokens.back());
              } else {
                currFuncCall->args.push_back(word);
              }

              index += 1;
          }
          if(currFuncCall->func.front() == '.')
          {
            currFuncCall->func = variables[currFuncCall->func.substr(1)];
          }
          for(int i = 0; i < currFuncCall->args.size(); i++)
          {
            if(currFuncCall->args[i].front() == '.') {
              currFuncCall->args[i] = variables[currFuncCall->args[i].substr(1)];
            }
          }

          funcs[currFuncCall->func](currFuncCall->args, currFuncCall->args.size());
          //std::cout << std::endl; 
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


