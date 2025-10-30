#include "../../main.h"

int initialized_log = 0;
char* dump_folder1 = "DUMP";
char* filename_logline = "logline.txt";
char cwd[1000] = {0};
#define ID_name_length 20
#define ID_array_size  20

struct stIDarray {
	char name[ID_name_length];
	int ID;
} IDarray[ID_array_size] = {0};
int IDarray_freeslot = 0;

//if label 0 - generates a timestamp
int log_generateID(char* label) {
	//tested
	if (IDarray_freeslot >= ID_array_size) {
		printf("IDarray full\n");
		return -1;
	}
	char b[80] = {0};
	if (label == 0) {
		format_datetime(&b, "%H-%M-%S_%d%m%Y");
		label = &b;
	}
	int sl = strlen(label);
	if (sl > ID_name_length) {
		//0 terminated
		sl = ID_name_length - 1;
	}
	memcpy(&IDarray[IDarray_freeslot].name, label, sl);
	IDarray[IDarray_freeslot].ID = (uint32_t)GetTickCount64();
	IDarray_freeslot++;
	// printf("name %s\n", IDarray[IDarray_freeslot - 1].name);
	return IDarray[IDarray_freeslot - 1].ID;
}
int _log_GetIndexbyID(int id) {
	for (int i = 0; i < ID_array_size; i++) {
		if (IDarray[i].ID == id) {
			return i;
		}
	}
	return -1;
}

void log_init() {
	if (initialized_log)
		return;
	make_directory(dump_folder1);
	getcwd(cwd, sizeof(cwd));
	initialized_log = 1;
}
void log_line(int id, char* format, ...) {
	//tested

#if defined(_DEBUG)
	int ind = _log_GetIndexbyID(id);
	if (ind == -1) {
		printf("cant find log id\n");
		return;
	}
	log_init();
	struct stIDarray* pa = &IDarray[ind];
	char buf[255] = {0};
	sprintf(&buf, "%s/%s", dump_folder1, pa->name);
	make_directory(&buf);
	sprintf(&buf, "%s/%s", &buf, filename_logline);

	FILE* pFile;
	if ((pFile = fopen(&buf, "a")) == NULL) {
		printf("The file dump_binary_name file was not opened\n");
	} else {
		va_list args;
		char buffer[256];
		va_start(args, format);
		vsprintf(buffer, format, args);
		fwrite(&buffer, strlen(&buffer), 1, pFile);
		va_end(args);
	}
	fclose(pFile);
#endif
}
void log_dump(int id, char* filename, int append, void* buffer, size_t size) {
	//tested
#if defined(_DEBUG)
	int ind = _log_GetIndexbyID(id);
	if (ind == -1) {
		printf("cant find log id\n");
		return;
	}
	log_init();
	struct stIDarray* pa = &IDarray[ind];
	char buf[255] = {0};
	sprintf(&buf, "%s/%s", dump_folder1, pa->name);
	make_directory(&buf);
	sprintf(&buf, "%s/%s", &buf, filename);

	char* fm;
	if (append)
		fm = "ab";
	else
		fm = "wb";
	FILE* pFile;
	if ((pFile = fopen(&buf, fm)) == NULL) {
		printf("The file dump_binary_name file was not opened\n");
	} else {
		fwrite(buffer, size, 1, pFile);
	}
	fclose(pFile);
#endif
}
