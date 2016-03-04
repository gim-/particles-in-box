#include "world.h"
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <cstdio>

using namespace std;


const double PI = M_PI;//3.1415926;

World::World(int nLeftParticles, int nRightParticles, double rParticle, double vInit, double loss, double width,
                     double height, double barrierX, double barrierWidth, double holeY,
                     double holeHeight, double deltaVTop, double deltaVBottom, double deltaVSide, double g,
                     int minToSimulate, double frames, string fileName) {
    this->g = g;
    this->loss = loss;
    geometry.rParticle = rParticle;
    geometry.xLeft = 0;
    geometry.particleXLeft = geometry.xLeft + geometry.rParticle;
    geometry.xRight = geometry.xLeft + width;
    geometry.particleXRight = geometry.xRight - geometry.rParticle;
    geometry.yMin = 0;
    geometry.yMax = geometry.yMin + height;
    geometry.particleYMax = geometry.yMax - geometry.rParticle;
    geometry.particleYMin = geometry.yMin + geometry.rParticle;
    geometry.xCenter = barrierX;
    geometry.wThickness= barrierWidth;
    geometry.wLeft = geometry.xCenter - geometry.wThickness/2;
    geometry.wRight = geometry.xCenter + geometry.wThickness/2;
    geometry.particleWLeft= geometry.wLeft - geometry.rParticle;
    geometry.particleWRight = geometry.wRight + geometry.rParticle;
    geometry.holeSize = holeHeight;
    geometry.holePosition = holeY;
    geometry.yGapTop = geometry.holePosition + geometry.holeSize/2;
    geometry.yGapBottom = geometry.holePosition - geometry.holeSize/2;
    geometry.particleHoleTop = geometry.yGapTop-geometry.rParticle;
    geometry.particleHoleBottom = geometry.yGapBottom+geometry.rParticle;
    this->particle = new SParticle[nLeftParticles + nRightParticles];
    this->nLeftParticles = nLeftParticles;
    this->nRightParticles = nRightParticles;
    this->vInit = vInit;
    this->deltaVBottom = deltaVBottom;
    this->deltaVTop = deltaVTop;
    this->deltaVSide = deltaVSide;
    this->fileName = fileName;
    this->minToSimulate = minToSimulate;
    this->frames = frames;

}

World::~World() {
    delete particle;
}

/*
 * Создание копии массива с частицами
*/
SParticle* World::getCopyParticles() {
    int numParticles = nLeftParticles + nRightParticles;
    SParticle *copy = new SParticle[numParticles];
    for (int i = 0; i < numParticles; i++) {
        copy[i] = particle[i];
    }
    return copy;
}

double getVelocity(const SParticle &p) {
    return sqrt(p.vX*p.vX + p.vY*p.vY);
}

double World::random(double from, double to) {
    return (to-from)*rand()/RAND_MAX + from;
}

/*
 * Вычисляет квадрат расстояния между двумя частицами (между центрами)
*/
double World::distance2(SParticle &p1, SParticle &p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return (dx*dx+dy*dy);
}


/**
 * Определяет, сближаются ли частицы
 */
bool World::approaching(SParticle &p1, SParticle &p2) {
    double dVx, dVy;	// Скорости сближения по осям
    if (p1.x < p2.x)
        dVx = p1.vX - p2.vX;
    else
        dVx = p2.vX - p1.vX;

    if (p1.y < p2.y)
        dVy = p1.vY - p2.vY;
    else
        dVy = p2.vY - p1.vY;

    if (dVx > 0 && dVy > 0)
        return true;	// Сближаются по обеим осям
    if (dVx < 0 && dVy < 0)
        return false;	// Удаляются по обеим осям
    if (dVx > 0 && dVy < 0)
        return (dVx > dVy);
    return (dVy > dVx);
}


/**
 * Рассчитывает столкновение частиц
 */
