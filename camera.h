#ifndef CAMERA_H
#define CAMERA_H
#include<cmath>
class Camera {
public:
        double zoom, dzoom, panfriction, zoomfriction, ratio, x, y, delta, dx, dy;
        int lastX, lastY, xres, yres;

        Camera(double x, double y, int xres, int yres) {
                dzoom = 0.0;
                zoom = 1;
                delta = 0.0;
                ratio = yres/((double)xres);
                this->x = x;
                this->y = y;
                this->dx = 0;
                this->dy = 0;
                this->xres = xres;
                this->yres = yres;
                zoomfriction = 0.994;
                panfriction = 0.9;
        }
        virtual ~Camera() {}


        void addVel(int lastX, int lastY) {
                this->lastX = lastX;
                this->lastY = lastY;
                dx += ((lastX > xres -10 ) * (-(lastX - xres + 10)*0.00001) +
                            (lastX < 10) * (-(lastX-10)*0.00001))/2;
                dy += ((lastY < 10) * ((lastY - 10)*0.00001) +
                            (lastY > yres - 10) * ((lastY - yres + 10)* 0.00001))/2;
        }
        void modZoom(double mod) {
                dzoom += mod;
        }

        void iter() {
                x += dx;
                y += dy;
                dx *= panfriction;
                dy *= panfriction;
                zoom *= dzoom + 1;
                dzoom *= zoomfriction;

                //qDebug("%4.4f %4.4f\n", temp.x, temp.y);
                x += xPixToDouble(lastX)*(-fabs(dzoom*5));
                y += yPixToDouble(lastY)*(-fabs(dzoom*5));
                if (zoom < 0.01) {
                        zoom = 0.01;
                } else if (zoom > 32) {
                        zoom = 32;
                }
        }

        double xPixToDouble(int x) {
                return ((x*2)/((double)xres)-1);
        }
        double yPixToDouble(int y) {
                return (1-(y*2)/((double)yres));
        }
        int xDoubleToPix(double x) {
                return (-x+1)/xres;
        }
        int yDoubleToPix(double y) {
                return (y+1)/yres;
        }
};
#endif

