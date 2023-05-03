#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <stack>

using namespace std;

int amountOfData = 0;

//Reading and saving data to files

//saving incorrect values to txt file
void saveIncorrectToTxt(const vector<string>& incorrectValues){
    ofstream file;
    file.open ("incorrect.txt");
    for(const auto & i : incorrectValues){
        file << i << endl;
    }
    file.close();
}

//function for reading specific amount of data from file
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

//overloaded function for reading all data from file
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

void swapElement(vector<int>& data, int index1, int index2){
    int temp = data[index1];
    data[index1] = data[index2];
    data[index2] = temp;
}

bool saveTestingResultToTxt(chrono::duration<double> duration, const string& sortingAlgorithmName, const vector<int>& data){
    ofstream file;
    file.open ("testingResaults.txt", ios_base::app);
    if(!file.is_open()){
        return false;
    }
    file<<"----------------------------------"<<endl;
    file<<"Amount of data: "<<data.size()<<endl;
    file<<"Time: "<<duration.count()<<endl;
    file<<"Sorting algorithm used: "<<sortingAlgorithmName<<endl;
    file<<"Sorted correctly: "<<checkIfSortedCorrectly(data)<<endl;
    file<<endl;
    file<<"----------------------------------"<<endl;
    file.close();
    return true;
}

void checkSortingAlgorithm(const vector<int>& data, string algorithmName, void (*sortingAlgorithm)(vector<int>&, int, int)){
    if(data.empty()){
        cout<<"Data is empty"<<endl;
        return;
    }
    vector<int> dataCopy = data;
    auto start = std::chrono::high_resolution_clock::now();
    sortingAlgorithm(dataCopy, 0, dataCopy.size()-1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    if(data.size() < 100){
        cout<<"Before sorting: "<<endl;
        showData(data);
        cout<<"After sorting: "<<endl;
        showData(dataCopy);
    }
    if(saveTestingResultToTxt(duration, algorithmName, dataCopy)) {
        cout << "Testing result saved to file" << endl;
    }else{
        cout<<"Error while saving testing result to file"<<endl;
    }
    cout<<"Time: "<<duration.count()<<endl;
}


//Sorting algorithms

//Quicksort

int partition(vector<int>& data, int leftIndex, int rightIndex) {
    int pivot = data[rightIndex];
    int i = leftIndex - 1;
    for (int j = leftIndex; j <= rightIndex - 1; j++) {
        if (data[j] < pivot) {
            i++;
            swapElement(data, i, j);
        }
    }
    swapElement(data, i + 1, rightIndex);
    return i + 1;
}

void quicksort(vector<int>& data, int leftIndex, int rightIndex) {
    if (leftIndex < rightIndex) {
        amountOfData++;
        if(amountOfData%1000 == 0){
            cout<<"Amount of data sorted: "<<amountOfData<<endl;
        }
        int pivot = partition(data, leftIndex, rightIndex);
        quicksort(data, leftIndex, pivot - 1);
        quicksort(data, pivot + 1, rightIndex);
    }
}

void iterativeQuicksort(vector<int>& data, int leftIndex, int rightIndex) {
    stack<int> stack;

    // Push the initial indices onto the stack
    stack.push(leftIndex);
    stack.push(rightIndex);

    while (!stack.empty()) {
        // Pop the indices from the stack
        int end = stack.top(); stack.pop();
        int start = stack.top(); stack.pop();

        // Partition the array and get the pivot index
        int pivot = partition(data, start, end);

        // If there are elements on the left side of the pivot, push their indices onto the stack
        if (pivot - 1 > start) {
            stack.push(start);
            stack.push(pivot - 1);
        }

        // If there are elements on the right side of the pivot, push their indices onto the stack
        if (pivot + 1 < end) {
            stack.push(pivot + 1);
            stack.push(end);
        }
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

//bucket sort

void bucketSort(vector<int>& data, int a, int b) {
    // buckets from range of 0 to 11
    int bucketsNumber = 11;
    vector<vector<int>> buckets(bucketsNumber);

    // assign elements to buckets
    for (int i : data) {
        int bucketIndex = i / bucketsNumber;
        buckets[bucketIndex].push_back(i);
    }

    // sort elements in each bucket
    for (vector<int>& bucket : buckets) {
        mergeSort(bucket, 0, bucket.size() - 1);
    }

    // merge buckets
    int index = 0;
    for(vector<int>& bucket : buckets){
        for(int i : bucket){
            data[index++] = i;
        }
    }
}

//check sorting algorithm


#pragma comment(linker, "/STACK:8000000")
int main() {
    vector<int> test = readData(10);
    vector<int> dane_10k = readData(10000);
    vector<int> dane_100k = readData(100000);
    vector<int> dane_500k = readData(500000);
    vector<int> tescik = readData(300000);
    vector<int> dane_1m = readData(1000000);
    vector<vector<int>> database = {test, dane_10k, dane_100k, dane_500k, dane_1m};

    //checkSortingAlgorithm(test, "quickSort", quicksort);
    //checkSortingAlgorithm(dane_10k, "iterativeQuickSort", quicksort);
    //checkSortingAlgorithm(dane_100k, "mergeSort", quicksort);
    checkSortingAlgorithm(tescik, "bucketSort", quicksort);
}

