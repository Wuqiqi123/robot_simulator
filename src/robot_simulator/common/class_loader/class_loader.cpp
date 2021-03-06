// Copyright(c) 2020-2021, Qiqi Wu<1258552199@qq.com>.
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// This file is created by Qiqi Wu, 2021/4/20

#include "robot_simulator/common/class_loader/class_loader.h"



namespace rs::common::class_loader
{


ClassLoader::ClassLoader(std::string lib_path)
    : lib_path_(std::move(lib_path))
{

}


}