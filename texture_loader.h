
#pragma once

#include "glew.h"
#include <string>
#include <wincodec.h>

HRESULT initCOM(void);
void shutdownCOM(void);
GLuint loadTexture(const std::wstring& filename);

