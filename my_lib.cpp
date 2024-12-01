#include "my_lib.h"

using namespace std;
using namespace cv;

void setPixel(int x, int y, Mat& image, Vec3b color) {
    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
        image.at<Vec3b>(y, x) = color;
    }
}

void fillBackground(Mat& image, Vec3b color) {
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            setPixel(x, y, image, color);
        }
    }
}


vector<MyPoint> drawLine(int x1, int y1, int x2, int y2, Mat& image, Vec3b color) {
    int x = x1, y = y1;
    int dx = x2 - x1, dy = y2 - y1;
    int ix = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;  
    int iy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;  
    dx = abs(dx);
    dy = abs(dy);

    vector<MyPoint> points;

    if (dx >= dy) {
        int a = 2 * dy - dx;  
        for (int i = 0; i <= dx; i++) {
            points.push_back({ x, y });
            setPixel(x, y, image, color);
            if (a >= 0) {  
                y += iy;  
                a -= 2 * dx;  
            }
            x += ix;  
            a += 2 * dy;  
        }
    }
    
    else {
        int a = 2 * dx - dy; 
        for (int i = 0; i <= dy; i++) {
            points.push_back({ x, y });
            setPixel(x, y, image, color);  
            if (a >= 0) {  
                x += ix;   
                a -= 2 * dy;  
            }
            y += iy; 
            a += 2 * dx; 
        }
    }
    
    return points;
}

void drawThickLine(int x1, int y1, int x2, int y2, int thickness, Mat& image, Vec3b color) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    float length = sqrt(dx * dx + dy * dy);

    float x = static_cast<float>(dx) / length;
    float y = static_cast<float>(dy) / length;

    float perpX = -y;
    float perpY = x;

    int halfWidth = thickness / 2;

    MyPoint corner1 = { static_cast<int>(x1 - perpX * halfWidth), static_cast<int>(y1 - perpY * halfWidth) };
    MyPoint corner2 = { static_cast<int>(x1 + perpX * halfWidth), static_cast<int>(y1 + perpY * halfWidth) };
    MyPoint corner3 = { static_cast<int>(x2 + perpX * halfWidth), static_cast<int>(y2 + perpY * halfWidth) };
    MyPoint corner4 = { static_cast<int>(x2 - perpX * halfWidth), static_cast<int>(y2 - perpY * halfWidth) };

    drawPolygon({ corner1, corner2, corner3, corner4, corner1 }, image, color, color, "even");
}


void drawDashLine(int x1, int y1, int x2, int y2, int dash, int pass, int thickness, Mat& image, Vec3b color) {
    const int dashCount = 10;
    vector<MyPoint> points;

    double step = static_cast<double>(1) / dashCount;
    double part = static_cast<double>(dash) / (dash + pass);

    for (int i = 0; i <= dashCount; ++i) {
        double t = static_cast<double>(i) / dashCount;
        int x = static_cast<int>(x1 + (x2 - x1) * t);
        int y = static_cast<int>(y1 + (y2 - y1) * t);
        points.push_back({ x, y });

        x = static_cast<int>(x1 + (x2 - x1) * (t+step*part));
        y = static_cast<int>(y1 + (y2 - y1) * (t+step*part));
        points.push_back({ x, y });
    }

    for (int i = 0; i < points.size() - 1; i += 2) {
        if (i + 1 < points.size()) {
            drawThickLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, thickness, image, color);
        }
    }
}


int cross(const MyPoint& p1, const MyPoint& p2, const MyPoint& p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}


bool intersect(const MyPoint& p1, const MyPoint& p2, const MyPoint& p3, const MyPoint& p4) {
    int d1 = cross(p1, p2, p3);
    int d2 = cross(p1, p2, p4);
    int d3 = cross(p3, p4, p1);
    int d4 = cross(p3, p4, p2);

    if (d1 * d2 < 0 && d3 * d4 < 0) {
        return true;
    }

    return false;
}

