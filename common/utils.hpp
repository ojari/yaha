#pragma once

extern void download(const char *url, const char *cache_file);
extern long get_epoch();
extern bool print_epoch(long epoch);
extern long convert2epoch(const char *time_str);