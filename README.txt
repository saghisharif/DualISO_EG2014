*************************************************************
HDR reconstruction for alternating gain (ISO) sensor readout
*************************************************************

DualISO reconstruction project is free software that performs a very robust and sensor noise-aware method on the output of the dualISO image that has been captured with Magic Lantern modification. This software can be used on other types of data for the reconstruction of a single HDR image. For more information please refer to the paper: https://diglib.eg.org/xmlui/handle/10.2312/egsh.20141006.025-028


This program has been developed by the Visual Computing Laboratory (VCL) at Linköping University. 

***************************************************************************************

The program requires careful calibration of the camera, where the following pictures are captured:

1- White (Flat-field) and Black images with ISO 100 
2- White (Flat-field) and Black images with ISO 1600 (if you are going to use ISO 100-1600, the higher ISO can be set to whatever you wish)
3- White (Flat-field) and Back images with Dual-ISO mode activated on ML.

*Note: The Black images are captured in the dark room with the camera lens on.

**Note: The white images are captured either with the lens removed or out-of-focus. If you have a very flat surface that can work too but be careful not to saturate the pixels which will create the wrong calibration data for the camera. 

***Note: The more images you capture, the more accurate the calibration will be. Approximately 40 images or more are required for calibration.

****Note: The calibration program is currently under Matlab and it will save the output as a mat file which can be read by the program.

****************************************************************************************
Dependencies:

-This program is dependent on the rawspeed library. https://github.com/klauspost/rawspeed
This library has been modified so that it works also under Linux. Based on this the program requires an XML file which has been included in the folder opensource/rawspeed/cameras.xml. 

-openCV, Eigen

****************************************************************************************
How to run the program:

The input arguments: ./dualiso inputfile.CR2 outputfile.exr cameras.xml Calibration.mat 
-h 		kernel size default value = 1.2
-fsizex Filter size x-dim default value = 11
-fsizey Filter size y-dim default value = 11
-M 		Selects the order of the polynomial (0,1,2) default = 2
