#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <stack>
#include <math.h>

using namespace std;




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
    vector<string> incorrectValues;
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
                incorrectValues.push_back(line);
            }
        }
    }
    saveIncorrectToTxt(incorrectValues);
    return data;
}

//Sorting algorithms

//quick sort

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
    if (leftIndex < rightIndex){
        int pivot = partition(data, leftIndex, rightIndex);
        quicksort(data, leftIndex, pivot - 1);
        quicksort(data, pivot + 1, rightIndex);
    }
}

void iterativeQuicksort(vector<int>& data, int leftIndex, int rightIndex) {
    stack<int> stack;

    stack.push(leftIndex);
    stack.push(rightIndex);

    while (!stack.empty()) {
        int end = stack.top(); stack.pop();
        int start = stack.top(); stack.pop();

        int pivot = partition(data, start, end);

        if (pivot - 1 > start) {
            stack.push(start);
            stack.push(pivot - 1);
        }

        if (pivot + 1 < end) {
            stack.push(pivot + 1);
            stack.push(end);
        }
    }
}

//merge sort

void mergeSortedArrays(vector<int>& data,  int const leftIndex,  int const middleIndex,  int const rightIndex){

    vector<int> leftSubArray(middleIndex - leftIndex + 1);
    vector<int> rightSubArray(rightIndex - middleIndex);

    int indexOfLeftSubArray = 0;
    int indexOfRightSubArray = 0;
    int indexOfMergedArray = leftIndex;

    for (int i = 0; i < leftSubArray.size(); i++){
        leftSubArray[i] = data[leftIndex + i];
    }

    for(int i =0; i< rightSubArray.size(); i++){
        rightSubArray[i] = data[middleIndex + 1 + i];
    }


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
    int bucketsNumber = 10;
    vector<vector<int>> buckets(bucketsNumber);

    for (int i : data) {
        int bucketIndex = i / bucketsNumber;
        buckets[bucketIndex].push_back(i);
    }

    for (vector<int>& bucket : buckets) {
        //mergeSort(bucket, 0, bucket.size() - 1);
        quicksort(bucket, 0, bucket.size() - 1);
    }

    int index = 0;
    for(vector<int>& bucket : buckets){
        for(int i : bucket){
            data[index++] = i;
        }
    }
}

//checking algorithms
void checkSpecificAlgorithm(const vector<int>& data, const string& algorithmName, void (*sortingAlgorithm)(vector<int>&, int, int)){
    vector<int> dataCopy = data;
    auto start = std::chrono::high_resolution_clock::now();
    sortingAlgorithm(dataCopy, 0, dataCopy.size()-1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    if(data.size()==10){
        cout<<"----------------------------------"<<endl;
        cout<<"Before sorting: "<<endl;
        showData(data);
        cout<<"After sorting: "<<endl;
        showData(dataCopy);
        cout<<"----------------------------------"<<endl;
    }

    string fileName = algorithmName + "ResultExtend.txt";

    ofstream file;
    file.open (fileName, ios_base::app);
    if(!file.is_open()){
        return ;
    }
    file<<"----------------------------------"<<endl;
    file<<"Amount of data: "<<data.size()<<endl;
    file<<"Time: "<<duration.count()<<endl;
    file<<"Sorting algorithm used: "<<algorithmName<<endl;
    file<<"Sorted correctly: "<<checkIfSortedCorrectly(dataCopy)<<endl;
    file<<endl;
    file<<"----------------------------------"<<endl;
    file.close();
    cout<<"Time: "<<duration.count()<<endl;
}
void checkAllSortingAlgorithms(const vector<int>& data, const string& algorithmName, void (*sortingAlgorithm)(vector<int>&, int, int)){
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

int main() {
    vector<vector<int>> database;
    vector<vector<int>> extendedDatabase;
    vector<string> sortingAlgorithmsName = {"quickSort", "iterativeQuickSort", "mergeSort", "bucketSort"};
    vector sortingFunctions = {quicksort, iterativeQuicksort, mergeSort, bucketSort};

    for (int i = 1; i < 7; i++) {
        for (int j = 1; j <= 4; j++) {
            int n;
            if(n>=pow(10,6)){
                break;
            }
            switch(j) {
                case 1:
                    n = pow(10, i);
                    break;
                case 2:
                    n = pow(10, i) * 2.5;
                    break;
                case 3:
                    n = pow(10, i) * 5.0;
                    break;
                default:
                    n = pow(10, i) * 7.5;
            }
            if(n==10 || n == 10000 || n == 100000 || n == 500000 || n == 1000000){
                auto start = std::chrono::high_resolution_clock::now();
                database.push_back(readData(n));
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration =std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                cout<<"Ilosc elementow: "<<n<<" Czas: "<<duration.count()<<"[ms]" <<endl;
            }
            extendedDatabase.push_back(readData(n));
        }
    }

    for(vector<int> data : database) {
        for (int i = 0; i < sortingAlgorithmsName.size(); i++) {
           checkAllSortingAlgorithms(data, sortingAlgorithmsName[i], sortingFunctions[i]);
           checkSpecificAlgorithm(data, sortingAlgorithmsName[i], sortingFunctions[i]);
        }}

}