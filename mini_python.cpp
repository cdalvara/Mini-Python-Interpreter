#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
vector<string> variables;
vector<string> variableValues;

string removeWhiteSpace(string theString) { //removes spaces
    string output = "";
    bool inString = false;
    for(int i =0; i < theString.size(); i++) {
        if(theString[i] != ' ') output = output + theString[i];
        if(theString[i] == ' ' && inString == true) output = output + theString[i];
        if(theString[i] == '"') inString = !inString;
    }
    return output;
}

bool numberOrString (string theString){ //returns true if input is a valid number
    if(theString[0] == '-') {
        for(int i = 1; i < theString.length(); i++) {
        if(isdigit(theString[i]) == false) return false;
        }
        return true;
    }
    for(int i = 0; i < theString.length(); i++) {
        if(isdigit(theString[i]) == false) return false;
    }
    return true;
}

bool validString (string theString) { //returns true if input is valid string
    if(theString[0] == '"' && theString[theString.length() - 1] == '"') return true;
    return false;
}

bool validArray (string theString) { //returns true if input is valid array
    bool openString = false;
    if(!(theString[0] == '[' && theString[theString.length() - 1] == ']')) return false;
    for(int i = 1; i < theString.size()-1; i++) {
        if(theString[i] == '[' || theString[i] == ']') {
            if (openString == false) return false;
        }
        else if(theString[i] = '"') openString = !openString;
    }

    return true;
}

int countElementsInArray(string array) { //counts the elements in Array
    int count = 0;
    bool insideString = false;
    for(int i = 0; i < array.size(); i++) {
        if(array[i] == '"') insideString = !insideString;
        if(array[i] == ',') {
            if(insideString == false) count++;
        }
    }
    return count + 1;
}

string getSlicedArray(string array, string startToFinish) { //cuts the array
    int start = 0;
    int finish = 0;
    string temp = "";
    bool toEnd = true;
    for(int i = 0; i < startToFinish.size(); i++) {
        if(startToFinish[i] == ':') {
            if(temp.length() != 0) {
                if(numberOrString(temp) == false) {
                    return "error";
                }
                start = stoi(temp);
            }
            temp = "";
        }
        else if(startToFinish[i] != ':') temp = temp + startToFinish[i];
    }
    if(temp.length() != 0) {
        if(numberOrString(temp) == false) return "error";
        finish = stoi(temp);
        toEnd = false;
    }
    int totalElements = countElementsInArray(array);
    if(toEnd == true) finish = totalElements;

    if(start < 0) start = totalElements + start;
    if(finish < 0) finish = totalElements + finish;

    if(start >= totalElements) return "[]";
    if(finish > totalElements) finish = totalElements;

    

    array = array.substr(1, array.size() - 2);
    string outputArray = "[";
    bool insideString = false;
    int count = 0;
    for(int j = 0; j < array.size(); j++) {
        if(start == finish) break;
        if(array[j] == '"') {
            insideString = !insideString;
        }
        if(array[j] == ',' && insideString == false) {
            count++;
            if(count != finish && outputArray.size() != 1) outputArray = outputArray + ',';
        }
        if(array[j] != ',' || insideString == true) {
            if(count >= start) {
                if(count <= finish) outputArray = outputArray + array[j];
            }
        }
    }
    outputArray = outputArray + "]";
    
    
    return outputArray;
    
}

string getElementOfArray2(string whichArray, string whichIndex) { //returns element stored in array
    int indexInValues;
    string output = "";
    for(int i = variables.size() - 1; i >= 0; i-- ) {
        if(whichArray == variables[i]) {
            indexInValues = i;
            break;
        }
    }
    if(validArray(variableValues[indexInValues]) == false) {
        output = "There is a problem";
        return output;
    }
    string array = variableValues[indexInValues];
    bool inString = false;
    int counter = 0;
    int whichIndex1;
    for(int a = 0; a < whichIndex.length(); a++) {
        if(whichIndex[a] == ':') {
            return getSlicedArray(array, whichIndex);
            break;
        }
    }
    if(numberOrString(whichIndex) == false) return "error";
    stringstream theStream(whichIndex);
    theStream >> whichIndex1;
    bool addStrings = false;
    for(int j = 1; j < array.size() - 1; j++) {
        if(array[j] == ',') {
            if(inString == false) {
                counter = counter + 1;
                if (counter > whichIndex1) break;
            }
        }
        else if(array[j] == '"') {
            inString = !inString;
            addStrings = true;
        }
        else if(array[j] != '"') {
            if(whichIndex1 == counter) {
                if(inString == true) output = output + array[j];
                else if(inString == false) {
                    if(array[j] != ' ') output = output + array[j];
                }
            }
        }
    }
    if(output.size() == 0) output = "error";
    if(addStrings == true) output = '"' + output +'"';
    return output;
}

