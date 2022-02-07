EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no        # Console for debug
COMMAND_ENABLE = no        # Commands for debug and configuration
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = no            # USB Nkey Rollover
COMBO_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes
TAP_DANCE_ENABLE = yes
ENCODER_ENABLE = yes       # ENables the use of one or more encoders
MOUSEKEY_ENABLE = no
LEADER_ENABLE = yes	
EXTRAFLAGS += -flto
# SRC += oneshot.c
SRC += features/casemodes.c