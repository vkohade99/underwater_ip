#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const int r1_max = 255, s1_max = 255, r2_max = 255, s2_max = 255;
int r1=70, s1=0, r2=140, s2=255;

static void on_trackbar( int, void* )
{
}

int computeOutput(int x, int r1, int s1, int r2, int s2)
{
    float result;
    if(0 <= x && x <= r1){
        result = s1/r1 * x;
    }else if(r1 < x && x <= r2){
        result = ((s2 - s1)/(r2 - r1)) * (x - r1) + s1;
    }else if(r2 < x && x <= 255){
        result = ((255 - s2)/(255 - r2)) * (x - r2) + s2;
    }
    return (int)result;
}

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

    Mat image;
    // Capture frame-by-frame
    cap >> image;

    // If the frame is empty, break immediately
    if (image.empty())
      break;

			Mat new_image = image.clone();

      namedWindow("Original Image", 1);
	    imshow("Original Image", image);
      image *= 1./255;
      cvtColor(image,image, CV_BGR2GRAY);
      // Try for 113,29,87,7
      createTrackbar( "r1", "Original Image", &r1, r1_max, on_trackbar );
      createTrackbar( "s1", "Original Image", &s1, s1_max, on_trackbar );
      createTrackbar( "r2", "Original Image", &r2, r2_max, on_trackbar );
      createTrackbar( "s2", "Original Image", &s2, s2_max, on_trackbar );

	    for(int y = 0; y < image.rows; y++){
	        for(int x = 0; x < image.cols; x++){
	            for(int c = 0; c < 3; c++){
	                int output = computeOutput(image.at<Vec3b>(y,x)[c], r1, s1, r2, s2);
	                new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(output);
	            }
	        }
	    }



	    namedWindow("New Image", 1);
	    imshow("New Image", new_image);
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
