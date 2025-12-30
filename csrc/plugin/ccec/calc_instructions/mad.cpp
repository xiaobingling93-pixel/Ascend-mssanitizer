/* -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2025 Huawei Technologies Co.,Ltd.
 *
 * MindStudio is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * ------------------------------------------------------------------------- */


#include "plugin/record_calc_instructions.h"

using namespace Sanitizer;
#if !(defined(__CCE_IS_AICORE__) && __CCE_IS_AICORE__ == 1 && defined(__DAV_C220_VEC__))
// #1530
// #1533
SANITIZER_REPORT(mad, __cc__ float *c, __ca__ __bf16 *a, __cb__ __bf16 *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 16, 16);
}

// #1531
// #1534
SANITIZER_REPORT(mad, __cc__ float *c, __ca__ __bf16 *a, __cb__ __bf16 *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 16, 16);
}

// #1532
// #1535
SANITIZER_REPORT(mad, __cc__ float *dst, __ca__ __bf16 *src0, __cb__ __bf16 *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool kDirectionAlign, bool isWeightOffset,
    bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 16, 16);
}

// #1536
// #1539
SANITIZER_REPORT(mad, __cc__ half *c, __ca__ half *a, __cb__ half *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 16, 16, 16, 512, 512, 512);
}

// #1537
SANITIZER_REPORT(mad, __cc__ half *c, __ca__ half *a, __cb__ half *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 16, 16, 16, 512, 512, 512);
}

// #1538
SANITIZER_REPORT(mad, __cc__ half *dst, __ca__ half *src0, __cb__ half *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset, bool isSparsity,
    bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 16, 16, 16, 512, 512, 512);
}

// #1540
SANITIZER_REPORT(mad, __cc__ half *c, __ca__ half *a, __cb__ half *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool isWeightOffset, bool ctrlMatrixC, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, 0, ctrlMatrixC, initMatrixC, 16, 16, 16, 512,
        512, 512);
}

// #1541
SANITIZER_REPORT(mad, __cc__ float *c, __ca__ half *a, __cb__ half *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 16, 16);
}

// #1542
SANITIZER_REPORT(mad, __cc__ float *c, __ca__ half *a, __cb__ half *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 16, 16);
}

// #1544
SANITIZER_REPORT(mad, __cc__ float *c, __ca__ half *a, __cb__ half *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 16, 16, 1024, 512, 512);
}

#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
// #1545
SANITIZER_REPORT(mad, __cc__ float *dst, __ca__ half *src0, __cb__ half *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool kDirectionAlign, bool isWeightOffset,
    bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 16, 16);
}
#else
// #1543
SANITIZER_REPORT(mad, __cc__ float *dst, __ca__ half *src0, __cb__ half *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset, bool isSparsity,
    bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 32, 16, 16);
}
#endif

// #1546
// #1549
SANITIZER_REPORT(mad, __cc__ float *c, __ca__ float *a, __cb__ float *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 32, 32, 512, 512, 512);
}

// #1547
// #1550
SANITIZER_REPORT(mad, __cc__ float *c, __ca__ float *a, __cb__ float *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 32, 32, 512, 512, 512);
}

// #1548
// #1551
SANITIZER_REPORT(mad, __cc__ float *dst, __ca__ float *src0, __cb__ float *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool kDirectionAlign, bool isWeightOffset,
    bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 32, 32, 512, 512, 512);
}

// #1552
// #1554
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ int16_t *a, __cb__ int8_t *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 16, 8);
}

// #1553
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ int16_t *a, __cb__ int8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t smaskOffset, unit_flag_t unitFlagMode, bool isWeightOffset, bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 16, 8);
}

// #1555
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ int16_t *a, __cb__ int8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool rightShift, bool ctrlMatrixB, bool isWeightOffset, bool ctrlMatrixC, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, 0, ctrlMatrixC, initMatrixC, 32, 16, 8);
}


// #1556
// #1561
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ int8_t *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 8, 8);
}

// #1557
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ int8_t *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 8, 8);
}

// #1559
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ int8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 8, 8, 1024, 512, 512);
}

