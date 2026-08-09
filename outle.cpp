#include "tok.h"

#include "le.h"

#define _OUTLE_

#define KEX_PARAM_SIZE 4096
#define KEX_PATH_SIZE 4096
#define KEX_MAX_ARGC 256
#define KEX_HEAP_RESERVE (64UL * 1024UL)

static unsigned int kexentry=0;
static unsigned int kexparam=0;
static unsigned int kexpath=0;
static unsigned int kexargv=0;
static unsigned int kexserviceend=0;

static idrec *FindKEXSymbol(idrec *root,const char *name)
{
	idrec *found;
	if(root==NULL)return NULL;
	if(strcmp(name,root->recid)==0)return root;
	found=FindKEXSymbol(root->left,name);
	if(found!=NULL)return found;
	return FindKEXSymbol(root->right,name);
}

static unsigned int KEXJump(int opcode)
{
	unsigned int loc;
	if(opcode!=0xE9){
		op(0x0F);
		op(opcode);
	}
	else op(opcode);
	loc=outptr;
	outdword(0);
	return loc;
}

static void PatchKEXJump(unsigned int loc,unsigned int target)
{
	int32_t value=(int32_t)target-(int32_t)(loc+4);
	memcpy(output+loc,&value,sizeof(value));
}

static void PatchKEXValue(unsigned int loc,unsigned int value)
{
	uint32_t patch=(uint32_t)value;
	memcpy(output+loc,&patch,sizeof(patch));
}

