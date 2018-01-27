#include <cstdio>
#include <cstring>
#include <cstdlib>
#define cimg_display 0
#include "CImg.h"
#include <sys/time.h>
#include <ctime>
#include "PermutohedralLatticeCPU.h"
#include "utils.h"

int main(int argc, char **argv) {
    if (argc < 5) {
        printf("Usage: ./bilateral <input file> <output file> <spatial standard deviation> <color standard deviation>\n");
        printf("        PNG, JPG are supported file formats.\n");
        printf("        For instance, try ./bilateral input.png output.png 4 0.5\n");
        return 1;
    }
    float pixel_depth=255.0;
    cimg_library::CImg<unsigned char> image(argv[1]);

    auto flat = get_flat_float_from_image(image, pixel_depth);

    float invSpatialStdev = 1.0f / atof(argv[3]);
    float invColorStdev = 1.0f / atof(argv[4]);

    auto positions = compute_kernel(image, invSpatialStdev, invColorStdev);


    // Filter the input with respect to the position vectors. (see permutohedral.h)
    int N = image.width() * image.height();
    auto out = new float [N * 3]{255};

    printf("Calling filter...\n");
    std::clock_t begin = std::clock();
    filter_cpu(flat, positions, 5, 3, N);
    std::clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("%f seconds\n", elapsed_secs);

    save_output(out, image, argv[2], pixel_depth);

    delete[] flat;
    delete[] positions;
    delete[] out;

    return 0;
}
