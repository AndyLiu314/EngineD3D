#pragma once

#define EWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define EWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())
#define EWND_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, __FILE__)
