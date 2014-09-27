/* WorldRenderer.cpp

Copyright (c) 2007 Evgeny A. Kozhevnikov <evgeny@kozhevnikov.org>

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

#include "WorldRenderer.h"
#include "World.h"
#include <math.h>

///////////////////////////////////////////////////////////////////////////
// CWorldRenderer

CWorldRenderer::CWorldRenderer()
{
}

CWorldRenderer::~CWorldRenderer()
{
}

//virtual
double CWorldRenderer::GetWorldSizeX()
{
	return (m_pWorld->GetGeometry()->Xright - m_pWorld->GetGeometry()->Xleft);
}

//virtual
double CWorldRenderer::GetWorldSizeY()
{
	return (m_pWorld->GetGeometry()->Ymax - m_pWorld->GetGeometry()->Ymin);
}

//virtual
double CWorldRenderer::GetDefaultWorldCenterX()
{
	return (m_pWorld->GetGeometry()->Xright + m_pWorld->GetGeometry()->Xleft)/2;
}

//virtual
double CWorldRenderer::GetDefaultWorldCenterY()
{
	return (m_pWorld->GetGeometry()->Ymax + m_pWorld->GetGeometry()->Ymin)/2;
}

//virtual
void CWorldRenderer::OnRenderScene()
{
	const double sin45 = 0.70710678118654752440084436210485;
	const double cos45 = sin45;
	SGeometry *pGeometry = m_pWorld->GetGeometry();
	double R = pGeometry->Rparticle;
	double Rsin45 = R * sin45;
	double Rcos45 = R * cos45;
	
	for (int i = 0; i < m_pWorld->GetParticleCount(); i++)
	{
		SParticle *pParticle = m_pWorld->GetParticle(i);
		if (pParticle->color)
			glColor3ubv(m_pWorld->getRightColor());
		else
			glColor3ubv(m_pWorld->getLeftColor());

		
		// TODO: use display  lists here for speed
		glBegin(GL_TRIANGLE_FAN);
			glVertex2d(pParticle->x+0, pParticle->y+0);
			glVertex2d(pParticle->x+R, pParticle->y+0);
			glVertex2d(pParticle->x+Rcos45, pParticle->y+Rsin45);
			glVertex2d(pParticle->x+0, pParticle->y+R);
			glVertex2d(pParticle->x-Rcos45, pParticle->y+Rsin45);
			glVertex2d(pParticle->x-R, pParticle->y+0);
			glVertex2d(pParticle->x-Rcos45, pParticle->y-Rsin45);
			glVertex2d(pParticle->x+0, pParticle->y-R);
			glVertex2d(pParticle->x+Rcos45, pParticle->y-Rsin45);
			glVertex2d(pParticle->x+R, pParticle->y+0);
		glEnd();
	}

	glColor3f(1,1,1);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
		glVertex2d(pGeometry->WLeft, pGeometry->Ymax);
		glVertex2d(pGeometry->WLeft, pGeometry->YGapTop);
		glVertex2d(pGeometry->WRight, pGeometry->YGapTop);
		glVertex2d(pGeometry->WRight, pGeometry->Ymax);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex2d(pGeometry->WLeft, pGeometry->Ymin);
		glVertex2d(pGeometry->WLeft, pGeometry->YGapBottom);
		glVertex2d(pGeometry->WRight, pGeometry->YGapBottom);
		glVertex2d(pGeometry->WRight, pGeometry->Ymin);
	glEnd();

//	COpenGL_2D::OnRenderScene();
}

//virtual
void CWorldRenderer::OnInitialized()
{
}
