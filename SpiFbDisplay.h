
#ifndef NIST_DEV_SPIFBDISPLAY_H
#define NIST_DEV_SPIFBDISPLAY_H

#include <linux/fb.h>
#include <opencv2/opencv.hpp>
#include <exception>

/**
 * Exception if link to frame buffer fails
 */
struct FrameBufferError : public std::exception {
    const char * what () const throw() {
        return "Unable to set up link to frame buffer!";
    }
};


/**
 * Class definition for the spi frame buffer implementation
 */
class SpiFbDisplay {
private:
    int fbfd = -1;
    char *fbp = 0;

    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
public:
    /**
     * Constructor which sets the memory for the screen informations
     */
    SpiFbDisplay();
    /**
     * Destructor to clear memory used by frame buffer
     */
    ~SpiFbDisplay();
    /**
     * Method to initialise the frame buffer link
     */
    void init();
    /**
     * Method to return the x-resolution of the screen
     * @return x-resolution
     */
    unsigned int getXres() {
        return vinfo.xres;
    }
    /**
     * Method to return the y-resolution of the screen
     * @return y-resolution
     */
    unsigned int getYres() {
        return vinfo.yres;
    }
    /**
     * Method to write new frame to the frame buffer
     *
     * @param image which gets written to the buffer
     */
    void update(cv::Mat *image);
    /**
     * Empty method to be conform with the ST7789 class
     */
    void close(void) {};
};


#endif //NIST_DEV_SPIFBDISPLAY_H