#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
// #1560
SANITIZER_REPORT(mad, __cc__ int32_t *dst, __ca__ int8_t *src0, __cb__ int8_t *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool kDirectionAlign, bool isWeightOffset,
    bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 8, 8);
}
#else
// #1558
SANITIZER_REPORT(mad, __cc__ int32_t *dst, __ca__ int8_t *src0, __cb__ int8_t *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset, bool isSparsity,
    bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 32, 8, 8);
}
#endif

// #1562
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ int8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool isWeightOffset, bool ctrlMatrixC, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, 0, ctrlMatrixC, initMatrixC, 32, 8, 8);
}
// #1563
// #1566
SANITIZER_REPORT(mad, __cc__ uint32_t *c, __ca__ uint8_t *a, __cb__ uint8_t *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 8, 8);
}

// #1564
SANITIZER_REPORT(mad, __cc__ uint32_t *c, __ca__ uint8_t *a, __cb__ uint8_t *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 8, 8);
}

// #1565
SANITIZER_REPORT(mad, __cc__ uint32_t *dst, __ca__ uint8_t *src0, __cb__ uint8_t *src1, uint16_t m, uint16_t k,
    uint16_t n, uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset,
    bool isSparsity, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 32, 8, 8);
}

// #1567
SANITIZER_REPORT(mad, __cc__ uint32_t *c, __ca__ uint8_t *a, __cb__ uint8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool isWeightOffset, bool ctrlMatrixC, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, 0, ctrlMatrixC, initMatrixC, 32, 8, 8);
}

// #1568
// #1573
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ uint8_t *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 8, 8);
}

// #1569
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ uint8_t *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 8, 8);
}

// #1571
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ uint8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 8, 8, 1024, 512, 512);
}
#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
// #1572
SANITIZER_REPORT(mad, __cc__ int32_t *dst, __ca__ uint8_t *src0, __cb__ uint8_t *src1, uint16_t m, uint16_t k,
    uint16_t n, uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool kDirectionAlign, bool isWeightOffset,
    bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 8, 8);
}
#else
// #1570
SANITIZER_REPORT(mad, __cc__ int32_t *dst, __ca__ uint8_t *src0, __cb__ uint8_t *src1, uint16_t m, uint16_t k,
    uint16_t n, uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset,
    bool isSparsity, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 32, 8, 8);
}
#endif
// #1574
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ uint8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool isWeightOffset, bool ctrlMatrixC, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, 0, ctrlMatrixC, initMatrixC, 32, 8, 8);
}

// #1575
// #1578
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ int8_t *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 8, 8);
}

// #1576
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ int8_t *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 8, 8);
}

// #1577
SANITIZER_REPORT(mad, __cc__ int32_t *dst, __ca__ uint8_t *src0, __cb__ int8_t *src1, uint16_t m, uint16_t k,
    uint16_t n, uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset,
    bool isSparsity, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 32, 8, 8);
}

// #1579
SANITIZER_REPORT(mad, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ int8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool isWeightOffset, bool ctrlMatrixC, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, 0, ctrlMatrixC, initMatrixC, 32, 8, 8);
}

// #1580
SANITIZER_REPORT(mad_b8u2, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ void *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 8, 2, 1024, 512, 128);
}

// #1581
SANITIZER_REPORT(mad_b8u2, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ void *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 8, 2, 1024, 512, 128);
}

// #1582
SANITIZER_REPORT(mad_b8u2, __cc__ int32_t *dst, __ca__ int8_t *src0, __cb__ void *src1, uint16_t m, uint16_t k,
    uint16_t n, uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset,
    bool isSparsity, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 32, 8, 2, 1024, 512, 128);
}

// #1583
SANITIZER_REPORT(mad_b8u2, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ void *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 8, 2, 1024, 512, 128);
}

// #1584
SANITIZER_REPORT(mad_b8u2, __cc__ int32_t *c, __ca__ uint8_t *a, __cb__ void *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 8, 2, 1024, 512, 128);
}

// #1585
SANITIZER_REPORT(mad_b8u2, __cc__ int32_t *dst, __ca__ uint8_t *src0, __cb__ void *src1, uint16_t m, uint16_t k,
    uint16_t n, uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset,
    bool isSparsity, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 32, 8, 2, 1024, 512, 128);
}

