# identify-mii
## Wii homebrew that prints the Bluetooth address of the console

Displays the Bluetooth MAC address of the Bluetooth Controller built into a given Wii (U) console, which is used communicate with game controllers (like Wiimotes) wirelessly.

Once the address of your console has been determined, you can try to use it in conjunction with [WiimoteEmulator](https://github.com/rnconrad/WiimoteEmulator) to have a Linux machine spoof a Wiimote.

## Install

### Homebrew Channel

If you haven't already, install the Homebrew Channel on your normal Wii or vWii (Wii Mode on a Wii U). There are a number of ways to do this, and it isn't too difficult.

Some important notes for the uninitiated:

- Game-based methods (Indiana Pwns, Bathaxx, etc.) are reliable and work on either console, but require you to have a specific game disc.
- LetterBomb and Internet Browser methods do not work on a vWii. _I'm out of the loop on whether they still work on normal Wiis, so please do your research._
- Wuphax lets you softmod your vWii if you've already softmodded your Wii U (with Haxchi). _I haven't tried it myself, but it is recommended by the community at [wiiu.hacks.guide](https://wiiu.hacks.guide)_.

Regardless of the method you choose, do your research first. Prefer text guides over videos whenever possible, as videos are much harder to update.

LMGTFY:

- [Wii](https://www.google.com/search?q=install+homebrew+channel)
- [vWii (Wii U)](https://www.google.com/search?q=install+homebrew+channel+vwii)

### identify-mii

Once the Homebrew Channel is installed, download the [latest release of identify-mii](https://github.com/ThatsJustCheesy/identify-mii/releases) and extract the zip into the `apps` folder in the root of your SD card.

Now you can pop the SD card back into the Wii and launch the Homebrew Channel; "identify-mii" should be listed there.

## Use

In the Homebrew Channel, select "identify-mii", then "Load". A basic text interface will apppear, and identify-mii will print your console's Bluetooth address. Take a picture of this or write it down!

To quit, press HOME on a Wiimote or any key on a USB keyboard connected to the console. If quitting doesn't work for some reason, just power off the console.

Enjoy :)

## Build from source

devkitPPC is required.

Build:

    ./make.sh

Clean:

    ./make.sh clean

Run (with Wiiload):

    ./make.sh run

### Package a release

Build, then copy `identify-mii.dol` and `meta.xml` into a folder named `identify-mii`, and finally rename `identify-mii.dol` to `boot.dol`. This is now a working release that can be placed in the `apps` folder.
