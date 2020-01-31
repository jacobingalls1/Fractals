#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

int main() {

    srand(time(NULL));

    const int SCREEN_SIZE = 1000, POINTS_ON_SCREEN = 10000, SPAWN_RATE = 500;
    int waveSpeed = 100;

    vector<Vector2f> vertices = {Vector2f(500, 0), Vector2f(0, 1000), Vector2f(1000, 1000)};
    int y = rand() % 1000;
    vector<Vector2f> points = {Vector2f(0, 0)};
    int randVert = -1;


    RenderWindow window(VideoMode(SCREEN_SIZE, SCREEN_SIZE), "Fractal");
    window.requestFocus();

    float time = 0;
    bool color = false;
    bool full = false;
    int place = 0;

    while( window.isOpen() ) {
        time += 10;
        if (time > 10000) {
            time = 0;
        }
        window.clear(Color::Black);

        if (points.size() > 2) {
            for (Vector2f v : points) {
                RectangleShape another;
                another.setPosition(v);
                another.setSize(Vector2f(1, 1));
                float xy = v.x + v.y;
                if (color) {
                    another.setFillColor(Color(100 * sin((xy + time) / waveSpeed) + 155,
                                               100 * sin((xy + time - (2 * waveSpeed)) / waveSpeed) + 155,
                                               100 * sin((xy + time + (2 * waveSpeed)) / waveSpeed) + 155));
                }
                window.draw(another);
            }
        } else {
            points = {};
            full = false;
            place = 0;
        }

        for (int i = 0; i < SPAWN_RATE; i++) {
            int newVert = rand() % vertices.size();
            if (vertices.size() > 3)
                while (newVert == randVert) {
                    newVert = rand() % vertices.size();
                }
            randVert = newVert;
            Vector2f yetAgain = (points[place - 1] + vertices[randVert]);
            if (!full) {
                points.push_back(Vector2f(yetAgain.x / 2, yetAgain.y / 2));
                if (points.size() >= POINTS_ON_SCREEN) full = true;
            } else {
                points[(place) % POINTS_ON_SCREEN] = Vector2f(yetAgain.x / 2, yetAgain.y / 2);
                place++;
                place %= POINTS_ON_SCREEN;
            }
        }

        window.display();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keyboard::Escape:
                        window.close();
                        break;
                    case Keyboard::Q:
                        if (vertices.size() > 2) {
                            vector<Vector2f> verticesNew = {};
                            for (int i = 1; i < vertices.size(); i++) {
                                verticesNew.push_back(vertices[i]);
                            }
                            vertices = verticesNew;
                        } else {
                            points = {Vector2f(0, 0)};
                        }
                        break;
                    case Keyboard::C:
                        color = !color;
                        break;
                    case Keyboard::Up:
                    case Keyboard::Add:
                        color = true;
                        waveSpeed--;
                        break;
                    case Keyboard::Down:
                    case Keyboard::Subtract:
                        color = true;
                        waveSpeed++;
                        break;
                }
            } else if (event.type == Event::MouseButtonPressed) {
                Vector2i pos = Mouse::getPosition(window);
                vertices.push_back(Vector2f(pos.x, pos.y));
                }
        }
    }

    return EXIT_SUCCESS;
}