string getElementOfArray(string valueToget) { //returns element in array
    string output = "";
    string findingVariable = "";
    string indexInArray = "";
    bool foundOpenBracket = false;
    bool foundClosedBracket = false;
    bool notArray = true;
    for(int i = 0; i < valueToget.size(); i++) {
        if(valueToget[i] != '[' && valueToget[i] != ']') {
            if(foundOpenBracket == false) {
                findingVariable = findingVariable + valueToget[i];
                notArray = false;
            }
            else if(foundOpenBracket == true) {
                if(valueToget[i] == ' ' && notArray == false) {
                    output = "There is a problem";
                    return output;
                }
                indexInArray = indexInArray + valueToget[i];
            }
        }
        else if(valueToget[i] == '[') foundOpenBracket = true;
        else if(valueToget[i] == ']') foundClosedBracket = true;
    }
    if (foundOpenBracket == false || notArray == true) {
        output = "Not trying to get element";
        return output;
    }
    
    if (foundOpenBracket == true && foundClosedBracket == true && notArray == false) output = getElementOfArray2(findingVariable, indexInArray);
    return output;
}


bool validArray2(string firstArray) {  //checks if array is valid
    bool openString = false;
    if(firstArray[0] == ',') return false;
    bool previousComma = false;
    bool previousValue = false;
    bool previousSpace = false;
    bool elementFirst = false;
    for(int j = 0; j < firstArray.size(); j++) {
        if(firstArray[j] == '"') openString = !openString;
        if(firstArray[j] == ',') {
            if(openString == false) {
                if(previousComma == true || elementFirst == false) return false;
                previousComma = true;
                previousValue = false;
                previousSpace = false;
            }
        }
        else if(firstArray[j] == ' ') {
            previousSpace = true;
        }
        else {
            if(openString == false) {
                if(previousValue == true && previousSpace == true) return false;
                previousComma = false;
                previousValue = true;
                elementFirst = true;
                previousSpace = false;
            }
        }

    }
    if(openString == true) return false;
    return true;
}

bool isPrint(string printer) { //checks if print is correct
    if(printer.substr(0,6) == "print(") return true;  
    return false;
}

bool thePrint(string printer) { //prints
    string variableToPrint;
    if(printer.substr(0,6) == "print(" && printer[printer.size() - 1] == ')') {
        variableToPrint = printer.substr(6, printer.size() - 7);
        variableToPrint.erase(remove_if(variableToPrint.begin(), variableToPrint.end(), ::isspace), variableToPrint.end());
        if(numberOrString(variableToPrint) == true) {
            cout << variableToPrint << endl;
            return true;
        }
        if(validString(variableToPrint) == true) {
            cout << variableToPrint.substr(1, variableToPrint.size() - 2) << endl;
            return true;
        }
        for(int i = variables.size() - 1; i >= 0; i--) {
            if(variableToPrint == variables[i]) {
                if(validArray(variableValues[i]) == true) {
                    for(int j = 0; j < variableValues[i].length(); j++) {
                        if(variableValues[i][j] == '"') variableValues[i][j] = '\'';
                    }
                    cout << variableValues[i] << endl;
                    return true;
                }
                if(validString(variableValues[i]) == true) {
                    cout << variableValues[i].substr(1, variableValues[i].size() - 2) << endl;
                    return true;
                }
                cout << variableValues[i] << endl;
                return true;
            }
        }
    }
    return false;
}

