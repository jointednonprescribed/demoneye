
#ifndef __DEMONEYE__ENV_H
#define __DEMONEYE__ENV_H 1

#include "gl.hpp"
#include "periphio.hpp"



namespace demoneye
{
	class EngineContext: public iTerminatable
	{
		GLFWContext               glfw;
		LoggerFMThread            logger;
		ResourceTable             restable;
		arraylist<iTerminatable&> termlist;
	public:
		static constexpr size_t DEFAULT_TERMLIST_SIZE = 20;

		enum subsystem_selection
		{
			MEMORY_SUBSYS  = 01, //  000 001
			THREAD_SUBSYS  = 03, //  000 011
			IO_SUBSYS      = 07, //  000 111
			PERIPH_SUBSYS  = 017, // 001 111
			VIDEO_SUBSYS   = 037, // 011 111
			AUDIO_SUBSYS   = 057, // 101 111

			ALL_SUBSYS     = 077, // 111 111

			_SUBSYS_BIT_MEMORY = 01,
			_SUBSYS_BIT_THREAD = 02,
			_SUBSYS_BIT_IO     = 04,
			_SUBSYS_BIT_PERIPH = 010,
			_SUBSYS_BIT_VIDEO  = 020,
			_SUBSYS_BIT_AUDIO  = 030
		};

		EngineContext();
		EngineContext(int subsystems);
		~EngineContext();

		virtual int InitMemorySubsystem();
		virtual int InitThreadSubsystem();
		virtual int InitIOSubsystem();
		virtual int InitPeripheralSubsystem();
		virtual int InitVideoSubsystem();
		virtual int InitAudioSubsystem();
		virtual int InitAllSubsystems();
		virtual int InitSubsystems(int subsystems);

		virtual int TerminateMemorySubsystem();
		virtual int TerminateThreadSubsystem();
		virtual int TerminateIOSubsystem();
		virtual int TerminatePeripheralSubsystem();
		virtual int TerminateVideoSubsystem();
		virtual int TerminateAudioSubsystem();
		virtual int TerminateAllSubsystems();
		virtual int TerminateSubsystems(int subsystems);

		virtual bool MemorySubsystemIsActive();
		virtual bool ThreadSubsystemIsActive();
		virtual bool IOSubsystemIsActive();
		virtual bool PeripheralSubsystemIsActive();
		virtual bool VideoSubsystemIsActive();
		virtual bool AudioSubsystemIsActive();
		virtual bool AllSubsystemsAreActive();
		virtual bool SubsystemsAreActive(int subsystems);

		virtual int Terminate() override;
	};
}



#endif // __DEMONEYE__ENV_H
