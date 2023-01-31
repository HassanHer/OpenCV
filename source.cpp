#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    
    //Image windows' declaration
    Mat image;
    Mat imageGr;
    Mat faceCrop;
    
    //Classifier declarations and model loading
    CascadeClassifier faceCascade;
    CascadeClassifier mouthCascade;
    CascadeClassifier noseCascade;
    faceCascade.load("Resources/haarcascade_frontalface_default.xml");
    mouthCascade.load("Resources/haarcascade_mcs_mouth.xml");
    noseCascade.load("Resources/haarcascade_mcs_nose.xml");
    if (faceCascade.empty()) { cout << "fXML not found"; return 0; }
    if (mouthCascade.empty()) { cout << "mXML not found"; return 0; }
    if (noseCascade.empty()) { cout << "nXML not found"; return 0; }
    
    //Vector declarations
    vector<Rect> faces;
    vector<Rect> mouthRect;
    vector<Rect> noseRect;

    //Input variable
    char x = 0;

    cout << ".. WELCOME TO PROJECT X ..\n";

PROGRAM:
    bool normCamera = false;
    bool greyCamera = false;

    cout << "\t MENU\n";
    cout << "Enter the number of the command you want to execute:\n 1-Open webcam\n 2-Open webcam in detailed\n 3-Exit\n" << endl;
    cin >> x;
    switch (x)
    {
    case '1':
        normCamera = true;
        break;
    case '2':
        normCamera = true;
        greyCamera = true;
        break;
    case '3':
        return 0;
        break;
    default:
        cout << "Unknown Input.";
        goto PROGRAM;
    }

    //Webcam
    VideoCapture cap(0);

    while (true)
    {
        //Webcam capture
        cap.read(image);

        //Conversion to gray, and detecting faces in the gray image
        cvtColor(image, imageGr, COLOR_BGR2GRAY);
        faceCascade.detectMultiScale(imageGr, faces, 1.1, 10);

        //Faces detected in the gray image
        for (int i = 0; i < faces.size(); i++)
        {
            
            mouthCascade.detectMultiScale(imageGr, mouthRect, 1.1, 75);
            noseCascade.detectMultiScale(imageGr, noseRect, 1.1, 35);
            for (int i = 0; i < mouthRect.size(); i++)
            {
                rectangle(imageGr, mouthRect[i].tl(), mouthRect[i].br(), Scalar(0, 255, 0), 10);
            }

            for (int i = 0; i < noseRect.size(); i++)
            {
                rectangle(imageGr, noseRect[i].tl(), noseRect[i].br(), Scalar(0, 255, 0), 10);
            }

            if (mouthRect.size() > 0 || noseRect.size() > 0)
            {
                faceCrop = image(faces[i]);
                imwrite("nomask/Face" + to_string(i) + ".png", faceCrop);
            }

            rectangle(image, faces[i].tl(), faces[i].br(), Scalar(0, 0, 255), 8);           
        }

        //Conditions for determining whether a person is wearing a mask, wearing a mask improperly or not wearing a mask at all.
        if (faces.size() > 0)
        {                   
            if (mouthRect.size() == 0 && noseRect.size() == 0)
                putText(image, "Mask found!", Point(0, 35), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
            else if (mouthRect.size() == 0)
                putText(image, "You are not wearing your mask properly.", Point(0, 35), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 110, 255), 2);
            else putText(image, "Mask not found.", Point(0, 35), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        }
        else putText(image, "Face not found.", Point(0, 35), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);     
        
        if (normCamera)
            imshow("Detection system", image);

        if (greyCamera)
            imshow("Gray Image", imageGr);
        
        waitKey(1);
        if (waitKey(1) == 27)
            break;
    }
    cap.release();
    destroyAllWindows();
    system("cls");
    goto PROGRAM;
}