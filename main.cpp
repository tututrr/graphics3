#include "my_lib.h" 

using namespace std;
using namespace cv;


void showImage(const Mat image) {
    imshow("Display window", image);
}

void saveImage(Mat* image, string name) {
    imwrite("/Users/hisoka/Downloads/graphics3/result/" + name, *image);
}

void printPoints(const vector<MyPoint>& points) {
    for (const auto& point : points) {
        cout << "(" << point.x << ", " << point.y << ")" << endl;
    }
}

void Lines() {
   Mat lines = Mat::zeros({400, 400}, CV_8UC3);

   fillBackground(lines, BLACK);
   drawLine(100, 100, 200, 300, lines, MAGENTA); 
   drawLine(200, 100, 100, 200, lines, MAGENTA);
   drawLine(300, 250, 350, 50, lines, WHITE);

    vector<MyPoint> points1 = drawLine(0, 0, 8, 3, lines, CYAN);
    cout << "Points for line from (0, 0) to (8, 3):" << endl;
    printPoints(points1);

    vector<MyPoint> points2 = drawLine(8, 3, 0, 0, lines, CYAN);
    cout << "Points for line from (8, 3) to (0, 0):" << endl;
    printPoints(points2);

    vector<MyPoint> points3 = drawLine(0, 0, 3, 8, lines, CYAN);
    cout << "Points for line from (0, 0) to (3, 8):" << endl;
    printPoints(points3);

    vector<MyPoint> points4 = drawLine(3, 8, 0, 0, lines, CYAN);
    cout << "Points for line from (3, 8) to (0, 0):" << endl;
    printPoints(points4);

    vector<MyPoint> points5 = drawLine(0, 0, -8, 3, lines, CYAN);
    cout << "Points for line from (0, 0) to (-8, 3):" << endl;
    printPoints(points5);


   saveImage(&lines, "lines.jpg");
}

void DefaultPolygons() {
    Mat polygonsDraw = Mat::zeros({ 1000, 1000 }, CV_8UC3);

    fillBackground(polygonsDraw, BLACK);

    vector<MyPoint> star = {
        {450, 250},
        {88, 368},
        {312, 60},
        {312, 440},
        {88, 132},
        {450, 250},
    };

    vector<MyPoint> star2 = {
        {650, 560},
        {288, 678},
        {512, 370},
        {512, 750},
        {288, 442},
        {650, 560},
    };

    vector<MyPoint> fig1 = {
        {700, 505},
        {800, 605},
        {700, 705},
        {900, 705},
        {1000, 605},
        {900, 505},
        {800, 405},
        {750, 555},
        {850, 655},
        {950, 555},
        {850, 455},
        {750, 455},
        {700, 505}
    };

    vector<MyPoint> fig2 = {
        {600, 200},
        {700, 300},
        {600, 400},
        {800, 400},
        {900, 300},
        {800, 200},
        {700, 100},
        {650, 250},
        {750, 350},
        {850, 250},
        {750, 150},
        {650, 150},
        {600, 200} 
    };

    drawPolygon(star, polygonsDraw, WHITE, MAGENTA, "even"); 
    drawPolygon(star2, polygonsDraw, WHITE, SILVER, "non");
    drawPolygon(fig1, polygonsDraw, WHITE, PINK, "non");
    drawPolygon(fig2, polygonsDraw, WHITE, PURPLE, "even");

    saveImage(&polygonsDraw, "polygons.jpg");
}

void Intersections() {
    Mat intersectionsDraw = Mat::zeros({ 1000, 1000 }, CV_8UC3);

    fillBackground(intersectionsDraw, BLACK);

    vector<vector<MyPoint>> shapes = {
        {
            {50, 50},
            {50, 100},
            {150, 100},
            {150, 50},
        },
        {
            {350, 300},
            {550, 500},
            {350, 500},
            {550, 300}, 
        },
        {
            {450, 250},
            {88, 368},
            {312, 60},
            {312, 440},
            {88, 132},
            {450, 250},
        },
        {
            {600, 600},
            {600, 700},
            {700, 700},
            {700, 600},
            {650, 650},
            {600, 600}
        },
        {
            {800, 400},
            {850, 400},
            {825, 350},
            {825, 450},
            {810, 450},
            {840, 470},
            {800, 400}
        }
    };

    for (int i = 0; i < 5; ++i) {
        Vec3b color = SILVER;

        if (hasIntersections(shapes[i])) {
            color = MAGENTA;
        }
        string method = "non";
        drawPolygon(shapes[i], intersectionsDraw, WHITE, color, method);
    }

    saveImage(&intersectionsDraw, "intersections.jpg");
}

void ConvexPolygons() {
    Mat convexDraw = Mat::zeros({ 1000, 1000 }, CV_8UC3);

    fillBackground(convexDraw, BLACK);

    vector<vector<MyPoint>> shapes = {
        {
            {850, 850},
            {850, 700},
            {750, 700},
            {850, 850},
        },
        {
            {500, 300},
            {550, 250},
            {575, 275},
            {600, 350},
            {550, 400},
            {500, 350},
            {500, 300}
        },
        {
            {450, 250},
            {88, 368},
            {312, 60},
            {312, 440},
            {88, 132},
            {450, 250},
        },
        {
            {850, 400},
            {900, 450},
            {950, 400},
            {900, 500},
            {900, 550},
            {850, 500},
            {800, 550},
            {850, 400}
        }
    };


    for (int i = 0; i < 4; ++i) {
        Vec3b color = BLUE;
        if (isConvex(shapes[i])) {
            color = PINK;
        }
        string method = "even";
        drawPolygon(shapes[i], convexDraw, WHITE, color, method);
    }

    saveImage(&convexDraw, "convex.jpg");
}

void ThickLines() {
    Mat thickLines = Mat::zeros({ 400, 400 }, CV_8UC3);

    fillBackground(thickLines, BLACK);
    drawThickLine(40, 200, 360, 40, 50, thickLines, WHITE);
    drawDashLine(70, 290, 280, 310, 6, 2, 20, thickLines, MAGENTA);
    drawDashLine(200, 150, 350, 200, 3, 8, 30, thickLines, BLUE);
    drawDashLine(150, 350, 150, 400, 4, 5, 50, thickLines, PINK);
    drawDashLine(300, 390, 50, 50, 5, 6, 4, thickLines, SILVER);

    saveImage(&thickLines, "thick_and_dashed_lines.jpg");
}

int main()
{
    Lines();
    DefaultPolygons();
    Intersections();
    ConvexPolygons();
    ThickLines();

    return 0;
}
