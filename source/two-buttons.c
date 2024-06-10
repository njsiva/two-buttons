/*
 * Copyright (C) 2024 J Siva
 *
 * This LV2 plugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This LV2 plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this LV2 plugin; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <lv2/core/lv2.h>
#include <stdlib.h>
#include "lv2-hmi.h"
#include "lv2/core/lv2_util.h"

#define PLUGIN_URI "http://example.org/two-buttons"

// Define the ports - the ttl defines two toggle controls, and one CV output
typedef enum {
	TOGGLE1_PORT = 0,
	TOGGLE2_PORT = 1,
	CV_OUTPUT_PORT = 2
} PortIndex;

// The main plugin struct to hold the toggle statuses and other state data
typedef struct {
	// Ports
	const float* toggle1;
	const float* toggle2;
	float* cv_output;

	// Features
	LV2_HMI_WidgetControl* hmi;

	// State variable (is the switch on)
	uint8_t state;

	// HMI Widgets addressing
	LV2_HMI_Addressing toggle_addressing1;
	LV2_HMI_Addressing toggle_addressing2;
} TogglePlugin;

// Instantiate the plugin
static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
							  double rate,
							  const char* bundle_path,
							  const LV2_Feature* const* features) {
	TogglePlugin* plugin = (TogglePlugin*)malloc(sizeof(TogglePlugin));

	// Query host features
	const char* missing = lv2_features_query(
			features,
			LV2_HMI__WidgetControl, &plugin->hmi, false,
			NULL);

	if (missing) {
		free(plugin);
		return NULL;
	}

	// set initial states - buttons turned off, no addressing information
	// available yet
	plugin->state = 0;
	plugin->toggle_addressing1 = NULL;
	plugin->toggle_addressing2 = NULL;

	return (LV2_Handle)plugin;
}

// Connect host's data locations to the plugin's ports
static void connect_port(LV2_Handle instance, uint32_t port, void* data_location) {
	TogglePlugin* plugin = (TogglePlugin*)instance;
	switch ((PortIndex)port) {
		case TOGGLE1_PORT:
			plugin->toggle1 = (const float*)data_location;
			break;
		case TOGGLE2_PORT:
			plugin->toggle2 = (const float*)data_location;
			break;
		case CV_OUTPUT_PORT:
			plugin->cv_output = (float*)data_location;
			break;
	}
}

// Trigger LED change on the HMI based on toggle state
void trigger_led_change(TogglePlugin* plugin, uint8_t port_index, bool onoff)
{
	if (plugin->hmi) {
		// If we are turning on the widget, use the Red colour, if not, turn it off
		LV2_HMI_LED_Colour colour = onoff ? LV2_HMI_LED_Colour_Red : LV2_HMI_LED_Colour_Off;

		switch (port_index) {
			case TOGGLE1_PORT:
				if (plugin->toggle_addressing1) {
					// Send to HMI
					plugin->hmi->set_led_with_brightness(plugin->hmi->handle, plugin->toggle_addressing1, colour, LV2_HMI_LED_Brightness_High);
				}
				break;
			case TOGGLE2_PORT:
				if (plugin->toggle_addressing2) {
					// Send to HMI
					plugin->hmi->set_led_with_brightness(plugin->hmi->handle, plugin->toggle_addressing2, colour, LV2_HMI_LED_Brightness_High);
				}
				break;
		}
	}
}

// Main processing function
static void run(LV2_Handle instance, uint32_t n_samples) {
	TogglePlugin* plugin = (TogglePlugin*)instance;

	// Check the toggle states
	uint8_t toggle1on = (*(plugin->toggle1) > 0.0f) ? 1 : 0;
	uint8_t toggle2on = (*(plugin->toggle2) > 0.0f) ? 1 : 0;

	// We want the state determination to be an XOR operation
	// If only one of them is on, it means the other has not attempted to toggle it off,
	// therefore, assume on. If both are on, it actually means that one turned
	// it on, and the other attempted to toggle it off, so assume off.
	uint8_t new_state = toggle1on ^ toggle2on;

	// If the state has changed, update the state and CV output
	if (plugin->state != new_state) {
		plugin->state = new_state;

		// Set the CV output to 10V if the state is on, 0V if off
		float cv_value = plugin->state ? 10.0f : 0.0f;
		bool onoff = plugin->state ? true : false;
		for (uint32_t i = 0; i < n_samples; i++) {
			plugin->cv_output[i] = cv_value;
		}

		// Update the LED status based on the new state
		trigger_led_change(plugin, TOGGLE1_PORT, onoff);
		trigger_led_change(plugin, TOGGLE2_PORT, onoff);
	}
}

// Cleanup the plugin instance
static void cleanup(LV2_Handle instance) {
	free(instance);
}

// Handle addressing of HMI widgets
static void addressed(LV2_Handle handle, uint32_t index, LV2_HMI_Addressing addressing, const LV2_HMI_AddressingInfo* info)
{
	TogglePlugin* plugin = (TogglePlugin*) handle;

	if (index == TOGGLE1_PORT) {
		plugin->toggle_addressing1 = addressing;
	}
	if (index == TOGGLE2_PORT) {
		plugin->toggle_addressing2 = addressing;
	}
}

// Handle unaddressing of HMI widgets
static void unaddressed(LV2_Handle handle, uint32_t index)
{
	TogglePlugin* plugin = (TogglePlugin*) handle;

	if (index == TOGGLE1_PORT) {
		plugin->toggle_addressing1 = NULL;
	}
	if (index == TOGGLE2_PORT) {
		plugin->toggle_addressing2 = NULL;
	}
}

// Extension data for the HMI feature
static const void* extension_data(const char* uri)
{
	static const LV2_HMI_PluginNotification hmiNotif = {
		addressed,
		unaddressed,
	};

	if (!strcmp(uri, LV2_HMI__PluginNotification))
		return &hmiNotif;

	return NULL;
}

// Descriptor for the plugin
static const LV2_Descriptor descriptor = {
	PLUGIN_URI,
	instantiate,
	connect_port,
	NULL,  // Activate function not used
	run,
	NULL,  // Deactivate function not used
	cleanup,
	extension_data
};

// Entry point for the plugin
LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index) {
	return (index == 0) ? &descriptor : NULL;
}

