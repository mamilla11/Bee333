import minimalmodbus
import serial.tools.list_ports
import time

mb = None
minimalmodbus.TIMEOUT = 10

def FindDevice():
    global mb

    for p in list(serial.tools.list_ports.comports()):
        if (p.vid == 0x0403) and (p.pid == 0x6001):
            mb = minimalmodbus.Instrument(str(p.device), 0x80, mode='ascii')
            if not mb.serial.is_open:
				mb.serial.open()
            print('Device Found!')
            return True
    
    return False


FindDevice()

while(1):
    for i in range(0,1):
        start_reg = 8 * i
        address      = mb.read_register(start_reg + 0, functioncode=0x03)
        status       = mb.read_register(start_reg + 1, functioncode=0x03)
        current      = mb.read_register(start_reg + 2, functioncode=0x03)
        power        = mb.read_register(start_reg + 3, functioncode=0x03)
        voltage      = mb.read_register(start_reg + 4, functioncode=0x03)
        light        = mb.read_register(start_reg + 5, functioncode=0x03)
        temp_1       = mb.read_register(start_reg + 6, functioncode=0x03)
        humi_1       = mb.read_register(start_reg + 7, functioncode=0x03)
        temp_2       = mb.read_register(start_reg + 8, functioncode=0x03)
        humi_2       = mb.read_register(start_reg + 9, functioncode=0x03)
        pressure     = mb.read_register(start_reg + 10, functioncode=0x03)
        acc_voltage  = mb.read_register(start_reg + 11, functioncode=0x03)
        rssi         = mb.read_register(start_reg + 12, functioncode=0x03)
        
        if (rssi & 0x8000):
            rssi = (rssi & 0x7FFF) * -1
        
        print('address:  0x%02X'   % address)
        print('current:  %.2f\tmA' % (current / 100.0))
        print('power:    %.2f\tmW' % (power / 100.0))
        print('voltage:  %d\tmV'   % voltage)
        print('light:    %d\tlux'  % (light * 2))
        print('temp_1:   %.1f\tC'  % (temp_1 / 10.0))
        print('humi_1:   %d\t%%rH' % humi_1)
        print('temp_2:   %.1f\tC'  % (temp_2 / 10.0))
        print('humi_2:   %d\t%%rH' % humi_2)
        print('pressure: %d\thPa'  % pressure)
        print('accum:    %d\tmV'   % acc_voltage)
        print('rssi:     %d'       % rssi)
        print('\n')
    
    time.sleep(1)