void AddKEXStartup()
{
	idrec *mainrec,*paramrec,*pathrec;
	unsigned int argv1,argv2,param1,path1,maincall;
	unsigned int skipws,skipone,copychar,inquote,openquote,closequote;
	unsigned int endarg,endall,done;
	unsigned int jskipspace,jskiptab,jempty,jmax,jquoted,jspace,jtab,jdouble,jsingle;
	unsigned int jinclose,jcopy1,jcopy2,jcopy3,jcopy4,jcopy5,jendall,jskipback;
	unsigned int postend;

	mainrec=FindKEXSymbol(treestart,"main");
	if(mainrec==NULL||mainrec->rectok!=tk_proc){
		preerror("KEX entry point 'main' is not found");
		return;
	}
	if(mainrec->newid==NULL||strcmp(mainrec->newid,"DD")!=0){
		preerror("KEX entry point must be main(dword argc, dword argv)");
		return;
	}
	if((mainrec->flag&f_typeproc)!=tp_pascal){
		preerror("KEX main must use the default pascal calling convention");
		return;
	}

	while(outptr&3)op(0);
	kexentry=outptr;

	/* Initialize the KolibriOS heap service before library startup. */
	op(0xB8); outdword(68);			/* mov eax,68 */
	op(0xBB); outdword(11);			/* mov ebx,11 */
	outword(0x40CD);				/* int 0x40 */

	op(0xBF); argv1=outptr; outdword(0);	/* mov edi,argv */
	op(0xB8); path1=outptr; outdword(0);	/* mov eax,path */
	op(0xAB);					/* stosd */
	op(0xBD); outdword(1);			/* mov ebp,1 */
	op(0xBE); param1=outptr; outdword(0);	/* mov esi,param */

	skipws=outptr;
	outword(0x068A);				/* mov al,[esi] */
	outword(0x203C);				/* cmp al,' ' */
	jskipspace=KEXJump(0x84);
	outword(0x093C);				/* cmp al,9 */
	jskiptab=KEXJump(0x84);
	outword(0xC084);				/* test al,al */
	jempty=KEXJump(0x84);
	outword(0xFD81); outdword(KEX_MAX_ARGC);	/* cmp ebp,max */
	jmax=KEXJump(0x83);			/* jae done */
	outword(0xF089);				/* mov eax,esi */
	op(0xAB);					/* stosd */
	op(0x45);					/* inc ebp */
	outword(0xF289);				/* mov edx,esi */
	outword(0xDB31);				/* xor ebx,ebx */

	copychar=outptr;
	op(0xAC);					/* lodsb */
	outword(0xC084);				/* test al,al */
	jendall=KEXJump(0x84);
	outword(0xDB84);				/* test bl,bl */
	jquoted=KEXJump(0x85);
	outword(0x203C);
	jspace=KEXJump(0x84);
	outword(0x093C);
	jtab=KEXJump(0x84);
	outword(0x223C);
	jdouble=KEXJump(0x84);
	outword(0x273C);
	jsingle=KEXJump(0x84);
	outword(0x0288);				/* mov [edx],al */
	op(0x42);					/* inc edx */
	jcopy1=KEXJump(0xE9);

	inquote=outptr;
	outword(0xD83A);				/* cmp al,bl */
	jinclose=KEXJump(0x84);
	outword(0x0288);
	op(0x42);
	jcopy2=KEXJump(0xE9);

	openquote=outptr;
	outword(0xC388);				/* mov bl,al */
	jcopy3=KEXJump(0xE9);

	closequote=outptr;
	outword(0xDB31);				/* xor ebx,ebx */
	jcopy4=KEXJump(0xE9);

	endarg=outptr;
	op(0xC6); op(0x02); op(0);		/* mov byte [edx],0 */
	jcopy5=KEXJump(0xE9);

	endall=outptr;
	op(0xC6); op(0x02); op(0);
	done=outptr;
	outword(0xC031);				/* xor eax,eax */
	op(0xAB);					/* argv[argc]=0 */
	op(0x55);					/* push ebp (argc) */
	op(0x68); argv2=outptr; outdword(0);	/* push argv */
	op(0xE8); maincall=outptr; outdword(0);
	op(0xB8); outdword(0xFFFFFFFF);
	outword(0x40CD);

	skipone=outptr;
	op(0x46);					/* inc esi */
	jskipback=KEXJump(0xE9);

	while(outptr&3)op(0);
	postend=outptr+postsize;
	paramrec=FindKEXSymbol(treestart,"param");
	pathrec=FindKEXSymbol(treestart,"program_path");
	if(paramrec!=NULL&&paramrec->recpost&&paramrec->recsize>=KEX_PARAM_SIZE)
		kexparam=outptr+paramrec->recnumber;
	else{
		kexparam=(postend+3)&~3;
		postend=kexparam+KEX_PARAM_SIZE;
	}
	if(pathrec!=NULL&&pathrec->recpost&&pathrec->recsize>=KEX_PATH_SIZE)
		kexpath=outptr+pathrec->recnumber;
	else{
		kexpath=(postend+3)&~3;
		postend=kexpath+KEX_PATH_SIZE;
	}
	if(kexparam+KEX_PARAM_SIZE>postend)postend=kexparam+KEX_PARAM_SIZE;
	if(kexpath+KEX_PATH_SIZE>postend)postend=kexpath+KEX_PATH_SIZE;
	kexargv=(postend+3)&~3;
	kexserviceend=kexargv+(KEX_MAX_ARGC+1)*4;

	PatchKEXValue(argv1,kexargv);
	PatchKEXValue(argv2,kexargv);
	PatchKEXValue(param1,kexparam);
	PatchKEXValue(path1,kexpath);
	PatchKEXJump(maincall,(unsigned int)mainrec->recnumber);
	PatchKEXJump(jskipspace,skipone);
	PatchKEXJump(jskiptab,skipone);
	PatchKEXJump(jskipback,skipws);
	PatchKEXJump(jempty,done);
	PatchKEXJump(jmax,done);
	PatchKEXJump(jquoted,inquote);
	PatchKEXJump(jspace,endarg);
	PatchKEXJump(jtab,endarg);
	PatchKEXJump(jdouble,openquote);
	PatchKEXJump(jsingle,openquote);
	PatchKEXJump(jinclose,closequote);
	PatchKEXJump(jcopy1,copychar);
	PatchKEXJump(jcopy2,copychar);
	PatchKEXJump(jcopy3,copychar);
	PatchKEXJump(jcopy4,copychar);
	PatchKEXJump(jcopy5,skipws);
	PatchKEXJump(jendall,endall);
}

unsigned short swapshort(unsigned int var)

{

unsigned short h,l;

	l=var>>8;

	h=var<<8;

	return l|h;

}

int MakeBin32()

{

	hout=CreateOutPut(outext,"wb");

	if(fwrite(output,outptr,1,hout)!=1){

		ErrWrite();

		fclose(hout);

		hout=NULL;

		return(-1);

	}

	return 0;

}

unsigned int EntryParamStr()

{

ITOK btok;

int bb=tk_id;

unsigned char *buf=(unsigned char *)"__bufcomstr";

	btok.number=0;

	searchtree(&btok,&bb,buf);

	if(bb==tk_id)btok.number=0;

	else{

		btok.number+=outptrdata;

		if((outptrdata%2)==1)btok.number++;	/* alignment of entire post data block manditory */

	}

	return btok.number;

}

