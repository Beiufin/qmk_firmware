USER_NAME := beiufin

#For platform and packs
ARM_ATSAM = SAMD51J18A
MCU = cortex-m4

# Bootloader selection
BOOTLOADER = md-boot

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       	# Enable Bootmagic Lite
MOUSEKEY_ENABLE = no        	# Mouse keys
EXTRAKEY_ENABLE = yes       	# Audio control and System control
CONSOLE_ENABLE = no         	# Console for debug
COMMAND_ENABLE = no         	# Commands for debug and configuration
NKRO_ENABLE = yes           	# Enable N-Key Rollover
BACKLIGHT_ENABLE = no       	# Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        	# Enable keyboard RGB underglow
AUDIO_ENABLE = no           	# Audio output
VIRTSER_ENABLE = no         	# USB Serial Driver
RAW_ENABLE = no             	# Raw device
AUTO_SHIFT_ENABLE = no      	# Auto Shift
APM_ENABLE = yes            	# APM tracking. Needed for APM animations
TAP_REPEAT_ENABLE = no      	# Tap Repeat
KEY_LOCK_TOGGLE_ENABLE = no 	# Key Lock Toggle
DUAL_RGB_MATRIX_ENABLE = yes   	# Dual RGB Matrix effects

# Custom RGB effects
RGB_MATRIX_CUSTOM_USER = yes
