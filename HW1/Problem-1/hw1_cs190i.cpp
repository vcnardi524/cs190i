//
// Created by Victor Nardi on 1/22/24.
//
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

int sampleSize = 8000;
std::string fileP1 = "fileP1.csv";
std::vector<std::string> fileLine = { "Sample #",
                                    "Monte Carlo with Uniform PDF", 
                                    "Monte Carlo Estimation with PDF proprtional to y = pi/2 - x",
                                    "Monte Carlo Estimation with PDF proportional to y = cos(x) using inversion method",
                                    "Monte Carlo Estimation with PDF proportional to y = cos(x) using rejection method"
                                  };




double uniformA(int sampleSize);
double samplingB(int sampleSize);
double inverseB(double x);
double pdfB(double x);
double samplingC(int sampleSize);
double inverseC(double x);
double pdfC(double x);
double cosRej(int sampleSize);
double drawSamplRej();
bool writeRecordToFile(std::string fileName, std::vector<std::string> contents);
bool emptyFile(std::string fileName);



//SAMPLING DISTRIBUTIONS//

//Problem A
double uniformA(int sampleSize){
    double xSample;
    double ySample;
    double estmr = 0;
    for(int i = 0; i < sampleSize; ++i){
        xSample  = std::rand() * 1.0 / RAND_MAX;
        xSample *= (M_PI / 2);  // transforming to 0:PI/2
        ySample  = std::cos(xSample);
        estmr   +=  ySample * (M_PI / 2) / sampleSize;
    }
    return estmr;
}

//Problem B
double samplingB(int sampleSize){
    double eSample;
    double estm = 0;
    for(int i = 0; i < sampleSize; ++i){
        eSample = std::rand() * 1.0 / RAND_MAX;
        estm += std::cos(inverseB(eSample)) / pdfB(eSample);
    }
    return estm /= (float) sampleSize;
}
double inverseB(double x){ 
    return ( (M_PI / 2) * (1 - std::sqrt(1 - x)) ); 
}
double pdfB(double x){
    return ( (4 / M_PI) - ((inverseB(x) * 8) / (M_PI * M_PI)) ); 
}

//Problem C
double samplingC(int sampleSize){
    double eSample;
    double estm = 0;
    for(int i = 0; i < sampleSize; ++i){
        eSample = std::rand() * 1.0 / RAND_MAX;
        estm += std::cos(inverseC(eSample)) / pdfC(eSample);
    }
    return estm /= (float) sampleSize;
}
double inverseC(double x){ 
    return ( std::asin(x) ); 
}
double pdfC(double x){
    return ( std::cos(std::asin(x)) ); 
}

//Problem D
double drawSamplRej(){
    double xSample = 0;
    double ySample = 2;
    while(std::cos(xSample) < ySample){
        xSample = std::rand() * 1.0 / RAND_MAX;
        xSample *= (M_PI / 2);  // transforming to 0:PI/2
        ySample = std::rand() * 1.0 / RAND_MAX; //from 0 to 1 for cos(x)
    }
    return xSample; // FIXME idk if this is right return
}

double cosRej(int sampleSize){
    double sample;
    double estmr = 0;
    double x;
    for(int i = 0; i < sampleSize; ++i){
        x = drawSamplRej();
        estmr += std::cos(x) / std::cos(x); // f(xSample) / pdf(x)
    }
    estmr /= (float) sampleSize;
    return estmr; //fixme currently converging to 0.9
}



//HELPER FUNCTION//

bool writeRecordToFile(std::string fileName, std::vector<std::string> contents){    
    size_t length = contents.size();
    std::ofstream file;
    file.open(fileName, std::ios_base::app);

    if(length != 5)
        throw runtime_error("insufficient args");

    if(file.is_open()){

        for(size_t i = 0; i < contents.size(); ++i){
            if(i == 4) 
                file << contents.at(i) << std::endl;
            else
                file << contents.at(i) << ",";
        }
    }
    file.close();

    return true;
}
bool emptyFile(std::string fileName){
    std::ofstream file;
    file.open(fileName, std::ios_base::trunc);
    if (!file.is_open()) {
        std::cerr << "Error opening the file: " << fileName << std::endl;
        return 1;
    }

    file.close();
    return true;
}


int main(){
    double estimator = 0;
    int seed = std::time(NULL);
    srand(seed);
    emptyFile(fileP1);
    
    //Monte carlo estimator
    //
    //writeRecordToFile(fileP1, fileLine);
    fileLine.clear();
    for (int j = 0; j < sampleSize; j += 5){
        
        fileLine.push_back(std::to_string(j));
        
        //part a
        estimator = 0; //resets the estimator
        estimator = uniformA(j);
        fileLine.push_back(std::to_string(estimator));


        //part b
        estimator = 0;
        estimator = samplingB(j);
        fileLine.push_back(std::to_string(estimator));

        //part c
        estimator = 0;
        estimator = samplingC(j);
        fileLine.push_back(std::to_string(estimator));

        //part d
        estimator = 0;
        estimator = cosRej(sampleSize);
        fileLine.push_back(std::to_string(estimator));
       
        writeRecordToFile(fileP1, fileLine);
        fileLine.clear();
    }
    return 0;
}