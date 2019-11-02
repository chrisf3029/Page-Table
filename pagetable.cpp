#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "pagetable.h"
#include "swapfile.h"
#include "process.h"
#include "global.h"


/*******************************************************************************
 * GLOBALS (you will use this in the translate() function)
 */

/*
 * Current page table register, used by the processor to find the running
 * processes page table. The scheduler will update this when it dispatches
 * a new process, so you may assume it always points to the page table for
 * the /current/ process. A page table is an array of pte_t.
 */

pte_t* ptbr; /* Page Table Base Register on the CPU */

/*******************************************************************************
 * Number of entries in a page table
 */

const uint16_t CPU_NUM_PTE = ((1<<16) / page_size); /* do NOT modify */

/*******************************************************************************
 * Number of frames in memory 
 */

const uint16_t CPU_NUM_FRAMES = (mem_size / page_size); /* do NOT modify */


/*******************************************************************************
 * TODO: Part 1
 * Implement the three functions below: get_vpn(), get_offset(), and create_paddr()
 *
 * HINT: You have access to the global variable page_size, which tells you how
 *       many addresses a single page contains. (Do NOT change the value of
 *       page_size, only read this variable.)
 *
 * HINT: While page_size will be a power of two, allowing you to use bit-wise
 *       arithmetic, consider using modulus division and integer division if
 *       that is more intuitive for you. (In other words, there are two ways to
 *       do the math; use whichever one you want.)
 */


/*******************************************************************************
 * Get the page number from a virtual address.
 * @param addr The virtual address.
 * @return     The virtual page number (VPN) for this addres.
 */

vpn_t get_vpn(vaddr_t virt_addr) { 
  vpn_t vpn = virt_addr / page_size;
  return vpn;
}


/*******************************************************************************
 * Get the offset for a particular address.
 * @param addr The virtual address.
 * @return     The offset into a page for the given virtual address.
 */

offset_t get_offset(vaddr_t virt_addr) {
  offset_t offset = virt_addr % page_size;
  return offset;
}


/*******************************************************************************
 * Create a physical address from a PFN and an offset.
 * @param The physical frame number (PFN).
 * @param The offset into the page.
 * @return The physical address.
 */

paddr_t create_paddr(pfn_t pfn, offset_t offset) {
  paddr_t addr = pfn * page_size + offset;
  return addr;
}


/*******************************************************************************
 * Looks up an address in the current page table and returns the translated
 * physical address. If the entry for the given page is not valid, traps to
 * the OS (simulated by calling the pagefault_handler function).
 *
 * @param virt_addr The virtual address to translate.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical address we are accessing.
 */

paddr_t translate(vaddr_t virt_addr, int write) {
  vpn_t vpn;
  offset_t offset;
  pfn_t pfn;
  paddr_t phys_addr;

  /* Sanity Check */
  assert(ptbr != NULL); /* do NOT modify */

  vpn = get_vpn(virt_addr);
  offset = get_offset(virt_addr);
  printf("Virtual Address %5.5hu (VPN: %5.5hu OFFSET: %5.5hu)\n",
         virt_addr, vpn, offset); /* do NOT modify this output statement */

  /* TODO: Part 2 
   * (replace this comment block with your code)
   * Determine the PFN corresponding to the passed in VPN.
   * Perform the lookup using the PTBR,
   *   which holds the address of the current process' page table.
   * Finally, construct the physical address from the PFN and offset.
   */
   
  if(ptbr[vpn].valid == 1){
    pfn = ptbr[vpn].pfn;
  }
  else
    pfn = pagefault_handler(vpn, 1);
   
  phys_addr = create_paddr(pfn, offset);
  printf("Physical Address %5.5hu (PFN: %5.5hu OFFSET: %5.5hu)\n",
         phys_addr, pfn, offset); /* do NOT modify this output statement */
  return phys_addr;
}
