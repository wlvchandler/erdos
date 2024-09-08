unsigned long long getTotalSysMem() {
    long pages = sysconf(_SC_PHYS_PAGES);
    long sz_page = sysconf(_SC_PAGE_SIZE);
    return pages * sz_page;
}
