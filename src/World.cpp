/* World.cpp

The MIT License (MIT)

Copyright (c) 2007 Evgeny A. Kozhevnikov <evgeny@kozhevnikov.org>,
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

#include "World.h"
#include <stdlib.h>
#include <time.h>
#include "worldsettings.h"
#include "mainwindow.h"
#include "calcthread.h"


#include <QFile>



const char *pszUnknownFileName = "<unknown>";
const char *pszSectionName = "Values";

const double PI = 3.1415926;

#define RGB(r,g,b) ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff)

///////////////////////////////////////////////////////////////////////////
// CWorld

CWorld::CWorld()
{
	nFluxFromLeft = nFluxFromRight = 0;
    // Создаём новый поток:
    this->Thread = new CalcThread(this);
    connect(this->Thread, &CalcThread::finished, Thread, &QObject::deleteLater); // На случай если поток завершился до завершения программы
}


CWorld::~CWorld()
{
}


/**
 * генерирует псевдослучайное число в диапазоне from...to
 */
double CWorld::random(double from, double to)
{
	return (to-from)*rand()/RAND_MAX + from;
}


/**
 * Вычисляет квадрат расстояния между двумя частицами (между центрами)
 */
double CWorld::Distance2(SParticle &p1, SParticle &p2)
{
	double dx = p1.x - p2.x;
	double dy = p1.y - p2.y;
	return (dx*dx+dy*dy);
}


/**
 * Определяет, сближаются ли частицы
 */
