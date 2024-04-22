*************************************************************
HDR reconstruction for alternating gain (ISO) sensor readout
*************************************************************

DualISO reconstruction project is a free software that performs a very robust and sensor noise aware method on the output of the dualISO image that has been captured with Magic Lantern modification. This software can be used on other type of data for the reconstruction of single HDR image. For more information please refer to the paper: http://vcl.itn.liu.se/publications/2014/HKU14/


This program has been develeped by Visual Computing Laboratory (VCL) at Linkoping university. 

***************************************************************************************

The program requires a carefule calibration of the camera, where the following pictures are captured:

1- White (Flat-field) and Black images with ISO 100 
2- White (Flat-field) and Black images with ISO 1600 (if you are going to use ISO 100-1600, the higher ISO can be set to whatever you wish)
3- White (Flat-field) and Back images with Dual-ISO mode activate on ML.

*Note: The Black images are captured in the dark room with camera lens on.

**Note: The white images are captured either with the lens removed or out-of-focued. If you have a very flat surface that can work too but be careful no to saturate the pixels which will create a wrong calibration data for the camera. 

***Note: The more images you capture, the more accurate the calibration will be. Approximately 40 images or more are required for calibration.

****Note: The calibration program is currently under matlab and it will save the output as mat file which can be read by the program.

****************************************************************************************
Dependencies:

-This program is dependent on rawspeed library. https://github.com/klauspost/rawspeed
This library has been modified so that it works also under Linux. Based on this the program requires an xml file which has been included in the folder opensource/rawspeed/cameras.xml. 

-openCV, Eigen

****************************************************************************************
How to run the program:

The input arguments: ./dualiso inputfile.CR2 outputfile.exr cameras.xml Calibration.mat 
-h 		kernel size default value = 1.2
-fsizex Filter size x-dim default value = 11
-fsizey Filter size y-dim default value = 11
-M 		Selects the order of the polynomial (0,1,2) default = 2