int MakeMEOS()

{

MEOSheader hdr;

	hout=CreateOutPut(outext,"wb");

	strcpy((char *)hdr.sign,"MENUET01");

	hdr.vers=1;

	hdr.start=EntryPoint();

	hdr.size=outptr;

	hdr.alloc_mem=Align(outptr+postsize+stacksize,16);

	hdr.esp=hdr.alloc_mem-4;

	hdr.I_Param=EntryParamStr();

	hdr.I_Path=0;

	memcpy(output+startptr,&hdr,sizeof(MEOSheader));

	if(fwrite(output,outptr,1,hout)!=1){

		ErrWrite();

		fclose(hout);

		hout=NULL;

		return(-1);

	}

	return 0;

}

int MakeKEX()

{

MEOSheader hdr;
unsigned long hardmin,allocmem;

	hardmin=kexserviceend+stacksize;
	if(hardmin<kexserviceend){
		preerror("KEX memory size overflow");
		return -1;
	}
	if(kexmem){
		if(kexmem>0xFFFFF000UL)allocmem=0xFFFFF000UL;
		else allocmem=(kexmem+4095)&~4095UL;
		if(allocmem<hardmin){
			preerror("KEX -MEM value is too small for image, BSS and stack");
			return -1;
		}
	}
	else{
		if(hardmin>0xFFFFFFFFUL-KEX_HEAP_RESERVE-4095){
			preerror("KEX automatic memory size overflow");
			return -1;
		}
		allocmem=(hardmin+KEX_HEAP_RESERVE+4095)&~4095UL;
	}

	memset(&hdr,0,sizeof(hdr));
	memcpy(hdr.sign,"MENUET01",8);
	hdr.vers=1;
	hdr.start=kexentry;
	hdr.size=outptr;
	hdr.alloc_mem=allocmem;
	hdr.esp=allocmem-4;
	hdr.I_Param=kexparam;
	hdr.I_Path=kexpath;
	memcpy(output,&hdr,sizeof(hdr));

	hout=CreateOutPut(outext,"wb");
	if(fwrite(output,outptr,1,hout)!=1){
		ErrWrite();
		fclose(hout);
		hout=NULL;
		return -1;
	}
	fclose(hout);
	hout=NULL;
	printf("KolibriOS memory: %lu bytes%s.\n",allocmem,kexmem?" (-MEM)":" (automatic)");
	return 0;

}

int MakeLE()

