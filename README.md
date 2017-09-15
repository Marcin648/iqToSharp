# iqToSharp
Converter RTL-SDR iq to SDRSharp WAV format.

## Examples

Basic
```
rtl_sdr -f 102000000 -g 50 fm.iq
iqToSharp -f 102000000 -i fm.iq -o radio
```

Custom sample rate
```
rtl_sdr -f 137000000 -s 1024000 -g 50 noaa.iq
iqToSharp -f 137000000 -s 1024000 -i noaa.iq -o noaa
```

## Video
[![](http://img.youtube.com/vi/N2gtyJAlTz8/0.jpg)](http://www.youtube.com/watch?v=N2gtyJAlTz8)
