# BikeUSB v2 *(WIP)*
Ever wanted to charge your phone of your bikes dynamo?
This Project does just that, for cheap (ish).

![a rendered image of the PCB front](/images/pcb_top_angle.png)
![a rendered image of the PCB back](/images/pcb_bot_angle.png)
images as of 2024-03-15

## Working Principle
The incoming AC voltage from the dynamo gets rectified by 4 shottky-diodes and buffered in a high-capacity capacitor.
Once a sufficient driving speed is reached and the capacitor is charged enough, the ATtiny1616 MCU on the board wakes up.

It measures the frequency of incoming AC voltage, which is corrolated to the driving speed, until it reaches the first threshold.
Once the first threshold is reached, it can be assumed that enough power is available to charge the phone at 2.5W.
The MCU enables the TPS63020 high efficiency DC/DC-Converter IC to generate 5V at the USB Type C Port.
After the power-good signal is received, the DAC and an external resistor are used to output a current of 80µA
into the CC line on the USB Type C port, signaling to the attached device to use 2.5W (500mA) at maximum.

If the frequency is high enough to reach the 2nd threshold, 7.5W charging will be signaled,
by letting a current of 180µA flow into the CC line.

The 2 buttons are used to program the thresholds. Connecting a 3W or 8W constant load, pressing the right button,
and driving at a steadily increasing speed will let the MCU detect the needed speed to generate enough power.
This prevents constant starting and stopping of the charging process, which is annoying.
