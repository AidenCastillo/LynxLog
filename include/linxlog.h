#ifndef LINXLOG_H
#define LINXLOG_H

void init_linxlog();
void add_file(const char *filename);
void commit(const char* message);
void show_log();
void status();


#endif // LINXLOG_H