void World::collision(SParticle &p1, SParticle &p2) {
    double factor = sqrt(1.-loss);
    double dx = p1.x-p2.x;
    double dx2 = dx*dx;
    double dy = p1.y-p2.y;
    double dy2 = dy*dy;
    double distance2 = dx2+dy2;
    double sin2 = dy2/distance2;
    double cos2 = dx2/distance2;
    double sincos = dx*dy/distance2;
    double dVx = p1.vX - p2.vX;
    double dVy = p1.vY - p2.vY;
    double v1X = dVx*sin2 - dVy*sincos + p2.vX;
    double v1Y = dVy*cos2 - dVx*sincos + p2.vY;
    double v2X = dVx*cos2 + dVy*sincos + p2.vX;
    double v2Y = dVy*sin2 + dVx*sincos + p2.vY;
    p1.vX = v1X*factor;
    p1.vY = v1Y*factor;
    p2.vX = v2X*factor;
    p2.vY = v2Y*factor;
    if (distance2 < 4*geometry.rParticle*geometry.rParticle)
    { // overlapped particles
        double distance = sqrt(distance2);
        double needToMove = 2*geometry.rParticle - distance;
        double cosine = dx/distance;
        double sine = dy/distance;
        if (dy > 0)
        {
            p1.x += needToMove*cosine;
            p1.y += needToMove*sine;
        }
        else
        {
            p2.x -= needToMove*cosine;
            p2.y -= needToMove*sine;
        }
    }
}


/**
 * Задает случайный вектор скорости (модуль постоянный и равный vInit)
 */
void World::randomVelocity(SParticle &p) {
    double angle = random(0, PI*2);			// направление движения частицы - случайный угол 0...2PI
    p.vX = vInit * cos(angle);
    p.vY = vInit * sin(angle);
}


/**
  * Вычисляет временнОй шаг исходя из скоростей частиц с таким расчетом, чтобы даже самая быстрая частица
  * за этот шаг не проходила расстояние, большее половины своего радиуса (это нужно чтобы избежать
  * пролета частиц сквозь границу ящика за один шаг, а также чтобы столкновения частиц между собой не были пропущены).
 */
double World::calcTimeStep() {
    double v, vMax = 0;
    for (int i = 0; i < getParticleCount(); i++) {
        v = getVelocity(particle[i]);
        if (v > vMax)
            vMax = v;
    }
    if (vMax == 0)
        vMax = vInit;
    if (vMax == 0)
        vMax = 0.1;
    deltaTime =  (geometry.rParticle/8)/vMax;
    return deltaTime;
}


bool World::initialDistribution() {
    int i, j;
    bool bTouching;
    // сначала в левой половине
    for (i = 0; i < getLeftParticleCount(); i++) {
        do {
            bTouching = false;
            particle[i].x = random(geometry.xLeft+geometry.rParticle, geometry.particleWLeft);
            particle[i].y = random(geometry.yMin+geometry.rParticle, geometry.yMax-geometry.rParticle);
            for (j = 0; j<i; j++) {
                if (inContact(particle[i], particle[j])) {
                    bTouching = true;
                    break;
                }
            }
        } while (bTouching);
        // координаты определили, теперь скорость
        randomVelocity(particle[i]);
        particle[i].color = 0; // red
    }

    // теперь в правой половине
    for (i = getLeftParticleCount(); i < getParticleCount(); i++) {
        do {
            bTouching = false;
            particle[i].x = random(geometry.particleWRight, geometry.xRight-geometry.rParticle);
            particle[i].y = random(geometry.yMin+geometry.rParticle, geometry.yMax-geometry.rParticle);
            for (j = nLeftParticles; j<i; j++) {
                if (inContact(particle[i], particle[j])) {
                    bTouching = true;
                    break;
                }
            }
        } while (bTouching);

        // координаты определили, теперь скорость
        randomVelocity(particle[i]);
        particle[i].color = 1; // blue
    }

    calcTimeStep();
    return true;
}