bool CWorld::Approaching(SParticle &p1, SParticle &p2)
{
	double dVx, dVy;	// Скорости сближения по осям
	if (p1.x < p2.x)
		dVx = p1.vx - p2.vx;
	else
		dVx = p2.vx - p1.vx;

	if (p1.y < p2.y)
		dVy = p1.vy - p2.vy;
	else
		dVy = p2.vy - p1.vy;

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
void CWorld::Collision(SParticle &p1, SParticle &p2)
{
	double factor = sqrt(1.-loss);
	
	// Old collision
/*	double dummy = p2.vx;
	p2.vx = p1.vx*factor;
	p1.vx = dummy*factor;
	dummy = p2.vy;
	p2.vy = p1.vy*factor;
	p1.vy = dummy*factor;*/

	// New collision
	double dx = p1.x-p2.x;
	double dx2 = dx*dx;
	double dy = p1.y-p2.y;
	double dy2 = dy*dy;
	double distance2 = dx2+dy2;
	double sin2 = dy2/distance2;
	double cos2 = dx2/distance2;
	double sincos = dx*dy/distance2;
	double dVx = p1.vx - p2.vx;
	double dVy = p1.vy - p2.vy;
	double V1x = dVx*sin2 - dVy*sincos + p2.vx;
	double V1y = dVy*cos2 - dVx*sincos + p2.vy;
	double V2x = dVx*cos2 + dVy*sincos + p2.vx;
	double V2y = dVy*sin2 + dVx*sincos + p2.vy;
	p1.vx = V1x*factor;
	p1.vy = V1y*factor;
	p2.vx = V2x*factor;
	p2.vy = V2y*factor;
	if (distance2 < 4*Geometry.Rparticle*Geometry.Rparticle)
	{ // overlapped particles
		double distance = sqrt(distance2);
		double NeedToMove = 2*Geometry.Rparticle - distance;
		double cosine = dx/distance;
		double sine = dy/distance;
		if (dy > 0)
		{
			p1.x += NeedToMove*cosine;
			p1.y += NeedToMove*sine;
		}
		else
		{
			p2.x -= NeedToMove*cosine;
			p2.y -= NeedToMove*sine;
		}
	}

/*	double angle = random(-PI/8, PI/8);
	double sine = sin(angle);
	double cosine = cos(angle);
	double vxnew = p1.vx*cosine + p1.vy*sine;
	double vynew = -p1.vx*sine + p1.vy*cosine;
	p1.vx = vxnew;
	p1.vy = vynew;
	vxnew = p2.vx*cosine + p2.vy*sine;
	vynew = -p2.vx*sine + p2.vy*cosine;
	p2.vx = vxnew;
	p2.vy = vynew;*/
}


/**
 * Задает случайный вектор скорости (модуль постоянный и равный Vinit)
 */
void CWorld::RandomVelocity(SParticle &p)
{
	double angle = random(0, PI*2);			// направление движения частицы - случайный угол 0...2PI
	p.vx = Vinit * cos(angle);
	p.vy = Vinit * sin(angle);
}


/**
  * Вычисляет временнОй шаг исходя из скоростей частиц с таким расчетом, чтобы даже самая быстрая частица
  * за этот шаг не проходила расстояние, большее половины своего радиуса (это нужно чтобы избежать
  * пролета частиц сквозь границу ящика за один шаг, а также чтобы столкновения частиц между собой не были пропущены).
 */
double CWorld::CalcTimeStep()
{
	double V, Vmax = 0;
	for (int i = 0; i < GetParticleCount(); i++)
	{
		V = GetVelocity(particle[i]);
		if (V > Vmax)
			Vmax = V;
	}
	if (Vmax == 0)
		Vmax = Vinit;
	if (Vmax == 0)
		Vmax = 0.1;
	DeltaTime =  (Geometry.Rparticle/8)/Vmax;
    return DeltaTime;
}


/**
 * Рассчитывает статистику
 */
void CWorld::RecalcStat()
{
	int i;
	nRight = nLeft = 0;
	VaverageR = VaverageL = 0;
	// Сначала просто суммируем модули скоростей, попутно считая частицы в каждой половине
	for (i = 0; i < GetParticleCount(); i++)
	{
		if (particle[i].x > Geometry.Xcenter)
		{
			++nRight;
			VaverageR += GetVelocity(particle[i]);
		}
		else
		{
			++nLeft;
			VaverageL += GetVelocity(particle[i]);
		}
	}
	// Теперь делим на число частиц, чтобы найти среднее (не забывая про случай, когда частиц нет)
	if (nRight != 0)
		VaverageR = VaverageR/nRight;
	else
		VaverageR = 0;
	if (nLeft != 0)
		VaverageL = VaverageL/nLeft;
	else
		VaverageL = 0;
}


/**
 *  Первоначальное распределение частиц
 */
bool CWorld::InitialDistribution(WorldSettings* dlg)
{

    bool bTouching;		// соприкасаются ли частицы
	int i,j;			// переменные циклов
	Time = 0;			// Текущее время
	nTimeSteps = 0;		// Количество пройденных шагов по времени
	

    nLeftParticles = dlg->getSetting("m_nLeftParticles").toInt();
    m_LeftColor.rgb = 0xFF0000; // TODO: dlg->getSetting("m_LeftColor").toInt();
    nRightParticles = dlg->getSetting("m_nRightParticles").toInt();
    m_RightColor.rgb = 0x0000FF; // TODO: dlg->getSetting("m_RightColor").toInt();

    g = dlg->getSetting("m_G").toDouble();
    loss = dlg->getSetting("m_LossRatio").toDouble();
    Geometry.Rparticle = dlg->getSetting("m_RParticle").toDouble();
    Geometry.Xleft = dlg->getSetting("m_Xleft").toDouble();
    Geometry.ParticleXleft = Geometry.Xleft + Geometry.Rparticle;
    Geometry.Xright = dlg->getSetting("m_Xright").toDouble();
    Geometry.ParticleXright = Geometry.Xright - Geometry.Rparticle;
    Geometry.Ymax = dlg->getSetting("m_Ytop").toDouble();
    Geometry.ParticleYmax = Geometry.Ymax - Geometry.Rparticle;
    Geometry.Ymin = dlg->getSetting("m_Ybottom").toDouble();
    Geometry.ParticleYmin = Geometry.Ymin + Geometry.Rparticle;
    Geometry.Xcenter = dlg->getSetting("m_WallXPosition").toDouble();
    Geometry.WThickness = dlg->getSetting("m_WallThickness").toDouble();
    Geometry.WLeft = Geometry.Xcenter - Geometry.WThickness/2;
    Geometry.WRight = Geometry.Xcenter + Geometry.WThickness/2;
    Geometry.ParticleWLeft = Geometry.WLeft - Geometry.Rparticle;
    Geometry.ParticleWRight = Geometry.WRight + Geometry.Rparticle;
    Geometry.HoleSize = dlg->getSetting("m_HoleSize").toDouble();
    Geometry.HolePosition = dlg->getSetting("m_HolePosition").toDouble();
    Geometry.YGapTop = Geometry.HolePosition + Geometry.HoleSize/2;
    Geometry.YGapBottom = Geometry.HolePosition - Geometry.HoleSize/2;
    Geometry.ParticleHoleTop = Geometry.YGapTop-Geometry.Rparticle;
    Geometry.ParticleHoleBottom = Geometry.YGapBottom+Geometry.Rparticle;
    Vinit = dlg->getSetting("m_Vinit").toDouble();
    DeltaVbottom = dlg->getSetting("m_BounceBottom").toDouble();
    DeltaVtop = dlg->getSetting("m_BounceTop").toDouble();
    DeltaVside = dlg->getSetting("m_BounceSide").toDouble();
    FileName = dlg->getSetting("m_FileName").toString();

	srand(time(NULL));	// запуск генератора случайных чисел
	// сначала в левой половине
	for (i = 0; i < GetLeftParticleCount(); i++)
	{
		do
		{
            bTouching = false;
			particle[i].x = random(Geometry.Xleft+Geometry.Rparticle, Geometry.ParticleWLeft);
			particle[i].y = random(Geometry.Ymin+Geometry.Rparticle, Geometry.Ymax-Geometry.Rparticle);
			for (j = 0; j<i; j++)
			{
				if (InContact(particle[i], particle[j]))
				{
                    bTouching = true;
					break;
				}
			}
		} while (bTouching);
		// координаты определили, теперь скорость
		RandomVelocity(particle[i]);
		particle[i].color = 0; // red
	}

	// теперь в правой половине
	for (i = GetLeftParticleCount(); i < GetParticleCount(); i++)
	{
		do
		{
            bTouching = false;
			particle[i].x = random(Geometry.ParticleWRight, Geometry.Xright-Geometry.Rparticle);
			particle[i].y = random(Geometry.Ymin+Geometry.Rparticle, Geometry.Ymax-Geometry.Rparticle);
			for (j = nLeftParticles; j<i; j++)
			{
				if (InContact(particle[i], particle[j]))
				{
                    bTouching = true;
					break;
				}
			}
		} while (bTouching);

		// координаты определили, теперь скорость
		RandomVelocity(particle[i]);
		particle[i].color = 1; // blue
	}

	CalcTimeStep();
    RecalcStat();
    FILE *fd = fopen(FileName.toStdString().c_str(), "w");
	if (fd)
    {
		fprintf(fd, "Particles:\n");
		fprintf(fd, "nLeftParticles = %d\n", nLeftParticles);
		fprintf(fd, "nRightParticles = %d\n", nRightParticles);
		fprintf(fd, "RParticle = %g m\n", Geometry.Rparticle);

		fprintf(fd, "\nCollisions:\n");
		fprintf(fd, "CollisionLossRatio = %g\n", loss);
		fprintf(fd, "DeltaVbottom = %g m/sec\n", DeltaVbottom);
		fprintf(fd, "DeltaVtop = %g m/sec\n", DeltaVtop);
		fprintf(fd, "DeltaVside = %g m/sec\n", DeltaVside);

		fprintf(fd, "\nGeometry:\n");
		fprintf(fd, "Xleft = %g m\n", Geometry.Xleft);
		fprintf(fd, "Xright = %g m\n", Geometry.Xright);
		fprintf(fd, "Ytop = %g m\n", Geometry.Ymax);
		fprintf(fd, "Ybottom = %g m\n", Geometry.Ymin);
		fprintf(fd, "WallXPosition = %g m\n", Geometry.Xcenter);
		fprintf(fd, "WallThickness = %g m\n", Geometry.WThickness);
		fprintf(fd, "HolePosition = %g m\n", Geometry.HolePosition);
		fprintf(fd, "HoleSize = %g m\n", Geometry.HoleSize);

		fprintf(fd, "\nInitial distribution:\n");
		fprintf(fd, "Vinit = %g m/sec\n", Vinit);

		fprintf(fd, "\nOther:\n");
		fprintf(fd, "g = %g m/sec**2\n", g);

		fprintf(fd, "\nTime\tnLeft\t<V>left\t<flux>left\t\tnRight\t<V>right\t<flux>right\n");
		fclose(fd);
    }
    emit onWorldInitialized();
    return true;
}


/**
 * Вызывается после каждого временного интервала
 */
void CWorld::OnIdle()
{
	// Записываем всю статистику в файл
    if ((nTimeSteps % 1000) == 0) {
        WriteStat();
        emit onParticleSCountChange();
    }

    bool bChanged = OneTimeStep();
	if (bChanged)
	{
		// Пересчитать статистику
        RecalcStat();
	}
    if ((nTimeSteps % 30) == 0)
	{
        emit RedrawWorld(this->Geometry);

	}
}

void CWorld::MoveParticle(SParticle &p, double dt)
{
    bool IsLeftBefore = p.x < Geometry.Xcenter;
	// По оси Х
	p.x = p.x + p.vx*dt;
    bool IsLeftAfter = p.x < Geometry.Xcenter;
	if (IsLeftAfter != IsLeftBefore)
	{
		if (IsLeftBefore)
			++nFluxFromLeft;
		else
			++nFluxFromRight;
	}
	// По оси Y
	p.y = p.y + p.vy*dt - g*dt*dt/2;
	p.vy = p.vy - g*dt;
}

/**
 * Записывает накопленную статистику в файл
 */
void CWorld::WriteStat()
{
    FILE *fd = fopen(FileName.toStdString().c_str(), "a");
	if (fd)
	{
		double FluxFromLeft, FluxFromRight;
		if (LastTimeWritten == Time)
		{
			FluxFromLeft = FluxFromRight = 0;
		}
		else
		{
			FluxFromLeft = nFluxFromLeft/(Time-LastTimeWritten);
			FluxFromRight = nFluxFromRight/(Time-LastTimeWritten);
		}

        fprintf(fd, "%7.2f\t%ld\t%7.2f\t%7.2f\t\t%ld\t%7.2f\t%7.2f\n", Time, nLeft, VaverageL, FluxFromLeft, nRight, VaverageR, FluxFromRight);
		fclose(fd);
		LastTimeWritten = Time;
		nFluxFromLeft = nFluxFromRight = 0;
	}
}

/**
 * Метод сортировки частиц по Х
 */
static int CompareParticleProc(const void *elem1, const void *elem2)
{
	SParticle *p1 = (SParticle *)elem1;
	SParticle *p2 = (SParticle *)elem2;
    if (p1->y < p2->y)
		return -1;
	else
		return 1;
}


/**
 * Выполняет моделирование одного временнОго шага
 * Возвращает true если в результате этого шага случилось хотя бы одно столкновение частицы
 * с другой частицей или со стенками (в результате чего изменилась скорость хоть одной частицы)
 * или хоть одна частица перелетела из одной половины в другую
 */
bool CWorld::OneTimeStep()
{
    int i, j;
	
	for (i = 0; i < GetParticleCount(); i++)
	{
		MoveParticle(particle[i], DeltaTime);
	}
	
	// Проверяем столкновение частиц между собой
	::qsort(particle, GetParticleCount(), sizeof(SParticle), CompareParticleProc);
	for (i = 0; i < GetParticleCount(); i++)
	{
		for (j = i+1; j < GetParticleCount(); j++)
		{
			if (particle[j].y - particle[i].y > 2*Geometry.Rparticle)
				break;
			if (InContact(particle[i], particle[j]) && Approaching(particle[i], particle[j]))
			{
				Collision(particle[i], particle[j]);
			}
		}
	}

	// Теперь проверяем столкновение частиц со стенками
	for (i = 0; i < GetParticleCount(); i++)
	{
		CorrectParticleByGeometry(particle[i]);
	}

	Time = Time+DeltaTime;
	nTimeSteps = nTimeSteps+1;
    return true;
}

/**
 * Корректирует движение частицы при столкновении с перегородкой и стенками
 * Возвращает true если частица столкнулась с перегородкой/стенками
 */
bool CWorld::CorrectParticleByGeometry(SParticle &p)
{
    bool bCorrected = false;
	double xx = p.x;
	double yy = p.y;
	double vx = p.vx;
	double vy = p.vy;
	if (yy > Geometry.ParticleYmax && vy > 0)
	{ // Верхняя стенка
		p.y = Geometry.ParticleYmax;
		BounceY(p);
		p.vy -= DeltaVtop;
        bCorrected = true;
	}
	else if (yy < Geometry.ParticleYmin && vy < 0)
	{ // Нижняя стенка
		p.y = Geometry.ParticleYmin;
		BounceY(p);
		p.vy += DeltaVbottom;
        bCorrected = true;
	}
	if (xx < Geometry.ParticleXleft && vx < 0)
	{ // левая стенка
		p.x = Geometry.ParticleXleft;
		BounceX(p);
		p.vx += DeltaVside;
        bCorrected = true;
	}
	else if (xx > Geometry.ParticleXright && vx > 0)
	{ // правая стенка
		p.x = Geometry.ParticleXright;
		BounceX(p);
		p.vx -= DeltaVside;
        bCorrected = true;
	}
	else if (xx > Geometry.ParticleWLeft && xx < Geometry.ParticleWRight)
	{// Частица может находиться внутри перегородки
		if (xx > Geometry.WLeft && xx < Geometry.WRight)
		{
			if (yy > Geometry.ParticleHoleTop && vy > 0)
			{ // Верхняя граница дырки
				p.y = Geometry.ParticleHoleTop;
				BounceY(p);
				p.vy -= DeltaVtop;
                bCorrected = true;
			}
			else if (yy < Geometry.ParticleHoleBottom && vy < 0)
			{ // Нижняя граница дырки
				p.y = Geometry.ParticleHoleBottom;
				BounceY(p);
				p.vy += DeltaVbottom;
                bCorrected = true;
			}
		}
		else if (yy > Geometry.YGapTop || yy < Geometry.YGapBottom)
		{ // Частица не попала в дырку в перегородке
			if (xx < Geometry.Xcenter && vx > 0)
			{
				p.x = Geometry.ParticleWLeft;
				BounceX(p);
				// и добавляем импульса влево
				p.vx -= DeltaVside;
                bCorrected = true;
			}
			else if (xx > Geometry.Xcenter && vx < 0)
			{ 
				p.x = Geometry.ParticleWRight;
				BounceX(p);
				// и добавляем импульса вправо
				p.vx += DeltaVside;
                bCorrected = true;
			}
		}
	}
	return bCorrected;
}


/**
 * Вызывается перед созданием мира. Отправитель (WorldSettings) передает параметры мира
 */
void CWorld::onWorldInitializationRequested() {
    this->InitialDistribution(qobject_cast<WorldSettings*>(this->sender()));
    this->Thread->start();
    return;
}


/**
 * Выход из потока перед прекращением работы
 */
void CWorld::onApplicationTerminate() {
    // Выходим из потока:
    this->Thread->requestInterruption();
    this->Thread->wait();
}
