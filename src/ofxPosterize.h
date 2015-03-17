//
//  ofxPosterize.h
//
//  Created by alberto on 2/9/13.
//
//

#ifndef __ofxPosterize__
#define __ofxPosterize__

#include <iostream>
#include <map>

#include "ofImage.h"
#include "ofxCv.h"

class ofxPosterize {
    ofImage image;
    cv::Mat mat;
    
public:
    ofxPosterize() {};
    ~ofxPosterize() {};
    
    //------------------------------------------------------------ color average
    static
    ofImage average(ofImage& image, int targetColorNumber) {
        ofImage result;
        cv::Mat src = ofxCv::toCv(image).clone();
        cv::Mat averageGrid;
        targetColorNumber = floor( sqrt(targetColorNumber)+0.5f );
        int height = src.rows/targetColorNumber;
        int width = src.cols/targetColorNumber;
        
        averageGrid = cv::Mat::zeros(height*targetColorNumber, width*targetColorNumber, src.type());
        
        for(int i=0; i<targetColorNumber; i++) {
            for(int j=0; j<targetColorNumber; j++) {
                cv::Mat roi = src( cv::Rect(j*width, i*height, width, height) );
                
                cv::Mat mean, stddev;
                meanStdDev(roi,mean,stddev);
                
                cv::rectangle(averageGrid,
                              cv::Point(j*width, i*height),
                              cv::Point(j*width+width, i*height+height),
                              cv::Scalar(mean.at<double>(0,0),
                                         mean.at<double>(0,1),
                                         mean.at<double>(0,2)),
                              CV_FILLED);
            }
        }
        
        ofxCv::toOf(averageGrid, result);
        result.update();
        return result;
    };
    
    //------------------------------------------------------------ kmeans
    static
    ofImage clusterize(ofImage& image, int targetColorNumber, int *colours) {
        ofImage result;
        cv::Mat src = ofxCv::toCv(image).clone();
        cv::Mat clustered, colored, bestLabels, centers, samples;
        vector<cv::Mat> bgr;
        int cols = src.cols;
        int rows = src.rows;
        int numpix = cols*rows;
        int K = targetColorNumber;
        
        int colors[K];
        for(int i = 0;i < K;i++){
            colors[i] = 255 / (i + 1);
        }
        
        cv::split(src, bgr);
        samples = cv::Mat::zeros(cols*rows, src.channels(), CV_32F);
        
        // i think there is a better way to split pixel bgr color
        for(int i = 0;i < numpix;i++) {
            if(src.channels()==1) {
                samples.at<float>(i, 0) = bgr[0].data[i] / 255.0;
            } else if(src.channels()==3) {
                samples.at<float>(i, 0) = bgr[0].data[i] / 255.0; // normalized between 0 and 1
                samples.at<float>(i, 1) = bgr[1].data[i] / 255.0;
                samples.at<float>(i, 2) = bgr[2].data[i] / 255.0;
            }
        }
        
        // call kmeans algorithm!
        cv::kmeans(samples, K, bestLabels,
                   cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
                   3, cv::KMEANS_PP_CENTERS, centers);
        
        // i think there is a better way to do this mayebe some Mat::reshape?
        clustered = cv::Mat(rows, cols, CV_32F);
        for(int i = 0;i < numpix;i++){
            int label = bestLabels.at<int>(0, i);
            clustered.at<float>(i / cols, i % cols) = (float)((colors[label]));
        }
        clustered.convertTo(clustered, CV_8U);
        
        // for each cluster..
        colored = cv::Mat(src.rows, src.cols, CV_8UC3);
        for(int i=0; i<K; i++) {
            cv::Mat mask;
            cv::Scalar meanColor;
            int color = 255 / (i + 1);
            
            // get the cluster as a mask
            cv::inRange(clustered, color - 1, color + 1, mask);
            
            // find the average color
            meanColor = cv::mean(src, mask);

            colours[i] = int(round(meanColor[0]));
            
            if(src.channels()==1) {
                colored.setTo(colours[i], mask);
            } else if(src.channels()==3) {
                colored.setTo(meanColor, mask);
            }
        }
        
        ofxCv::toOf(colored, result);
        result.update();
        return result;
    };
    
    //------------------------------------------------------------ extract colors
    static
    map<int, int> getHistogram(ofImage& image){
        map<int, int> colors;
        for(int x=0; x<image.width; x++) {
            for(int y=0; y<image.height; y++) {
                ofColor c = image.getColor(x, y);
                colors[c.getHex()]++;
            }
        }
        return colors;
    };
    
private:
    
};


#endif /* defined(__ofxPosterize__) */
