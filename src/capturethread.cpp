/*
 * Copyright (c) Özkan Pakdil <ozkan.pakdil@gmail.com>
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <qimageblitz/qimageblitz.h>
#include "capturethread.h"
#include "settings.h"
#include "imageeffect.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

void xioctl(int fh, int request, void *arg) {
    int r;
    do {
        r = v4l2_ioctl(fh, request, arg);
    } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

    if (r == -1) {
        qCritical() << "error " << errno << " " << strerror(errno);
        return;
    }
}

CaptureThread::CaptureThread() {
    effect = 0;
    running = false;
}

void CaptureThread::updateImageSettings() {

    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;

    memset (&queryctrl, 0, sizeof (queryctrl));
    memset (&control, 0, sizeof (control));
    queryctrl.id = V4L2_CID_BRIGHTNESS;
    control.id = V4L2_CID_BRIGHTNESS;

    if (-1 == v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
        if (errno != EINVAL) {
            qCritical() << "VIDIOC_QUERYCTRL";
            exit (EXIT_FAILURE);
        } else {
            qDebug() << "V4L2_CID_BRIGHTNESS is not supported.";
        }
    } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        qDebug() << "V4L2_CID_BRIGHTNESS is not supported.";
    } else {
        control.value = (queryctrl.maximum - queryctrl.minimum) * Settings::brightness()/100;
        if (-1 == v4l2_ioctl (fd, VIDIOC_S_CTRL, &control)) {
            qCritical() << "VIDIOC_S_CTRL";
            exit (EXIT_FAILURE);
        }
    }

    memset (&queryctrl, 0, sizeof (queryctrl));
    memset (&control, 0, sizeof (control));
    queryctrl.id = V4L2_CID_CONTRAST;
    control.id = V4L2_CID_CONTRAST;

    if (-1 == v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
        if (errno != EINVAL) {
            qCritical() << "VIDIOC_QUERYCTRL";
            exit (EXIT_FAILURE);
        } else {
            qDebug() << "V4L2_CID_CONTRAST is not supported.";
        }
    } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        qDebug() << "V4L2_CID_CONTRAST is not supported.";
    } else {
        control.value = (queryctrl.maximum - queryctrl.minimum) * Settings::contrast()/100;
        if (-1 == v4l2_ioctl (fd, VIDIOC_S_CTRL, &control)) {
            qCritical() << "VIDIOC_S_CTRL";
            exit (EXIT_FAILURE);
        }
    }


    memset (&queryctrl, 0, sizeof (queryctrl));
    memset (&control, 0, sizeof (control));
    queryctrl.id = V4L2_CID_SATURATION;
    control.id = V4L2_CID_SATURATION;

    if (-1 == v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
        if (errno != EINVAL) {
            qCritical() << "VIDIOC_QUERYCTRL";
            exit (EXIT_FAILURE);
        } else {
            qDebug() << "V4L2_CID_SATURATION is not supported.";
        }
    } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        qDebug() << "V4L2_CID_SATURATION is not supported.";
    } else {
        control.value = (queryctrl.maximum - queryctrl.minimum) * Settings::saturation()/100;
        if (-1 == v4l2_ioctl (fd, VIDIOC_S_CTRL, &control)) {
            qCritical() << "VIDIOC_S_CTRL";
            exit (EXIT_FAILURE);
        }
    }


    memset (&queryctrl, 0, sizeof (queryctrl));
    memset (&control, 0, sizeof (control));
    queryctrl.id = V4L2_CID_HUE;
    control.id = V4L2_CID_HUE;

    if (-1 == v4l2_ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
        if (errno != EINVAL) {
            qCritical() << "VIDIOC_QUERYCTRL";
            exit (EXIT_FAILURE);
        } else {
            qDebug() << "V4L2_CID_HUE is not supported.";
        }
    } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        qDebug() << "V4L2_CID_HUE is not supported.";
    } else {
        control.value = (queryctrl.maximum - queryctrl.minimum) * Settings::hue()/100;
        if (-1 == v4l2_ioctl (fd, VIDIOC_S_CTRL, &control)) {
            qCritical() << "VIDIOC_S_CTRL";
            exit (EXIT_FAILURE);
        }
    }

}


// process video data
void CaptureThread::run() {
    while (devam) {
        mutex.lock();
        do {
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* Timeout. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            r = select(fd + 1, &fds, NULL, NULL, &tv);
        } while ((r == -1 && (errno = EINTR)));

        if (r == -1) {
            qDebug() << "select";
            quit();
            return;
        }

        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        xioctl(fd, VIDIOC_DQBUF, &buf);

        if (v4lconvert_convert(v4lconvert_data,
                               &src_fmt,
                               &fmt,
                               (unsigned char*)buffers[buf.index].start, buf.bytesused,
                               dst_buf, fmt.fmt.pix.sizeimage) < 0) {
            if (errno != EAGAIN)
                qDebug() << "v4l_convert";
        }

        unsigned char* asil=(unsigned char*)malloc(fmt.fmt.pix.sizeimage+qstrlen(header));
        memmove(asil, dst_buf, fmt.fmt.pix.sizeimage);
        memmove(asil+qstrlen(header), asil, fmt.fmt.pix.sizeimage);
        memcpy(asil,header,qstrlen(header));

        QImage *qq=new QImage();

        if (qq->loadFromData(asil,fmt.fmt.pix.sizeimage+qstrlen(header), "PPM")) {

            if (Settings::normalize()) {
                Blitz::normalize(*qq);
            }

            ImageEffect::applyEffect(*qq, effect);

            QTransform outTransform;
            outTransform.scale(Settings::mirror() ? -1 : 1, Settings::flip() ? -1 : 1);

            emit renderedImage(qq->transformed(outTransform));
        }
        free(asil);
        delete qq;
        if (delay>0) {
            this->msleep(delay);
        }
        xioctl(fd, VIDIOC_QBUF, &buf);
        di++;
        mutex.unlock();
    }
}

int CaptureThread::stop() {
    running = false;
    devam=false;
    mutex.lock();
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(fd, VIDIOC_STREAMOFF, &type);
    for (unsigned int i = 0; i < n_buffers; ++i)
        v4l2_munmap(buffers[i].start, buffers[i].length);

    v4l2_close(fd);
    fd = -1;
    mutex.unlock();
    quit();
    return 0;
}

int CaptureThread::start() {
    wait();

    devam=false;
    fd = -1;

    // read config
    dev_name = Settings::node();
    width    = Settings::width();
    height   = Settings::height();
    fps      = Settings::fps();
    if (fps>0) {
        delay = 1000/fps;
    }
    else { delay = 0; }

    // open webcam device node
    fd = v4l2_open(dev_name.toStdString().c_str(), O_RDWR | O_NONBLOCK, 0);
    if (fd < 0) {
        qCritical() << "Cannot open device";
        quit();
        return 1;
    }

    updateImageSettings();

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = width;
    fmt.fmt.pix.height      = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    xioctl(fd, VIDIOC_S_FMT, &fmt);
    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) {
        qCritical() << "Libv4l didn't accept RGB24 format. Can't proceed.";
        quit();
        return 1;
    }
    emit startedCapture(fmt.fmt.pix.width, fmt.fmt.pix.height);

    v4lconvert_data = v4lconvert_create(fd);
    if (v4lconvert_data == NULL)
        qDebug() << "v4lconvert_create";
    if (v4lconvert_try_format(v4lconvert_data, &fmt, &src_fmt) != 0)
        qDebug() << "v4lconvert_try_format";
    xioctl(fd, VIDIOC_S_FMT, &src_fmt);
    dst_buf = (unsigned char*)malloc(fmt.fmt.pix.sizeimage);

    CLEAR(req);
    req.count = 2;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    xioctl(fd, VIDIOC_REQBUFS, &req);

    buffers = (buffer*)calloc(req.count, sizeof(*buffers));
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        CLEAR(buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        xioctl(fd, VIDIOC_QUERYBUF, &buf);

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
                                             PROT_READ | PROT_WRITE, MAP_SHARED,
                                             fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start) {
            qDebug() << "mmap";
            quit();
            return 1;
        }
    }

    for (unsigned int i = 0; i < n_buffers; ++i) {
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        xioctl(fd, VIDIOC_QBUF, &buf);
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(fd, VIDIOC_STREAMON, &type);

    di=0;
    sprintf(header,"P6\n%d %d 255\n",fmt.fmt.pix.width,fmt.fmt.pix.height);
    devam=true;

    // start processing video data
    running = true;
    QThread::start();
    return 0;
}
