#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mmu.h"

#define RESIDENTSETSIZE 24
#define PAGE_SIZE 4096
#define MAX_PROCESSES 4

extern char *base;
extern int idproc;
extern int systemframetablesize;
extern int processpagetablesize;

extern struct SYSTEMFRAMETABLE *systemframetable;
extern struct PROCESSPAGETABLE processpagetable[];

int get_lru_page();
int get_physical_frame();
bool page_has_virtual_frame_assigned(int page);
int get_virtual_frame();
int get_free_frame(int start, int end);
void swap_in_file(int frame);
void write_swap();
void read_swap();

/**
 * If there's available space in the physical memory, assigns the page to the physical memory.
 * If there's space in the the virtual memory, assigns the page to the virtual memory and swaps
 * it with the LRU page from the physical memory.
 */
int pagefault(char *vaddress) {
  int frame;
  int process_page = (int) vaddress >> 12;
  int frames_per_process = systemframetablesize / MAX_PROCESSES;

  // Each process can have 2 frames asisgned in the main memory at most (frames_per_process)
  // First check if there's available space in the main memory
  if (countframesassigned() < frames_per_process) {
    frame = get_physical_frame();
    if (frame == NINGUNO) {
      return NINGUNO;
    }
    // Write the frame in the swap file
    char *address = systemframetable[frame].paddress;
    write_swap(address, frame);
    processpagetable[process_page].framenumber = frame;
  } else {
    // Check if the page is already in the virtual space, if not assign it to some frame
    // and swap it to the main memory
    if (!page_has_virtual_frame_assigned(process_page)) {
      frame = get_virtual_frame();
      if (frame == NINGUNO) {
        return NINGUNO;
      }
      processpagetable[process_page].framenumber = frame;
    }
    swap_in_file(process_page);
  }

  processpagetable[process_page].presente = 1;

  return 1;
}

/**
 * Returns the least recently used page in the processpagetable that is in the main memory.
 * This page is the one with the smallest tlastaccess
 */
int get_lru_page() {
  int lru_page_index = 0;
  for (int i = 1; i < processpagetablesize; i++) {
    bool smaller_last_access = processpagetable[i].tlastaccess < processpagetable[lru_page_index].tlastaccess;
    if (smaller_last_access && processpagetable[i].presente) {
      lru_page_index = i;
    }
  }
  return lru_page_index;
}

/**
 * Returns and assigns an unassigned frame in the physical area
 */
int get_physical_frame() {
  return get_free_frame(0, systemframetablesize);
}

/**
 * True if a page is assigned to a virtual frame, this is the last 8 frames in
 * the swap memory
 */
bool page_has_virtual_frame_assigned(int page) {
  int frame = processpagetable[page].framenumber;
  int min_frame = systemframetablesize;
  int max_frame = systemframetablesize * 2;
  return min_frame <= frame && frame < max_frame;
}

/**
 * Returns and assigns an unassigned frame in the virtual area
 */
int get_virtual_frame() {
  return get_free_frame(systemframetablesize, systemframetablesize * 2);
}

/**
 * Returns and assigns an unassigned fram in the range [start, end)
 */
int get_free_frame(int start, int end) {
  for (int i = start; i < end; i++) {
    if (!systemframetable[i].assigned) {
      systemframetable[i].assigned = 1;
      return i;
    }
  }
  return NINGUNO;
}

/**
 * Swaps a page from the virtual memory with the LRU page from the phisical memory
 * and saves it in the swap file.
 */
void swap_in_file(int page_in) {
  // Choose a page to replace (the least used page)
  int page_out = get_lru_page();

  int frame_out = processpagetable[page_out].framenumber;
  int frame_in = processpagetable[page_in].framenumber;

  // Save the page if it has been modified directly from the systemtable
  if (processpagetable[page_out].modificado == 1) {
    char *address = systemframetable[frame_out].paddress;
    write_swap(address, frame_out);
  }

  // Swap the physical frame with the virtual frame
  char frame_in_content[PAGE_SIZE];
  char frame_out_content[PAGE_SIZE];

  read_swap(frame_in_content, frame_in);
  read_swap(frame_out_content, frame_out);

  write_swap(frame_in_content, frame_out);
  write_swap(frame_out_content, frame_in);

  // Updates the page table
  processpagetable[page_in].framenumber = frame_out;
  processpagetable[page_in].presente = 1;
  processpagetable[page_in].modificado = 0;

  processpagetable[page_out].framenumber= frame_in;
  processpagetable[page_out].presente = 0;
  processpagetable[page_out].modificado = 0;
}

/**
 * Reads a page from the swap file and saves it in the provided buffer
 */
void read_swap(char* buffer, int frame) {
  FILE *file = fopen("swap", "rb");
  fseek(file, frame * PAGE_SIZE, SEEK_SET);
  fread(buffer, 1, PAGE_SIZE, file);
  fclose(file);
}

/**
 * Writes a page in the given buffer to the swap file
 */
void write_swap(char* buffer, int frame) {
  FILE *file = fopen("swap", "wb");
  fseek(file, frame * PAGE_SIZE, SEEK_SET);
  fwrite(buffer, 1, PAGE_SIZE, file);
  fclose(file);
}
