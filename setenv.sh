#LD_LIBRARY_PATH=/home/analog/Qt/5.9/lib/:/home/analog/gcc-linaro-5.4.1-2017.05-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib/
#export QT_DEBUG_PLUGINS=1
#export QT_LOGGING_RULES=qt.qpa.input=true
#export QT_MESSAGE_PATTERN="[%{type}] ( file://%{file}:%{line} ) - %{message}"
#QT_PLUGIN_PATH=/home/analog/Qt/5.9/plugins
export QT_QPA_EVDEV_KEYBOARD_PARAMETERS=/dev/input/by-path/platform-gpio_keys-event:grab=1
export QT_QPA_EVDEV_MOUSE_PARAMETERS=/dev/input/by-path/platform-rotary-event:grab=1
export QT_QPA_FB_DRM=1
export QT_QPA_FB_HIDECURSOR=0
export QT_QPA_FB_TSLIB=1
export QT_QPA_FONTDIR=/home/analog/Qt/fonts
export QT_QPA_PLATFORM=linuxfb
export QT_QPA_GENERIC_PLUGINS=evdevmouse,evdevkeyboard