bool sub(string first, string second) { // function for subtraction
    bool firstSaved = false;
    bool secondSaved = false;
    if(getElementOfArray(first) != "Not trying to get element"){
        if(getElementOfArray(first) == "error") return false;
        first = getElementOfArray(first);
    }
    if(getElementOfArray(second) != "Not trying to get element"){
        if(getElementOfArray(second) == "error") return false;
        second = getElementOfArray(second);
    }
    for(int i = variables.size() - 2; i >= 0 ; i--) {
        if (first == variables[i] && firstSaved == false) {
            firstSaved == true;
            first = variableValues[i];
        }
        if (second == variables[i] && secondSaved == false){
            secondSaved == true;
            second = variableValues[i];
        }
        if (firstSaved == true && secondSaved == true) break;
    }


    if(numberOrString(first) == true && numberOrString(second) == true) {
        int firstInt = stoi(first);
        int secondInt = stoi(second);
        variableValues[variableValues.size()-1] =  to_string(firstInt - secondInt);
        return true;
    }

    return false;
}

bool add(string first, string second) { //function for adding
    bool isIntFirst = false;
    int intFirst, intSecond;
    string stringFirst, stringSecond;
    bool firstSaved = false;
    bool secondSaved = false;


    
    if(getElementOfArray(first) != "Not trying to get element"){
        if(getElementOfArray(first) == "error") return false;
        first = getElementOfArray(first);
    }
    if(getElementOfArray(second) != "Not trying to get element"){
        if(getElementOfArray(second) == "error") return false;
        second = getElementOfArray(second);
    }

    for(int i = variables.size() - 2; i >= 0 ; i--) {
        if (first == variables[i] && firstSaved == false) {
            firstSaved == true;
            first = variableValues[i];
        }
        if (second == variables[i] && secondSaved == false){
            secondSaved == true;
            second = variableValues[i];
        }
        if (firstSaved == true && secondSaved == true) break;
    }
    
    if(numberOrString(first) == false &&validString(first) == false && validArray(first ) == false) first = '"' + first + '"';
    if(numberOrString(second) == false &&validString(second) == false && validArray(second) == false) second = '"' + second +'"';
    if(numberOrString(first) == true) {
        isIntFirst = true;
        stringstream firstValue(first);
        firstValue >> intFirst;
    }
    if(numberOrString(second) == true) {
        if(isIntFirst == false) return false;
        stringstream secondValue(second);
        secondValue >> intSecond;
        variableValues[variableValues.size()-1] =  to_string(intFirst + intSecond); 
        return true;
    }
    if(validString(first) == true) {
        if(validString(second) == true) {
            stringFirst = first.substr(1, first.length() - 2);
            stringSecond = second.substr(1, second.length() - 2);
            variableValues[variableValues.size()-1] = '"' + stringFirst + stringSecond + '"';
            return true;
        }
    }
    if(validArray(first) == true) {
        if(validArray(second) == true) {
            string firstArray = first.substr(1, first.length() - 2);
            string secondArray = second.substr(1, second.length() - 2);
            string finalArray = "[";
            bool inString = false;
            if(validArray2(firstArray) == true && validArray2(secondArray) == true) {
                //variableValues[variableValues.size()-1] = '[' + firstArray + ',' + secondArray + ']';
                for(int i = 0; i < firstArray.size(); i++) {
                    if(firstArray[i] != ' ' && inString == false) finalArray = finalArray + firstArray[i];
                    else if(firstArray[i] == ' ' && inString == true) finalArray = finalArray + ' ';
                    else if(finalArray[i] == '"') inString = !inString;
                }
                finalArray = finalArray + ',';
                inString = false;
                for(int i = 0; i < secondArray.size(); i++) {
                    if(secondArray[i] != ' ' && inString == false) finalArray = finalArray + secondArray[i];
                    else if(secondArray[i] == ' ' && inString == true) finalArray = finalArray + ' ';
                    else if(finalArray[i] == '"') inString = !inString;
                }
                finalArray = finalArray +']';
                variableValues[variableValues.size()-1] = finalArray;

                return true;
            }
        }
    }
    
    
    return false;
    
}

bool ifTrue = false;
bool elseTrue = false;
bool nestedIfTrue = false;
bool nestedElseTrue = false;

