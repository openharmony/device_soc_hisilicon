#####################################################################################################
#                                               Sample Running Guide
#####################################################################################################

1. Processors to Which the Sample Applies
   Hi3559V200 / Hi3556V200 / Hi3516AV300 / Hi3516DV300 / Hi3516CV500 / Hi3518EV300

2. LCD Dependency (Touch Function Not Supported)
  By default, LCD with the resolution of 320 x 240 (landscape mode) are supported. For other resolutions, adaptation is required. For details, see the sample_hifb.h file. Modify the screen and screen resolution information.

3. Driver Loading
   Load the .ko driver files of TDE, HiFB, and VO.

4. Font Library Adaptation
  By default, the simhei.ttf vector font library is used in the sample. Due to copyright restrictions, the release package does not contain any font library. To demonstrate the sample, obtain the font library from the C:\Windows\Fonts directory. The font library path is ./res/font/ttf.

5. Build of the Sample with the Touch Function
(1) Go to the middleware/sample/higv/auto_sample directory and run the make command to generate the auto_sample.bin file.
(2) Burn the auto_sample.bin file to the board by using the TFTP tool.
(3) Create the home/test directory on the board (LiteOS supports only the absolute path, and the root path of the sample resource files is home/test), and mount the auto_sample folder to the home/test directory. You are advised to use an SD card.
(4) Run the sample command over the serial port.
Corresponding images are displayed on the screen.

6. Sample Running
  After the auto sample is run, the automatic switchover of the control UI can be viewed, and the button control is displayed on the home screen.
#####################################################################################################
