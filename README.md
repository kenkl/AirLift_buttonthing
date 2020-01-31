# AirLift_buttonthing

### Borrowing things from [buttonthing](https://github.com/kenkl/buttonthing) to build an experimental device. 

Using a [DC Boarduino](https://www.adafruit.com/product/72) (it's been discontinued for a number of years now, but it's a simple Arduino-like clone that I've a couple copies of hanging about, so...) with an [AirDrop](https://www.adafruit.com/product/4201) to make a button thing to send commands to [Lights](https://github.com/kenkl/lights). 

It's a crude build - most of the comments from the sample sketch, WiFiWebClient.ino, are left in place for now, and the breadboard build is a pretty casual assembly of the parts needed to make it work:

![AirLift_buttonthing](https://i.imgur.com/mYJM0Xn.jpg "AirLift buttonthing")

It *does* work, and might be a way to revive/recycle some of the older Arduino-like devices in my bin with internet connectivity. An inspiration for this is the idea of combining what I've assembled in [pirthing](https://github.com/kenkl/pirthing) with the long-standing usefulness of [MotionNightlight](https://github.com/kenkl/MotionNightlight) to let it send triggers to Lights to control Hue lights in addition to its on-board illumination. MotionNightlight is built around a Boarduino, and this would modernise it nicely.

The code is assembled/compiled/uploaded with VisualStudioCode with PlatformIO, and includes the Adafruit WiFiNINA libraries (dated 2019-09-03) in /lib.