bool compareInts2(string first, string second, string comparer) { //compares integers
    int firstInt = stoi(first);
    int secondInt = stoi(second);
    if(comparer.size() == 1) {
        if(comparer == ">") {
            nestedIfTrue = (firstInt > secondInt);
            return true;
        }
        if(comparer == "<") {
            nestedIfTrue = (firstInt < secondInt); 
            return true;
        }
        return false;
    }

    if(comparer.size() == 2) {
        if(comparer == ">=") {
            nestedIfTrue = (firstInt >= secondInt);
            return true;
        }
        if(comparer == "<=") {
            nestedIfTrue = (firstInt <= secondInt); 
            return true;
        }
        if(comparer == "==") {
            nestedIfTrue = (firstInt == secondInt);
            return true;
        }
        if(comparer == "!=") {
            nestedIfTrue = (firstInt != secondInt);
            return true;
        }
        return false;
    }
    return false;
}

bool compareStrings2(string first, string second, string comparer) { //compares strings
    first = first.substr(1, first.size() -1);
    second = second.substr(1, second.size() - 1);
    if(comparer.size() == 1) {
        if(comparer == ">") {
            nestedIfTrue = (first > second);
            return true;
        }
        if(comparer == "<") {
            nestedIfTrue = (first < second); 
            return true;
        }
        return false;
    }

    if(comparer.size() == 2) {
        if(comparer == ">=") {
            nestedIfTrue = (first >= second);
            return true;
        }
        if(comparer == "<=") {
            nestedIfTrue = (first <= second); 
            return true;
        }
        if(comparer == "==") {
            nestedIfTrue = (first == second);
            return true;
        }
        if(comparer == "!=") {
            nestedIfTrue = (first != second);
            return true;
        }
        return false;
    }
    
    return false;
}

bool checkNestedStateCorrectSyntax(vector<string> ifState) { //checks if nested if/else is correct
    string firstElement = ifState[1];
    string comparer = ifState[2];
    string secondElement = ifState[3];
    if(ifState[3] == "=") {
        comparer = comparer + '=';
        secondElement = ifState[4];
    }
    else if(ifState[3] == ">" || ifState[3] == "<" || ifState[3] == "!") return false;
    if(secondElement.size() == 0) return false;
    if(validString(secondElement) == true) {
        if(ifState.size() != 6) return false;
        else if(ifState.size() == 6) {
            if(ifState[5] != ":") return false;
        }
    } 
    else if(secondElement[secondElement.size() -1] != ':' && validString(secondElement) == false) {
        return false;
    } 
    secondElement = secondElement.substr(0, secondElement.size() -1);


    bool firstSaved = false;
    bool secondSaved = false;
    for(int i = variables.size() - 1; i >= 0; i--) {
        if(firstElement == variables[i] && firstSaved == false) {
            firstSaved = true;
            firstElement = variableValues[i];
        }
        if(secondElement == variables[i] && secondSaved == false) {
            secondSaved = true;
            secondElement = variableValues[i];
        }
        if(firstSaved == true && secondSaved == true) break;
    }
    if(getElementOfArray(firstElement) == "There is a problem") return false;
    if(getElementOfArray(firstElement) != "Not trying to get element") firstElement = getElementOfArray(firstElement);
    if(getElementOfArray(secondElement) == "There is a problem") return false;
    if(getElementOfArray(secondElement) != "Not trying to get element") secondElement = getElementOfArray(secondElement);


    if(numberOrString(firstElement) == true) {
        if(numberOrString(secondElement) == false) return false;
        return compareInts2(firstElement, secondElement, comparer);

    }
    if(numberOrString(firstElement) == false) {
        if(numberOrString(secondElement) == true) return false;
        return compareStrings2(firstElement, secondElement, comparer);
    }
    
    return false;
}

bool compareInts(string first, string second, string comparer) { //compares integers
    int firstInt = stoi(first);
    int secondInt = stoi(second);
    if(comparer.size() == 1) {
        if(comparer == ">") {
            ifTrue = (firstInt > secondInt);
            return true;
        }
        if(comparer == "<") {
            ifTrue = (firstInt < secondInt); 
            return true;
        }
        return false;
    }

    if(comparer.size() == 2) {
        if(comparer == ">=") {
            ifTrue = (firstInt >= secondInt);
            return true;
        }
        if(comparer == "<=") {
            ifTrue = (firstInt <= secondInt); 
            return true;
        }
        if(comparer == "==") {
            ifTrue = (firstInt == secondInt);
            return true;
        }
        if(comparer == "!=") {
            ifTrue = (firstInt != secondInt);
            return true;
        }
        return false;
    }
    return false;
}

