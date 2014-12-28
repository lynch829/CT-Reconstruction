/* Test cases 
 * Huayu Zhang, Dec 2014 */

#pragma once
#include "CTsystem.h"

/* Demo1 : a 300mm * 300mm cube, reconstruction matrix size: 8*8 
 * 10 detectors, 8 angles 
 * projection file: data/cube300mm.prj */
void demo1();

/* Demo2 : a disk(r=250mm), reconstruction matrix size: 64 * 64
 * 36 detectors, 36 angles
 * projection file: data/cylinder.prj */ 
void demo2();

/* Demo3 : a disk O(-200,-200), r=70, a rectangle O(0,200),(150,75)
 * a square O(200,-200) (100,100)
 * reconstruction matrix size: 256 * 256
 * 400 detectors, 216 angles
 * projection file: data/csrbigdata.prj */
void demo3();