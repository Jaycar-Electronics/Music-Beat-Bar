# Music-Beat-Bar

Pump up the Jam with this beat bar! Get a visual display that bounces in tune with the music. Uses a new 8-bit-friendly Fast Fourier Transform library to detect different frequencies and pulses the bars for base, midranges, and treble. Contained in a tidy little box so you can take it and hang it up at your next party.


## Bill of Materials

|Qty| Code | Description |
|---|---|---|
|1 | [XC4414](http://jaycar.com.au/p/XC4414) | Arduino Nano
|1 | [XC4384](http://jaycar.com.au/p/XC4384) | Monochrome OLED screen
|1 | [HB6082](http://jaycar.com.au/p/HB6082) | Black enclosure
|4 | [XC4380](http://jaycar.com.au/p/XC4380) | RGB Strip module
|1 | [XC4438](http://jaycar.com.au/p/XC4438) | Microphone sound sensor module
|1 | [WC6026](http://jaycar.com.au/p/WC6026) | Socket socket jumper leads
|1 | [ST0335](http://jaycar.com.au/p/ST0335) | Toggle switch
|1 | [PH9235](http://jaycar.com.au/p/PH9235) | 9V battery module


## Connection table

| Nano Pin | Connection |
| :------------- | :------------- |
| A0 | Analog Module A0       |
| A4 | LCD SDA pin |
| A5 | LCD SCL pin |
| VIN | toggle switch centre |
| D4 | LED Strip #1 DIN |

Also connect all of the VCC and 5V connections. The battery holder will connect to one side of the toggle switch, which will allow you to turn on/off the unit by feeding 9V into the `VIN` pin of the Nano.

Each of the LED strips will be connected end to end to each other, and will make a zig-zag pattern across the front of the case, as shown in the below diagram.

![](images/pixels.png)

It is important to ensure that each of the `DOUT` and `DIN` matches up, the DATA-OUT of one module must feed into the DATA-IN of the next module.

## Assembly

## Source code
The code in this project uses a library called "FHT" which has been included in with these sketch files. This file was originally created by [Open Music Labs](http://wiki.openmusiclabs.com/wiki/ArduinoFHT).

We want to use a Fast Fourier Transform, or an FFT, to break down audio into a series of frequencies, so we can see how loud the base / mid-range / treble is for a particular audio sample. Keep in mind that the arduino is by no means powerful, and the audio module isn't the greatest quality, so this will result in a much smaller and much noisier signal, but it is more than enough for our purposes.

The FHT library we are using, uses a more real-number friendly FFT conversion algorithm, resulting in a much faster and memory friendly response for our little Arduino.

Here's a bit of a breakdown of the different sections of code:

| Line ranges | Description |
| --- | --- |
| 42 - 54 | Here we take the data out of the analog digital converter for this particular moment in time, as you can see it is a very slim slice of time so we must do this quickly for all points in the array.
| 56 - 59| These are recommended for the library, and so we'll do them here as well. Original comments are included. |
| 72 - 81 | Our additions, just doing some basic signal processing to clear up the signal and make them more deterministic. We also then draw a series of rects onto the screen representing the different heights for the frequencies. |
| 94 - 97 | Here we just pull out random frequencies and put it into a length-4 array, these are the magnitudes for our 4 bar graphs |

Notice how, because the LED bar graph essentially zig-zags around the front panel, we flip the 2nd and 4th pixel orders, going from `(i%8)` to `7-(i%8)` so that all the columns appear to be
## Programming

There's a few different options that you can do in regards to programming this unit. One of the first ideas is to have a multi-colored rainbow sliding through each of the rows of LEDs, another is to have the screen display an audio waveform or have some sort of circular pattern that seems to popular.

With this unit being a simple Nano, there's nothing to programming it other than connecting it up to the Arduino IDE,  and pressing upload.

## Use

Simply turn the device on and put near speakers or in a loud room, the microphone will automatically pick up the beats playing in the room and will dance around on the screen accordingly.
