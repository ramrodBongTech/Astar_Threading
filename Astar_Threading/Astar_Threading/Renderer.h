#pragma once
#define SDL_MAIN_HANDLED
#ifdef __APPLE__
#include "SDL2/SDL.h"
#elif defined(_WIN64) || defined(_WIN32)
#include "SDL.h"
#endif

#include "BasicTypes.h"

// Responsible for all drawing operations
// Abstracts away SDL specific drawing functions
class Renderer
{
public:
	Renderer();															// Standard Constructor
	bool			init(const Size2D& winSize, const char* title);		// Standard Initialise
	void			drawRect(const Rect& r, const Colour& c);			// Draw a rectangle in pixel coordinates
	void			drawWorldRect(const Rect& r, const Colour& c);		// Draw a rectangle in world coordinates
	void			drawEmptyRect(const Rect& r, const Colour& c);		// Draw a rectangle with hollow centre in pixel coordinates
	void			drawEmptyWorldRect(const Rect& r, const Colour& c);	// Draw a rectangle with hollow centre in world coordinates
	void			present();											// Swaps buffers
	void			clear(const Colour&c);								// Clears the window
	Point2D			worldToScreen(const Point2D&p);						// Converts a point from world coordinates to screen coordinates
	Rect			worldToScreen(const Rect&r);						// Converts a rectangle from world coordinates to screen coordinates

	void			setViewPort(const Rect&r);							// Sets the viewport

	void			destroy();											// Destroys render objects
	~Renderer();														// Standard Destructor

	SDL_Renderer*	getRenderer();										// Returns the renderer

private:
	Size2D			windowSize;											// Size of window in pixels

	Point2D			viewportBottomLeft;									// Position of window in world coordinates
	Size2D			viewportSize;										// Size of window in world coordinates

	int				w;													// The width of the window
	int				h;													// The height of the window

	SDL_Window*		window;												// The window object
	SDL_Renderer*	sdl_renderer;										// The renderer object
};

