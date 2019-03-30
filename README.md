# AffoFly_RX
AffoFly RC Receiver. It is a part of [AffoFly](https://github.com/utsxumiao/AffoFly) -- a complete guild of DIY drone at affordable cost.
It is developed to only work with [AffoFly_TX](https://github.com/utsxumiao/AffoFly_TX) currently but might work with more protocols in the future.

### Features
- Popular 2.4G frequency which is free to use in most countries.
- Binding procedure does not require computer.
- 12 channels on CPPM/PPM output
- 6 channels on PWM output
- LED status indication

### Required Materials
I've estimated total cost of the whole lot should be something $3 - $4.

| Material | QTY |
| -------- | --- | ---- |
| Arduino Pro Mini 3.3v | 1 |
| NRF24L01+ | 1 |
| LED RED | 1 |
| LED BLUE | 1 |
| 2.54mm PIN | n |
| Jumper Socket | 1 |
| 10uF Electrolytic Capacitor | 1 |
| 0.1uf Ceremic Capacitor | 1 |
| 3.5mm Audio Jack | 1 (optional) |

### Warning
This software is a work in progress and has not been tested in field yet. Code should be only used for reference until I feel comfortable to make it as a release version. DRONE CAN BE VERY DANGEROUS SO REMOVE YOUR PROPELLER UNTIL ACTUAL FLY!!!

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### Side Notes
 - **Radio Frequency**. The NRF24L01+ module provides 126 channels (0 - 125) which map to frequencies from 2400mhz to 2525mhz respectively, if you are planning to use it in areas close to residential like even indoor (a lot of WIFI) better stick to channels from 85 (2485mhz) and above to minimize interference, it gives you 40 channels to choose from, for simplicity I prefer starts from 100 and 25 channels is plenty for me. According to [List of WLAN channels](https://en.wikipedia.org/wiki/List_of_WLAN_channels) most WIFI routers are using frenquency under 2484mhz.

### Acknowlegements
- [parkchan2017](https://github.com/parkchan2017). Developed Menu System for AffoFly_TX, improved and taught me programming in C/C++. (I am a hopeless .NET developer)
- [iforce2d](https://www.youtube.com/channel/UCTXOorupCLqqQifs2jbz7rQ). He has heaps of interesting, informative and yet good quality videos.
- [Savage Makers](http://savagemakers.com/differential-drive-tank-drive-in-arduino/). I've referenced/copied code and methodology from Savage Makers in Dual Motor Differential PWM module.
- [MultiWii Community](http://www.multiwii.com)
- [Arduino](https://www.arduino.cc)

### Licensing
[MIT License](/LICENSE)
