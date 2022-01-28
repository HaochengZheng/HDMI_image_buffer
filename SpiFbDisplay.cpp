#include "SpiFbDisplay.h"

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>


SpiFbDisplay::SpiFbDisplay() {
    memset(&finfo, 0, sizeof(finfo));
    memset(&vinfo, 0, sizeof(vinfo));
}


void SpiFbDisplay::init() {
    fbfd = open("/dev/fb0", O_RDWR);

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        BOOST_LOG_TRIVIAL(fatal) << "Unable to read fixed screen information!";
        throw FrameBufferError();
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        BOOST_LOG_TRIVIAL(fatal) << "Unable to read variable screen information!";
        throw FrameBufferError();
    }
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
        BOOST_LOG_TRIVIAL(fatal) << "Error setting variable information!";
        throw FrameBufferError();
    }

    fbp = (char*) mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
}


void SpiFbDisplay::update(cv::Mat *image) {
    cv::Mat imageBGRA;
    cv::cvtColor(*image, imageBGRA, cv::COLOR_RGBA2BGRA);

    if (finfo.smem_len != imageBGRA.total() * imageBGRA.elemSize()) {
        BOOST_LOG_TRIVIAL(error) << "Image size doesn't match frame buffer size!";
        throw FrameBufferError();
    }

    uchar *data;
    data = imageBGRA.data;

    if ((int) fbp == -1) {
        BOOST_LOG_TRIVIAL(error) << "Failed to mmap frame buffer!";
        throw FrameBufferError();
    }
    else
        memcpy(fbp, data, finfo.smem_len);

}


SpiFbDisplay::~SpiFbDisplay() {
    if (fbp != 0)
        munmap(fbp, finfo.smem_len);
    if (fbfd != -1)
        ::close(fbfd);
}