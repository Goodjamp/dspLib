import math
import string
import sys

scaleCoeff = 1000
coeff = 0.0

if len(sys.argv) != 3:
    print("Wrong argument number")
    quit()

coefNumber = int(sys.argv[1], 10)
try:
    pathToFile = sys.argv[2]
except:
    print("The coeficient size not e number")
    quit()

if ((coefNumber - 1) & coefNumber) != 0:
    print("The coeficient size must be pow2")
    quit()

FileName = "FftRotateCoeff"

print("size = {}".format(coefNumber))

rotateCoefFile = open(pathToFile + FileName + ".c", "w+");
rotateCoefFile.write("\
#include <stdint.h>\n\
#include \"" + FileName + ".h\"\n\
\n\
static const float re[{size}] = {{\n".format(size = coefNumber)) # double "{{" - it is escape for insert symbol "{"" 

for k in range(0, coefNumber):
    rotator = math.cos(-1.0 * 2 * math.pi * k / coefNumber)
    #rotator = int(rotator * scaleCoeff)
    rotateCoefFile.write("    {:.5},\n".format(0.000000 if (rotator < 0.00001) & (rotator > -0.00001) else rotator))

rotateCoefFile.write("};\n\n");

rotateCoefFile.write("\
static const float im[{size}] = {{\n".format(size = coefNumber))

for k in range(0, coefNumber):
    rotator = math.sin(-1.0 *2 * math.pi * k / coefNumber)
    #rotator = int(rotator * scaleCoeff)
    rotateCoefFile.write("    {:.5},\n".format(0.000000 if (rotator < 0.00001) & (rotator > -0.00001) else rotator))

rotateCoefFile.write("};\n\n");

rotateCoefFile.write("static const FftRotateCoef rotateCoef = {{\n\
    .re = re,\n\
    .im = im,\n\
    .size = {size},\n\
    .scaleCoeff = {scaleC}\n\
}};\n\
\n\
const FftRotateCoef *fftGetCoeff(void)\
{{\n\
    return &rotateCoef;\n\
}}".format(size = coefNumber, scaleC = scaleCoeff));

rotateCoefFile.close();

rotateCoefFile = open(pathToFile + FileName + ".h", "w+");
rotateCoefFile.write("\
#include <stdint.h>\n\
\n\
#ifndef __ROTATE_COEFF_H__\n\
#define __ROTATE_COEFF_H__\n\
\n\
typedef struct {\n\
    const float *re;\n\
    const float *im;\n\
    uint32_t size;\n\
    uint32_t scaleCoeff;\n\
} FftRotateCoef;\n\
\n\
const FftRotateCoef *fftGetCoeff(void);\n\
\n\
#endif\n\
")