
#ifndef __DEMONEYE__RESMGR_H
#define __DEMONEYE__RESMGR_H 1

#include "logger.hpp"



namespace demoneye
{
	class iResourceLocation
	{
	public:
		iResourceLocation() = delete;
		iResourceLocation(const iResourceLocation&) = delete;
		~iResourceLocation();

		/* Open the resource location if it isn't already open. */
		virtual int Open() =0;
		/* Destroy the resource location (must be called by each resource that
		.  uses it when that resource is done using the object). */
		virtual int Destroy() noexcept =0;
		/* Destroy the resource location (must be called by Destroy()). */
		virtual int Close() =0;

		/* State function for when the resource location is open. */
		virtual bool IsOpen() const noexcept =0;
		/* State function for when the resource location is initialized. */
		virtual bool IsInitialized() const noexcept =0;
	};

	class iResource
	{
		iResourceLocation *loc;
	public:

		iResource() = delete;
		iResource(const iResource&) = delete;
		~iResource();

		/* Resource destruction procedure, return 0 on success, nonzero on failure. */
		virtual int  Destroy() noexcept =0;
		/* Open the resource to get it ready for loading (this will be done
		.  automatically if it not already done once a load of the resource data
		.  is requested). */
		virtual int  Open() =0;
		/* Open the resource to get it ready for loading (this will be done
		.  automatically if it not already done once a load of the resource data
		.  is requested). */
		virtual memref<char> LoadBytes() =0;
		/* State function for when the resource is still initialized. */
		virtual bool IsInitialized() const noexcept =0;
		/* State function for when the resource is still open. */
		virtual bool IsOpen() const noexcept =0;
		/* State function for when the resource is loaded. */
		virtual bool IsLoaded() const noexcept =0;

		template <typename _LoadTyp>
		memref<_LoadTyp> Load()
		{
			return LoadBytes().ReinterpretType<_LoadTyp>();
		}

		/* Get the resource location. */
		iResourceLocation* Location() const noexcept;
	};

	class ResourceEntry
	{
		iResource *resource;
	public:

		ResourceEntry(iResource *res);
		~ResourceEntry();
	};

	class ResourceTable
	{
		memref<ResourceEntry> table;
	
	public:
		ResourceTable();
		ResourceTable(const ResourceTable&);
		~ResourceTable();
	};
}


/* Standard Resource Location Types */
#ifndef __DEMONEYE__resmgr_RESTYPES_H
#	include "resmgr/reslocs.hpp"
#endif

/* Standard Resource Types */
#ifndef __DEMONEYE__resmgr_RESTYPES_H
#	include "resmgr/restypes.hpp"
#endif



#endif // __DEMONEYE__RESMGR_H
