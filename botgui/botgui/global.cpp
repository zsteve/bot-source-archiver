#include "global.h"

void loadIncExcList(list<crcAbleFile>& inc, list<crcAbleFile>& exc)
{
	// attempts to load from workingdir
	string wDir=workingdir;
	file incFile, excFile;
	fileList incList, excList;
	incFile.setpath(wDir+"\\inclist.txt");
	excFile.setpath(wDir+"\\exclist.txt");
	crcAbleFile tempFile;
	if(incFile.exists())
	{
		try{
			incList.load((char*)incFile.load());
		}catch(FormatInvalidException e){
			MessageBox(NULL, "Error : inclist.txt is invalid. Bot will now delete it", "Error", MB_OK);
			incFile.kill();
			incFile.create();
			incFile.write((uchar*)STANDARD_FILELIST_HEADER, strlen(STANDARD_FILELIST_HEADER));
			incList.load((char*)incFile.load());
		}
		for(list<fileList::entry>::iterator it=incList.getList().begin();
			it!=incList.getList().end(); ++it)
		{
			tempFile.setpath(it->getpath());
			inc.push_back(tempFile);
		}
	}else{
		incFile.create();
		incFile.write((uchar*)STANDARD_FILELIST_HEADER, strlen(STANDARD_FILELIST_HEADER));
		incList.load((char*)incFile.load());
		for(list<fileList::entry>::iterator it=incList.getList().begin();
			it!=incList.getList().end(); ++it)
		{
			tempFile.setpath(it->getpath());
			inc.push_back(tempFile);
		}
	}
	if(excFile.exists())
	{
		try{
			excList.load((char*)excFile.load());
		}catch(FormatInvalidException e){
			MessageBox(NULL, "Error : exclist.txt is invalid. Bot will now delete it", "Error", MB_OK);
			excFile.kill();
			excFile.create();
			excFile.write((uchar*)STANDARD_FILELIST_HEADER, strlen(STANDARD_FILELIST_HEADER));
			excList.load((char*)excFile.load());
		}
		for(list<fileList::entry>::iterator it=excList.getList().begin();
			it!=excList.getList().end(); ++it)
		{
			tempFile.setpath(it->getpath());
			exc.push_back(tempFile);
		}
	}else{
		excFile.create();
		excFile.write((uchar*)STANDARD_FILELIST_HEADER, strlen(STANDARD_FILELIST_HEADER));
		excList.load((char*)excFile.load());
		for(list<fileList::entry>::iterator it=excList.getList().begin();
			it!=excList.getList().end(); ++it)
		{
			tempFile.setpath(it->getpath());
			exc.push_back(tempFile);
		}
	}
}

void saveIncExcList(list<crcAbleFile>& inc, list<crcAbleFile>& exc)
{
	fileList incList, excList;
	file incFile, excFile;
	string wDir=workingdir;
	incFile.setpath(wDir+"\\inclist.txt");
	excFile.setpath(wDir+"\\exclist.txt");
	incList.getList().clear();
	excList.getList().clear();
	fileList::entry tempEntry;
	for(list<crcAbleFile>::iterator it=inc.begin();
		it!=inc.end(); ++it)
	{
		tempEntry.setpath(it->getpath());
		incList.getList().push_back(tempEntry);
	}
	if(!incFile.exists())
	{
		incFile.create();	
	}
	incFile.write((uchar*)incList.compile().c_str(), incList.compile().size());
	for(list<crcAbleFile>::iterator it=exc.begin();
		it!=exc.end(); ++it)
	{
		tempEntry.setpath(it->getpath());
		excList.getList().push_back(tempEntry);
	}
	excFile.write((uchar*)excList.compile().c_str(), excList.compile().size());
}

void loadLists(fileList& fl, crcList& cl)
{
	// loads lists, and has error checking
	string wDir=workingdir;
	crclist.setpath(wDir+"\\crclist.txt");
	filelist.setpath(wDir+"\\filelist.txt");
	if(filelist.exists())
	{
		try{
			fl.load((char*)filelist.load());
		}catch(FormatInvalidException e){
			MessageBox(NULL,"Error : filelist.txt is invalid. Bot will now delete it", "Error", MB_OK);
			filelist.kill();
			filelist.create();
			filelist.write((uchar*)STANDARD_FILELIST_HEADER, strlen(STANDARD_FILELIST_HEADER));
			fl.load((char*)filelist.load());
		}
	}else{
		filelist.create();
		filelist.write((uchar*)STANDARD_FILELIST_HEADER, strlen(STANDARD_FILELIST_HEADER));
		fl.load((char*)filelist.load());
	}
	if(crclist.exists())
	{
		try{
			cl.load((char*)crclist.load());
		}catch(FormatInvalidException e){
			MessageBox(NULL,"Error : crclist.txt is invalid. Bot will now delete it", "Error", MB_OK);
			crclist.kill();
			crclist.create();
			crclist.write((uchar*)STANDARD_CRCLIST_HEADER, strlen(STANDARD_CRCLIST_HEADER));
			cl.load((char*)crclist.load());
		}
	}else{
		crclist.create();
		crclist.write((uchar*)STANDARD_CRCLIST_HEADER, strlen(STANDARD_CRCLIST_HEADER));
		cl.load((char*)crclist.load());
	}
}

void saveLists(fileList& fl, crcList& cl)
{
	try{
		filelist.write((uchar*)fl.compile().c_str(), fl.compile().length());
	}catch(std::exception e){
		filelist.write((uchar*)STANDARD_FILELIST_HEADER, strlen(STANDARD_FILELIST_HEADER));
	}
	try{
		crclist.write((uchar*)cl.compile().c_str(), cl.compile().length());
	}catch(std::exception e){
		crclist.write((uchar*)STANDARD_CRCLIST_HEADER, strlen(STANDARD_CRCLIST_HEADER));
	}
	return;
}