// #1586
SANITIZER_REPORT(mad_f16u2, __cc__ half *c, __ca__ half *a, __cb__ void *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 16, 16, 2, 512, 512, 64);
}

// #1587
SANITIZER_REPORT(mad_f16u2, __cc__ half *c, __ca__ half *a, __cb__ void *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 16, 16, 2, 512, 512, 64);
}

// #1588
SANITIZER_REPORT(mad_f16u2, __cc__ half *dst, __ca__ half *src0, __cb__ void *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset, bool isSparsity,
    bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 16, 16, 2, 512, 512, 64);
}

// #1589
// #1594
SANITIZER_REPORT(mad_s4, __cc__ int32_t *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 4, 4);
}

// #1590
SANITIZER_REPORT(mad_s4, __cc__ int32_t *c, __ca__ void *a, __cb__ void *b, uint16_t m, uint16_t k, uint16_t n,
    bool init_val_controlC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, 0, init_val_controlC, 32, 4, 4);
}

#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
// #1592
SANITIZER_REPORT(mad_s4, __cc__ int32_t *c, __ca__ void *a, __cb__ void *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 4, 4, 1024, 512, 512);
}
#else
// #1595
SANITIZER_REPORT(mad_s4, __cc__ int32_t *c, __ca__ void *a, __cb__ void *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool isWeightOffset, bool ctrlMatrixC, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, 0, ctrlMatrixC, initMatrixC, 32, 4, 4);
}
#endif

#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
// #1593
SANITIZER_REPORT(mad_s4, __cc__ int32_t *dst, __ca__ void *src0, __cb__ void *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool kDirectionAlign, bool isWeightOffset,
    bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 4, 4);
}
#else
// #1591
SANITIZER_REPORT(mad_s4, __cc__ int32_t *dst, __ca__ void *src0, __cb__ void *src1, uint16_t m, uint16_t k, uint16_t n,
    uint8_t featOffset, uint8_t smaskOffset, bool ctrlMatrixA, bool ctrlMatrixB, bool isWeightOffset, bool isSparsity,
    bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, 0, 0, 0, initMatrixC, 32, 4, 4);
}
#endif

// #1596
SANITIZER_REPORT(mad_s8s4, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ void *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 8, 4);
}

// #1597
SANITIZER_REPORT(mad_s8s4, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ void *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool isWeightOffset, bool ctrlMatrixC, bool initMatrixC)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, 0, ctrlMatrixC, initMatrixC, 32, 8, 4);
}

// #1598
SANITIZER_REPORT(mad_sp, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ int8_t *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 8, 8);
}

// #1599
SANITIZER_REPORT(mad_sp, __cc__ int32_t *c, __ca__ int8_t *a, __cb__ int8_t *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlagMode, bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, 0, 0, cmatrixSource, cmatrixInitVal, 32, 8, 8);
}

// #1600
SANITIZER_REPORT(mad_tf322f32, __cc__ float *c, __ca__ float *a, __cb__ float *b, uint64_t config)
{
    RecordMatrixOpConfigFunc(EXTRA_PARAMS, c, a, b, config, 32, 32, 32);
}
// #1601
SANITIZER_REPORT(mad_tf322f32, __cc__ float *c, __ca__ float *a, __cb__ float *b, uint16_t m, uint16_t k, uint16_t n,
    uint8_t unitFlag, bool kDirectionAlign, bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, c, a, b, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 32, 32);
}

// #1602
SANITIZER_REPORT(mad_tf322f32, __cc__ float *dst, __ca__ float *src0, __cb__ float *src1, uint16_t m, uint16_t k,
    uint16_t n, uint8_t featOffset, uint8_t smaskOffset, uint8_t unitFlag, bool kDirectionAlign, bool isWeightOffset,
    bool cmatrixSource, bool cmatrixInitVal)
{
    RecordMatrixOpFunc(EXTRA_PARAMS, dst, src0, src1, m, k, n, unitFlag, kDirectionAlign, cmatrixSource, cmatrixInitVal,
        32, 32, 32);
}

#endif