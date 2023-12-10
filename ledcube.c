#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define DS 17
#define STCP 27
#define SHCP 22

bool cubeData[8][64];


void writePin(int pin, bool mode) {

    if (mode) {
        gpioWrite(pin, 1);
    } else {
        gpioWrite(pin, 0);
    }

}

/*
 * return (rand() % 1) == 0;
 * 26.01.19
*/
bool randomBoolean() {

    if (rand() % 1 == 0) {
        return false;
    } else {
        return true;
    }

}


void setLed(int x, int y, int z, bool mode) {

    if(z < 0) return;
    if(x < 0) return;
    if(y < 0) return;
    if(x > 7) return;
    if(y > 7) return;
    if(z > 7) return;

    cubeData[z][x + y * 8] = mode;

}


void mirrorAlongAchsis(char achsis) {

    bool mode;

    if (achsis == 'x') {
        for (int x=0; x<4; x++) {
            for (int y=0; y<8; y++) {
                for (int z=0; z<8; z++) {
                    mode = cubeData[z][(7-x) + y * 8];
                    cubeData[z][(7-x) + y * 8] = cubeData[z][x + y * 8];
                    cubeData[z][x + y * 8] = mode;
                }
            }
        }
    } else if (achsis == 'y') {
        for (int y=0; y<4; y++) {
            for (int x=0; x<8; x++) {
                for (int z=0; z<8; z++) {
                    mode = cubeData[z][x + (7-y) * 8];
                    cubeData[z][x + (7-y) * 8] = cubeData[z][x + y * 8];
                    cubeData[z][x + y * 8] = mode;
                }
            }
        }
    } else if (achsis == 'z') {
        for (int z=0; z<4; z++) {
            for (int x=0; x<8; x++) {
                for (int y=0; y<8; y++) {
                    mode = cubeData[(7-z)][x + y * 8];
                    cubeData[(7-z)][x + y * 8] = cubeData[z][x + y * 8];
                    cubeData[z][x + y * 8] = mode;
                }
            }
        }
    }

}


/*
 * use switch
 * 26.01.19
*/
void mirrorFromCorner(int corner) {

    if (corner == 1) {
        mirrorAlongAchsis('x');
    } else if (corner == 2) {
        mirrorAlongAchsis('y');
    } else if (corner == 3) {
        mirrorAlongAchsis('x');
        mirrorAlongAchsis('y');
    } else if (corner == 4) {
        mirrorAlongAchsis('z');
    } else if (corner == 5) {
        mirrorAlongAchsis('x');
        mirrorAlongAchsis('z');
    } else if (corner == 6) {
        mirrorAlongAchsis('y');
        mirrorAlongAchsis('z');
    } else if (corner == 7) {
        mirrorAlongAchsis('x');
        mirrorAlongAchsis('y');
        mirrorAlongAchsis('z');
    }

}


void drawCube(int x0, int y0, int z0, int x1, int y1, int z1, bool mode) {

    for (int x=x0; x<x1+1; x++) {
        for (int y=y0; y<y1+1; y++) {
            for (int z=z0; z<z1+1; z++) {
                setLed(x, y, z, mode);
            }
        }
    }

}


void cubeOff() {

    for (int layer=0; layer<8; layer++) {
        for (int pixel=0; pixel<64; pixel++) {
            cubeData[layer][pixel] = false;
        }
    }

}


void cubeOn() {

    for (int layer=0; layer<8; layer++) {
        for (int pixel=0; pixel<64; pixel++) {
            cubeData[layer][pixel] = true;
        }
    }

}


void execute(double executionTime) {

    double startExecution = time_time();

    while ((time_time() - startExecution) < executionTime) {

        for (int iteration=0; iteration<8; iteration++) {
            gpioWrite(SHCP, 0);

            for (int pixel=0; pixel<64; pixel++) {
                gpioWrite(STCP, 0);
                writePin(DS, cubeData[iteration][pixel]);
                gpioWrite(STCP, 1);
            }

            for (int layer=0; layer<8; layer++) {
                gpioWrite(STCP, 0);

                if (iteration == layer) {
                    gpioWrite(DS, 1);
                } else {
                    gpioWrite(DS, 0);
                }

                gpioWrite(STCP, 1);
            }

            gpioWrite(SHCP, 1);

        }

    }

}


