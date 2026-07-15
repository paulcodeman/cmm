#include <stdio.h>
#include <sys/stat.h>

typedef struct _BIT_{
	unsigned int siz:8;
	unsigned int ofs:24;
}BIT;

struct _PROCINFO_
{
	char *buf;	// ¤аҐб вҐЄбв  Їа®жҐ¤гал
	void *classteg;	// ¤аҐб вҐЈ , Ј¤Ґ ®ЇаҐ¤Ґ«Ґ­  Їа®жҐ¤га 
	unsigned int warn:1;
	unsigned int speed:1;
	unsigned int lst:1;
	unsigned int typestring:2;
	unsigned int inlinest:1;
	unsigned int code32:1;
	unsigned int align:1;
	unsigned int acycle:1;
	unsigned int idasm:1;
	unsigned int opnum:1;
	unsigned int de:1;
	unsigned int ostring:1;
	unsigned int uselea:1;
	unsigned int regoverstack:1;
	unsigned int sizeacycle;
	char chip;
};

struct idrec
{
	union{
		struct idrec *left;
		struct localrec *next;
	};
	struct idrec *right;	//ЇаҐ¤л¤гй Ё б«Ґ¤гой § ЇЁбм
	char recid[IDLENGTH];	//Ё¬п
	unsigned int flag;
	char *newid;  //Ў«®Є б ¤ ­л¬Ё, ¤«п бвагЄвга  ¤аҐб вҐЈ ,¤«п Їа®жҐ¤га Ї а ¬Ґвал
	int rectok;		//вЁЇ
	int recrm;    //¤«п бвагЄвга зЁб«® Є®ЇЁ©
	int recsegm;
	int recpost;
	int recsize;
	int recsib;
	int line;	//­®¬Ґа «Ё­ЁЁ
	int file;	//д ©«
	int count;	//бзҐвзЁЄ ЁбЇ®«м§®ў ­Ёп
	unsigned short type;
	unsigned short npointr;
	union{
		char *sbuf;	//гЄ § вҐ«м ­  Ў«®Є Ёбе®¤­®Ј® вҐЄбв 
		_PROCINFO_ *pinfo;
	};
	union{
		long recnumber;
		long long reclnumber;
		double recdnumber;
		float recfnumber;
	};
};

struct localinfo
{
	int usedfirst;
	int usedlast;
	int start;
	int end;
	int level;
	int count;
};

struct localrec
{
/*	struct localrec *next;
	int localtok;
	unsigned short type;
	unsigned short npointr;
	union{
		unsigned int localnumber;
		idrec *rec;
	};
	int locsize;
	char localid[IDLENGTH];
	unsigned char fuse;	//д« Ј ЁбЇ®«м§®ў ­Ёп
	unsigned char flag;	//д« Ј static*/
	idrec rec;
	localinfo li;
	unsigned char fuse;	//д« Ј ЁбЇ®«м§®ў ­Ёп
};

#define INITBPPAR 1	//Ё­ЁжЁ «Ё§ жЁп BP Ї®б«Ґ Ї а ¬Ґва®ў
#define INITBPLOC 2 //Ё­ЁжЁ «Ё§ жЁп BP Ї®б«Ґ «®Є «м­ле
#define INITBPENTER 4
#define INITBPADDESP 8

struct HEADLOC
{
	int type;	//вЁЇ § Ј®«®ўЄ 
	unsigned int ofs; // ¤аҐб §­ зҐ­Ёп
	unsigned int num;	//ўҐ«ЁзЁ­  §­ зҐ­Ёп
};

struct treelocalrec
{
	treelocalrec *next;
	localrec *lrec;
	int initbp;
	int level;
	unsigned int addesp;
	int endline;
};

typedef struct _ITOK_
{
	int rm;
	int segm;
	int post;
	int sib;
	union{
		long number;
		long long lnumber;
		double dnumber;
		float fnumber;
	};
	union{
		int size;
		BIT bit;
	};
	unsigned short type;
	unsigned short npointr;
union{
 		idrec *rec;
		localrec *locrec;
	};
	char name[IDLENGTH];
	unsigned int flag;
}ITOK;

struct elementteg
{
	union{
		void *nteg;	// ¤аҐб вҐЈ  ў«®¦Ґ­­®© бвагЄвгал
		idrec *rec;
	};
	int tok;
	union{
		unsigned int numel;	//зЁб«® н«Ґ¬Ґ­в®ў нв®Ј® вЁЇ 
		BIT bit;
	};
	unsigned int ofs;	//б¬ҐйҐ­ЁҐ ®в ­ з «  бвагЄвгал
	char name[IDLENGTH];
};

