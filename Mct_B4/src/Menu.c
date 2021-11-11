#include "Menu.h"
#include "LCD.h"

static void init() {
    FrontIO.init();
    LCD.backlight(0xFF);
}

static bool handle_keypress() {
    // TODO: FrontIO.get_keys();
    return 0;
}

static void draw_menu(const struct MenuState *state) {
	switch (state->screen) {
	default:
	case SCREEN_HOME:
		LCD.write_4lines(
				"Tempeatur:   22.25.C",
				"Status:      Aus",
				"Events:      0/16",
				""
				);
		break;
	case SCREEN_EVENTS:

		break;
	case SCREEN_EVENT_DETAILS:

		break;
	}
}

static void loop_once(struct MenuState *state) {
        draw_menu(state);

}

const struct menu Menu = {
        .init = init,
        .loop_once = loop_once,
};
