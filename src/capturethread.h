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

#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include "libv4l2.h"
#include "libv4lconvert.h"

class CaptureThread : public QThread
{
    Q_OBJECT

public:
    CaptureThread();
    int stop();
    int start();
    bool running;
protected:
    void run();
signals:
    void renderedImage(const QImage &image);
    void startedCapture(int width, int height);

private:
    QMutex mutex;

    int width, height, fps, delay;

    bool devam;

    struct buffer {
        void   *start;
        size_t length;
    };
    struct buffer                   *buffers;

    struct v4l2_format              fmt;
    struct v4l2_buffer              buf;
    struct v4l2_requestbuffers      req;
    enum v4l2_buf_type              type;
    fd_set                          fds;
    struct timeval                  tv;
    int                             r, fd;
    unsigned int                    n_buffers;
    QString                         dev_name;

    struct v4l2_format src_fmt;
    unsigned char *dst_buf;
    struct v4lconvert_data *v4lconvert_data;
    int di;
    char header [50];
};

#endif // CAPTURETHREAD_H
