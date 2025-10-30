#pragma once
double* ExecuteInternal(char* query, uint32_t withResults);  //returns array indexes 0-n
void SetGlobalVar(const char* varName, const char* varValue);
char* GetGlobalVar_string(const char* varName);
double luaResToDouble(double* result, int index);
int64_t luaResToInteger(double* result, int index);
char* luaResToString(double* result, int index);
