#!/bin/bash

# Check GUI components
echo "Checking GUI components..."
if [ -f /usr/bin/rfsom-box-gui ] && \
   [ -f /usr/bin/rfsom-box-gui-start.sh ] && \
   [ -d /usr/share/rfsom-box-gui ] && \
   [ -f /lib/systemd/system/rfsom-box-gui.service ]; then
    echo "✓ GUI components installed successfully!"
else
    echo "✗ GUI components failed to install!"
    [ ! -f /usr/bin/rfsom-box-gui ] && echo "  Missing: rfsom-box-gui executable"
    [ ! -f /usr/bin/rfsom-box-gui-start.sh ] && echo "  Missing: rfsom-box-gui-start.sh"
    [ ! -d /usr/share/rfsom-box-gui ] && echo "  Missing: share directory"
    [ ! -f /lib/systemd/system/rfsom-box-gui.service ] && echo "  Missing: systemd service"
    exit 1
fi

# Check TUN/TAP components
echo "Checking TUN/TAP components..."
if [ -f /usr/bin/modemd ] && \
   [ -f /usr/bin/ip_reg ] && \
   [ -f /usr/bin/ip_monitor ] && \
   [ -f /usr/bin/restart_modem_gui.sh ] && \
   [ -f /usr/bin/ip_reg_default.sh ] && \
   [ -f /usr/share/rfsom-box-gui/modem_filter.ftr ]; then
    echo "✓ TUN/TAP components installed successfully!"
else
    echo "✗ TUN/TAP components failed to install!"
    [ ! -f /usr/bin/modemd ] && echo "  Missing: modemd"
    [ ! -f /usr/bin/ip_reg ] && echo "  Missing: ip_reg"
    [ ! -f /usr/bin/ip_monitor ] && echo "  Missing: ip_monitor"
    [ ! -f /usr/bin/restart_modem_gui.sh ] && echo "  Missing: restart_modem_gui.sh"
    [ ! -f /usr/bin/ip_reg_default.sh ] && echo "  Missing: ip_reg_default.sh"
    [ ! -f /usr/share/rfsom-box-gui/modem_filter.ftr ] && echo "  Missing: modem_filter.ftr"
    exit 1
fi

# Check FFT plot component
echo "Checking FFT plot component..."
if [ -f /usr/bin/fft_plot ]; then
    echo "✓ FFT plot component installed successfully!"
else
    echo "✗ FFT plot component failed to install!"
    echo "  Missing: fft_plot executable"
    exit 1
fi

echo ""
echo "All components installed successfully!"

