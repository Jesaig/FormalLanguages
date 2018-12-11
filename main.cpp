#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cstdio>

struct StackElement{
    std::vector<std::vector<int>> possibleWord;

    StackElement(size_t lengthOfTest){
        possibleWord = std::vector<std::vector<int>>(lengthOfTest, std::vector<int>(lengthOfTest + 1, 0));
    };
    ~StackElement() = default;
};

std::stack<StackElement> res; // stack is for parsing the expression
size_t sizeOfWord;
std::map<char, std::vector<int>> positions;
size_t lengthOfTheBiggestSuffix;
std::string test;

void callERROR(size_t numOfArguments){
    if(res.size() < numOfArguments){
        std::cout << "Incorrect input."
                  << std::endl
                  << "Check that all operators consist necessary quantity of arguments."
                  << std::endl;
        exit(1);
    }
}

void callERROR(){
    if(res.size() > 1){
        std::cout << "Incorrect input."
                  << std::endl
                  << "Check that all operators consist necessary quantity of arguments."
                  << std::endl;
        exit(1);
    }
}
void addition(std::string& test){
    callERROR(2);

    StackElement firstArgument = res.top();
    res.pop();
    StackElement secondArgument = res.top();
    res.pop();

    StackElement resultOfAddition(sizeOfWord);

    for (int i = 0; i < sizeOfWord; ++i) {
        for (int j = i + 1; j < sizeOfWord + 1; ++j) {
            if (firstArgument.possibleWord[i][j] || secondArgument.possibleWord[i][j]) {
                resultOfAddition.possibleWord[i][j] = 1;
            }
        }
    }

    res.push(resultOfAddition);
}


void concatenation(std::string& test){
    callERROR(2);

    StackElement secondArgument = res.top();
    res.pop();
    StackElement firstArgument = res.top();
    res.pop();
    StackElement resultOfConcatenation(sizeOfWord);

    for (size_t i = 0; i < sizeOfWord; ++i)
        for (size_t j = i; j < sizeOfWord + 1; ++j){
            if (i == j
                && firstArgument.possibleWord[i][j]
                && secondArgument.possibleWord[i][j]) {
                resultOfConcatenation.possibleWord[i][j] = 1;
            }
            for (size_t k = i; k < j; ++k)
                if (firstArgument.possibleWord[i][k] && secondArgument.possibleWord[k][j]) {
                    resultOfConcatenation.possibleWord[i][j] = 1;
                    break;
                }
        }

    res.push(resultOfConcatenation);
}

void KleeneStar(unsigned& sizeOfWord, std::string& test) {
    callERROR(1);
    StackElement argument = res.top();
    res.pop();

    for (size_t i = 0; i < sizeOfWord; ++i) {
        argument.possibleWord[i][i] = 1;
    }

    res.push(argument);
    for (size_t i = 0; i < sizeOfWord; ++i) {
        res.push(argument);
        concatenation(test);
    }
}

void readRegExp(std::string& regExp, std::string& test){
    sizeOfWord = test.length();

    for (int i = 0; i < sizeOfWord; ++i) {
        if (test[i] == 'a') {
            positions['a'].push_back(i);
        }
        if (test[i] == 'b') {
            positions['b'].push_back(i);
        }
        if (test[i] == 'c') {
            positions['c'].push_back(i);
        }
    }

    for (auto symb : regExp) {
        StackElement element(sizeOfWord);

        switch (symb) {
            case '1':
                for (auto pos : positions['1']) {
                    element.possibleWord[pos][pos + 1] = 1;
                }
                res.push(element);
                break;
            case 'a':
                for (auto pos : positions['a']) {
                    element.possibleWord[pos][pos + 1] = 1;
                }
                res.push(element);
                break;
            case 'b':
                for (auto pos : positions['b']) {
                    element.possibleWord[pos][pos + 1] = 1;
                }
                res.push(element);
                break;
            case 'c':
                for (auto pos : positions['c']) {
                    element.possibleWord[pos][pos + 1] = 1;
                }
                res.push(element);
                break;
            case '+':
                addition(test);
                break;
            case '.':
                concatenation(test);
                break;

            case '*':
                KleeneStar(sizeOfWord, test);
                break;
        }
    }
}

bool isEmptyWordIncluded() {
    StackElement resCondition = res.top();
    res.pop();

    for(size_t i = 0; i < resCondition.possibleWord.size(); ++i) {
        if(resCondition.possibleWord[i][i]) {
            lengthOfTheBiggestSuffix = 0;
            return true;
        }
    }
    return false;
}

bool checkIfSuffixIsIncluded(std::string& test){
    StackElement resCondition = res.top();

    int firstPos = 0;
    while (firstPos < sizeOfWord) {
        if (resCondition.possibleWord[firstPos][sizeOfWord]) {
            lengthOfTheBiggestSuffix = sizeOfWord - firstPos;
            return true;
        } else {
            ++firstPos;
        }
    }
    return isEmptyWordIncluded();
}

int main() {
    std::string regularExpression;
    std::string exampleOfTest;
    std::cin >> regularExpression >> exampleOfTest;

    readRegExp(regularExpression, exampleOfTest);

    callERROR();

    if (checkIfSuffixIsIncluded(exampleOfTest)) {
        std::cout << lengthOfTheBiggestSuffix;
    } else {
        std::cout << "INF";
    }

    return 0;
}
