
#ifndef __DEMONEYE__resmgr_RESLOCS_H
#define __DEMONEYE__resmgr_RESLOCS_H 1

#ifndef __DEMONEYE__RESMGR_H
#	include "../resmgr.hpp"
#endif



namespace demoneye
{
	typedef class FileResourceLocation: public iResourceLocation
	{
		FileHandle file;

	public:
		FileResourceLocation();
		FileResourceLocation(const FileResourceLocation&);

		FileResourceLocation(const FileHandle&);

		FileResourceLocation(const char *pathname);

		~FileResourceLocation();
	} FileResLocation, FileResLoc;

	typedef class ZippedResourceLocation: public FileResourceLocation
	{
	public:
		enum Format
		{
			NULL_FORMAT = 0,
			
			A,
			RAR,
			ZIP,
			TAR,
			TARGZ,
			TARXZ,
			TARBZ2,
		};

	private:
		Format format;

	public:

		ZippedResourceLocation();
		ZippedResourceLocation(const ZippedResourceLocation&);

		/* Construct resource location to some file or directory (`ar_query`)
		.  within a given archive file (`ar_path`) of a given format (`ar_fmt`). */
		ZippedResourceLocation(Format ar_fmt, const char *ar_path, const char *ar_query);

		~ZippedResourceLocation();
	} ZippedResLocation, ZippedResLoc;
}



#endif // __DEMONEYE__resmgr_RESLOCS_H
