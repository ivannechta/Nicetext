#define TOKEN 257
#define PUNCT 258
#define WEIGHT 259
#define BAD 260
typedef union {
	char *string;
	unsigned long  number;
} YYSTYPE;
extern YYSTYPE yylval;
