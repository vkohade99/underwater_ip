#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(){

  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
  VideoCapture cap("/home/taluqdaar/13june2019/pq_gate.avi");

  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  while(1){

    Mat my_img;
    // Capture frame-by-frame
    cap >> my_img;

    // If the frame is empty, break immediately
    if (my_img.empty())
      break;

			int filterFactor = 1;
	    //Mat my_img = imread("~/catkin_ws/src/underwater_ip/portrait.jpg",IMREAD_COLOR);
	    Mat orig_img = my_img.clone();
			namedWindow("original", WINDOW_AUTOSIZE);
	    imshow("original",my_img);

	    Mat simg;

	    cvtColor(my_img, simg, CV_BGR2GRAY);

	    long int N = simg.rows*simg.cols;

	    int histo_b[256];
	    int histo_g[256];
	    int histo_r[256];

	    for(int i=0; i<256; i++){
	        histo_b[i] = 0;
	        histo_g[i] = 0;
	        histo_r[i] = 0;
	    }
	    Vec3b intensity;

	    for(int i=0; i<simg.rows; i++){
	        for(int j=0; j<simg.cols; j++){
	            intensity = my_img.at<Vec3b>(i,j);

	            histo_b[intensity.val[0]] = histo_b[intensity.val[0]] + 1;
	            histo_g[intensity.val[1]] = histo_g[intensity.val[1]] + 1;
	            histo_r[intensity.val[2]] = histo_r[intensity.val[2]] + 1;
	        }
	    }

	    for(int i = 1; i<256; i++){
	        histo_b[i] = histo_b[i] + filterFactor * histo_b[i-1];
	        histo_g[i] = histo_g[i] + filterFactor * histo_g[i-1];
	        histo_r[i] = histo_r[i] + filterFactor * histo_r[i-1];
	    }

	    int vmin_b=0;
	    int vmin_g=0;
	    int vmin_r=0;
	    int s1 = 3;
	    int s2 = 3;

	    while(histo_b[vmin_b+1] <= N*s1/100){
	        vmin_b = vmin_b +1;
	    }
	    while(histo_g[vmin_g+1] <= N*s1/100){
	        vmin_g = vmin_g +1;
	    }
	    while(histo_r[vmin_r+1] <= N*s1/100){
	        vmin_r = vmin_r +1;
	    }

	    int vmax_b = 255-1;
	    int vmax_g = 255-1;
	    int vmax_r = 255-1;

	    while(histo_b[vmax_b-1]>(N-((N/100)*s2)))
	    {
	        vmax_b = vmax_b-1;
	    }
	    if(vmax_b < 255-1){
	        vmax_b = vmax_b+1;
	    }
	    while(histo_g[vmax_g-1]>(N-((N/100)*s2)))
	    {
	        vmax_g = vmax_g-1;
	    }
	    if(vmax_g < 255-1){
	        vmax_g = vmax_g+1;
	    }
	    while(histo_r[vmax_r-1]>(N-((N/100)*s2)))
	    {
	        vmax_r = vmax_r-1;
	    }
	    if(vmax_r < 255-1){
	        vmax_r = vmax_r+1;
	    }

	    for(int i=0; i<simg.rows; i++)
	    {
	        for(int j=0; j<simg.cols; j++)
	        {

	            intensity = my_img.at<Vec3b>(i,j);

	            if(intensity.val[0]<vmin_b){
	                intensity.val[0] = vmin_b;
	            }
	            if(intensity.val[0]>vmax_b){
	                intensity.val[0]=vmax_b;
	            }


	            if(intensity.val[1]<vmin_g){
	                intensity.val[1] = vmin_g;
	            }
	            if(intensity.val[1]>vmax_g){
	                intensity.val[1]=vmax_g;
	            }


	            if(intensity.val[2]<vmin_r){
	                intensity.val[2] = vmin_r;
	            }
	            if(intensity.val[2]>vmax_r){
	                intensity.val[2]=vmax_r;
	            }

	            my_img.at<Vec3b>(i,j) = intensity;
	        }
	    }

	    for(int i=0; i<simg.rows; i++){
	        for(int j=0; j<simg.cols; j++){

	            intensity = my_img.at<Vec3b>(i,j);
	            intensity.val[0] = (intensity.val[0] - vmin_b)*255/(vmax_b-vmin_b);
	            intensity.val[1] = (intensity.val[1] - vmin_g)*255/(vmax_g-vmin_g);
	            intensity.val[2] = (intensity.val[2] - vmin_r)*255/(vmax_r-vmin_r);
	            my_img.at<Vec3b>(i,j) = intensity;
	        }
	    }


	    // sharpen image using "unsharp mask" algorithm
	    Mat blurred; double sigma = 1, threshold = 5, amount = 1;
	    GaussianBlur(my_img, blurred, Size(), sigma, sigma);
	    Mat lowContrastMask = abs(my_img - blurred) < threshold;
	    Mat sharpened = my_img*(1+amount) + blurred*(-amount);
	    my_img.copyTo(sharpened, lowContrastMask);
			namedWindow("New Image", WINDOW_AUTOSIZE);
	    imshow("New Image",sharpened);

    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
  }

  // When everything done, release the video capture object
  cap.release();

  // Closes all the frames
  destroyAllWindows();

}
