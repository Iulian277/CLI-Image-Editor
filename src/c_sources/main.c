#include "../headers/commons.h"
#include "../headers/image.h"
#include "../headers/rotate.h"
#include "../headers/noise_red.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"



int main(void)
{
    int width, height, comp;
    unsigned char *pixels = stbi_load("./images/img_before_compression.jpg", &width, &height, &comp, STBI_rgb);

    Image *image = alloc_image(height, width, STBI_rgb);

    data_to_matrix(image, pixels);
    double **A_rec_red   = svd(image->red,   height, width, MIN(width, height));
    double **A_rec_green = svd(image->green, height, width, MIN(width, height));
    double **A_rec_blue  = svd(image->blue,  height, width, MIN(width, height));

    replace_pixels(image->red,   A_rec_red,   height, width);
    replace_pixels(image->green, A_rec_green, height, width);
    replace_pixels(image->blue,  A_rec_blue,  height, width);

    matrix_to_data(image, pixels);
    stbi_write_png("./images/png4_out.png", width, height, comp, pixels, width * comp);


    dealloc_image(image);
    free(pixels);
    
    return 0;
}