void animation_01(int runTime) {

    double executionTime = 0.04;
    double startExecution = time_time();

    while ((time_time() - startExecution) < runTime) {

        if (rand() % 3 == 0) {
            if (rand() % 2 == 0) {

                for (int x=0; x<8; x++) {
                    cubeOff();

                    for (int y=0; y<8; y++) {
                        for (int z=0; z<8; z++) {
                            setLed(x, y, z, true);
                        }
                    }
                    execute(executionTime);
                }
            } else {

                for (int x=7; x>=0; x--) {
                    cubeOff();

                    for (int y=0; y<8; y++) {
                        for (int z=0; z<8; z++) {
                            setLed(x, y, z, true);
                        }
                    }
                    execute(executionTime);
                }

            }
        } else if (rand() % 3 == 1) {
            if (rand() % 2 == 0) {

                for (int y=0; y<8; y++) {
                    cubeOff();

                    for (int x=0; x<8; x++) {
                        for (int z=0; z<8; z++) {
                            setLed(x, y, z, true);
                        }
                    }
                    execute(executionTime);
                }
            } else {

                for (int y=7; y>=0; y--) {
                    cubeOff();

                    for (int x=0; x<8; x++) {
                        for (int z=0; z<8; z++) {
                            setLed(x, y, z, true);
                        }
                    }
                    execute(executionTime);
                }

            }
        } else if (rand() % 3 == 2) {
            if (rand() % 2 == 0) {

                for (int z=0; z<8; z++) {
                    cubeOff();

                    for (int x=0; x<8; x++) {
                        for (int y=0; y<8; y++) {
                            setLed(x, y, z, true);
                        }
                    }
                    execute(executionTime);
                }
            } else {

                for (int z=7; z>=0; z--) {
                    cubeOff();

                    for (int x=0; x<8; x++) {
                        for (int y=0; y<8; y++) {
                            setLed(x, y, z, true);
                        }
                    }
                    execute(executionTime);
                }

            }
        }

    }

}


void animation_02(int runTime) {

    double executionTime = 0.1;
    double startExecution = time_time();
    cubeOff();

    while ((time_time() - startExecution) < runTime) {

        for (int on=0; on<1; on++) {
            cubeData[rand() % 8][rand() % 64] = true;
        }
        for (int off=0; off<3; off++) {
            cubeData[rand() % 8][rand() % 64] = false;
        }

        execute(executionTime);

    }

}


void animation_03(int runTime) {

    double executionTime = 0.15;
    double startExecution = time_time();
    cubeOff();

    while ((time_time() - startExecution) < runTime) {

        for (int layer=1; layer<8; layer++) {
            for (int pixel=0; pixel<64; pixel++) {
                cubeData[layer-1][pixel] = cubeData[layer][pixel];
            }

        }

        for (int pixel=0; pixel<64; pixel++) {
            cubeData[7][pixel] = false;
        }

        for (int raindrops=0; raindrops<rand() % 7; raindrops++) {
            cubeData[7][rand() % 64] = true;
        }

        execute(executionTime);

    }

}


void animation_04(int runTime) {

    double executionTime = 0.035;
    double startExecution = time_time();
    cubeOff();

    for (int x=0; x<8; x++) {
        for (int y=0; y<8; y++) {
            setLed(x, y, (rand() % 2) * 7, true);
        }
    }

    execute(1.0);

    while ((time_time() - startExecution) < runTime) {

        int x = rand() % 8;
        int y = rand() % 8;

        if (cubeData[0][x + y * 8]) {
            for (int z=1; z<8; z++) {
                setLed(x, y, z-1, false);
                setLed(x, y, z, true);
                execute(executionTime);
            }
        } else if (cubeData[7][x + y * 8]) {
            for (int z=6; z>=0; z--) {
                setLed(x, y, z+1, false);
                setLed(x, y, z, true);
                execute(executionTime);
            }
        }

    }

}


void animation_05(int runTime) {

    double executionTime = 0.05;
    double startExecution = time_time();
    cubeOff();

    while ((time_time() - startExecution) < runTime) {

        int corner = rand() % 8;

        for (int size=7; size>=2; size--) {
            cubeOff();

            drawCube(0, 0, 0, size, size, size, true);

            drawCube(0, 1, 1, size, size-1, size-1, false);
            drawCube(1, 0, 1, size-1, size, size-1, false);
            drawCube(1, 1, 0, size-1, size-1, size, false);

            mirrorFromCorner(corner);
            execute(executionTime);

        }

        cubeOff();

        drawCube(0, 0, 0, 1, 1, 1, true);
        mirrorFromCorner(corner);
        execute(executionTime*2);

        for (int size=2; size<8; size++) {
            cubeOff();

            drawCube(0, 0, 0, size, size, size, true);

            drawCube(0, 1, 1, size, size-1, size-1, false);
            drawCube(1, 0, 1, size-1, size, size-1, false);
            drawCube(1, 1, 0, size-1, size-1, size, false);

            mirrorFromCorner(corner);
            execute(executionTime);

        }

    }

}


void animation_06(int runTime) {

    double executionTime = 0.0175;
    double startExecution = time_time();
    cubeOff();

    double originX = 3.5;
    double originY = 3.5;

    double a = 4;
    double b = 0.8;
    double c;
    double d = 4;

    while ((time_time() - startExecution) < runTime) {

        for (double i=0; i<(1/b)*2*3.14159; i=i+0.1) {
            cubeOff();

            for (int x=0; x<8; x++) {
                for (int y=0; y<8; y++) {
                    c = sqrt(pow((originX - x), 2) + pow((originY - y), 2));
                    int z = (int) a * sin(b * (c + i)) + d;
                    setLed(x, y, z, true);
                }
            }
            execute(executionTime);
        }

    }

}