bool compareStrings(string first, string second, string comparer) { //compares strings
    first = first.substr(1, first.size() -1);
    second = second.substr(1, second.size() - 1);
    if(comparer.size() == 1) {
        if(comparer == ">") {
            ifTrue = (first > second);
            return true;
        }
        if(comparer == "<") {
            ifTrue = (first < second); 
            return true;
        }
        return false;
    }

    if(comparer.size() == 2) {
        if(comparer == ">=") {
            ifTrue = (first >= second);
            return true;
        }
        if(comparer == "<=") {
            ifTrue = (first <= second); 
            return true;
        }
        if(comparer == "==") {
            ifTrue = (first == second);
            return true;
        }
        if(comparer == "!=") {
            ifTrue = (first != second);
            return true;
        }
        return false;
    }
    
    return false;
}

bool checkIfStateCorrectSyntax(vector<string> ifState) { //checks if if/else is correct
    string firstElement = ifState[1];
    string comparer = ifState[2];
    string secondElement = ifState[3];
    if(ifState[3] == "=") {
        comparer = comparer + '=';
        secondElement = ifState[4];
    }
    else if(ifState[3] == ">" || ifState[3] == "<" || ifState[3] == "!") return false;
    if(secondElement.size() == 0) return false;
    if(validString(secondElement) == true) {
        if(ifState.size() != 6) return false;
        else if(ifState.size() == 6) {
            if(ifState[5] != ":") return false;
        }
    } 
    else if(secondElement[secondElement.size() -1] != ':' && validString(secondElement) == false) {
        return false;
    } 
    secondElement = secondElement.substr(0, secondElement.size() -1);


    bool firstSaved = false;
    bool secondSaved = false;
    for(int i = variables.size() - 1; i >= 0; i--) {
        if(firstElement == variables[i] && firstSaved == false) {
            firstSaved = true;
            firstElement = variableValues[i];
        }
        if(secondElement == variables[i] && secondSaved == false) {
            secondSaved = true;
            secondElement = variableValues[i];
        }
        if(firstSaved == true && secondSaved == true) break;
    }
    if(getElementOfArray(firstElement) == "There is a problem") return false;
    if(getElementOfArray(firstElement) != "Not trying to get element") firstElement = getElementOfArray(firstElement);
    if(getElementOfArray(secondElement) == "There is a problem") return false;
    if(getElementOfArray(secondElement) != "Not trying to get element") secondElement = getElementOfArray(secondElement);


    if(numberOrString(firstElement) == true) {
        if(numberOrString(secondElement) == false) return false;
        return compareInts(firstElement, secondElement, comparer);

    }
    if(numberOrString(firstElement) == false) {
        if(numberOrString(secondElement) == true) return false;
        return compareStrings(firstElement, secondElement, comparer);
    }
    
    return false;
}

