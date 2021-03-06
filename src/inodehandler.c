#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/t2fs.h"
#include "../include/inodehandler.h"
#include "../include/blockhandler.h"

//TODO documentação

bool getInodeToBeingWorkedInode(int inodeIndex) {
    int upperDiskBlocks = controller->boot.superblockSize
                        + controller->boot.freeBlocksBitmapSize
                        + controller->boot.freeInodeBitmapSize;
    int blockToRead = upperDiskBlocks + (int) inodeIndex/(controller->boot.blockSize*SECTOR_SIZE);
    if(blockToRead > upperDiskBlocks + controller->boot.inodeAreaSize)
        return ERROR;

    readBlockToBeingWorkedBlock(blockToRead);
    int inodeStructSize = 32;
    unsigned char buffer[inodeStructSize];

    memcpy(buffer, &beingWorkedBlock[inodeIndex*inodeStructSize], inodeStructSize);
    beingWorkedInode = (struct t2fs_inode*) buffer;

    return SUCCESS;
}

bool printInodeContentFromBeingWorkedInode() {
    int firstDataPointer = beingWorkedInode->dataPtr[0];
    int secondDataPointer = beingWorkedInode->dataPtr[1];
    int singleIndirectDataPointer = beingWorkedInode->singleIndPtr;
    int doubleIndirectDataPointer = beingWorkedInode->doubleIndPtr;

    struct t2fs_record* records;

    if(firstDataPointer != INVALID_PTR) {
        printf("Has first data pointer, printing... \n");
        records = inodeDataPointerToRecords(firstDataPointer);
        printRecords(records);
    }

    if(secondDataPointer != INVALID_PTR) {
        printf("Has first data pointer, printing... \n");
        records = inodeDataPointerToRecords(secondDataPointer);
        printRecords(records);
    }

    if(singleIndirectDataPointer != INVALID_PTR) {
        printf("Has singleIndirectDataPointer \n");
    }

    if(doubleIndirectDataPointer != INVALID_PTR) {
        printf("Has doubleIndirectDataPointer \n");
    }



    return SUCCESS;
}

struct t2fs_record* inodeDataPointerToRecords(DWORD dataPtr) {
    readBlockToAuxiliaryWorkingBlock(dataPtr);

    int recordSize = sizeof(struct t2fs_record);
    unsigned char buffer[recordSize];

    struct t2fs_record* records = malloc(sizeof(struct t2fs_record) * 16);

    int index;
    for(index = 0; index < 16; index++) { //1024(blockSize) divides by 64 (recordSize)
        memcpy(buffer, &auxiliaryWorkingBlock[index*recordSize], recordSize);
        beingWorkedRecord = (struct t2fs_record*) buffer;
        records[index] = *beingWorkedRecord;
    }
    return records;
}

struct t2fs_record* inodeDataPointerGetFirstRecord(DWORD dataPtr) {
    readBlockToAuxiliaryWorkingBlock(dataPtr);

    int recordSize = sizeof(struct t2fs_record);
    unsigned char buffer[recordSize];

    struct t2fs_record* record = malloc(sizeof(struct t2fs_record));

    memcpy(buffer, &auxiliaryWorkingBlock, recordSize);
    record = (struct t2fs_record*) buffer;

    return record;
}

bool inodeAppendRecordToBufferBlock(DWORD dataPtr, struct t2fs_record* newRecord, unsigned char bufferBlock[]) {
    readBlockToAuxiliaryWorkingBlock(dataPtr);

    int recordSize = sizeof(struct t2fs_record);
    unsigned char buffer[recordSize];

    struct t2fs_record* bufferRecord;

    int index;
    for(index = 0; index < 16; index++) { //1024(blockSize) divides by 64 (recordSize)
        memcpy(buffer, &auxiliaryWorkingBlock[index*recordSize], recordSize);
        bufferRecord = (struct t2fs_record*) buffer;
        if(bufferRecord->TypeVal == TYPEVAL_INVALIDO) {
            memcpy(&bufferBlock[index*recordSize], newRecord, recordSize);
            return SUCCESS;
        }
    }
    return ERROR;
}

bool prepareNewRecordsBlockBuffer(unsigned char bufferBlock[]) {
    int recordSize = sizeof(struct t2fs_record);

    struct t2fs_record* bufferRecord = malloc(sizeof(struct t2fs_record));

    bufferRecord->TypeVal = TYPEVAL_INVALIDO;
    strncpy(bufferRecord->name, "\0",1);
    bufferRecord->inodeNumber = 0;

    int index;
    for(index = 0; index < 16; index++) { //1024(blockSize) divides by 64 (recordSize)
        memcpy(&bufferBlock[index*recordSize], bufferRecord, recordSize);
    }

    free(bufferRecord);
    return SUCCESS;
}

bool inodeRemoveRecordAndReturnBufferBlock(DWORD dataPtr, char* name, unsigned char bufferBlock[]) {
    readBlockToAuxiliaryWorkingBlock(dataPtr);

    int recordSize = sizeof(struct t2fs_record);
    unsigned char buffer[recordSize];

    struct t2fs_record* bufferRecord;

    int index;
    for(index = 0; index < 16; index++) { //1024(blockSize) divides by 64 (recordSize)
        memcpy(buffer, &auxiliaryWorkingBlock[index*recordSize], recordSize);
        bufferRecord = (struct t2fs_record*) buffer;
        if(!strcmp(bufferRecord->name, name)) {
            bufferRecord->TypeVal = TYPEVAL_INVALIDO;
            memcpy(&bufferBlock[index*recordSize], bufferRecord, recordSize);
            return SUCCESS;
        }
    }
    return ERROR;
}

bool inodeFillRecordsToBufferBlock(DWORD dataPtr, unsigned char bufferBlock[]) {
    readBlockToAuxiliaryWorkingBlock(dataPtr);

    int recordSize = sizeof(struct t2fs_record);
    unsigned char buffer[recordSize];

    struct t2fs_record* bufferRecord;

    int index;
    for(index = 0; index < 16; index++) { //1024(blockSize) divides by 64 (recordSize)
        memcpy(buffer, &auxiliaryWorkingBlock[index*recordSize], recordSize);
        bufferRecord = (struct t2fs_record*) buffer;
        memcpy(&bufferBlock[index*recordSize], bufferRecord, recordSize);
        }
    return SUCCESS;
}