void animation_07(int runTime) {

    double executionTime = 0.012;
    double startExecution = time_time();
    cubeOff();

    double originX;
    double originY;

    double a = 4;
    double b = 0.5;
    double c;
    double d = 4;

    while ((time_time() - startExecution) < runTime) {

        for (double circle=0; circle<2*3.14159; circle=circle+(1/b)*0.1*3.14159) {

            for (double i=0; i<(1/b)*2*3.14159; i=i+0.1) {
                cubeOff();

                originX = 7 * cos(circle + (i/20)) + 3.5;
                originY = 7 * sin(circle + (i/20)) + 3.5;

                for (int x=0; x<8; x++) {
                    for (int y=0; y<8; y++) {
                        c = sqrt(pow((originX - x), 2) + pow((originY - y), 2));
                        int z = (int) a * sin(b * (c + i)) + d;
                        setLed(x, y, z, true);
                    }
                }
                execute(executionTime);
            }

        }

    }

}


#define ct (time_time()-startLaunch)
#define et (time_time()-startExplosion)
bool apply(float px, float x, float py, float y, float pz, float z) {

    double executionTime = 0.04;

    if(!(((int)px) == (int)x && ((int)py) == (int)y && ((int)pz) == (int)z)) {
        printf("  z: %f,  pz: %f\n" , z, pz);
        setLed(px,py,pz, false);
        setLed(x, y, z, true);
        execute(executionTime);
        return true;
    }
    return false;
}

void animation_08(int runTime) {

    double executionTime = 0.001;
    double startExecution = time_time();
    double startLaunch = time_time();
    cubeOff();

    float v_start = 10;
    float g = 13;

    //Arrays
    int amount = 25;

    float ax[amount];
    float ay[amount];
    float az[amount];

    float avx[amount];
    float avy[amount];
    float avz[amount];

    //main loop
    while ((time_time() - startExecution) < runTime) {

        startLaunch = time_time();

        cubeOff();

        //Start rocket
        int x_start = (rand() % 6) + 1;
        int y_start = (rand() % 6) + 1;
        int z_start = 0;

        float x = 1;
        float y = 1;
        float z = 0;

        while(z < 7) {

            z = v_start*ct - ct*ct/2;

            cubeOff();
            setLed(x_start, y_start, z, true);
            execute(executionTime);

        }
        cubeOff();

        //Start explosion
        //Init V
        for (int i = 0; i < amount; i++) {
            avx[i] = (rand() % 1000 - 500) / 1000.0;
            avy[i] = (rand() % 1000 - 500) / 1000.0;
            avz[i] = (rand() % 1000 - 500) / 1000.0;
            float vlength = sqrt(avx[i]*avx[i] + avy[i]*avy[i]+avz[i]*avz[i]);
            avx[i] = avx[i]/vlength;
            avy[i] = avy[i]/vlength;
            avz[i] = avz[i]/vlength;
        }

        float s = 4 + (rand()%4);

        double startExplosion = time_time();

        z_start = z;

        //Update coordinates
        while(et < 2) {
            cubeOff();
            for(int i = 0; i < amount; i++) {

                ax[i] = x_start + avx[i]*s*et + g*0*ct*et;
                ay[i] = y_start + avy[i]*s*et + g*0*ct*ct;
                az[i] = z_start + avz[i]*s*et - 0.5*g*et*et;
                if( az[i] > 7 ) az[i] = 7;

                setLed(ax[i], ay[i], az[i], true);
                execute(executionTime);
            printf("et: %f \n",et);
            }
        }

        printf("Hallo");

    }

}


void runAllAnimations(int runTime) {

    double startExecution = time_time();

    while ((time_time() - startExecution) < runTime) {
        animation_01(10);
        animation_02(10);
        animation_03(10);
        animation_04(10);
        animation_05(10);
        animation_06(10);
        animation_07(10);
        animation_08(10);
    }

}

void off() {

    cubeOff();
    execute(0.01);

}


int main(int argc, char *argv[]) {

    srand(time(NULL));

    if (gpioInitialise() < 0) {
       return 1;
    }

    gpioSetMode(DS, PI_OUTPUT);
    gpioSetMode(STCP, PI_OUTPUT);
    gpioSetMode(SHCP, PI_OUTPUT);

    char arg1 = argv[1][0];
    char arg2 = argv[1][1];

    if (arg1 == '0' && arg2 == '0') {
        runAllAnimations(3600);
        off();
    } else if (arg1 == '0' && arg2 == '1') {
        animation_01(3600);
        off();
    } else if (arg1 == '0' && arg2 == '2') {
        animation_02(3600);
        off();
    } else if (arg1 == '0' && arg2 == '3') {
        animation_03(3600);
        off();
    } else if (arg1 == '0' && arg2 == '4') {
        animation_04(3600);
        off();
    } else if (arg1 == '0' && arg2 == '5') {
        animation_05(3600);
        off();
    } else if (arg1 == '0' && arg2 == '6') {
        animation_06(3600);
        off();
    } else if (arg1 == '0' && arg2 == '7') {
        animation_07(3600);
        off();
    } else if (arg1 == '0' && arg2 == '8') {
        animation_08(3600);
        off();
    }

    else if (arg1 == '0' && arg2 == 'o') {
        off();
    }

}