struct structteg
{
	struct structteg *left;	//б«Ґ¤гойЁ© вҐЈ
	struct structteg *right;	//б«Ґ¤гойЁ© вҐЈ
	unsigned int size;	//а §¬Ґа вҐЈ 
	unsigned int numoper;	//зЁб«® ®ЇҐа ­¤®ў бвагЄвгал
	struct elementteg *baza;	// ¤аҐб б ®ЇЁб ­ЁҐ¬ н«Ґ¬Ґ­в®ў вҐЈ 
	unsigned int flag;
	char name[IDLENGTH];
};

struct listexport
{
	long address;
	char name[IDLENGTH];
};

typedef struct _IOFS_
{
	unsigned int ofs;
	unsigned int line;	//­®¬Ґа «Ё­ЁЁ
	unsigned int file;	//д ©«
	unsigned char dataseg;
}IOFS;

typedef struct _UNDEFOFF_
{
	struct _UNDEFOFF_ *next;
	IOFS *pos;	//ЎгдҐа б  ¤аҐб ¬Ё ®вЄг¤  ббл«ЄЁ
	int num;	//зЁб«® ббл«®Є ­  нвг ¬ҐвЄг
	char name[IDLENGTH];
}UNDEFOFF;

typedef struct _LISTCOM_
{
	char name[IDLENGTH];
}LISTCOM;

typedef struct _SINFO_
{
	char *bufstr;
	int size;
}SINFO;

//бвагЄвга  бЇЁбЄ  api-Їа®жҐ¤га
typedef struct _APIPROC_
{
	struct idrec *recapi;
}APIPROC;

//
typedef struct _DLLLIST_
{
	struct _DLLLIST_ *next;	//б«Ґ¤гой п DLL
	struct _APIPROC_ *list;	//бЇЁб®Є Їа®жҐ¤га
	unsigned short num;     //зЁб«® Їа®жҐ¤га
	char name[IDLENGTH];	//Ё¬п DLL
}DLLLIST;

typedef struct _PE_HEADER_
{
	long sign;	//бЁЈ­ вга  - ўбҐЈ¤   'PE'
	short cpu;    //¬Ё­ вЁЇ CPU - ўбҐЈ¤  0x14C
	short numobj;	//зЁб«® ўе®¤®ў ў в Ў«Ёжг ®ЎкҐЄв®ў
	long date_time;	//¤ в  ¬®¤ЁдЁЄ жЁЁ «Ё­ЄҐа®¬
	long pCOFF;
	long COFFsize;
	short NTheadsize;	//а §¬Ґа § Ј®«®ўЄ  PE ®в MAGIC - ўбҐЈ¤  0xE0
	short flags;
	short Magic;	//­ §­ зҐ­ЁҐ ЇаЈа ¬¬л
	short LinkVer;	//ўҐабЁп «Ё­ЄҐа 
	long sizecode;
	long sizeinitdata;
	long sizeuninitdata;
	long EntryRVA;	// ¤аҐб ®в­®бЁв IMAGE BASE Ї® Є®в®а®¬г ЇҐаҐ¤ Ґвбп гЇа ў«Ґ­ЁҐ
	long basecode;	//RVA бҐЄжЁп, Є®в®а п б®¤Ґа¦Ёв Їа®Ја ¬¬­л© Є®¤
	long basedata;	//RVA бҐЄжЁп,б®¤Ґа¦ й п ¤ ­­лҐ
	long ImageBase;	//ўЁавг «м­л© ­ з «м­л©  ¤аҐб § Јаг§ЄЁ Їа®Ја ¬¬л
	long objAlig;	//ўла ў­Ёў ­ЁҐ Їа®Ја ¬¬­ле бҐЄжЁ©
	long fileAlig;	//‚ла ў­Ёў ­ЁҐ бҐЄжЁ© ў д ©«Ґ
	long OSver;	//­®¬Ґа ўҐабЁЁ ®ЇҐа бЁбвҐ¬л ­Ґ®Ўе Їа®Ја ¬¬Ґ
	long userver;
	long SubSysVer;
	long rez;
	long imagesize;	//а §¬Ґа ў Ў ©в е § Јаг¦ Ґ¬®Ј® ®Ўа §  б § Ј®«®ўЄ ¬Ё ўла ў­Ґ­л©
	long headsize;	//а §¬ ўбҐе § Ј®«®ўЄ®ў stub+PE+objtabl
	long checksum;
	short SubSys;	//®ЇҐа жЁ®­­ п бЁбв ­Ґ®Ўе ¤«п § ЇгбЄ 
	short DLLflag;
	long stackRezSize;
	long stackComSize;
	long heapRezSize;
	long heapComSize;
	long loaderFlag;
	long numRVA;	//ўбҐЈ¤  10
	long exportRVA;
	long exportSize;
	long importRVA;
	long importSize;
	long resourRVA;
	long resourSize;
	long exceptRVA;
	long exceptSize;
	long securRVA;
	long securSize;
	long fixupRVA;
	long fixupSize;
	long debugRVA;
	long debugSize;
	long descripRVA;
	long descripSize;
	long machinRVA;
	long machinSize;
	long tlsRVA;
	long tlsSize;
	long loadConfRVA;
	long loadConfSize;
	long rez2[2];
	long iatRVA;
	long iatSize;
	long rez3[6];
}PE_HEADER;

