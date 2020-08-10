// #include <stdio.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>
// #include <malloc.h>
// #include <ogcsys.h>
// #include <gccore.h>
// #include <ogc/ipc.h>
// #include <ogc/wpad.h>

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

// /** Alignment required for USB structures (I don't know if this is 32 or less). */
// #define USB_ALIGN __attribute__ ((aligned(32)))
//
// char hciCommandData1[] USB_ALIGN = {
//     /* bmRequestType */
//     0x20
// };
//
// char hciCommandData2[] USB_ALIGN = {
//     /* bmRequest */
//     0x00
// };
//
// char hciCommandData3[] USB_ALIGN = {
//     /* wValue */
//     0x00, 0x00
// };
//
// char hciCommandData4[] USB_ALIGN = {
//     /* wIndex */
//     0x00, 0x00
// };
//
// char hciCommandData5[] USB_ALIGN = {
//     /* wLength */
//     0x03, 0x00
// };
//
// char hciCommandData6[] USB_ALIGN = {
//     /* unknown; set to zero. */
//     0x00
// };
//
// char hciCommandData7[] USB_ALIGN = {
//     /* Mesage payload. */
//
//     // See Bluetooth Core Specification v5.2 (BCSv5.2)
//     // For the HCI_Read_BD_ADDR host-to-controller command:
//     // OGF (opcode group field)   = 0x04   (= 0100b)   (BCSv5.2 § 7.4, p 2223)
//     // OCF (opcode command field) = 0x0009 (= 1001b)   (BCSv5.2 § 7.4.6, p 2231)
//     // OGF is upper 6 bits, OCF is lower 10 bits
//     // ∴ full command opcode = 0001 0000 0000 1001 = 0x1009
//     //                         ^^^^ ^^~~ ~~~~ ~~~~
//     //                          OGF       OCF
//     // finally, we reverse the byte order to obtain:
//     0x09, 0x10,
//
//     // The length of command parameters we are passing is:
//     0x00
// };
//
// /** Vectors of data transfered. */
// ioctlv hciCommandData[] USB_ALIGN = {
//     {
//         hciCommandData1,
//         sizeof(hciCommandData1)
//     },
//     {
//         hciCommandData2,
//         sizeof(hciCommandData2)
//     },
//     {
//         hciCommandData3,
//         sizeof(hciCommandData3)
//     },
//     {
//         hciCommandData4,
//         sizeof(hciCommandData4)
//     },
//     {
//         hciCommandData5,
//         sizeof(hciCommandData5)
//     },
//     {
//         hciCommandData6,
//         sizeof(hciCommandData6)
//     },
//     {
//         hciCommandData7,
//         sizeof(hciCommandData7)
//     }
// };

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

    // puts("Opening file descriptor for internal USB Bluetooth Controller...");
    // s32 fd = IOS_Open("/dev/usb/oh1/57e/305", IPC_OPEN_WRITE);
    // printf("Received file descriptor: %d.\n\n", fd);
    
    // identifymii_pause(1);

    // puts("Sending HCI_Read_BD_ADDR to Controller via USB Device Request (ioctl)...");
    // int rv = IOS_Ioctlv(fd, 0, 6, 1, hciCommandData);
    // printf("Received return code: %d.\n\n", rv);
    
    // puts("Sending ")
    // ioctlv(fd, '''2''', 2, 1, vectors); //Interrupt

    identifymii_pause(1);

    // IOS_Close(fd);
    // puts("Closed file descriptor for internal USB Bluetooth Controller.\n");
    
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