{

LE_Header hdr;

Object_Table ot[2];

Page_Map_Table *pmt;

unsigned int i;

int headerofs;

int headsize=sizeof(LE_Header);

unsigned long *fpto;

int sizeb;

	CreatStub(stubfile);

	memset(&hdr,0,sizeof(LE_Header));

#ifdef _WC_

	hdr.Signature='EL';

#else

	hdr.Signature='LE';

#endif

	hdr.CPU_Type=i80386;

	hdr.Target_OS=1;

	hdr.Type_Flags=0x200;

	hdr.Number_Of_Memory_Pages=outptr/0x1000+1;

	hdr.Initial_CS=1;

	hdr.Initial_EI=EntryPoint();

	hdr.Initial_SS=2;

	hdr.Initial_ESP=stacksize;

	hdr.Memory_Page_Size=0x1000;

	hdr.Bytes_On_Last_Page=outptr%0x1000;

	hdr.Object_Table_Offset=sizeof(LE_Header);

	hdr.Object_Table_Entries=2;

	headerofs=ftell(hout);

	if(fwrite(&hdr,sizeof(LE_Header),1,hout)!=1){

errwrite:

		ErrWrite();

		fclose(hout);

		hout=NULL;

		return(-1);

	}

	ot[0].Virtual_Segment_Size=outptr+postsize;//hdr.Initial_ESP;

	ot[0].Relocation_Base_Address=0x10000;

	ot[0].ObjTableFlags=0x2045;

	ot[0].Page_MAP_Index=1;

	ot[0].Page_MAP_Entries=hdr.Number_Of_Memory_Pages;

	ot[1].Virtual_Segment_Size=hdr.Initial_ESP;

	ot[1].Relocation_Base_Address=Align(outptr+postsize,0x10000)+0x10000;

	ot[1].ObjTableFlags=0x2043;

	ot[1].Page_MAP_Index=2;

	ot[0].Reserved=ot[1].Reserved=ot[1].Page_MAP_Entries=0;

	if(fwrite(&ot,sizeof(Object_Table)*2,1,hout)!=1)goto errwrite;

	hdr.Object_Page_Map_Table_Offset=headsize+=sizeof(Object_Table)*2;

	sizeb=sizeof(Page_Map_Table)*hdr.Number_Of_Memory_Pages;

	pmt=(Page_Map_Table *)MALLOC(sizeb);

	for(i=0;i<hdr.Number_Of_Memory_Pages;i++){

		(pmt+i)->High_Page_Number=swapshort(((i+1)/256));

		(pmt+i)->Low_Page_Number=(unsigned char)((i+1)%256);

		(pmt+i)->FLAGS=0;

	}

	if(fwrite(pmt,sizeb,1,hout)!=1)goto errwrite;

	free(pmt);

	hdr.Resource_Table_Offset=hdr.Resident_Names_Table_Offset=headsize+=sizeb;

	i=strlen(rawfilename);

	string2[0]=(unsigned char)i;

	strcpy((char *)&string2[1],rawfilename);

	*(long *)&string2[i+2]=0;

	if(fwrite(string2,i+5,1,hout)!=1)goto errwrite;

	hdr.Entry_Table_Offset=headsize+=i+4;

	headsize++;

	sizeb=sizeof(unsigned long)*(hdr.Number_Of_Memory_Pages+1);

	fpto=(unsigned long *)MALLOC(sizeb);

	hdr.Fixup_Page_Table_Offset=headsize;

	memset(fpto,0,sizeb);

//	for(i=0;i<(hdr.Number_Of_Memory_Pages+1);i++){

//		fpto[i]=0;

//	}

	if(fwrite(fpto,sizeb,1,hout)!=1)goto errwrite;

	headsize+=sizeb;

//Fixup_Record_Table

int sizefixpage=0;

	hdr.Fixup_Record_Table_Offset=headsize;

	for(i=0;i<hdr.Number_Of_Memory_Pages;i++){

		unsigned int startblc=i*4096;	//адрес первого блока

		for(unsigned int j=0;j<posts;j++){	//обходим всю таблицу post

			if(((postbuf+j)->type==CALL_32I||

	    ((postbuf+j)->type>=POST_VAR32&&(postbuf+j)->type<=FIX_CODE32))&&

			(postbuf+j)->loc>=startblc&&(postbuf+j)->loc<(startblc+4096)){

				int sizerec;

				string2[0]=7;

				*(unsigned short *)&string2[2]=(unsigned short)((postbuf+j)->loc-startblc);

				string2[4]=1;

				unsigned int val=*(unsigned long *)&output[(postbuf+j)->loc];

				if(val<65536){

					string2[1]=0;

					*(unsigned short *)&string2[5]=(unsigned short)val;

					sizerec=7;

				}

				else{

					string2[1]=0x10;

					*(unsigned long *)&string2[5]=val;

					sizerec=9;

				}

				sizefixpage+=sizerec;

				if(fwrite(string2,sizerec,1,hout)!=1)goto errwrite;

			}

			fpto[i+1]=sizefixpage;

		}

	}

	headsize+=sizefixpage;

//Imported_Procedure_Name_Table

	hdr.Imported_Module_Names_Table_Offset=

			hdr.Imported_Procedure_Name_Table_Offset=headsize;

	string2[0]=0;

	if(fwrite(string2,1,1,hout)!=1)goto errwrite;

	headsize++;

	hdr.Fixup_Section_Size=headsize-hdr.Fixup_Page_Table_Offset;//9+sizefixpage;

	hdr.Loader_Section_Size=headsize-sizeof(LE_Header);

	hdr.Automatic_Data_Object=2;

	hdr.Data_Pages_Offset=Align(headsize+headerofs,16/*1024*/);

	ChSize(hdr.Data_Pages_Offset);

	fseek(hout,headerofs,SEEK_SET);

	if(fwrite(&hdr,sizeof(LE_Header),1,hout)!=1)goto errwrite;

	fseek(hout,headerofs+hdr.Fixup_Page_Table_Offset,SEEK_SET);

	if(fwrite(fpto,sizeof(unsigned long)*(hdr.Number_Of_Memory_Pages+1),1,hout)!=1)goto errwrite;

	free(fpto);

	fseek(hout,0,SEEK_END);

	return 0;

}
