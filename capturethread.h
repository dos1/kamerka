#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QWaitCondition>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
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