bool readLine(vector<string> theLine) { //reads a line
    bool equalChecker = false;
    bool previousVar = false;
    bool previousPlus = false;
    bool previousMinus = false;
    
    if(theLine[0] == "=" || theLine[theLine.size() - 1] == "=" || theLine[0] == "+" || theLine[0] == "-") return false;
    if(theLine[0] == "if") {
        elseTrue = false;
        return checkIfStateCorrectSyntax(theLine);
    }
    if(theLine[0] == "else:") {
        elseTrue = !ifTrue;
        ifTrue = false;
        return true;
    }
    if(theLine[0] == " if") {
        nestedElseTrue = false;
        return checkNestedStateCorrectSyntax(theLine);
    }
    if(theLine[0] == " else:") {
        nestedElseTrue = !nestedIfTrue;
        nestedIfTrue = false;
        return true;
    }
    if(theLine[0][1] != ' ') {
        nestedIfTrue = false;
        nestedElseTrue = false;
    }
    if(theLine[0][0] != ' ') {
        ifTrue = false;
        elseTrue = false;
    }
    

    else if(theLine[0][0] == ' ' && ifTrue == false && elseTrue == false) return true;
    else if(theLine[0][0] == ' ' && theLine[0][1] != ' ' & ifTrue == true && elseTrue == false) theLine[0] = removeWhiteSpace(theLine[0]); 
    else if(theLine[0][0] == ' ' && theLine[0][1] != ' ' & elseTrue == true && ifTrue == false) theLine[0] = removeWhiteSpace(theLine[0]);
    else if(theLine[0][0] == ' ' && theLine[0][1] == ' ' && nestedIfTrue == false && nestedElseTrue == false) return true;
    else if(theLine[0][0] == ' ' && theLine[0][1] == ' ' & nestedIfTrue == true && nestedElseTrue == false) theLine[0] = removeWhiteSpace(theLine[0]);
    else if(theLine[0][0] == ' ' && theLine[0][1] == ' ' & nestedElseTrue == true && nestedIfTrue == false) theLine[0] = removeWhiteSpace(theLine[0]);

    string temp = "";

    for(int i = 0; i < theLine.size(); i++) {
        if((theLine[i] != "if") && (theLine[i] != "else:") && (theLine[i] != "+") && (theLine[i] != "=") && (theLine[i] != "-") && (theLine[i] != " if") && (theLine[i] != " else:")) {

            for(int j = 0; j < theLine[i].length(); j++) {
                if(theLine[i][j] != '(' && theLine[i][j] != ')') {
                    temp = temp + theLine[i][j];
                }
                else if(theLine[i][j] == '(' || theLine[i][j] == ')') {
                    if(equalChecker == false) temp = temp + theLine[i][j];
                }
            }

            theLine[i] = temp;
            temp = "";
            


            if(previousPlus == true) {
                previousPlus = false;
                previousVar = true;
                previousMinus = false;
                if(add(variableValues[variableValues.size() -1], theLine[i]) == false) return false;
            }
            else if(previousMinus == true) {
                previousMinus = false;
                previousPlus = false;
                previousVar = true;
                if(sub(variableValues[variableValues.size() -1], theLine[i]) == false) return false;
            }
            else {
                previousPlus = false;
                previousMinus = false;
                if(equalChecker == false) {
                    if(previousVar == false) {
                        if(isPrint(theLine[i]) == false) variables.push_back(theLine[i]);
                        else {
                            if(thePrint(theLine[i]) == false) return false;
                        } 
                        previousVar = !previousVar;   
                    }
                    else if(previousVar == true) return false;
                }
                else if(equalChecker == true) {
                    if(previousVar == false) { 
                        bool alreadyPushed = false;
                        for(int b = variables.size() - 2; b >= 0; b--) {
                            if(theLine[i] == variables[b]) {
                                variableValues.push_back(variableValues[b]);
                                alreadyPushed = true;
                                break;
                            }
                        }
                        if(alreadyPushed == false) {
                            string toValues = getElementOfArray(theLine[i]);
                            if(toValues == "There is a problem") return false;
                            if(toValues != "Not trying to get element") {
                                variableValues.push_back(toValues);
                                alreadyPushed = true;
                            } 
                        }
                        if (alreadyPushed == false) variableValues.push_back(theLine[i]);
                        previousVar = !previousVar;
                    }
                    else if(previousVar == true) return false;
                }
            }
        }
        else if(theLine[i] == "+") {
            if(previousPlus == true || previousMinus == true) return false;
            previousPlus = true;
            previousVar = false;
            
        }
        else if(theLine[i] == "-") {
            if(previousMinus == true || previousPlus == true) return false;
            previousMinus = true;
            previousVar = false;
        }
        else if(theLine[i] == "=") {
            if(equalChecker == true || previousPlus == true || previousMinus == true) return false;
            equalChecker = true;
            previousVar = false;
        }


    }

    if(previousPlus == true || previousMinus == true) return false;
    return true;
}

