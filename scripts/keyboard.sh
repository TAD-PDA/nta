sudo insmod ./git/soft_uart/soft_uart.ko gpio_tx=26 gpio_rx=19
stty 4800 -F /dev/ttySOFT0 icanon min 1
sleep 1
echo Keyboard TTY setup complete!