void World::writeParameters() {
    ofstream out(fileName, ios::binary | ios::out);
    if (out.is_open()) {
        //box
        out.write((char*)&geometry.xRight, sizeof(geometry.xRight));
        out.write((char*)&geometry.yMax, sizeof(geometry.yMax));

        //Collisions
        out.write((char*)&deltaVTop, sizeof(deltaVTop));
        out.write((char*)&deltaVBottom, sizeof(deltaVBottom));
        out.write((char*)&deltaVSide, sizeof(deltaVSide));

        //membrana
        out.write((char*)&geometry.xCenter, sizeof(geometry.xCenter)); //x середины перегородки
        out.write((char*)&geometry.wThickness, sizeof(geometry.wThickness)); //ширина
        double YCenter = (geometry.yGapTop+geometry.yGapBottom) / 2; //y середины дырки
        out.write((char*)&YCenter, sizeof(YCenter));
        out.write((char*)&geometry.holeSize, sizeof(geometry.holeSize)); //высота дырки

        //collision2
        out.write((char*)&loss, sizeof(loss));

        //particles
        out.write((char*)&geometry.rParticle, sizeof(geometry.rParticle)); //radius

        //other
        out.write((char*)&g, sizeof(g));
        int partCount = getParticleCount();
        out.write((char*)&partCount, sizeof(partCount));
        out.close();
    }
    this->headPointerParticles = 12 * sizeof(double) + sizeof(int); // в файл записывается 12 параметров типа double и 1 параметр типа int
}

void World::readParticlesState(int stateNum) {
    SParticle currParticle;
    uint16_t id;
    ifstream in(fileName, ios::binary | ios::in | ios::app);
    size_t particleSize = sizeof(currParticle.x) + sizeof(currParticle.y) +
                          sizeof(currParticle.vX) + sizeof(currParticle.vY) + sizeof(id);
    int pointerPosition = headPointerParticles + stateNum * getParticleCount() * particleSize;
    in.seekg(pointerPosition);
    for (int i = 0; i < getParticleCount(); i++) {
        in >> time;
        in >> currParticle.x >> currParticle.y >> currParticle.vX >> currParticle.vY;
        in >> id;
        if (id & 1) {
            currParticle.color = 1;
        } else {
            currParticle.color = 0;
        }
        particle[i] = currParticle;
    }
}

/**
 * Записывает накопленную статистику в файл
 */
void World::writeStat() {
    ofstream out(fileName, ios::binary | ios::out | ios::app);
    if (out.is_open()) {
        out.write((char *) &time, sizeof(time));
        for (int i = 0; i < getParticleCount(); i++) {
            out.write((char *) &particle[i].x, sizeof(particle[i].x));
            out.write((char *) &particle[i].y, sizeof(particle[i].y));
            out.write((char *) &particle[i].vX, sizeof(particle[i].vX));
            out.write((char *) &particle[i].vY, sizeof(particle[i].vY));
            uint16_t id;
            if (!particle[i].color)
                id = 2 * i;
            else
                id = 2 * i + 1;
            out.write((char *) &id, sizeof(id));
        }
        out.close();
    }
}

/**
 * Метод сортировки частиц по Х
 */
static int compareParticleProc(const void *elem1, const void *elem2) {
    SParticle *p1 = (SParticle *)elem1;
    SParticle *p2 = (SParticle *)elem2;
    if (p1->y < p2->y)
        return -1;
    else
        return 1;
}

void World::moveParticle(SParticle &p, double dt) {
    bool IsLeftBefore = p.x < geometry.xCenter;
    // По оси Х
    p.x = p.x + p.vX*dt;
    bool IsLeftAfter = p.x < geometry.xCenter;
    if (IsLeftAfter != IsLeftBefore) {
        if (IsLeftBefore)
            ++nFluxFromLeft;
        else
            ++nFluxFromRight;
    }
    // По оси Y
    p.y = p.y + p.vY*dt - g*dt*dt/2;
    p.vY = p.vY - g*dt;
}

/**
 * Выполняет моделирование одного временнОго шага
 * Возвращает true если в результате этого шага случилось хотя бы одно столкновение частицы
 * с другой частицей или со стенками (в результате чего изменилась скорость хоть одной частицы)
 * или хоть одна частица перелетела из одной половины в другую
 */