bool ifStatement = false;
bool elseStatement = false;
bool nestedIfStatement = false;
bool nestedElseStatement = false;
bool readLines(string theLine) { //reads lines and passes each one to readLine()
    bool insideString = false;
    bool insideBrackets = false;
    vector<string> lineArguments;
    string temp = "";

    if(theLine.empty()) return true;

    if (theLine[0] == '#') return true;
    if(theLine[0] != ' ') {
        elseStatement = false;
        ifStatement = false;
    }
    if(theLine[2] != ' ') {
        nestedIfStatement = false;
        nestedElseStatement = false;
    }
    for(int i = 0; i < theLine.length(); i++) {
        if(theLine[i] == ' ' && insideString == false) {
            if(lineArguments.size() == 0) {
                if(temp == "if") {
                    lineArguments.push_back(temp);
                    temp = "";
                    ifStatement = true;
                }
                if(temp.size() == 0) {
                    if(ifStatement == true || elseStatement == true ){
                        if(elseStatement == true ) temp = temp + ' ';
                        else if(lineArguments.size() == 0) temp = temp +' ';
                    }
                }
                if(temp == " if") {
                    lineArguments.push_back(temp);
                    temp= "";
                    nestedIfStatement = true;
                }
                if(temp == " ") {
                    if(nestedIfStatement == true || nestedElseStatement == true ){
                        if(nestedElseStatement == true ) temp = temp + " ";
                        else if(lineArguments.size() == 0) temp = temp +" ";
                    }
                }
                
            }
            else {
                if(temp.size() != 0 && insideBrackets == false) {
                    lineArguments.push_back(temp);
                    temp = "";
                }
            }
        }
        else if(theLine[i] == ' ' && insideString == true) {
            temp = temp + " ";
        }
        else if(theLine[i] != ' ') {
            if (insideString == true) {
                temp = temp + theLine[i];
                if(theLine[i] == '"' && temp.substr(0,7) != "print(\"" && temp.substr(0,8) != " print(\"" && temp.substr(0,9) != "  print(\"" && temp.substr(0,10) != "   print(\"" && temp.substr(0,11) != "    print(\"") {
                    if(insideBrackets != true) {
                        lineArguments.push_back(temp);
                        temp = "";
                        insideString = false;
                    }
                    
                }
            }
            else if((theLine[i] == '+') || (theLine[i] == '-') || (theLine[i] == '=') || (theLine[i] == '>') || (theLine[i] == '<') || (theLine[i] == '!')) {
                
                if(insideBrackets == true) temp = temp + theLine[i];
                else {
                     if(temp.size() != 0) lineArguments.push_back(temp);
                    string temp1(1, theLine[i]);
                    lineArguments.push_back(temp1);
                    temp = "";
                }
            }


            
            else {
                if(theLine[i] == '#') break;
                if (theLine[i] == '"') insideString = !insideString;
                if(theLine[i] == '[') insideBrackets = true;
                if(theLine[i] ==']') insideBrackets = false;
                temp = temp + theLine[i];
            }
        }
    }
    if(temp.size() != 0) {
        if(temp == "else:") elseStatement = true;
        if(temp == " else:") nestedElseStatement = true;
        lineArguments.push_back(temp);
    }

    if(readLine(lineArguments) == false) {
        cout <<"error" << theLine << endl;
        return false;
    }
    /*for(int i = 0; i < lineArguments.size(); i++) {
        cout << lineArguments[i] << endl;
    }
    cout << "------" << endl; */

    return true;

}

int main(int argc, char *argv[]) {

    if (argc == 1) { //checks if an input file was given
        cout << "Error: No file given\n";
        return 0;
    }

    vector<string> fileVector; //using vectors. Access time is O(1)
    
    string line;
    ifstream theFile(argv[1]);
    if(theFile.is_open()) {
        while(getline(theFile, line)) {
            if (line.length() != 0) fileVector.push_back(line);
        }
        theFile.close();
    }
    else {
        cout << "Error: file does not exist\n";
        return 0;
    }


    for(int i = 0; i < fileVector.size(); i++) {
        if(readLines(fileVector[i]) == false) return 0;
    } 
    /*for(int a = 0; a < variables.size(); a++) cout << variables[a] << endl;
    cout << "-------" << endl;
    for(int a = 0; a < variableValues.size(); a++) cout << variableValues[a] << endl;*/

    return 0;
}