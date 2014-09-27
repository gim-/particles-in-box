/* WorldRenderer.h

The MIT License (MIT)

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

#ifndef __WORLD_RENDERER_H_INCLUDED__
#define __WORLD_RENDERER_H_INCLUDED__

#include "OpenGLRenderer.h"

// Forward declarations
class CWorld;

class CWorldRenderer : public COpenGL_2D
{
// Construction/destruction
public:
	CWorldRenderer();
	virtual ~CWorldRenderer();

// Public overridable methods
public:

// Protected overridable methods
protected:
	virtual void OnRenderScene();
	virtual void OnInitialized();
	virtual LOGICAL IsIsotropic() { return LOGICAL_FALSE; }
	virtual double GetWorldSizeX();
	virtual double GetWorldSizeY();
	virtual double GetDefaultWorldCenterX();
	virtual double GetDefaultWorldCenterY();

// Public methods
public:
	void SetWorld(CWorld *pWorld) { m_pWorld = pWorld; SetDefaultView(); }

// Protected methods
protected:

// Private methods
private:

// Protected members
protected:
	CWorld *m_pWorld;

// Private members
private:
};


#endif //__WORLD_RENDERER_H_INCLUDED__