bool World::oneTimeStep(double DeltaTime) {
    int i, j;

    for (i = 0; i < getParticleCount(); i++) {
        moveParticle(particle[i], DeltaTime);
    }

    // Проверяем столкновение частиц между собой
    ::qsort(particle, getParticleCount(), sizeof(SParticle), compareParticleProc);
    for (i = 0; i < getParticleCount(); i++) {
        for (j = i+1; j < getParticleCount(); j++) {
            if (particle[j].y - particle[i].y > 2*geometry.rParticle)
                break;
            if (inContact(particle[i], particle[j]) && approaching(particle[i], particle[j])) {
                collision(particle[i], particle[j]);
            }
        }
    }

    // Теперь проверяем столкновение частиц со стенками
    for (i = 0; i < getParticleCount(); i++) {
        correctParticleByGeometry(particle[i]);
    }

    time = time+DeltaTime;
    return true;
}

/**
 * Корректирует движение частицы при столкновении с перегородкой и стенками
 * Возвращает true если частица столкнулась с перегородкой/стенками
 */
bool World::correctParticleByGeometry(SParticle &p) {
    bool bCorrected = false;
    double xx = p.x;
    double yy = p.y;
    double vX = p.vX;
    double vY = p.vY;
    if (yy > geometry.particleYMax && vY > 0) { // Верхняя стенка
        p.y = geometry.particleYMax;
        bounceY(p);
        p.vY -= deltaVTop;
        bCorrected = true;
    }
    else if (yy < geometry.particleYMin && vY < 0) { // Нижняя стенка
        p.y = geometry.particleYMin;
        bounceY(p);
        p.vY += deltaVBottom;
        bCorrected = true;
    }
    if (xx < geometry.particleXLeft && vX < 0) { // левая стенка
        p.x = geometry.particleXLeft;
        bounceX(p);
        p.vX += deltaVSide;
        bCorrected = true;
    }
    else if (xx > geometry.particleXRight && vX > 0) { // правая стенка
        p.x = geometry.particleXRight;
        bounceX(p);
        p.vX -= deltaVSide;
        bCorrected = true;
    }
    else if (xx > geometry.particleWLeft&& xx < geometry.particleWRight) {
        // Частица может находиться внутри перегородки
        if (xx > geometry.wLeft && xx < geometry.wRight) {
            if (yy > geometry.particleHoleTop && vY > 0) { // Верхняя граница дырки
                p.y = geometry.particleHoleTop;
                bounceY(p);
                p.vY -= deltaVTop;
                bCorrected = true;
            }
            else if (yy < geometry.particleHoleBottom && vY < 0) { // Нижняя граница дырки
                p.y = geometry.particleHoleBottom;
                bounceY(p);
                p.vY += deltaVBottom;
                bCorrected = true;
            }
        }
        else if (yy > geometry.yGapTop || yy < geometry.yGapBottom) { // Частица не попала в дырку в перегородке
            if (xx < geometry.xCenter && vX > 0) {
                p.x = geometry.particleWLeft;
                bounceX(p);
                // и добавляем импульса влево
                p.vX -= deltaVSide;
                bCorrected = true;
            }
            else if (xx > geometry.xCenter && vX < 0) {
                p.x = geometry.particleWRight;
                bounceX(p);
                // и добавляем импульса вправо
                p.vX += deltaVSide;
                bCorrected = true;
            }
        }
    }
    return bCorrected;
}

void World::simulate() {
    queue<double> snapSeconds;
    unsigned int numSeconds = minToSimulate * 60;
    unsigned int currTime = 0;
    initialDistribution();
    writeParameters();
    writeStat();
    for (int i = 1; i < int(frames * numSeconds) + 2; i++) {
        snapSeconds.push((double)i / frames);
    }

    double currSnap = snapSeconds.front();
    snapSeconds.pop();

    while (!snapSeconds.empty() && currTime < numSeconds) {
        double timeStep = calcTimeStep();
        if (timeStep > (currSnap - currTime) && (currSnap - currTime) != 0) {
            timeStep = (currSnap - currTime);
        }
	    if (currTime <= currSnap && currSnap <= currTime + timeStep) {
            writeStat();
            emit updateCurrTime(currTime);
            currSnap = snapSeconds.front();
            snapSeconds.pop();
        }
        currTime += timeStep;
        oneTimeStep(timeStep);
    }
}
