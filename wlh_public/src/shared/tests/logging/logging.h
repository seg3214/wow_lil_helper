
extern int log_generateID(char* label);
extern void log_line(int id, char* format, ...);
extern void log_dump(int id, char* filename, int append, void* buffer, size_t size);
