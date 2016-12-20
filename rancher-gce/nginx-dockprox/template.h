/*
FILE
	template.h
PURPOSE
NOTES
AUTHOR/LEGAL
	(C) 2006-2017 Gary Wallis.
	GPLv3 License Applies
*/



//Template system items
typedef struct t_template {
        char const *cpName[100];//pointers to var_name strings
        char const *cpValue[100];//pointers to substitution strings
} t_template ;

void Template(char *cTemplate, struct t_template *template, FILE *fp);

//Globals
extern void AppFunctions(FILE *fp,char *cFunction);
