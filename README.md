# AirLift_buttonthing

### Borrowing things from [buttonthing](https://github.com/kenkl/buttonthing) to build an experimental device. 

Using a [DC Boarduino](https://www.adafruit.com/product/72) (it's been discontinued for a number of years now, but it's a simple Arduino-like clone that I've a couple copies of hanging about, so...) with an [AirDrop](https://www.adafruit.com/product/4201) to make a button thing to send commands to [Lights](https://github.com/kenkl/lights). 

It's a crude build - most of the comments from the sample sketch, WiFiWebClient.ino, are left in place for now, and the breadboard build is a pretty casual assembly of the parts needed to make it work:

![AirLift_buttonthing](https://i.imgur.com/mYJM0Xn.jpg "AirLift buttonthing")

It *does* work, and might be a way to revive/recycle some of the older Arduino-like devices in my bin with internet connectivity. An inspiration for this is the idea of combining what I've assembled in [pirthing](https://github.com/kenkl/pirthing) with the long-standing usefulness of [MotionNightlight](https://github.com/kenkl/MotionNightlight) to let it send triggers to Lights to control Hue lights in addition to its on-board illumination. MotionNightlight is built around a Boarduino, and this would modernise it nicely.

The code is assembled/compiled/uploaded with VisualStudioCode with PlatformIO, and includes the Adafruit WiFiNINA libraries (dated 2019-09-03) in /lib.

#### Changelog

2020-01-30:

- Initial build. Mildly amazed that it worked so well with this little effort.

2020-02-03:

- Caught a problem. I suspect that it's related to being on a pretty bare-bones ATMEGA328P; another platform may do better. What happens is that after a certain amount of time or calls, *something* will get hung up and it throws "No Socket available" when trying to connect. I moved all the WiFi initialization from setup() to doWiFi() in an attempt to work around the problem. That didn't work - I suspect a resource issue on the ATMEGA328P. So, I've implemented a [software reset](https://www.theengineeringprojects.com/2015/11/reset-arduino-programmatically.html) to "reboot" the whole thing after flashing the LED a few times to provide feedback. That *appears* to work great.
- Over in [Lights](https://github.com/kenkl/lights), I implemented but7.php to abstract kcstog.php a couple weeks ago. Let's use that here.

