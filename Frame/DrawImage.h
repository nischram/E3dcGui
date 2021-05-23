/*
gcc -g -o image  image.c
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/ioctl.h>
#include <signal.h>

// 'global' variables to store screen info
int fbfd = 0;
char *fbpI = 0;
//long int page_size = 0;   //Funktion scheint ungenutzt
//int cur_page = 0;         //var ist unbenutzt
//struct fb_var_screeninfo orig_var;   //in der framebuffer.c deklariert
//struct fb_var_screeninfo vinfo;      //in der framebuffer.c deklariert
//struct fb_fix_screeninfo fix;        //in der framebuffer.c deklariert
//int kbfd = 0;            //Funktion habe ich deaktiviert
static struct fb_image image;

int read_ppm(char *fpath, struct fb_image *image) {

	int errval = 0;
	FILE* fp = 0;
    int bytes_per_pixel = 2; // 16 bit

    fp = fopen(fpath, "r");
    if (fp == 0) {
        errval = errno;
        fprintf(stderr, "Error opening file %i (errno=%d).\n", errval, errno);
        return errval;
    }

	char magic[2];
	int width = -1;
	int height = -1;
	int depth = -1;

	if ( (fread(magic, 2, 1, fp) == 1)
		&& (memcmp("P6", magic, 2) == 0) )
	{
		//fprintf(stderr, "Got P6 ppm.\n");

		if (fscanf(fp, "%d %d\n", &width, &height) == 2) {
			//fprintf(stderr, "w=%d, h=%d\n", width, height);
		}
		else {
			fprintf(stderr, "Read size failed.\n");
			width = height = -1;
            errval = EINVAL;
		}

		if (fscanf(fp, "%d\n", &depth) == 1) {
			//fprintf(stderr, "d=%d\n", depth);
		}
		else
		{
			fprintf(stderr, "Read depth failed.\n");
			depth = -1;
            errval = EINVAL;
		}

		if (depth != 255) {
			fprintf(stderr, "Only 255 depth supported.\n");
			depth = -1;
            errval = EINVAL;
		}

	}
	else {
		fprintf(stderr, "Not a P6 ppm.\n");
        errval = EINVAL;
	}

	if ( (width > -1) && (height > -1) && (depth = -1) ) {
		// header read ok

        image->dx = 0;
        image->dy = 0;
        image->width = width;
        image->height = height;
        image->fg_color = 0;
        image->bg_color = 0;
        image->depth = 16;

        // allocate memory
        image->data = malloc(width * height * bytes_per_pixel);
        if (image->data == 0) {
            fprintf(stderr, "Failed to allocate memory.\n");
            errval = ENOMEM;
        }
        else {
            // read
            int ydraw;
            int xdraw;
            unsigned char rgb[3];

            for (ydraw = 0; ydraw < height; ydraw++) {
                for (xdraw = 0; xdraw < width; xdraw++) {
                    if (fread(rgb, 3, 1, fp) == 1) {
                        unsigned char r = rgb[0];
                        unsigned char g = rgb[1];
                        unsigned char b = rgb[2];
                        unsigned short rgb565 = ((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3);
                        // store pixel in memory
                        unsigned int pix_offset = (ydraw * width + xdraw ) * bytes_per_pixel;
                        *((unsigned short *)(image->data + pix_offset)) = rgb565;
                    }
                    else {
                        errval = errno;
                        fprintf(stderr, "Read data failed (errno=%d).\n", errval);
                        break;
                    }
                }
                if (errval != 0)
                    break;
            }
        }
	}

    fclose(fp);

    return errval;
}

// draw
void draw(struct fb_image *image, int posx, int posy) {
    int ydraw;
    int xdraw;
    for (ydraw = 0; ydraw < image->height; ydraw++) {
        for (xdraw = 0; xdraw < image->width; xdraw++) {
            // get pixel from image
            unsigned int img_pix_offset = (ydraw * image->width + xdraw) * 2;
            unsigned short c = *(unsigned short *)(image->data + img_pix_offset);
            // plot pixel to screen
            unsigned int fb_pix_offset = (xdraw + posx) * 2 + (ydraw + posy) * fix.line_length;
            *((unsigned short*)(fbpI + fb_pix_offset)) = c;
        }
    }
}

void cleanup() {
    // reset cursor
    /*if (kbfd >= 0) {
        ioctl(kbfd, KDSETMODE, KD_TEXT);
        close(kbfd);
    }*/
    // unmap fb file from memory
    munmap(fbpI, fix.smem_len);
    // reset the display mode
    /*if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_var)){
        printf("Error re-setting variable information.\n");
		}*/
    // close fb file
    close(fbfd);
    // free image data
    free((void *)image.data);
}

void sig_handler(int signo) {
    //cleanup();    //doppelte Freigabe des Speichers
    exit(signo);
}
int DrawImage(char* fileName, int posx, int posy)
{
		// fill Path with File
		char Path[128];
		snprintf (Path, (size_t)128, "/home/pi/E3dcGui/Image/%s.ppm", fileName);
		// read the image file
    int ret = read_ppm(Path, &image);
    if (ret != 0) {
        //printf("Reading image failed. errno = %i\n",ret);
        return ret;
    }

    // Open the file for reading and writing
    fbfd = open(framebuffer_File, O_RDWR);
    if (fbfd == -1) {
      //printf("Error: cannot open framebuffer device.\n");
      return(1);
    }

    // set up signal handler to handle Ctrl+C
    signal(SIGINT, sig_handler);

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &var)){
      //printf("Error reading variable information.\n");
		}

    // Store for reset (copy var to var_orig)
    memcpy(&orig_var, &var, sizeof(struct fb_var_screeninfo));

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fix)){
      //printf("Error reading fixed information.\n");
		}
    //page_size = fix.line_length * var.yres;

    // hide cursor
    /*kbfd = open("/dev/tty", O_WRONLY);
    if (kbfd >= 0) {
        ioctl(kbfd, KDSETMODE, KD_GRAPHICS);
    }*/

    // map fb to user mem
    fbpI = (char*)mmap(0,
              fix.smem_len,
              PROT_READ | PROT_WRITE,
              MAP_SHARED,
              fbfd,
              0);

    if ((int)fbpI == -1) {
        //printf("Failed to mmap.\n");
    }
    else {
        // draw...
        draw(&image, posx, posy);
				//printf("Draw %s Ok\n",fileName);
    }
    cleanup();
    return 0;
}
