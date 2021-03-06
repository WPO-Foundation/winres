// winres.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "winres.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    DEVMODE mode;
    memset(&mode, 0, sizeof(mode));
    mode.dmSize = sizeof(mode);
    DWORD x = 0, y = 0, bpp = 0;

    int index = 0;
    DWORD targetWidth = 1920;
    DWORD targetHeight = 1200;
    DWORD min_bpp = 15;
    while( EnumDisplaySettings( NULL, index, &mode) ) {
      index++;
      bool use_mode = false;
      if (x >= targetWidth && y >= targetHeight && bpp >= 24) {
        // we already have at least one suitable resolution.  
        // Make sure we didn't overshoot and pick too high of a resolution
        // or see if a higher bpp is available
        if (mode.dmPelsWidth >= targetWidth && mode.dmPelsWidth <= x &&
            mode.dmPelsHeight >= targetHeight && mode.dmPelsHeight <= y &&
            mode.dmBitsPerPel >= bpp)
          use_mode = true;
      } else {
        if (mode.dmPelsWidth == x && mode.dmPelsHeight == y) {
          if (mode.dmBitsPerPel >= bpp)
            use_mode = true;
        } else if ((mode.dmPelsWidth >= targetWidth ||
                    mode.dmPelsWidth >= x) &&
                   (mode.dmPelsHeight >= targetHeight ||
                    mode.dmPelsHeight >= y) && 
                   mode.dmBitsPerPel >= min_bpp) {
            use_mode = true;
        }
      }
      if (use_mode) {
          x = mode.dmPelsWidth;
          y = mode.dmPelsHeight;
          bpp = mode.dmBitsPerPel;
      }
    }

    // get the current settings
    if (x && y && bpp && 
      EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode)) {
      if (mode.dmPelsWidth < x || 
          mode.dmPelsHeight < y || 
          mode.dmBitsPerPel < bpp) {
        DEVMODE newMode;
        memcpy(&newMode, &mode, sizeof(mode));
      
        newMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        newMode.dmBitsPerPel = bpp;
        newMode.dmPelsWidth = x;
        newMode.dmPelsHeight = y;
        ChangeDisplaySettings( &newMode, CDS_UPDATEREGISTRY | CDS_GLOBAL );
      }
    }

    return 0;
}
