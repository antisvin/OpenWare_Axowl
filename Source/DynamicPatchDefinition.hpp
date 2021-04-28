#ifndef __DynamicPatchDefinition_hpp__
#define __DynamicPatchDefinition_hpp__

#include "PatchDefinition.hpp"
#include "ProgramHeader.h"
#include "Storage.h"

class DynamicPatchDefinition : public PatchDefinition {
private:
  typedef void (*ProgramFunction)(void);
  ProgramFunction programFunction;
  uint32_t* linkAddress;
  uint32_t* jumpAddress;
  uint32_t programSize;
  char programName[24];
  Resource* sourceResource = NULL;
  void* sourceAddress = NULL;
  
  bool load(ProgramHeader* header, uint32_t sz){
    linkAddress = header->linkAddress;
    programSize = (uint32_t)header->endAddress - (uint32_t)header->linkAddress;
    if(sz != programSize)
      return false;
    stackBase = header->stackBegin;
    stackSize = (uint32_t)header->stackEnd - (uint32_t)header->stackBegin;
    jumpAddress = header->jumpAddress;
    programVector = header->programVector;
    strlcpy(programName, header->programName, sizeof(programName));
    programFunction = (ProgramFunction)jumpAddress;
    return true;
  }
  void copy(){
    if(sourceResource){
      if(sourceResource->isMemoryMapped()){
	/* copy program to ram */
	memcpy(linkAddress, sourceResource->getData(), programSize);
      }else{
	storage.readResource(sourceResource, linkAddress, programSize);
      }
    }else if(sourceAddress){
      memcpy(linkAddress, sourceAddress, programSize);
    }
  }
public:
  DynamicPatchDefinition() :
    PatchDefinition(programName, 2, 2) {}
  // called on program load from flash
  bool load(Resource* resource){
    sourceAddress = NULL;
    sourceResource = resource;
    if(resource->isMemoryMapped()){
      return load((ProgramHeader*)resource->getData(), resource->getDataSize());
    }else{
      ProgramHeader header;
      storage.readResource(resource, &header, sizeof(header));
      return load(&header, resource->getDataSize());
    }
    return false;
  }
  // called on program RUN from RAM
  bool load(void* address, uint32_t sz){
    sourceAddress = address;
    sourceResource = NULL;
    ProgramHeader* header = (ProgramHeader*)address;
    return load(header, sz);
  }
  bool verify(){
    // check we've got an entry function
    if(programFunction == NULL)
      return false;
    // check magic
    if((*(uint32_t*)linkAddress) != 0xDADAC0DE)
      return false;
    extern char _PATCHRAM, _PATCHRAM_SIZE;
    if((linkAddress == (uint32_t*)&_PATCHRAM && programSize <= (uint32_t)(&_PATCHRAM_SIZE)))
      return true;
#ifdef USE_PLUS_RAM
    extern char _PLUSRAM, _PLUSRAM_SIZE;
    if((linkAddress == (uint32_t*)&_PLUSRAM && programSize <= (uint32_t)(&_PLUSRAM_SIZE)))
      return true;
#endif
    return false;
  }
  void run(){
    copy();
    programFunction();
  }
  uint32_t getProgramSize(){
    return programSize;
  }
  uint32_t* getLinkAddress(){
    return linkAddress;
  }
};


#endif // __DynamicPatchDefinition_hpp__
