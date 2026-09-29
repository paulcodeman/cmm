/* Automatic short jumps (option -AJ).

   C-- has two spellings of every control statement: `if else while for
   return break continue goto switch` emit near jumps (rel32), their upper
   case twins `IF ELSE WHILE ...` emit short ones (rel8) and fail with
   "jump distance too large" when the target is out of range.  Picking the
   short form by hand for every statement is tedious, so with -AJ the
   compiler does it itself:

   the driver (main) runs the compiler again as a child with -AJPASS=<file>.
   In such a pass every keyword is read as its upper case twin
   (AutoJumpPromote), except the ones listed in <file>, which are read as the
   lower case one -- also when the source spells them in upper case, so an
   IF that does not fit no longer stops the build.  When a promoted
   statement then fails the distance check, the error is not reported: the
   statement is appended to <file> (AutoJumpFailed) and the pass exits with
   E_AUTOJUMP_RETRY.  The driver repeats until a pass needs no new demotion.
   Demoting only makes code longer, so the list only grows and the loop ends;
   the driver falls back to an ordinary compile if it does not.

   A statement is identified by (source file name, line, keyword).  Several
   equal keywords on one line are promoted or demoted together. */

#include "tok.h"

char *autojumpfile=NULL;	/* -AJPASS=<file>: this process is a pass */
int autojumpnew=0;	/* demotions added by this pass */

struct AJREC{
	char *file;
	unsigned int line;
	int tok;	/* lower case keyword token */
};

static AJREC *demoted=NULL;
static int numdemoted=0,maxdemoted=0;
static AJREC *promoted=NULL;
static int numpromoted=0,maxpromoted=0;

static const int ajtab[][2]={
	{tk_if,tk_IF},{tk_else,tk_ELSE},{tk_while,tk_WHILE},{tk_for,tk_FOR},
	{tk_return,tk_RETURN},{tk_break,tk_BREAK},{tk_continue,tk_CONTINUE},
	{tk_goto,tk_GOTO},{tk_switch,tk_SWITCH}
};
#define AJTABSIZE (int)(sizeof(ajtab)/sizeof(ajtab[0]))

static const char *CurFileName(unsigned int file)
{
	if(startfileinfo==NULL)return "";
	const char *n=(startfileinfo+file)->filename;
	return n?n:"";
}

static void AddRec(AJREC **list,int *num,int *max,const char *file,unsigned int line,int tok)
{
	for(int i=0;i<*num;i++){
		AJREC *r=*list+i;
		if(r->line==line&&r->tok==tok&&strcmp(r->file,file)==0)return;
	}
	if(*num==*max){
		*max=*max?*max*2:64;
		*list=(AJREC *)realloc(*list,*max*sizeof(AJREC));
		if(*list==NULL){
			puts("Not enough memory for -AJ");
			exit(e_outofmemory);
		}
	}
	(*list+*num)->file=strdup(file);
	(*list+*num)->line=line;
	(*list+*num)->tok=tok;
	(*num)++;
}

static int IsDemoted(const char *file,unsigned int line,int tok)
{
	for(int i=0;i<numdemoted;i++){
		AJREC *r=demoted+i;
		if(r->line==line&&r->tok==tok&&strcmp(r->file,file)==0)return TRUE;
	}
	return FALSE;
}

/* read the demotion list written by the previous passes */
void AutoJumpLoad()
{
	FILE *f=fopen(autojumpfile,"rt");
	char buf[1024];
	if(f==NULL)return;
	while(fgets(buf,sizeof(buf),f)){
		unsigned int line;
		int t;
		char *p;
		if(sscanf(buf,"%d %u",&t,&line)!=2)continue;
		p=strchr(buf,'\t');
		if(p==NULL)continue;
		p++;
		p[strcspn(p,"\r\n")]=0;
		AddRec(&demoted,&numdemoted,&maxdemoted,p,line,t);
	}
	fclose(f);
}

/* lexer hook: a lower case keyword token was just read */
int AutoJumpPromote(int tok)
{
	if(autojumpfile==NULL)return tok;
	for(int i=0;i<AJTABSIZE;i++){
		if(ajtab[i][0]!=tok&&ajtab[i][1]!=tok)continue;
		/* written in either case: short unless a previous pass found that it
		   does not fit (then an upper case IF quietly becomes if as well) */
		const char *file=CurFileName(currentfileinfo);
		if(IsDemoted(file,linenumber,ajtab[i][0]))return ajtab[i][0];
		AddRec(&promoted,&numpromoted,&maxpromoted,file,linenumber,ajtab[i][0]);
		return ajtab[i][1];
	}
	return tok;
}

/* A short jump of a statement of kind `tok` (lower case token) reported at
   `line` does not fit.  If the statement was promoted by this pass, demote it
   and return TRUE (the error is not reported); otherwise return FALSE.
   The line of an error can be a line or two after the keyword (it is taken
   after the keyword was scanned), so the nearest promotion above it counts. */
int AutoJumpFailed(int tok,unsigned int line,unsigned int file)
{
	AJREC *best=NULL;
	if(autojumpfile==NULL)return FALSE;
	const char *fname=CurFileName(file);
	for(int i=0;i<numpromoted;i++){
		AJREC *r=promoted+i;
		if(r->tok!=tok||r->line>line||r->line+2<line||strcmp(r->file,fname)!=0)continue;
		if(best==NULL||r->line>best->line)best=r;
	}
	if(best==NULL)return FALSE;
	if(!IsDemoted(best->file,best->line,tok)){
		FILE *f=fopen(autojumpfile,"at");
		if(f){
			fprintf(f,"%d %u\t%s\n",tok,best->line,best->file);
			fclose(f);
		}
		AddRec(&demoted,&numdemoted,&maxdemoted,best->file,best->line,tok);
		autojumpnew++;
	}
	return TRUE;
}

/* jumperror() receives the upper case spelling */
int AutoJumpFailedName(char *type,unsigned int line,unsigned int file)
{
	static const struct{const char *name; int tok;} names[]={
		{"IF",tk_if},{"ELSE",tk_else},{"WHILE",tk_while},{"FOR",tk_for},
		{"RETURN",tk_return},{"BREAK",tk_break},{"CONTINUE",tk_continue},
		{"GOTO",tk_goto},{"SWITCH",tk_switch}
	};
	for(unsigned int i=0;i<sizeof(names)/sizeof(names[0]);i++){
		if(strcmp(type,names[i].name)==0)return AutoJumpFailed(names[i].tok,line,file);
	}
	return FALSE;
}
