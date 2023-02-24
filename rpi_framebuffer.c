
/* rpi_framebuffer.c                                                                  */
/* Mapping the Raspberry Pi's (RPi-4) video framebuffer to memory for direct access   */
/* and testing it by writing to it.                                                   */
/* Drawing a filled color rectangular box and an RGB565 raw bitmap file with          */
/* fixed name and dimensions.                                                         */
/* Basic code found here:  https://gist.github.com/Darfk/5790622                      */
/* Modified and enhanced by DL8NDQ, 2023-02-23                                        */
/* Status: Functional                                                                 */
/* Idea for further development: Use a fontset with characters of equal width as an   */
/* array of small bitmaps. Load it into memory and construct a funtion to read out    */
/* the character images according to an ASCII character.                              */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


int main (int argc, char* argv[])
{
    // sample file created with commands (ImageMagick and FFmpeg required):
    //     convert -background black -fill lightgreen -font FreeMono-Bold -pointsize 28 \
    //         label:"the quick brown fox ...\n0123456789-123456789..." textimg.png
    //     identify textimg.png   # gives dimensions = 392x59
    //     ffmpeg -vcodec png -i textimg.png -vcodec rawvideo -f rawvideo -pix_fmt rgb565 textimg_raw.rgb565
    FILE *raw565img;                      // file pointer of RGB565 raw bitmap image
    char *imgname="textimg_raw.rgb565";   // file name = "textimg_raw.rgb565"
    int ixd=392, iyd=59;                  // dimensions of image: x=392 y=59

    /* framebuffer file descriptor     */
    int fbfd;

    /* tty file descriptor             */
    /* if we can't open the tty because it is not writeable then we'll just leave it in text mode  */
    int ttyfd;

    /* structures for important fb information  */
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
  
    /* screen size in bytes            */
    /* x * y * (bits/px) / 8           */
    unsigned long int screensize;

    /* variables for painting things   */
    int xdim, ydim, xi, yi, xliob, yliob, red, green, blue, pxcolor;
    unsigned long int xyadr;

    /* pixel specification 16 bits     */
    typedef short pixel_t;

    pixel_t* fbp;     // frame buffer pointer is addressing 16 bit pixel values

    if (argc != 8)  {
        printf(" ---> rpi_framebuffer.c \n");
        printf("      DL8NDQ, 2023-feb-23 \n");
        printf("      Direct writing into RPi4 frame buffer \n");
        printf("      Usage: ./rpi_framebuffer <xdim> <ydim> <xliob> <yliob> <red> <green> <blue> \n");
        printf("          with properties of rectangle: \n");
        printf("          <xdim> <ydim> = dimensions of rectangle, <xliob> <yliob> = px coords of top left corner, \n");
        printf("          <red> <green> <blue> = RGB color values for pixels: 0-255 each. \n");
        return(-1);
    }

    if (!(sscanf(argv[1],"%d",&xdim)))   return(-2);
    if (!(sscanf(argv[2],"%d",&ydim)))   return(-3);
    if (!(sscanf(argv[3],"%d",&xliob)))  return(-4);
    if (!(sscanf(argv[4],"%d",&yliob)))  return(-5);
    if (!(sscanf(argv[5],"%d",&red)))    return(-6);  else  red   = (red   & 0xFF);
    if (!(sscanf(argv[6],"%d",&green)))  return(-6);  else  green = (green & 0xFF);
    if (!(sscanf(argv[7],"%d",&blue)))   return(-6);  else  blue  = (blue  & 0xFF);

    red   = red   * 32/256;     // rescale red   color value to 5 bits
    green = green * 64/256;     // rescale green color value to 6 bits
    blue  = blue  * 32/256;     // rescale blue  color value to 5 bits
    pxcolor = red * 2048 + green * 64 + blue;     // combine RGB to 16bit color word (Bits15...0 = rrrrrggggggbbbbb)
    printf(" ---> Color word = %d with components: R = %d G = %d B = %d \n", pxcolor, red, green, blue);

    if ((raw565img=fopen(imgname,"rb"))==NULL)
    {
        printf (" ---> No image file %s found for opening! \n", imgname);
        return(-7);
    }
    else  printf(" ---> Raw RGB565 image file is %s \n", imgname);

    /* open framebuffer                 */
    fbfd = open("/dev/fb0", O_RDWR);
    if(fbfd == -1)
    {
        printf(" ---> Error: cannot open framebuffer device \n");
        exit(1);
    }

    /* set the tty to graphics mode     */
    /* get fixed screen information     */
    if(ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        printf(" ---> Error: Reading fixed information \n");
        exit(2);
    }

    /* get variable screen information  */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        printf(" ---> Error: Reading variable information \n");
        exit(3);
    }

    printf(" ---> Device Id = %s\n", finfo.id);
    printf(" ---> Screen: %d px wide, %d px high , %d bits per px \n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
  
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8 ;
    printf(" ---> Screen size = %lu bytes \n", screensize);

    unsigned int num_pixels = screensize / sizeof ( pixel_t );
    printf(" ---> Number of pixels on screen = %u \n", num_pixels);
  
    fbp = (pixel_t*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if ((int)fbp == -1)
    {
        printf(" ---> Error: Failed to map framebuffer device to memory \n");
        exit(4);
    }

    /* attempt to open the tty and set it to graphics mode  */
    ttyfd = open("/dev/tty1", O_RDWR);
    if (ttyfd == -1)
        printf(" ---> Error: Could not open the tty \n");
    else  ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
  
    /* do your display renderings here : Plot a color filled rectangle:  */
    for (yi=0; yi<ydim; yi++)           // offset in vertical axis
    {
        for (xi=0; xi<xdim; xi++)       // offset in horizontal axis
        {
            xyadr = xliob + xi + vinfo.xres * ( yliob + yi );
//            fbp[ xyadr ] = 0b1111100000011111;                 // violett = rrrrrggggggbbbbb
                    // take care not to write outside the memory array to avoid system hangup:
            if (xyadr < num_pixels)  fbp[ xyadr ] = pxcolor;     // rrrrrggggggbbbbb
        }
    }

    sleep(2);    /* hold the drawing for 2 seconds   */

    /* finally plot the Raw-RGB565 bitmap image to a fixed screen position:   */
    for (yi=0; yi<iyd; yi++)            // offset in vertical axis
    {
        for (xi=0; xi<ixd; xi++)        // offset in horizontal axis
        {
            xyadr = 960 + xi + vinfo.xres * ( 600 + yi );   // plot from position x=960 y=600 to down and right
                    // take care not to write outside the memory array to avoid system hangup:
            if (xyadr < num_pixels)  fbp[ xyadr ] = fgetc(raw565img) + 256 * fgetc(raw565img);
        }
    }

    sleep(4);    /* hold the drawing for 4 seconds   */

    /* unmap the memory and release all the files    */
    munmap(fbp, screensize);

    if (ttyfd != -1)
    {
        ioctl(ttyfd, KDSETMODE, KD_TEXT);
        close(ttyfd);
    }

    close(fbfd);
    fclose(raw565img);

    return 0;
}
