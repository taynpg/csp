#ifndef BOX_CAL_DATA_H_
#define BOX_CAL_DATA_H_

// 分配节气时间差值
bool AllocJieQi(int** pData);
// 释放节气差值内存
void FreeIntList(int* pData);
// 分配农历表
bool AllocLunar(int** pData);

#endif