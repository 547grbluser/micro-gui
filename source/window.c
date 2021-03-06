
#include "ugui/window.h"

#include <stdlib.h>

#include "ugui/layer.h"
#include "ugui/types.h"

/***			Public				***/

struct ugui_window_s {
	ugui_layer_t* base_layer;
	ugui_window_handlers_t handlers;
	ugui_window_event_handler_t on_event;
	void* event_ctx;
};

ugui_window_t* ugui_window_create(uint32_t w, uint32_t h)
{
	ugui_window_t* window = malloc(sizeof(struct ugui_window_s));

	window->base_layer = ugui_layer_create((ugui_rect_t) {
		.x = 0, .y = 0, .w = w, .h = h
	});

	window->handlers.load = NULL;
	window->handlers.unload = NULL;
	window->on_event = NULL;
	window->event_ctx = NULL;

	return window;
}

ugui_layer_t* ugui_window_get_base_layer(ugui_window_t* window)
{
	return window->base_layer;
}

void ugui_window_set_window_handlers(ugui_window_t* window,
                                     ugui_window_handlers_t* handlers)
{
	window->handlers.load = handlers->load;
	window->handlers.unload = handlers->unload;
}

void ugui_window_set_event_handler(ugui_window_t *window, ugui_window_event_handler_t on_event, void* event_ctx)
{
	window->on_event = on_event;
	window->event_ctx = event_ctx;
}

void ugui_window_destroy(ugui_window_t *window)
{
	ugui_layer_destroy(window->base_layer);

	free(window);
}

/***			Private				***/

void _ugui_window_load(ugui_window_t *window)
{
	if (window->handlers.load != NULL) window->handlers.load(window);
}

void _ugui_window_unload(ugui_window_t *window)
{
	if (window->handlers.unload != NULL) window->handlers.unload(window);
}

void _ugui_window_put_event(ugui_window_t *window, int event)
{
	if (window->on_event) window->on_event(window, event, window->event_ctx);
}

void _ugui_window_update(ugui_window_t *window, void* graphics_ctx)
{
	//Render base layer
	ugui_layer_t* base_layer = window->base_layer;
	_ugui_layer_update(base_layer, graphics_ctx);
}

