// Copyright 2019-2021 RnD Center "ELVEES", JSC

#ifndef _DMA_H_
#define _DMA_H_

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    union {
        int64_t AdrExt;
        int32_t AdrExtL;
    };
    union {
        int64_t AdrInt;
        int32_t AdrIntL;
    };
    int32_t OfsExt;
    int32_t OfsInt;
    int32_t RowsCols;
    int32_t Cfg;
    union {
        int64_t AdrTask;
        int32_t AdrTaskL;
    };
    int32_t Csr;
    int32_t _alignment;
} E50_DmaTask;

typedef struct {
    union {
        int64_t AdrExt;
        int32_t AdrExtL;
    };
    union {
        int64_t AdrInt;
        int32_t AdrIntL;
    };
    int32_t OfsExt;
    int32_t OfsInt;
    int32_t RowsCols;
    int32_t Run;
    int32_t Done;
    int32_t Init;
    int32_t Irq;
    int32_t Mask;
    int32_t IrqT;
    int32_t Cfg;
    int32_t Csr;        // run done (load&run loadonly request) irq irqm busy actv
    int32_t AdrTaskL;
    int32_t AdrTaskH;
} E50_DmaChan;

typedef struct E50_DmaChain {
    size_t ntasks;
    E50_DmaTask *tasks;

    E50_DmaChain(size_t ntasks);

    ~E50_DmaChain() {
        // free(tasks);
    }

    int AddDmaChain(size_t pos, uint64_t xy, uint64_t mm, int32_t ofsext, int32_t ofsint,
                    int len, int cfg);
    void SetDmaChainForChannel(volatile E50_DmaChan* ch);
} E50_DmaChain;

#ifdef BUILD_FOR_MCOM03
#define E50_DMA_REGS 0x2823800
#define E50_DMA_CH0R 0x2823000
#define E50_DMA_CH1R 0x2823080
#define E50_DMA_CH2R 0x2823100
#define E50_DMA_CH3R 0x2823180
#else
#define E50_DMA_REGS 0x10223800
#define E50_DMA_CH0R 0x10223000
#define E50_DMA_CH1R 0x10223080
#define E50_DMA_CH2R 0x10223100
#define E50_DMA_CH3R 0x10223180
#endif

#define DmaCh0 ((E50_DmaChan*)E50_DMA_CH0R)
#define DmaCh1 ((E50_DmaChan*)E50_DMA_CH1R)
#define DmaCh2 ((E50_DmaChan*)E50_DMA_CH2R)
#define DmaCh3 ((E50_DmaChan*)E50_DMA_CH3R)

#define DMA_CFG_RD ((0 << 0) | (3 << 3) | (3 << 7) | (2 << 23) | (2 << 26))
#define DMA_CFG_WR ((1 << 0) | (3 << 3) | (3 << 7) | (2 << 23) | (2 << 26))

int DmaInit(volatile E50_DmaChan* ch, uint64_t xy, uint64_t mm, int32_t ofsext, int32_t ofsint,
            int len, int cfg);
void DmaRun(volatile E50_DmaChan* ch);
void DmaWait(volatile E50_DmaChan* ch);

#endif
