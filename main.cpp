#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


//Reading and saving data to files
void saveIncorrectToTxt(const vector<string>& incorrectValues){
    ofstream file;
    file.open ("incorrect.txt");
    for(const auto & i : incorrectValues){
        file << i << endl;
    }
    file.close();
}

vector<int> readData(int amount) {
    ifstream file("../projekt2_dane.csv");
    vector<int> data;
    string line;
    getline(file, line); //skipping first line in file

    while (getline(file, line) && data.size() < amount) {
        if(!line.empty()){
            int ratingPosition = line.find_last_of(',') + 1;
            try{
                string rating = line.substr(ratingPosition);
                int ratingInt = stoi(rating);
                data.push_back(ratingInt);
            }catch (const std::invalid_argument& e) {
            }
        }
    }
    return data;
}

//overloaded function for reading all data
vector<int> readData() {
    ifstream file("../projekt2_dane.csv");
    vector<int> data;
    string line;
    vector<string> incorrectValues;
    getline(file, line); // //skipping first line in file

    while (getline(file, line)) {
        if(!line.empty()){
            int ratingPosition = line.find_last_of(',') + 1;
            try{
                string rating = line.substr(ratingPosition);
                int ratingInt = stoi(rating);
                data.push_back(ratingInt);
            }catch (const std::invalid_argument& e) {
                incorrectValues.push_back(line);
            }
        }
    }
    saveIncorrectToTxt(incorrectValues);
    return data;
}


//Utility functions

bool checkIfSortedCorrectly(const vector<int>& data){
    for(int i = 0; i < data.size()-1; i++){
        if(data[i] > data[i+1]){
            return false;
        }
    }
    return true;
}

void showData(const vector<int>& data){
    for(int i : data){
        cout<<i<< " ";
    }
    cout<<endl;
}

void swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}


//Quicksort
int partition(vector<int>& data, int leftIndex, int rightIndex) {
    int pivot = data[rightIndex];
    int i = leftIndex - 1;
    for (int j = leftIndex; j <= rightIndex - 1; j++) {
        if (data[j] < pivot) {
            i++;
            swap(data[i], data[j]);
        }
    }
    swap(data[i + 1], data[rightIndex]);
    return i + 1;
}


void quicksort(vector<int>& data, int leftIndex, int rightIndex) {
    if (leftIndex < rightIndex) {
        int pivot = partition(data, leftIndex, rightIndex);
        quicksort(data, leftIndex, pivot - 1);
        quicksort(data, pivot + 1, rightIndex);
    }
}

//merge sort

void mergeSortedArrays(vector<int>& data,  int const leftIndex,  int const middleIndex,  int const rightIndex){

    //divide data into two sub arrays
    vector<int> leftSubArray(middleIndex - leftIndex + 1);
    vector<int> rightSubArray(rightIndex - middleIndex);

    //declaring indexes for sub arrays
    int indexOfLeftSubArray = 0;
    int indexOfRightSubArray = 0;
    int indexOfMergedArray = leftIndex;

    //copy data to sub arrays
    for (int i = 0; i < leftSubArray.size(); i++){
        leftSubArray[i] = data[leftIndex + i];
    }

    for(int i =0; i< rightSubArray.size(); i++){
        rightSubArray[i] = data[middleIndex + 1 + i];
    }


    //merging sub arrays
    while(indexOfLeftSubArray < leftSubArray.size() && indexOfRightSubArray < rightSubArray.size()){
        if(leftSubArray[indexOfLeftSubArray] <= rightSubArray[indexOfRightSubArray]){
            data[indexOfMergedArray] = leftSubArray[indexOfLeftSubArray];
            indexOfLeftSubArray++;
        }else{
            data[indexOfMergedArray] = rightSubArray[indexOfRightSubArray];
            indexOfRightSubArray++;
        }
        indexOfMergedArray++;
    }


    //copying remaining elements of left and right sub arrays
    while (indexOfLeftSubArray < leftSubArray.size()) {
        data[indexOfMergedArray++] = leftSubArray[indexOfLeftSubArray++];
    }

    while (indexOfRightSubArray < rightSubArray.size()) {
        data[indexOfMergedArray++] = rightSubArray[indexOfRightSubArray++];
    }
}

void mergeSort(vector<int>& data, int const leftIndex, int const rightIndex){
    if(leftIndex >= rightIndex){
        return;
    }
    int middleIndex = leftIndex + (rightIndex - leftIndex) / 2;
    mergeSort(data, leftIndex, middleIndex);
    mergeSort(data, middleIndex + 1, rightIndex);
    mergeSortedArrays(data, leftIndex, middleIndex, rightIndex);
}



int main() {
    vector<int> test = readData(10);
    vector<int> dane_10k = readData(10000);
    vector<int> dane_100k = readData(100000);
    vector<int> dane_500k = readData(500000);
    vector<int> dane_1m = readData(1000000);
    vector<int> dane_max = readData();

    cout << "Wczytano dane" << endl;
    cout<<"Dane 10k: "<<dane_10k.size()<<endl;
    cout<<"Dane 100k: "<<dane_100k.size()<<endl;
    cout<<"Dane 500k: "<<dane_500k.size()<<endl;
    cout<<"Dane 1m: "<<dane_1m.size()<<endl;
    cout<<"Dane max: "<<dane_max.size()<<endl;

    cout<<"--------------PRZED SORTOWANIEM-------------- "<<endl;
    showData(test);

    cout<<"Czy posortowana [test]: "<<checkIfSortedCorrectly(test)<<endl;
    cout<<"Czy posortowana [100k]: "<<checkIfSortedCorrectly(dane_100k)<<endl;


    //quicksort(dane_100k,0,dane_100k.size()-1);
    //quicksort(test,0,test.size()-1);
    mergeSort(test,0,test.size()-1);
    mergeSort(dane_100k,0,dane_100k.size()-1);

    cout<<"--------------PO SORTOWANIU-------------- "<<endl;
    showData(test);

    cout<<"Czy posortowana [test]: "<<checkIfSortedCorrectly(test)<<endl;
    cout<<"Czy posortowana [100k]: "<<checkIfSortedCorrectly(dane_100k)<<endl;

}

