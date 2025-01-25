#include <iostream>
#include <string>
#include <vector>
int main(int argc, char* argv[]) {  
    // Separate numbers
    std::string input, cache;
    std::vector<std::string> numbers;
    for (int i = 1; i < argc; ++i) {
        numbers.push_back(argv[i]);
    }
    
    // Convert to int.
    int result = 0;
    for (int i = 0; i < numbers.size(); ++i) {
        result += std::stoi(numbers[i]);
    }
    std::cout << result;
    // Print result.
}