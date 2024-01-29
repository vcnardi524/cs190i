#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;
int DIM = 300;
//sampling methods
void uniformSample(int sampleSize, cv::Mat &image, cv::Mat &sampleImage);
void strategicSample(int sampleSize, cv::Mat &image, cv::Mat &sampleImage);

//helper
int  getIndex(std::vector<double> &cdfVec);
int  getIndex(std::vector<double> &cdfVec,  int box, int dim);
void createCDF(std::vector<unsigned int> &pdfVec, std::vector<double> &cdfVec, double magnitude);
void createCDF(std::vector<double> &cdfVec, double magnitude);
void inverseGray(cv::Mat &image);




void uniformSample(int sampleSize, cv::Mat &image, cv::Mat &sampleImage){
    std::vector<unsigned int> oneDImage; // ~ pixels in images
    std::vector<double> cdfImage; // ~ pixels in images
    double magnitude = 0; 
    int count  = 0;
    int row;
    int col;
    int index;
    double xSample;


    for(int i = 0; i < (int)image.rows; ++i){
        for(int j = 0; j < (int)image.cols; ++j){
            oneDImage.push_back(static_cast<int>(image.at<uchar>(i, j)));
            magnitude += oneDImage.at(count);
            count++;
        }
    }

    createCDF(oneDImage, cdfImage, magnitude);

    for(int i = 0; i < sampleSize; ++i){

        index = getIndex(cdfImage);
        // std::cout << index << std::endl;
        col = index %  image.cols;
        row = (index - col) / image.cols - 1;

        sampleImage.at<uchar>(row, col) = 255 - oneDImage.at(index);
    }
    return; //fixme stub
}

void strategicSample(int sampleSize, cv::Mat &image, cv::Mat &sampleImage){
    int dim = DIM;
    int samplePerBox = 1;
    std::vector<double> rowVector; 
    std::vector<double> colVector;

    double sum = 0;
    double magnitude = 0;
    int rowIndex;
    int colIndex;

    for(int i = 0; i < image.rows; ++i){
        sum = 0;
        for(int j = 0; j < image.cols; ++j){
            sum += image.at<uchar>(i, j);
        }
        colVector.push_back(sum);
        magnitude += sum;
    }
    createCDF(colVector, magnitude);

    for(int x = 0; x < dim; ++x){     ///// for each box 
                std::cout << x << std::endl;

        for(int i = 0; i < dim; ++i){ /////


            for(int j = 0; j < samplePerBox; ++j){ //# of samples

                rowIndex = getIndex(colVector, x, dim);
                magnitude = 0;
                for(int k = 0; k < image.cols; ++k){
                    rowVector.push_back(image.at<uchar>(rowIndex, k));
                    magnitude += rowVector.at(k);
                }
                
                createCDF(rowVector, magnitude);
                colIndex = getIndex(rowVector, i, dim);
                rowVector.clear();

                //std::cout << rowIndex << " = row, column =" << colIndex << std::endl;
                sampleImage.at<uchar>(rowIndex, colIndex) = (unsigned int)255 - image.at<uchar>(rowIndex, colIndex);
            }

        }
    }
}


int getIndex(std::vector<double> &cdfVec){
    double xSample = std::rand() * 1.0 / RAND_MAX;
    auto it = std::lower_bound(cdfVec.begin(), cdfVec.end(), xSample);
    return (it - cdfVec.begin());
}
int getIndex(std::vector<double> &cdfVec, int box, int dim){

    double xSample = std::rand() * 1.0 / RAND_MAX;
    xSample /= dim;
    xSample += static_cast<float>(box)/dim;

    auto it = std::lower_bound(cdfVec.begin(), cdfVec.end(), xSample);

    return (it - cdfVec.begin());
}
void createCDF(std::vector<unsigned int> &pdfVec, std::vector<double> &cdfVec, double magnitude){
    double sum = 0;
    for(size_t i = 0; i < pdfVec.size(); ++i){
        sum += pdfVec.at(i) / magnitude;
        cdfVec.push_back(sum);
    }

}
void createCDF(std::vector<double> &cdfVec, double magnitude){ //fixme unsigned int might have
    double sum = 0;
    for(size_t i = 0; i < cdfVec.size(); ++i){
        sum += cdfVec.at(i) / magnitude;
        cdfVec.at(i) = sum;
    }
}
void inverseGray(cv::Mat &image){
    for(int i = 0; i < image.rows; ++i){
        for(int j = 0; j < image.cols; ++j){
            //inverse gray scale 
            image.at<uchar>(i, j) = (unsigned int)255 - image.at<uchar>(i, j); 
        }
    }
}

int main() {
    // Read the image file
    cv::Mat image = cv::imread("/Users/victor/projects/cs190i/Problem-2/photos/obama.jpeg", 0);
    cv::Mat uImage = cv::Mat::ones(image.rows, image.cols, CV_8U) * 255; 
    cv::Mat sImage = cv::Mat::ones(image.rows, image.cols, CV_8U) * 255; 

    int sampleSize = DIM * DIM;
    std::cout << image.rows * image.cols << std::endl;
    bool check;
    
    

    //inverse gray scale
    inverseGray(image);
    //sample
    uniformSample(sampleSize, image, uImage);
    strategicSample(sampleSize, image, sImage);
    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image" << std::endl;
        return -1;
    }
    
    
    check = imwrite("/Users/victor/projects/cs190i/Problem-2/photos/obamaSampleRandom.jpeg", uImage); 
    if (check == false) { 
        std::cout << "Mission - Saving the image, FAILED" << endl; 
        cin.get(); 
        return -1; 
    }
    
    check = imwrite("/Users/victor/projects/cs190i/Problem-2/photos/obamaSampleStrat.jpeg", sImage); 
    if (check == false) { 
        std::cout << "Mission - Saving the image, FAILED" << endl; 
        cin.get(); 
        return -1; 
    }

    cv::namedWindow("Display Image", cv::WINDOW_NORMAL);
    cv::imshow("Display Image", image);
    cv::waitKey(0);

    return 0;
}