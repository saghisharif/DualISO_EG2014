# HDR Reconstruction for Alternating Gain (ISO) Sensor Readout

This repository contains the source code for the HDR reconstruction of images captured with alternating gain (ISO) sensor readout, a technique discussed in the 2014 Eurographics Short Paper. This method is robust, sensor noise-aware, and primarily intended for images processed by the Magic Lantern firmware modification. The tool can also be applied to other types of dual ISO data to reconstruct a single HDR image. For more detailed information, refer to the [Eurographics Short Paper](https://diglib.eg.org/xmlui/handle/10.2312/egsh.20141006.025-028).

## Developed By
Saghi Hajisharif at the Visual Computing Laboratory (VCL) at Linköping University.

## Calibration Requirements
Proper calibration of the camera is crucial for accurate HDR reconstruction. The calibration process involves capturing multiple sets of images under different conditions:

1. White (Flat-field) and Black images at ISO 100.
2. White (Flat-field) and Black images at ISO 1600. Note: If using ISO 100-1600, the higher ISO can be set to any desired value.
3. White (Flat-field) and Black images with Dual-ISO mode activated on Magic Lantern (ML).

### Additional Notes:
- Black images should be captured in a dark room with the camera lens capped.
- White images should be captured with the lens removed or out-of-focus. If using a flat surface, ensure it does not saturate the pixels, which can lead to incorrect calibration data.
- Capture at least 40 images to ensure precise calibration.

### Calibration Software
The calibration program, written in Matlab, outputs a `.mat` file used by the main HDR reconstruction program.

## Dependencies
- **RawSpeed Library**: Modified for compatibility with Linux. [GitHub Repository](https://github.com/klauspost/rawspeed)
- **OpenCV**
- **Eigen**

Required configuration file included at `opensource/rawspeed/cameras.xml`.

## Usage
To run the program, use the following command format:
./dualiso inputfile.CR2 outputfile.exr cameras.xml Calibration.mat [options]


### Options
- `-h`: Kernel size (default = 1.2)
- `-fsizex`: Filter size in x-dimension (default = 11)
- `-fsizey`: Filter size in y-dimension (default = 11)
- `-M`: Order of the polynomial (0, 1, or 2; default = 2)

## Citation
If you use this tool in your research or wish to refer to the Eurographics short paper, please cite the following:


@inproceedings{10.2312:egsh.20141006,
booktitle = {Eurographics 2014 - Short Papers},
editor = {Eric Galin and Michael Wand},
title = {{HDR Reconstruction for Alternating Gain (ISO) Sensor Readout}},
author = {Hajisharif, Saghi and Kronander, Joel and Unger, Jonas},
year = {2014},
publisher = {The Eurographics Association},
ISSN = {1017-4656},
DOI = {10.2312/egsh.20141006}
}



## License
This project is released as free software under the terms specified in the LICENSE file.
