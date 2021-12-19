# PageSwapProgram
 A simple implemetation for memory page loading and swaping

This is a term project for course operating system. It implement a simple memory page loading function.

### Requirement
The requirement provides a randomly generated binary file, as the target page file to be loaded into memory. It requires to read 1000 random addresses, and fetch the content in the page file. It has to be done by searching in the "physical memory" that is simulated in this program.

The file will be divided into 256\*256 pages, and so does the "physical" memory. When reading an address, it must be searched in the "physical memory". To do this, a page table must be maintained, and the address must be converted into the form of physical page number + offset.

### Result
The result of this program is exactly same with the result that the instructor of this course provide. Hence, this project can be said completely successful.
