#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include "Image.h"
using namespace std;

int main(int argc, char* argv[]) {
    vector<string> methods{"multiply", "subtract", "overlay", "screen", "combine", "flip", "onlyred", "onlygreen", "onlyblue", "addred", "addgreen", "addblue", "scalered", "scalegreen", "scaleblue"};
    vector<string> fileMethods{"multiply", "subtract", "overlay", "screen"};
    vector<string> special{"combine"};
    vector<string> numMethods{"addred", "addgreen", "addblue", "scalered", "scalegreen", "scaleblue"};
    vector<string> pureMethods{"onlyblue", "onlyred", "onlygreen", "flip"};
    vector<string> manipulation;
    vector<char*> argument;
    vector<bool> isMethod;
    vector<bool> needFile;
    vector<bool> needNum;
    vector<bool> noNeed;
    vector<bool> isCombine;
    int counterPure = 0;
    int counterCombine = 0;
    for (int i = 3; i < argc; i++) {
        string name = argv[i];
        if (find(methods.begin(), methods.end(), name) != methods.end()){
            manipulation.push_back(name);
            isMethod.push_back(true);
        }
        else {argument.push_back(argv[i]); isMethod.push_back(false);}
    }
    int counter = 0;
    for (auto & method : manipulation) {
        counter++;
        for (auto & file : fileMethods) {
            if (method == file) {
                needFile.push_back(true);
                break;
            }
        }
        for (auto & num : numMethods) {
            if (method == num) {
                needNum.push_back(true);
                break;
            }
        }
        for (auto & pure : pureMethods) {
            if (method == pure) {
                noNeed.push_back(true);
                counterPure++;
                break;
            }
        }
        for (auto & spec : special) {
            if (method == spec){
                isCombine.push_back(true);
                counterCombine++;
                break;
            }
        }
        if (needFile.size() != counter) {needFile.push_back(false);}
        if (needNum.size() != counter) {needNum.push_back(false);}
        if (noNeed.size() != counter) {noNeed.push_back(false);}
        if (isCombine.size() != counter) {isCombine.push_back(false);}
    }
    int totalArg = 0;
    for (int i = 0; i < manipulation.size(); i++) {
        if (needFile.at(i) || needNum.at(i)) {totalArg++;}
        else if (isCombine.at(i)) {totalArg+=2;}
    }
    if (argc < 3) {
        if (argc == 1 || strcmp(argv[1], "--help") == 0) {
            cout << "Project 2: Image Processing, Spring 2023" << endl << endl;
            cout << "Usage:" << endl << "\t" << argv[0] << " [output] [firstImage] [method] [...]";
        }
        else {
            cout << "Invalid file name." << endl;
        }
    }
    else if (argc < 4) {
        char* outPtr = strstr(argv[1], ".tga");
        char* trackPtr = strstr(argv[2], ".tga");
        ifstream test(argv[2], ios_base::binary);
        if (outPtr == nullptr || trackPtr == nullptr) {
            cout << "Invalid file name." << endl;
        }
        else if (!test.is_open()) {
            cout << "File does not exist." << endl;
        }
        else {
            cout << "Invalid method name." << endl;
        }
    }
    else if (argc < 5 && (!isMethod.at(0) || noNeed.empty() || (noNeed.empty() && isMethod.at(0) && (manipulation.size() != argument.size())))) {
        char* outPtr = strstr(argv[1], ".tga");
        char* trackPtr = strstr(argv[2], ".tga");
        ifstream test(argv[2], ios_base::binary);
        if (outPtr == nullptr || trackPtr == nullptr) {
            cout << "Invalid file name." << endl;
        }
        else if (!test.is_open()) {
            cout << "File does not exist." << endl;
        }
        else if (!isMethod.at(0)){
            cout << "Invalid method name." << endl;
        }
        else {
            cout << "Missing argument." << endl;
        }
    }
    else {
        Image *imagePtr = nullptr;
        Image image;
        int argCounter = 0;
        if (totalArg == argument.size()) {
            for (int i = 0; i < manipulation.size(); i++) {
                if (needFile.at(i)) {
                    string firstFile = argv[2];
                    char *ptr = strstr(argv[1], ".tga");
                    char *ptr1 = strstr(argv[2], ".tga");
                    char *ptr2 = strstr(argument.at(argCounter), ".tga");
                    ifstream test1(argv[2], ios_base::binary);
                    ifstream test2(argument.at(argCounter), ios_base::binary);
                    if (ptr == nullptr || ptr1 == nullptr) {
                        cout << "Invalid file name." << endl;
                        return 0;
                    } else if (ptr2 == nullptr) {
                        cout << "Invalid argument, invalid file name." << endl;
                        return 0;
                    } else if (!test1.is_open()) {
                        cout << "File does not exist." << endl;
                        return 0;
                    } else if (!test2.is_open()) {
                        cout << "Invalid argument, file does not exist." << endl;
                        return 0;
                    } else {
                        if (manipulation.at(i).find("multiply") != string::npos) {
                            image = multiply(argument.at(argCounter), firstFile, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                        if (manipulation.at(i).find("subtract") != string::npos) {
                            image = subtract(argument.at(argCounter), firstFile, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                        if (manipulation.at(i).find("overlay") != string::npos) {
                            image = overlay(firstFile, argument.at(argCounter), imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                        if (manipulation.at(i).find("screen") != string::npos) {
                            image = screen(argument.at(argCounter), firstFile, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                    }
                }
                if (needNum.at(i)) {
                    string firstFile = argv[2];
                    char *ptr = strstr(argv[1], ".tga");
                    char *ptr1 = strstr(argv[2], ".tga");
                    ifstream test1(argv[2], ios_base::binary);
                    bool isInt = false;
                    if (argument.size() == totalArg) {
                        string num = argument.at(argCounter);
                        for (char j: num) {
                            if (isdigit(j)) { isInt = true; }
                            else { isInt = false; }
                        }
                    } else {
                        cout << "Missing argument." << endl;
                        return 0;
                    }

                    if (ptr == nullptr || ptr1 == nullptr) {
                        cout << "Invalid file name." << endl;
                        return 0;
                    } else if (!test1.is_open()) {
                        cout << "File does not exist." << endl;
                        return 0;
                    } else if (!isInt) {
                        cout << "Invalid argument, expected number." << endl;
                        return 0;
                    } else {
                        string numVal = argument.at(argCounter);
                        int val = stoi(numVal);
                        if (manipulation.at(i).find("addgreen") != string::npos) {
                            image = addgreen(firstFile, val, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                        if (manipulation.at(i).find("addred") != string::npos) {
                            image = addred(firstFile, val, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                        if (manipulation.at(i).find("addblue") != string::npos) {
                            image = addblue(firstFile, val, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                        if (manipulation.at(i).find("scalegreen") != string::npos) {
                            image = scalegreen(firstFile, val, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                        if (manipulation.at(i).find("scalered") != string::npos) {
                            image = scalered(firstFile, val, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                        if (manipulation.at(i).find("scaleblue") != string::npos) {
                            image = scaleblue(firstFile, val, imagePtr);
                            imagePtr = &image;
                            argCounter++;
                        }
                    }
                }
                if (noNeed.at(i)) {
                    string firstFile = argv[2];
                    char *ptr = strstr(argv[1], ".tga");
                    char *ptr1 = strstr(argv[2], ".tga");
                    ifstream test1(argv[2], ios_base::binary);
                    if (ptr == nullptr || ptr1 == nullptr) {
                        cout << "Invalid file name." << endl;
                        return 0;
                    } else if (!test1.is_open()) {
                        cout << "File does not exist." << endl;
                        return 0;
                    } else {
                        if (manipulation.at(i).find("onlyred") != string::npos) {
                            image = redonly(firstFile, imagePtr);
                            imagePtr = &image;
                        } else if (manipulation.at(i).find("onlygreen") != string::npos) {
                            image = greenonly(firstFile, imagePtr);
                            imagePtr = &image;
                        } else if (manipulation.at(i).find("onlyblue") != string::npos) {
                            image = blueonly(firstFile, imagePtr);
                            imagePtr = &image;
                        } else if (manipulation.at(i).find("flip") != string::npos) {
                            image = flip(firstFile, imagePtr);
                            imagePtr = &image;
                        }
                    }
                }
                if (isCombine.at(i)) {
                    if (totalArg != argument.size()) {
                        cout << "Missing argument" << endl;
                        return 0;
                    }
                    char *ptr = strstr(argv[1], ".tga");
                    char *ptr1 = strstr(argv[2], ".tga");
                    char *ptr2 = strstr(argument.at(argCounter), ".tga");
                    char *ptr3 = strstr(argument.at(argCounter + 1), ".tga");
                    ifstream test1(argv[2], ios_base::binary);
                    ifstream test2(argument.at(argCounter), ios_base::binary);
                    ifstream test3(argument.at(argCounter + 1), ios_base::binary);
                    if (ptr == nullptr || ptr1 == nullptr) {
                        cout << "Invalid file name." << endl;
                        return 0;
                    } else if (ptr2 == nullptr || ptr3 == nullptr) {
                        cout << "Invalid argument, invalid file name." << endl;
                        return 0;
                    } else if (!test1.is_open()) {
                        cout << "File does not exist." << endl;
                        return 0;
                    } else if (!test2.is_open() || !test3.is_open()) {
                        cout << "Invalid argument, file does not exist." << endl;
                        return 0;
                    } else {
                        string firstFile = argv[2];
                        if (manipulation.at(i).find("combine") != string::npos) {
                            image = combine(argument.at(argCounter + 1), argument.at(argCounter), firstFile, imagePtr);
                            imagePtr = &image;
                            argCounter += 2;
                        }
                    }
                }
            }
            write(argv[1], imagePtr);
        }
        else {
            int finalCount = 3;
            while (finalCount < argc) {
                if (isMethod.at(finalCount-3)) {
                    string name = argv[finalCount];
                    if (find(fileMethods.begin(), fileMethods.end(), name) != fileMethods.end()) {
                        if (argc < finalCount + 2) {
                            cout << "Missing argument." << endl;
                            return 0;
                        } else {
                            string firstFile = argv[2];
                            char *ptr = strstr(argv[1], ".tga");
                            char *ptr1 = strstr(argv[2], ".tga");
                            char *ptr2 = strstr(argv[finalCount+1], ".tga");
                            ifstream test1(argv[2], ios_base::binary);
                            ifstream test2(argv[finalCount+1], ios_base::binary);
                            if (ptr == nullptr || ptr1 == nullptr) {
                                cout << "Invalid file name." << endl;
                                return 0;
                            } else if (ptr2 == nullptr) {
                                cout << "Invalid argument, invalid file name." << endl;
                                return 0;
                            } else if (!test1.is_open()) {
                                cout << "File does not exist." << endl;
                                return 0;
                            } else if (!test2.is_open()) {
                                cout << "Invalid argument, file does not exist." << endl;
                                return 0;
                            }
                            else {finalCount+=2;}
                        }
                    }
                    else if (find(numMethods.begin(), numMethods.end(), name) != numMethods.end()) {
                        if (argc < finalCount + 2) {
                            cout << "Missing argument." << endl;
                            return 0;
                        }
                        else {
                            string firstFile = argv[2];
                            char *ptr = strstr(argv[1], ".tga");
                            char *ptr1 = strstr(argv[2], ".tga");
                            ifstream test1(argv[2], ios_base::binary);
                            bool isInt = false;
                            string num = argv[finalCount+1];
                            for (char j: num) {
                                if (isdigit(j)) { isInt = true; }
                                else { isInt = false; }
                            }
                            if (ptr == nullptr || ptr1 == nullptr) {
                                cout << "Invalid file name." << endl;
                                return 0;
                            }
                            else if (!test1.is_open()) {
                                cout << "File does not exist." << endl;
                                return 0;
                            }
                            else if (!isInt) {
                                cout << "Invalid argument, expected number." << endl;
                                return 0;
                            }
                            else {finalCount+=2;}
                        }
                    }
                    else if (find(pureMethods.begin(), pureMethods.end(), name) != pureMethods.end()) {
                        string firstFile = argv[2];
                        char *ptr = strstr(argv[1], ".tga");
                        char *ptr1 = strstr(argv[2], ".tga");
                        ifstream test1(argv[2], ios_base::binary);
                        if (ptr == nullptr || ptr1 == nullptr) {
                            cout << "Invalid file name." << endl;
                            return 0;
                        } else if (!test1.is_open()) {
                            cout << "File does not exist." << endl;
                            return 0;
                        } else {
                            finalCount++;
                        }
                    }
                    else if (name == "combine") {
                        if (argc < finalCount+3) {
                            cout << "Missing argument." << endl;
                            return 0;
                        }
                        else {
                            char *ptr = strstr(argv[1], ".tga");
                            char *ptr1 = strstr(argv[2], ".tga");
                            char *ptr2 = strstr(argv[finalCount+1], ".tga");
                            char *ptr3 = strstr(argv[finalCount+2], ".tga");
                            ifstream test1(argv[2], ios_base::binary);
                            ifstream test2(argv[finalCount+1], ios_base::binary);
                            ifstream test3(argv[finalCount+2], ios_base::binary);
                            if (ptr == nullptr || ptr1 == nullptr) {
                                cout << "Invalid file name." << endl;
                                return 0;
                            } else if (ptr2 == nullptr || ptr3 == nullptr) {
                                cout << "Invalid argument, invalid file name." << endl;
                                return 0;
                            } else if (!test1.is_open()) {
                                cout << "File does not exist." << endl;
                                return 0;
                            } else if (!test2.is_open() || !test3.is_open()) {
                                cout << "Invalid argument, file does not exist." << endl;
                                return 0;
                            } else {
                                finalCount+=3;
                            }
                        }
                    }
                }
                else {
                    cout << "Invalid method name." << endl;
                    return 0;
                }
            }
        }
    }
}
