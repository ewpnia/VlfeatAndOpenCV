/*
#include<stdio.h>

extern "C" {
#include <vl/generic.h>
}

int main (int argc, const char * argv[]) {
  VL_PRINT ("Hello world!\n") ;
  system("Pause");
  return 0;
}
*/
/*
#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("test.jpg");
    if(img.empty())
    {
        cout<<"error";
        return -1;
    }
    imshow("test",img);
    waitKey();

    return 0;
}
*/

extern "C"{
#include <vl/generic.h>
#include <vl/stringop.h>
#include <vl/pgm.h>
#include <vl/sift.h>
#include <vl/getopt_long.h>
};


#include <stdio.h>
#include <tchar.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//#pragma comment(lib, "opencv_core230d.lib")
//#pragma comment(lib, "opencv_imgproc230d.lib")
//#pragma comment(lib, "opencv_highgui230d.lib")
//#pragma comment(lib, "opencv_objdetect230d.lib")

using namespace cv;
using namespace std;

extern "C"{
#include <vl/generic.h>
#include <vl/stringop.h>
#include <vl/pgm.h>
#include <vl/sift.h>
#include <vl/getopt_long.h>
};

int _tmain(int argc, _TCHAR* argv[])
{
    VL_PRINT ("Hello world!\n") ;
    char *ImagePath="test.jpg";
    IplImage *Image=cvLoadImage(ImagePath,0);
    //  int min=0;
    //  min=Image->width>Image->height?Image->height:Image->width;
    int noctaves=4,nlevels=2,o_min=0;
    // noctaves=(int)(log(min)/log(2));
    vl_sift_pix *ImageData=new vl_sift_pix[Image->height*Image->width];
    unsigned char *Pixel;
    for (int i=0;i<Image->height;i++)
    {
        for (int j=0;j<Image->width;j++)
        {
            Pixel=(unsigned char*)(Image->imageData+i*Image->width+j);
            ImageData[i*Image->width+j]=*(Pixel);
        }
    }
    VlSiftFilt *SiftFilt=NULL;
    SiftFilt=vl_sift_new(Image->width,Image->height,noctaves,nlevels,o_min);
    int KeyPoint=0;
    int idx=0;
    if (vl_sift_process_first_octave(SiftFilt,ImageData)!=VL_ERR_EOF)
    {
        while (TRUE)
        {
            //����ÿ���еĹؼ���
            vl_sift_detect(SiftFilt);
            //����������ÿ����
            KeyPoint+=SiftFilt->nkeys;
            VlSiftKeypoint *pKeyPoint=SiftFilt->keys;
            for (int i=0;i<SiftFilt->nkeys;i++)
            {
                VlSiftKeypoint TemptKeyPoint=*pKeyPoint;
                pKeyPoint++;
                cvDrawCircle(Image,cvPoint(TemptKeyPoint.x,TemptKeyPoint.y),TemptKeyPoint.sigma/2,CV_RGB(255,0,0));
                idx++;
                //���㲢����ÿ����ķ���
                double angles[4];
                int angleCount=vl_sift_calc_keypoint_orientations(SiftFilt,angles,&TemptKeyPoint);
                for (int j=0;j<angleCount;j++)
                {
                    double TemptAngle=angles[j];
                    printf("%d: %f\n",j,TemptAngle);
                    //����ÿ�����������
                    float *Descriptors=new float[128];
                    vl_sift_calc_keypoint_descriptor(SiftFilt,Descriptors,&TemptKeyPoint,TemptAngle);
                    int k=0;
                    while (k<128)
                    {
                        printf("%d: %f",k,Descriptors[k]);
                        printf("; ");
                        k++;
                    }

                    printf("\n");
                    delete []Descriptors;
                    Descriptors=NULL;
                }
            }
            //��һ��
            if (vl_sift_process_next_octave(SiftFilt)==VL_ERR_EOF)
            {
                break;
            }
            //free(pKeyPoint);
            KeyPoint=NULL;
        }
    }
    vl_sift_delete(SiftFilt);
    delete []ImageData;
    ImageData=NULL;
    cvNamedWindow("Source Image",1);
    cvShowImage("Source Image",Image);
    cvWaitKey(0);
    cvReleaseImage(&Image);
    cvDestroyAllWindows();
    return 0;
}