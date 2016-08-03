#ifndef PARTICLE_IN_BOX_WORLD_WORLD_H
#define PARTICLE_IN_BOX_WORLD_WORLD_H

#include <vector>
#include <string>
#include <math.h>
#include <QObject>
#include <QVector>

const int nMaxParticles = 12000; //65536;		// Максимальное число частиц

struct UiParams {
    int nLeftParticles;
    int nRightParticles;
    double rParticle;
    double vInit;
    double vLoss;
    double boxWidth;
    double boxHeight;
    double barrierX;
    double barrierWidth;
    double holeY;
    double holeHeight;
    double deltaVTop;
    double deltaVBottom;
    double deltaVSide;
    double g;
    double minToSimulate;
    double fps;
};

struct SParticle {
    double x,y;		// position
    double vX, vY;	// velocity
    unsigned short int color;		// 0 частица родилась слева, 1 - справа
};

struct SGeometry {
    double xLeft, particleXLeft;			// Левая граница (в метрах)
    double xCenter, particleWLeft, particleWRight;			// Перегородка (в метрах)
    double wThickness;		// Wall thickness
    double wLeft, wRight;
    double yGapTop;			// Верхний край перегородки
    double yGapBottom;		// Нижний край перегородки
    double particleHoleTop, particleHoleBottom;
    double holeSize, holePosition;
    double xRight, particleXRight;			// Правая граница (в метрах)
    double yMax, particleYMax;			// Верхняя граница (в метрах)
    double yMin, particleYMin;			// Нижняя граница (в метрах)
    double rParticle;		// Радиус одной частицы  (в метрах)
};

double particleVelocity(const SParticle& particle);


class World: public QObject {
    Q_OBJECT
public:
    World(int nLeftParticles, int nRightParticles, double rParticle, double vInit, double loss, double width,
              double height, double barrierX, double barrierWidth, double holeY,
              double holeHeight, double deltaVTop, double deltaVBottom, double deltaVSide, double g,
              int minToSimulate, double frames, QString fileName, UiParams params, QObject* parent = 0);
    World(QString fileName, QObject* parent = nullptr);
    ~World();

public:
    bool initialDistribution();
    double calcTimeStep();
    int getLeftParticleCount() const { return nLeftParticles; }
    int getRightParticleCount() const { return nRightParticles; }
    int getParticleCount() const { return (nLeftParticles+nRightParticles); }
    SParticle *getParticle(int iParticle) { return particle + iParticle; }
    SGeometry *getGeometry() { return &geometry; }
    unsigned char *getLeftColor() { return mLeftColor.c; }
    unsigned char *getRightColor() { return mRightColor.c; }
    double getTime() const { return time; }
    SParticle* getCopyParticles();
    void simulate();
    void readParticlesState(int stateNum);
    QVector<SParticle> getParticles() const;
    QString getFileName() { return fileName; }
// Public members
public:
    double			time;				// Текущее время (в секундах)
    double			deltaTime;			// Шаг по времени (в секундах)
    unsigned long	nRight;				// Количество частиц в правой части
    unsigned long	nLeft;				// Количество частиц в левой части
    double			vAverageR;			// Средняя скорость частиц в правой части
    double			vAverageL;			// Средняя скорость частиц в левой части
    int nLeftParticles, nRightParticles;// Реальное число частиц
    int nParticles;

    unsigned short int getStateCount() const;


// Protected methods
protected:
    double random(double from, double to);
    double distance2(SParticle &p1, SParticle &p2);
    double distance(SParticle &p1, SParticle &p2) { return sqrt(distance2(p1, p2)); }
    bool inContact(SParticle &p1, SParticle &p2) { return (distance(p1, p2) < 2*geometry.rParticle); }
    bool approaching(SParticle &p1, SParticle &p2);
    void collision(SParticle &p1, SParticle &p2);
    void randomVelocity(SParticle &p);
    void moveParticle(SParticle &p, double dt);
    void bounceX(SParticle &p) { p.vX = -p.vX; }
    void bounceY(SParticle &p) { p.vY = -p.vY; }
    void writeStat();
    void writeParameters();
    bool correctParticleByGeometry(SParticle &p);
    bool oneTimeStep(double);


protected:
    SParticle *particle;		// Сами частицы
    SGeometry geometry;
    UiParams params;
    double vInit;			// Начальная скорость всех частиц (в метрах/сек)
    // Collisions
    double deltaVTop;		// Добавок скорости, который получает частица, ударяясь о верхнюю стенку
    double deltaVBottom;	// Добавок скорости, который получает частица, ударяясь о нижнюю стенку
    double deltaVSide;		// Добавок скорости, который получает частица, ударяясь о боковую стенку
    double loss;			// Коэффициент диссипации кинетической энергии

    double g;				// Ускорение свободного падения (в м/с**2)

    unsigned short minToSimulate;
    unsigned short frames;

    QString fileName;		// Имя файла с записанной статистикой
    std::vector<double> heightDistrArr;
    std::vector<double> maxwellDistrArr;
    std::vector<double> particleDistrArr;

    // Вычисляемые величины (переменные)
    unsigned long	nTimeSteps;			// Количество пройденных шагов по времени

    double			lastTimeWritten;	// Время, которое было записано в файл последний раз
    int				nFluxFromLeft;
    int				nFluxFromRight;
    union {
        unsigned char c[4];
        unsigned int rgb;
    } mLeftColor, mRightColor;


signals:
    void onSimulationFinished();
    void onSimulationStep();
    void onSimulationProgress(int secondsSimulated);
    void onWorldInitialized(SGeometry geometry, QVector<SParticle> particles);
    void onStateChanged(QVector<SParticle> particles);
public slots:
    void startSimulation();
};
#endif //PARTICLE_IN_BOX_WORLD_WORLD_H
