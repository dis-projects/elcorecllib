// Copyright 2019 RnD Center "ELVEES", JSC

#include <stdint.h>
#include <malloc.h>


#include "dma.h"

#define DMA_CSR_NEXT_CHAIN (1 << 2)  // Chain isn't last
#define DMA_CSR_RUN (1 << 0)  // Start channel
#define DMA_CSR_DONE (1 << 1)  // Done channel
#define DMA_CSR_IRQM (1 << 6)  // Enable interrupts
#define DMA_CSR_CHAINLOAD (1 << 3)  // Enable dmachain

#define DMA_CH_STOP (DMA_CSR_RUN | DMA_CSR_DONE)  // Stop channel condition
#define DMA_CH_STOP_MASK 0x10F

static int32_t LogBurstSize(uint64_t xy, uint64_t mm, int32_t ofsext, int32_t ofsint) {
    uint64_t sum = xy | mm | ofsext | ofsint;
    if (sum & 1) return 0;
    if (sum & 2) return 1;
    if (sum & 3) return 2;
    if (sum & 4) return 3;
    return 4;
}

int DmaInit(volatile E50_DmaChan *ch, uint64_t xy, uint64_t mm, int32_t ofsext, int32_t ofsint,
            int len, int cfg) {
    int32_t burst = LogBurstSize(xy, mm, ofsext, ofsint);
    ch->Mask = 0;
    ch->Run = 0;
    while (ch->Run)
        continue;
    ch->Done = 0;
    while (ch->Done)
        continue;
    ch->Irq = 1;
    ch->AdrExt = (uint64_t)mm;
    ch->AdrInt = (uint64_t)xy;
    ch->OfsExt = ofsext;
    ch->OfsInt = ofsint;
    ch->RowsCols = len / (1 << burst) -1;
    if (ch->RowsCols >= UINT16_MAX)
        return 1;
    ch->AdrTaskH = 0;
    ch->AdrTaskL = 0;
    ch->Cfg = cfg | (burst << 11);
    ch->Csr = 0;
    return 0;
}

void DmaRun(volatile E50_DmaChan* ch) {
    uint32_t tmp_val = DMA_CSR_RUN;
    if (ch->AdrTaskL || ch->AdrTaskH)
        tmp_val = DMA_CSR_CHAINLOAD;

    // cache flush
    asm volatile("cchinv 0xFFFC, 0");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");

    ch->Csr = tmp_val;
}

void DmaWait(volatile E50_DmaChan* ch) {
    if (ch->Run) {
        ch->Mask = 0;
        while ((ch->Csr & DMA_CH_STOP_MASK) != DMA_CH_STOP)
            continue;
        ch->Irq = 1;
    }
}

E50_DmaChain::E50_DmaChain(size_t ntasks) : ntasks(ntasks) {
    tasks = (E50_DmaTask *) memalign(64, sizeof(E50_DmaTask) * ntasks);
}

int E50_DmaChain::AddDmaChain(size_t pos, uint64_t xy, uint64_t mm, int32_t ofsext,
                              int32_t ofsint, int len, int cfg) {
    int32_t burst = LogBurstSize(xy, mm, ofsext, ofsint);

    if (pos >= ntasks)
        return 1;

    tasks[pos].AdrExt = (uint64_t)mm;
    tasks[pos].AdrInt = (uint64_t)xy;
    tasks[pos].OfsExt = ofsext;
    tasks[pos].OfsInt = ofsint;
    tasks[pos].RowsCols = len / (1 << burst) - 1;
    if (tasks[pos].RowsCols >= UINT16_MAX)
        return 1;
    tasks[pos].Cfg = cfg | (burst << 11);
    tasks[pos].Csr = DMA_CSR_RUN | DMA_CSR_IRQM;

    if (pos < ntasks - 1) {
        tasks[pos].AdrTask = (uint64_t)&tasks[pos + 1];
        tasks[pos].Csr |= DMA_CSR_NEXT_CHAIN;
    }
    return 0;
}

void E50_DmaChain::SetDmaChainForChannel(volatile E50_DmaChan* ch) {
    ch->Mask = 0;
    ch->Run = 0;
    while (ch->Run)
        continue;
    ch->Done = 0;
    while (ch->Done)
        continue;
    ch->Irq = 1;
    ch->AdrTaskL = (uint32_t)tasks;
    ch->AdrTaskH = 0;
}
