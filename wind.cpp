#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <math.h>

using namespace std;

int width = 500, height = 500;
int max_parts = 200;


class Point {
public:
    class Wind {
        public:
            int weight = 1;
            int speed_x, speed_y;
            Wind(int speed_x, int speed_y):
                speed_x{speed_x}, speed_y{speed_y}
            { }
            Wind(): Wind(0, 0)
            { }

            void addWeight() { this->weight++; }
            int getWeight() { return weight; }
    };

    int x, y;
    Wind wind;
    Point(int x, int y): x{x}, y{y}
    {
        int speed_x = -2 + rand() % 5;
        int speed_y = -2 + rand() % 5;
        wind = Wind(speed_x, speed_y);
    }
    Point(): Point(0, 0)
    { }

    void move()
    {
        x += wind.speed_x;
        y += wind.speed_y;
    }

    void speedup(Point& other)
    {
        if(!(other.wind.speed_x == other.wind.speed_y && other.wind.speed_x == 0) &&
                this->wind.getWeight() <= other.wind.getWeight()) {
            this->wind = other.wind;
            other.wind.addWeight();
        }
    }
};

void outputParts(vector<Point> points, size_t points_count) {
    for(size_t i = 0; i < points_count; i++) {
        cout << points[i].x << " " << points[i].y << endl;
    }
    cout << "END_LIST\n";
}


int main(int argc, char* argv[]) {
    srand((unsigned) time(NULL));

    if(argc > 1)
        width = atoi(argv[1]);
    if(argc > 2)
        height = atoi(argv[2]);
    if(argc > 3)
        max_parts = atoi(argv[3]);

    vector<Point> points { };
    while(size(points) < max_parts) {
        points.push_back(Point(width / 4 + rand() % (width / 2),
                                height / 4 + rand() % (height / 2)));
    }

    for(;;) {
        // Endless wind
        // if(size(points) < max_parts)
        //    points.push_back(Point(width / 4 + rand() % width,
        //                           height / 4 + rand() % height));
        for(size_t i = 0; i < size(points); i++) {
            points[i].move();
            for(size_t j = 0; j < size(points); j++) {
                if(i == j)
                    continue;
                int dist = (int)(sqrt(
                            pow(points[j].x - points[i].x, 2) +
                            pow(points[j].y - points[i].y, 2))
                            );
                if(dist <= 10) {
                    points[j].speedup(points[i]);
                }
            }
            if(points[i].x > width || points[i].x < 0 ||
                    points[i].y > height || points[i].y < 0)
                points.erase(points.begin() + i);
        }
        outputParts(points, size(points));
        this_thread::sleep_for(chrono::milliseconds(50)); // FPS
    }
}
