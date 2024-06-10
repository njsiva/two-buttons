# Two-Button LV2 Plugin

## Overview

The Two-Button LV2 Plugin is a custom plugin designed to handle two toggle switches with associated control voltage (CV) outputs. This plugin is designed to use with a MOD device, allowing users to use two toggle switches to toggle the same CV port like a two-way switch. If toggle A is turned on, and toggle B is associated with a MOD hardware button but not in an off state, the CV outputs HIGH and the toggle B LED will turn on to reflect this status. If Toggle B is also turned on, then this effectively turns off (toggles) the previous state and the CV and LED reflect this. Alternatively, if A is off, but B is turned on, CV and LED turn on and toggle A will now toggle the state. In other words, the CV and LED will display/act like an XOR output for the toggles. This can be useful when a MIDI controller with latching buttons are attached to a MOD device and in conjunction with a hardware button can be used to turn on or off an effects pedal or other toggle operated function.

## Features

- Two toggle switches.
- Each toggle switch controls a single combined CV output.
- Integrated support for LED indicators.
- Bypass functionality for easy switching.

## License
This project is licensed under the GNU General Public License v3.0. See the LICENSE file for details.

## Acknowledgements
Thanks to the MOD community for their support and resources.