typedef struct _OBJECT_ENTRY_
{
	char name[8];
	long vsize;
	long sectionRVA;
	long psize;
	long pOffset;
	unsigned long PointerToRelocations;
	unsigned long PointerToLinenumbers;
	unsigned short NumberOfRelocations;
	unsigned short NumberOfLinenumbers;
	long flags;
}OBJECT_ENTRY;

typedef struct _EXPORT_TABLE_
{
	unsigned long Flags;
	unsigned long Time;
	unsigned short Version[2];
	unsigned long NameRVA;
	unsigned long OriginalBase;
	unsigned long NumFunc;
	unsigned long NumName;
	unsigned long AddressRVA;
	unsigned long NamePRVA;
	unsigned long OrdinalRVA;
}EXPORT_TABLE;

struct ftime {
	unsigned ft_tsec:5;  /* ¤ўҐ бҐЄг­¤л */
	unsigned ft_min:6;   /* ¬Ё­гвл */
	unsigned ft_hour:5;  /* з бл */
	unsigned ft_day:5;   /* ¤Ґ­м */
	unsigned ft_month:4; /* ¬Ґбпж */
	unsigned ft_year:7;  /* Ј®¤-1980 */
};


typedef struct _STRING_LIST_
{
	void *next;	//б«Ґ¤гой п бвагЄвга 
	unsigned int len; //¤«Ё­  бва®ЄЁ
	unsigned int ofs;	// ¤аҐб ў ўле®¤­®¬ д ©«Ґ
	unsigned char type;	//вЁЇ вҐа¬Ё­ в®а 
	unsigned char plase;	//Ј¤Ґ бҐ©з б бва®Є  - post or data
}STRING_LIST;

struct FILEINFO
{
	char *filename;
	int numdline;
	idrec *stlist;
	union{
		struct ftime time;
		unsigned short lineidx[2];
	};
};

struct EWAR{
	FILE *file;
	char *name;
};

typedef struct _ICOMP_
{
	unsigned int type;
	unsigned int loc;
	unsigned int use_cxz;
}ICOMP;

typedef struct _RETLIST_
{
	unsigned int line;
	unsigned int loc;
	unsigned int type;
//	int use;
}RETLIST;

enum{
	singlcase,startmulti,endmulti};

typedef struct _ISW_
{
	unsigned char type;
	unsigned int postcase;
	unsigned long value;
}ISW;

struct postinfo
{
	unsigned int loc;
	unsigned int num;
	unsigned short type;
	unsigned short line;
	unsigned short file;

};

typedef struct _EXE_DOS_HEADER_
{
	unsigned short sign;
	unsigned short numlastbyte;
	unsigned short numpage;
	unsigned short numreloc;
	unsigned short headsize;
	unsigned short minmem;
	unsigned short maxmem;
	unsigned short initSS;
	unsigned short initSP;
	unsigned short checksum;
	unsigned short initIP;
	unsigned short initCS;
	unsigned short ofsreloc;
	unsigned short overlay;
	unsigned long  fullsize;
}EXE_DOS_HEADER;

