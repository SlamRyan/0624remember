#ifndef BASEADDR_H
#define BASEADDR_H

#include <string>

struct InsUpgrad   //aaa //用于保存升级过程中的状态和相关信息
{
    uint32_t inUpdate;//32
    uint32_t baseAddr;
    uint32_t totalPage;
    uint32_t currentPage;
    uint32_t pageSize;
    uint32_t upgradStatus;


};

#endif // BASEADDR_H
