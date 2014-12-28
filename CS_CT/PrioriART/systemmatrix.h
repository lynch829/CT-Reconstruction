/*
* Implementation of Sparse Matrix specialized for tomography system matrix
*/

#pragma once
#include <map>
#include <cassert>
#include "ctimage.h"
#include "CTsystem.h"

using namespace std;

typedef map<int, double> ROW;

struct SystemInfo;

