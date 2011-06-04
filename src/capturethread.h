/*
 * Copyright (c) Özkan Pakdil <ozkan.pakdil@gmail.com>
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 (or higher) of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QWaitCondition>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include "libv4l2.h"
#include "libv4lconvert.h"

class CaptureThread : public QThread
{
    Q_OBJECT

public:
    CaptureThread(QObject *parent = 0);
    ~CaptureThread();
    bool devam;
    bool storeImage;
    int autoshoot;

    struct v4l2_format              fmt;
    struct v4l2_buffer              buf;
    struct v4l2_requestbuffers      req;
    enum v4l2_buf_type              type;
    fd_set                          fds;
    struct timeval                  tv;
    int                             r, fd;
    unsigned int                    n_buffers;
    char                            *dev_name;
    char                            out_name[256];
    FILE                            *fout;
    struct buffer {
            void   *start;
            size_t length;
    };
    static void xioctl(int fh, int request, void *arg)
    {
            int r;

            do {
                    r = v4l2_ioctl(fh, request, arg);
            } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

            if (r == -1) {
                    fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
                    return;
            }
    };
    struct buffer                   *buffers;

    void stopUlan();
    int startUlan();
    void setFileName(QString);
    void setDeviceName(QString);
protected:
    void run();
signals:
    void renderedImage(const QImage &image);

private:
    QMutex mutex;
    QWaitCondition condition;
    void closeVideoOut();
    QString fileName;
    QString deviceName;
};

#endif // CAPTURETHREAD_H
