
#ifndef __DEMONEYE__resmgr_RESTYPES_H
#define __DEMONEYE__resmgr_RESTYPES_H 1

#ifndef __DEMONEYE__RESMGR_H
#	include "../resmgr.hpp"
#endif



namespace demoneye
{
	class ImageResource: public iResource
	{
	public:
		enum Format
		{
			NULL_FORMAT = 0,

			PNG,
			JPEG,
			BMP,
		};

	private:
		Format format;

	public:

		ImageResource(Format fmt, FileResourceLocation resloc);

		virtual int  Destroy() noexcept override;
		virtual bool IsInitialized() const noexcept override;
		virtual bool IsLoaded() const noexcept override;
	};

	class VideoResource: public iResource
	{
	public:
		enum Format
		{
			NULL_FORMAT = 0,

			MP4,
		};

	private:
		Format format;

	public:

		VideoResource(Format fmt, FileResourceLocation resloc);

		virtual int  Destroy() noexcept override;
		virtual bool IsInitialized() const noexcept override;
		virtual bool IsLoaded() const noexcept override;
	};

	class AudioResource: public iResource
	{
	public:
		enum Format
		{
			NULL_FORMAT = 0,

			MP3,
			WAV,
			MIDI,
		};

	private:
		Format format;

	public:

		AudioResource(Format fmt, FileResourceLocation resloc);

		virtual int  Destroy() noexcept override;
		virtual bool IsInitialized() const noexcept override;
		virtual bool IsLoaded() const noexcept override;
	};
}



#endif // __DEMONEYE__resmgr_RESTYPES_H
