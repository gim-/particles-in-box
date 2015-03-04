/* World.h

The MIT License (MIT)

Copyright (c) 2007 Evgeny A. Kozhevnikov <evgeny@kozhevnikov.org>
    Gennady Gaidukov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


Revision history:
	02 Mar 2007 - initial creation

*/



#ifndef __WORLD_H_INCLUDED__
#define __WORLD_H_INCLUDED__

#include <QObject>
#include <QString>
#include <math.h>
#include "calcthread.h"
#include <QVector>

// Forward declarations

// TODO: change nMaxParticles
// Was reduced to 12000 because of 'Stack Overflow' exception
const int nMaxParticles = 12000; //65536;		// Максимальное число частиц

struct SParticle
{
    double x,y;		// position
    double vx, vy;	// velocity
    int color;		// 0 частица родилась слева, 1 - справа
};

struct SGeometry
{
    double Xleft, ParticleXleft;			// Левая граница (в метрах)
    double Xcenter, ParticleWLeft, ParticleWRight;			// Перегородка (в метрах)
    double WThickness;		// Wall thickness
    double WLeft, WRight;
    double YGapTop;			// Верхний край перегородки
    double YGapBottom;		// Нижний край перегородки
    double ParticleHoleTop, ParticleHoleBottom;
    double HoleSize, HolePosition;
    double Xright, ParticleXright;			// Правая граница (в метрах)
    double Ymax, ParticleYmax;			// Верхняя граница (в метрах)
    double Ymin, ParticleYmin;			// Нижняя граница (в метрах)
    double Rparticle;		// Радиус одной частицы  (в метрах)
};

Q_DECLARE_METATYPE(SGeometry)

class WorldSettings;


class CWorld : public QObject
{
    Q_OBJECT

    friend class GLWidget;
// Construction/destruction
public:
    CWorld();
	virtual ~CWorld();

// Public overridable methods
public:

// Protected overridable methods
protected:

// Public methods
public:
    bool InitialDistribution(WorldSettings*);
    double CalcTimeStep();
    void OnIdle();
	int GetLeftParticleCount() { return nLeftParticles; }
	int GetRightParticleCount() { return nRightParticles; }
	int GetParticleCount() { return (nLeftParticles+nRightParticles); }
	SParticle *GetParticle(int iParticle) { return particle + iParticle; }
	SGeometry *GetGeometry() { return &Geometry; }
	unsigned char *getLeftColor() { return m_LeftColor.c; }
    unsigned char *getRightColor() { return m_RightColor.c; }
    void setHeightDistActive(bool isActive) {heightDistActive = isActive;}
    void setMaxwellDistActive(bool isActive) {maxwellDistActive = isActive;}


// Public members
public:
	double			Time;				// Текущее время (в секундах)
	double			DeltaTime;			// Шаг по времени (в секундах)
	unsigned long	nRight;				// Количество частиц в правой части
	unsigned long	nLeft;				// Количество частиц в левой части
	double			VaverageR;			// Средняя скорость частиц в правой части
	double			VaverageL;			// Средняя скорость частиц в левой части
	int nLeftParticles, nRightParticles;// Реальное число частиц


// Protected methods
protected:
	int LoadInt(char *pszValueName, int nDefaultValue);
	void SaveInt(char *pszValueName, int nValue);
	double LoadDouble(char *pszValueName, double fDefaultValue);
	void SaveDouble(char *pszValueName, double fValue);
	double random(double from, double to);
	double Distance2(SParticle &p1, SParticle &p2);
	double Distance(SParticle &p1, SParticle &p2) { return sqrt(Distance2(p1, p2)); }
    bool InContact(SParticle &p1, SParticle &p2) { return (Distance(p1, p2) < 2*Geometry.Rparticle); }
    bool Approaching(SParticle &p1, SParticle &p2);
	void Collision(SParticle &p1, SParticle &p2);
    void RandomVelocity(SParticle &p);
	void RecalcStat();
	void MoveParticle(SParticle &p, double dt);
	void BounceX(SParticle &p) { p.vx = -p.vx; }
	void BounceY(SParticle &p) { p.vy = -p.vy; }
    void WriteStat();
    bool CorrectParticleByGeometry(SParticle &p);
    bool OneTimeStep();

    const QVector<double> *CalculateHeightDistribution();
    const QVector<double> *CalculateMaxwellDistDistribution();
    bool HeightDistIsActive() {return heightDistActive;}
    bool MaxwellDistIsActive() {return maxwellDistActive;}

// Private methods
private:

// Protected members
protected:
    CalcThread *Thread;                     // Отедльный поток
	SParticle particle[nMaxParticles];		// Сами частицы
	SGeometry Geometry;
	double Vinit;			// Начальная скорость всех частиц (в метрах/сек)
	// Collisions
	double DeltaVtop;		// Добавок скорости, который получает частица, ударяясь о верхнюю стенку
	double DeltaVbottom;	// Добавок скорости, который получает частица, ударяясь о нижнюю стенку
	double DeltaVside;		// Добавок скорости, который получает частица, ударяясь о боковую стенку
	double loss;			// Коэффициент диссипации кинетической энергии

	double g;				// Ускорение свободного падения (в м/с**2)

    int Heights;            // Количество разбиений по Oy

    QString FileName;		// Имя файла с записанной статистикой
    QVector<double> heightDistrArr;
    QVector<double> maxwellDistrArr;

	// Вычисляемые величины (переменные)
	unsigned long	nTimeSteps;			// Количество пройденных шагов по времени

	double			LastTimeWritten;	// Время, которое было записано в файл последний раз
	int				nFluxFromLeft;
	int				nFluxFromRight;
	union
	{
		unsigned char c[4];
        unsigned int rgb;
    } m_LeftColor, m_RightColor;

    bool heightDistActive = false;
    bool maxwellDistActive = false;

// Private members
private:

// Slots & Signals

public slots:
    void onWorldInitializationRequested(); // Called by dialog
    void onApplicationTerminate();

signals:
    void onWorldInitialized();
    void RedrawWorld(SGeometry);
    void onParticleSCountChange();
    void RedrawHeightGraph(const QVector<double>*);
    void RedrawMaxwellDistGraph(const QVector<double>*);
};


#endif //__WORLD_H_INCLUDED__

