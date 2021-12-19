#include <stdio.h>
#include <stdlib.h>
#define MAX_PAGE 256
#define PAGE_SIZE 256

/*
We will use a linked list to represent page table data structure.
The following is an node for one page-frame relationship.
*/
typedef struct PageFrameEntry{
	int page;
	int frame;
	struct PageFrameEntry* next;
}pageFrameEntry;

pageFrameEntry* pageTable;
int frameOccupancy[MAX_PAGE];
char* physicalMemory;
int pageFault;
int findPage(int);
int loadPage(pageFrameEntry*, int);
int getFirstEmptyFrame();
int convertAddress(int);

/*This function is used for showing some middle result to debug, not used anymore.
void showPageTable(){
	pageFrameEntry* currentEntry = pageTable;
	while(currentEntry){
		printf("currentEntry : page %d, frame: %d, next: %p\n", currentEntry->page, currentEntry->frame, currentEntry->next);
		currentEntry = currentEntry->next;
	}
}*/

int main(int argc, char *argv[]){
	physicalMemory = (char*)malloc(MAX_PAGE*PAGE_SIZE);
	FILE* addresses = fopen("addresses.txt", "r");
	int address;
	int physicalAddress;
	FILE* result = fopen("result.txt","w");
	for(int i = 0; i < 1000; i++){
		//showPageTable(); Show some middle result to debug, not used anymore.
		fscanf(addresses, "%d", &address);
		//printf("fscanf() works.%d\n",address); Show some middle result to debug, not used anymore.
		physicalAddress = convertAddress(address);
		fprintf(result, "Virtual address: %d Physical address: %d Value: %d\n", address, physicalAddress, (char)physicalMemory[physicalAddress]);
	}
	fprintf(result, "Page faults: %d\n", pageFault);
	return 0;
}

int convertAddress(int logicalAddress){
	/*returns physicalAddress*/
	int page = logicalAddress>>8;//Page is the high 8 digit, taking it
	int physicalMemoryOffset = findPage(page)<<8;//Put the frame number on high 8 digit
	return (logicalAddress&0b11111111)/*Taking the low 8 digits*/ + physicalMemoryOffset;
}

int findPage(int page){
	/*
	Go through pageTable to see if the required page has a frame.
	If so, return the frame; else call loadPage and return the frame of loaded page.
	Return the frame number.
	*/
	if(pageTable == NULL)
		return loadPage(pageTable, page);
	
	pageFrameEntry* currentEntry = pageTable;
	do{
		if(currentEntry->page == page)
			return currentEntry->frame;
		else
		if(currentEntry->next == NULL)
			return loadPage(currentEntry, page);
		else
			currentEntry = currentEntry->next;
	}while(1);
}

int loadPage(pageFrameEntry *lastEntry, int page){
	/*
	Will need lastEntry to attach the new pageFrameEntry to its next.
	*/
	pageFrameEntry* newEntry = (pageFrameEntry*)malloc(sizeof(pageFrameEntry));
	newEntry->next = NULL;//Initialize the memory. If not doing so, system will give some memory with garbage content causing bug.
	if(pageTable == NULL)
		pageTable = newEntry;
	else
		lastEntry->next = newEntry;//Attach the new entry to the last entry of pageTable.
	newEntry->page = page;
	int newFrame = getFirstEmptyFrame();
	newEntry->frame = newFrame;
	frameOccupancy[newFrame] = 1;
	int physicalMemoryOffset = newFrame*PAGE_SIZE;
	long readingOffset = page*PAGE_SIZE;
	
	/*Start to read content into physicalMemory, to location newFrame.*/
	FILE* fp = fopen("BACKING_STORE.bin","r");
	fseek(fp, readingOffset, SEEK_SET);
	for(int i = 0; i < PAGE_SIZE; i++){
		physicalMemory[physicalMemoryOffset + i] = fgetc(fp);
	}
	fclose(fp);
	
	pageFault++;
	return newFrame;
}

int getFirstEmptyFrame(){
	for(int i = 0; i < MAX_PAGE; i++){
		if(frameOccupancy[i] == 0)
			return i;
	}
	return -1;
}