typedef struct _FSWI_
{
	ISW *info;
	int sizetab;	//зЁб«® н«Ґ¬Ґ­в®ў
	int type;	//а §ап¤­®бвм
	int numcase;	//зЁб«® ЁбЇ®«м§гҐ¬ле н«Ґ¬Ґ­в®ў
	int defal;	//§­ зҐ­ЁҐ Ї® г¬®«з ­Ёо.
	int ptb;	// ¤аҐб гЄ § вҐ«п ­  нвг в Ў«Ёжг ў Ў«®ЄҐ Є®¤ 
	int ptv;	// ¤аҐб в ў«Ёжл ўҐ«ЁзЁ­
	int mode;	//вЁЇ switch
	int razr;	//а §ап¤­®бвм ўҐ«ЁзЁ­
}FSWI;

struct paraminfo
{
	unsigned int ofspar;
	unsigned char type[8];
};

struct MEOSheader
{
	unsigned char sign[8];
	unsigned long vers;
	unsigned long start;
	unsigned long size;
	unsigned long alloc_mem;
	unsigned long esp;
	unsigned long I_Param;
	unsigned long I_Icon;
};

#ifdef OPTVARCONST

struct LVIC{
	idrec *rec;
//	int blocks;
	int typevar;
	int contype;	//вЁЇ б®¤Ґа¦Ё¬®Ј®
	union{
		long number;
		long long lnumber;
		double dnumber;
		float fnumber;
	};
};

struct BLVIC
{
	int sizevic;
	LVIC *listvic;
};

#endif

#define SIZEIDREG 256
#define NOINREG 8
#define SKIPREG 9

struct REGEQVAR
{
	REGEQVAR *next;
	char name[IDLENGTH];
	unsigned char razr;
};

struct REGISTERSTAT
{
	union{
		REGEQVAR *next;
#ifdef OPTVARCONST
		BLVIC *bakvic;
#endif
	};
	union{
		char id[SIZEIDREG];
		void *stringpar;
		unsigned long number;
	};
	unsigned char type;
	unsigned char razr;
};

struct SAVEREG
{
	unsigned int size;	//а §¬Ґа Ї ¬пвЁ ¤«п аҐЈЁбва®ў
	unsigned char all;	//ўбҐ аҐЈЁбвал
	unsigned char reg[8];	//Є ав  аҐЈЁбва®ў
};

struct SAVEPAR
{
 unsigned char ooptimizespeed;
 unsigned char owarning;
 unsigned char odbg;
 unsigned char odosstring;
 unsigned char ouseinline;
 unsigned char oam32; 		      // аҐ¦Ё¬ 32 ЎЁв­®©  ¤аҐб жЁЁ
 unsigned char oalignword;
 unsigned char oAlignCycle;       //ўла ў­Ёў вм ­ з «  жЁЄ«®ў
 unsigned char oidasm;	// ббҐ¬Ў«Ґа­лҐ Ё­бвагЄжЁЁ бзЁв вм Ё¤Ґ­вЁдЁЄ в®а ¬Ё
 int ooptnumber;
 int odivexpand;
 unsigned char ooptstr;	//®ЇвЁ¬Ё§ жЁп бва®Є®ўле Є®­бв ­в
 unsigned char ochip;
 int           oaligncycle;
 unsigned char ouselea;
 unsigned char oregoverstack;
};

struct COM_MOD
{
	COM_MOD *next;
	unsigned char *input; 	 /* dynamic input buffer */
	unsigned int endinptr;		 /* end index of input array */
	unsigned int inptr; 		 /* index in input buffer */
	unsigned int inptr2; 		 /* index in input buffer */
	unsigned int linenumber;
	unsigned int currentfileinfo;
	int numparamdef;	//зЁб«® Ї а ¬Ґва®ў ў вҐЄгйҐ¬ define
	char *declareparamdef;	//бЇЁб®Є ®Ўкпў«Ґ­ле Ї а ¬Ґва®ў define
	char *paramdef;	//бЇЁб®Є ­®ўле Ї а ¬Ґва®ў
	int freze;	//д« Ј § ЇаҐйҐ­Ёп г¤ «Ґ­Ёп бвагЄвгал
};

struct LISTRELOC {
	unsigned int val;
};

struct LISTFLOAT
{
	union{
		float fnum;
		double dnum;
		unsigned long num[2];
	};
	int type;
	unsigned int ofs;
};

struct LILV
{
	unsigned int ofs;
	int size;
	localrec *rec;
};

struct WARNACT
{
	void (*fwarn)(char *str,unsigned int line,unsigned int file);
	unsigned char usewarn;
};
