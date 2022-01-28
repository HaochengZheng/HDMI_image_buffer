# HDMI_image_buffer
This is a script that allows you to show images/videos when you are using command lind version of Raspberry Pi OS.
It requires OpenCV and boost log(Only for debug, feel free to delete)

This script will directly write to Raspberry Pi's frame buffer.

Here is an example on how to use it:

SpiFbDisplay display;
display.init(); // init the display first
//Assume I have a image
Mat image:
display.update(& image);//now the image is written to frame buffer

You can also use display.getYres() and display.getXres() to acquire x and y resolution of output
