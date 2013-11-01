#include "fileprocessor.h"
#include "status.h"

extern status botStatus;

void expandFiles(list<crcAbleFile>& expList)
{
	/** expandFiles() - helper function for processFiles() 
	 * expands all directories into the list
	 */
	int fAttributes;
	list<crcAbleFile> outList;
	crcAbleFile convFile;
	for(list<crcAbleFile>::iterator it=expList.begin();
		it!=expList.end(); ++it)
	{
		fAttributes=GetFileAttributes(it->getpath().c_str());
		if(fAttributes==FILE_ATTRIBUTE_DIRECTORY)
		{
			directory dir;
			dir.setpath(it->getpath());
			dir.listcontents();
			for(list<file>::iterator it2=dir.contents.begin();
				it2!=dir.contents.end(); ++it2)
			{
				// insert elements 
				convFile.setpath(it2->getpath());
				outList.push_back(convFile);
			}
			if(it==expList.end())
				break;
		}else{
			outList.push_back(*it);
		}
	}
	expList=outList;
}

string fileProcessor::processFiles(list<crcAbleFile> include, list<crcAbleFile> exclude)
{
	/** processFiles() - processes the list of include files and exclude files
	 *  and creates an instance of 7zip with the parameters.
	 * @param : include, exclude
	 * @throw : none
	 * @return : the path to the file outputted by the compression program
	 **/
	// directive:
	// remove all excluded files from include files
	// expand all include files
	// update file list
	// crc:
	// if file is in crc list
	//	if crc is up to date : add file to exclude files
	//	if crc is not up to date : update crc for file
	// if file is not in crc list
	//	add file to crc list

	if(include.empty() && exclude.empty())
		return "";

	string outFilePath="";
	fileList fList;
	crcList cList;

	loadLists(fList, cList);

	list<crcAbleFile> includeNew;
	includeNew.clear();
	includeNew=include;
	// now we need to expand include files
	botStatus.setCurrentStatus("Expanding Files...");
	expandFiles(includeNew);
	if(gSetting.option.incremental)
	{
		// and now we are going to update file list and CRC list
		for(list<crcAbleFile>::iterator it=includeNew.begin();
			it!=includeNew.end(); ++it)
		{
			botStatus.setCurrentStatus("Calculating CRC of "+it->getpath());
			if(!fList.checkfile(*it))
			{
				// if file is nonexistent in file list
				// we add it
				fList.addfile(*it);
			}
			if(!it->exists())
			{
				// if file does not exist, we must remove it from the list
				fList.removefile(*it);
				continue;
			}
			if(!cList.checkfile(*it))
			{
				// if file is not present in CRC list
				// we add it
				cList.addfile(*it);
			}else{
				// if file is in CRC list
				try{
					if(cList.getcrc(*it)==it->getCRC())
					{
						// if CRCs match, then we need to add this one to exclude files
						exclude.push_back(*it);
					}else{
						// if CRCs don't match, we update the CRC
						cList.updatefile(*it);
					}
				}catch(FileEmptyException e)
				{
					
				}
			}
		}
	}
	saveLists(fList, cList);
	botStatus.setCurrentStatus("Setting up command line parameters");
	string outStr("");
	for(list<crcAbleFile>::iterator iit=includeNew.begin();
		iit!=includeNew.end(); ++iit)
	{
		outStr+=iit->getpath();
		outStr+="\n";
	}
	string incl("");
	string excl("");
	string param("");
	// first we process exclude files
	for(list<crcAbleFile>::iterator it=exclude.begin();
		it!=exclude.end(); ++it)
	{
		excl+=" -r -x\"";
		excl+=it->getpath();
		excl+="\"";
		excl+=" ";
	}
	// and then include files
	for(list<crcAbleFile>::iterator it=include.begin();
		it!=include.end(); ++it)
	{
		incl+=" -r \"";
		incl+=it->getpath();
		incl+="\"";
	}
	// now we can create the command line for the compressor
	param=" a ";
	param+=gSetting.getFilename();
	param+=" -ep1 ";
	param+=excl;
	param+=incl;
	botStatus.setCurrentStatus("Running compression program");
	ShellExecute(NULL, "open", "rar.exe", (char*)param.c_str(), workingdir, SW_SHOW);
	outFilePath=gSetting.getFilename();
	gSetting.incrementVersion();
	{
		string outStr=gSetting.serialize();
		{
			file outFile;
			string iniPath=workingdir;
			iniPath+="\\bot.ini";
			outFile.setpath(iniPath);
			outFile.write((char*)outStr.c_str());
		}
	}
	return outFilePath;
}