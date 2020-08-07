// Derived from devkitPPC USB Keyboard sample
// and https://wiibrew.org/wiki//dev/usb/oh1
// (massive props to whoever wrote the sample on that wiki page).
// Also consult Universal Serial Bus Specification Revision 2.0, § 9.3 (USB Device Requests)
// and Bluetooth Core Specification Version 5.2, § 5.4.1, § 7.4.6.

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <wiikeyboard/keyboard.h>
#include <bte/bte.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

bool quitapp = false;

void keypress_callback(char sym) {
    quitapp = true;
}

void identifymii_pause(int sec) {
	VIDEO_WaitVSync();
    sleep(sec);
}

struct bd_addr bdaddr;

s32 bdaddr_callback(s32 result, void *userdata) {
    printf("Result code: %d\n", result);
    printf("Your console's Bluetooth address is:\n");
    for (int i = 0; i < 6; i++) {
        printf("%x%s", bdaddr.addr[i], i == 5 ? "" : ":");
    }
    printf("\n\n");
    return result;
}

int main(int argc, char **argv) {
	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	KEYBOARD_Init(keypress_callback);

	puts("identify-mii v1.0.0\nBy Ian Gregory\nBased on samples from devkitPPC and wiibrew.org\n");

	identifymii_pause(3);

    // Always returns ERR_OK.
    (void)BTE_ReadBdAddr(&bdaddr, bdaddr_callback);

    identifymii_pause(1);

    puts("Press HOME or any keyboard key to quit.\nHappy hacking :)");
	do {
		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);

		(void)getchar();

		// We return to the launcher application via exit
		if (pressed & WPAD_BUTTON_HOME) quitapp = true;

		// Wait for the next frame
		VIDEO_WaitVSync();
	} while(!quitapp);

	return 0;
}