bool hasIntersections(const vector<MyPoint>& points) {
    int n = points.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 2; j < n - 1; ++j) {
            if (j != i + 1) {
                if (intersect(points[i], points[i + 1], points[j], points[j + 1])) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isConvex(const vector<MyPoint>& points) {
    int n = points.size();
    if (n < 3) return false;

    int res = 0; 

    for (int i = 0; i < n; ++i) {
        int dx1 = points[(i + 1) % n].x - points[i].x;
        int dy1 = points[(i + 1) % n].y - points[i].y;
        int dx2 = points[(i + 2) % n].x - points[(i + 1) % n].x;
        int dy2 = points[(i + 2) % n].y - points[(i + 1) % n].y;

        int cross = dx1 * dy2 - dy1 * dx2;

        if (res == 0) {
            res = cross;
        }
        else {
            if (res * cross < 0) {
                return false;
            }
        }
    }
    return true; 
}


bool NonZeroWinding(const vector<MyPoint>& points, MyPoint point) {
    int windingNumber = 0;

    size_t numPoints = points.size();
    for (size_t i = 0; i < numPoints; ++i) {
        const MyPoint& p1 = points[i];
        const MyPoint& p2 = points[(i + 1) % numPoints];

        if (p1.y <= point.y) {
            if (p2.y > point.y && cross(p1, p2, point) > 0) {
                windingNumber++;
            }
        }
        else {
            if (p2.y <= point.y && cross(p1, p2, point) < 0) {
                windingNumber--;
            }
        }
    }

    return windingNumber != 0;
}


CLPointType Classify(double x1, double y1, double x2, double y2, double x, double y) {
    double ax = x2 - x1;
    double ay = y2 - y1;
    double bx = x - x1;
    double by = y - y1;
    double s = ax * by - bx * ay;
    if (s > 0) return LEFT;
    if (s < 0) return RIGHT;
    if ((ax * bx < 0) || (ay * by < 0)) //противоположно направлению
        return BEHIND; //позади
    if ((ax * ax + ay * ay) < (bx * bx + by * by))
        return BEHIND; //впереди
    if (x1 == x && y1 == y) //совпадает с началом
        return ORIGIN;
    if (x2 == x && y2 == y) //совпадает с концом
        return DESTINATION;
    return BETWEEN; //между
}

EType EdgeType(double ox, double oy, double dx, double dy, double ax, double ay) {
    switch (Classify(ox, oy, dx, dy, ax, ay)) {
    case LEFT:
        if (ay > oy && ay <= dy) return CROSS_LEFT; // пересекающая, A слева
        return INESSENTIAL; // безразличная
    case RIGHT:
        if (ay > dy && ay <= oy) return CROSS_RIGHT; // пересекающая, A справа
        return INESSENTIAL; // безразличная
    case BETWEEN:
    case ORIGIN:
    case DESTINATION:
        return TOUCHING; // касающаяся
    default:
        return INESSENTIAL; // безразличная
    }
}


PType PInPolygonEOMode(double x, double y, double* px, double* py, int n) {
    int param = 0;
    for (int i = 0; i < n; i++) {
        switch (EdgeType(px[i], py[i], px[(i + 1) % n], py[(i + 1) % n], x, y)) {
        case TOUCHING: // если лежит на полигоне, то заведомо принадлежит
            return INSIDE;
        case CROSS_LEFT:
        case CROSS_RIGHT:
            param = 1 - param; // изменяем значение четности
        case INESSENTIAL:
            break;
        }
    }
    return (param == 1) ? INSIDE : OUTSIDE; // нечетное количество пересечений - внутри, четное - снаружи
}

void floodNonZeroWinding(const vector<MyPoint>& points, Vec3b background, Mat& image) {
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;

    for (const auto& pt : points) {
        minX = min(minX, pt.x);
        minY = min(minY, pt.y);
        maxX = max(maxX, pt.x);
        maxY = max(maxY, pt.y);
    }

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (NonZeroWinding(points, {x, y})) {
                setPixel(x, y, image, background);
            }
        }
    }
}

void floodEvenOdd(const vector<MyPoint>& points, Vec3b background, Mat& image) {
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;

    int n = points.size();
    vector<double> px(n), py(n);

    for (int i = 0; i < n; i++) {
        px[i] = points[i].x;
        py[i] = points[i].y;
    }

    for (const auto& pt : points) {
        minX = min(minX, pt.x);
        minY = min(minY, pt.y);
        maxX = max(maxX, pt.x);
        maxY = max(maxY, pt.y);
    }
    
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (PInPolygonEOMode(x, y, px.data(), py.data(), n) == INSIDE) {
                setPixel(x, y, image, background);
            }
        }
    }
}


void drawPolygon(const vector<MyPoint>& points, Mat& image, Vec3b borderColor, Vec3b background, string method) {   
    if (method == "non") {
        floodNonZeroWinding(points, background, image);
    }
    else {
        floodEvenOdd(points, background, image);
    }
    for (int i = 0; i < points.size() - 1; ++i) {
        vector<MyPoint> border = drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, image, borderColor);
    }
}